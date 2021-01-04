
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

		// Serializing object delta from origin into data node
		virtual void SerializeDelta(DataValue& node, const IObject& origin) const {}

		// Deserializing object delta from origin from data node
		virtual void DeserializeDelta(const DataValue& node, const IObject& origin) {};

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

		// Beginning serialization delta callback
		virtual void OnSerializeDelta(DataValue& node, const IObject& origin) const {}

		// Completion deserialization delta callback
		virtual void OnDeserializedDelta(const DataValue& node, const IObject& origin) {}

		IOBJECT(ISerializable);

	protected:
		// Serializing object into data node
		virtual void SerializeBasic(DataValue& node) const {}

		// Deserializing object from data node
		virtual void DeserializeBasic(const DataValue& node) {}

		// Serializing object into data node
		virtual void SerializeDeltaBasic(DataValue& node, const IObject& origin) const {}

		// Deserializing object from data node
		virtual void DeserializeDeltaBasic(const DataValue& node, const IObject& origin) {}
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
			value.DeserializeBasic(data);
			value.OnDeserialized(data);
		}
	};

	// ----------------------------
	// Serializable field attribute
	// ----------------------------
	class SerializableAttribute: public IAttribute
	{
		ATTRIBUTE_COMMENT_DEFINITION("SERIALIZABLE");
		ATTRIBUTE_SHORT_DEFINITION("SERIALIZABLE_ATTRIBUTE");
	};

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
        DeserializeTypeProcessor processor(node);															    \
    	ProcessBaseTypes(const_cast<CLASS*>(this), processor);													\
    	ProcessFields(const_cast<CLASS*>(this), processor);														\
	}																											\
    void Deserialize(const o2::DataValue& node) override                                                        \
    {												                                                            \
        node.Get(*this);                                                                                        \
	}												                                                            \
    void SerializeDeltaBasic(o2::DataValue& node, const IObject& origin) const override                         \
    {						                                                                                    \
    	SerializeDeltaTypeProcessor<CLASS> processor(node, dynamic_cast<const CLASS&>(origin));                 \
		ProcessBaseTypes(const_cast<CLASS*>(this), processor);													\
		ProcessFields(const_cast<CLASS*>(this), processor);														\
	}	                                                                                                        \
    void SerializeDelta(o2::DataValue& node, const IObject& origin) const override                              \
    {						                                                                                    \
		node.SetValueDelta(*this, origin);                                                                      \
	}	                                                                                                        \
    void DeserializeDeltaBasic(const o2::DataValue& node, const IObject& origin) override                       \
    {												                                                            \
    	DeserializeDeltaTypeProcessor<CLASS> processor(node, dynamic_cast<const CLASS&>(origin));               \
    	ProcessBaseTypes(const_cast<CLASS*>(this), processor);													\
    	ProcessFields(const_cast<CLASS*>(this), processor);														\
	}																											\
    void DeserializeDelta(const o2::DataValue& node, const IObject& origin) override                            \
    {												                                                            \
        node.GetValueDelta(*this, origin);                                                                      \
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

#include "o2/Utils/Serialization/SerializeFieldProcessors.h"

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
	PUBLIC_FUNCTION(void, SerializeDelta, DataValue&, const IObject&);
	PUBLIC_FUNCTION(void, DeserializeDelta, const DataValue&, const IObject&);
	PUBLIC_FUNCTION(String, SerializeToString);
	PUBLIC_FUNCTION(void, DeserializeFromString, const String&);
	PUBLIC_FUNCTION(void, OnSerialize, DataValue&);
	PUBLIC_FUNCTION(void, OnDeserialized, const DataValue&);
	PUBLIC_FUNCTION(void, OnSerializeDelta, DataValue&, const IObject&);
	PUBLIC_FUNCTION(void, OnDeserializedDelta, const DataValue&, const IObject&);
	PROTECTED_FUNCTION(void, SerializeBasic, DataValue&);
	PROTECTED_FUNCTION(void, DeserializeBasic, const DataValue&);
	PROTECTED_FUNCTION(void, SerializeDeltaBasic, DataValue&, const IObject&);
	PROTECTED_FUNCTION(void, DeserializeDeltaBasic, const DataValue&, const IObject&);
}
END_META;
