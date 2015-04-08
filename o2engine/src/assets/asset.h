#pragma once

#include "public.h"
#include "util/file_system/file_info.h"
#include "util/ref_object.h"
#include "util/serialize_util.h"
#include "util/time_utils.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class Asset
{
	friend class Assets;

	DEFINE_TYPE(Asset);

protected:
	FileLocation mLocation;

public:
	Asset();
	Asset(const String& location);
	Asset(const FileLocation& location);
	Asset(const Asset& asset);
	virtual ~Asset();

	Asset& operator=(const Asset& asset);

	FileLocation GetLocation() const;
	String GetPath() const;
	uint GetFileId() const;

	void Save(const String& path, bool rebuildAssetsImmediately = true);

protected:
	virtual void SaveData() = 0;
};

struct AssetInfo: public Serializable
{
public:
	enum class Type { File, Image, Atlas, Folder };

	FileLocation mLocation;
	Type         mType;
	WideTime     mWriteTime;

public:
	AssetInfo();
	AssetInfo(const FileLocation& location, Type type, const WideTime& writeTime);

	bool operator==(const AssetInfo& other) const;

	SERIALIZBLE_METHODS(AssetInfo);
};
typedef Array<AssetInfo> AssetsInfosArr;

CLOSE_O2_NAMESPACE
