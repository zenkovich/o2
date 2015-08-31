#pragma once

#include "Utils/CommonTypes.h"
#include "Utils/FileSystem/File.h"
#include "Utils/Serialization.h"
#include "Utils/Time.h"

namespace o2
{
	// ---------
	// File info
	// ---------
	class FileInfo
	{
	public:
		String   mPath;
		FileType mFileType;
		TimeStamp mCreatedDate;
		TimeStamp mAccessDate;
		TimeStamp mEditDate;
		UInt     mSize;

		bool operator==(const FileInfo& other) const;
	};

	// ---------
	// Path info
	// ---------
	class FolderInfo
	{
	public:
		typedef Vector<FileInfo> FilesVec;
		typedef Vector<FolderInfo> PathsVec;

		String   mPath;  // Path of this 
		FilesVec mFiles; // Files vector
		PathsVec mFolders; // Paths vector

	public:
		// Equal operator
		bool operator==(const FolderInfo& other) const;

		// Returns true if this contains specified path or file
		bool IsFileExist(const String& filePath);

		// Cuts repeating path parts to short style
		// Sample: 
		// SomePath/SomePath2
		// -SomePath/SomePath2/ff.x
		// -SomePath/SomePath2/ffy.x
		// Converting to:
		// SomePath/SomePath2
		// -ff.x
		// -ffy.x
		void ClampPathNames();

	protected:
		// Cut path recursive function
		void ProcessPathNamesClamping(int charCount);
	};
}
