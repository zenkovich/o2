#include "stdafx.h"

#ifdef PLATFORM_WINDOWS

#include <Windows.h>
#include "Utils/FileSystem/FileSystem.h"

#include "Application/Application.h"
#include "Utils/Debug/Debug.h"
#include "Utils/Debug/Log/LogStream.h"

namespace o2
{

#undef DeleteFile
#undef CopyFile
#undef MoveFile
#undef CreateDirectory
#undef RemoveDirectory

	FolderInfo FileSystem::GetFolderInfo(const String& path) const
	{
		FolderInfo res;
		res.path = path;

		WIN32_FIND_DATA f;
		HANDLE h = FindFirstFile(path + "/*", &f);
		if (h != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(f.cFileName, ".") == 0 || strcmp(f.cFileName, "..") == 0)
					continue;

				if (f.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
					res.folders.Add(GetFolderInfo(path + "/" + f.cFileName));
				else
					res.files.Add(GetFileInfo(path + "/" + f.cFileName));
			} while (FindNextFile(h, &f));
		}
		else
			mInstance->mLog->Error("Failed GetPathInfo: Error opening directory " + path);

		FindClose(h);

		return res;
	}

	bool FileSystem::FileCopy(const String& source, const String& dest) const
	{
		FileDelete(dest);
		FolderCreate(ExtractPathStr(dest));
		return CopyFileA(source.Data(), dest.Data(), TRUE) == TRUE;
	}

	bool FileSystem::FileDelete(const String& file) const
	{
		return DeleteFileA(file.Data()) == TRUE;
	}

	bool FileSystem::FileMove(const String& source, const String& dest) const
	{
		String destFolder = GetParentPath(dest);

		if (!IsFolderExist(destFolder))
			FolderCreate(destFolder);

		return MoveFileA(source.Data(), dest.Data()) == TRUE;
	}

	FileInfo FileSystem::GetFileInfo(const String& path) const
	{
		FileInfo res;
		res.path = "invalid_file";

		FILETIME creationTime, lastAccessTime, lastWriteTime;
		HANDLE hFile = CreateFileA(path.Data(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
		{
			auto err = GetLastError();
			return res;
		}

		if (!GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime))
		{
			CloseHandle(hFile);
			return res;
		}

		SYSTEMTIME stUTC, stLocal;

		FileTimeToSystemTime(&creationTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
		res.createdDate = TimeStamp(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

		FileTimeToSystemTime(&lastAccessTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
		res.accessDate = TimeStamp(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

		FileTimeToSystemTime(&lastWriteTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
		res.editDate = TimeStamp(stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, stLocal.wMonth, stLocal.wYear);

		res.path = path;
		String extension = path.SubStr(path.FindLast(".") + 1);
		res.fileType = FileType::File;

		for (auto iext : mInstance->mExtensions)
		{
			if (iext.second.Contains(extension))
			{
				res.fileType = iext.first;
				break;
			}
		}

		DWORD dwSizeHigh = 0, dwSizeLow = 0;
		dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
		res.size = (dwSizeHigh * (MAXDWORD+1)) + dwSizeLow;

		CloseHandle(hFile);

		return res;
	}

	bool FileSystem::SetFileEditDate(const String& path, const TimeStamp& time) const
	{
		FILETIME lastWriteTime;
		HANDLE hFile = CreateFileA(path.Data(), GENERIC_READ | FILE_WRITE_ATTRIBUTES,
								   FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == NULL)
			return false;

		SYSTEMTIME stLocal, stUTC;

		stLocal.wSecond = time.mSecond;
		stLocal.wDayOfWeek = 0;
		stLocal.wMilliseconds = 0;
		stLocal.wMinute = time.mMinute;
		stLocal.wHour = time.mHour;
		stLocal.wDay = time.mDay;
		stLocal.wMonth = time.mMonth;
		stLocal.wYear = time.mYear;

		TzSpecificLocalTimeToSystemTime(NULL, &stLocal, &stUTC);
		SystemTimeToFileTime(&stUTC, &lastWriteTime);
		if (!SetFileTime(hFile, NULL, NULL, &lastWriteTime))
		{
			auto error = GetLastError();
			printf("err %i\n", error);
			CloseHandle(hFile);
			return false;
		}

		CloseHandle(hFile);

		return true;
	}

	bool FileSystem::FolderCreate(const String& path, bool recursive /*= true*/) const
	{
		if (IsFolderExist(path))
			return true;

		if (!recursive)
			return CreateDirectoryA(path.Data(), NULL) == TRUE;

		if (CreateDirectoryA(path.Data(), NULL) == TRUE)
			return true;

		String extrPath = ExtractPathStr(path);
		if (extrPath == path)
			return false;

		return FolderCreate(extrPath, true);
	}

	bool FileSystem::FolderCopy(const String& from, const String& to) const
	{
		if (!IsFolderExist(from) || !IsFolderExist(to))
			return false;

		SHFILEOPSTRUCT s = { 0 };
		s.hwnd = o2Application.mHWnd;
		s.wFunc = FO_COPY;
		s.fFlags = FOF_SILENT;
		s.pTo = to;
		s.pFrom = from;
		auto res = SHFileOperation(&s);
		return res == 0;
	}

	bool FileSystem::FolderRemove(const String& path, bool recursive /*= true*/) const
	{
		if (!IsFolderExist(path))
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
					FolderRemove(path + "/" + f.cFileName, true);
				else
					FileDelete(path + "/" + f.cFileName);
			} while (FindNextFile(h, &f));
		}

		FindClose(h);

		return RemoveDirectoryA(path.Data()) == TRUE;
	}

	bool FileSystem::Rename(const String& old, const String& newPath) const
	{
		int res = rename(old, newPath);
		return res == 0;
	}

	bool FileSystem::IsFolderExist(const String& path) const
	{
		DWORD tp = GetFileAttributes(path.Data());

		if (tp == INVALID_FILE_ATTRIBUTES)
			return false;

		if (tp & FILE_ATTRIBUTE_DIRECTORY)
			return true;

		return false;
	}

	bool FileSystem::IsFileExist(const String& path) const
	{
		DWORD tp = GetFileAttributes(path.Data());

		if (tp == INVALID_FILE_ATTRIBUTES)
			return false;

		if (tp & FILE_ATTRIBUTE_DIRECTORY)
			return false;

		return true;
	}
}

#endif // PLATFORM_WINDOWS