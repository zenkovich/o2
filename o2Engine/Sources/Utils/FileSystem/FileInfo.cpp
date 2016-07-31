#include "FileInfo.h"

#include "Utils/Math/Math.h"

namespace o2
{
	bool FileInfo::operator==(const FileInfo& other) const
	{
		return mPath == other.mPath && mFileType == other.mFileType && mCreatedDate == other.mCreatedDate &&
			mAccessDate == other.mAccessDate && mEditDate == other.mEditDate && mSize == other.mSize;
	}


	bool FolderInfo::operator==(const FolderInfo& other) const
	{
		return mPath == other.mPath;
	}

	bool FolderInfo::IsFileExist(const String& filePath)
	{
		for (auto file:mFiles)
		{
			if (file.mPath == filePath)
				return true;
		}

		for (auto path:mFolders)
		{
			if (path.IsFileExist(filePath))
				return true;
		}

		return false;
	}

	void FolderInfo::ClampPathNames()
	{
		ProcessPathNamesClamping(mPath.Length() + 1);
	}

	void FolderInfo::ProcessPathNamesClamping(int charCount)
	{
		mPath = mPath.SubStr(Math::Min(charCount, (int)mPath.Length()));

		for (FileInfo& file:mFiles)
		{
			file.mPath = file.mPath.SubStr(Math::Min(charCount, file.mPath.Length()));
		}

		for (FolderInfo& path:mFolders)
			path.ProcessPathNamesClamping(charCount);
	}
}