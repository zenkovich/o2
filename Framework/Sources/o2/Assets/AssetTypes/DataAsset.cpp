#include "o2/stdafx.h"
#include "DataAsset.h"

#include "o2/Assets/Assets.h"

namespace o2
{

	DataAsset::DataAsset():
		Asset()
	{
		mMeta = mnew MetaInfo();
	}

	DataAsset::DataAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		ID() = o2Assets.GetAssetId(path);

		Load();
	}

	DataAsset::DataAsset(const UID& id):
		Asset()
	{
		mMeta = mnew MetaInfo();
		ID() = id;
		mPath = o2Assets.GetAssetPath(id);

		Load();
	}

	DataAsset::DataAsset(const DataAsset& asset):
		Asset(asset), meta(this)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		ID() = asset.GetAssetId();

		data = asset.data;
	}

	DataAsset::~DataAsset()
	{
	}

	DataAsset& DataAsset::operator=(const DataAsset& asset)
	{
		Asset::operator=(asset);

		data = asset.data;

		*mMeta = *(MetaInfo*)(asset.mMeta);

		return *this;
	}

	bool DataAsset::operator==(const DataAsset& other) const
	{
		return mMeta->IsEqual(other.mMeta);
	}

	bool DataAsset::operator!=(const DataAsset& other) const
	{
		return !mMeta->IsEqual(other.mMeta);
	}

	DataAsset::MetaInfo* DataAsset::GetMeta() const
	{
		return (MetaInfo*)mMeta;
	}

	const char* DataAsset::GetFileExtensions() const
	{
		return "xml cfg";
	}

	void DataAsset::LoadData(const String& path)
	{
		data.Clear();
		data.LoadFromFile(path);
	}

	void DataAsset::SaveData(const String& path)
	{
		data.SaveToFile(path);
	}

	const Type* DataAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(DataAsset);
	}

	DataAssetRef DataAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<DataAsset>();
	}

}

DECLARE_CLASS(o2::DataAsset);

DECLARE_CLASS(o2::DataAssetRef);

DECLARE_CLASS(o2::DataAsset::MetaInfo);
