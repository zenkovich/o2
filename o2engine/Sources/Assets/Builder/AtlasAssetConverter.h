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
		Vector<Type::Id> GetProcessingAssetsTypes() const;

		// Converts atlas by path
		void ConvertAsset(const AssetTree::AssetNode& node);

		// Removes atlas by path
		void RemoveAsset(const AssetTree::AssetNode& node);

		// Moves atlas
		void MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo);

		// Post processing atlases. Here checking atlases for rebuild
		Vector<AssetId> AssetsPostProcess();

		// Resets converter
		void Reset();

		IOBJECT(AtlasAssetConverter);

	public:
		// ----------------
		// Atlas image info
		// ----------------
		struct Image: public ISerializable
		{
			AssetId   mId;   // Image asset id @SERIALIZABLE
			TimeStamp mTime; // Image asset edited date @SERIALIZABLE

		public:
			// Default constructor
			Image() {}

			// Constructor
			Image(AssetId id, const TimeStamp& time);

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
		Vector<AssetId> CheckRebuildingAtlases();

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
