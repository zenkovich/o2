#include "CppSyntaxParser.h"

#include "SyntaxTree/SyntaxTree.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Debug.h"
#include "Utils/Timer.h"

namespace CodeTool
{

	CppSyntaxParser::CppSyntaxParser()
	{
		auto availableParserTypes = TypeOf(ICppSyntaxStatementParser).DerivedTypes();
		for (auto parserType : availableParserTypes)
			mParsers.Add((ICppSyntaxStatementParser*)parserType->CreateSample());
	}

	CppSyntaxParser::~CppSyntaxParser()
	{
		for (auto parser : mParsers)
			delete parser;
	}

	void CppSyntaxParser::Parse(SyntaxTree& syntaxTree, const String& sourcesPath)
	{
		o2Debug.Log("Begin parsing from %s", sourcesPath);
		Timer timer;

		mSyntaxTree = &syntaxTree;
		mSourcesPath = sourcesPath;

		FolderInfo srcFolderInfo = o2FileSystem.GetFolderInfo(sourcesPath);
		ParseSourcesInFolder(srcFolderInfo);

		o2Debug.Log("Parsing ended for %f seconds", timer.GetTime());
	}

	void CppSyntaxParser::ParseSourcesInFolder(FolderInfo& folderInfo)
	{
		for (auto& fileInfo : folderInfo.mFiles)
		{
			if (!fileInfo.mPath.EndsWith(".h"))
				continue;

			SyntaxFile* syntaxFile = mnew SyntaxFile();
			mSyntaxTree->mFiles.Add(syntaxFile);

			ParseFile(*syntaxFile, fileInfo);

			for (auto& nestedFolderInfo : folderInfo.mFolders)
				ParseSourcesInFolder(nestedFolderInfo);
		}
	}

	void CppSyntaxParser::ParseFile(SyntaxFile& file, const FileInfo& fileInfo)
	{
		file.mPath = fileInfo.mPath;
		file.mData = o2FileSystem.ReadFile(fileInfo.mPath);
		file.mLastEditedDate = fileInfo.mEditDate;

		ParseSyntaxSection(*file.mGlobalNamespace, file.mData, file);
	}

	void CppSyntaxParser::ParseSyntaxSection(SyntaxSection& section, const String& source, SyntaxFile& file)
	{
		section.mLength = source.Length();
		section.mData = source;
		section.mFile = &file;

		int caret = 0;
		SyntaxProtectionSection protectionSection = SyntaxProtectionSection::Public;
		while (caret < section.mLength)
		{
			bool parsedByKeywork = false;
			for (auto parser : mParsers)
			{
				const char* keyWord = parser->GetKeyWord();

				int i = 0;
				bool success = true;
				while (keyWord[i] != '\0')
				{
					if (source[i + caret] != keyWord[i])
					{
						success = false;
						break;
					}
					i++;
				}

				if (!success)
					continue;

				parser->Parse(section, caret, protectionSection);
				parsedByKeywork = true;

				break;
			}

			if (parsedByKeywork)
				continue;

			// read and parse block
		}
	}

	const char* ICppSyntaxStatementParser::GetKeyWord() const
	{
		return "";
	}

	bool ICppSyntaxStatementParser::IsPossibleInNamespace() const
	{
		return true;
	}

	bool ICppSyntaxStatementParser::IsPossibleInClass() const
	{
		return true;
	}

	void CppSyntaxNamespaceParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxMultilineCommentParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxCommentParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxPragmaParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxIncludeParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxDefineParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxIfMacroParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxClassParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxStructParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxTemplateParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxTypedefParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxEnumParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxUsingParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxPublicSectionParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxPrivateSectionParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxProtectedSectionParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	void CppSyntaxFriendParser::Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection)
	{

	}

	String ReadWord(const String& data, int& caret,
					const String& breakSymbols = " \n\r(){}.,;+-*/=@!|&*:~\\",
					const String& skipSymbols = " \n\r")
	{
		String res = "";
		int braces = 0, sqBraces = 0, trBraces = 0, fgBraces = 0;

		for (; caret < data.Length(); caret++)
		{
			if (!skipSymbols.Contains(data[caret]))
				break;
		}

		for (; caret < data.Length; caret++)
		{

			if (breakSymbols.Contains(data[caret]) && fgBraces == 0 && braces == 0 && sqBraces == 0 && trBraces == 0)
				break;

			switch (data[caret])
			{
			case '{': fgBraces++; break;
			case '}': fgBraces--; break;

			case ')': braces--; break;
			case '(': braces++; break;

			case '[': sqBraces++; break;
			case ']': sqBraces--; break;

			case '<': trBraces++; break;
			case '>': trBraces--; break;
			}

			res += data[caret];
		}

		return res;
	}

	String ReadBlock(String data, ref int caret)
	{
		int begin = caret;
		int braces = 0, fgBraces = 0, sqBraces = 0, trBraces = 0;
		bool isInString = false;

		for (; caret < data.Length; caret++)
		{
			if (data[caret] == '{')
				break;

			if (data[caret] == ';')
				return data.SubString(begin, caret - begin);
		}

		caret++;
		fgBraces++;

		bool complete = false;
		for (; caret < data.Length && !complete; caret++)
		{
			if (isInString)
			{
				if (data[caret] == '"' && data[caret - 1] != '\\')
					isInString = false;

				continue;
			}

			switch (data[caret])
			{
			case '{': fgBraces++; break;
			case '}':
			fgBraces--;
			if (fgBraces == 0 && braces == 0 && sqBraces == 0)
				complete = true;
			break;

			case ')': braces--; break;
			case '(': braces++; break;

			case '[': sqBraces++; break;
			case ']': sqBraces--; break;

			case '<': trBraces++; break;
			case '>': trBraces--; break;

			case '"':
			isInString = true;
			break;
			}
		}

		caret--;

		return data.SubString(begin, Math.Min(caret, data.Length - 1) - begin + 1);
	}
	String ReadBraces(String data, ref int caret)
	{
		int begin = caret;
		int braces = 0, fgBraces = 0, sqBraces = 0, trBraces = 0;
		bool isInString = false;

		for (; caret < data.Length; caret++)
		{
			if (data[caret] == '(')
				break;

			if (data[caret] == ';')
				return data.SubString(begin, caret - begin);
		}

		caret++;
		braces++;

		bool complete = false;
		for (; caret < data.Length && !complete; caret++)
		{
			if (isInString)
			{
				if (data[caret] == '"' && data[caret - 1] != '\\')
					isInString = false;

				continue;
			}

			switch (data[caret])
			{
			case '}': fgBraces--; break;
			case '{': fgBraces++; break;

			case '[': sqBraces++; break;
			case ']': sqBraces--; break;

			case '<': trBraces++; break;
			case '>': trBraces--; break;

			case '(': braces++; break;
			case ')':
			braces--;
			if (fgBraces == 0 && braces == 0 && sqBraces == 0 && trBraces == 0)
				complete = true;
			break;

			case '"':
			isInString = true;
			break;
			}
		}

		String res = data.SubString(begin, Math.Min(caret, data.Length) - begin);
		if (res.Last() == ')')
			res = res.Remove(res.Length - 1, 1);

		if (res.First() == '(')
			res = res.Remove(0, 1);

		return res;
	}

	char GetNextSymbol(String data, int begin, String skipSymbols = " \n\r\t()[]{}")
	{
		for (; begin < data.Length; begin++)
			if (!skipSymbols.Contains(data[begin]))
				return data[begin];

		return '\0';
	}

	List<String> Split(String data, char splitSymbol)
	{
		List<String> res = new List<String>();
		int braces = 0, sqBraces = 0, trBraces = 0, fgBraces = 0;

		int lastSplit = 0;
		for (int i = 0; i < data.Length; i++)
		{
			switch (data[i])
			{
			case '{': fgBraces++; break;
			case '}': fgBraces--; break;

			case '(': braces++; break;
			case ')': braces--; break;

			case '<': trBraces++; break;
			case '>': trBraces--; break;

			case '[': sqBraces++; break;
			case ']': sqBraces--; break;
			}

			if (braces == 0 && sqBraces == 0 && trBraces == 0 && fgBraces == 0 && data[i] == splitSymbol)
			{
				res.Add(data.SubString(lastSplit, i - lastSplit));
				lastSplit = i + 1;
			}
		}
		res.Add(data.SubString(lastSplit, data.Length - lastSplit));

		return res;
	}
}