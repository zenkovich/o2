#include "atlas_asset.h"

#include "app/application.h"
#include "assets.h"
#include "config/asset_config.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(AtlasAsset);

AtlasAsset::AtlasAsset():
Asset(), mAttachedToFolder(false)
{
}

AtlasAsset::AtlasAsset(const String& location):
Asset(location)
{
	LoadData();
}

AtlasAsset::AtlasAsset(const FileLocation& location):
Asset(location)
{
	LoadData();
}

AtlasAsset::~AtlasAsset()
{
}

AtlasAsset& AtlasAsset::operator=(const AtlasAsset& asset)
{
	return *this;
}

void AtlasAsset::AddImage(const FileLocation& imageLocation)
{
	foreach(mImages, imgIt)
	if (imgIt->mLocation == imageLocation)
		return;

	mImages.Add(ImageDef(imageLocation, true));
}

bool AtlasAsset::RemoveImage(const FileLocation& imageLocation)
{
	foreach(mImages, imgIt)
	{
		if (imgIt->mLocation == imageLocation)
		{
			if (!imgIt->mLinkedDirectly)
				return false; //because you can't remove image from atlas which linked from attached folder

			mImages.Remove(imgIt);
			return true;
		}
	}

	return false;
}

AtlasAsset::ImagesDefsArr AtlasAsset::GetImagesArray() const
{
	return mImages;
}

void AtlasAsset::SetMaxSize(const Vec2F& maxSize)
{
	mMaxSize = maxSize;
}

Vec2F AtlasAsset::GetMaxSize() const
{
	return mMaxSize;
}

void AtlasAsset::SetName(const String& name)
{
	//TODO: check atlas name!
	mAtlasName = name;
}

String AtlasAsset::GetName() const
{
	return mAtlasName;
}

void AtlasAsset::AttachToFolder(const FileLocation& folderLocation)
{
	mAttachFolder = folderLocation;
	mAttachedToFolder = true;
}

FileLocation AtlasAsset::GetAttachedFolder() const
{
	return mAttachFolder;
}

bool AtlasAsset::IsAttachedToFolder() const
{
	return mAttachedToFolder;
}

void AtlasAsset::DeattachFolder()
{
	mAttachedToFolder = false;
}

void AtlasAsset::LoadData()
{
	//load basic parameters from config
	AtlasAssetConfig* config = static_cast<AtlasAssetConfig*>(AppAssets()->mAssetsConfigs.GetAssetConfig(mLocation));
	mMaxSize = config->mMaxSize;
	mAtlasName = config->mName;
	mAttachedToFolder = config->mAttachedToFolder;
	mAttachFolder = config->mAttachFolderLocation;

	//load images list from built asset
	String buildedAssetPath = AppAssets()->GetAssetFullPath(mLocation);
	Serializer serializer;
	if (serializer.Load(buildedAssetPath))
		serializer.Serialize(mImages, "images");
}

void AtlasAsset::SaveData()
{
	//save config	
	AtlasAssetConfig* config = static_cast<AtlasAssetConfig*>(AppAssets()->mAssetsConfigs.GetAssetConfig(mLocation));
	config->mMaxSize = mMaxSize;
	config->mName = mAtlasName;
	config->mAttachedToFolder = mAttachedToFolder;
	config->mAttachFolderLocation = mAttachFolder;

	foreach(mImages, imgIt)
	{
		if (!imgIt->mLinkedDirectly)
			continue;

		ImageAsset imageAsset(imgIt->mLocation);
		imageAsset.SetAtlasName(mAtlasName);
		imageAsset.Save(imageAsset.GetLocation().mPath, false);
	}
}

SERIALIZE_METHOD_IMPL(AtlasAsset)
{
	SERIALIZE_ID(mAtlasName, "atlasName");
	SERIALIZE_ID(mMaxSize, "maxSize");
	SERIALIZE_ID(mAttachedToFolder, "attachedToFolder");
	SERIALIZE_ID(&mAttachFolder, "attachoFolderLocation");

	return true;
}


AtlasAsset::ImageDef::ImageDef(const FileLocation& location /*= FileLocation()*/, bool linkedDirectly /*= true*/):
mLocation(location), mLinkedDirectly(linkedDirectly)
{
}

bool AtlasAsset::ImageDef::operator=(const ImageDef& other)
{
	return mLocation == other.mLocation && mLinkedDirectly == other.mLinkedDirectly;
}

SERIALIZE_METHOD_IMPL(AtlasAsset::ImageDef)
{
	SERIALIZE_ID(&mLocation, "location");
	SERIALIZE_ID(mLinkedDirectly, "linkedDirectly");
	return true;
}

CLOSE_O2_NAMESPACE