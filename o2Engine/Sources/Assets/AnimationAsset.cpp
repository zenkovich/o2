#include "AnimationAsset.h"

#include "Assets/Assets.h"

namespace o2
{
	AnimationAsset::AnimationAsset():
		Asset()
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	AnimationAsset::AnimationAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);
		InitializeProperties();

		Load();
	}

	AnimationAsset::AnimationAsset(AssetId id):
		Asset()
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		Load();
	}

	AnimationAsset::AnimationAsset(const AnimationAsset& asset):
		Asset(asset)
	{
		animation = asset.animation;
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();
		InitializeProperties();
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

	void AnimationAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(AnimationAsset, meta, GetMeta);
	}

	Type::Id AnimationAsset::MetaInfo::GetAssetType() const
	{
		return AnimationAsset::type.ID();
	}
}