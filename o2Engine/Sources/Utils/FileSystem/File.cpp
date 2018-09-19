#include "stdafx.h"
#include "File.h"

#include "Utils/Reflection/Reflection.h"

namespace o2
{
	InFile::InFile() :
		mOpened(false)
	{}

	InFile::InFile(const String& filename) :
		mOpened(false)
	{
		Open(filename);
	}

	InFile::~InFile()
	{
		Close();
	}

	const String& InFile::GetFilename() const
	{
		return mFilename;
	}

	bool InFile::IsOpened() const
	{
		return mOpened;
	}


	OutFile::OutFile() :
		mOpened(false)
	{}

	OutFile::OutFile(const String& filename) :
		mOpened(false)
	{
		Open(filename);
	}

	OutFile::~OutFile()
	{
		Close();
	}

	const String& OutFile::GetFilename() const
	{
		return mFilename;
	}

	bool OutFile::IsOpened() const
	{
		return mOpened;
	}

}

ENUM_META_(o2::FileType, FileType)
{
	ENUM_ENTRY(Atlas);
	ENUM_ENTRY(Config);
	ENUM_ENTRY(File);
	ENUM_ENTRY(Image);
}
END_ENUM_META;
