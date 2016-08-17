#pragma once

#include "Utils/Serializable.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/String.h"
#include "Utils/TimeStamp.h"

#undef GetClassName

using namespace o2;

namespace CodeTool
{
	class SyntaxClass;
	class SyntaxClassInheritance;
	class SyntaxClassMeta;
	class SyntaxComment;
	class SyntaxEnum;
	class SyntaxFile;
	class SyntaxFunction;
	class SyntaxNamespace;
	class SyntaxSection;
	class SyntaxType;
	class SyntaxVariable;

	typedef Vector<String>             StringsVec;
	typedef Dictionary<String, String> StringStringDict;
	typedef Vector<SyntaxClass*>       SyntaxClassesVec;
	typedef Vector<SyntaxComment*>     SyntaxCommentsVec;
	typedef Vector<SyntaxEnum*>        SyntaxEnumsVec;
	typedef Vector<SyntaxFile*>        SyntaxFilesVec;
	typedef Vector<SyntaxFunction*>    SyntaxFunctionsVec;
	typedef Vector<SyntaxSection*>     SyntaxSectionsVec;
	typedef Vector<SyntaxVariable*>    SyntaxVariablesVec;
	typedef Vector<SyntaxClassMeta*>   SyntaxClassMetasVec;

	enum class SyntaxProtectionSection { Public, Private, Protected };

	class SyntaxTree: public ISerializable
	{
	public:
		SyntaxTree();
		~SyntaxTree();

		const SyntaxFilesVec& GetFiles() const;

		SERIALIZABLE(SyntaxTree);

	protected:
		SyntaxFilesVec mFiles; // @SERIALIZABLE

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
		String           mPath;                      // @SERIALIZABLE
		String           mData;                      // xx
		TimeStamp        mLastEditedDate;            // @SERIALIZABLE
		SyntaxNamespace* mGlobalNamespace = nullptr; // @SERIALIZABLE

		friend class CppSyntaxParser;
	};

	class SyntaxEntry: public ISerializable
	{
	public:
		SyntaxEntry() {}
		virtual ~SyntaxEntry() {}

		int GetBegin() const;
		int GetLength() const;
		int GetEnd() const;
		const String& GetData() const;
		SyntaxFile* GetOwnerFile() const;

		SERIALIZABLE(SyntaxEntry);

	protected:
		int            mBegin = 0;         // @SERIALIZABLE
		int            mLength = 0;        // @SERIALIZABLE
		String         mData;              // xx
		SyntaxFile*    mFile = nullptr;    // xx
		SyntaxComment* mComment = nullptr; // xx

		friend class CppSyntaxParser;
	};

	class SyntaxComment: public SyntaxEntry
	{
	public:
		SERIALIZABLE(SyntaxComment);
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
		const SyntaxClassMetasVec& GetClassMetas() const;

		virtual bool IsClass() const;

		SERIALIZABLE(SyntaxSection);

	protected: 
		String              mName;                    // @SERIALIZABLE
		String              mFullName;                // @SERIALIZABLE
		SyntaxSection*      mParentSection = nullptr; // xx
		SyntaxFunctionsVec  mFunctions;               // @SERIALIZABLE
		SyntaxVariablesVec  mVariables;               // @SERIALIZABLE
		SyntaxSectionsVec   mSections;                // @SERIALIZABLE
		SyntaxEnumsVec      mEnums;                   // @SERIALIZABLE
		SyntaxCommentsVec   mComments;                // @SERIALIZABLE
		SyntaxClassMetasVec mClassMetas;              // @SERIALIZABLE

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

	class SyntaxClassInheritance: public SyntaxEntry
	{
	public:
		SyntaxClassInheritance() {}
		SyntaxClassInheritance(const String& className, SyntaxProtectionSection type);

		const String& GetClassName() const;
		SyntaxProtectionSection GetInheritanceType() const;

		bool operator==(const SyntaxClassInheritance& other) const;

		SERIALIZABLE(SyntaxClassInheritance);

	protected:
		String                  mClassName;       // @SERIALIZABLE
		SyntaxProtectionSection mInheritanceType; // @SERIALIZABLE
	};
	typedef Vector<SyntaxClassInheritance> SyntaxClassInheritancsVec;

	class SyntaxClass: public SyntaxSection
	{
	public:
		const SyntaxClassInheritancsVec& GetBaseClassesNames() const;
		SyntaxClassesVec GetBaseClasses() const;
		bool IsTemplate() const;
		const String& GetTemplateParameters() const;
		SyntaxProtectionSection GetClassSection() const;

		virtual bool IsClass() const;

		SERIALIZABLE(SyntaxClass);

	protected:
		SyntaxClassInheritancsVec mBaseClasses;                                    // @SERIALIZABLE
		bool                      mIsMeta = false;                                 // @SERIALIZABLE
		String                    mTemplateParameters;                             // @SERIALIZABLE
		SyntaxProtectionSection   mClassSection = SyntaxProtectionSection::Public; // @SERIALIZABLE

		friend class CppSyntaxParser;
	};

	class SyntaxClassMeta: public SyntaxEntry
	{
	public:
		const String& GetClassName() const;

		SERIALIZABLE(SyntaxClassMeta);

	protected:
		String mClassName; // @SERIALIZABLE

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
		String mName;                // @SERIALIZABLE
		bool   mIsContant = false;   // @SERIALIZABLE
		bool   mIsReference = false; // @SERIALIZABLE
		bool   mIsPointer = false;   // @SERIALIZABLE

		friend class CppSyntaxParser;
	};

	class SyntaxVariable: public SyntaxEntry
	{
	public:
		const SyntaxType& GetSynType() const;
		const String& GetName() const;
		const StringsVec& GetAttributes() const;
		SyntaxProtectionSection GetClassSection() const;

		SERIALIZABLE(SyntaxVariable);

	protected:
		SyntaxType              mType;                                           // @SERIALIZABLE
		String                  mName;                                           // @SERIALIZABLE
		StringsVec              mAttributes;                                     // @SERIALIZABLE
		SyntaxProtectionSection mClassSection = SyntaxProtectionSection::Public; // @SERIALIZABLE
		bool                    mIsStatic = false;                               // @SERIALIZABLE

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
		SyntaxProtectionSection GetClassSection() const;

		SERIALIZABLE(SyntaxFunction);

	protected:
		SyntaxType              mReturnType;									 // @SERIALIZABLE
		String                  mName;											 // @SERIALIZABLE
		SyntaxVariablesVec      mParameters;									 // @SERIALIZABLE
		SyntaxProtectionSection mClassSection = SyntaxProtectionSection::Public; // @SERIALIZABLE
		bool                    mIsStatic = false;								 // @SERIALIZABLE
		bool                    mIsVirtual = false;								 // @SERIALIZABLE
		bool                    mIsContstant = false;							 // @SERIALIZABLE
		String                  mTemplates;                                      // @SERIALIZABLE

		friend class CppSyntaxParser;
	};

	class SyntaxEnum: public SyntaxEntry
	{
	public:
		const String& GetName() const;
		const StringStringDict& GetEntries() const;
		SyntaxProtectionSection GetClassSection() const;

		SERIALIZABLE(SyntaxEnum);

	protected:																	
		String                  mName;											 // @SERIALIZABLE
		StringStringDict        mEntries;										 // @SERIALIZABLE
		SyntaxProtectionSection mClassSection = SyntaxProtectionSection::Public; // @SERIALIZABLE

		friend class CppSyntaxParser;
	};
}