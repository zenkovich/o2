#include "o2/stdafx.h"

#ifdef PLATFORM_ANDROID

#include "Utils/FileSystem/FileSystem.h"

#include "Application/Application.h"
#include "Utils/Debug/Debug.h"
#include "Utils/Debug/Log/LogStream.h"

namespace o2
{
	AAssetManager* FileSystem::GetAssetManager() const
	{
        return o2Application.GetAssetManager();
	}

	FolderInfo FileSystem::GetFolderInfo(const String& path) const
	{
		FolderInfo res;
		res.mPath = path;

        if (path.StartsWith(GetAndroidAssetsPath()))
        {
            String assetsPath = path.SubStr(((String) GetAndroidAssetsPath()).Length()).TrimedEnd("/\\");
            AAssetDir *assetDir = AAssetManager_openDir(o2FileSystem.GetAssetManager(),
                                                        assetsPath.Data());

            if (!assetDir)
                return res;

            AAssetDir_rewind(assetDir);

            do
            {
                const char *fileName = AAssetDir_getNextFileName(assetDir);
                if (!fileName)
                    break;

                o2Debug.Log("-- List dir " + path + ": " + (String) fileName);
            } while (true);

            AAssetDir_close(assetDir);
        }

		return res;
	}

	bool FileSystem::FileCopy(const String& source, const String& dest) const
	{
		FileDelete(dest);
		FolderCreate(ExtractPathStr(dest));
		return true;
	}

	bool FileSystem::FileDelete(const String& file) const
	{
		return true;
	}

	bool FileSystem::FileMove(const String& source, const String& dest) const
	{
		String destFolder = GetParentPath(dest);

		if (!IsFolderExist(destFolder))
			FolderCreate(destFolder);

		return true;
	}

	FileInfo FileSystem::GetFileInfo(const String& path) const
	{
		FileInfo res;

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

		return res;
	}

	bool FileSystem::SetFileEditDate(const String& path, const TimeStamp& time) const
	{
        return false;
	}

	bool FileSystem::FolderCreate(const String& path, bool recursive /*= true*/) const
	{
		if (IsFolderExist(path))
			return true;

		if (!recursive)
			return true;

		//if (CreateDirectoryA(path.Data(), NULL) == TRUE)
		//	return true;

		String extrPath = ExtractPathStr(path);
		if (extrPath == path)
			return false;

		return FolderCreate(extrPath, true);
	}

	bool FileSystem::FolderCopy(const String& from, const String& to) const
	{
		if (!IsFolderExist(from) || !IsFolderExist(to))
			return false;

		return true;
	}

	bool FileSystem::FolderRemove(const String& path, bool recursive /*= true*/) const
	{
		if (!IsFolderExist(path))
			return false;

		if (!recursive)
			return true;

		return true;
	}

	bool FileSystem::Rename(const String& old, const String& newPath) const
	{
		int res = rename(old, newPath);
		return res == 0;
	}

	bool FileSystem::IsFolderExist(const String& path) const
	{
		return false;
	}

	bool FileSystem::IsFileExist(const String& path) const
	{
		return false;
	}
}

#endif // PLATFORM_ANDROID