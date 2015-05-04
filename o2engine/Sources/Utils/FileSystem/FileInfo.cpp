#include "FileInfo.h"

#include "Utils/Math/Math.h"

namespace o2
{
	FileLocation::FileLocation(const String& path /*= ""*/, UInt id /*= 0*/):
		mPath(path), mId(id)
	{
	}

	SERIALIZE_METHOD_IMPL(FileLocation)
	{
		SERIALIZE_ID(mPath, "path");
		SERIALIZE_ID(mId, "id");
		return true;
	}

	bool FileLocation::operator==(const FileLocation& other) const
	{
		if (mId != 0 && other.mId != 0)
			return mId == other.mId;

		return mPath == other.mPath;
	}

	bool FileLocation::operator!=(const FileLocation& other) const
	{
		return !(other == *this);
	}


	bool FileInfo::operator==(const FileInfo& other) const
	{
		return mPath == other.mPath && mFileType == other.mFileType && mCreatedDate == other.mCreatedDate &&
			mAccessDate == other.mAccessDate && mEditDate == other.mEditDate && mSize == other.mSize;
	}


	bool PathInfo::operator==(const PathInfo& other) const
	{
		return mPath == other.mPath;
	}

	bool PathInfo::IsFileExist(const String& filePath)
	{
		for (auto file:mFiles)
		{
			if (file.mPath == filePath)
				return true;
		}

		for (auto path:mPaths)
		{
			if (path.IsFileExist(filePath))
				return true;
		}

		return false;
	}

	void PathInfo::ClampPathNames()
	{
		ProcessPathNamesClamping(mPath.Length() + 1);
	}

	void PathInfo::ProcessPathNamesClamping(int charCount)
	{
		mPath = mPath.SubStr(Math::Min(charCount, (int)mPath.Length()), Math::Max((int)mPath.Length() - charCount, 0));

		for (auto file:mFiles)
		{
			file.mPath = file.mPath.SubStr(Math::Min(charCount, file.mPath.Length()),
										   Math::Max(file.mPath.Length() - charCount, 0));
		}

		for (auto path:mPaths)
			path.ProcessPathNamesClamping(charCount);
	}

}