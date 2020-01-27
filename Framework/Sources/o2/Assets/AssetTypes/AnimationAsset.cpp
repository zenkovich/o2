#include "o2/stdafx.h"
#include "AnimationAsset.h"

#include "o2/Assets/Assets.h"

namespace o2
{
	AnimationAsset::AnimationAsset():
		Asset()
	{
		mMeta = mnew MetaInfo();
	}

	AnimationAsset::AnimationAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		ID() = o2Assets.GetAssetId(path);

		Load();
	}

	AnimationAsset::AnimationAsset(const UID& id):
		Asset()
	{
		mMeta = mnew MetaInfo();
		ID() = id;
		mPath = o2Assets.GetAssetPath(id);

		Load();
	}

	AnimationAsset::AnimationAsset(const AnimationAsset& asset):
		Asset(asset), meta(this)
	{
		animation = asset.animation;
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		ID() = asset.GetUID();
	}

	AnimationAsset::~AnimationAsset()
	{}

	AnimationAsset& AnimationAsset::operator=(const AnimationAsset& asset)
	{
		Asset::operator=(asset);
		animation = asset.animation;

		*mMeta = *(MetaInfo*)(asset.mMeta);

		return *this;
	}

	bool AnimationAsset::operator==(const AnimationAsset& other) const
	{
		return mMeta->IsEqual(other.mMeta);
	}

	bool AnimationAsset::operator!=(const AnimationAsset& other) const
	{
		return !mMeta->IsEqual(other.mMeta);
	}

	AnimationAsset::MetaInfo* AnimationAsset::GetMeta() const
	{
		return (MetaInfo*)mMeta;
	}

	const char* AnimationAsset::GetFileExtensions() const
	{
		return "anim anm";
	}

	void AnimationAsset::LoadData(const String& path)
	{
		DataNode data;
		data.LoadFromFile(path);
		animation = data;
	}

	void AnimationAsset::SaveData(const String& path)
	{
		DataNode data;
		data = animation;
		data.SaveToFile(path);
	}

	const Type* AnimationAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(AnimationAsset);
	}

	AnimationAssetRef AnimationAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<AnimationAsset>();
	}

}

DECLARE_CLASS(o2::AnimationAsset);

DECLARE_CLASS(o2::AnimationAssetRef);

DECLARE_CLASS(o2::AnimationAsset::MetaInfo);
