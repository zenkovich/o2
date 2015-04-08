#pragma once

#include "asset.h"

OPEN_O2_NAMESPACE

class Bitmap;

class ImageAsset: public Asset, public Serializable
{
	DEFINE_TYPE(ImageAsset);

protected:
	Bitmap* mImage;
	String  mAtlas;

public:
	ImageAsset();
	ImageAsset(const String& location);
	ImageAsset(const FileLocation& location);
	ImageAsset(Bitmap* image);
	~ImageAsset();

	ImageAsset& operator=(const ImageAsset& asset);

	Bitmap* GetImage();
	void SetImage(Bitmap* image);

	void SetAtlasName(const String& atlasName);

	SERIALIZBLE_METHODS(ImageAsset);

protected:
	void LoadData();
	void SaveData();
	void LoadImage();
};

CLOSE_O2_NAMESPACE
