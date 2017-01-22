#include "SyntaxTree.h"

#include <algorithm>

SyntaxFile::SyntaxFile():
	mGlobalNamespace(new SyntaxNamespace())
{}

SyntaxFile::~SyntaxFile()
{
	delete mGlobalNamespace;
}

const string& SyntaxFile::GetPath() const
{
	return mPath;
}

const string& SyntaxFile::GetData() const
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

void SyntaxFile::SaveTo(pugi::xml_node& node) const
{
	node.append_attribute("path") = mPath.c_str();
	mLastEditedDate.SaveTo(node.append_child("date"));
	mGlobalNamespace->SaveTo(node.append_child("globalNamespace"));
}

void SyntaxFile::LoadFrom(pugi::xml_node& node)
{
	mPath = node.attribute("path").as_string();
	mLastEditedDate.LoadFrom(node.child("date"));

	delete mGlobalNamespace;
	mGlobalNamespace = new SyntaxNamespace();
	mGlobalNamespace->LoadFrom(node.child("globalNamespace"));
}

int ISyntaxExpression::GetBegin() const
{
	return mBegin;
}

int ISyntaxExpression::GetLength() const
{
	return mLength;
}

int ISyntaxExpression::GetEnd() const
{
	return mBegin + mLength;
}

int ISyntaxExpression::GetLine() const
{
	return mLine;
}

const string& ISyntaxExpression::GetData() const
{
	return mData;
}

SyntaxFile* ISyntaxExpression::GetOwnerFile() const
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

	for (auto x : mComments)
		delete x;

	for (auto x : mTypedefs)
		delete x;

	for (auto x : mUsingNamespaces)
		delete x;
}

SyntaxSection* SyntaxSection::GetParentSection() const
{
	return mParentSection;
}

const string& SyntaxSection::GetName() const
{
	return mName;
}

const string& SyntaxSection::GetFullName() const
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

const SyntaxTypedefsVec& SyntaxSection::GetTypedefs() const
{
	return mTypedefs;
}

const SyntaxUsingNamespacesVec& SyntaxSection::GetUsingNamespaces() const
{
	return mUsingNamespaces;
}

const SyntaxCommentsVec& SyntaxSection::GetComments() const
{
	return mComments;
}

SyntaxSectionsVec SyntaxSection::GetAllSections() const
{
	SyntaxSectionsVec res = mSections;

	for (auto x : mSections)
	{
		for (auto y : x->GetAllSections())
			res.push_back(y);
	}

	return res;
}

SyntaxClassesVec SyntaxSection::GetAllClasses() const
{
	SyntaxClassesVec res;
	
	for (auto x : mSections)
	{
		if (x->IsClass())
			res.push_back(dynamic_cast<SyntaxClass*>(x));
	}

	for (auto x : mSections)
	{
		for (auto y : x->GetAllClasses())
			res.push_back(y);
	}

	return res;
}

SyntaxEnumsVec SyntaxSection::GetAllEnums() const
{
	SyntaxEnumsVec res = mEnums;

	for (auto x : mSections)
	{
		for (auto y : x->GetAllEnums())
			res.push_back(y);
	}

	return res;
}

bool SyntaxSection::IsClass() const
{
	return false;
}

const SyntaxAttributesVec& SyntaxSection::GetAttributes() const
{
	return mAttributes;
}

void SyntaxSection::SaveTo(pugi::xml_node& node) const
{
	node.append_attribute("name") = mName.c_str();
	node.append_attribute("fullname") = mFullName.c_str();

	pugi::xml_node sectionsNode = node.append_child("sections");
	for (auto x : mSections)
		x->SaveTo(sectionsNode.append_child("section"));

	pugi::xml_node typedefsNode = node.append_child("typedefs");
	for (auto x : mTypedefs)
		x->SaveTo(typedefsNode.append_child("typedef"));

	pugi::xml_node usingsNode = node.append_child("usings");
	for (auto x : mUsingNamespaces)
		x->SaveTo(usingsNode.append_child("typedef"));
}

void SyntaxSection::LoadFrom(pugi::xml_node& node)
{
	mName = node.attribute("name").as_string();
	mFullName = node.attribute("fullname").as_string();

	pugi::xml_node sectionsNode = node.child("sections");
	for (auto node:sectionsNode)
	{
		if (node.type() != pugi::node_element)
			continue;

		SyntaxSection* newSection;
		if (node.attribute("isClass"))
			newSection = new SyntaxClass();
		else
			newSection = new SyntaxNamespace();

		newSection->LoadFrom(node);
		newSection->mParentSection = this;
		mSections.push_back(newSection);
	}

	pugi::xml_node typedefsNode = node.child("typedefs");
	for (auto node:typedefsNode)
	{
		if (node.type() != pugi::node_element)
			continue;

		SyntaxTypedef* newTypedef = new SyntaxTypedef();
		newTypedef->LoadFrom(node);
		mTypedefs.push_back(newTypedef);
	}

	pugi::xml_node usingsNode = node.child("usings");
	for (auto node:usingsNode)
	{
		if (node.type() != pugi::node_element)
			continue;

		SyntaxUsingNamespace* newUsing = new SyntaxUsingNamespace();
		newUsing->LoadFrom(node);
		mUsingNamespaces.push_back(newUsing);
	}
}

SyntaxNamespace::SyntaxNamespace()
{}

const SyntaxFunctionsVec& SyntaxClass::GetFunctions() const
{
	if (mSourceClass)
		return mSourceClass->GetFunctions();

	return SyntaxSection::GetFunctions();
}

const SyntaxVariablesVec& SyntaxClass::GetVariables() const
{
	if (mSourceClass)
		return mSourceClass->GetVariables();

	return SyntaxSection::GetVariables();
}

const SyntaxSectionsVec& SyntaxClass::GetSections() const
{
	if (mSourceClass)
		return mSourceClass->GetSections();

	return SyntaxSection::GetSections();
}

const SyntaxEnumsVec& SyntaxClass::GetEnums() const
{
	if (mSourceClass)
		return mSourceClass->GetEnums();

	return SyntaxSection::GetEnums();
}

const SyntaxTypedefsVec& SyntaxClass::GetTypedefs() const
{
	if (mSourceClass)
		return mSourceClass->GetTypedefs();

	return SyntaxSection::GetTypedefs();
}

const SyntaxUsingNamespacesVec& SyntaxClass::GetUsingNamespaces() const
{
	if (mSourceClass)
		return mSourceClass->GetUsingNamespaces();

	return SyntaxSection::GetUsingNamespaces();
}

SyntaxSectionsVec SyntaxClass::GetAllSections() const
{
	if (mSourceClass)
		return mSourceClass->GetAllSections();

	return SyntaxSection::GetAllSections();
}

SyntaxClassesVec SyntaxClass::GetAllClasses() const
{
	if (mSourceClass)
		return mSourceClass->GetAllClasses();

	return SyntaxSection::GetAllClasses();
}

bool SyntaxClass::IsClass() const
{
	return true;
}

const SyntaxClassInheritancsVec& SyntaxClass::GetBaseClasses() const
{
	return mBaseClasses;
}

bool SyntaxClass::IsTemplate() const
{
	if (!mTemplateParameters.empty())
		return true;

	if (mParentSection)
	{
		SyntaxClass* cls = dynamic_cast<SyntaxClass*>(mParentSection);
		return cls && cls->IsTemplate();
	}

	return false;
}

bool SyntaxClass::IsMetaClass() const
{
	return mIsMeta;
}

const string& SyntaxClass::GetTemplateParameters() const
{
	return mTemplateParameters;
}

SyntaxProtectionSection SyntaxClass::GetClassSection() const
{
	return mClassSection;
}

const string& SyntaxClass::GetAttributeCommentDef() const
{
	return mAttributeCommentDef;
}

const string& SyntaxClass::GetAttributeShortDef() const
{
	return mAttributeShortDef;
}

const SyntaxAttributesVec& SyntaxClass::GetAttributes() const
{
	if (mSourceClass)
		return mSourceClass->GetAttributes();

	return SyntaxSection::GetAttributes();
}

void SyntaxClass::SaveTo(pugi::xml_node& node) const
{
	SyntaxSection::SaveTo(node);

	node.append_attribute("isClass") = true;
	node.append_attribute("isMeta") = mIsMeta;
	node.append_attribute("templates") = mTemplateParameters.c_str();
	node.append_attribute("protection") = (int)mClassSection;
	node.append_attribute("attributeCommentDef") = mAttributeCommentDef.c_str();
	node.append_attribute("attributeShortDef") = mAttributeShortDef.c_str();

	pugi::xml_node baseClassesNode = node.append_child("baseClasses");
	for (auto& x : mBaseClasses)
		x.SaveTo(baseClassesNode.append_child("class"));
}

void SyntaxClass::LoadFrom(pugi::xml_node& node)
{
	SyntaxSection::LoadFrom(node);

	mIsMeta = node.attribute("isMeta").as_bool();
	mTemplateParameters = node.attribute("templates").as_string();
	mClassSection = (SyntaxProtectionSection)node.attribute("protection").as_int();
	mAttributeCommentDef = node.attribute("attributeCommentDef").as_string();
	mAttributeShortDef = node.attribute("attributeShortDef").as_string();

	pugi::xml_node baseClassesNode = node.child("baseClasses");
	for (auto node : baseClassesNode)
	{
		SyntaxClassInheritance x;
		x.LoadFrom(node);
		mBaseClasses.push_back(x);
	}
}

const string& SyntaxType::GetName() const
{
	return mName;
}

bool SyntaxType::IsConstant() const
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

const SyntaxType& SyntaxVariable::GetVariableType() const
{
	return mType;
}

const string& SyntaxVariable::GetName() const
{
	return mName;
}

SyntaxProtectionSection SyntaxVariable::GetClassSection() const
{
	return mClassSection;
}

bool SyntaxVariable::IsStatic() const
{
	return mIsStatic;
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

const string& SyntaxFunction::GetName() const
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

bool SyntaxFunction::IsTemplate() const
{
	return !mTemplates.empty();
}

const string& SyntaxFunction::GetTemplates() const
{
	return mTemplates;
}

bool SyntaxFunction::IsStatic() const
{
	return mIsStatic;
}

const string& SyntaxEnum::GetName() const
{
	return mName;
}

const string& SyntaxEnum::GetFullName() const
{
	return mFullName;
}

const StringStringDict& SyntaxEnum::GetEntries() const
{
	return mEntries;
}

SyntaxProtectionSection SyntaxEnum::GetClassSection() const
{
	return mClassSection;
}

SyntaxSection* SyntaxEnum::GetOwnerSyntaxSection() const
{
	return mOwnerSection;
}

SyntaxClassInheritance::SyntaxClassInheritance(const string& className, SyntaxProtectionSection type):
	mClassName(className), mInheritanceType(type)
{}

const string& SyntaxClassInheritance::GetClassName() const
{
	return mClassName;
}

SyntaxClass* SyntaxClassInheritance::GetClass()
{
	return mClass;
}

SyntaxProtectionSection SyntaxClassInheritance::GetInheritanceType() const
{
	return mInheritanceType;
}

void SyntaxClassInheritance::SaveTo(pugi::xml_node& node) const
{
	node.append_attribute("name") = mClassName.c_str();
	node.append_attribute("protection") = (int)mInheritanceType;
}

void SyntaxClassInheritance::LoadFrom(pugi::xml_node& node)
{
	mClassName = node.attribute("name").as_string();
	mInheritanceType = (SyntaxProtectionSection)node.attribute("protection").as_int();
}

bool SyntaxClassInheritance::operator==(const SyntaxClassInheritance& other) const
{
	return mInheritanceType == other.mInheritanceType && mClassName == other.mClassName;
}

const string& SyntaxUsingNamespace::GetUsingNamespaceName() const
{
	return mUsingNamespaceName;
}

SyntaxSection* SyntaxUsingNamespace::GetUsingNamespace() const
{
	return mUsingNamespace;
}

void SyntaxUsingNamespace::SaveTo(pugi::xml_node& node) const
{
	node.append_attribute("name") = mUsingNamespaceName.c_str();
}

void SyntaxUsingNamespace::LoadFrom(pugi::xml_node& node)
{
	mUsingNamespaceName = node.attribute("name").as_string();
}

const string& SyntaxTypedef::GetWhatName() const
{
	return mWhatName;
}

const string& SyntaxTypedef::GetNewDefName() const
{
	return mNewDefName;
}

SyntaxSection* SyntaxTypedef::GetWhat() const
{
	return mWhatSection;
}

SyntaxSection* SyntaxTypedef::GetNewDef() const
{
	return mWhatSection;
}

void SyntaxTypedef::SaveTo(pugi::xml_node& node) const
{
	node.append_attribute("what") = mWhatName.c_str();
	node.append_attribute("newDef") = mNewDefName.c_str();
}

void SyntaxTypedef::LoadFrom(pugi::xml_node& node)
{
	mWhatName = node.attribute("what").as_string();
	mNewDefName = node.attribute("newDef").as_string();
}

TimeStamp::TimeStamp(int seconds /*= 0*/, int minutes /*= 0*/, int hours /*= 0*/, int days /*= 0*/, int months /*= 0*/,
					 int years /*= 0*/):
	second(seconds), minute(minutes), hour(hours), day(days), month(months), year(years)
{}

void TimeStamp::SaveTo(pugi::xml_node& node) const
{
	node.append_attribute("year") = year;
	node.append_attribute("month") = month;
	node.append_attribute("day") = day;
	node.append_attribute("hour") = hour;
	node.append_attribute("minute") = minute;
	node.append_attribute("second") = second;
}

void TimeStamp::LoadFrom(pugi::xml_node& node)
{
	year = node.attribute("year").as_int();
	month = node.attribute("month").as_int();
	day = node.attribute("day").as_int();
	hour = node.attribute("hour").as_int();
	minute = node.attribute("minute").as_int();
	second = node.attribute("second").as_int();
}

bool TimeStamp::operator!=(const TimeStamp& wt) const
{
	return !(*this == wt);
}

bool TimeStamp::operator==(const TimeStamp& wt) const
{
	return second == wt.second && minute == wt.minute && hour == wt.hour && day == wt.day && month == wt.month &&
		year == wt.year;
}

const vector<string>& SyntaxAttributes::GetAttributesList() const
{
	return mAttributesList;
}
