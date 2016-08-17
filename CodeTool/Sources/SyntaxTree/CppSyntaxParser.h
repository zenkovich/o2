#pragma once

#include "SyntaxTree/SyntaxTree.h"
#include "Utils/Containers/Vector.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/IObject.h"
#include "Utils/String.h"

using namespace o2;

namespace CodeTool
{
	class CppSyntaxParser
	{
	public:
		CppSyntaxParser();
		~CppSyntaxParser();

		void Parse(SyntaxTree& syntaxTree, const String& sourcesPath);
		void ParseFile(SyntaxFile& file, const FileInfo& fileInfo);

	protected:
		typedef void(CppSyntaxParser::*ParserDelegate)(SyntaxSection&, int&, int&, SyntaxProtectionSection&);

		struct ExpressionParser
		{
			const char*    keyWord = nullptr;
			bool           isPossibleInClass = true;
			bool           isPossibleInNamespace = true;
			ParserDelegate parser;

			ExpressionParser() {}
			ExpressionParser(const char* keyWord, ParserDelegate parser, bool isPossibleInClass = true,
							 bool isPossibleInNamespace = true);
		};
		typedef Vector<ExpressionParser*> ParsersVec;

	protected:
		SyntaxTree* mSyntaxTree = nullptr;
		String      mSourcesPath;
		ParsersVec  mParsers;

	protected:
		void InitializeParsers();

		void ParseSourcesInFolder(FolderInfo& folderInfo);
		void ParseSyntaxSection(SyntaxSection& section, const String& source, SyntaxFile& file,
								SyntaxProtectionSection protectionSection);

		String ReadWord(const String& data, int& caret, int& line,
						const char* breakSymbols = " \n\r(){}.,;+-*/=@!|&*:~\\",
						const char* skipSymbols = " \n\r");
		String ReadBlock(const String& data, int& caret, int& line);
		String ReadBraces(const String& data, int& caret, int& line);
		char GetNextSymbol(const String& data, int begin, const char* skipSymbols = " \n\r\t()[]{}");
		Vector<String> Split(const String& data, char splitSymbol);
		void RemoveComments(String& input);

		void TryParseBlock(SyntaxSection& section, const String& block, int blockBegin, int& caret, int& line,
						   SyntaxProtectionSection& protectionSection);

		bool IsFunction(const String& data);

		SyntaxVariable* ParseVariable(const String& data, SyntaxProtectionSection& protectionSection, int begin, int end);
		SyntaxFunction* ParseFunction(const String& data, SyntaxProtectionSection& protectionSection, int begin, int end);

		void ParseNamespace(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseComment(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseMultilineComment(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParsePragma(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseInclude(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseDefine(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseIfMacros(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseMetaClass(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseClass(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseStruct(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseClassOrStruct(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection,
								bool isClass, bool isMeta, const String& templates);
		void ParseTemplate(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseTypedef(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseEnum(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseUsing(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParsePublicSection(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParsePrivateSection(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseProtectedSection(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseFriend(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
		void ParseClassMeta(SyntaxSection& section, int& caret, int& line, SyntaxProtectionSection& protectionSection);
	};
}