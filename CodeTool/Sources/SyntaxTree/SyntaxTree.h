#pragma once

#include "Utils/Serializable.h"
#include "Utils/Containers/Vector.h"
#include "Utils/String.h"
#include "Utils/TimeStamp.h"

using namespace o2;

namespace CodeTool
{
	class SyntaxFile;
	class SyntaxNamespace;
	class SyntaxSection;
	class SyntaxClass;
	class SyntaxType;
	class SyntaxVariable;
	class SyntaxFunction;
	class SyntaxEnum;

	typedef Vector<SyntaxSection*>  SyntaxSectionsVec;
	typedef Vector<SyntaxClass*>    SyntaxClassesVec;
	typedef Vector<SyntaxVariable*> SyntaxVariablesVec;
	typedef Vector<SyntaxFunction*> SyntaxFunctionsVec;
	typedef Vector<SyntaxFile*>     SyntaxFilesVec;
	typedef Vector<SyntaxEnum*>     SyntaxEnumsVec;
	typedef Vector<String>          StringsVec;

	enum class ClassBodySection { Public, Private, Protected };

	class SyntaxTree: public ISerializable
	{
	public:
		SyntaxTree();
		~SyntaxTree();

		const SyntaxFilesVec& GetFiles() const;
		SyntaxNamespace* GetGlobalNamespace() const;

		SERIALIZABLE(SyntaxTree);

	protected:
		SyntaxFilesVec   mFiles; 
		SyntaxNamespace* mGlobalNamespace = nullptr; 

		friend class CppSyntaxParser;
	};

	class SyntaxFile: public ISerializable
	{
	public:
		SyntaxFile();
		~SyntaxFile();

		const String& GetPath() const;
		const String& GetData() const;
		const TimeStamp& GetLastEditedDate() const;
		SyntaxNamespace* GetGlobalNamespace() const;

		SERIALIZABLE(SyntaxFile);

	protected:
		String           mPath;
		String           mData;
		TimeStamp        mLastEditedDate;
		SyntaxNamespace* mGlobalNamespace = nullptr;

		friend class CppSyntaxParser;
	};

	class SyntaxEntry: public ISerializable
	{
	public:
		int GetBegin() const;
		int GetLength() const;
		int GetEnd() const;
		const String& GetData() const;
		SyntaxFile* GetOwnerFile() const;

		SERIALIZABLE(SyntaxEntry);

	protected:
		int         mBegin = 0;
		int         mLength = 0;
		String      mData;
		SyntaxFile* mFile = nullptr;

		friend class CppSyntaxParser;
	};

	class SyntaxSection: public SyntaxEntry
	{
	public:
		SyntaxSection();
		~SyntaxSection();

		SyntaxSection* GetParentSection() const;
		const String& GetName() const;
		const String& GetFullName() const;
		const SyntaxFunctionsVec& GetFunctions() const;
		const SyntaxVariablesVec& GetVariables() const;
		const SyntaxSectionsVec& GetSections() const;
		const SyntaxEnumsVec& GetEnums() const;

		virtual bool IsClass() const;

		SERIALIZABLE(SyntaxSection);

	protected:
		String             mName;     
		String             mFullName;
		SyntaxSection*     mParentSection = nullptr;
		SyntaxFunctionsVec mFunctions;
		SyntaxVariablesVec mVariables;
		SyntaxSectionsVec  mSections; 
		SyntaxEnumsVec     mEnums;

		friend class CppSyntaxParser;
		friend class SyntaxTree;
	};

	class SyntaxNamespace: public SyntaxSection
	{
	public:
		SyntaxNamespace();

		virtual bool IsClass() const;

		SERIALIZABLE(SyntaxNamespace);

		friend class CppSyntaxParser;
	};

	class SyntaxClass: public SyntaxSection
	{
	public:
		const StringsVec& GetBaseClassesNames() const;
		SyntaxClassesVec GetBaseClasses() const;
		bool IsTemplate() const;
		const String& GetTemplateParameters() const;
		ClassBodySection GetClassSection() const;

		virtual bool IsClass() const;

		SERIALIZABLE(SyntaxClass);

	protected:
		StringsVec       mBaseClasses;
		bool             mIsTemplate = false;
		String           mTemplateParameters;
		ClassBodySection mClassSection = ClassBodySection::Public;

		friend class CppSyntaxParser;
	};

	class SyntaxType: public SyntaxEntry
	{
	public:
		const String& GetName() const;
		bool IsContant() const;
		bool IsReference() const;
		bool IsPointer() const;

		SERIALIZABLE(SyntaxType);

	protected:
		String mName;
		bool   mIsContant = false;
		bool   mIsReference = false;
		bool   mIsPointer = false;

		friend class CppSyntaxParser;
	};

	class SyntaxVariable: public SyntaxEntry
	{
	public:
		const SyntaxType& GetSynType() const;
		const String& GetName() const;
		const StringsVec& GetAttributes() const;
		ClassBodySection GetClassSection() const;

		SERIALIZABLE(SyntaxVariable);

	protected:
		SyntaxType       mType;
		String           mName;
		StringsVec       mAttributes;
		ClassBodySection mClassSection = ClassBodySection::Public;

		friend class CppSyntaxParser;
	};

	class SyntaxFunction: public SyntaxEntry
	{
	public:
		SyntaxFunction();
		~SyntaxFunction();

		const SyntaxType& GetReturnType() const;
		const String& GetName() const;
		const SyntaxVariablesVec& GetParameters() const;
		ClassBodySection GetClassSection() const;

		SERIALIZABLE(SyntaxFunction);

	protected:
		SyntaxType         mReturnType;
		String             mName;
		SyntaxVariablesVec mParameters;
		ClassBodySection   mClassSection = ClassBodySection::Public;

		friend class CppSyntaxParser;
	};

	class SyntaxEnum: public SyntaxEntry
	{
	public:
		const String& GetName() const;
		const StringsVec& GetEntries() const;
		ClassBodySection GetClassSection() const;

		SERIALIZABLE(SyntaxEnum);

	protected:
		String           mName;
		StringsVec       mEntries;
		ClassBodySection mClassSection = ClassBodySection::Public;

		friend class CppSyntaxParser;
	};
}