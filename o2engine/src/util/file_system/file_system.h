#pragma once

#include <map>
#include "file.h"
#include "file_info.h"
#include "public.h"
#include "util/string.h"

OPEN_O2_NAMESPACE
	
/** File system, singleton. Containing basic resource path, extension strings. */
class FileSystem
{
public:
	typedef Vector<String> ExtensionsVec;
	typedef std::map<FileType, ExtensionsVec> ExtensionsMap;

private:
	String        mResourcesPath; /**< Basic application resources path. */
	ExtensionsMap mExtensions;   /**< Extensions strings map. */

public:
	FileSystem();
	~FileSystem();

	/** Returns resource path. */
	const String& GetResourcesPath() const;

	/** Returns vector of extension strings for extension type. */
	const ExtensionsVec& GetExtensions(FileType fileType) const;

	/** Returns full file path with extension by file type. */
	String GetFilePathByExt(const String& path, FileType fileType) const;

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

CLOSE_O2_NAMESPACE