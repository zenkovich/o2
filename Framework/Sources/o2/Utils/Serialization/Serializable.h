
#pragma once

#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Types/String.h"
#include "o2/Utils/Singleton.h"

namespace o2
{
	// -----------------------------
	// Serializable object interface
	// -----------------------------
	class ISerializable: public IObject
	{
	public:
		// Serializing object into data node
		virtual void Serialize(DataValue& node) const {}

		// Deserializing object from data node
		virtual void Deserialize(const DataValue& node) {};

		// Serializes data to string
		String SerializeToString() const;

		// Deserializes data from string
		void DeserializeFromString(const String& str);

		// Assign operator from data node
		ISerializable& operator=(const DataValue& node) { return *this; };

		// DataDocument converting operator
		operator DataDocument() const { return DataDocument(); }

		// Beginning serialization callback
		virtual void OnSerialize(DataValue& node) const {}

		// Completion deserialization callback
		virtual void OnDeserialized(const DataValue& node) {}

		IOBJECT(ISerializable);

	protected:
		// Serializing object into data node
		virtual void SerializeBasic(DataValue& node) const {}

		// Deserializing object from data node
		virtual void DeserializeBasic(const DataValue& node) {}
	};

	// ----------------------------
	// Serializable field attribute
	// ----------------------------
	class SerializableAttribute: public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("SERIALIZABLE");
		ATTRIBUTE_SHORT_DEFINITION("SERIALIZABLE_ATTRIBUTE");
	};

	class SerializeTypeProcessor
	{
	public:
		struct BaseFieldProcessor;

		template<typename _type>
		struct SerializeDefaultFieldProcessor;

	public:
		DataValue& node;

	public:
		SerializeTypeProcessor(DataValue& node):node(node) {}

		template<typename _object_type>
		void StartBases(_object_type* object, Type* type) {}

		template<typename _object_type>
		void StartFields(_object_type* object, Type* type) {}

		template<typename _object_type, typename _base_type>
		void BaseType(_object_type* object, Type* type, const char* name);

		BaseFieldProcessor StartField();

		struct BaseFieldProcessor
		{
			DataValue& node;

			BaseFieldProcessor(DataValue& node):node(node) {}

			template<typename _attribute_type, typename ... _args>
			auto AddAttribute(_args ... args);

			template<typename _type>
			BaseFieldProcessor& SetDefaultValue(const _type& value);

			template<typename _object_type, typename _field_type>
			BaseFieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
											_field_type& field);

			void SetProtectSection(ProtectSection section);
		};

		struct SerializeFieldProcessor: public BaseFieldProcessor
		{
			SerializeFieldProcessor(DataValue& node):BaseFieldProcessor(node) {}

			template<typename _attribute_type, typename ... _args>
			SerializeFieldProcessor& AddAttribute(_args ... args);

			template<typename _type>
			SerializeDefaultFieldProcessor<_type> SetDefaultValue(const _type& value);

			template<typename _object_type, typename _field_type>
			SerializeFieldProcessor& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
												 _field_type& field);
		};

		template<typename _type>
		struct SerializeDefaultFieldProcessor: public SerializeFieldProcessor
		{
			_type defaultValue;

			SerializeDefaultFieldProcessor(DataValue& node, const _type& defaultValue):
				SerializeFieldProcessor(node), defaultValue(defaultValue)
			{}

			template<typename _object_type, typename _field_type>
			SerializeDefaultFieldProcessor<_type>& FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*),
															   _field_type& field);
		};
	};

	template<typename T>
	struct DataValue::Converter<T, typename std::enable_if<std::is_base_of<ISerializable, T>::value>::type>
	{
		static constexpr bool isSupported = true;

		static void Write(const T& value, DataValue& data)
		{
			value.SerializeBasic(data);
			value.OnSerialize(data);
		}

		static void Read(T& value, const DataValue& data)
		{
			struct helper
			{
				static void ReadObject(void* object, const ObjectType& type, const DataValue& node)
				{
					for (auto baseType : type.GetBaseTypes())
					{
						const ObjectType* baseObjectType = dynamic_cast<const ObjectType*>(baseType.type);
						if (!baseObjectType)
							continue;

						void* baseObject = (*baseType.dynamicCastUpFunc)(object);
						ReadObject(baseObject, *baseObjectType, node);
					}

					for (auto& field : type.GetFields())
					{
						auto srlzAttribute = field.GetAttribute<SerializableAttribute>();
						if (srlzAttribute)
						{
							auto fldNode = node.FindMember(field.GetName());
							if (fldNode)
								field.DeserializeFromObject(object, *fldNode);
						}
					}
				}
			};

			const ObjectType& type = dynamic_cast<const ObjectType&>(value.GetType());
			void* objectPtr = type.DynamicCastFromIObject(dynamic_cast<IObject*>(&value));
			helper::ReadObject(objectPtr, type, data);

			if (value.GetType().IsBasedOn(TypeOf(ISerializable)))
				dynamic_cast<ISerializable&>(value).OnDeserialized(data);
		}
	};

	template<typename _object_type, typename _base_type>
	void SerializeTypeProcessor::BaseType(_object_type* object, Type* type, const char* name)
	{
		if constexpr (std::is_base_of<ISerializable, _base_type>::value && !std::is_same<ISerializable, _base_type>::value)
			object->_base_type::SerializeBasic(node);
	}

	template<typename _attribute_type, typename ... _args>
	auto SerializeTypeProcessor::BaseFieldProcessor::AddAttribute(_args ... args)
	{
		if constexpr (std::is_same<_attribute_type, SerializableAttribute>::value)
			return SerializeFieldProcessor(node);
		else
			return *this;
	}

	template<typename _object_type, typename _field_type>
	SerializeTypeProcessor::BaseFieldProcessor& SerializeTypeProcessor::BaseFieldProcessor::FieldBasics(_object_type* object, Type* type, const char* name,
																										void*(*pointerGetter)(void*), _field_type& field)
	{
		return *this;
	}

	template<typename _type>
	SerializeTypeProcessor::BaseFieldProcessor& SerializeTypeProcessor::BaseFieldProcessor::SetDefaultValue(const _type& value)
	{
		return *this;
	}

	template<typename _attribute_type, typename ... _args>
	SerializeTypeProcessor::SerializeFieldProcessor& SerializeTypeProcessor::SerializeFieldProcessor::AddAttribute(_args ... args)
	{
		return *this;
	}

	template<typename _type>
	SerializeTypeProcessor::SerializeDefaultFieldProcessor<_type> SerializeTypeProcessor::SerializeFieldProcessor::SetDefaultValue(const _type& value)
	{
		return SerializeDefaultFieldProcessor<_type>(node, value);
	}

	template<typename _object_type, typename _field_type>
	SerializeTypeProcessor::SerializeFieldProcessor& SerializeTypeProcessor::SerializeFieldProcessor::FieldBasics(_object_type* object, Type* type, const char* name,
																												  void*(*pointerGetter)(void*), _field_type& field)
	{
		_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));

		if constexpr (std::is_default_constructible<_field_type>::value && SupportsEqualOperator<_field_type>::value)
		{
			if (Math::Equals(*fieldPtr, _field_type()))
				return *this;
		}

		node.AddMember(name).Set(*fieldPtr);
		return *this;
	}

	template<typename _type>
	template<typename _object_type, typename _field_type>
	SerializeTypeProcessor::SerializeDefaultFieldProcessor<_type>& SerializeTypeProcessor::SerializeDefaultFieldProcessor<_type>::FieldBasics(_object_type* object, Type* type, const char* name, void*(*pointerGetter)(void*), _field_type& field)
	{
		_field_type* fieldPtr = (_field_type*)((*pointerGetter)(object));
		if (*fieldPtr != defaultValue)
			node.AddMember(name).Set(*fieldPtr);

		return *this;
	}


	// Serialization implementation macros
#define SERIALIZABLE_MAIN(CLASS)  							                                                    \
    IOBJECT_MAIN(CLASS)																							\
                                                                                                                \
    void SerializeBasic(o2::DataValue& node) const override                                                     \
    {						                                                                                    \
    	SerializeTypeProcessor processor(node);																	\
		ProcessBaseTypes(const_cast<CLASS*>(this), processor);													\
		ProcessFields(const_cast<CLASS*>(this), processor);														\
	}	                                                                                                        \
    void Serialize(o2::DataValue& node) const override                                                          \
    {						                                                                                    \
		node.Set(*this);                                                                                        \
	}	                                                                                                        \
    void DeserializeBasic(const o2::DataValue& node) override                                                   \
    {												                                                            \
        node.Get(*this);                                                                                        \
	}																											\
    void Deserialize(const o2::DataValue& node) override                                                        \
    {												                                                            \
        DeserializeBasic(node);                                                                                 \
	}												                                                            \
	CLASS& operator=(const o2::DataValue& node) 		                                                        \
	{												                                                            \
		Deserialize(node); return *this; 			                                                            \
	}                                                                                                           \
	operator DataDocument() const                                                                               \
	{                																							\
		DataDocument doc;																						\
		Serialize(doc);																							\
		return doc;                                                                                             \
	}

#define SERIALIZABLE(CLASS)                                                                                     \
    SERIALIZABLE_MAIN(CLASS)                                                                                    \
		                                                                                                        \
    template<typename _type_processor> static void ProcessBaseTypes(CLASS* object, _type_processor& processor); \
    template<typename _type_processor> static void ProcessFields(CLASS* object, _type_processor& processor);    \
    template<typename _type_processor> static void ProcessMethods(CLASS* object, _type_processor& processor);   

#define SERIALIZABLE_MANUAL(CLASS) \
    SERIALIZABLE(CLASS)

#define SERIALIZABLE_ATTRIBUTE() \
    AddAttribute<SerializableAttribute>()
}

CLASS_BASES_META(o2::ISerializable)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::ISerializable)
{
}
END_META;
CLASS_METHODS_META(o2::ISerializable)
{

	PUBLIC_FUNCTION(void, Serialize, DataValue&);
	PUBLIC_FUNCTION(void, Deserialize, const DataValue&);
	PUBLIC_FUNCTION(String, SerializeToString);
	PUBLIC_FUNCTION(void, DeserializeFromString, const String&);
	PUBLIC_FUNCTION(void, OnSerialize, DataValue&);
	PUBLIC_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, SerializeBasic, DataValue&);
	PROTECTED_FUNCTION(void, DeserializeBasic, const DataValue&);
}
END_META;
