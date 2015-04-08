#include "file_asset.h"

#include "app/application.h"
#include "assets.h"
#include "util/file_system/file.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(FileAsset);

FileAsset::FileAsset():
Asset(), mData(NULL), mDataSize(0)
{
}

FileAsset::FileAsset(const String& location):
Asset(location), mData(NULL), mDataSize(0)
{
	LoadData();
}

FileAsset::FileAsset(const FileLocation& location):
Asset(location), mData(NULL), mDataSize(0)
{
	LoadData();
}

FileAsset::FileAsset(const FileAsset& asset):
Asset(), mData(NULL), mDataSize(0)
{
	if (asset.mData == NULL)
		return;

	mDataSize = asset.mDataSize;
	mData = mnew char[mDataSize];
	memcpy(mData, asset.mData, mDataSize);
}

FileAsset::~FileAsset()
{
	SafeRelease(mData);
}

FileAsset& FileAsset::operator=(const FileAsset& asset)
{
	SafeRelease(mData);
	mDataSize = 0;

	if (asset.mData == NULL)
		return *this;

	mDataSize = asset.mDataSize;
	mData = mnew char[mDataSize];
	memcpy(mData, asset.mData, mDataSize);

	return *this;
}

char* FileAsset::GetData() const
{
	return mData;
}

uint FileAsset::GetDataSize() const
{
	return mDataSize;
}

void FileAsset::SetData(char* data, uint size)
{
	SafeRelease(mData);
	mData = mnew char[size];
	memcpy(mData, data, size);
	mDataSize = size;
}

void FileAsset::SaveData()
{
	OutFile file(AppAssets()->GetAssetSourceFullPath(mLocation.mPath));

	if (mData)
		file.WriteData(mData, mDataSize);

	file.Close();
}

void FileAsset::LoadData()
{
	InFile file(AppAssets()->GetAssetFullPath(mLocation.mPath));

	if (!file.IsOpened())
	{
		AppAssets()->mLog->Error("Failed to load asset: %s, can't open file.", mLocation.mPath.c_str());
		return;
	}

	mDataSize = file.GetDataSize();
	mData = mnew char[mDataSize];
	file.ReadData(mData, mDataSize);
}

CLOSE_O2_NAMESPACE