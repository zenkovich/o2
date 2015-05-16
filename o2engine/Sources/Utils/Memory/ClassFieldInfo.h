#pragma once

#include "Utils/String.h"
#include "Utils/Data/DataDoc.h"
#include "Utils/Memory/IObject.h"
#include "Utils/Containers/Dictionary.h"

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

		virtual DataNode Serialize() = 0;
		virtual void Deserialize(DataNode& node) = 0;
	};

	template<typename _type>
	class ClassFieldInfo : public IClassFieldInfo
	{
		friend class ClassFieldRegistrator;
		friend class SerializeHelper;

	protected:
		IObject* mOwner;
		_type*   mValuePtr;

	protected:
		ClassFieldInfo(IObject* owner, _type* valuePtr, const String& name);

	public:
		DataNode Serialize();
		void Deserialize(DataNode& node);
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

	class SerializeHelper
	{
		IObject* mOwner;
		static Dictionary<String, IObject*> mObjectSamples;

	public:
		SerializeHelper(IObject* owner);

		template<typename _type>
		SerializeHelper& AddField(_type& field, const char* id);
	};

#define SRLZ(NAME) NAME = ClassFieldRegistrator(this, #NAME, &NAME)
#define SERIALIZE(NAME) ClassFieldRegistrator(this, #NAME, &NAME)

#define SERIALIZABLE(CLASS)                                               \
	CLASS& operator=(DataNode& node) { Deserialize(node); return *this; } \
	operator DataNode() { return Serialize(); }                           \
	SerializeHelper _fregi = SerializeHelper(this)

#define FIELD(NAME) .AddField(NAME, #NAME)

	template<typename _type>
	ClassFieldInfo<_type>::ClassFieldInfo(IObject* owner, _type* valuePtr, const String& name) :
		IClassFieldInfo(name), mOwner(owner), mValuePtr(valuePtr)
	{
	}

	template<typename _type>
	DataNode ClassFieldInfo<_type>::Serialize()
	{
		DataNode res;
		res = *mValuePtr;
		res.SetName(mName);
		return res;
	}

	template<typename _type>
	void ClassFieldInfo<_type>::Deserialize(DataNode& node)
	{
		*mValuePtr = node;
	}

	template<typename _type>
	ClassFieldRegistrator::operator _type()
	{
		ClassFieldInfo<_type>* fld = new ClassFieldInfo<_type>(mOwner, (_type*)mPtr, mId);
		mOwner->mFields.Add(fld);

		return *(_type*)mPtr;
	}

	template<typename _type>
	SerializeHelper& SerializeHelper::AddField(_type& field, const char* id)
	{
		ClassFieldInfo<_type>* fld = new ClassFieldInfo<_type>(mOwner, &field, id);
		mOwner->mFields.Add(fld);
		return *this;
	}
}