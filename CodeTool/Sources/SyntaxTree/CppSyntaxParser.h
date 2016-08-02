#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/String.h"

using namespace o2;

namespace CodeTool
{
	class SyntaxTree;
	class SyntaxFile;

	class CppSyntaxParser
	{
	public:
		void Parse(SyntaxTree& syntaxTree, const String& sourcesPath);

	protected:
		void ParseSourcesInFolder(FolderInfo& folderInfo);
		void ParseFile(SyntaxFile& file, const String& path);

	protected:
		SyntaxTree* mSyntaxTree = nullptr;
		String      mSourcesPath;
	};
}