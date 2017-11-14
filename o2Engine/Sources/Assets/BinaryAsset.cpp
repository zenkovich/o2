#include "BinaryAsset.h"

#include "Assets/Assets.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	const Type* BinaryAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(BinaryAsset);
	}

	BinaryAsset::BinaryAsset():
		Asset(), mData(nullptr), mDataSize(0)
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	BinaryAsset::BinaryAsset(const String& path):
		Asset(), mData(nullptr), mDataSize(0)
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);
		InitializeProperties();

		Load();
	}

	BinaryAsset::BinaryAsset(UID id):
		Asset(), mData(nullptr), mDataSize(0)
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		Load();
	}

	BinaryAsset::BinaryAsset(const BinaryAsset& asset):
		Asset(asset)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();

		if (asset.mDataSize > 0)
		{
			mDataSize = asset.mDataSize;
			mData = new char[mDataSize];
			memcpy(mData, asset.mData, mDataSize);
		}
		else
		{
			mDataSize = 0;
			mData = nullptr;
		}

		InitializeProperties();
	}

	BinaryAsset::~BinaryAsset()
	{
		if (mData)
			delete[] mData;
	}

	BinaryAsset& BinaryAsset::operator=(const BinaryAsset& asset)
	{
		Asset::operator=(asset);

		if (mData)
			delete[] mData;

		if (asset.mDataSize > 0)
		{
			mDataSize = asset.mDataSize;
			mData = new char[mDataSize];
			memcpy(mData, asset.mData, mDataSize);
		}
		else
		{
			mDataSize = 0;
			mData = nullptr;
		}

		*mMeta = *(MetaInfo*)(asset.mMeta);

		return *this;
	}

	bool BinaryAsset::operator==(const BinaryAsset& other) const
	{
		return mMeta->IsEqual(other.mMeta);
	}

	bool BinaryAsset::operator!=(const BinaryAsset& other) const
	{
		return !mMeta->IsEqual(other.mMeta);
	}

	char* BinaryAsset::GetData() const
	{
		return mData;
	}

	UInt BinaryAsset::GetDataSize() const
	{
		return mDataSize;
	}

	void BinaryAsset::SetData(char* data, UInt size)
	{
		if (mData)
			delete[] mData;

		if (size > 0)
		{
			mDataSize = size;
			mData = new char[mDataSize];
			memcpy(mData, data, mDataSize);
		}
		else
		{
			mDataSize = 0;
			mData = nullptr;
		}
	}

	BinaryAsset::MetaInfo* BinaryAsset::GetMeta() const
	{
		return (MetaInfo*)mMeta;
	}

	const char* BinaryAsset::GetFileExtensions() const
	{
		return "bin";
	}

	void BinaryAsset::LoadData(const String& path)
	{
		InFile file(path);
		if (!file.IsOpened())
			GetAssetsLogStream()->Error("Failed to load binary asset data: can't open file %s", path);

		mDataSize = file.GetDataSize();
		mData = new char[mDataSize];
		file.ReadFullData(mData);
	}

	void BinaryAsset::SaveData(const String& path)
	{
		OutFile file(path);
		if (mDataSize > 0 && mData)
			file.WriteData(mData, mDataSize);
	}

	void BinaryAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(BinaryAsset, data, GetData);
		INITIALIZE_GETTER(BinaryAsset, dataSize, GetDataSize);
		INITIALIZE_GETTER(BinaryAsset, meta, GetMeta);
	}

	BinaryAssetRef BinaryAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<BinaryAsset>();
	}

}

DECLARE_CLASS(o2::BinaryAsset);

DECLARE_CLASS(o2::BinaryAssetRef);

DECLARE_CLASS(o2::BinaryAsset::MetaInfo);
