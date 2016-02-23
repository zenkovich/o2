#include "ActorAsset.h"

#include "Assets/Assets.h"

namespace o2
{
	ActorAsset::ActorAsset():
		Asset()
	{
		actor.ExcludeFromScene();
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	ActorAsset::ActorAsset(const String& path):
		Asset()
	{
		actor.ExcludeFromScene();
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);
		InitializeProperties();

		Load();
	}

	ActorAsset::ActorAsset(AssetId id):
		Asset()
	{
		actor.ExcludeFromScene();
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		Load();
	}

	ActorAsset::ActorAsset(const ActorAsset& asset):
		Asset(asset)
	{
		actor = asset.actor;
		actor.ExcludeFromScene();
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	ActorAsset::~ActorAsset()
	{}

	ActorAsset& ActorAsset::operator=(const ActorAsset& asset)
	{
		Asset::operator=(asset);
		actor = asset.actor;

		return *this;
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
	}

	void ActorAsset::SaveData(const String& path)
	{
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