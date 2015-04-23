#include "FileLogStream.h"

#include <fstream>

namespace o2
{
	FileLogStream::FileLogStream(const TString& fileName):
		LogStream(), mFilename(fileName)
	{
		std::fstream ofs(mFilename.c_str(), std::ios::out);
		if (ofs) 
			ofs.close();
	}

	FileLogStream::FileLogStream(const TString& id, const TString& fileName):
		LogStream(id), mFilename(fileName)
	{
		std::fstream ofs(mFilename.c_str(), std::ios::out);
		if (ofs)
			ofs.close();
	}

	FileLogStream::~FileLogStream()
	{
	}

	void FileLogStream::OutStrEx(const TString& str)
	{
		std::fstream ofs(mFilename.c_str(), std::ios::app);
		if (ofs)
		{
			ofs << str.c_str() << std::endl;
			ofs.close();
		}
	}
}