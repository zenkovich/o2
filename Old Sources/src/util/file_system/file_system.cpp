#include "file_system.h"
#include <Windows.h>
#include "util/time_utils.h"

OPEN_O2_NAMESPACE

#undef DeleteFile
#undef CopyFile
#undef MoveFile
#undef CreateDirectory
#undef RemoveDirectory

FileSystem::FileSystem()
{
	mExtensions[FileType::Image].push_back("png");
	mExtensions[FileType::Image].push_back("jpg");
	mExtensions[FileType::Image].push_back("dds");
	mExtensions[FileType::Image].push_back("pvr");

	mExtensions[FileType::Config].push_back("xml");
	mExtensions[FileType::Config].push_back("txt");
	mExtensions[FileType::Config].push_back("cfg");

	mExtensions[FileType::Atlas].push_back("atlas");

	mResourcesPath = ASSETS_BUILDED_PATH;
}

FileSystem::~FileSystem()
{
}

const FileSystem::ExtensionsVec& FileSystem::GetExtensions(FileType fileType) const
{
	return mExtensions.at(fileType);
}

String FileSystem::GetFilePathByExt(const String& path, FileType fileType) const
{
	ExtensionsVec exts = GetExtensions(fileType);
	FOREACH(ExtensionsVec, exts, ext)
	{
		String fullPath = path + "." + (*ext);
		if (IsFileExist(fullPath))
			return fullPath;
	}

	return path + "." + exts[0];
}

const String& FileSystem::GetResourcesPath() const
{
	return mResourcesPath;
}

PathInfo FileSystem::GetPathInfo(const String& path) const
{
	PathInfo res;
	res.mPath = path;

	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile((path + "/*").c_str(), &f);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(f.cFileName, ".") == 0 || strcmp(f.cFileName, "..") == 0)
				continue;

			if (f.dwFileAttributes == FILE_ATTRIBUTE_DIREctorY)
				res.mPaths.Add(GetPathInfo(path + "/" + f.cFileName));
			else
				res.mFiles.Add(GetFileInfo(path + "/" + f.cFileName));
		}
		while (FindNextFile(h, &f));
	}
	else
		gLog->Error("Failed getPathInfo: Error opening directory %s", path.c_str());

	FindClose(h);

	return res;
}

bool FileSystem::CopyFile(const String& source, const String& dest) const
{
	DeleteFile(dest);
	CreateDirectory(ExtractPath(dest));
	return CopyFileA(source.c_str(), dest.c_str(), TRUE) == TRUE;
}

bool FileSystem::DeleteFile(const String& file) const
{
	return DeleteFileA(file.c_str()) == TRUE;
}

bool FileSystem::MoveFile(const String& source, const String& dest) const
{
	return MoveFileA(source.c_str(), dest.c_str()) == TRUE;
}

FileInfo FileSystem::GetFileInfo(const String& path) const
{
	FileInfo res;
	res.mPath = "invalid_file";

	FILETIME creationTime, lastAccessTime, lastWriteTime;
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
		return res;

	if (!GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime))
	{
		CloseHandle(hFile);
		return res;
	}

	SYSTEMTIME stUTC, stLocal;

	FileTimeToSystemTime(&creationTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	res.mCreatedDate = WideTime(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

	FileTimeToSystemTime(&lastAccessTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	res.mAccessDate = WideTime(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

	FileTimeToSystemTime(&lastWriteTime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	res.mEditDate = WideTime(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

	res.mPath = path;
	String extension = path.substr(path.rfind(".") + 1);
	res.mFileType = FileType::File;

	FOREACH_CONST(ExtensionsMap, mExtensions, iext)
	{
		if (FIND(iext->second, extension) != iext->second.end())
		{
			res.mFileType = iext->first;
			break;
		}
	}

	DWORD dwSizeHigh=0, dwSizeLow=0;
	dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
	res.mSize = (dwSizeHigh * (MAXDWORD+1)) + dwSizeLow;
	res.mCheckSumm = res.mSize;

	CloseHandle(hFile);

	return res;
}

bool FileSystem::CreateDirectory(const String& path, bool recursive /*= true*/) const
{
	if (IsDirectoryExist(path))
		return true;

	if (!recursive)
		return CreateDirectoryA(path.c_str(), NULL) == TRUE;

	if (CreateDirectoryA(path.c_str(), NULL) == TRUE)
		return true;

	String extrPath = ExtractPath(path);
	if (extrPath == path)
		return false;

	return CreateDirectory(extrPath, true);
}

bool FileSystem::RemoveDirectory(const String& path, bool recursive /*= true*/) const
{
	if (!IsDirectoryExist(path))
		return false;

	if (!recursive)
		return RemoveDirectoryA(path.c_str()) == TRUE;

	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile((path + "/*").c_str(), &f);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(f.cFileName, ".") == 0 || strcmp(f.cFileName, "..") == 0)
				continue;

			if (f.dwFileAttributes == FILE_ATTRIBUTE_DIREctorY)
				RemoveDirectory(path + "/" + f.cFileName, true);
			else
				DeleteFile(path + "/" + f.cFileName);
		}
		while (FindNextFile(h, &f));
	}

	FindClose(h);

	return RemoveDirectoryA(path.c_str()) == TRUE;
}

bool FileSystem::IsDirectoryExist(const String& path) const
{
	DWORD tp = GetFileAttributes(path.c_str());

	if (tp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (tp & FILE_ATTRIBUTE_DIREctorY)
		return true;

	return false;
}

bool FileSystem::IsFileExist(const String& path) const
{
	DWORD tp = GetFileAttributes(path.c_str());

	if (tp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (tp & FILE_ATTRIBUTE_DIREctorY)
		return false;

	return true;
}

CLOSE_O2_NAMESPACE
