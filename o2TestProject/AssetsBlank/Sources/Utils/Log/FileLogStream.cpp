#include "FileLogStream.h"

#include <fstream>

namespace o2
{
	FileLogStream::FileLogStream(const String& fileName):
		LogStream(), mFilename(fileName)
	{
		std::fstream ofs(mFilename, std::ios::out);
		if (ofs) 
			ofs.close();
	}

	FileLogStream::FileLogStream(const WString& id, const String& fileName):
		LogStream(id), mFilename(fileName)
	{
		std::fstream ofs(mFilename, std::ios::out);
		if (ofs)
			ofs.close();
	}

	FileLogStream::~FileLogStream()
	{
	}

	void FileLogStream::OutStrEx(const WString& str)
	{
		std::fstream ofs(mFilename, std::ios::app);
		if (ofs)
		{
			ofs << ((String)str).Data() << std::endl;
			ofs.close();
		}
	}
}