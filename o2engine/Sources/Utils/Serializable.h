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

	// ----------------------------
	// Serializable field attribute
	// ----------------------------
	class SerializableAttribute: public IAttribute
	{
		// Returns copy of this
		IAttribute* Clone() const { return new SerializableAttribute(*this); }
	};

	// Serialization implementation macros
#define SERIALIZABLE(CLASS)                                                                                            \
private:                                                                                                               \
	static Type type;								                                                                   \
                                                                                                                       \
	friend struct o2::Type::TypeCreator<CLASS>;                                                                        \
                                                                                                                       \
    template<typename _type>                                                                                           \
	friend const Type& o2::_TypeOf();                                                                                  \
    friend class o2::TypeInitializer;                                                                                  \
    friend class o2::Reflection;                                                                                       \
    template<typename _type>                                                                                           \
    friend struct o2::Type::TypeCreator;                                                                               \
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
			auto srlzAttribute = field->Attribute<SerializableAttribute>();										       \
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
			auto srlzAttribute = field->Attribute<SerializableAttribute>();										       \
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
}