#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/CustomRTTI.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/Serialization.h"

namespace o2
{
	class AssetConfig: public ISerializable
	{
	public:
		DEFINE_TYPE(AssetConfig);

		FileLocation mLocation;
		bool         mIncludeBuild;

	public:
		AssetConfig();
		AssetConfig(const FileLocation& location, bool includeBuild = true);
		virtual ~AssetConfig();

		virtual AssetConfig* Clone() const;

		SERIALIZABLE_IMPL(AssetConfig);

		FIELDS()
		{
			FIELD(mLocation)     SERIALIZABLE;
			FIELD(mIncludeBuild) SERIALIZABLE;
		}
		/*SERIALIZABLE_FIELDS(AssetConfig)
			FIELD(mLocation)
			FIELD(mIncludeBuild);
		END_SERIALIZABLE_FIELDS;*/
	};
	typedef Vector<AssetConfig*> AssetsConfigsVec;


	class ImageAssetConfig: public AssetConfig
	{
	public:
		DEFINE_TYPE(ImageAssetConfig);

		float  mScale;
		String mAtlas;

	public:
		ImageAssetConfig();
		ImageAssetConfig(const FileLocation& location, const String& atlas = "", float scale = 1.0f, bool includeBuild = true);

		virtual AssetConfig* Clone() const;

		/*SERIALIZABLE_FIELDS_INHERITED(ImageAssetConfig, AssetConfig)
			FIELD(mScale)
			FIELD(mAtlas);
		END_SERIALIZABLE_FIELDS;*/
	};


	class AtlasAssetConfig: public AssetConfig
	{
	public:
		DEFINE_TYPE(AtlasAssetConfig);

		Vec2F        mMaxSize;
		String       mName;
		bool         mAttachedToFolder;
		FileLocation mAttachFolderLocation;

	public:
		AtlasAssetConfig();
		AtlasAssetConfig(const FileLocation& location, const String& name, const Vec2F maxSize = Vec2F(2048, 2048),
						 bool includeBuild = true);

		virtual AssetConfig* Clone() const;

		/*SERIALIZABLE_FIELDS_INHERITED(AtlasAssetConfig, AssetConfig)
			FIELD(mMaxSize)
			FIELD(mAttachedToFolder)
			FIELD(mAttachFolderLocation)
			FIELD(mName);
		END_SERIALIZABLE_FIELDS;*/
	};
}