#include "ActorAsset.h"

#include "Assets/Assets.h"

namespace o2
{
	ActorAsset::ActorAsset():
		Asset()
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();

		actor.ExcludeFromScene();
		actor.mIsAsset = true;
		actor.mAssetId = IdRef();
	}

	ActorAsset::ActorAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);
		InitializeProperties();

		actor.ExcludeFromScene();
		actor.mIsAsset = true;
		actor.mAssetId = IdRef();

		Load();
	}

	ActorAsset::ActorAsset(AssetId id):
		Asset()
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		actor.ExcludeFromScene();
		actor.mIsAsset = true;
		actor.mAssetId = IdRef();

		Load();
	}

	ActorAsset::ActorAsset(const ActorAsset& asset):
		Asset(asset)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();

		actor = asset.actor;
		actor.ExcludeFromScene();
		actor.mIsAsset = true;
		actor.mAssetId = IdRef();

		InitializeProperties();
	}

	ActorAsset::~ActorAsset()
	{}

	ActorAsset& ActorAsset::operator=(const ActorAsset& asset)
	{
		Asset::operator=(asset);

		actor = asset.actor;
		actor.mIsAsset = true;
		actor.mAssetId = IdRef();

		*mMeta = *(MetaInfo*)(asset.mMeta);

		return *this;
	}

	bool ActorAsset::operator==(const ActorAsset& other) const
	{
		return mMeta->IsEqual(other.mMeta);
	}

	bool ActorAsset::operator!=(const ActorAsset& other) const
	{
		return !mMeta->IsEqual(other.mMeta);
	}

	ActorAsset::MetaInfo* ActorAsset::GetMeta() const
	{
		return (MetaInfo*)mMeta;
	}

	const char* ActorAsset::GetFileExtensions() const
	{
		return "prefab prf actor act";
	}

	void ActorAsset::LoadData(const String& path)
	{
		DataNode data;
		data.LoadFromFile(path);
		actor = data;
		actor.ExcludeFromScene();
		actor.mIsAsset = true;
		actor.mAssetId = IdRef();
	}

	void ActorAsset::SaveData(const String& path)
	{
		actor.mIsAsset = true;
		actor.mAssetId = IdRef();

		DataNode data;
		data = actor;
		data.SaveToFile(path);
	}

	void ActorAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(ActorAsset, meta, GetMeta);
	}

	Type::Id ActorAsset::MetaInfo::GetAssetType() const
	{
		return ActorAsset::type.ID();
	}
}