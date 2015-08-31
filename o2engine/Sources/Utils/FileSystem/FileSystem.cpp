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

	CREATE_SINGLETON(FileSystemStuff);

	FileSystemStuff::FileSystemStuff()
	{
		mLog = mnew LogStream("File System");
		Debug.GetLog()->BindStream(mLog);

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

	FileSystemStuff::~FileSystemStuff()
	{
	}

	const String& FileSystemStuff::GetResourcesPath() const
	{
		return mInstance->mResourcesPath;
	}

	FolderInfo FileSystemStuff::GetFolderInfo(const String& path) const
	{
		FolderInfo res;
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
					res.mFolders.Add(GetFolderInfo(path + "/" + f.cFileName));
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

	bool FileSystemStuff::CopyFile(const String& source, const String& dest) const
	{
		RemoveFile(dest);
		CreateDirectory(ExtractPathStr(dest));
		return CopyFileA(source.Data(), dest.Data(), TRUE) == TRUE;
	}

	bool FileSystemStuff::RemoveFile(const String& file) const
	{
		return DeleteFileA(file.Data()) == TRUE;
	}

	bool FileSystemStuff::MoveFile(const String& source, const String& dest) const
	{
		return MoveFileA(source.Data(), dest.Data()) == TRUE;
	}

	FileInfo FileSystemStuff::GetFileInfo(const String& path) const
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
		res.mCreatedDate = TimeStamp(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

		FileTimeToSystemTime(&lastAccessTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
		res.mAccessDate = TimeStamp(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

		FileTimeToSystemTime(&lastWriteTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
		res.mEditDate = TimeStamp(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

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

	bool FileSystemStuff::CreateDirectory(const String& path, bool recursive /*= true*/) const
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

		return CreateDirectory(extrPath, true);
	}

	bool FileSystemStuff::RemoveDirectory(const String& path, bool recursive /*= true*/) const
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
					RemoveDirectory(path + "/" + f.cFileName, true);
				else
					RemoveFile(path + "/" + f.cFileName);
			}
			while (FindNextFile(h, &f));
		}

		FindClose(h);

		return RemoveDirectoryA(path.Data()) == TRUE;
	}

	bool FileSystemStuff::IsDirectoryExist(const String& path) const
	{
		DWORD tp = GetFileAttributes(path.Data());

		if (tp == INVALID_FILE_ATTRIBUTES)
			return false;

		if (tp & FILE_ATTRIBUTE_DIRECTORY)
			return true;

		return false;
	}

	bool FileSystemStuff::IsFileExist(const String& path) const
	{
		DWORD tp = GetFileAttributes(path.Data());

		if (tp == INVALID_FILE_ATTRIBUTES)
			return false;

		if (tp & FILE_ATTRIBUTE_DIRECTORY)
			return false;

		return true;
	}

	String FileSystemStuff::ExtractPathStr(const String& path) const
	{
		return path.SubStr(0, path.FindLast("/"));
	}

	String FileSystemStuff::GetFileExtension(const String& filePath)
	{
		return filePath.SubStr(filePath.FindLast("."));
	}

	String FileSystemStuff::GetFileNameWithoutExtension(const String& filePath)
	{
		return filePath.SubStr(0, filePath.FindLast("."));
	}

	String FileSystemStuff::GetPathWithoutDirectories(const String& path)
	{
		return path.SubStr(Math::Max(path.FindLast("/"), path.FindLast("\\")));
	}

	String FileSystemStuff::GetParentPath(const String& path)
	{
		return path.SubStr(0, Math::Max(path.FindLast("/"), path.FindLast("\\")));
	}

}