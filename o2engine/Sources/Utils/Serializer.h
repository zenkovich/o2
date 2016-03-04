#pragma once

#include "Utils/Data/DataDoc.h"

namespace o2
{
	class Actor;

	struct Serializer
	{
		template<typename T>
		struct Dummy
		{
		public:
			static void Serialize(T& x, DataNode& data)
			{
				//printf("Dummy %s\n", typeid(T).name());
			}

			static void Deserialize(T& x, DataNode& data)
			{
				//printf("Dummy %s\n", typeid(T).name());
			}
		};

		template<typename T>
		struct Fundamental
		{
			static void Serialize(T& x, DataNode& data)
			{
				//printf("Fundamental %s\n", typeid(T).name());
				data = x;
			}

			static void Deserialize(T& x, DataNode& data)
			{
				//printf("Fundamental %s\n", typeid(T).name());
				x = data;
			}
		};

		template<typename T, typename X = std::conditional<DataNode::IsSupport<T>::value, Fundamental<T>, Dummy<T>>::type>
		struct TSerializer { typedef X type; };

		template<typename T, typename X = TSerializer<T>::type>
		static void Serialize(T& param, DataNode& data)
		{
			X::Serialize(param, data);
		}

		template<typename T>
		static void Serialize(T* param, DataNode& data)
		{
			*data.AddNode("Type") = TypeOf(T).Name();
			*data.AddNode("Value") = param ? Serialize<T>(*param, data) : (String)"null";
		}

		static void Serialize(Actor* actor, DataNode& data);

		template<typename T, typename X = TSerializer<T>::type>
		static void Deserialize(T& param, DataNode& data)
		{
			X::Deserialize(param, data);
		}

		template<typename T>
		static void Deserialize(T*& param, DataNode& data)
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

		static void Deserialize(Actor*& actor, DataNode& data);
	};
}
