#pragma once

#include "IAssetConverter.h"

namespace o2
{
	// ----------------
	// Folder converter
	// ----------------
	class FolderAssetConverter: public IAssetConverter
	{
	public:
		// Returns vector of processing assets types
		Vector<const Type*> GetProcessingAssetsTypes() const;

		// Converts folder by path
		void ConvertAsset(const AssetInfo& node);

		// Removes folder by path
		void RemoveAsset(const AssetInfo& node);

		// Moves folder to new path
		void MoveAsset(const AssetInfo& nodeFrom, const AssetInfo& nodeTo);

		// Post processing folders
		Vector<UID> AssetsPostProcess();

		// Resets converter
		void Reset();

		IOBJECT(FolderAssetConverter);

	protected:
		Vector<String> mRemovedFolders; // Removes folders
	};
}

CLASS_BASES_META(o2::FolderAssetConverter)
{
	BASE_CLASS(o2::IAssetConverter);
}
END_META;
CLASS_FIELDS_META(o2::FolderAssetConverter)
{
	FIELD().NAME(mRemovedFolders).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::FolderAssetConverter)
{

	PUBLIC_FUNCTION(Vector<const Type*>, GetProcessingAssetsTypes);
	PUBLIC_FUNCTION(void, ConvertAsset, const AssetInfo&);
	PUBLIC_FUNCTION(void, RemoveAsset, const AssetInfo&);
	PUBLIC_FUNCTION(void, MoveAsset, const AssetInfo&, const AssetInfo&);
	PUBLIC_FUNCTION(Vector<UID>, AssetsPostProcess);
	PUBLIC_FUNCTION(void, Reset);
}
END_META;
