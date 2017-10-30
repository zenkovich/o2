#pragma once

#include "IAssetConverter.h"
#include "Assets/Builder/AssetsBuilder.h"
#include "Utils/RectPacker.h"

namespace o2
{
	class Bitmap;

	// -----------------
	// Atlases converter
	// -----------------
	class AtlasAssetConverter: public IAssetConverter
	{
	public:
		// Returns vector of processing assets types
		Vector<const Type*> GetProcessingAssetsTypes() const;

		// Converts atlas by path
		void ConvertAsset(const AssetTree::AssetNode& node);

		// Removes atlas by path
		void RemoveAsset(const AssetTree::AssetNode& node);

		// Moves atlas
		void MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo);

		// Post processing atlases. Here checking atlases for rebuild
		Vector<UID> AssetsPostProcess();

		// Resets converter
		void Reset();

		IOBJECT(AtlasAssetConverter);

	public:
		// ----------------
		// Atlas image info
		// ----------------
		struct Image: public ISerializable
		{
			UID       id;   // Image asset id @SERIALIZABLE
			TimeStamp time; // Image asset edited date @SERIALIZABLE

		public:
			// Default constructor
			Image() {}

			// Constructor
			Image(UID id, const TimeStamp& time);

			// Check equal operator
			bool operator==(const Image& other) const;

			SERIALIZABLE(Image);
		};
		typedef Vector<Image> ImagesVec;

		// ------------------------
		// Image packing definition
		// ------------------------
		struct ImagePackDef
		{
			Bitmap*               mBitmap = nullptr;    // Image bitmap pointer
			RectsPacker::Rect*    mPackRect = nullptr;  // Image pack rectangle pointer
			AssetTree::AssetNode* mAssetInfo = nullptr; // Asset information

			// Check equal operator
			bool operator==(const ImagePackDef& other) const;
		};
		typedef Vector<ImagePackDef> ImagePackDefsVec;

	protected:
		// Checks images for attaching to base atlas
		void CheckBasicAtlas();

		// Checks atlases for rebuilding
		Vector<UID> CheckRebuildingAtlases();

		// Checks atlas for rebuilding
		bool CheckAtlasRebuilding(AssetTree::AssetNode* atlasInfo);

		// Returns true if atlas needs to rebuild
		bool IsAtlasNeedRebuild(ImagesVec& currentImages, ImagesVec& lastImages);

		// Rebuilds atlas
		void RebuildAtlas(AssetTree::AssetNode* atlasInfo, ImagesVec& images);

		// Saves image asset data
		void SaveImageAsset(ImagePackDef& imgDef);
	};
}

CLASS_BASES_META(o2::AtlasAssetConverter)
{
	BASE_CLASS(o2::IAssetConverter);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAssetConverter)
{
}
END_META;
CLASS_METHODS_META(o2::AtlasAssetConverter)
{

	PUBLIC_FUNCTION(Vector<const Type*>, GetProcessingAssetsTypes);
	PUBLIC_FUNCTION(void, ConvertAsset, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(void, RemoveAsset, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(void, MoveAsset, const AssetTree::AssetNode&, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(Vector<UID>, AssetsPostProcess);
	PUBLIC_FUNCTION(void, Reset);
	PROTECTED_FUNCTION(void, CheckBasicAtlas);
	PROTECTED_FUNCTION(Vector<UID>, CheckRebuildingAtlases);
	PROTECTED_FUNCTION(bool, CheckAtlasRebuilding, AssetTree::AssetNode*);
	PROTECTED_FUNCTION(bool, IsAtlasNeedRebuild, ImagesVec&, ImagesVec&);
	PROTECTED_FUNCTION(void, RebuildAtlas, AssetTree::AssetNode*, ImagesVec&);
	PROTECTED_FUNCTION(void, SaveImageAsset, ImagePackDef&);
}
END_META;

CLASS_BASES_META(o2::AtlasAssetConverter::Image)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAssetConverter::Image)
{
	PUBLIC_FIELD(id).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(time).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AtlasAssetConverter::Image)
{
}
END_META;
