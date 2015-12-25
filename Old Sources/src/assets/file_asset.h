#pragma once

#include "asset.h"

OPEN_O2_NAMESPACE

class FileAsset: public Asset
{
	DEFINE_TYPE(FileAsset);

protected:
	char* mData;
	uint  mDataSize;

protected:
	FileAsset();
	FileAsset(const String& location);
	FileAsset(const FileLocation& location);
	FileAsset(const FileAsset& asset);
	virtual ~FileAsset();

	FileAsset& operator=(const FileAsset& asset);

	char* GetData() const;
	uint GetDataSize() const;

	void SetData(char* data, uint size);

protected:
	void LoadData();
	void SaveData();
};

CLOSE_O2_NAMESPACE
