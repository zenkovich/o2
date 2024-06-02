#include "o2/stdafx.h"
#include "FileInfo.h"

#include "o2/Utils/Math/Math.h"

namespace o2
{
    bool FileInfo::operator==(const FileInfo& other) const
    {
        return path == other.path && editDate == other.editDate && size == other.size;
    }


    bool FolderInfo::operator==(const FolderInfo& other) const
    {
        return path == other.path;
    }

    bool FolderInfo::IsFileExist(const String& filePath)
    {
        for (auto& file:files)
        {
            if (file.path == filePath)
                return true;
        }

        for (auto& path:folders)
        {
            if (path.IsFileExist(filePath))
                return true;
        }

        return false;
    }

    void FolderInfo::ClampPathNames()
    {
        ProcessPathNamesClamping(path.Length());
    }

    void FolderInfo::ProcessPathNamesClamping(int charCount)
    {
        path = path.SubStr(Math::Min(charCount, (int)path.Length()));

        for (FileInfo& file:files)
            file.path = file.path.SubStr(Math::Min(charCount, file.path.Length()));

        for (FolderInfo& path:folders)
            path.ProcessPathNamesClamping(charCount);
    }
}
