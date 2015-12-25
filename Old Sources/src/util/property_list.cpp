#include "property_list.h"

OPEN_O2_NAMESPACE

PropertyList::PropertyList(const String& name /*= ""*/):
mPropertyListName(name), mParentPropertyList(NULL)
{
}

PropertyList::~PropertyList()
{
	if (mParentPropertyList)
		mParentPropertyList->RemoveChildPropertyList(this, false);

	FOREACH(PropertiesListsVec, mChildPropertyLists, child)
		SafeRelease(*child);
}

void PropertyList::AddChildPropertyList(PropertyList* propList)
{
	mChildPropertyLists.push_back(propList);
	propList->mParentPropertyList = this;
}

void PropertyList::RemoveChildPropertyList(PropertyList* propList, bool release /*= true*/)
{
	PropertiesListsVec::iterator fnd = FIND(mChildPropertyLists, propList);
	if (fnd == mChildPropertyLists.end())
		return;

	if (release)
		SafeRelease(*fnd);

	mChildPropertyLists.erase(fnd);
}

void PropertyList::RemoveAllChildPropertyLists()
{
	mChildPropertyLists.clear();
}

PropertyList::IProperty* PropertyList::GetPropertyBase(const String& pathName)
{
	int delPos = pathName.find("/");
	String pathPart = pathName.substr(0, delPos);

	if (delPos == pathName.npos)
	{
		FOREACH(PropertiesVec, mPropertiesList, prop)
		if ((*prop)->mName == pathPart)
			return *prop;
	}

	FOREACH(PropertiesListsVec, mChildPropertyLists, child)
	{
		if ((*child)->mPropertyListName == pathPart)
		{
			return (*child)->GetPropertyBase(pathName.substr(delPos + 1));
		}
	}

	return NULL;
}

void PropertyList::SetPropertyListName(const String& name)
{
	mPropertyListName = name;
}


String PropertyList::IProperty::GetPath()
{
	String res = mName;
	PropertyList* propList = mOwnerPropList;
	while (propList->mParentPropertyList)
	{
		res = propList->mPropertyListName + "/" + res;
		propList = propList->mParentPropertyList;
	}

	return res;
}

CLOSE_O2_NAMESPACE