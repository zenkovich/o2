#include "asset_file_info.h"

#include "app/application.h"
#include "assets/assets.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(AssetBuildInfo);

SERIALIZE_METHOD_IMPL(AssetBuildInfo)
{
	SERIALIZE_ID(&mLocation, "location");
	SERIALIZE_ID(mWriteTime, "writeTime");

	return true;
}

AssetConfig* AssetBuildInfo::GetConfigsSample() const
{
	AssetConfig* res = mnew AssetConfig();
	res->mLocation = mLocation;
	return res;
}

bool AssetBuildInfo::IsEquals(AssetBuildInfo* other)
{
	return mLocation.mId == other->mLocation.mId && mLocation.mPath == other->mLocation.mPath &&
		mWriteTime == other->mWriteTime;
}

void AssetBuildInfo::CopyFrom(const AssetBuildInfo* other)
{
	mLocation = other->mLocation;
	mWriteTime = other->mWriteTime;
}

AssetBuildInfo* AssetBuildInfo::Clone() const
{
	AssetBuildInfo* res = mnew AssetBuildInfo();
	res->CopyFrom(this);
	return res;
}

REGIST_TYPE(ImageAssetBuildInfo);

SERIALIZE_INHERITED_METHOD_IMPL(ImageAssetBuildInfo)
{
	SERIALIZE_ID(mAtlasName, "atlas");
	SERIALIZE_ID(mScale, "scale");

	return true;
}

void ImageAssetBuildInfo::InitFromConfigs(AssetConfig* config)
{
	ImageAssetConfig* imgConfig = dynamic_cast<ImageAssetConfig*>(config);

	if (imgConfig == NULL)
	{
		LogError("Failed to get image configs: %s. Incorrect config type!", mLocation.mPath);
		return;
	}

	mAtlasName = imgConfig->mAtlas;
	mScale = imgConfig->mScale;
}

AssetConfig* ImageAssetBuildInfo::GetConfigsSample() const
{
	ImageAssetConfig* res = mnew ImageAssetConfig();
	res->mLocation = mLocation;
	res->mAtlas = mAtlasName;
	res->mScale = mScale;
	return res;
}

bool ImageAssetBuildInfo::IsEquals(AssetBuildInfo* other)
{
	if (other->getType() != getType())
		return false;

	ImageAssetBuildInfo* imgOther = static_cast<ImageAssetBuildInfo*>(other);

	return AssetBuildInfo::IsEquals(other) && mAtlasName == imgOther->mAtlasName && mScale == imgOther->mScale;
}

void ImageAssetBuildInfo::CopyFrom(const AssetBuildInfo* other)
{
	AssetBuildInfo::CopyFrom(other);

	if (other->getType() != getType())
		return;

	const ImageAssetBuildInfo* imgOther = static_cast<const ImageAssetBuildInfo*>(other);

	mAtlasName = imgOther->mAtlasName;
	mScale = imgOther->mScale;
}

ImageAssetBuildInfo::ImageAssetBuildInfo():
mScale(1.0f), mAtlas(NULL)
{
}

AssetBuildInfo* ImageAssetBuildInfo::Clone() const
{
	AssetBuildInfo* res = mnew ImageAssetBuildInfo();
	res->CopyFrom(this);
	return res;
}


REGIST_TYPE(AtlasAssetBuildInfo);

AtlasAssetBuildInfo::AtlasAssetBuildInfo():
mMaxSize(2048.0f, 2048.0f), mName("unnamed"), mAttachedToFolder(false), mAttachFolder(NULL)
{
}

void AtlasAssetBuildInfo::InitFromConfigs(AssetConfig* config)
{
	AtlasAssetConfig* atlConfig = dynamic_cast<AtlasAssetConfig*>(config);

	if (atlConfig == NULL)
	{
		LogError("Failed to get atlas configs: %s. Incorrect config type!", mLocation.mPath);
		return;
	}

	mName = atlConfig->mName;
	mMaxSize = atlConfig->mMaxSize;
	mAttachedToFolder = atlConfig->mAttachedToFolder;
	mAttachFolderLocation = atlConfig->mAttachFolderLocation;
}

AssetConfig* AtlasAssetBuildInfo::GetConfigsSample() const
{
	AtlasAssetConfig* res = mnew AtlasAssetConfig();
	res->mLocation = mLocation;
	res->mName = mName;
	res->mMaxSize= mMaxSize;
	res->mAttachedToFolder = mAttachedToFolder;
	res->mAttachFolderLocation = mAttachFolderLocation;
	return res;
}

bool AtlasAssetBuildInfo::IsEquals(AssetBuildInfo* other)
{
	if (other->GetType() != GetType())
		return false;

	AtlasAssetBuildInfo* atlOther = static_cast<AtlasAssetBuildInfo*>(other);

	if (!(AssetBuildInfo::IsEquals(other) && mName == atlOther->mName && mMaxSize == atlOther->mMaxSize &&
		mImages.Count() == atlOther->mImages.Count()))
		return false;

	foreach(mImages, imgIt)
	{
		ImageAssetBuildInfo* otherImg = NULL;
		foreach(atlOther->mImages, imgIt2)
		{
			if ((*imgIt)->mLocation == (*imgIt2)->mLocation)
			{
				otherImg = *imgIt2;
				break;
			}
		}

		if (!otherImg)
			return false;

		if (!otherImg->IsEquals(*imgIt))
			return false;
	}

	return true;
}

void AtlasAssetBuildInfo::CopyFrom(const AssetBuildInfo* other)
{
	AssetBuildInfo::CopyFrom(other);

	if (other->GetType() != GetType())
		return;

	const AtlasAssetBuildInfo* atlOther = static_cast<const AtlasAssetBuildInfo*>(other);

	mMaxSize = atlOther->mMaxSize;
	mName = atlOther->mName;
	mAttachedToFolder = atlOther->mAttachedToFolder;
	mAttachFolderLocation = atlOther->mAttachFolderLocation;
}

AssetBuildInfo* AtlasAssetBuildInfo::Clone() const
{
	AssetBuildInfo* res = mnew AtlasAssetBuildInfo();
	res->CopyFrom(this);
	return res;
}

SERIALIZE_INHERITED_METHOD_IMPL(AtlasAssetBuildInfo)
{
	SERIALIZE_ID(mName, "name");
	SERIALIZE_ID(mMaxSize, "maxSize");
	SERIALIZE_ID(mAttachedToFolder, "attachedToFolder");
	SERIALIZE_ID(&mAttachFolderLocation, "attachoFolderLocation");

	return true;
}


REGIST_TYPE(FolderAssetBuildInfo);

FolderAssetBuildInfo::FolderAssetBuildInfo():
mAttachedAtlas(NULL), mParentFolder(NULL)
{
}

FolderAssetBuildInfo::~FolderAssetBuildInfo()
{
	Clear();
}

void FolderAssetBuildInfo::Clear()
{
	ReleaseArray(mInsideAssets);
}

SERIALIZE_INHERITED_METHOD_IMPL(FolderAssetBuildInfo)
{
	SERIALIZE_ID(mInsideAssets, "insideAssets");

	return true;
}

AssetConfig* FolderAssetBuildInfo::GetConfigsSample() const
{
	AssetConfig* res = mnew AssetConfig();
	res->mLocation = mLocation;
	return res;
}

AssetBuildInfo* FolderAssetBuildInfo::GetInsideAsset(const FileLocation& location, bool recursive /*= false*/)
{
	if (location == FileLocation())
		return this;

	foreach(mInsideAssets, assetIt)
	{
		if ((*assetIt)->mLocation == location)
			return *assetIt;
	}

	if (!recursive)
		return NULL;


	foreach(mInsideAssets, assetIt)
	{
		if ((*assetIt)->GetType() == FolderAssetBuildInfo::GetStaticType())
		{
			AssetBuildInfo* res = static_cast<FolderAssetBuildInfo*>(*assetIt)->GetInsideAsset(location, true);
			if (res)
				return res;
		}
	}

	return NULL;
}

AssetBuildInfo* FolderAssetBuildInfo::Clone() const
{
	AssetBuildInfo* res = mnew FolderAssetBuildInfo();
	res->CopyFrom(this);
	return res;
}

void FolderAssetBuildInfo::AddInsideAsset(AssetBuildInfo* asset)
{
	if (asset->getType() == FolderAssetBuildInfo::getStaticType())
		static_cast<FolderAssetBuildInfo*>(asset)->mParentFolder = this;

	mInsideAssets.add(asset);
}

AssetsBuildInfosArr FolderAssetBuildInfo::GetAllInsideAssets() const
{
	AssetsBuildInfosArr res;
	foreach_const(mInsideAssets, assetIt)
	{
		res.add(*assetIt);
		if ((*assetIt)->GetType() == FolderAssetBuildInfo::getStaticType())
			res.add((static_cast<FolderAssetBuildInfo*>(*assetIt))->GetAllInsideAssets());
	}

	return res;
}

void FolderAssetBuildInfo::LinkAtlases()
{
	LinkChildFolders(NULL);

	AtlasAssetsBuildInfosArr allAtlases;
	ImageAssetsBuildInfosArr allImages;
	AssetsBuildInfosArr insideAssets = GetAllInsideAssets();

	mAttachedAtlas = NULL;

	foreach(AssetsBuildInfosArr, insideAssets, assetIt)
	{
		if ((*assetIt)->GetType() == AtlasAssetBuildInfo::getStaticType())
		{
			AtlasAssetBuildInfo* atlas = static_cast<AtlasAssetBuildInfo*>(*assetIt);
			atlas->mAttachFolder = NULL;
			allAtlases.add(atlas);
		}

		if ((*assetIt)->GetType() == ImageAssetBuildInfo::getStaticType())
		{
			ImageAssetBuildInfo* image = static_cast<ImageAssetBuildInfo*>(*assetIt);
			image->mAtlas = NULL;
			allImages.add(image);
		}

		if ((*assetIt)->GetType() == FolderAssetBuildInfo::getStaticType())
		{
			FolderAssetBuildInfo* folder = static_cast<FolderAssetBuildInfo*>(*assetIt);
			folder->mAttachedAtlas = NULL;
		}
	}

	foreach(AtlasAssetsBuildInfosArr, allAtlases, atlasIt)
	{
		if ((*atlasIt)->mAttachedToFolder)
		{
			FolderAssetBuildInfo* folder = static_cast<FolderAssetBuildInfo*>(GetInsideAsset((*atlasIt)->mAttachFolderLocation, true));
			(*atlasIt)->mAttachFolder = folder;
			folder->mAttachedAtlas = *atlasIt;
		}
	}

	foreach(ImageAssetsBuildInfosArr, allImages, imageIt)
	{
		if (!(*imageIt)->mAtlasName.empty())
		{
			foreach(AtlasAssetsBuildInfosArr, allAtlases, atlasIt)
			{
				if ((*atlasIt)->mName == (*imageIt)->mAtlasName)
				{
					(*imageIt)->mAtlas = *atlasIt;
					break;
				}
			}
		}
	}

	LinkImages();
}

void FolderAssetBuildInfo::LinkImages()
{
	AtlasAssetBuildInfo* upAtlas = NULL;
	FolderAssetBuildInfo* upAtlasFolder = this;
	while (upAtlas == NULL && upAtlasFolder != NULL)
	{
		if (upAtlasFolder->mAttachedAtlas)
		{
			upAtlas = upAtlasFolder->mAttachedAtlas;
			break;
		}

		upAtlasFolder = upAtlasFolder->mParentFolder;
	}

	if (!upAtlas)
		return;

	foreach(AssetsBuildInfosArr, mInsideAssets, assetIt)
	{
		if ((*assetIt)->GetType() == ImageAssetBuildInfo::getStaticType())
		{
			ImageAssetBuildInfo* image = static_cast<ImageAssetBuildInfo*>(*assetIt);
			if (image->mAtlasName.empty())
			{
				image->mAtlas = upAtlas;
				upAtlas->mImages.add(image);
			}
		}

		if ((*assetIt)->GetType() == FolderAssetBuildInfo::getStaticType())
			static_cast<FolderAssetBuildInfo*>(*assetIt)->LinkImages();
	}
}

void FolderAssetBuildInfo::LinkChildFolders(FolderAssetBuildInfo* parentFolder)
{
	mParentFolder = parentFolder;

	foreach(AssetsBuildInfosArr, mInsideAssets, assetIt)
	{
		if ((*assetIt)->GetType() == FolderAssetBuildInfo::getStaticType())
			static_cast<FolderAssetBuildInfo*>(*assetIt)->LinkChildFolders(this);
	}
}

CLOSE_O2_NAMESPACE