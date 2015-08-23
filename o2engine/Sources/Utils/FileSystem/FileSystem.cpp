#include "FileSystem.h"

#include <Windows.h>
#include "Utils/Log/LogStream.h"
#include "Utils/Debug.h"

namespace o2
{

#undef DeleteFile
#undef CopyFile
#undef MoveFile
#undef CreateDirectory
#undef RemoveDirectory

	CREATE_SINGLETON(FileSystem);

	FileSystem::FileSystem()
	{
		mLog = mnew LogStream("File System");
		Debug::GetLog()->BindStream(mLog);

		mExtensions.Add(FileType::Image, StringsVec());
		mExtensions[FileType::Image].Add("png");
		mExtensions[FileType::Image].Add("jpg");
		mExtensions[FileType::Image].Add("dds");
		mExtensions[FileType::Image].Add("pvr");

		mExtensions.Add(FileType::Config, StringsVec());
		mExtensions[FileType::Config].Add("xml");
		mExtensions[FileType::Config].Add("txt");
		mExtensions[FileType::Config].Add("cfg");

		mExtensions.Add(FileType::Atlas, StringsVec());
		mExtensions[FileType::Atlas].Add("atlas");
	}

	FileSystem::~FileSystem()
	{
	}

	const String& FileSystem::GetResourcesPath()
	{
		return mInstance->mResourcesPath;
	}

	PathInfo FileSystem::GetPathInfo(const String& path)
	{
		PathInfo res;
		res.mPath = path;

		WIN32_FIND_DATA f;
		HANDLE h = FindFirstFile(path + "/*", &f);
		if (h != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(f.cFileName, ".") == 0 || strcmp(f.cFileName, "..") == 0)
					continue;

				if (f.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
					res.mPaths.Add(GetPathInfo(path + "/" + f.cFileName));
				else
					res.mFiles.Add(GetFileInfo(path + "/" + f.cFileName));
			}
			while (FindNextFile(h, &f));
		}
		else
			mInstance->mLog->Error("Failed GetPathInfo: Error opening directory %s", path.Data());

		FindClose(h);

		return res;
	}

	bool FileSystem::CopyFile_(const String& source, const String& dest)
	{
		DeleteFile_(dest);
		CreateDirectory_(ExtractPathStr(dest));
		return CopyFileA(source.Data(), dest.Data(), TRUE) == TRUE;
	}

	bool FileSystem::DeleteFile_(const String& file)
	{
		return DeleteFileA(file.Data()) == TRUE;
	}

	bool FileSystem::MoveFile_(const String& source, const String& dest)
	{
		return MoveFileA(source.Data(), dest.Data()) == TRUE;
	}

	FileInfo FileSystem::GetFileInfo(const String& path)
	{
		FileInfo res;
		res.mPath = "invalid_file";

		FILETIME creationTime, lastAccessTime, lastWriteTime;
		HANDLE hFile = CreateFileA(path.Data(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
		String extension = path.SubStr(path.FindLast(".") + 1);
		res.mFileType = FileType::File;

		for (auto iext:mInstance->mExtensions)
		{
			if (iext.Value().Contains(extension))
			{
				res.mFileType = iext.Key();
				break;
			}
		}

		DWORD dwSizeHigh=0, dwSizeLow=0;
		dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
		res.mSize = (dwSizeHigh * (MAXDWORD+1)) + dwSizeLow;

		CloseHandle(hFile);

		return res;
	}

	bool FileSystem::CreateDirectory_(const String& path, bool recursive /*= true*/)
	{
		if (IsDirectoryExist(path))
			return true;

		if (!recursive)
			return CreateDirectoryA(path.Data(), NULL) == TRUE;

		if (CreateDirectoryA(path.Data(), NULL) == TRUE)
			return true;

		String extrPath = ExtractPathStr(path);
		if (extrPath == path)
			return false;

		return CreateDirectory_(extrPath, true);
	}

	bool FileSystem::RemoveDirectory_(const String& path, bool recursive /*= true*/)
	{
		if (!IsDirectoryExist(path))
			return false;

		if (!recursive)
			return RemoveDirectoryA(path.Data()) == TRUE;

		WIN32_FIND_DATA f;
		HANDLE h = FindFirstFile((path + "/*").Data(), &f);
		if (h != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(f.cFileName, ".") == 0 || strcmp(f.cFileName, "..") == 0)
					continue;

				if (f.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
					RemoveDirectory_(path + "/" + f.cFileName, true);
				else
					DeleteFile_(path + "/" + f.cFileName);
			}
			while (FindNextFile(h, &f));
		}

		FindClose(h);

		return RemoveDirectoryA(path.Data()) == TRUE;
	}

	bool FileSystem::IsDirectoryExist(const String& path)
	{
		DWORD tp = GetFileAttributes(path.Data());

		if (tp == INVALID_FILE_ATTRIBUTES)
			return false;

		if (tp & FILE_ATTRIBUTE_DIRECTORY)
			return true;

		return false;
	}

	bool FileSystem::IsFileExist(const String& path)
	{
		DWORD tp = GetFileAttributes(path.Data());

		if (tp == INVALID_FILE_ATTRIBUTES)
			return false;

		if (tp & FILE_ATTRIBUTE_DIRECTORY)
			return false;

		return true;
	}

	String FileSystem::ExtractPathStr(const String& path)
	{
		return path.SubStr(0, path.FindLast("/"));
	}
}