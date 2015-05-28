#pragma once

#include "Utils/Memory/IObject.h"
#include "Utils/String.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/Data/DataDoc.h"

namespace o2
{
	class IClassFieldInfo
	{
	protected:
		String mName;
		void*  mOwner;

	protected:
		IClassFieldInfo(void* owner, const String& name);

	public:
		const String& Name() const;

		virtual DataNode Serialize() = 0;
		virtual void     Deserialize(const DataNode& node) = 0;
	};

	template<typename _type>
	class ClassFieldInfo: public IClassFieldInfo
	{
		friend class ClassFieldRegistrator;
		friend class SerializeHelper;

	protected:
		_type& mValue;

	public:
		ClassFieldInfo(void* owner, _type& value, const String& name);

		DataNode Serialize();
		void     Deserialize(const DataNode& node);
	};

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

	template<typename _type>
	ClassFieldInfo<_type>::ClassFieldInfo(void* owner, _type& value, const String& name):
		IClassFieldInfo(owner, name), mValue(value)
	{
	}

	template<typename _type>
	DataNode ClassFieldInfo<_type>::Serialize()
	{
		DataNode res;
		res = mValue;
		res.SetName(mName);
		return res;
	}

	template<typename _type>
	void ClassFieldInfo<_type>::Deserialize(const DataNode& node)
	{
		mValue = node;
	}
}