#pragma once

#include "IAssetConverter.h"
#include "o2/Assets/Builder/AssetsBuilder.h"
#include "o2/Utils/Tools/RectPacker.h"

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
		void ConvertAsset(const AssetsTree::AssetInfo& node);

		// Removes atlas by path
		void RemoveAsset(const AssetsTree::AssetInfo& node);

		// Moves atlas
		void MoveAsset(const AssetsTree::AssetInfo& nodeFrom, const AssetsTree::AssetInfo& nodeTo);

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
			Image(const UID& id, const TimeStamp& time);

			// Check equal operator
			bool operator==(const Image& other) const;

			SERIALIZABLE(Image);
		};

		// ------------------------
		// Image packing definition
		// ------------------------
		struct ImagePackDef
		{
			Bitmap*                bitmap = nullptr;    // Image bitmap pointer
			RectsPacker::Rect*     packRect = nullptr;  // Image pack rectangle pointer
			AssetsTree::AssetInfo* assetInfo = nullptr; // Asset information

			// Check equal operator
			bool operator==(const ImagePackDef& other) const;
		};

	protected:
		// Checks images for attaching to base atlas
		void CheckBasicAtlas();

		// Checks atlases for rebuilding
		Vector<UID> CheckRebuildingAtlases();

		// Checks atlas for rebuilding
		bool CheckAtlasRebuilding(AssetsTree::AssetInfo* atlasInfo);

		// Returns true if atlas needs to rebuild
		bool IsAtlasNeedRebuild(Vector<Image>& currentImages, Vector<Image>& lastImages);

		// Rebuilds atlas
		void RebuildAtlas(AssetsTree::AssetInfo* atlasInfo, Vector<Image>& images);

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
	PUBLIC_FUNCTION(void, ConvertAsset, const AssetsTree::AssetInfo&);
	PUBLIC_FUNCTION(void, RemoveAsset, const AssetsTree::AssetInfo&);
	PUBLIC_FUNCTION(void, MoveAsset, const AssetsTree::AssetInfo&, const AssetsTree::AssetInfo&);
	PUBLIC_FUNCTION(Vector<UID>, AssetsPostProcess);
	PUBLIC_FUNCTION(void, Reset);
	PROTECTED_FUNCTION(void, CheckBasicAtlas);
	PROTECTED_FUNCTION(Vector<UID>, CheckRebuildingAtlases);
	PROTECTED_FUNCTION(bool, CheckAtlasRebuilding, AssetsTree::AssetInfo*);
	PROTECTED_FUNCTION(bool, IsAtlasNeedRebuild, Vector<Image>&, Vector<Image>&);
	PROTECTED_FUNCTION(void, RebuildAtlas, AssetsTree::AssetInfo*, Vector<Image>&);
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
