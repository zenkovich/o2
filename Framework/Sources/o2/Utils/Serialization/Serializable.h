
#pragma once

#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Serialization/DataNode.h"
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
		virtual DataNode Serialize() const { return DataNode(); };

		// Deserializing object from data node
		virtual void Deserialize(const DataNode& node) {};

		// DataNode converting operator
		operator DataNode() { return Serialize(); };

		// Assign operator from data node
		ISerializable& operator=(const DataNode& node) { return *this; };

		// Beginning serialization callback
		virtual void OnSerialize(DataNode& node) const {}

		// Completion deserialization callback
		virtual void OnDeserialized(const DataNode& node) {}

		IOBJECT(ISerializable);

	protected:
		// Serializing object into data node
		void SerializeBasic(const IObject& thisObject, DataNode& node) const;

		// Deserializing object from data node
		void DeserializeBasic(IObject& thisObject, const DataNode& node);
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
#define SERIALIZABLE(CLASS)  							                                                        \
private:                                                                                                        \
	static o2::Type* type;							                                                            \
                                                                                                                \
    template<typename __type, typename _getter>                                                                 \
	friend const o2::Type& o2::GetTypeOf();                                                                     \
                                                                                                                \
	template<typename T>                                                                                        \
	friend struct o2::RegularTypeGetter;                                                                        \
                                                                                                                \
	template<typename T, typename X>                                                                            \
	friend struct o2::GetTypeHelper;                                                                            \
                                                                                                                \
	template<typename __type>                                                                                   \
	friend class o2::TObjectType;                                                                               \
                                                                                                                \
    template<typename __type>                                                                                   \
	friend class PointerValueProxy;                                                                            \
                                                                                                                \
    friend class o2::TypeInitializer;                                                                           \
    friend class o2::Reflection;                                                                                \
    friend class o2::DataNode;                                                                                  \
                                                                                                                \
public:                                                                                                         \
	typedef CLASS thisclass;                                                                                    \
	IObject* Clone() const override { return mnew CLASS(*this); }                                               \
	const o2::Type& GetType() const override { return *type; };                                                 \
                                                                                                                \
    template<typename _type_processor> static void ProcessType(CLASS* object, _type_processor& processor)       \
	{                                                                                                           \
		processor.template Start<CLASS>(object, type);                                                          \
		ProcessBaseTypes<_type_processor>(object, processor);                                                   \
		ProcessFields<_type_processor>(object, processor);                                                      \
		ProcessMethods<_type_processor>(object, processor);                                                     \
	}                                                                                                           \
		                                                                                                        \
    template<typename _type_processor> static void ProcessBaseTypes(CLASS* object, _type_processor& processor); \
    template<typename _type_processor> static void ProcessFields(CLASS* object, _type_processor& processor);    \
    template<typename _type_processor> static void ProcessMethods(CLASS* object, _type_processor& processor);   \
                                                                                                                \
    o2::DataNode Serialize() const override                                                                     \
    {												                                                            \
        o2::DataNode res;                                                                                       \
        SerializeBasic(*this, res);                                                                             \
        return res;                                                                                             \
	}												                                                            \
    void Deserialize(const o2::DataNode& node) override                                                         \
    {												                                                            \
        DeserializeBasic(*this, node);                                                                          \
	}												                                                            \
	CLASS& operator=(const o2::DataNode& node) 		                                                            \
	{												                                                            \
		Deserialize(node); return *this; 			                                                            \
	} 												                                                            \
	operator o2::DataNode() 						                                                            \
	{ 												                                                            \
		return Serialize(); 						                                                            \
	}            									                                                            

#define SERIALIZABLE_ATTRIBUTE() \
    AddAttribute(new SerializableAttribute())
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

	PUBLIC_FUNCTION(DataNode, Serialize);
	PUBLIC_FUNCTION(void, Deserialize, const DataNode&);
	PUBLIC_FUNCTION(void, OnSerialize, DataNode&);
	PUBLIC_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, SerializeBasic, const IObject&, DataNode&);
	PROTECTED_FUNCTION(void, DeserializeBasic, IObject&, const DataNode&);
}
END_META;
