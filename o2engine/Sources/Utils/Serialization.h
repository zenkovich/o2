#pragma once

#include "Utils/Memory/IObject.h"
#include "Utils/String.h"
#include "Utils/Containers/Dictionary.h"

namespace o2
{
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

	public:
		SerializeHelper(IObject* owner);

		template<typename _type>
		SerializeHelper& AddField(_type& field, const char* id);
	};

	class SerializableTypesSamples
	{
		static Dictionary<String, IObject*> mObjectSamples;

	public:
		template<typename _type>
		struct Regist { Regist(); };

	public:
		static IObject* CreateSample(const String& type);
	};

#define SRLZ(NAME) NAME = ClassFieldRegistrator(this, #NAME, &NAME)
#define SERIALIZE(NAME) ClassFieldRegistrator(this, #NAME, &NAME)

#define SERIALIZABLE(CLASS)                                                     \
	static SerializableTypesSamples::Regist<CLASS> _srlzTypeReg;                \
	CLASS* CreateSample() const { return mnew CLASS(); }                        \
	CLASS& operator=(const DataNode& node) { Deserialize(node); return *this; } \
	operator DataNode() { return Serialize(); }                                 \
	SerializeHelper _srlzHelper = SerializeHelper(this)

#define FIELD(NAME) .AddField(NAME, #NAME)

#define SERIALIZABLE_REG(CLASS) SerializableTypesSamples::Regist<CLASS> CLASS::_srlzTypeReg; 

	template<typename _type>
	SerializeHelper& SerializeHelper::AddField(_type& field, const char* id)
	{
		ClassFieldInfo<_type>* fld = new ClassFieldInfo<_type>(mOwner, &field, id);
		mOwner->mFields.Add(fld);
		return *this;
	}

	template<typename _type>
	SerializableTypesSamples::Regist<_type>::Regist()
	{
		_type* object = new _type();
		SerializableTypesSamples::mObjectSamples.Add(typeid(*object).name(), object);
	}
}