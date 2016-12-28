#include "Assets.h"

#include "Assets/Asset.h"
#include "Assets/BinaryAsset.h"
#include "Assets/Builder/AssetsBuilder.h"
#include "Assets/FolderAsset.h"
#include "Config/ProjectConfig.h"
#include "Utils/Debug.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	DECLARE_SINGLETON(Assets);

	Assets::Assets()
	{
		InitializeProperties();

		mAssetsFolderPath = ASSETS_PATH;
		mDataFolderPath = DATA_PATH;

		mLog = mnew LogStream("Assets");
		o2Debug.GetLog()->BindStream(mLog);

		mAssetsBuilder = mnew AssetsBuilder();

		LoadAssetTypes();

		if (ASSETS_PREBUILDING_ENABLE)
			RebuildAssets();
		else
			LoadAssetsTree();
	}

	Assets::~Assets()
	{
		delete mAssetsBuilder;
	}

	String Assets::GetAssetsPath() const
	{
		return o2Config.GetProjectPath() + mAssetsFolderPath;
	}

	String Assets::GetDataPath() const
	{
		return mDataFolderPath;
	}

	String Assets::GetAssetPath(UID id) const
	{
		return GetAssetInfo(id).mPath;
	}

	UID Assets::GetAssetId(const String& path) const
	{
		return GetAssetInfo(path).mId;
	}

	AssetInfo Assets::GetAssetInfo(UID id) const
	{
		return mAssetsTree.FindAssetInfo(id);
	}

	AssetInfo Assets::GetAssetInfo(const String& path) const
	{
		return mAssetsTree.FindAssetInfo(path);
	}

	UID Assets::GetRandomAssetId()
	{
		return UID();
	}

	const Assets::TypesExtsDict Assets::GetAssetsExtensionsTypes() const
	{
		return mAssetsTypes;
	}

	const Type* Assets::GetStdAssetType() const
	{
		return mStdAssetType;
	}

	const Type* Assets::GetAssetTypeByExtension(const String& extension) const
	{
		if (mAssetsTypes.ContainsKey(extension))
			return mAssetsTypes[extension];

		return mStdAssetType;
	}

	Asset* Assets::LoadAsset(const AssetInfo& info)
	{
		const Type* assetType = Reflection::GetType(info.mType);
		if (!assetType)
		{
			mLog->Error("Failed to load asset by info: incorrect type (%i)", info.mType);
			return nullptr;
		}

		Asset* res = (Asset*)assetType->CreateSample();
		res->Load(info);
		return res;
	}

	bool Assets::IsAssetExist(const String& path) const
	{
		return GetAssetInfo(path).mId != 0;
	}

	bool Assets::IsAssetExist(UID id) const
	{
		return GetAssetInfo(id).mId != 0;
	}

	bool Assets::IsAssetExist(const AssetInfo& info) const
	{
		return GetAssetInfo(info.mId).mId == 0;
	}

	bool Assets::RemoveAsset(Asset* asset, bool rebuildAssets /*= true*/)
	{
		return RemoveAsset(asset->GetAssetId());
	}

	bool Assets::RemoveAsset(const String& path, bool rebuildAssets /*= true*/)
	{
		return RemoveAsset(GetAssetInfo(path), rebuildAssets);
	}

	bool Assets::RemoveAsset(UID id, bool rebuildAssets /*= true*/)
	{
		return RemoveAsset(GetAssetInfo(id), rebuildAssets);
	}

	bool Assets::RemoveAsset(const AssetInfo& info, bool rebuildAssets /*= true*/)
	{
		if (info.mId == 0)
		{
			mLog->Error("Can't remove asset by id (%s) - asset isn't exist", info.mPath);
			return false;
		}

		o2FileSystem.FileDelete(GetAssetsPath() + info.mPath + ".meta");

		if (info.mType == TypeOf(FolderAsset).ID())
			o2FileSystem.FolderRemove(GetAssetsPath() + info.mPath);
		else
			o2FileSystem.FileDelete(GetAssetsPath() + info.mPath);

		if (rebuildAssets)
			RebuildAssets();

		return true;
	}

	bool Assets::CopyAsset(Asset* asset, const String& dest, bool rebuildAssets /*= true*/)
	{
		return CopyAsset(asset->GetAssetId(), dest, rebuildAssets);
	}

	bool Assets::CopyAsset(const String& path, const String& dest, bool rebuildAssets /*= true*/)
	{
		return CopyAsset(GetAssetInfo(path), dest, rebuildAssets);
	}

	bool Assets::CopyAsset(UID id, const String& dest, bool rebuildAssets /*= true*/)
	{
		return CopyAsset(GetAssetInfo(id), dest, rebuildAssets);
	}

	bool Assets::CopyAsset(const AssetInfo& info, const String& dest, bool rebuildAssets /*= true*/)
	{
		if (info.mId == 0)
		{
			mLog->Error("Can't copy asset %s - asset isn't exist", info.mPath);
			return false;
		}

		if (IsAssetExist(dest) != 0)
		{
			mLog->Error("Can't copy asset %s \nto new path %s\n - another asset exist in target path",
						info.mPath, dest);
			return false;
		}

		if (info.mType == TypeOf(FolderAsset).ID())
		{
			o2FileSystem.FolderCreate(GetAssetsPath() + dest);
			FolderAsset folderAsset(info.mId);

			for (auto inInfo : folderAsset.GetContainingAssetsInfos())
				CopyAsset(inInfo, dest + o2FileSystem.GetPathWithoutDirectories(inInfo.mPath), false);
		}
		else o2FileSystem.FileCopy(GetAssetsPath() + info.mPath, GetAssetsPath() + dest);

		if (rebuildAssets)
			RebuildAssets();

		return true;
	}

	bool Assets::MoveAsset(Asset* asset, const String& newPath, bool rebuildAssets /*= true*/)
	{
		return MoveAsset(asset->GetAssetId(), newPath);
	}

	bool Assets::MoveAsset(const String& path, const String& newPath, bool rebuildAssets /*= true*/)
	{
		return MoveAsset(GetAssetInfo(path), newPath, rebuildAssets);
	}

	bool Assets::MoveAsset(UID id, const String& newPath, bool rebuildAssets /*= true*/)
	{
		return MoveAsset(GetAssetInfo(id), newPath, rebuildAssets);
	}

	bool Assets::MoveAsset(const AssetInfo& info, const String& newPath, bool rebuildAssets /*= true*/)
	{
		if (info.mId == 0)
		{
			mLog->Error("Can't remove asset by path (%s) - asset isn't exist", info.mPath);
			return false;
		}

		if (GetAssetId(newPath) != 0)
		{
			mLog->Error("Can't remove asset by path (%s) \nto new path (%s)\n - another asset exist in target path",
						info.mPath, newPath);
			return false;
		}

		o2FileSystem.FileMove(GetAssetsPath() + info.mPath + ".meta", GetAssetsPath() + newPath + ".meta");

		if (info.mType == TypeOf(FolderAsset).ID())
			o2FileSystem.FileMove(GetAssetsPath() + info.mPath, GetAssetsPath() + newPath);
		else
			o2FileSystem.FileMove(GetAssetsPath() + info.mPath, GetAssetsPath() + newPath);

		if (rebuildAssets)
			RebuildAssets();

		return true;
	}

	bool Assets::MoveAssets(const Vector<AssetInfo>& assets, const String& destPath, bool rebuildAssets /*= true*/)
	{
		bool res = true;
		for (auto& info : assets)
		{
			if (!o2Assets.MoveAsset(info, destPath + "/" + o2FileSystem.GetPathWithoutDirectories(info.mPath), false))
				res = false;
		}

		if (rebuildAssets)
			RebuildAssets();

		return res;
	}

	bool Assets::RenameAsset(Asset* asset, const String& newName, bool rebuildAssets /*= true*/)
	{
		return RenameAsset(GetAssetInfo(asset->GetAssetId()), newName, rebuildAssets);
	}

	bool Assets::RenameAsset(const String& path, const String& newName, bool rebuildAssets /*= true*/)
	{
		return RenameAsset(GetAssetInfo(path), newName, rebuildAssets);
	}

	bool Assets::RenameAsset(UID id, const String& newName, bool rebuildAssets /*= true*/)
	{
		return RenameAsset(GetAssetInfo(id), newName, rebuildAssets);
	}

	bool Assets::RenameAsset(const AssetInfo& info, const String& newName, bool rebuildAssets /*= true*/)
	{
		if (info.mId == 0)
		{
			mLog->Error("Can't rename asset by path (%s) - asset isn't exist", info.mPath);
			return false;
		}

		String parentFolder = o2FileSystem.GetParentPath(info.mPath);
		String assetShortName = o2FileSystem.GetPathWithoutDirectories(info.mPath);
		String newFullName = parentFolder + "/" + newName;

		if (GetAssetId(newFullName) != 0)
		{
			mLog->Error("Can't rename asset by path (%s) \nto (%s)\n - another asset exist in target path",
						info.mPath, newName);
			return false;
		}

		o2FileSystem.Rename(GetAssetsPath() + info.mPath + ".meta", GetAssetsPath() + newFullName + ".meta");
		o2FileSystem.Rename(GetAssetsPath() + info.mPath, GetAssetsPath() + newFullName);

		if (rebuildAssets)
			RebuildAssets();

		return true;
	}

	void Assets::RebuildAssets(bool forcible /*= false*/)
	{
		auto changedAssetsIds = mAssetsBuilder->BuildAssets(GetAssetsPath(), GetDataPath());
		mAssetsTree.BuildTree(DATA_PATH);

		onAssetsRebuilded(changedAssetsIds);
	}

	const AssetTree& Assets::GetAssetsTree() const
	{
		return mAssetsTree;
	}

	String Assets::MakeUniqueAssetName(const String& path)
	{
		if (!IsAssetExist(path))
			return path;

		String extension = o2FileSystem.GetFileExtension(path);
		String withoutExtension = o2FileSystem.GetFileNameWithoutExtension(path);

		if (withoutExtension.EndsWith("copy"))
		{
			if (extension.IsEmpty())
				return MakeUniqueAssetName(withoutExtension.SubStr(0, withoutExtension.FindLast("copy")) + "copy 1");
			else
				return MakeUniqueAssetName(withoutExtension.SubStr(0, withoutExtension.FindLast("copy")) + "copy 1." + extension);
		}

		int copyPos = withoutExtension.FindLast("copy");
		if (copyPos >= 0)
		{
			String end = withoutExtension.SubStr(copyPos + 4).Trimed(" ");
			int endNum = (int)end;

			if (endNum > 0)
			{
				if (extension.IsEmpty())
					return MakeUniqueAssetName(withoutExtension.SubStr(0, copyPos) + "copy " + (String)(endNum + 1));
				else
					return MakeUniqueAssetName(withoutExtension.SubStr(0, copyPos) + "copy " + (String)(endNum + 1) + "." + extension);
			}
		}

		if (extension.IsEmpty())
			return MakeUniqueAssetName(path + " copy");
		
		return MakeUniqueAssetName(withoutExtension + " copy." + extension);
	}

	void Assets::LoadAssetsTree()
	{
		mAssetsTree.BuildTree(DATA_PATH);
	}

	void Assets::LoadAssetTypes()
	{
		mStdAssetType = &TypeOf(BinaryAsset);

		auto assetTypes = TypeOf(Asset).GetDerivedTypes();

		for (auto type : assetTypes)
		{
			auto sample = (Asset*)type->CreateSample();
			String extensions = sample->GetFileExtensions();
			auto extensionsVec = extensions.Split(" ");

			delete sample;

			for (auto ext : extensionsVec)
			{
				if (mAssetsTypes.ContainsKey(ext))
				{
					mLog->Warning("Assets extensions duplicating: %s, at %s and %s", 
								  ext, mAssetsTypes[ext]->GetName(), type->GetName());
					continue;
				}

				mAssetsTypes.Add(ext, type);
			}
		}
	}

	void Assets::InitializeProperties()
	{
		INITIALIZE_GETTER(Assets, assetsPath, GetAssetsPath);
	}
}