#include "AssetConfig.h"

namespace o2
{
	REGIST_TYPE(AssetConfig);

	AssetConfig::AssetConfig():
		mIncludeBuild(true)
	{
	}

	AssetConfig::AssetConfig(const FileLocation& location, bool includeBuild /*= true*/):
		mLocation(location), mIncludeBuild(includeBuild)
	{
	}

	AssetConfig::~AssetConfig()
	{
	}

	AssetConfig* AssetConfig::Clone() const
	{
		return new AssetConfig(*this);
	}


	REGIST_TYPE(ImageAssetConfig);

	ImageAssetConfig::ImageAssetConfig():
		mScale(1.0f)
	{
	}

	ImageAssetConfig::ImageAssetConfig(const FileLocation& location, const String& atlas /*= ""*/, float scale /*= 1.0f*/,
									   bool includeBuild /*= true*/):
									   AssetConfig(location, includeBuild), mAtlas(atlas), mScale(scale)
	{
	}

	AssetConfig* ImageAssetConfig::Clone() const
	{
		return new ImageAssetConfig(*this);
	}


	REGIST_TYPE(AtlasAssetConfig);

	AtlasAssetConfig::AtlasAssetConfig():
		mMaxSize(2048, 2048), mAttachedToFolder(false)
	{
	}

	AtlasAssetConfig::AtlasAssetConfig(const FileLocation& location, const String& name, const Vec2F maxSize /*= vec2f(2048, 2048)*/,
									   bool includeBuild /*= true*/):
									   AssetConfig(location, includeBuild), mName(name), mMaxSize(maxSize), mAttachedToFolder(false)
	{
	}

	AssetConfig* AtlasAssetConfig::Clone() const
	{
		return new AtlasAssetConfig(*this);
	}
}