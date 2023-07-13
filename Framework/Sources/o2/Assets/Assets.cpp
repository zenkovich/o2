#include "o2/stdafx.h"
#include "Assets.h"

#include "o2/Assets/Asset.h"
#include "o2/Assets/Types/BinaryAsset.h"
#include "o2/Assets/Types/FolderAsset.h"
#include "o2/Config/ProjectConfig.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/EngineSettings.h"

namespace o2
{
	DECLARE_SINGLETON(Assets);

	Assets::Assets()
	{
		mLog = mnew LogStream("Assets");
		o2Debug.GetLog()->BindStream(mLog);

		LoadAssetsTree();
	}

	Assets::~Assets()
	{
	}

	String Assets::GetAssetsPath() const
	{
		return ::GetAssetsPath();
	}

	String Assets::GetBuiltAssetsPath() const
	{
		return ::GetBuiltAssetsPath();
	}

	const String& Assets::GetAssetPath(const UID& id) const
	{
		return GetAssetInfo(id).path;
	}

	const UID& Assets::GetAssetId(const String& path) const
	{
		auto& info = GetAssetInfo(path);
		return info.meta ? info.meta->ID() : UID::empty;
	}

	const AssetInfo& Assets::GetAssetInfo(const UID& id) const
	{
		for (auto tree : mAssetsTrees)
		{
			if (auto res = tree->Find(id))
				return *res;
		}

		return AssetInfo::empty;
	}

	const AssetInfo& Assets::GetAssetInfo(const String& path) const
	{
		for (auto tree : mAssetsTrees)
		{
			if (auto res = tree->Find(path))
				return *res;
		}

		return AssetInfo::empty;
	}

	const Map<String, const Type*> Assets::GetAssetsExtensionsTypes()
	{
		static Map<String, const Type*> assetTypes;
		if (assetTypes.IsEmpty())
		{
			auto derivedAssetTypes = TypeOf(Asset).GetDerivedTypes();

			for (auto type : derivedAssetTypes)
			{
				String extensions = type->InvokeStatic<const char*>("GetFileExtensions");
				auto extensionsVec = extensions.Split(" ");

				for (const auto& ext : extensionsVec)
				{
					if (assetTypes.ContainsKey(ext))
					{
						o2Debug.LogWarning("Assets extensions duplicating: " + ext + ", at " + assetTypes[ext]->GetName() +
										   " and " + type->GetName());
						continue;
					}

					assetTypes.Add(ext, type);
				}
			}
		}

		return assetTypes;
	}

	const Type* Assets::GetStdAssetType()
	{
		return &TypeOf(BinaryAsset);
	}

	const Type* Assets::GetAssetTypeByExtension(const String& extension)
	{
		if (GetAssetsExtensionsTypes().ContainsKey(extension))
			return GetAssetsExtensionsTypes().Get(extension);

		return GetStdAssetType();
	}

	AssetRef Assets::GetAssetRef(const String& path)
	{
		auto cached = FindAssetCache(path);

		if (!cached)
		{
			auto& assetInfo = GetAssetInfo(path);
			if (!assetInfo.IsValid())
				return AssetRef();

			auto type = assetInfo.meta->GetAssetType();
			Asset* asset = (Asset*)type->CreateSample();
			asset->Load(assetInfo);

			cached = FindAssetCache(asset->GetUID());
		}

		return AssetRef(cached->asset, &cached->referencesCount);
	}

	AssetRef Assets::GetAssetRef(const UID& id)
	{
		auto cached = FindAssetCache(id);

		if (!cached)
		{
			auto& assetInfo = GetAssetInfo(id);
			if (!assetInfo.IsValid())
			{
				o2Debug.LogError("Can't load asset by id - " + (String)id);
				return AssetRef();
			}

			Asset* asset = (Asset*)assetInfo.meta->GetAssetType()->CreateSample();
			asset->Load(assetInfo);

			cached = FindAssetCache(id);
		}

		return AssetRef(cached->asset, &cached->referencesCount);
	}

	bool Assets::IsAssetExist(const String& path) const
	{
		return GetAssetInfo(path).meta->ID() != UID::empty;
	}

	bool Assets::IsAssetExist(const UID& id) const
	{
		return GetAssetInfo(id).meta->ID() != UID::empty;
	}

	bool Assets::RemoveAsset(const AssetRef& asset)
	{
		return RemoveAsset(asset->GetUID());
	}

	bool Assets::RemoveAsset(const String& path)
	{
		return RemoveAsset(GetAssetInfo(path));
	}

	bool Assets::RemoveAsset(const UID& id)
	{
		return RemoveAsset(GetAssetInfo(id));
	}

	bool Assets::RemoveAsset(const AssetInfo& info)
	{
		if (info.meta == nullptr)
		{
			mLog->Error("Can't remove asset '" + info.path + "' - asset isn't exist");
			return false;
		}

		o2FileSystem.FileDelete(GetAssetsPath() + info.path + ".meta");

		if (info.meta->GetAssetType() == &TypeOf(FolderAsset))
			o2FileSystem.FolderRemove(GetAssetsPath() + info.path);
		else
			o2FileSystem.FileDelete(GetAssetsPath() + info.path);

		return true;
	}

	bool Assets::CopyAsset(const AssetRef& asset, const String& dest)
	{
		return CopyAsset(asset->GetUID(), dest);
	}

	bool Assets::CopyAsset(const String& path, const String& dest)
	{
		return CopyAsset(GetAssetInfo(path), dest);
	}

	bool Assets::CopyAsset(const UID& id, const String& dest)
	{
		return CopyAsset(GetAssetInfo(id), dest);
	}

	bool Assets::CopyAsset(const AssetInfo& info, const String& dest)
	{
		if (info.meta == nullptr)
		{
			mLog->Error("Can't copy asset '" + info.path + "' - asset isn't exist");
			return false;
		}

		if (IsAssetExist(dest) != 0)
		{
			mLog->Error("Can't copy asset '" + info.path + "' to new path '" + dest + "' - another asset exist in target path");
			return false;
		}

		if (info.meta->GetAssetType() == &TypeOf(FolderAsset))
		{
			o2FileSystem.FolderCreate(GetAssetsPath() + dest);
			FolderAssetRef folderAsset(info.path);

			for (auto inInfo : folderAsset->GetChildrenAssets())
				CopyAsset(inInfo, dest + o2FileSystem.GetPathWithoutDirectories(inInfo->GetPath()));
		}
		else o2FileSystem.FileCopy(GetAssetsPath() + info.path, GetAssetsPath() + dest);

		return true;
	}

	bool Assets::MoveAsset(const AssetRef& asset, const String& newPath)
	{
		return MoveAsset(asset->GetUID(), newPath);
	}

	bool Assets::MoveAsset(const String& path, const String& newPath)
	{
		return MoveAsset(GetAssetInfo(path), newPath);
	}

	bool Assets::MoveAsset(const UID& id, const String& newPath)
	{
		return MoveAsset(GetAssetInfo(id), newPath);
	}

	bool Assets::MoveAsset(const AssetInfo& info, const String& newPath)
	{
		if (info.meta == nullptr)
		{
			mLog->Error("Can't remove asset '" + info.path + "' - asset isn't exist");
			return false;
		}

		if (GetAssetId(newPath) != 0)
		{
			mLog->Error("Can't remove asset '" + info.path + "' to '" + newPath + "' - another asset exist in target path");
			return false;
		}

		o2FileSystem.FileMove(GetAssetsPath() + info.path + ".meta", GetAssetsPath() + newPath + ".meta");

		if (info.meta->GetAssetType() == &TypeOf(FolderAsset))
			o2FileSystem.FileMove(GetAssetsPath() + info.path, GetAssetsPath() + newPath);
		else
			o2FileSystem.FileMove(GetAssetsPath() + info.path, GetAssetsPath() + newPath);

		return true;
	}

	bool Assets::MoveAssets(const Vector<UID>& assets, const String& destPath)
	{
		bool res = true;
		for (auto& id : assets)
		{
			auto& info = GetAssetInfo(id);
			if (!o2Assets.MoveAsset(id, destPath + "/" + o2FileSystem.GetPathWithoutDirectories(info.path)))
				res = false;
		}

		return res;
	}

	bool Assets::RenameAsset(const AssetRef& asset, const String& newName)
	{
		return RenameAsset(GetAssetInfo(asset->GetUID()), newName);
	}

	bool Assets::RenameAsset(const String& path, const String& newName)
	{
		return RenameAsset(GetAssetInfo(path), newName);
	}

	bool Assets::RenameAsset(const UID& id, const String& newName)
	{
		return RenameAsset(GetAssetInfo(id), newName);
	}

	bool Assets::RenameAsset(const AssetInfo& info, const String& newName)
	{
		if (info.meta == nullptr)
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

		return true;
	}

	const Vector<AssetsTree*>& Assets::GetAssetsTrees() const
	{
		return mAssetsTrees;
	}

	const AssetsTree& Assets::GetAssetsTree() const
	{
		return *mMainAssetsTree;
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
		mAssetsTrees.Clear();

		auto editorAssetsTree = mnew AssetsTree();
		editorAssetsTree->DeserializeFromString(o2FileSystem.ReadFile(::GetEditorBuiltAssetsTreePath()));

		mMainAssetsTree = mnew AssetsTree();
		mMainAssetsTree->DeserializeFromString(o2FileSystem.ReadFile(::GetBuiltAssetsTreePath()));
        mMainAssetsTree->assetsPath = ::GetAssetsPath();
        mMainAssetsTree->builtAssetsPath = ::GetBuiltAssetsPath();

		mAssetsTrees.Add(mMainAssetsTree);
		mAssetsTrees.Add(editorAssetsTree);
	}

	void Assets::CheckAssetsUnload()
	{
		return;
		const bool checkDuplications = false;
		if (checkDuplications)
		{
			for (int i = 0; i < mCachedAssets.Count(); i++)
			{
				auto cache = mCachedAssets[i];
				for (int j = i + 1; j < mCachedAssets.Count(); j++)
				{
					auto otherCache = mCachedAssets[j];

					if (!cache->asset->GetPath().IsEmpty() && cache->asset->GetPath() == otherCache->asset->GetPath())
						o2Debug.LogError("Duplicate asset path \"" + cache->asset->GetPath() + "\"");

					if (cache->asset->GetUID() == otherCache->asset->GetUID())
						o2Debug.LogError("Duplicate asset UID \"" + cache->asset->GetPath() + "\" - " + cache->asset->GetUID());

					if (cache->asset == otherCache->asset)
						o2Debug.LogError("Duplicate asset cache");
				}
			}
		}

		auto cachedAssets = mCachedAssets;
		for (auto cached : cachedAssets)
		{
			if (cached->referencesCount <= 0)
				delete cached->asset;
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
		mCachedAssetsByUID.TryGetValue(id, res);
		return res;
	}

	void Assets::ClearAssetsCache()
	{
		auto cached = mCachedAssets;
		mCachedAssets.Clear();
		mCachedAssetsByPath.Clear();
		mCachedAssetsByUID.Clear();
		mAssetsTrees.Clear();

		for (auto cache : cached)
		{
			if (cache->referencesCount == 0)
				delete cache->asset;
		}
	}

	Assets::AssetCache* Assets::AddAssetCache(Asset* asset)
	{
		auto cached = mnew AssetCache();
		cached->asset = asset;
		cached->referencesCount = 0;

		mCachedAssets.Add(cached);

		if constexpr (IS_EDITOR)
		{
			if (asset->GetPath() == "stdFont.ttf")
				o2Debug.Log("asd");

			if (mCachedAssetsByUID.ContainsKey(asset->GetUID()))
				mLog->Error("Duplicated asset cache id: " + (String)asset->GetUID() + " - \"" + asset->GetPath() + "\"");

			if (!asset->GetPath().IsEmpty() && mCachedAssetsByPath.ContainsKey(asset->GetPath()))
				mLog->Error("Duplicated asset cache path: \"" + asset->GetPath() + "\"");
		}

		mCachedAssetsByPath[asset->GetPath()] = cached;
		mCachedAssetsByUID[asset->GetUID()] = cached;

		return cached;
	}

	void Assets::RemoveAssetCache(Asset* asset)
	{
		AssetCache* cached = nullptr;
		auto fnd = mCachedAssetsByUID.find(asset->GetUID());
		if (fnd != mCachedAssetsByUID.end()) {
			cached = fnd->second;
			mCachedAssetsByUID.erase(fnd);
		}

		auto fnd2 = mCachedAssetsByPath.find(asset->GetPath());
		if (fnd2 != mCachedAssetsByPath.end()) {
			mCachedAssetsByPath.erase(fnd2);
		}

		if (cached) {
			mCachedAssets.Remove(cached);
			delete cached;
		}
		else {
			for (auto cache : mCachedAssets)
			{
				if (cache->asset == asset)
				{
					mCachedAssets.Remove(cache);
					break;
				}
			}

			o2Debug.Log("Asset cache not found!");
		}
	}

	Assets::AssetCache* Assets::UpdateAssetCache(Asset* asset, const String& oldPath, const UID& oldUID)
	{
		AssetCache* cached = nullptr;

		auto fnd = mCachedAssetsByUID.find(oldUID);
		if (fnd != mCachedAssetsByUID.end()) {
			cached = fnd->second;
			mCachedAssetsByUID.erase(fnd);
		}

		auto fnd2 = mCachedAssetsByPath.find(oldPath);
		if (fnd2 != mCachedAssetsByPath.end()) {
			mCachedAssetsByPath.erase(fnd2);
		}

		if (cached)
		{
			if (mCachedAssetsByUID.ContainsKey(asset->GetUID()))
				mLog->Error("Duplicated asset cache id: " + (String)asset->GetUID() + " - \"" + asset->GetPath() + "\"");

			mCachedAssetsByUID[asset->GetUID()] = cached;
			mCachedAssetsByPath[asset->GetPath()] = cached;
		}
		else
		{
			o2Debug.LogError("Can't find asset cache: " + (String)asset->GetUID() + " - \"" + asset->GetPath() + "\"");
		}

		return cached;
	}

	Assets::AssetCache::~AssetCache()
	{
		Assert(referencesCount == 0, "Some references not removed for asset");
	}

}
