#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Data/DataDoc.h"
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
		virtual DataNode Serialize() { return DataNode(); }

		// Deserializing object from data node
		virtual void Deserialize(const DataNode& node) {}

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
#define SERIALIZABLE(CLASS)                                                                                            \
private:                                                                                                               \
	static Type type;								                                                                   \
                                                                                                                       \
	friend struct o2::Type::TypeAgent<CLASS>;                                                                          \
                                                                                                                       \
    template<typename _type>                                                                                           \
	friend const Type& o2::_TypeOf();                                                                                  \
    friend class o2::TypeInitializer;                                                                                  \
    friend class o2::Reflection;                                                                                       \
    template<typename _type>                                                                                           \
    friend struct o2::Type::TypeAgent;                                                                                 \
    friend class o2::DataNode;                                                                                         \
                                                                                                                       \
public:                                                                                                                \
	CLASS* Clone() const { return mnew CLASS(*this); }                                                                 \
	Type& GetType() const { return type; };	                                                                           \
	CLASS& operator=(const DataNode& node) 			                                                                   \
	{												                                                                   \
		Deserialize(node); return *this; 			                                                                   \
	} 												                                                                   \
	operator DataNode() 							                                                                   \
	{ 												                                                                   \
		return Serialize(); 						                                                                   \
	}                                                                                                                  \
    DataNode Serialize()																				               \
	{																												   \
		DataNode res;																								   \
		OnSerialize(res);																							   \
		for (auto field : GetType().Fields())																		   \
		{																											   \
			auto srlzAttribute = field->Attribute<ISerializableAttribute>();										   \
			if (srlzAttribute)																						   \
			{																										   \
				field->SerializeObject((void*)field->GetValuePtrStrong<char>(this), *res.AddNode(field->Name()));      \
			}																										   \
		}																											   \
																													   \
		return res;																									   \
	}																												   \
																													   \
	void Deserialize(const DataNode& node)															                   \
	{																												   \
		for (auto field : GetType().Fields())																		   \
		{																											   \
			auto srlzAttribute = field->Attribute<ISerializableAttribute>();										   \
			if (srlzAttribute)																						   \
			{																										   \
				auto fldNode = node.GetNode(field->Name());															   \
				if (fldNode)																						   \
				{																									   \
					field->DeserializeObject((void*)field->GetValuePtrStrong<char>(this), *fldNode);			       \
				}																									   \
			}																										   \
		}																											   \
		OnDeserialized(node);																						   \
	}																												   \
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