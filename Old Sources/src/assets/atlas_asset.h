#pragma once

#include "asset.h"
#include "image_asset.h"

OPEN_O2_NAMESPACE

class AtlasAsset: public Asset, public Serializable
{
	DEFINE_TYPE(AtlasAsset);

public:
	struct ImageDef: public Serializable
	{
		FileLocation mLocation;
		bool         mLinkedDirectly;

		ImageDef(const FileLocation& location = FileLocation(), bool linkedDirectly = true);

		SERIALIZBLE_METHODS(ImageDef);

		bool operator=(const ImageDef& other);
	};
	typedef Array<ImageDef> ImagesDefsArr;

protected:
	String        mAtlasName;
	Vec2F         mMaxSize;
	bool          mAttachedToFolder;
	FileLocation  mAttachFolder;
	ImagesDefsArr mImages;

public:
	AtlasAsset();
	AtlasAsset(const String& location);
	AtlasAsset(const FileLocation& location);
	~AtlasAsset();

	void AddImage(const FileLocation& imageLocation);
	bool RemoveImage(const FileLocation& imageLocation);
	ImagesDefsArr GetImagesArray() const;

	void SetMaxSize(const Vec2F& maxSize);
	Vec2F GetMaxSize() const;

	void SetName(const String& name);
	String GetName() const;

	void AttachToFolder(const FileLocation& folderLocation);
	FileLocation GetAttachedFolder() const;
	bool IsAttachedToFolder() const;
	void DeattachFolder();

	SERIALIZBLE_METHODS(AtlasAsset);

protected:
	void LoadData();
	void SaveData();

	AtlasAsset& operator=(const AtlasAsset& asset);
};

CLOSE_O2_NAMESPACE
