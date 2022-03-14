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
		DataDocument data; // Asset data

	public:
		// Default constructor
		DataAsset();

		// Copy-constructor
		DataAsset(const DataAsset& asset);

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
	FIELD().PUBLIC().NAME(data);
}
END_META;
CLASS_METHODS_META(o2::DataAsset)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const DataAsset&);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const char*, GetFileExtensions);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetEditorIcon);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(int, GetEditorSorting);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, IsAvailableToCreateFromEditor);
	FUNCTION().PROTECTED().SIGNATURE(void, LoadData, const String&);
	FUNCTION().PROTECTED().SIGNATURE(void, SaveData, const String&);
}
END_META;
