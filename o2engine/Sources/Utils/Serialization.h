#pragma once

#include "Utils/String.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/Data/DataDoc.h"
#include "Utils/Reflection.h"

namespace o2
{

	/** Serializable object interface. */
	class ISerializable
	{
	public:
		/** Returns empty sample of this object type. */
		virtual ISerializable* CreateSample() const = 0;

		/** Serializing object into data node. */
		virtual DataNode Serialize() = 0;

		/** Deserializing object from data node. */
		virtual void Deserialize(const DataNode& node) = 0;

		/** DataNode converting operator. */
		virtual operator DataNode() = 0;

		/** Assign operator from data node. */
		virtual ISerializable& operator=(const DataNode& node) = 0;

	protected:
		/**Beginning serialization callback. */
		virtual void OnSerialize() {}

		/** Completion deserialization callback. */
		virtual void OnDeserialized() {}
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

	/** Serialization fields processor. */
	class SerializationFieldsProcessor
	{
	public:
		/** Overridden field processor. */
		template<typename _type>
		class FieldProcessor: public IFieldProcessor<_type, SerializationFieldsProcessor>
		{
		public:
			/** ctor. */
			FieldProcessor(_type& field, const String& name, SerializationFieldsProcessor& owner);

			/** Function indicating that field is serializable. */
			FieldProcessor& Serializable();
		};

	private:
		DataNode& mDataNode; /** Target data node reference. */

	public:
		/** ctor. */
		SerializationFieldsProcessor(DataNode& dataNode);

		/** Processing field function, returns FieldProcessor object, which that you can add indicating functions (aka attributes). */
		template<typename _type>
		FieldProcessor<_type> ProcessField(_type& field, const String& name);
	};

	/** Deserialization fields processor. */
	class DeserializationFieldsProcessor
	{
	public:
		/** Overridden field processor. */
		template<typename _type>
		class FieldProcessor: public IFieldProcessor<_type, DeserializationFieldsProcessor>
		{
		public:
			/** ctor. */
			FieldProcessor(_type& field, const String& name, DeserializationFieldsProcessor& owner);

			/** Function indicating that field is serializable. */
			FieldProcessor& Serializable();
		};

	private:
		const DataNode& mDataNode; /** Source data node reference. */

	public:
		/** ctor. */
		DeserializationFieldsProcessor(const DataNode& dataNode);
		/** Processing field function, returns FieldProcessor object, which that you can add indicating functions (aka attributes). */
		template<typename _type>
		FieldProcessor<_type> ProcessField(_type& field, const String& name);
	};

/** Serialization helping macros. */
#define SERIALIZABLE_IMPL(CLASS)                                                \
	static SerializableTypesSamples::Regist<CLASS> _srlzTypeReg;                \
	CLASS* CreateSample() const 												\
	{																			\
		return mnew CLASS();													\
	}                        													\
	DataNode Serialize()                                                        \
	{                                                              				\
		OnSerialize();															\
		DataNode res;															\
		SerializationFieldsProcessor processor(res);							\
		ProcessFields(processor);												\
		return res;																\
	}																			\
	void Deserialize(const DataNode& node)										\
	{																			\
		DeserializationFieldsProcessor processor(node);							\
		ProcessFields(processor);												\
		OnDeserialized();														\
	}																			\
	CLASS& operator=(const DataNode& node) 										\
	{																			\
		Deserialize(node); return *this; 										\
	} 																			\
	operator DataNode() 														\
	{ 																			\
		return Serialize(); 													\
	}                                                                                             
                
/** Serializable type registration macros. */
#define SERIALIZABLE_REG(CLASS) SerializableTypesSamples::Regist<CLASS> CLASS::_srlzTypeReg; 
	
#pragma region SerializableTypesSamples implementation

	template<typename _type>
	SerializableTypesSamples::Regist<_type>::Regist()
	{
		_type* object = new _type();
		SerializableTypesSamples::mObjectSamples.Add(typeid(*object).name(), object);
	}

#pragma endregion SerializableTypesSamples implementation

#pragma region SerializationFieldsProcessor implementation
	
	template<typename _type>
	SerializationFieldsProcessor::FieldProcessor<_type> SerializationFieldsProcessor::ProcessField(_type& field, const String& name)
	{
		return FieldProcessor<_type>(field, name, *this);
	}

	template<typename _type>
	SerializationFieldsProcessor::FieldProcessor<_type>& SerializationFieldsProcessor::FieldProcessor<_type>::Serializable()
	{
		*mOwner.mDataNode.AddNode(mName) = mFieldRef;
		return *this;
	}

	template<typename _type>
	SerializationFieldsProcessor::FieldProcessor<_type>::FieldProcessor(_type& field, const String& name, SerializationFieldsProcessor& owner):
		IFieldProcessor(field, name, owner)
	{}

#pragma endregion SerializationFieldsProcessor implementation

#pragma region DeserializationFieldsProcessor implementation
	
	template<typename _type>
	DeserializationFieldsProcessor::FieldProcessor<_type> DeserializationFieldsProcessor::ProcessField(_type& field, const String& name)
	{
		return FieldProcessor<_type>(field, name, *this);
	}

	template<typename _type>
	DeserializationFieldsProcessor::FieldProcessor<_type>& DeserializationFieldsProcessor::FieldProcessor<_type>::Serializable()
	{
		mFieldRef = *mOwner.mDataNode.GetNode(mName);
		return *this;
	}

	template<typename _type>
	DeserializationFieldsProcessor::FieldProcessor<_type>::FieldProcessor(_type& field, const String& name, DeserializationFieldsProcessor& owner):
		IFieldProcessor(field, name, owner)
	{}
	
#pragma endregion DeserializationFieldsProcessor implementation
}