#pragma once

#include "Utils/CommonTypes.h"
#include "Utils/Time.h"
#include "Utils/FileSystem/File.h"

namespace o2
{
	class FileInfo
	{
	public:
		String   mPath;
		FileType mFileType;
		WideTime mCreatedDate;
		WideTime mAccessDate;
		WideTime mEditDate;
		UInt     mSize;

		bool operator==(const FileInfo& other) const;
	};

	class PathInfo
	{
	public:
		typedef Array<FileInfo> FilesArr;
		typedef Array<PathInfo> PathsArr;

		String   mPath;
		FilesArr mFiles;
		PathsArr mPaths;

	public:
		bool operator==(const PathInfo& other) const;

		bool IsFileExist(const String& filePath);
		void ClampPathNames();

	protected:
		void ProcessPathNamesClamping(int charCount);
	};

	class FileLocation
	{
	public:
		String mPath;
		UInt   mId;

	public:
		FileLocation(const String& path = "", UInt id = 0);

		bool operator==(const FileLocation& other) const;
		bool operator!=(const FileLocation& other) const;
	};
	typedef Array<FileLocation> FileLocationsArr;
}
