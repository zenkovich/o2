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
		Vector<Type::Id> GetProcessingAssetsTypes() const;

		// Converts folder by path
		void ConvertAsset(const String& path);

		// Removes folder by path
		void RemoveAsset(const String& path);

		// Moves folder to new path
		void MoveAsset(const String& pathFrom, const String& pathTo);

		// Post processing folders
		void AssetsPostProcess();

		// Resets converter
		void Reset();

		IOBJECT(FolderAssetConverter)
		{
			BASE_CLASS(IAssetConverter);
		}

	protected:
		Vector<String> mRemovedFolders; // Removes folders
	};
}