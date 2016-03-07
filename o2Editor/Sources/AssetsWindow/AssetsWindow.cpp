#include "AssetsWindow.h"

#include "Application/Application.h"
#include "Assets/ActorAsset.h"
#include "Assets/AnimationAsset.h"
#include "Assets/Assets.h"
#include "Assets/AssetsTree.h"
#include "Assets/FolderAsset.h"
#include "Assets/ImageAsset.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/FoldersTree.h"
#include "AssetsWindow/UIAssetIcon.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/GridLayout.h"
#include "UI/Label.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "Utils/Clipboard.h"
#include "Utils/Delegates.h"
#include "Utils/FileSystem/FileSystem.h"

DECLARE_SINGLETON(Editor::AssetsWindow);

namespace Editor
{
	AssetsWindow::AssetsWindow()
	{
		InitializeWindow();
	}

	AssetsWindow::~AssetsWindow()
	{}

	void AssetsWindow::SelectAsset(AssetId id)
	{
		String assetPath = o2Assets.GetAssetPath(id);
		String folder = o2FileSystem.GetParentPath(assetPath);

		if (GetOpenedFolderPath() != folder)
			OpenFolder(folder);

		mAssetsGridScroll->SelectAsset(id);
	}

	void AssetsWindow::SelectAsset(const String& path)
	{
		AssetId id = o2Assets.GetAssetId(path);
		String folder = o2FileSystem.GetParentPath(path);

		if (GetOpenedFolderPath() != folder)
			OpenFolder(folder);

		mAssetsGridScroll->SelectAsset(id);
	}

	void AssetsWindow::SelectAsset(const Vector<AssetId>& ids)
	{
		for (auto id : ids)
			SelectAsset(id);
	}

	void AssetsWindow::SelectAssets(const Vector<String>& paths)
	{
		for (auto path : paths)
			SelectAsset(path);
	}

	void AssetsWindow::OpenAsset(AssetId id)
	{
		OpenAsset(o2Assets.GetAssetPath(id));
	}

	void AssetsWindow::OpenAsset(const String& path)
	{
		String fullPath = o2FileSystem.GetParentPath(o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + path);
		ShellExecute(NULL, "explore", fullPath, NULL, NULL, SW_SHOWNORMAL);
	}

	void AssetsWindow::OpenAndEditAsset(AssetId id)
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
	}

	void AssetsWindow::ShowAssetIcon(AssetId id)
	{
		DeselectAssets();
		SelectAsset(id);
	}

	void AssetsWindow::ShowAssetIcon(const String& path)
	{
		DeselectAssets();
		SelectAsset(path);
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

	void AssetsWindow::CutAssets(const Vector<String>& assetsPaths)
	{
		mCuttingAssets.Clear();

		Vector<WString> paths;
		for (auto& path : assetsPaths)
		{
			String fullPath = o2Application.GetBinPath() + "/" + o2Assets.GetAssetsPath() + path;
			mCuttingAssets.Add(Pair<AssetId, String>(o2Assets.GetAssetId(path), fullPath));
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

			if (mCuttingAssets.ContainsPred([&](auto x) { return x.mSecond == path; }))
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

	void AssetsWindow::CreateFolderAsset(const String& targetPath)
	{
		FolderAsset folderAsset;
		folderAsset.Save(o2Assets.MakeUniqueAssetName(targetPath + "/New folder"));

		o2Assets.RebuildAssets();
	}

	void AssetsWindow::CreatePrefabAsset(const String& targetPath)
	{
		ActorAsset folderAsset;
		folderAsset.Save(o2Assets.MakeUniqueAssetName(targetPath + "/New prefab.prefab"));

		o2Assets.RebuildAssets();
	}

	void AssetsWindow::CreateScriptAsset(const String& targetPath)
	{

	}

	void AssetsWindow::CreateAnimationAsset(const String& targetPath)
	{
		AnimationAsset folderAsset;
		folderAsset.Save(o2Assets.MakeUniqueAssetName(targetPath + "/New animation.anim"));

		o2Assets.RebuildAssets();
	}

	void AssetsWindow::InitializeWindow()
	{
		o2Assets.onAssetsRebuilded += Function<void(const Vector<AssetId>&)>(this, &AssetsWindow::OnAssetsRebuilded);

		mWindow->caption = "Assets";
		mWindow->name = "assets window";
		if (auto iconLayer = mWindow->GetLayer("icon"))
		{
			*((Sprite*)iconLayer->drawable) = Sprite("ui/UI2_folder_icon.png");
			iconLayer->layout = Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 1));
		}

		mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 18));

		// up panel
		UIWidget* upPanel = mnew UIWidget();
		upPanel->name = "up panel";
		upPanel->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		upPanel->AddLayer("back", mnew Sprite("ui/UI_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		UIButton* searchButton = o2UI.CreateWidget<UIButton>("search");
		searchButton->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
		upPanel->AddChild(searchButton);

		mFilterButton = o2UI.CreateWidget<UIButton>("menu filter");
		mFilterButton->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 1));
		mFilterButton->onClick += Function<void()>(this, &AssetsWindow::OnMenuFilterPressed);
		upPanel->AddChild(mFilterButton);

		mSearchEditBox = o2UI.CreateWidget<UIEditBox>("backless");
		mSearchEditBox->layout = UIWidgetLayout::BothStretch(19, 2, 21, -2);
		mSearchEditBox->onChanged += Function<void(const WString&)>(this, &AssetsWindow::OnSearchEdited);
		upPanel->AddChild(mSearchEditBox);

		mWindow->AddChild(upPanel);

		// down panel
		UIWidget* downPanel = mnew UIWidget();
		downPanel->AddLayer("back", mnew Sprite("ui/UI2_small_panel_down_back.png"),
							Layout::BothStretch(-4, -5, -4, -5));
		downPanel->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 20, 0);

		auto showTreeBtn = o2UI.CreateWidget<UIButton>("menu tree");
		showTreeBtn->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(0, 0));
		showTreeBtn->onClick += Function<void()>(this, &AssetsWindow::OnShowTreePressed);
		downPanel->AddChild(showTreeBtn);

		mSelectedAssetPathLabel = o2UI.CreateWidget<UILabel>();
		mSelectedAssetPathLabel->layout = UIWidgetLayout::BothStretch(20, 0, 0, 0);
		mSelectedAssetPathLabel->text = "Assets/Folder/Image.png";
		mSelectedAssetPathLabel->horOverflow = UILabel::HorOverflow::Dots;
		mSelectedAssetPathLabel->horAlign = HorAlign::Left;
		downPanel->AddChild(mSelectedAssetPathLabel);

		mWindow->AddChild(downPanel);

		// folders tree
		mFoldersTree = mnew UIAssetsFoldersTree();
		mFoldersTree->layout = UIWidgetLayout(0.0f, 1.0f, 0.5f, 0.0f, 0.0f, -18.0f, 0.0f, 18.0f);

		auto separatorLayer = mFoldersTree->AddLayer("separator", mnew Sprite("ui/UI_Ver_separator.png"),
													 Layout::VerStretch(HorAlign::Right, -2, 0, 5, 0));
		mFoldersTree->RebuildTree();
		mWindow->AddChild(mFoldersTree);

		// assets scroll & grid
		mAssetsGridScroll = o2UI.CreateWidget<UIAssetsIconsScrollArea>();
		mAssetsGridScroll->layout = UIWidgetLayout(0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -18.0f, 0.0f, 18.0f);
		mWindow->AddChild(mAssetsGridScroll);

		// separator
		mSeparatorHandle.isUnderPoint = [=](const Vec2F& point) {
			return separatorLayer->drawable->IsUnderPoint(point);
		};

		mSeparatorHandle.onMoved = [&](const Input::Cursor& cursor) {
			float anchorDelta = cursor.mDelta.x / mWindow->layout.width;
			mFoldersTree->layout.anchorRight += anchorDelta;
			mAssetsGridScroll->layout.anchorLeft += anchorDelta;
		};

		mWindow->onDraw += [&]() { mSeparatorHandle.OnDrawn(); };

		mSeparatorHandle.cursorType = CursorType::SizeWE;

		OpenFolder("");
	}

	void AssetsWindow::OnSearchEdited(const WString& search)
	{

	}

	void AssetsWindow::OnMenuFilterPressed()
	{

	}

	void AssetsWindow::OnShowTreePressed()
	{

	}

	void AssetsWindow::OnAssetsRebuilded(const Vector<AssetId>& changedAssets)
	{
		mFoldersTree->RebuildTree();
		mAssetsGridScroll->UpdateAssetsPath();
	}

	void AssetsWindow::CopyAssetFolder(const String& src, const String& dst)
	{
		o2FileSystem.FolderCreate(dst);

		FolderInfo info = o2FileSystem.GetFolderInfo(src);
		info.ClampPathNames();

		for (auto& file : info.mFiles)
		{
			if (!file.mPath.EndsWith(".meta"))
				o2FileSystem.FileCopy(src + "/" + file.mPath, dst + "/" + file.mPath);
		}

		for (auto& folder : info.mFolders)
			CopyAssetFolder(src + "/" + folder.mPath, dst + "/" + folder.mPath);
	}
}