#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Data/DataDoc.h"
#include "Utils/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/String.h"

namespace o2
{
	// -----------------------------
	// Serializable object interface
	// -----------------------------
	class ISerializable: public IObject
	{
	public:
		// Serializing object into data node
		virtual DataNode Serialize() { return DataNode(); };

		// Deserializing object from data node
		virtual void Deserialize(const DataNode& node) {};

		// DataNode converting operator
		virtual operator DataNode() { return Serialize(); };

		// Assign operator from data node
		virtual ISerializable& operator=(const DataNode& node) { return *this; };

		IOBJECT(ISerializable);

	protected:
		// Beginning serialization callback
		virtual void OnSerialize(DataNode& node) {}

		// Completion deserialization callback
		virtual void OnDeserialized(const DataNode& node) {}
	};

	// --------------------------------------
	// Serializable field attribute interface
	// --------------------------------------
	class ISerializableAttribute: public IAttribute
	{
	public:
		virtual void Serialize(void* object, DataNode& data) const = 0;
		virtual void Deserialize(void* object, const DataNode& data) const = 0;
	};

	// -----------------------------------
	// Specialized serialization attribute
	// -----------------------------------
	template<typename _type>
	class SerializableAttribute: public ISerializableAttribute
	{
	public:
		void Serialize(void* object, DataNode& data) const;
		void Deserialize(void* object, const DataNode& data) const;

		IAttribute* Clone() const { return mnew SerializableAttribute(*this); }
	};

// Serialization implementation macros
#define SERIALIZABLE(CLASS)                                                                \
    CLASS* Clone() const { return mnew CLASS(*this); }                                     \
	static Type* type;								                                       \
	const Type& GetType() const { return *type; };	                                       \
	DataNode Serialize()                                                        		   \
	{                                                              						   \
		DataNode res;																	   \
		OnSerialize(res);																   \
		for (auto field : type->Fields())												   \
		{																				   \
			ISerializableAttribute* srlzAttr = field->Attribute<ISerializableAttribute>(); \
			if (srlzAttr)																   \
				srlzAttr->Serialize(this, res);										       \
		}																				   \
																						   \
		return res;																		   \
	}																					   \
	void Deserialize(const DataNode& node)												   \
	{																					   \
		for (auto field : type->Fields())												   \
		{																				   \
			ISerializableAttribute* srlzAttr = field->Attribute<ISerializableAttribute>(); \
			if (srlzAttr)																   \
				srlzAttr->Deserialize(this, node);										   \
		}																				   \
		OnDeserialized(node);   														   \
	}																					   \
	CLASS& operator=(const DataNode& node) 												   \
	{																					   \
		Deserialize(node); return *this; 												   \
	} 																					   \
	operator DataNode() 																   \
	{ 																					   \
		return Serialize(); 															   \
	}                                                                                      \
	static void InitializeType(CLASS* sample)   

	template<typename _type>
	void SerializableAttribute<_type>::Deserialize(void* object, const DataNode& data) const
	{
		auto node = data.GetNode(mOwnerFieldInfo->Name());
		if (node)
		{
			_type* valPtr = mOwnerFieldInfo->GetValuePtrStrong<_type>(object);
			*valPtr = *node;
		}
	}

	template<typename _type>
	void SerializableAttribute<_type>::Serialize(void* object, DataNode& data) const
	{
		*data.AddNode(mOwnerFieldInfo->Name()) = mOwnerFieldInfo->GetValue<_type>(object);
	}
}