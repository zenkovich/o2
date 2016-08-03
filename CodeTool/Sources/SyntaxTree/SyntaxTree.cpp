#include "SyntaxTree.h"

namespace CodeTool
{

	SyntaxTree::SyntaxTree():
		mGlobalNamespace(new SyntaxNamespace())
	{}

	SyntaxTree::~SyntaxTree()
	{
		for (auto file : mFiles)
			delete file;

		mGlobalNamespace->mSections.Clear();
		mGlobalNamespace->mEnums.Clear();
		mGlobalNamespace->mFunctions.Clear();
		mGlobalNamespace->mVariables.Clear();

		delete mGlobalNamespace;
	}

	const SyntaxFilesVec& SyntaxTree::GetFiles() const
	{
		return mFiles;
	}

	SyntaxNamespace* SyntaxTree::GetGlobalNamespace() const
	{
		return mGlobalNamespace;
	}

	SyntaxFile::SyntaxFile():
		mGlobalNamespace(new SyntaxNamespace())
	{}

	SyntaxFile::~SyntaxFile()
	{
		delete mGlobalNamespace;
	}

	const String& SyntaxFile::GetPath() const
	{
		return mPath;
	}

	const String& SyntaxFile::GetData() const
	{
		return mData;
	}

	const TimeStamp& SyntaxFile::GetLastEditedDate() const
	{
		return mLastEditedDate;
	}

	SyntaxNamespace* SyntaxFile::GetGlobalNamespace() const
	{
		return mGlobalNamespace;
	}

	int SyntaxEntry::GetBegin() const
	{
		return mBegin;
	}

	int SyntaxEntry::GetLength() const
	{
		return mLength;
	}

	int SyntaxEntry::GetEnd() const
	{
		return mBegin + mLength;
	}

	const String& SyntaxEntry::GetData() const
	{
		return mData;
	}

	SyntaxFile* SyntaxEntry::GetOwnerFile() const
	{
		return mFile;
	}

	SyntaxSection::SyntaxSection()
	{}

	SyntaxSection::~SyntaxSection()
	{
		for (auto x : mFunctions)
			delete x;

		for (auto x : mVariables)
			delete x;

		for (auto x : mEnums)
			delete x;

		for (auto x : mSections)
			delete x;
	}

	SyntaxSection* SyntaxSection::GetParentSection() const
	{
		return mParentSection;
	}

	const String& SyntaxSection::GetName() const
	{
		return mName;
	}

	const String& SyntaxSection::GetFullName() const
	{
		return mFullName;
	}

	const SyntaxFunctionsVec& SyntaxSection::GetFunctions() const
	{
		return mFunctions;
	}

	const SyntaxVariablesVec& SyntaxSection::GetVariables() const
	{
		return mVariables;
	}

	const SyntaxSectionsVec& SyntaxSection::GetSections() const
	{
		return mSections;
	}

	const SyntaxEnumsVec& SyntaxSection::GetEnums() const
	{
		return mEnums;
	}

	bool SyntaxSection::IsClass() const
	{
		return false;
	}

	SyntaxNamespace::SyntaxNamespace()
	{}

	bool SyntaxNamespace::IsClass() const
	{
		return false;
	}

	const StringsVec& SyntaxClass::GetBaseClassesNames() const
	{
		return mBaseClasses;
	}

	SyntaxClassesVec SyntaxClass::GetBaseClasses() const
	{
		return SyntaxClassesVec();
	}

	bool SyntaxClass::IsTemplate() const
	{
		return mIsTemplate;
	}

	const String& SyntaxClass::GetTemplateParameters() const
	{
		return mTemplateParameters;
	}

	SyntaxProtectionSection SyntaxClass::GetClassSection() const
	{
		return mClassSection;
	}

	bool SyntaxClass::IsClass() const
	{
		return true;
	}

	const String& SyntaxType::GetName() const
	{
		return mName;
	}

	bool SyntaxType::IsContant() const
	{
		return mIsContant;
	}

	bool SyntaxType::IsReference() const
	{
		return mIsReference;
	}

	bool SyntaxType::IsPointer() const
	{
		return mIsPointer;
	}

	const SyntaxType& SyntaxVariable::GetSynType() const
	{
		return mType;
	}

	const String& SyntaxVariable::GetName() const
	{
		return mName;
	}

	const StringsVec& SyntaxVariable::GetAttributes() const
	{
		return mAttributes;
	}

	SyntaxProtectionSection SyntaxVariable::GetClassSection() const
	{
		return mClassSection;
	}

	SyntaxFunction::SyntaxFunction()
	{}

	SyntaxFunction::~SyntaxFunction()
	{
		for (auto x : mParameters)
			delete x;
	}

	const SyntaxType& SyntaxFunction::GetReturnType() const
	{
		return mReturnType;
	}

	const String& SyntaxFunction::GetName() const
	{
		return mName;
	}

	const SyntaxVariablesVec& SyntaxFunction::GetParameters() const
	{
		return mParameters;
	}

	SyntaxProtectionSection SyntaxFunction::GetClassSection() const
	{
		return mClassSection;
	}

	const String& SyntaxEnum::GetName() const
	{
		return mName;
	}

	const StringsVec& SyntaxEnum::GetEntries() const
	{
		return mEntries;
	}

	SyntaxProtectionSection SyntaxEnum::GetClassSection() const
	{
		return mClassSection;
	}

}