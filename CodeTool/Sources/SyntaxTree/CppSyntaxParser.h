#pragma once

#include "SyntaxTree/SyntaxTree.h"
#include "Utils/Containers/Vector.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/IObject.h"
#include "Utils/String.h"

using namespace o2;

namespace CodeTool
{
	class ICppSyntaxStatementParser;

	class CppSyntaxParser
	{
	public:
		CppSyntaxParser();
		~CppSyntaxParser();

		void Parse(SyntaxTree& syntaxTree, const String& sourcesPath);

	protected:
		void ParseSourcesInFolder(FolderInfo& folderInfo);
		void ParseFile(SyntaxFile& file, const FileInfo& fileInfo);

	protected:
		typedef Vector<ICppSyntaxStatementParser*> SyntaxParsersVec;

		SyntaxTree*      mSyntaxTree = nullptr;
		String           mSourcesPath;
		SyntaxParsersVec mParsers;

	protected:
		void ParseSyntaxSection(SyntaxSection& section, const String& source, SyntaxFile& file);

	};

	class ICppSyntaxStatementParser: public IObject
	{
	public:
		virtual ~ICppSyntaxStatementParser() {}
		virtual const char* GetKeyWord() const;
		virtual bool IsPossibleInNamespace() const;
		virtual bool IsPossibleInClass() const;
		virtual void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection) {}

		IOBJECT(ICppSyntaxStatementParser);
	};

	class CppSyntaxNamespaceParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "namespace"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxNamespaceParser);
	};

	class CppSyntaxMultilineCommentParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "/*"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxMultilineCommentParser);
	};

	class CppSyntaxCommentParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "//"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxCommentParser);
	};

	class CppSyntaxPragmaParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "#pragma"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxPragmaParser);
	};

	class CppSyntaxIncludeParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "#include"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxIncludeParser);
	};

	class CppSyntaxDefineParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "#define"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxDefineParser);
	};

	class CppSyntaxIfMacroParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "#if"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxIfMacroParser);
	};

	class CppSyntaxClassParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "class"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxClassParser);
	};

	class CppSyntaxStructParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "struct"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxStructParser);
	};

	class CppSyntaxTemplateParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "template"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxTemplateParser);
	};

	class CppSyntaxTypedefParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "typedef"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxTypedefParser);
	};

	class CppSyntaxEnumParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "enum"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxEnumParser);
	};

	class CppSyntaxUsingParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "using"; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxUsingParser);
	};

	class CppSyntaxPublicSectionParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "public:"; }
		bool IsPossibleInNamespace() const { return false; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxPublicSectionParser);
	};

	class CppSyntaxPrivateSectionParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "private:"; }
		bool IsPossibleInNamespace() const { return false; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxPrivateSectionParser);
	};

	class CppSyntaxProtectedSectionParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "protected:"; }
		bool IsPossibleInNamespace() const { return false; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxProtectedSectionParser);
	};

	class CppSyntaxFriendParser: public ICppSyntaxStatementParser
	{
	public:
		const char* GetKeyWord() const { return "friend"; }
		bool IsPossibleInNamespace() const { return false; }
		void Parse(SyntaxSection& section, int& caret, SyntaxProtectionSection& protectionSection);

		IOBJECT(CppSyntaxFriendParser);
	};
}