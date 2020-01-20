#include "o2/stdafx.h"

#ifdef PLATFORM_WINDOWS

#include "o2/Utils/FileSystem/File.h"
#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
    bool InFile::Open(const String& filename)
    {
        Close();

        mIfstream.open(filename, std::ios::binary);

        if (!mIfstream.is_open())
        {
            return false;
        }

        mOpened = true;
        mFilename = filename;

        return true;
    }

    bool InFile::Close()
    {
        if (mOpened)
            mIfstream.close();

        return true;
    }

    UInt InFile::ReadFullData(void *dataPtr)
    {
        mIfstream.seekg(0, std::ios::beg);
        mIfstream.seekg(0, std::ios::end);
        UInt length = (UInt)mIfstream.tellg();
        mIfstream.seekg(0, std::ios::beg);

        mIfstream.read((char*)dataPtr, length);

        return length;
    }

    WString InFile::ReadFullData()
    {
        UInt len = GetDataSize();
        wchar_t* buffer = mnew wchar_t[len/sizeof(wchar_t) + 1];

        ReadData(buffer, len);
        buffer[len/sizeof(wchar_t)] = '\0';

		return WString(buffer);
    }

    void InFile::ReadData(void *dataPtr, UInt bytes)
    {
        auto& r = mIfstream.read((char*)dataPtr, bytes);
    }

    void InFile::SetCaretPos(UInt pos)
    {
        mIfstream.seekg(pos, std::ios::beg);
    }

    UInt InFile::GetCaretPos()
    {
        return (UInt)mIfstream.tellg();
    }

    UInt InFile::GetDataSize()
    {
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
