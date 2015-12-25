#pragma once

#include "public.h"
#include "util/callback.h"

OPEN_O2_NAMESPACE

//TODO: needs hardly refactor

/** Object property list class. Contains properties array, processing properties. */
class PropertyList
{
	friend class IProperty;

public:
	class IProperty
	{
	public:
		String        mName;          /** Name of property. */
		PropertyList* mOwnerPropList; /** Owner property list. NULL, if no owner. */
		CallbackChain onChangeEvent;  /** Calls when calling setter. */

		IProperty(const String& name):
			mName(name), mOwnerPropList(NULL) {}

		virtual ~IProperty() {}

		String GetPath();

	protected:
		template<typename _class>
		void CheckPropertyList(_class* tclass)
		{
			PropertyList* propList = dynamic_cast<PropertyList*>(tclass);
			if (propList)
			{
				propList->mPropertiesList.push_back(this);
				mOwnerPropList = (propList);
			}
		}
	};

	template<typename _type>
	class PropertyBase: public IProperty
	{
	public:
		PropertyBase():IProperty("") {}

		/** Returns value. */
		virtual _type Get() const = 0;
		virtual void  Set(const _type& value) = 0;

		PropertyBase& operator=(const _type& value)
		{
			Set(value);
			return *this;
		}

		PropertyBase& operator+=(const _type& value)
		{
			*this = *this + value;
			return *this;
		}

		_type operator+(const _type& value)
		{
			return Get() + value;
		}

		PropertyBase& operator-=(const _type& value)
		{
			*this = *this - value;
			return *this;
		}

		_type operator-(const _type& value)
		{
			return Get() - value;
		}

		PropertyBase& operator*=(const _type& value)
		{
			*this = *this * value;
			return *this;
		}

		_type operator*(const _type& value)
		{
			return Get() * value;
		}

		PropertyBase& operator/=(const _type& value)
		{
			*this = *this / value;
			return *this;
		}

		_type operator/(const _type& value)
		{
			return Get() / value;
		}
	};

	typedef Vector<IProperty*> PropertiesVec;
	typedef Vector<PropertyList*> PropertiesListsVec;

protected:
	String             mPropertyListName;   /** Property list name. */
	PropertyList*      mParentPropertyList; /** Parent property list. */
	PropertiesListsVec mChildPropertyLists; /** Child properties list array. */
	PropertiesVec      mPropertiesList;     /** Properties array .*/

public:
	PropertyList(const String& name = "");
	virtual ~PropertyList();

	void SetPropertyListName(const String& name);
	void AddChildPropertyList(PropertyList* propList);
	void RemoveChildPropertyList(PropertyList* propList, bool release = true);
	void RemoveAllChildPropertyLists();

	template<typename T>
	PropertyBase<T>* GetProperty(const String& pathName)
	{
		return static_cast<PropertyBase<T>*>(GetPropertyBase(pathName));
	}

protected:
	IProperty* GetPropertyBase(const String& pathName);
};

CLOSE_O2_NAMESPACE
