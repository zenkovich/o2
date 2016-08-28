
#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Data/DataNode.h"
#include "Utils/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/String.h"
#include "Utils/Singleton.h"

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
		virtual operator DataNode() { return Serialize(); };

		// Assign operator from data node
		virtual ISerializable& operator=(const DataNode& node) { return *this; };

		IOBJECT(ISerializable);

	protected:
		// Beginning serialization callback
		virtual void OnSerialize(DataNode& node) const {}

		// Completion deserialization callback
		virtual void OnDeserialized(const DataNode& node) {}

		// Serializing object into data node
		DataNode SerializeBasic(const void* thisObject) const;

		// Deserializing object from data node
		void DeserializeBasic(const DataNode& node, const void* thisObject);
	};

	// ----------------------------
	// Serializable field attribute
	// ----------------------------
	class SerializableAttribute: public IAttribute
	{
		// Returns copy of this
		IAttribute* Clone() const { return new SerializableAttribute(*this); }
	};

	// Serialization implementation macros
#define SERIALIZABLE(CLASS)                            \
private:                                               \
	static o2::Type* type;							   \
                                                       \
    template<typename _type>                           \
	friend const o2::Type& o2::GetTypeOf();            \
                                                       \
    template<typename _type>                           \
    friend struct o2::Type::SampleCreator;             \
                                                       \
    friend class o2::TypeInitializer;                  \
    friend class o2::Reflection;                       \
    friend class o2::DataNode;                         \
                                                       \
public:                                                \
	CLASS* Clone() const { return mnew CLASS(*this); } \
	o2::Type& GetType() const { return *type; };       \
    o2::DataNode Serialize() const                     \
    {												   \
        return SerializeBasic((const void*)this);      \
	}												   \
    void Deserialize(const o2::DataNode& node)         \
    {												   \
        DeserializeBasic(node, (const void*)this);     \
	}												   \
	CLASS& operator=(const o2::DataNode& node) 		   \
	{												   \
		Deserialize(node); return *this; 			   \
	} 												   \
	operator o2::DataNode() 						   \
	{ 												   \
		return Serialize(); 						   \
	}            									   \
                                                       \
private:                                               \
	static void InitializeType(o2::Type* type)      
}