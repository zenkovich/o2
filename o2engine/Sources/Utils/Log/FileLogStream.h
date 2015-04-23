#pragma once

#include "Utils/Log/LogStream.h"

namespace o2
{
	class FileLogStream:public LogStream
	{
		TString mFilename;

	public:
		FileLogStream(const TString& fileName);
		FileLogStream(const TString& id, const TString& fileName);
		~FileLogStream();

	protected:
		void OutStrEx(const TString& str);
	};
}