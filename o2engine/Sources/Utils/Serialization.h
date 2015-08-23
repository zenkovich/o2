#pragma once

#include "Utils/String.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/Data/DataDoc.h"

namespace o2
{
	/** Class field info interface. */
	class IClassFieldInfo
	{
	protected:
		String mName;  /** Name of field. */
		void*  mOwner; /** Owner object pointer. */

	protected:
		/** ctor. */
		IClassFieldInfo(void* owner, const String& name);

	public:
		/** Returns name of field. */
		const String& Name() const;

		/** Serializing field into data node. */
		virtual DataNode Serialize() = 0;

		/** Deserializing field from data node. */
		virtual void     Deserialize(const DataNode& node) = 0;
	};

	/** Template field info. Contains reference to field. */
	template<typename _type>
	class ClassFieldInfo: public IClassFieldInfo
	{
		friend class ClassFieldRegistrator;
		friend class SerializeHelper;

	protected:
		_type& mValue; /** Reference field. */

	public:
		/** ctor. */
		ClassFieldInfo(void* owner, _type& value, const String& name);

		/** Serializing field into data node. */
		DataNode Serialize();

		/** Deserializing field from data node. */
		void     Deserialize(const DataNode& node);
	};

	/** Serializable object interface. */
	class ISerializable
	{
	public:
		typedef Vector<Ptr<IClassFieldInfo>> FieldsVec;

		/** Returns empty sample of this object type. */
		virtual ISerializable* CreateSample() const = 0;

		/** Returns array with fields infos. */
		virtual FieldsVec      GetFields();

		/** Serializing object into data node. */
		virtual DataNode       Serialize();

		/** Deserializing object from data node. */
		virtual void           Deserialize(const DataNode& node);

		/** DataNode converting operator. */
		virtual operator DataNode() = 0;

		/** Assign operator from data node. */
		virtual ISerializable& operator=(const DataNode& node) = 0;
	};

	/** Static serializable types container. */
	class SerializableTypesSamples
	{
		static Dictionary<String, ISerializable*> mObjectSamples; /** Serializable objects samples dictionary. */

	public:
		template<typename _type>
		struct Regist { Regist(); };

	public:
		/** Returns empty object type sample. */
		static ISerializable* CreateSample(const String& type);
	};

#define SERIALIZABLE_FIELDS(CLASS)                                              \
	static SerializableTypesSamples::Regist<CLASS> _srlzTypeReg;                \
	CLASS* CreateSample() const { return mnew CLASS(); }                        \
	CLASS& operator=(const DataNode& node) { Deserialize(node); return *this; } \
	operator DataNode() { return Serialize(); }                                 \
	FieldsVec GetFields() { FieldsVec res;


#define BASE_CLASS_FIELDS(BASE_CLASS) res.Add(BASE_CLASS.GetFields());

#define SERIALIZABLE_FIELDS_INHERITED(CLASS, BASE_CLASS)                        \
	static SerializableTypesSamples::Regist<CLASS> _srlzTypeReg;                \
	CLASS* CreateSample() const { return mnew CLASS(); }                        \
	CLASS& operator=(const DataNode& node) { Deserialize(node); return *this; } \
	operator DataNode() { return Serialize(); }                                 \
	FieldsVec GetFields()                                                       \
	{                                                                           \
	FieldsVec res;                                                              \
	res.Add(BASE_CLASS::GetFields());                                            


#define FIELD(NAME) res.Add(mnew ClassFieldInfo<decltype(NAME)>(this, NAME, #NAME));

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