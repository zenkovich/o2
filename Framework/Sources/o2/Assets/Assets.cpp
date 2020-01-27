#include "o2/stdafx.h"
#include "Assets.h"

#include "o2/Assets/Asset.h"
#include "o2/Assets/BinaryAsset.h"
#include "o2/Assets/Builder/AssetsBuilder.h"
#include "o2/Assets/FolderAsset.h"
#include "o2/Config/ProjectConfig.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	DECLARE_SINGLETON(Assets);

	Assets::Assets()
	{
		mAssetsFolderPath = o2Config.GetProjectPath() + ::GetAssetsPath();
		mDataFolderPath = o2Config.GetProjectPath() + ::GetBuiltAssetsPath();

		mLog = mnew LogStream("Assets");
		o2Debug.GetLog()->BindStream(mLog);

		mAssetsBuilder = mnew AssetsBuilder();

		LoadAssetTypes();

		if (::IsAssetsPrebuildEnabled())
			RebuildAssets();
		else
			LoadAssetsTree();
	}

	Assets::~Assets()
	{
		delete mAssetsBuilder;
	}

	const String& Assets::GetAssetsPath() const
	{
		return mAssetsFolderPath;
	}

	const String& Assets::GetBuiltAssetsPath() const
	{
		return mDataFolderPath;
	}

	const String& Assets::GetAssetPath(const UID& id) const
	{
		return GetAssetInfo(id).path;
	}

	const UID& Assets::GetAssetId(const String& path) const
	{
		return GetAssetInfo(path).id;
	}

	const AssetInfo& Assets::GetAssetInfo(const UID& id) const
	{
		if (auto res = mAssetsTree.Find(id))
			return *res;

		return AssetInfo::empty;
	}

	const AssetInfo& Assets::GetAssetInfo(const String& path) const
	{
		if (auto res = mAssetsTree.Find(path))
			return *res;

		return AssetInfo::empty;
	}

	const Map<String, const Type*> Assets::GetAssetsExtensionsTypes() const
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
			return mAssetsTypes.Get(extension);

		return mStdAssetType;
	}

	AssetRef Assets::GetAssetRef(const String& path)
	{
		auto cached = FindAssetCache(path);

		if (!cached)
		{
			auto assetInfo = mAssetsTree.Find(path);
			if (!assetInfo->assetType)
				return AssetRef();

			Asset* asset = (Asset*)assetInfo->assetType->CreateSample();
			asset->Load(path);

			cached = mnew AssetCache();
			cached->asset = asset;
			cached->referencesCount = 0;

			mCachedAssets.Add(cached);
			mCachedAssetsByPath[cached->asset->GetPath()] = cached;
			mCachedAssetsByPath[cached->asset->GetAssetId()] = cached;
		}

		return AssetRef(cached->asset, &cached->referencesCount);
	}

	AssetRef Assets::GetAssetRef(const UID& id)
	{
		auto cached = FindAssetCache(id);

		if (!cached)
		{
			auto assetInfo = mAssetsTree.Find(id);
			if (!assetInfo->assetType)
				return AssetRef();

			Asset* asset = (Asset*)assetInfo->assetType->CreateSample();
			asset->Load(id);

			cached = mnew AssetCache();
			cached->asset = asset;
			cached->referencesCount = 0;

			mCachedAssets.Add(cached);
			mCachedAssetsByPath[cached->asset->GetPath()] = cached;
			mCachedAssetsByPath[cached->asset->GetAssetId()] = cached;
		}

		return AssetRef(cached->asset, &cached->referencesCount);
	}

	bool Assets::IsAssetExist(const String& path) const
	{
		return GetAssetInfo(path).id != 0;
	}

	bool Assets::IsAssetExist(const UID& id) const
	{
		return GetAssetInfo(id).id != 0;
	}

	bool Assets::IsAssetExist(const AssetInfo& info) const
	{
		return GetAssetInfo(info.id).id == 0;
	}

	bool Assets::RemoveAsset(const AssetRef& asset, bool rebuildAssets /*= true*/)
	{
		return RemoveAsset(asset->GetAssetId());
	}

	bool Assets::RemoveAsset(const String& path, bool rebuildAssets /*= true*/)
	{
		return RemoveAsset(GetAssetInfo(path), rebuildAssets);
	}

	bool Assets::RemoveAsset(const UID& id, bool rebuildAssets /*= true*/)
	{
		return RemoveAsset(GetAssetInfo(id), rebuildAssets);
	}

	bool Assets::RemoveAsset(const AssetInfo& info, bool rebuildAssets /*= true*/)
	{
		if (info.id == 0)
		{
			mLog->Error("Can't remove asset by id (" + info.path + ") - asset isn't exist");
			return false;
		}

		o2FileSystem.FileDelete(GetAssetsPath() + info.path + ".meta");

		if (info.assetType == &TypeOf(FolderAsset))
			o2FileSystem.FolderRemove(GetAssetsPath() + info.path);
		else
			o2FileSystem.FileDelete(GetAssetsPath() + info.path);

		if (rebuildAssets)
			RebuildAssets();

		return true;
	}

	bool Assets::CopyAsset(const AssetRef& asset, const String& dest, bool rebuildAssets /*= true*/)
	{
		return CopyAsset(asset->GetAssetId(), dest, rebuildAssets);
	}

	bool Assets::CopyAsset(const String& path, const String& dest, bool rebuildAssets /*= true*/)
	{
		return CopyAsset(GetAssetInfo(path), dest, rebuildAssets);
	}

	bool Assets::CopyAsset(const UID& id, const String& dest, bool rebuildAssets /*= true*/)
	{
		return CopyAsset(GetAssetInfo(id), dest, rebuildAssets);
	}

	bool Assets::CopyAsset(const AssetInfo& info, const String& dest, bool rebuildAssets /*= true*/)
	{
		if (info.id == 0)
		{
			mLog->Error("Can't copy asset " + info.path + " - asset isn't exist");
			return false;
		}

		if (IsAssetExist(dest) != 0)
		{
			mLog->Error("Can't copy asset " + info.path + "\nto new path " + dest + "\n - another asset exist in target path");
			return false;
		}

		if (info.assetType == &TypeOf(FolderAsset))
		{
			o2FileSystem.FolderCreate(GetAssetsPath() + dest);
			FolderAsset folderAsset(info.id);

			for (auto inInfo : folderAsset.GetContainingAssetsInfos())
				CopyAsset(inInfo, dest + o2FileSystem.GetPathWithoutDirectories(inInfo.path), false);
		}
		else o2FileSystem.FileCopy(GetAssetsPath() + info.path, GetAssetsPath() + dest);

		if (rebuildAssets)
			RebuildAssets();

		return true;
	}

	bool Assets::MoveAsset(const AssetRef& asset, const String& newPath, bool rebuildAssets /*= true*/)
	{
		return MoveAsset(asset->GetAssetId(), newPath);
	}

	bool Assets::MoveAsset(const String& path, const String& newPath, bool rebuildAssets /*= true*/)
	{
		return MoveAsset(GetAssetInfo(path), newPath, rebuildAssets);
	}

	bool Assets::MoveAsset(const UID& id, const String& newPath, bool rebuildAssets /*= true*/)
	{
		return MoveAsset(GetAssetInfo(id), newPath, rebuildAssets);
	}

	bool Assets::MoveAsset(const AssetInfo& info, const String& newPath, bool rebuildAssets /*= true*/)
	{
		if (info.id == 0)
		{
			mLog->Error("Can't remove asset by path (" + info.path + ") - asset isn't exist");
			return false;
		}

		if (GetAssetId(newPath) != 0)
		{
			mLog->Error("Can't remove asset by path (" + info.path + ") \nto new path (" + newPath +
						")\n - another asset exist in target path");
			return false;
		}

		o2FileSystem.FileMove(GetAssetsPath() + info.path + ".meta", GetAssetsPath() + newPath + ".meta");

		if (info.assetType == &TypeOf(FolderAsset))
			o2FileSystem.FileMove(GetAssetsPath() + info.path, GetAssetsPath() + newPath);
		else
			o2FileSystem.FileMove(GetAssetsPath() + info.path, GetAssetsPath() + newPath);

		if (rebuildAssets)
			RebuildAssets();

		return true;
	}

	bool Assets::MoveAssets(const Vector<AssetInfo>& assets, const String& destPath, bool rebuildAssets /*= true*/)
	{
		bool res = true;
		for (auto& info : assets)
		{
			if (!o2Assets.MoveAsset(info, destPath + "/" + o2FileSystem.GetPathWithoutDirectories(info.path), false))
				res = false;
		}

		if (rebuildAssets)
			RebuildAssets();

		return res;
	}

	bool Assets::RenameAsset(const AssetRef& asset, const String& newName, bool rebuildAssets /*= true*/)
	{
		return RenameAsset(GetAssetInfo(asset->GetAssetId()), newName, rebuildAssets);
	}

	bool Assets::RenameAsset(const String& path, const String& newName, bool rebuildAssets /*= true*/)
	{
		return RenameAsset(GetAssetInfo(path), newName, rebuildAssets);
	}

	bool Assets::RenameAsset(const UID& id, const String& newName, bool rebuildAssets /*= true*/)
	{
		return RenameAsset(GetAssetInfo(id), newName, rebuildAssets);
	}

	bool Assets::RenameAsset(const AssetInfo& info, const String& newName, bool rebuildAssets /*= true*/)
	{
		if (info.id == 0)
		{
			mLog->Error("Can't rename asset by path (" + info.path + ") - asset isn't exist");
			return false;
		}

		String parentFolder = o2FileSystem.GetParentPath(info.path);
		String assetShortName = o2FileSystem.GetPathWithoutDirectories(info.path);
		String newFullName = parentFolder + "/" + newName;

		if (GetAssetId(newFullName) != 0)
		{
			mLog->Error("Can't rename asset by path (" + info.path + ") \nto (" + newName + ")\n - another asset exist in target path");
			return false;
		}

		o2FileSystem.Rename(GetAssetsPath() + info.path + ".meta", GetAssetsPath() + newFullName + ".meta");
		o2FileSystem.Rename(GetAssetsPath() + info.path, GetAssetsPath() + newFullName);

		if (rebuildAssets)
			RebuildAssets();

		return true;
	}

	void Assets::RebuildAssets(bool forcible /*= false*/)
	{
		ClearAssetsCache();

		auto changedAssetsIds = mAssetsBuilder->BuildAssets(o2Config.GetProjectPath() + ::GetEditorAssetsPath(),
															o2Config.GetProjectPath() + ::GetEditorBuiltAssetsPath(),
															o2Config.GetProjectPath() + ::GetEditorBuiltAssetsTreePath(), forcible);

		changedAssetsIds += mAssetsBuilder->BuildAssets(o2Config.GetProjectPath() + ::GetAssetsPath(),
														o2Config.GetProjectPath() + ::GetBuiltAssetsPath(),
														o2Config.GetProjectPath() + ::GetBuiltAssetsTreePath(), forcible);

		LoadAssetsTree();

		onAssetsRebuilded(changedAssetsIds);
	}

	const AssetsTree& Assets::GetAssetsTree() const
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
		mAssetsTree.Clear();
		DataNode data;

		data.LoadFromFile(o2Config.GetProjectPath() + ::GetEditorBuiltAssetsTreePath());
		mAssetsTree.Deserialize(data);

		data.Clear();
		data.LoadFromFile(o2Config.GetProjectPath() + ::GetBuiltAssetsTreePath());
		mAssetsTree.Deserialize(data);
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
					mLog->Warning("Assets extensions duplicating: " + ext + ", at " + mAssetsTypes[ext]->GetName() +
								  " and " + type->GetName());
					continue;
				}

				mAssetsTypes.Add(ext, type);
			}
		}
	}

	Assets::AssetCache* Assets::FindAssetCache(const String& path)
	{
		Assets::AssetCache* res = nullptr;
		mCachedAssetsByPath.TryGetValue(path, res);
		return res;
	}

	Assets::AssetCache* Assets::FindAssetCache(const UID& id)
	{
		Assets::AssetCache* res = nullptr;
		mCachedAssetsByPath.TryGetValue(id, res);
		return res;
	}

	void Assets::ClearAssetsCache()
	{
		auto cached = mCachedAssets;
		mCachedAssets.Clear();

		for (auto cache : cached)
		{
			if (cache->referencesCount == 0)
				delete cache->asset;
		}
	}

	void Assets::AddAssetCache(AssetRef& ref)
	{
		auto cached = mnew AssetCache();
		cached->asset = ref.mAssetPtr;
		cached->referencesCount = 1;

		mCachedAssets.Add(cached);
		mCachedAssetsByPath[cached->asset->GetAssetId()] = cached;
	}

	Assets::AssetCache::~AssetCache()
	{
		delete asset;
	}

}
