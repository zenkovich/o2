#include "o2Editor/stdafx.h"
#include "AssetsWindow.h"

#include "o2/Application/Application.h"
#include "o2/Assets/ActorAsset.h"
#include "o2/Assets/AnimationAsset.h"
#include "o2/Assets/Assets.h"
#include "o2/Assets/AssetsTree.h"
#include "o2/Assets/DataAsset.h"
#include "o2/Assets/FolderAsset.h"
#include "o2/Assets/ImageAsset.h"
#include "o2Editor/AssetsWindow/AssetsIconsScroll.h"
#include "o2Editor/AssetsWindow/FoldersTree.h"
#include "o2Editor/AssetsWindow/AssetIcon.h"
#include "o2Editor/Core/EditorConfig.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/GridLayout.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Utils/System/Clipboard.h"
#include "o2/Utils/Delegates.h"
#include "o2/Utils/FileSystem/FileSystem.h"

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
		o2Assets.onAssetsRebuilded += THIS_FUNC(OnAssetsRebuilded);

		mWindow->caption = "Assets";
		mWindow->name = "assets window";
		mWindow->SetIcon(mnew Sprite("ui/UI4_folder_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 1)));
		mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 18));

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

		mSeparatorCoef = o2EditorConfig.GetProjectUserData()["layout/assetsWindow/separator_coef"];

		if (mSeparatorCoef < FLT_EPSILON)
			mSeparatorCoef = 0.5f;

		mFoldersTree->layout->anchorRight = mSeparatorCoef;
		mAssetsGridScroll->layout->anchorLeft = mSeparatorCoef;

		mSeparatorHandle.onMoved = [&](const Input::Cursor& cursor) {
			float anchorDelta = cursor.delta.x / mWindow->layout->width;
			mFoldersTree->layout->anchorRight += anchorDelta;
			mAssetsGridScroll->layout->anchorLeft += anchorDelta;

			auto& userData = o2EditorConfig.GetProjectUserData();
			userData["layout/assetsWindow/separator_coef"].SetValue(mFoldersTree->layout->GetAnchorRight());
		};

		mAssetsGridScroll->onDraw += [&]() { mSeparatorHandle.OnDrawn(); };

		mSeparatorHandle.cursorType = CursorType::SizeWE;
	}

	void AssetsWindow::InitializeFoldersTreeVisibleState()
	{
		mFoldersTreeShowCoef = 1.0f;
		mFoldersTreeVisible = true;

		mFoldersTreeShowAnim.SetTarget(this);
		*mFoldersTreeShowAnim.AddAnimationValue<float>("mFoldersTreeShowCoef") =
			AnimatedValue<float>::EaseInOut(0, 1, 0.4f);

		mFoldersTreeShowAnim.onUpdate = [&](float dt) {
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
		mFoldersTree = mnew AssetsFoldersTree();
		*mFoldersTree->layout = WidgetLayout(0.0f, 1.0f, 0.5f, 0.0f, 0.0f, -18.0f, 0.0f, 18.0f);

		auto separatorLayer = mFoldersTree->AddLayer("separator", mnew Sprite("ui/UI4_Ver_separator.png"),
													 Layout::VerStretch(HorAlign::Right, -2, 0, 5, 0));
		mFoldersTree->UpdateView();
		mWindow->AddChild(mFoldersTree);

		// assets scroll & grid
		mAssetsGridScroll = o2UI.CreateWidget<AssetsIconsScrollArea>();
		*mAssetsGridScroll->layout = WidgetLayout(0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -18.0f, 0.0f, 18.0f);
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
		Widget* downPanel = mnew Widget();
		downPanel->AddLayer("back", mnew Sprite("ui/UI4_small_panel_down_back.png"),
							Layout::BothStretch(-4, -5, -4, -5));
		*downPanel->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 20, 0);

		auto showTreeBtn = o2UI.CreateWidget<Button>("menu tree");
		*showTreeBtn->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(0, 0));
		showTreeBtn->onClick += THIS_FUNC(OnShowTreePressed);
		downPanel->AddChild(showTreeBtn);

		mSelectedAssetPathLabel = o2UI.CreateWidget<Label>();
		*mSelectedAssetPathLabel->layout = WidgetLayout::BothStretch(20, 0, 0, 0);
		mSelectedAssetPathLabel->text = "o2/Assets/Folder/Image.png";
		mSelectedAssetPathLabel->horOverflow = Label::HorOverflow::Dots;
		mSelectedAssetPathLabel->horAlign = HorAlign::Left;
		downPanel->AddChild(mSelectedAssetPathLabel);

		mWindow->AddChild(downPanel);
	}

	void AssetsWindow::InitializeUpPanel()
	{
		Widget* upPanel = mnew Widget();
		upPanel->name = "up panel";
		*upPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		upPanel->AddLayer("back", mnew Sprite("ui/UI4_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		Button* searchButton = o2UI.CreateWidget<Button>("search");
		*searchButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(1, 1));
		upPanel->AddChild(searchButton);

		mFilterButton = o2UI.CreateWidget<Button>("menu filter");
		*mFilterButton->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 0));
		mFilterButton->onClick += THIS_FUNC(OnMenuFilterPressed);
		upPanel->AddChild(mFilterButton);

		mSearchEditBox = o2UI.CreateWidget<EditBox>("backless");
		*mSearchEditBox->layout = WidgetLayout::BothStretch(19, 2, 21, -2);
		mSearchEditBox->onChanged += THIS_FUNC(OnSearchEdited);
		upPanel->AddChild(mSearchEditBox);

		mWindow->AddChild(upPanel);
	}

	void AssetsWindow::Update(float dt)
	{
		IEditorWindow::Update(dt);
		mFoldersTreeShowAnim.Update(dt);
	}

	void AssetsWindow::SelectAsset(UID id)
	{
		String assetPath = o2Assets.GetAssetPath(id);
		String folder = o2FileSystem.GetParentPath(assetPath);

		if (GetOpenedFolderPath() != folder)
			OpenFolder(folder);

		mAssetsGridScroll->SelectAsset(id);
	}

	void AssetsWindow::SelectAsset(const String& path)
	{
		UID id = o2Assets.GetAssetId(path);
		String folder = o2FileSystem.GetParentPath(path);

		if (GetOpenedFolderPath() != folder)
			OpenFolder(folder);

		mAssetsGridScroll->SelectAsset(id);
	}

	void AssetsWindow::SelectAsset(const Vector<UID>& ids)
	{
		for (auto id : ids)
			SelectAsset(id);
	}

	void AssetsWindow::SelectAssets(const Vector<String>& paths)
	{
		for (auto path : paths)
			SelectAsset(path);
	}

	void AssetsWindow::OpenAsset(UID id)
	{
		OpenAsset(o2Assets.GetAssetPath(id));
	}

	void AssetsWindow::OpenAsset(const String& path)
	{
		String fullPath = o2FileSystem.GetParentPath(o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + path);
		ShellExecute(NULL, "explore", fullPath, NULL, NULL, SW_SHOWNORMAL);
	}

	void AssetsWindow::OpenAndEditAsset(UID id)
	{
		OpenAndEditAsset(o2Assets.GetAssetPath(id));
	}

	void AssetsWindow::OpenAndEditAsset(const String& path)
	{
		String fullPath = o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + path;
		ShellExecute(NULL, "edit", fullPath, NULL, NULL, SW_SHOWNORMAL);
	}

	void AssetsWindow::DeselectAssets()
	{
		mAssetsGridScroll->DeselectAllAssets();
	}

	Vector<AssetInfo> AssetsWindow::GetSelectedAssets() const
	{
		return mAssetsGridScroll->GetSelectedAssets();
	}

	String AssetsWindow::GetOpenedFolderPath() const
	{
		return mAssetsGridScroll->GetViewingPath();
	}

	void AssetsWindow::OpenFolder(const String& path)
	{
		mAssetsGridScroll->SetViewingPath(path);
		mFoldersTree->SelectAndExpandFolder(path);
		mSelectedAssetPathLabel->text = path;
	}

	void AssetsWindow::ShowAssetIcon(UID id)
	{
		String assetPath = o2Assets.GetAssetPath(id);
		String folder = o2FileSystem.GetParentPath(assetPath);

		if (GetOpenedFolderPath() != folder)
			OpenFolder(folder);

		mAssetsGridScroll->HightlightAsset(id);
	}

	void AssetsWindow::ShowAssetIcon(const String& path)
	{
		UID assetId = o2Assets.GetAssetId(path);
		String folder = o2FileSystem.GetParentPath(path);

		if (GetOpenedFolderPath() != folder)
			OpenFolder(folder);

		mAssetsGridScroll->HightlightAsset(assetId);
	}

	void AssetsWindow::CopyAssets(const Vector<String>& assetsPaths)
	{
		mCuttingAssets.Clear();
		mAssetsGridScroll->UpdateCuttingAssets();

		Vector<WString> paths;
		for (auto& path : assetsPaths)
			paths.Add(o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + path);

		Clipboard::CopyFiles(paths);
	}

	void AssetsWindow::CopyAssetFolder(const String& src, const String& dst)
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

	void AssetsWindow::CutAssets(const Vector<String>& assetsPaths)
	{
		mCuttingAssets.Clear();

		Vector<WString> paths;
		for (auto& path : assetsPaths)
		{
			String fullPath = o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + path;
			mCuttingAssets.Add(Pair<UID, String>(o2Assets.GetAssetId(path), fullPath));
			paths.Add(fullPath);
		}

		Clipboard::CopyFiles(paths);

		mAssetsGridScroll->UpdateCuttingAssets();
	}

	void AssetsWindow::PasteAssets(const String& targetPath)
	{
		Vector<WString> paths = Clipboard::GetCopyFiles();
		for (auto path : paths)
		{
			String fileName = o2FileSystem.GetPathWithoutDirectories(path);
			bool isFolder = o2FileSystem.IsFolderExist(path);

			String copyFileName = o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + targetPath + "/" + fileName;
			String extension = o2FileSystem.GetFileExtension(fileName);
			String fileNameWithoutExt = o2FileSystem.GetFileNameWithoutExtension(fileName);
			bool endsAsCopy = fileNameWithoutExt.EndsWith("copy");
			int i = 0;
			while (o2FileSystem.IsFileExist(copyFileName))
			{
				copyFileName = o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + targetPath + "/" +
					fileNameWithoutExt;

				if (!endsAsCopy)
					copyFileName += " copy";

				if (i > 0)
					copyFileName += (String)(i + 1) + "." + extension;

				if (!isFolder)
					copyFileName += "." + extension;

				i++;
			}

			if (mCuttingAssets.ContainsPred([&](auto x) { return x.second == path; }))
			{
				o2FileSystem.FileMove(path, copyFileName);
				o2FileSystem.FileMove(path + ".meta", copyFileName + ".meta");
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

	void AssetsWindow::DeleteAssets(const Vector<String>& assetsPaths)
	{
		mCuttingAssets.Clear();
		mAssetsGridScroll->UpdateCuttingAssets();

		for (auto& path : assetsPaths)
			o2Assets.RemoveAsset(path, false);

		o2Assets.RebuildAssets();
	}

	void AssetsWindow::ImportAssets(const String& targetPath)
	{

	}

	void AssetsWindow::CreateFolderAsset(const String& targetPath, const String& name)
	{
		FolderAssetRef folderAsset = FolderAssetRef::CreateAsset();
		folderAsset->Save(o2Assets.MakeUniqueAssetName(targetPath + "/" + name));
	}

	void AssetsWindow::CreatePrefabAsset(const String& targetPath)
	{
		ActorAssetRef folderAsset = ActorAssetRef::CreateAsset();
		folderAsset->Save(o2Assets.MakeUniqueAssetName(targetPath + "/New prototye.proto"));

		o2Assets.RebuildAssets();
	}

	void AssetsWindow::CreateScriptAsset(const String& targetPath)
	{

	}

	void AssetsWindow::CreateAnimationAsset(const String& targetPath)
	{
		AnimationAssetRef folderAsset = AnimationAssetRef::CreateAsset();
		folderAsset->Save(o2Assets.MakeUniqueAssetName(targetPath + "/New animation.anim"));

		o2Assets.RebuildAssets();
	}

	Sprite* AssetsWindow::GetAssetIconSprite(const AssetRef& asset)
	{
		const Type& type = asset->GetType();

		if (type == TypeOf(ImageAsset))
			return mnew Sprite(asset->GetPath());
		else if (type == TypeOf(ActorAsset))
			return mnew Sprite("ui/UI4_actor_icon.png");
		else if (type == TypeOf(FolderAsset))
			return mnew Sprite("ui/UI4_big_folder_icon.png");
		else if (type == TypeOf(DataAsset))
			return mnew Sprite("ui/UI4_big_text_file_icon.png");
		else if (type == TypeOf(AnimationAsset))
			return mnew Sprite("ui/UI4_anim_file_icon.png"); 

		return mnew Sprite("ui/UI4_big_file_icon.png"); 
	}

	void AssetsWindow::OnSearchEdited(const WString& search)
	{}

	void AssetsWindow::OnMenuFilterPressed()
	{}

	void AssetsWindow::OnShowTreePressed()
	{
		mFoldersTreeVisible = !mFoldersTreeVisible;

		if (mFoldersTreeVisible)
			mFoldersTreeShowAnim.PlayForward();
		else
			mFoldersTreeShowAnim.PlayBack();
	}

	void AssetsWindow::OnAssetsRebuilded(const Vector<UID>& changedAssets)
	{
		mFoldersTree->UpdateView();
		mFoldersTree->SelectAndExpandFolder(mAssetsGridScroll->GetViewingPath());

		mAssetsGridScroll->UpdateAssetsGridByCurrentPath();
	}
}

DECLARE_CLASS(Editor::AssetsWindow);
