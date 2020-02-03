#include "o2/stdafx.h"
#include "DataAsset.h"

#include "o2/Assets/Assets.h"

namespace o2
{
	DataAsset::DataAsset()
	{}

	DataAsset::DataAsset(const DataAsset& other):
		TAsset(other), data(other.data), meta(this)
	{
		data = other.data;
	}

	DataAsset& DataAsset::operator=(const DataAsset& asset)
	{
		Asset::operator=(asset);
		data = asset.data;

		return *this;
	}

	DataAsset::Meta* DataAsset::GetMeta() const
	{
		return (Meta*)mInfo.meta;
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

DECLARE_CLASS(o2::DataAsset);

DECLARE_CLASS(o2::DataAsset::Meta);
