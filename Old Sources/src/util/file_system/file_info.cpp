#include "file_info.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(FileLocation)
{
	SERIALIZE_ID(mPath, "path");
	SERIALIZE_ID(mId, "id");
	return true;
}

bool FileLocation::operator==(const FileLocation& v) const
{
	if (mId != 0 && v.mId != 0)
		return mId == v.mId;

	return mPath == v.mPath;
}

bool FileLocation::operator!=(const FileLocation& v) const
{
	return !(v == *this);
}


bool FileInfo::operator==(const FileInfo& fi) const
{
	return mPath == fi.mPath && mFileType == fi.mFileType && mCreatedDate == fi.mCreatedDate &&
		mAccessDate == fi.mAccessDate && mEditDate == fi.mEditDate && mSize == fi.mSize;
}


bool PathInfo::operator==(const PathInfo& pi) const
{
	return mPath == pi.mPath;
}

bool PathInfo::IsFileExist(const String& path)
{
	foreach(mFiles, fileIt)
	if (fileIt->mPath == path)
		return true;

	foreach(mPaths, pathIt)
	if (pathIt->IsFileExist(path))
		return true;

	return false;
}

void PathInfo::ClampPathNames()
{
	ProcessPathNamesClamping(mPath.length() + 1);
}

void PathInfo::ProcessPathNamesClamping(int charCount)
{
	mPath = mPath.substr(Min(charCount, (int)mPath.length()), Max((int)mPath.length() - charCount, 0));

	foreach(mFiles, fileIt)
		fileIt->mPath = fileIt->mPath.substr(Min(charCount, (int)fileIt->mPath.length()),
		Max((int)fileIt->mPath.length() - charCount, 0));

	foreach(mPaths, pathIt)
		pathIt->ProcessPathNamesClamping(charCount);
}

CLOSE_O2_NAMESPACE