#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Singleton.h"
#include "Utils/String.h"


namespace o2
{
	class LogStream;

	// File system access macros
#define o2FileSystem o2::FileSystemStuff::Instance()

	// ---------------------------------------------------
	// File system. Using for working with files and paths
	// ---------------------------------------------------
	class FileSystemStuff: public Singleton<FileSystemStuff>
	{
		typedef Vector<String> StringsVec;
		typedef Dictionary<FileType, StringsVec> ExtnsionsDict;

		Ptr<LogStream> mLog;           // File system log stream
		ExtnsionsDict  mExtensions;    // Extensions arrays dictionary
		String         mResourcesPath; // Basic application resources path

	public:
		// Default constructor
		FileSystemStuff();

		// Destructor
		~FileSystemStuff();

		// Returns resource path
		const String& GetResourcesPath() const;

		// Returns info of paths and files of specified path
		FolderInfo GetFolderInfo(const String& path) const;

		// Returns file info
		FileInfo GetFileInfo(const String& path) const;

		// Copying file
		bool FileCopy(const String& source, const String& dest) const;

		// Deletion file
		bool FileDelete(const String& file) const;

		// Moving file
		bool FileMove(const String& source, const String& dest) const;

		// Creates folder
		bool FolderCreate(const String& path, bool recursive = true) const;

		// Removes directory
		bool FolderRemove(const String& path, bool recursive = true) const;

		// Returns true if specified directory exist
		bool IsFolderExist(const String& path) const;

		// Returns true if specified file exist
		bool IsFileExist(const String& path) const;

		// Returns path from string
		String ExtractPathStr(const String& path) const;

		static String GetFileExtension(const String& filePath);
		static String GetFileNameWithoutExtension(const String& filePath);
		static String GetPathWithoutDirectories(const String& path);
		static String GetParentPath(const String& path);
	};
}