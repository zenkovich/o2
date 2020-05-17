#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Utils/Serialization/DataValue.h"

namespace o2
{
	// ----------
	// Data asset
	// ----------
	class DataAsset: public AssetWithDefaultMeta<DataAsset>
	{
	public:
		DataValue data; // Asset data

	public:
		// Check equals operator
		DataAsset& operator=(const DataAsset& asset);

		// Returns extensions string
		static const char* GetFileExtensions();

		// Returns editor icon
		static String GetEditorIcon() { return "ui/UI4_big_text_file_icon.png"; }

		// Returns editor sorting weight
		static int GetEditorSorting() { return 94; }

		// Is this asset type is available to create from editor's assets window
		static bool IsAvailableToCreateFromEditor() { return true; }

		SERIALIZABLE(DataAsset);

	protected:
		// Default constructor
		DataAsset();

		// Copy-constructor
		DataAsset(const DataAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		// Saves data
		void SaveData(const String& path) const override;

		friend class Assets;
	};

	typedef Ref<DataAsset> DataAssetRef;
}

CLASS_BASES_META(o2::DataAsset)
{
	BASE_CLASS(o2::AssetWithDefaultMeta<DataAsset>);
}
END_META;
CLASS_FIELDS_META(o2::DataAsset)
{
	PUBLIC_FIELD(data);
}
END_META;
CLASS_METHODS_META(o2::DataAsset)
{

	PUBLIC_STATIC_FUNCTION(const char*, GetFileExtensions);
	PUBLIC_STATIC_FUNCTION(String, GetEditorIcon);
	PUBLIC_STATIC_FUNCTION(int, GetEditorSorting);
	PUBLIC_STATIC_FUNCTION(bool, IsAvailableToCreateFromEditor);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;
