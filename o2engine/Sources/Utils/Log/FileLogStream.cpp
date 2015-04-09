#include "FileLogStream.h"

#include <fstream>

namespace o2
{
	FileLogStream::FileLogStream(const std::string& fileName):
		LogStream(), mFilename(fileName)
	{
		std::fstream ofs(mFilename.c_str(), std::ios::out);
		if (ofs) 
			ofs.close();
	}

	FileLogStream::FileLogStream(const std::string& id, const std::string& fileName):
		LogStream(id), mFilename(fileName)
	{
		std::fstream ofs(mFilename.c_str(), std::ios::out);
		if (ofs)
			ofs.close();
	}

	FileLogStream::~FileLogStream()
	{
	}

	void FileLogStream::OutStrEx(const std::string& str)
	{
		std::fstream ofs(mFilename.c_str(), std::ios::out);
		if (ofs)
		{
			ofs << str.c_str() << std::endl;
			ofs.close();
		}
	}
}