#include "o2Editor/stdafx.h"
#include "AssetsWindow.h"

#include "o2/Application/Application.h"
#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Assets/Assets.h"
#include "o2/Assets/AssetsTree.h"
#include "o2/Assets/Types/DataAsset.h"
#include "o2/Assets/Types/FolderAsset.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/GridLayout.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Utils/Function/Function.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/Utils/System/Clipboard.h"
#include "o2Editor/AssetsWindow/AssetIcon.h"
#include "o2Editor/AssetsWindow/AssetsIconsScroll.h"
#include "o2Editor/AssetsWindow/FoldersTree.h"
#include "o2Editor/Core/EditorConfig.h"

DECLARE_SINGLETON(Editor::AssetsWindow);

namespace Editor
{
	AssetsWindow::AssetsWindow()
	{
		if (Application::IsReady())
			InitializeWindow();
	}

	AssetsWindow::~AssetsWindow()
	{}

	void AssetsWindow::InitializeWindow()
	{
		o2Assets.onAssetsRebuilt += THIS_FUNC(OnAssetsRebuilt);

		mWindow->caption = "Assets";
		mWindow->name = "assets window";
		mWindow->SetIcon(mmake<Ref<Sprite>>("ui/UI4_folder_icon.png"));
		mWindow->SetIconLayout(make_ref<WidgetLayout>(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 1))));
		mWindow->SetViewLayout(make_ref<WidgetLayout>(Layout::BothStretch(-2, 0, 0, 18)));
		mWindow->SetClippingLayout(make_ref<WidgetLayout>(Layout::BothStretch(-1, 0, 0, 18)));

		InitializeUpPanel();
		InitializeDownPanel();
		InitializeFoldersTree();
		InitializeFoldersTreeVisibleState();
		InitializeFoldersTreeSeparator();

		OpenFolder("");
	}

	void AssetsWindow::InitializeFoldersTreeSeparator()
	{
		auto separatorLayer = mFoldersTree->FindLayer("separator");

		mSeparatorHandle.isUnderPoint = [=](const Vec2F& point) {
			RectF rt = separatorLayer->GetDrawable()->GetRect();
			rt.left -= 2; rt.right += 2;
			return rt.IsInside(point);
		};

		mSeparatorCoef = 0.0f; // o2EditorConfig.GetProjectUserData()["layout/assetsWindow/separator_coef"];

		if (mSeparatorCoef < FLT_EPSILON)
			mSeparatorCoef = 0.2f;

		mFoldersTree->layout->anchorRight = mSeparatorCoef;
		mAssetsGridScroll->layout->anchorLeft = mSeparatorCoef;

		mSeparatorHandle.onMoved = [&](const Input::Cursor& cursor) {
			float anchorDelta = cursor.delta.x / mWindow->layout->width;
			mFoldersTree->layout->anchorRight += anchorDelta;
			mAssetsGridScroll->layout->anchorLeft += anchorDelta;

// 			auto& userData = o2EditorConfig.GetProjectUserData();
// 			userData["layout/assetsWindow/separator_coef"].Set(mFoldersTree->layout->GetAnchorRight());
		};

		mAssetsGridScroll->onDraw += [&]() { mSeparatorHandle.OnDrawn(); };

		mSeparatorHandle.cursorType = CursorType::SizeWE;
	}

	void AssetsWindow::InitializeFoldersTreeVisibleState()
	{
		mFoldersTreeShowCoef = 1.0f;
		mFoldersTreeVisible = true;

		mFoldersTreeShowAnim->SetTarget(this);
		mFoldersTreeShowAnim->SetClip(AnimationClip::EaseInOut("mFoldersTreeShowCoef", 0.0f, 1.0f, 0.4f));
		mFoldersTreeShowAnim->GoToEnd();

		mFoldersTreeShowAnim->onUpdate = [&](float dt) {
			mFoldersTree->layout->anchorRight = mSeparatorCoef*mFoldersTreeShowCoef;
			mAssetsGridScroll->layout->anchorLeft = mSeparatorCoef*mFoldersTreeShowCoef;

			float disableCoef = 0.2f;
			if (mFoldersTreeShowCoef < disableCoef && mFoldersTree->IsEnabled())
				mFoldersTree->Hide();

			if (mFoldersTreeShowCoef >= disableCoef && !mFoldersTree->IsEnabled())
				mFoldersTree->Show();
		};
	}

	void AssetsWindow::InitializeFoldersTree()
	{
		mFoldersTree = mmake<Ref<AssetsFoldersTree>>();
		*mFoldersTree->layout = make_ref<WidgetLayout>(0.0f, 1.0f, 0.5f, 0.0f, 5.0f, -18.0f, 0.0f, 18.0f);

		mFoldersTree->AddLayer("separator", mmake<Ref<Sprite>>("ui/UI4_Ver_separator.png"),
							   make_ref<WidgetLayout>(Layout::VerStretch(HorAlign::Right, -2, 0, 5, 0)));
		
		mFoldersTree->UpdateView();
		mWindow->AddChild(mFoldersTree);

		// assets scroll & grid
		mAssetsGridScroll = o2UI.CreateWidget<AssetsIconsScrollArea>();
		*mAssetsGridScroll->layout = make_ref<WidgetLayout>(0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -18.0f, 0.0f, 18.0f);
		mWindow->AddChild(mAssetsGridScroll);

		mAssetsGridScroll->onAssetsSelected = [&](const Vector<String>& assetsPaths) {
			if (!assetsPaths.IsEmpty())
				mSelectedAssetPathLabel->text = assetsPaths.Last();
			else
				mSelectedAssetPathLabel->text = mAssetsGridScroll->GetViewingPath();
		};
	}

	void AssetsWindow::InitializeDownPanel()
	{
		Ref<Widget> downPanel = mmake<Ref<Widget>>();
		downPanel->AddLayer("back", mmake<Ref<Sprite>>("ui/UI4_small_panel_down_back.png"),
							make_ref<WidgetLayout>(Layout::BothStretch(-4, -5, -4, -5)));
		*downPanel->layout = make_ref<WidgetLayout>(WidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 20, 0));

		auto showTreeBtn = o2UI.CreateWidget<Ref<Button>>("menu tree");
		*showTreeBtn->layout = make_ref<WidgetLayout>(WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(0, 0)));
		showTreeBtn->onClick += THIS_FUNC(O#include <memory>

using namespace std;

// Helper class for creating Ref<T> objects
template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return make_shared<T>(forward<Args>(args)...);
}

class WidgetLayout
{
public:
    WidgetLayout() {}
};

class Widget
{
public:
    Widget() {}
    virtual ~Widget() {}

    Ref<WidgetLayout> layout;

    void AddLayer(const string& name, const Ref<Sprite>& sprite, const WidgetLayout& layout)
    {
        // implementation
    }

    void AddChild(const Ref<Widget>& child)
    {
        // implementation
    }
};

class Button
{
public:
    Button() {}
    virtual ~Button() {}

    Ref<WidgetLayout> layout;
    function<void()> onClick;

    // other functions and members
};

class Label
{
public:
    Label() {}
    virtual ~Label() {}

    Ref<WidgetLayout> layout;
    string text;
    enum HorOverflow { Dots };
    enum HorAlign { Left };

    HorOverflow horOverflow;
    HorAlign horAlign;

    // other functions and members
};

class AssetsWindow
{
public:
    AssetsWindow()
    {
        mWindow = mmake<Widget>();
    }

    void InitializeDownPanel()
    {
        Ref<Widget> downPanel = mmake<Widget>();
        downPanel->layout = mmake<WidgetLayout>();

        mAssetsGridScroll = mmake<GridScroll>();
        downPanel->AddChild(mAssetsGridScroll);

        mSelectedAssetPathLabel = mmake<Label>();
        mSelectedAssetPathLabel->layout = mmake<WidgetLayout>();
        mSelectedAssetPathLabel->text = "o2/Assets/Folder/Image.png";
        mSelectedAssetPathLabel->horOverflow = Label::HorOverflow::Dots;
        mSelectedAssetPathLabel->horAlign = Label::HorAlign::Left;
        downPanel->AddChild(mSelectedAssetPathLabel);

        mWindow->AddChild(downPanel);
    }

    void InitializeUpPanel()
    {
        Ref<Widget> upPanel = mmake<Widget>();
        upPanel->layout = mmake<WidgetLayout>();

        Button* searchButton = mmake<Button>();
        searchButton->layout = mmake<WidgetLayout>();
        upPanel->AddChild(searchButton);

        mFilterButton = mmake<Button>();
        mFilterButton->layout = mmake<WidgetLayout>();
        mFilterButton->onClick += THIS_FUNC(OnMenuFilterPressed);
        upPanel->AddChild(mFilterButton);

        mSearchEditBox = mmake<EditBox>();
        mSearchEditBox->layout = mmake<WidgetLayout>();
        mSearchEditBox->onChanged += THIS_FUNC(OnSearchEdited);
        upPanel->AddChild(mSearchEditBox);

        mWindow->AddChild(upPanel);
    }

    void Update(float dt)
    {
        IEditorWindow::Update(dt);
        mFoldersTreeShowAnim->Update(dt);
    }

    void SelectAsset(const UID& id)
    {
        String assetPath = o2Assets.GetAssetPath(id);
        String folder = o2FileSystem.GetParentPath(assetPath);

        if (GetOpenedFolderPath() != folder)
            OpenFolder(folder);

        mAssetsGridScroll->SelectAsset(id);
    }

    void SelectAsset(const String& path)
    {
        const UID& id = o2Assets.GetAssetId(path);
        String folder = o2FileSystem.GetParentPath(path);

        if (GetOpenedFolderPath() != folder)
            OpenFolder(folder);

        mAssetsGridScroll->SelectAsset(id);
    }

    void SelectAsset(const Vector<UID>& ids)
    {
        for (auto id : ids)
            SelectAsset(id);
    }

    void SelectAssets(const Vector<String>& paths)
    {
        for (auto path : paths)
            SelectAsset(path);
    }

    void OpenAsset(const UID& id)
    {
        OpenAsset(o2Assets.GetAssetPath(id));
    }

    void OpenAsset(const String& path)
    {
        String fullPath = o2FileSystem.GetParentPath(o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + path);
#if defined PLATFORM_WINDOWS
        ShellExecute(NULL, "explore", fullPath, NULL, NULL, SW_SHOWNORMAL);
#endif
    }

    void OpenAndEditAsset(const UID& id)
    {
        OpenAndEditAsset(o2Assets.GetAssetPath(id));
    }

    void OpenAndEditAsset(const String& path)
    {
        String fullPath = o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + path;
#if defined PLATFORM_WINDOWS
        ShellExecute(NULL, "edit", fullPath, NULL, NULL, SW_SHOWNORMAL);
#endif
    }

    void DeselectAssets()
    {
        mAssetsGridScroll->DeselectAllAssets();
    }

    const Vector<Ref<AssetInfo>>& GetSelectedAssets() const
    {
        return mAssetsGridScroll->GetSelectedAssets();
    }

    String GetOpenedFolderPath() const
    {
        return mAssetsGridScroll->GetViewingPath();
    }

    void OpenFolder(const String& path)
    {
        mAssetsGridScroll->SetViewingPath(path);
        mFoldersTree->SelectAndExpandFolder(path);
        mSelectedAssetPathLabel->text = path;
    }

    void ShowAssetIcon(const UID& id)
    {
        String assetPath = o2Assets.GetAssetPath(id);
        String folder = o2FileSystem.GetParentPath(assetPath);

        ShowAssetIcon(folder, id);
    }

    void ShowAssetIcon(const String& path)
    {
        UID assetId = o2Assets.GetAssetId(path);
        String folder = o2FileSystem.GetParentPath(path);

        ShowAssetIcon(folder, assetId);
    }

    void ShowAssetIcon(const String& folder, UID assetId)
    {
        if (GetOpenedFolderPath() != folder)
            OpenFolder(folder);

        mAssetsGridScroll->HighlightAsset(assetId);
    }

    void CopyAssets(const Vector<String>& assetsPaths)
    {
        mCuttingAssets.Clear();
        mAssetsGridScroll->UpdateCuttingAssets();

        Vector<WString> paths;
        for (auto& path : assetsPaths)
            paths.Add(o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + path);

        Clipboard::CopyFiles(paths);
    }

    void CopyAssetFolder(const String& src, const String& dst)
    {
        o2FileSystem.FolderCreate(dst);

        FolderInfo info = o2FileSystem.GetFolderInfo(src);
        info.ClampPathNames();

        for (auto& file : info.files)
        {
            if (!file.path.EndsWith(".meta"))
                o2FileSystem.FileCopy(src + "/" + file.path, dst + "/" + file.path);
        }

        for (auto& folder : info.folders)
            CopyAssetFolder(src + "/" + folder.path, dst + "/" + folder.path);
    }

    void CutAssets(const Vector<String>& assetsPaths)
    {
        mCuttingAssets.Clear();

        Vector<WString> paths;
        for (aut#include <memory>

class RefBase
{
public:
    RefBase() : mRefCounter(1)
    {}

protected:
    virtual ~RefBase()
    {}

public:
    void AddRef()
    {
        ++mRefCounter;
    }

    void Release()
    {
        if (--mRefCounter == 0)
        {
            delete this;
        }
    }

private:
    int mRefCounter;
};

template<typename T>
class Ref
{
public:
    Ref() : mPtr(nullptr)
    {}

    Ref(T* ptr) : mPtr(ptr)
    {}

    Ref(const Ref<T>& other)
    {
        mPtr = other.mPtr;
        if (mPtr)
            mPtr->AddRef();
    }

    template<typename Y>
    Ref(const Ref<Y>& other)
    {
        mPtr = dynamic_cast<T*>(other.Get());
        if (mPtr)
            mPtr->AddRef();
    }

    ~Ref()
    {
        if (mPtr)
            mPtr->Release();
    }

    void Reset(T* ptr = nullptr)
    {
        if (ptr != mPtr)
        {
            if (mPtr)
                mPtr->Release();
            mPtr = ptr;
            if (mPtr)
                mPtr->AddRef();
        }
    }

    T* Get() const
    {
        return mPtr;
    }

    T& operator* () const
    {
        return *mPtr;
    }

    T* operator-> () const
    {
        return mPtr;
    }

    bool operator== (const Ref<T>& other) const
    {
        return mPtr == other.mPtr;
    }

    bool operator!= (const Ref<T>& other) const
    {
        return mPtr != other.mPtr;
    }

    template<typename Y>
    Ref<Y> Cast()
    {
        return Ref<Y>(dynamic_cast<Y*>(mPtr));
    }

    template<typename Y>
    friend class Ref;

private:
    T* mPtr;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
class WeakRef
{
public:
    WeakRef() : mPtr(nullptr)
    {}

    WeakRef(const Ref<T>& ref) : mPtr(ref.Get())
    {}

    WeakRef(const WeakRef<T>& other)
    {
        mPtr = other.mPtr;
    }

    ~WeakRef()
    {}

    void Reset()
    {
        mPtr = nullptr;
    }

    Ref<T> Lock() const
    {
        return Ref<T>(mPtr);
    }

    bool expired() const
    {
        return mPtr == nullptr;
    }

    WeakRef<T>& operator= (const WeakRef<T>& rhs)
    {
        if (this != &rhs)
            mPtr = rhs.mPtr;
        return *this;
    }

    WeakRef<T>& operator= (const Ref<T>& rhs)
    {
        mPtr = rhs.Get();
        return *this;
    }

    bool operator== (const WeakRef<T>& rhs) const
    {
        return mPtr == rhs.mPtr;
    }

    bool operator!= (const WeakRef<T>& rhs) const
    {
        return mPtr != rhs.mPtr;
    }

    friend class Ref<T>;

private:
    T* mPtr;
};

class RefBase {};

class Clipboard
{
public:
    static void CopyFiles(const Vector<Ref<String>>& files)
    {}

    static Vector<Ref<String>> GetCopyFiles()
    {
        return Vector<Ref<String>>();
    }
};

class Type
{
public:
    bool operator== (const Type& other) const
    {
        return true;
    }
};

template<typename T>
class DynamicCast
{
public:
    DynamicCast(const Ref<T>& obj) : mObj(obj)
    {}

    template<typename U>
    operator Ref<U>() const
    {
        return mObj.Cast<U>();
    }

private:
    Ref<T> mObj;
};

class String {};

class UID {};

class Asset {};

class ImageAsset : public Asset {};

class ActorAsset : public Asset {};

class FolderAsset : public Asset {};

class DataAsset : public Asset {};

class AnimationAsset : public Asset {};

class Sprite
{
public:
    Sprite(const Ref<String>& path)
    {}

    Sprite(const char* path)
    {}
};

class o2Application
{
public:
    static String GetBinPath()
    {
        return String();
    }
};

class o2Assets
{
public:
    static String GetAssetsPath()
    {
        return String();
    }

    static UID GetAssetId(const Ref<String>& path)
    {
        return UID();
    }

    static void RebuildAssets()
    {}
};

class o2FileSystem
{
public:
    static String GetPathWithoutDirectories(const Ref<String>& path)
    {
        return String();
    }

    static String GetFileExtension(const Ref<String>& fileName)
    {
        return String();
    }

    static String GetFileNameWithoutExtension(const Ref<String>& fileName)
    {
        return String();
    }

    static bool IsFileExist(const Ref<String>& path)
    {
        return false;
    }

    static void FileMove(const Ref<String>& src, const Ref<String>& dest)
    {}

    static void FileCopy(const Ref<String>& src, const Ref<String>& dest)
    {}

    static void RemoveAsset(const Ref<String>& path)
    {}
};

namespace Editor
{
    class AssetsWindow
    {
    public:
        void CutAssets(const Vector<Ref<String>>& assetsPaths)
        {
            for (const auto& path : assetsPaths)
            {
                Ref<String> fullPath = mmake<String>(o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + path);
                mCuttingAssets.Add(Pair<UID, Ref<String>>(o2Assets.GetAssetId(path), fullPath));
                paths.Add(fullPath);
            }

            Clipboard::CopyFiles(paths);

            mAssetsGridScroll->UpdateCuttingAssets();
        }

        void PasteAssets(const Ref<String>& targetPath)
        {
            Vector<Ref<String>> paths = Clipboard::GetCopyFiles();
            for (auto path : paths)
            {
                Ref<String> fileName = mmake<String>(o2FileSystem.GetPathWithoutDirectories(path));
                Ref<String> extension = mmake<String>(o2FileSystem.GetFileExtension(fileName));
                Ref<String> fileNameWithoutExt = mmake<String>(o2FileSystem.GetFileNameWithoutExtension(fileName));

                bool isFolder = extension->IsEmpty();

                Ref<String> copyFileName = mmake<String>(o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + targetPath + "/" + fileName);
                bool endsAsCopy = fileNameWithoutExt->EndsWith("copy");
                int i = 0;
                while (o2FileSystem.IsFileExist(copyFileName))
                {
                    copyFileName = mmake<String>(o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + targetPath + "/" +
                        fileNameWithoutExt);

                    if (!endsAsCopy)
                        *copyFileName += " copy";

                    if (i > 0)
                        *copyFileName += (String)(i + 1) + "." + extension;

                    if (!isFolder)
                        *copyFileName += "." + extension;

                    i++;
                }

                if (mCuttingAssets.Contains([&](auto x) { return x.second == path; }))
                {
                    o2FileSystem.FileMove(path, copyFileName);
                    o2FileSystem.FileMove(mmake<String>(path + ".meta"), mmake<String>(copyFileName + ".meta"));
                }
                else
                {
                    if (!isFolder)
                        o2FileSystem.FileCopy(path, copyFileName);
                    else
                        CopyAssetFolder(path, copyFileName);
                }
            }

            mCuttingAssets.Clear();
            mAssetsGridScroll->UpdateCuttingAssets();

            o2Assets.RebuildAssets();
        }

        void DeleteAssets(const Vector<Ref<String>>& assetsPaths)
        {
            mCuttingAssets.Clear();
            mAssetsGridScroll->UpdateCuttingAssets();

            for (const auto& path : assetsPaths)
                o2Assets.RemoveAsset(path);

            o2Assets.RebuildAssets();
        }

        Sprite* GetAssetIconSprite(const Ref<Asset>& asset)
        {
            const Type& type = asset->GetType();

            if (type == TypeOf(ImageAsset))
                return mmake<Sprite>(asset->GetPath());
            else if (type == TypeOf(ActorAsset))
                return mmake<Sprite>("ui/UI4_actor_icon.png");
            else if (type == TypeOf(FolderAsset))
                return mmake<Sprite>("ui/UI4_big_folder_icon.png");
            else if (type == TypeOf(DataAsset))
                return mmake<Sprite>("ui/UI4_big_text_file_icon.png");
            else if (type == TypeOf(AnimationAsset))
                return mmake<Sprite>("ui/UI4_anim_file_icon.png");

            return mmake<Sprite>("ui/UI4_big_file_icon.png");
        }

        void OnSearchEdited(const Ref<WString>& search)
        {}

        void OnMenuFilterPressed()
        {}

        void OnShowTreePressed()
        {
            mFoldersTreeVisible = !mFoldersTreeVisible;

            if (mFoldersTreeVisible)
                mFoldersTreeShowAnim->PlayForward();
            else
                mFoldersTreeShowAnim->PlayBack();
        }

        void OnAssetsRebuilt(const Vector<UID>& changedAssets)
        {
            mFoldersTree->UpdateView();
            mFoldersTree->SelectAndExpandFolder(mAssetsGridScroll->GetViewingPath());

            mAssetsGridScroll->UpdateAssetsByCurrentPath();
        }

    private:
        Vector<Pair<UID, Ref<String>>> mCuttingAssets;
        Vector<Ref<String>> paths;
        bool mFoldersTreeVisible;
        Ref<Object> mAssetsGridScroll;
        Ref<Object> mFoldersTree;
        Ref<Object> mFoldersTreeShowAnim;

        void CopyAssetFolder(const Ref<String>& sourcePath, const Ref<String>& destPath)
        {
            // Implementation of CopyAssetFolder method
        }
    };
}

// --- META ---

DECLARE_CLASS(Editor::AssetsWindow, Editor__AssetsWindow);

// --- END META ---