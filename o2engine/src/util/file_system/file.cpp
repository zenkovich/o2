#include "file.h"

#include "app/application_base_interface.h"
#include "file_system.h"

OPEN_O2_NAMESPACE

InFile::InFile():
mOpened(false)
{
}

InFile::InFile(const String& filename, FileType type /*= cFileType::FT_FILE*/):
mOpened(false)
{
	Open(filename, type);
}

InFile::~InFile()
{
	Close();
}

bool InFile::Open(const String& filename, FileType type /*= cFileType::FT_FILE*/)
{
	Close();

	if (type == FileType::File)
	{
		mIfstream.open(filename.c_str(), std::ios::binary);

		if (!mIfstream.is_open())
		{
			return false;
		}
	}
	else
	{
		bool success = false;

		const FileSystem::ExtensionsVec extensions = AppFileSystem()->GetExtensions(type);
		for (FileSystem::ExtensionsVec::const_iterator it = extensions.cbegin(); it != extensions.cend(); ++it)
		{
			String resFilename = AppFileSystem()->GetResourcesPath() + filename + "." + (*it);

			mIfstream.open(resFilename.c_str(), std::ios::binary);

			if (mIfstream.is_open())
			{
				success = true;
				break;
			}
		}

		if (!success)
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

uint InFile::ReadFullData(void *dataPtr)
{
	mIfstream.seekg(0, std::ios::beg);
	mIfstream.seekg(0, std::ios::end);
	uint length = (uint)mIfstream.tellg();
	mIfstream.seekg(0, std::ios::beg);

	mIfstream.read((char*)dataPtr, length);

	return length;
}

void InFile::ReadData(void *dataPtr, uint bytes)
{
	mIfstream.read((char*)dataPtr, bytes);
}

void InFile::SetCaretPos(uint pos)
{
	mIfstream.seekg(pos, std::ios::beg);
}

uint InFile::GetCaretPos()
{
	return (uint)mIfstream.tellg();
}

uint InFile::GetDataSize()
{
	mIfstream.seekg(0, std::ios::beg);
	mIfstream.seekg(0, std::ios::end);
	return (long unsigned int)mIfstream.tellg();
}

const String& InFile::GetFilename() const
{
	return mFilename;
}

bool InFile::IsOpened() const
{
	return mOpened;
}

//cOutFile
OutFile::OutFile():
mOpened(false)
{

}

OutFile::OutFile(const String& filename, FileType type /*= cFileType::FT_FILE*/):
mOpened(false)
{
	Open(filename, type);
}

OutFile::~OutFile()
{
	Close();
}

bool OutFile::Open(const String& filename, FileType type /*= cFileType::FT_FILE*/)
{
	Close();

	String resFilename = filename;
	if (type != FileType::File)
	{
		String extensionStr;
		const FileSystem::ExtensionsVec extensions = AppFileSystem()->GetExtensions(type);

		if (extensions.size() > 0)
			extensionStr = extensions[0];

		resFilename = AppFileSystem()->GetResourcesPath() + filename + "." + extensionStr;
	}

	mOfstream.open(resFilename.c_str(), std::ios::binary);

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

void OutFile::WriteData(const void* dataPtr, uint bytes)
{
	mOfstream.write((const char*)dataPtr, bytes);
}

const String& OutFile::GetFilename() const
{
	return mFilename;
}

bool OutFile::IsOpened() const
{
	return mOpened;
}

CLOSE_O2_NAMESPACE