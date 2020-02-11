#include "o2/stdafx.h"
#include "DataAsset.h"

#include "o2/Assets/Assets.h"

namespace o2
{
	DataAsset::DataAsset()
	{}

	DataAsset::DataAsset(const DataAsset& other):
		AssetWithDefaultMeta<DataAsset>(other), data(other.data)
	{
		data = other.data;
	}

	DataAsset& DataAsset::operator=(const DataAsset& asset)
	{
		Asset::operator=(asset);
		data = asset.data;

		return *this;
	}

	const char* DataAsset::GetFileExtensions() const
	{
		return "xml";
	}

	void DataAsset::LoadData(const String& path)
	{
		data.Clear();
		data.LoadFromFile(path);
	}

	void DataAsset::SaveData(const String& path) const
	{
		data.SaveToFile(path);
	}
}

DECLARE_CLASS_MANUAL(o2::AssetWithDefaultMeta<o2::DataAsset>);
DECLARE_CLASS_MANUAL(o2::DefaultAssetMeta<o2::DataAsset>);
DECLARE_CLASS_MANUAL(o2::Ref<o2::DataAsset>);

DECLARE_CLASS(o2::DataAsset);
