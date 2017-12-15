#include "DataAsset.h"

#include "Assets/Assets.h"

namespace o2
{

	DataAsset::DataAsset():
		Asset()
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	DataAsset::DataAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);
		InitializeProperties();

		Load();
	}

	DataAsset::DataAsset(UID id):
		Asset()
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		Load();
	}

	DataAsset::DataAsset(const DataAsset& asset):
		Asset(asset)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();

		data = asset.data;

		InitializeProperties();
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

	void DataAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(DataAsset, meta, GetMeta);
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

CLASS_META(o2::DataAsset)
{
	BASE_CLASS(o2::Asset);

	PUBLIC_FIELD(data);
	PUBLIC_FIELD(meta);

	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(o2::DataAssetRef)
{
	BASE_CLASS(o2::AssetRef);


	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_META(o2::DataAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);


	PUBLIC_FUNCTION(const Type*, GetAssetType);
}
END_META;
