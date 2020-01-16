#pragma once

#include "o2/Utils/FileSystem/File.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/System/Time/Time.h"
#include "o2/Utils/Types/CommonTypes.h"

namespace o2
{
	// ---------
	// File info
	// ---------
	class FileInfo
	{
	public:
		String    path;
		FileType  fileType;
		TimeStamp createdDate;
		TimeStamp accessDate;
		TimeStamp editDate;
		UInt      size;

		bool operator==(const FileInfo& other) const;
	};

	// ---------
	// Path info
	// ---------
	class FolderInfo
	{
	public:
		String             path;    // Path of this 
		Vector<FileInfo>   files;   // Files vector
		Vector<FolderInfo> folders; // Paths vector

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
