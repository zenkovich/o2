#include "File.h"

namespace o2
{

#pragma region InFile implementation

	InFile::InFile():
		mOpened(false)
	{
	}

	InFile::InFile(const TString& filename):
		mOpened(false)
	{
		Open(filename);
	}

	InFile::~InFile()
	{
		Close();
	}

	bool InFile::Open(const TString& filename)
	{
		Close();

		mIfstream.open(filename.c_str(), std::ios::binary);

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

	void InFile::ReadData(void *dataPtr, UInt bytes)
	{
		mIfstream.read((char*)dataPtr, bytes);
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
		return (long unsigned int)mIfstream.tellg();
	}

	const TString& InFile::GetFilename() const
	{
		return mFilename;
	}

	bool InFile::IsOpened() const
	{
		return mOpened;
	}

#pragma endregion InFile implementation

#pragma region OutFile implementation

	OutFile::OutFile():
		mOpened(false)
	{
	}

	OutFile::OutFile(const TString& filename):
		mOpened(false)
	{
		Open(filename);
	}

	OutFile::~OutFile()
	{
		Close();
	}

	bool OutFile::Open(const TString& filename)
	{
		Close();

		mOfstream.open(filename.c_str(), std::ios::binary);

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

	const TString& OutFile::GetFilename() const
	{
		return mFilename;
	}

	bool OutFile::IsOpened() const
	{
		return mOpened;
	}

#pragma endregion OutFile implementation

}