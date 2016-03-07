#pragma once

#include "Utils/Data/DataDoc.h"

namespace o2
{
	class Actor;

	// ----------------------------------------
	// Unified type object serializer interface
	// ----------------------------------------
	struct Serializer
	{
		// ------------------------------
		// Dummy serializer. Does nothing
		// ------------------------------
		template<typename T>
		struct Dummy
		{
		public:
			static void Serialize(T& x, DataNode& data) {}
			static void Deserialize(T& x, DataNode& data) {}
		};

		// --------------------------------------------------------------------------------------
		// Regular and fundamental serializer, works for types, that convertible to/from DataNode
		// --------------------------------------------------------------------------------------
		template<typename T>
		struct Fundamental
		{
			static void Serialize(T& x, DataNode& data);
			static void Deserialize(T& x, DataNode& data);
		};

		// Serializes param to data
		template<typename T, typename X = std::conditional<DataNode::IsSupport<T>::value, Fundamental<T>, Dummy<T>>::type>
		static void Serialize(T& param, DataNode& data);

		// Serializes object from param to data
		template<typename T>
		static void Serialize(T* param, DataNode& data);

		// Serializes actor to data (as object or asset)
		static void Serialize(Actor* actor, DataNode& data);

		// Deserializes param from data
		template<typename T, typename X = std::conditional<DataNode::IsSupport<T>::value, Fundamental<T>, Dummy<T>>::type>
		static void Deserialize(T& param, DataNode& data);

		// Deserializes object by param from data
		template<typename T>
		static void Deserialize(T*& param, DataNode& data);

		// Deserializes actor form data (as object or asset)
		static void Deserialize(Actor*& actor, DataNode& data);
	};

	template<typename T>
	void Serializer::Fundamental<T>::Deserialize(T& x, DataNode& data)
	{
		x = data;
	}

	template<typename T>
	void Serializer::Fundamental<T>::Serialize(T& x, DataNode& data)
	{
		data = x;
	}

	template<typename T, typename X /*= SerializerSelector<T>::type*/>
	void Serializer::Serialize(T& param, DataNode& data)
	{
		X::Serialize(param, data);
	}

	template<typename T>
	void Serializer::Serialize(T* param, DataNode& data)
	{
		*data.AddNode("Type") = TypeOf(T).Name();
		*data.AddNode("Value") = param ? Serialize<T>(*param, data) : (String)"null";
	}

	template<typename T>
	void Serializer::Deserialize(T*& param, DataNode& data)
	{
		String type;
		ISerializable* value = nullptr;

		if (auto typeNode = data.GetNode("Type"))
		{
			type = *typeNode;

			if (auto valueNode = data.GetNode("Value"))
			{
				if (valueNode->Data() == "null")
					*param = nullptr;
				else
				{
					param = static_cast<ISerializable*>(Reflection::CreateTypeSample(type));
					*param = *valueNode;
				}
			}
		}
	}

	template<typename T, typename X /*= SerializerSelector<T>::type*/>
	void Serializer::Deserialize(T& param, DataNode& data)
	{
		X::Deserialize(param, data);
	}
}
