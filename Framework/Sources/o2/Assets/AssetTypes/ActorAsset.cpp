#include "o2/stdafx.h"
#include "ActorAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Scene/Actor.h"

namespace o2
{
	ActorAsset::ActorAsset():
		Asset()
	{
		mMeta = mnew MetaInfo();

		mActor = mnew Actor(ActorCreateMode::NotInScene);
		mActor->mIsAsset = true;
		mActor->mAssetId = ID();
	}

	ActorAsset::ActorAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		ID() = o2Assets.GetAssetId(path);

		mActor = mnew Actor(ActorCreateMode::NotInScene);
		mActor->mIsAsset = true;
		mActor->mAssetId = ID();

		Load();
	}

	ActorAsset::ActorAsset(const UID& id):
		Asset()
	{
		mMeta = mnew MetaInfo();
		ID() = id;
		mPath = o2Assets.GetAssetPath(id);

		mActor = mnew Actor(ActorCreateMode::NotInScene);
		mActor->mIsAsset = true;
		mActor->mAssetId = ID();

		Load();
	}

	ActorAsset::ActorAsset(const ActorAsset& asset):
		Asset(asset), meta(this)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		ID() = asset.GetUID();

		mActor = mnew Actor(ActorCreateMode::NotInScene);
		mActor->mIsAsset = true;
		mActor->mAssetId = ID();
		*mActor = *asset.mActor;
	}

	ActorAsset::~ActorAsset()
	{}

	ActorAsset& ActorAsset::operator=(const ActorAsset& asset)
	{
		Asset::operator=(asset);

		*mActor = *asset.mActor;
		mActor->mIsAsset = true;
		mActor->mAssetId = ID();

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
		return "proto prt actor act";
	}

	Actor* ActorAsset::GetActor() const 
	{
		return mActor;
	}

	void ActorAsset::LoadData(const String& path)
	{
		DataNode data;
		data.LoadFromFile(path);
		mActor->Deserialize(data);
		mActor->mIsAsset = true;
		mActor->mAssetId = ID();
	}

	void ActorAsset::SaveData(const String& path)
	{
		mActor->mIsAsset = true;
		mActor->mAssetId = ID();

		DataNode data;
		data = mActor->Serialize();
		data.SaveToFile(path);
	}

	const Type* ActorAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(ActorAsset);
	}

	ActorAssetRef ActorAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<ActorAsset>();
	}

}

DECLARE_CLASS(o2::ActorAsset);

DECLARE_CLASS(o2::ActorAssetRef);

DECLARE_CLASS(o2::ActorAsset::MetaInfo);
