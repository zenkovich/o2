#pragma once

#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/String.h"
#include "o2/Utils/Types/UID.h"
#include "o2/Utils/Property.h"

namespace o2
{
	class Actor;
	class Component;
	class DataNode;
	class IObject;
	class ISerializable;
	class Type;

	// -------------------
	// Tree-like data node
	// -------------------
	class DataNode
	{
	protected:
		// Specialize this template class for your custom serialization types
		template<typename _type, typename _enable = void>
		struct Converter
		{
			static constexpr bool isSupported = false;
			using __type = typename std::conditional<std::is_same<void, _type>::value, int, _type>::type;

			static void Write(const __type& value, DataNode& data) {}
			static void Read(__type& value, const DataNode& data) {}
		};

	public:
		enum class Format { Xml, JSON, Binary };

		typedef Vector<DataNode*> DataNodesVec;
		typedef DataNodesVec::Iterator Iterator;
		typedef DataNodesVec::ConstIterator ConstIterator;

		// Is DataNode supporting type trait
		template<typename _type>
		struct IsSupport
		{
			static constexpr bool value = Converter<_type>::isSupported;
		};

	public:
		// Default constructor
		DataNode();

		// Constructor with name
		explicit DataNode(const WString& name);

		// Copy-constructor
		DataNode(const DataNode& other);

		// Destructor
		virtual ~DataNode();

		// Assign operator
		DataNode& operator=(const DataNode& value);

		// Assign operator
		template<typename _type>
		DataNode& operator=(const _type& value);

		// Cast to type operator
		template<typename _type>
		operator _type() const;

		// Sets value. Using DataNodeConverter specializations
		template<typename _type>
		DataNode& SetValue(const _type& value);

		// Gets value. Using DataNodeConverter specializations
		template<typename _type>
		void GetValue(_type& value) const;

		// Sets objects with delta from source object
		DataNode& SetValueDelta(const IObject& object, const IObject& source);

		// Gets objects with delta from source object
		void GetValueDelta(IObject& object, const IObject& source) const;

		// [] assign operator. nodePath sample: "node/node/abc/cde"
		DataNode& operator[](const WString& nodePath);

		// [] assign operator. nodePath sample: "node/node/abc/cde"
		DataNode& operator[](const char* nodePath);

		// Equals operator
		bool operator==(const DataNode& other) const;

		// Not equals operator
		bool operator!=(const DataNode& other) const;

		// Returns is node data empty and have no children
		bool IsEmpty() const;

		// Removes all children
		void Clear();

		// Return parent node
		DataNode* GetParent() const;

		// Return node by path. nodePath sample: "node/node/abc/cde"
		DataNode* GetNode(const WString& nodePath) const;

		// Add new node with name
		DataNode* AddNode(const WString& name);

		// Add node
		DataNode* AddNode(DataNode* node);

		// Removes node
		bool RemoveNode(DataNode* node);

		// Removes node by name
		bool RemoveNode(const WString& name);

		// Returns name of node
		WString GetName() const;

		// Sets name of node
		void SetName(const WString& name);

		// Returns data reference
		WString& Data();

		// Returns constant data reference
		const WString& Data() const;

		// Returns constant reference to children list
		const DataNode::DataNodesVec& GetChildNodes() const;

		// Loads data structure from file
		bool LoadFromFile(const String& fileName);

		// Loads data structure from string
		bool LoadFromData(const WString& data);

		// Saves data to file with specified format
		bool SaveToFile(const String& fileName, Format format = Format::Xml) const;

		// Saves data to string
		WString SaveAsWString(Format format = Format::Xml) const;

		// Begin iterator
		Iterator Begin();

		// End iterator
		Iterator End();

		// Begin constant iterator
		ConstIterator Begin() const;

		// End constant iterator
		ConstIterator End() const;

		// Begin iterator (for range based "for")
		Iterator begin();

		// End iterator (for range based "for")
		Iterator end();

		// Begin constant iterator (for range based "for")
		ConstIterator begin() const;

		// End constant iterator (for range based "for")
		ConstIterator end() const;

	protected:
		WString      mName;       // Name of node
		WString      mData;       // Node data
		DataNode*    mParent;     // Node parent
		DataNodesVec mChildNodes; // Children nodes
	};
}

#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
	template<typename _type>
	void DataNode::GetValue(_type& value) const
	{
		Converter<_type>::Read(value, *this);
	}

	template<typename _type>
	DataNode& DataNode::SetValue(const _type& value)
	{
		Converter<_type>::Write(value, *this);
		return *this;
	}

	template<typename _type>
	DataNode::operator _type() const
	{
		_type result;
		GetValue(result);
		return result;
	}

	template<typename _type>
	DataNode& DataNode::operator=(const _type& value)
	{
		SetValue(value);
		return *this;
	}

	template<>
	struct DataNode::Converter<DataNode>
	{
		static constexpr bool isSupported = true;

		static void Write(const DataNode& value, DataNode& data)
		{
			for (auto child : data.mChildNodes)
				delete child;

			data.mChildNodes.Clear();

			for (auto child : value.mChildNodes)
				data.mChildNodes.Add(mnew DataNode(*child));

			data.mData = value.mData;
		}

		static void Read(DataNode& value, const DataNode& data)
		{
			Write(data, value);
		}
	};

	template<>
	struct DataNode::Converter<char*>
	{
		static constexpr bool isSupported = true;

		using charPtr = char*;

		static void Write(const charPtr& value, DataNode& data)
		{
			data.mData = value;
		}

		static void Read(charPtr& value, const DataNode& data)
		{
			memcpy(value, ((String)data.mData).Data(), sizeof(char)*data.mData.Length());
		}
	};

	template<>
	struct DataNode::Converter<wchar_t*>
	{
		static constexpr bool isSupported = true;

		using wcharPtr = wchar_t*;

		static void Write(const wcharPtr& value, DataNode& data)
		{
			data.mData = value;
		}

		static void Read(wcharPtr value, const DataNode& data)
		{
			memcpy(value, data.mData.Data(), sizeof(wchar_t)*data.mData.Length());
		}
	};

	template<>
	struct DataNode::Converter<bool>
	{
		static constexpr bool isSupported = true;

		static void Write(const bool& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(bool& value, const DataNode& data)
		{
			value = (bool)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<int>
	{
		static constexpr bool isSupported = true;

		static void Write(const int& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(int& value, const DataNode& data)
		{
			value = (int)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<float>
	{
		static constexpr bool isSupported = true;

		static void Write(const float& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(float& value, const DataNode& data)
		{
			value = (float)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<UInt>
	{
		static constexpr bool isSupported = true;

		static void Write(const UInt& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(UInt& value, const DataNode& data)
		{
			value = (UInt)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<UInt64>
	{
		static constexpr bool isSupported = true;

		static void Write(const UInt64& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(UInt64& value, const DataNode& data)
		{
			value = (UInt64)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<String>
	{
		static constexpr bool isSupported = true;

		static void Write(const String& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(String& value, const DataNode& data)
		{
			value = (String)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<WString>
	{
		static constexpr bool isSupported = true;

		static void Write(const WString& value, DataNode& data)
		{
			data.mData = value;
		}

		static void Read(WString& value, const DataNode& data)
		{
			value = data.mData;
		}
	};

	template<>
	struct DataNode::Converter<UID>
	{
		static constexpr bool isSupported = true;

		static void Write(const UID& value, DataNode& data)
		{
			data.mData = (String)value;
		}

		static void Read(UID& value, const DataNode& data)
		{
			value = data.mData;
		}
	};

	template<>
	struct DataNode::Converter<Vec2F>
	{
		static constexpr bool isSupported = true;

		static void Write(const Vec2F& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(Vec2F& value, const DataNode& data)
		{
			value = (Vec2F)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<Vec2I>
	{
		static constexpr bool isSupported = true;

		static void Write(const Vec2I& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(Vec2I& value, const DataNode& data)
		{
			value = (Vec2I)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<RectF>
	{
		static constexpr bool isSupported = true;

		static void Write(const RectF& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(RectF& value, const DataNode& data)
		{
			value = (RectF)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<RectI>
	{
		static constexpr bool isSupported = true;

		static void Write(const RectI& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(RectI& value, const DataNode& data)
		{
			value = (RectI)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<BorderF>
	{
		static constexpr bool isSupported = true;

		static void Write(const BorderF& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(BorderF& value, const DataNode& data)
		{
			value = (BorderF)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<BorderI>
	{
		static constexpr bool isSupported = true;

		static void Write(const BorderI& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(BorderI& value, const DataNode& data)
		{
			value = (BorderI)data.mData;
		}
	};

	template<>
	struct DataNode::Converter<Color4>
	{
		static constexpr bool isSupported = true;

		static void Write(const Color4& value, DataNode& data)
		{
			data.mData = (WString)value;
		}

		static void Read(Color4& value, const DataNode& data)
		{
			value = (Color4)data.mData;
		}
	};

	template<typename T>
	struct DataNode::Converter<T, typename std::enable_if<std::is_pointer<T>::value && !std::is_const<T>::value &&
		std::is_base_of<o2::IObject, typename std::remove_pointer<T>::type>::value>::type>
	{
		static constexpr bool isSupported = true;

		static void Write(const T& value, DataNode& data)
		{
			if (value)
			{
				data.AddNode("Type")->SetValue(value->GetType().GetName());
				data.AddNode("Value")->SetValue(*value);
			}
		}

		static void Read(T& value, const DataNode& data)
		{
			if (auto typeNode = data.GetNode("Type"))
			{
				String typeName = *typeNode;

				if (auto valueNode = data.GetNode("Value"))
				{
					auto type = Reflection::GetType(typeName);
					void* sample = type->CreateSample();
					if (type->GetUsage() == Type::Usage::Object)
					{
						auto objectType = dynamic_cast<const ObjectType*>(type);
						value = dynamic_cast<T>(objectType->DynamicCastToIObject(sample));
					}
					else
						value = static_cast<T>(sample);

					if (value)
						valueNode->GetValue(*value);
				}
			}
		}
	};

	template<typename T>
	struct DataNode::Converter<T, typename std::enable_if<std::is_pointer<T>::value && !std::is_const<T>::value && 
		!std::is_base_of<o2::IObject, typename std::remove_pointer<T>::type>::value>::type>
	{
		static constexpr bool isSupported = DataNode::Converter<std::remove_pointer<T>::type>::isSupported;

		static void Write(const T& value, DataNode& data)
		{
			DataNode::Converter<std::remove_pointer<T>::type>::Write(*value, data);
		}

		static void Read(T& value, const DataNode& data)
		{
			DataNode::Converter<std::remove_pointer<T>::type>::Read(*value, data);
		}
	};

	template<typename T>
	struct DataNode::Converter<Vector<T>>
	{
		static constexpr bool isSupported = true;

		static void Write(const Vector<T>& value, DataNode& data)
		{
			data.Clear();

			for (auto v : value)
				data.AddNode("Element")->SetValue(v);
		}

		static void Read(Vector<T>& value, const DataNode& data)
		{
			value.Clear();

			for (auto childNode : data.mChildNodes)
			{
				T v = T();
				childNode->GetValue(v);
				value.Add(v);
			}
		}
	};

	template<typename _key, typename _value>
	struct DataNode::Converter<Map<_key, _value>>
	{
		static constexpr bool isSupported = true;

		static void Write(const Map<_key, _value>& value, DataNode& data)
		{
			data.Clear();

			for (auto& kv : value)
			{
				DataNode* child = data.AddNode("Element");
				child->AddNode("Key")->SetValue(kv.first);
				child->AddNode("Value")->SetValue(kv.second);
			}
		}

		static void Read(Map<_key, _value>& value, const DataNode& data)
		{
			value.Clear();

			for (auto childNode : data.mChildNodes)
			{
				auto keyNode = childNode->GetNode("Key");
				auto valueNode = childNode->GetNode("Value");

				if (keyNode && valueNode)
				{
					_value v = _value();
					_key k = _key();
					keyNode->GetValue(k);
					valueNode->GetValue(v);
					value.Add(k, v);
				}
			}
		}
	};

	template<typename T>
	struct DataNode::Converter<T, typename std::enable_if<std::is_enum<T>::value>::type>
	{
		static constexpr bool isSupported = true;

		static void Write(const T& value, DataNode& data)
		{
			data.mData = Reflection::GetEnumName<T>(value);
		}

		static void Read(T& value, const DataNode& data)
		{
			value = Reflection::GetEnumValue<T>(data.mData);
		}
	};

	template<typename T>
	struct DataNode::Converter<T, typename std::enable_if<IsProperty<T>::value>::type>
	{
		static constexpr bool isSupported = DataNode::IsSupport<T::valueType>::value;
		using TValueType = typename T::valueType;

		static void Write(const T& value, DataNode& data)
		{
			data.SetValue(value.Get());
		}

		static void Read(T& value, const DataNode& data)
		{
			TValueType val;
			data.GetValue(val);
			value.Set(val);
		}
	};

	template<typename T>
	struct DataNode::Converter<T, typename std::enable_if<std::is_base_of<IObject, T>::value || std::is_same<IObject, T>::value>::type>
	{
		static constexpr bool isSupported = true;

		static void Write(const T& value, DataNode& data)
		{
			struct helper
			{
				static void WriteObject(void* object, const ObjectType& type, DataNode& node)
				{
					for (auto baseType : type.GetBaseTypes())
					{
						const ObjectType* baseObjectType = dynamic_cast<const ObjectType*>(baseType.type);
						if (!baseObjectType)
							continue;

						void* baseObject = (*baseType.dynamicCastUpFunc)(object);
						WriteObject(baseObject, *baseObjectType, node);
					}

					for (auto field : type.GetFields())
					{
						auto srlzAttribute = field->GetAttribute<SerializableAttribute>();
						if (srlzAttribute)
							field->SerializeFromObject(object, *node.AddNode(field->GetName()));
					}
				}
			};

			if (value.GetType().IsBasedOn(TypeOf(ISerializable)))
				dynamic_cast<const ISerializable&>(value).OnSerialize(data);

			const ObjectType& type = dynamic_cast<const ObjectType&>(value.GetType());
			void* objectPtr = type.DynamicCastFromIObject(const_cast<IObject*>(dynamic_cast<const IObject*>(&value)));

			helper::WriteObject(objectPtr, type, data);
		}

		static void Read(T& value, const DataNode& data)
		{
			struct helper
			{
				static void ReadObject(void* object, const ObjectType& type, const DataNode& node)
				{
					for (auto baseType : type.GetBaseTypes())
					{
						const ObjectType* baseObjectType = dynamic_cast<const ObjectType*>(baseType.type);
						if (!baseObjectType)
							continue;

						void* baseObject = (*baseType.dynamicCastUpFunc)(object);
						ReadObject(baseObject, *baseObjectType, node);
					}

					for (auto field : type.GetFields())
					{
						auto srlzAttribute = field->GetAttribute<SerializableAttribute>();
						if (srlzAttribute)
						{
							auto fldNode = node.GetNode(field->GetName());
							if (fldNode)
								field->DeserializeFromObject(object, *fldNode);
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
}
