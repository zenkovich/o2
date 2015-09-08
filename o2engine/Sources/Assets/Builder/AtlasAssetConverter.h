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
		void ConvertAsset(const String& path);

		// Removes atlas by path
		void RemoveAsset(const String& path);

		// Moves atlas
		void MoveAsset(const String& pathFrom, const String& pathTo);

		// Post processing atlases. Here checking atlases for rebuild
		void AssetsPostProcess();

		// Resets converter
		void Reset();

		IOBJECT(AtlasAssetConverter)
		{
			BASE_CLASS(IAssetConverter);
		}

	public:
		// ----------------
		// Atlas image info
		// ----------------
		struct Image: public ISerializable
		{
			UInt      mId;   // Image asset id
			TimeStamp mTime; // Image asset edited date

		public:
			// Default constructor
			Image() {}

			// Constructor
			Image(UInt id, const TimeStamp& time);

			// Check equal operator
			bool operator==(const Image& other) const;

			SERIALIZABLE_IMPL(Image);
			IOBJECT(Image)
			{
				SRLZ_FIELD(mId);
				SRLZ_FIELD(mTime);
			}
		};
		typedef Vector<Image> ImagesVec;

		// ------------------------
		// Image packing definition
		// ------------------------
		struct ImagePackDef
		{
			Ptr<Bitmap>               mBitmap;    // Image bitmap pointer
			Ptr<RectsPacker::Rect>    mPackRect;  // Image pack rectangle pointer
			Ptr<AssetTree::AssetNode> mAssetInfo; // Asset information

			// Check equal operator
			bool operator==(const ImagePackDef& other) const;
		};
		typedef Vector<ImagePackDef> ImagePackDefsVec;

	protected:
		// Checks images for attaching to base atlas
		void CheckBasicAtlas();

		// Checks atlases for rebuilding
		void CheckRebuildingAtlases();

		// Checks atlas for rebuilding
		void CheckAtlasRebuilding(Ptr<AssetTree::AssetNode> atlasInfo);

		// Returns true if atlas needs to rebuild
		bool IsAtlasNeedRebuild(ImagesVec& currentImages, ImagesVec& lastImages);

		// Rebuilds atlas
		void RebuildAtlas(Ptr<AssetTree::AssetNode> atlasInfo, ImagesVec& images);

		// Saves image asset data
		void SaveImageAsset(ImagePackDef& imgDef);
	};
}
