#pragma once

#include "public.h"
#include "file.h"
#include "util/time_utils.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

class FileInfo
{
public:
	String   mPath;
	FileType mFileType;
	WideTime mCreatedDate;
	WideTime mAccessDate;
	WideTime mEditDate;
	uint     mSize;
	uint64   mCheckSumm;

	bool operator==(const FileInfo& fi) const;
};

class PathInfo
{
public:
	typedef Array<FileInfo> FilesArr;
	typedef Array<PathInfo> PathsArr;

	String   mPath;
	FilesArr mFiles;
	PathsArr mPaths;

	bool operator==(const PathInfo& pi) const;

	bool IsFileExist(const String& path);
	void ClampPathNames();

protected:
	void ProcessPathNamesClamping(int charCount);
};

class FileLocation: public Serializable
{
public:
	String mPath;
	uint   mId;

	FileLocation(const String& path = "", uint id = 0):mPath(path), mId(id) {}

	bool operator==(const FileLocation& v) const;
	bool operator!=(const FileLocation& v) const;

	SERIALIZBLE_METHODS(FileLocation);
};
typedef Array<FileLocation> FileLocationsArr;

CLOSE_O2_NAMESPACE
