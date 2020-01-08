#include "stdafx.h"

#if defined PLATFORM_ANDROID || true

#include "Utils/FileSystem/File.h"

#include "Utils/Reflection/Reflection.h"
#include "Utils/FileSystem/FileSystem.h"

namespace o2
{
    bool InFile::Open(const String& filename)
    {
        Close();

        if (filename.StartsWith(GetAndroidAssetsPath()))
        {
            String assetsPath = filename.SubStr(((String)GetAndroidAssetsPath()).Length());
            mAsset = AAssetManager_open(o2FileSystem.GetAssetManager(), assetsPath, AASSET_MODE_STREAMING);

            if (!mAsset)
                return false;
        }
        else
        {
            mIfstream.open(filename, std::ios::binary);

            if (!mIfstream.is_open())
                return false;
        }

        mOpened = true;
        mFilename = filename;

        return true;
    }

    bool InFile::Close()
    {
        if (mOpened)
        {
            if (mAsset)
                AAsset_close(mAsset);
            else
                mIfstream.close();
        }

        return true;
    }

    UInt InFile::ReadFullData(void *dataPtr)
    {
        UInt length = 0;

        if (mAsset)
        {
            length = (UInt)AAsset_getLength(mAsset);
            AAsset_read(mAsset, dataPtr, length);
        }
        else
        {
            mIfstream.seekg(0, std::ios::beg);
            mIfstream.seekg(0, std::ios::end);
            length = (UInt) mIfstream.tellg();
            mIfstream.seekg(0, std::ios::beg);

            mIfstream.read((char *) dataPtr, length);
        }

        return length;
    }

    String InFile::ReadFullData()
    {
        UInt len = GetDataSize();
        char* buffer = mnew char[len + 1];

        ReadData(buffer, len);
        buffer[len] = '\0';

        return String(buffer);
    }

    void InFile::ReadData(void *dataPtr, UInt bytes)
    {
        if (mAsset)
            AAsset_read(mAsset, dataPtr, bytes);
        else
            mIfstream.read((char*)dataPtr, bytes);
    }

    void InFile::SetCaretPos(UInt pos)
    {
        if (mAsset)
            AAsset_seek(mAsset, pos, SEEK_SET);
        else
            mIfstream.seekg(pos, std::ios::beg);
    }

    UInt InFile::GetCaretPos()
    {
        if (mAsset)
            return (UInt)AAsset_seek(mAsset, 0, SEEK_CUR);

        return (UInt)mIfstream.tellg();
    }

    UInt InFile::GetDataSize()
    {
        if (mAsset)
            return (UInt)AAsset_getLength(mAsset);

        mIfstream.seekg(0, std::ios::beg);
        mIfstream.seekg(0, std::ios::end);
        UInt res = (long unsigned int)mIfstream.tellg();
        mIfstream.seekg(0, std::ios::beg);

        return res;
    }

    bool OutFile::Open(const String& filename)
    {
        Close();

        mOfstream.open(filename, std::ios::binary);

        if (!mOfstream.is_open())
        {
            return false;
        }

        mOpened = true;
        mFilename = filename;

        return true;
    }

    bool OutFile::Close()
    {
        if (mOpened)
            mOfstream.close();

        return true;
    }

    void OutFile::WriteData(const void* dataPtr, UInt bytes)
    {
        mOfstream.write((const char*)dataPtr, bytes);
    }
}

#endif