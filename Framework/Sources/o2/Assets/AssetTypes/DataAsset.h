#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Utils/Serialization/DataNode.h"

namespace o2
{
	// ----------
	// Data asset
	// ----------
	class DataAsset: public TAsset<DataAsset>
	{
	public:
		class Meta;

	public:
		PROPERTIES(DataAsset);
		GETTER(Meta*, meta, GetMeta); // Meta information getter

	public:
		DataNode data; // Asset data

	public:
		// Check equals operator
		DataAsset& operator=(const DataAsset& asset);

		// Returns meta information
		Meta* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const override;

		SERIALIZABLE(DataAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class Meta: public TAssetMeta<DataAsset>
		{
		public:
			SERIALIZABLE(Meta);
		};

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
	BASE_CLASS(o2::TAsset<DataAsset>);
}
END_META;
CLASS_FIELDS_META(o2::DataAsset)
{
	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(data);
}
END_META;
CLASS_METHODS_META(o2::DataAsset)
{

	PUBLIC_FUNCTION(Meta*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;

CLASS_BASES_META(o2::DataAsset::Meta)
{
	BASE_CLASS(o2::TAssetMeta<DataAsset>);
}
END_META;
CLASS_FIELDS_META(o2::DataAsset::Meta)
{
}
END_META;
CLASS_METHODS_META(o2::DataAsset::Meta)
{
}
END_META;
