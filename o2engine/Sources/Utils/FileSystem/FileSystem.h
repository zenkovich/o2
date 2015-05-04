#pragma once

#include "Utils/Singleton.h"
#include "Utils/String.h"
#include "Utils/FileSystem/FileInfo.h"

namespace o2
{
	class LogStream;

	/** File system, singleton. Contains basic resource path. */
	class FileSystem: Singleton<FileSystem>
	{
		LogStream* mLog;
		String     mResourcesPath; /**< Basic application resources path. */

	public:
		FileSystem();
		~FileSystem();

		/** Returns resource path. */
		const String& GetResourcesPath() const;

		/** Returns info of paths and files of specified path. */
		PathInfo GetPathInfo(const String& path) const;

		/** Returns file info. */
		FileInfo GetFileInfo(const String& path) const;

		/** Copying file. */
		bool CopyFile(const String& source, const String& dest) const;

		/** Deletion file. */
		bool DeleteFile(const String& file) const;

		/** Moving file. */
		bool MoveFile(const String& source, const String& dest) const;

		/** Creates folder. */
		bool CreateDirectory(const String& path, bool recursive = true) const;

		/** Removes directory. */
		bool RemoveDirectory(const String& path, bool recursive = true) const;

		/** Returns true if specified directory exist. */
		bool IsDirectoryExist(const String& path) const;

		/** Returns true if specified file exist. */
		bool IsFileExist(const String& path) const;
	};
}