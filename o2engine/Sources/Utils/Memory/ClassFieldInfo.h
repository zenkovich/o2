#pragma once

#include "Utils/String.h"
#include "Utils/Data/DataDoc.h"
#include "Utils/Memory/IObject.h"

namespace o2
{
	class IClassFieldInfo
	{
	protected:
		String mName;

	protected:
		IClassFieldInfo(const String& name);

	public:
		const String& Name() const;

		virtual IClassFieldInfo& operator=(DataNode& data) = 0;
		virtual operator DataNode() = 0;
	};

	template<typename _type>
	class ClassFieldInfo : public IClassFieldInfo
	{
		friend class ClassFieldRegistrator;

	protected:
		IObject* mOwner;
		_type*   mValuePtr;

	protected:
		ClassFieldInfo(IObject* owner, _type* valuePtr, const String& name);

	public:
		ClassFieldInfo& operator=(DataNode& data);
		operator DataNode();
	};

	class ClassFieldRegistrator
	{
		IObject* mOwner;
		String   mId;
		void*    mPtr;

	public:
		ClassFieldRegistrator(IObject* owner, const char* id, void* ptr);

		template<typename _type>
		operator _type();
	};

#define SRLZ(NAME) NAME = ClassFieldRegistrator(this, #NAME, &NAME)
#define SERIALIZABLE(NAME) ClassFieldRegistrator(this, #NAME, &NAME)

	template<typename _type>
	ClassFieldInfo<_type>::ClassFieldInfo(IObject* owner, _type* valuePtr, const String& name) :
		IClassFieldInfo(name), mOwner(owner), mValuePtr(valuePtr)
	{
	}

	template<typename _type>
	ClassFieldInfo<_type>::operator DataNode()
	{
		DataNode res;
		res = *mValuePtr;
		res.SetName(mName);
		return res;
	}

	template<typename _type>
	ClassFieldInfo<_type>& ClassFieldInfo<_type>::operator=(DataNode& data)
	{
		*mValuePtr = (_type)data;
		return *this;
	}

	template<typename _type>
	ClassFieldRegistrator::operator _type()
	{
		ClassFieldInfo<_type>* fld = new ClassFieldInfo<_type>(mOwner, (_type*)mPtr, mId);
		mOwner->mFields.Add(fld);

		return *(_type*)mPtr;
	}
}