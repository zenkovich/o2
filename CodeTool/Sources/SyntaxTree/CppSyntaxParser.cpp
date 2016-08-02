#include "CppSyntaxParser.h"

#include "SyntaxTree/SyntaxTree.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Debug.h"

namespace CodeTool
{

	void CppSyntaxParser::Parse(SyntaxTree& syntaxTree, const String& sourcesPath)
	{
		mSyntaxTree = &syntaxTree;
		mSourcesPath = sourcesPath;

		FolderInfo srcFolderInfo = o2FileSystem.GetFolderInfo(sourcesPath);
		ParseSourcesInFolder(srcFolderInfo);
	}

	void CppSyntaxParser::ParseSourcesInFolder(FolderInfo& folderInfo)
	{
		for (auto& fileInfo : folderInfo.mFiles)
		{
			if (!fileInfo.mPath.EndsWith(".h"))
				continue;

			SyntaxFile* syntaxFile = mnew SyntaxFile();
			mSyntaxTree->mFiles.Add(syntaxFile);

			ParseFile(*syntaxFile, mSourcesPath + fileInfo.mPath);

			for (auto& nestedFolderInfo : folderInfo.mFolders)
				ParseSourcesInFolder(nestedFolderInfo);
		}
	}

	void CppSyntaxParser::ParseFile(SyntaxFile& file, const String& path)
	{
		o2Debug.Log("Parse header %s", path);
	}
}