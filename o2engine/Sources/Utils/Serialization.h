#pragma once

#include "Utils/Memory/IObject.h"
#include "Utils/String.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/Data/DataDoc.h"

namespace o2
{
	class IClassFieldInfo;

	class Serializable
	{
	public:
		typedef Array<IClassFieldInfo*> FieldsArr;

		virtual Serializable* CreateSample() const = 0;
		virtual FieldsArr     GetFields();
		virtual DataNode      Serialize();
		virtual void          Deserialize(const DataNode& node);

		virtual operator DataNode() = 0;
		virtual IObject& operator=(const DataNode& node) = 0;
	};

	class SerializableTypesSamples
	{
		static Dictionary<String, Serializable*> mObjectSamples;

	public:
		template<typename _type>
		struct Regist { Regist(); };

	public:
		static Serializable* CreateSample(const String& type);
	};

#define SERIALIZABLE_FIELDS(CLASS)                                              \
	static SerializableTypesSamples::Regist<CLASS> _srlzTypeReg;                \
	CLASS* CreateSample() const { return mnew CLASS(); }                        \
	CLASS& operator=(const DataNode& node) { Deserialize(node); return *this; } \
	operator DataNode() { return Serialize(); }                                 \
	FieldsArr GetFields() { FieldsArr res;

#define BULLSHIT \
	}

#define BASE_CLASS_FIELDS(BASE_CLASS) res.Add(BASE_CLASS.GetFields());

#define SERIALIZABLE_FIELDS_INHERITED(CLASS, BASE_CLASS)                        \
	static SerializableTypesSamples::Regist<CLASS> _srlzTypeReg;                \
	CLASS* CreateSample() const { return mnew CLASS(); }                        \
	CLASS& operator=(const DataNode& node) { Deserialize(node); return *this; } \
	operator DataNode() { return Serialize(); }                                 \
	FieldsArr GetFields()                                                       \
	{                                                                           \
	FieldsArr res;                                                              \
	res.Add(BASE_CLASS.GetFields());                                            


#define FIELD(NAME) res.Add(new ClassFieldInfo<decltype(NAME)>(this, NAME, #NAME));

#define END_SERIALIZABLE_FIELDS \
	return res;                 \
	}                         

#define SERIALIZABLE_REG(CLASS) SerializableTypesSamples::Regist<CLASS> CLASS::_srlzTypeReg; 
	
	template<typename _type>
	SerializableTypesSamples::Regist<_type>::Regist()
	{
		_type* object = new _type();
		SerializableTypesSamples::mObjectSamples.Add(typeid(*object).name(), object);
	}
}