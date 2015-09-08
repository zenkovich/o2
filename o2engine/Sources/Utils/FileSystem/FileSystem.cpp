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

	FileSystem::~FileSystem()
	{
	}

	const String& FileSystem::GetResourcesPath() const
	{
		return mInstance->mResourcesPath;
	}

	FolderInfo FileSystem::GetFolderInfo(const String& path) const
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

		for (auto iext : mInstance->mExtensions)
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
			}
			while (FindNextFile(h, &f));
		}

		FindClose(h);

		return RemoveDirectoryA(path.Data()) == TRUE;
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

	String FileSystem::ExtractPathStr(const String& path) const
	{
		return path.SubStr(0, path.FindLast("/"));
	}

	String FileSystem::GetFileExtension(const String& filePath)
	{
		return filePath.SubStr(filePath.FindLast(".")).TrimedStart(".");
	}

	String FileSystem::GetFileNameWithoutExtension(const String& filePath)
	{
		return filePath.SubStr(0, filePath.FindLast("."));
	}

	String FileSystem::GetPathWithoutDirectories(const String& path)
	{
		return path.SubStr(Math::Max(path.FindLast("/"), path.FindLast("\\")));
	}

	String FileSystem::GetParentPath(const String& path)
	{
		return path.SubStr(0, Math::Max(path.FindLast("/"), path.FindLast("\\")));
	}

	String FileSystem::ReadFile(const String& path)
	{
		InFile file(path);
		if (!file.IsOpened())
			return String();

		return file.ReadFullData();
	}

	void FileSystem::WriteFile(const String& path, const String& data)
	{
		OutFile file(path);
		file.WriteData(path.Data(), path.Length());
	}

}