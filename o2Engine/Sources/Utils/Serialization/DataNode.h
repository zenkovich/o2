#pragma once

#include "Utils/Types/Containers/Dictionary.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Types/String.h"
#include "Utils/Types/UID.h"
#include "Utils/Property.h"

namespace o2
{
	class Actor;
	class Component;
	class DataNode;
	class IObject;
	class ISerializable;
	class Type;

	// -------------------------------
	// Custom data converter interface
	// -------------------------------
	class IDataNodeTypeConverter
	{
	public:
		// Converts data to DataNode
		virtual void ToData(const void* object, DataNode& data) {}

		// Converts from DataNode to object
		virtual void FromData(void* object, const DataNode& data) {}

		// Checks that type is supports by this converter
		virtual bool IsConvertsType(const Type* type) const { return false; }
	};

	// -------------------
	// Tree-like data node
	// -------------------
	class DataNode
	{
	public:
		enum class Format { Xml, JSON, Binary };

		typedef Vector<DataNode*> DataNodesVec;
		typedef DataNodesVec::Iterator Iterator;
		typedef DataNodesVec::ConstIterator ConstIterator;

		// Is DataNode supporting type trait
		template<typename T>
		struct IsSupportBasic :
			std::integral_constant<bool,
			std::is_fundamental<T>::value ||
			 std::is_enum<T>::value ||
			 std::is_base_of<ISerializable, T>::value ||
			 std::is_base_of<ISerializable, typename std::remove_pointer<T>::type>::value ||
			 std::is_same<Color4, T>::value ||
			 std::is_same<RectI, T>::value ||
			 std::is_same<RectF, T>::value ||
			 std::is_same<BorderI, T>::value ||
			 std::is_same<BorderF, T>::value ||
			 std::is_same<Vec2I, T>::value ||
			 std::is_same<Vec2F, T>::value ||
			 std::is_same<String, T>::value ||
			 std::is_same<WString, T>::value ||
			 std::is_same<UID, T>::value ||
			 std::is_same<DataNode, T>::value>
		{};

		template<typename T>
		struct IsSupport :
			std::integral_constant<bool,
			(IsSupportBasic<T>::value || (IsProperty<T>::value && IsSupportBasic<typename ExtractPropertyValueType<T>::type>::value)) 
			&& !std::is_const<T>::value && !std::is_const<typename std::remove_pointer<T>::type>::value>
		{};

		template<typename T2>
		struct IsSupport<Vector<T2>> : IsSupport<T2> {};

		template<typename T2, typename T3>
		struct IsSupport<Dictionary<T2, T3>> : std::integral_constant<bool, IsSupport<T2>::value && IsSupport<T3>::value> {};

	protected:
		// --------------------
		// Enums data converter
		// --------------------
		template<typename _type>
		class EnumDataConverter
		{
		public:
			static void ToData(const _type& object, DataNode& data);
			static void FromData(_type& object, const DataNode& data);
		};

		// ----------------------------
		// IObject types data converter
		// ----------------------------
		template<typename _type>
		class ObjectDataConverter
		{
		public:
			static void ToData(const _type& object, DataNode& data);
			static void FromData(_type& object, const DataNode& data);
		};

		// ----------------------------
		// Property types data converter
		// ----------------------------
		template<typename _type>
		class PropertyDataConverter
		{
		public:
			static void ToData(const _type& object, DataNode& data);
			static void FromData(_type& object, const DataNode& data);
		};

		// -------------------------------
		// Returns data converter by _type
		// -------------------------------
		template<typename _type>
		struct ConverterSelector
		{
			typedef typename std::conditional<std::is_enum<_type>::value, EnumDataConverter<_type>,
				typename std::conditional<(IsProperty<_type>::value && IsSupport<typename ExtractPropertyValueType<_type>::type>::value), PropertyDataConverter<_type>, ObjectDataConverter<_type>>::type
			>::type type;
		};

		// --------------------------------------------------
		// Returns when _type is supported by data converters
		// --------------------------------------------------
		template<typename _type>
		struct IsSupportsConverter : std::integral_constant<bool, std::is_enum<_type>::value || std::is_base_of<IObject, _type>::value || (IsProperty<_type>::value && IsSupport<typename ExtractPropertyValueType<_type>::type>::value)>
		{};

	public:
		// Default constructor
		DataNode();

		// Constructor with name
		DataNode(const WString& name);

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

		// Sets value from other node
		DataNode& SetValue(const DataNode& other);

		// Sets value from object withous custom converters
		DataNode& SetValueRaw(const IObject& object);

		// Sets value as difference between object and source
		DataNode& SetValueDelta(const IObject& object, const IObject& source);

		// Assign operator string value
		DataNode& SetValue(char* value);

		// Assign operator wide string value
		DataNode& SetValue(wchar_t* value);

		// Sets value from integer value
		DataNode& SetValue(int value);

		// Sets value from integer value
		DataNode& SetValue(unsigned long value);

		// Sets value from integer value
		DataNode& SetValue(long long int value);

		// Sets value from integer value
		DataNode& SetValue(UInt64 value);

		// Sets value from boolean value
		DataNode& SetValue(bool value);

		// Sets value from float value
		DataNode& SetValue(float value);

		// Sets value from float value
		DataNode& SetValue(double value);

		// Sets value from unsigned integer value
		DataNode& SetValue(UInt value);

		// Sets value from string value
		DataNode& SetValue(const String& value);

		// Sets value from wide string value
		DataNode& SetValue(const WString& value);

		// Sets value from float vector value
		DataNode& SetValue(const Vec2F& value);

		// Sets value from integer vector value
		DataNode& SetValue(const Vec2I& value);

		// Sets value from float rectangle value
		DataNode& SetValue(const RectF& value);

		// Sets value from integer rectangle value
		DataNode& SetValue(const RectI& value);

		// Sets value from float border value
		DataNode& SetValue(const BorderF& value);

		// Sets value from integer border value
		DataNode& SetValue(const BorderI& value);

		// Sets value from color value
		DataNode& SetValue(const Color4& value);

		// Sets value from UID value
		DataNode& SetValue(const UID& value);

		// Sets value from pointer value, only for objects, based on IObject
		template<typename _type, typename X = typename std::enable_if<std::is_base_of<IObject, _type>::value>::type>
		DataNode& SetValue(const _type* value);

		// Sets value from pointer value, only for objects, based on IObject
		template<typename _type, typename X = typename std::enable_if<std::is_base_of<IObject, _type>::value>::type>
		DataNode& SetValueRaw(const _type* value);

		// Sets value from vector value
		template<typename _type, typename X = typename std::enable_if<std::is_base_of<IObject, _type>::value || std::is_base_of<IObject, typename std::remove_pointer<_type>::type>::value>::type>
		DataNode& SetValueRaw(const Vector<_type>& value);

		// Sets value from vector value
		template<typename _type>
		DataNode& SetValue(const Vector<_type>& value);

		// Sets value from dictionary value
		template<typename _key, typename _value>
		DataNode& SetValue(const Dictionary<_key, _value>& value);

		// Sets value from enum class or IObject based value
		template<typename _type, typename _conv = typename ConverterSelector<_type>::type, typename X = typename std::enable_if<IsSupportsConverter<_type>::value>::type>
		DataNode& SetValue(const _type& value);

		// Gets value
		void GetValue(DataNode& other) const;

		// Gets value as wide string
		void GetValue(wchar_t*& value) const;

		// Gets value as boolean
		void GetValue(bool& value) const;

		// Gets value as integer
		void GetValue(int& value) const;

		// Gets value as float
		void GetValue(float& value) const;

		// Gets value as unsigned integer
		void GetValue(UInt& value) const;

		// Gets value as unsigned integer
		void GetValue(UInt64& value) const;

		// Gets value as string
		void GetValue(String& value) const;

		// Gets value as wide string
		void GetValue(WString& value) const;

		// Gets value as float vector
		void GetValue(Vec2F& value) const;

		// Gets value as integer vector
		void GetValue(Vec2I& value) const;

		// Gets value as float rectangle
		void GetValue(RectF& value) const;

		// Gets value as integer rectangle
		void GetValue(RectI& value) const;

		// Gets value as float border
		void GetValue(BorderF& value) const;

		// Gets value as integer border
		void GetValue(BorderI& value) const;

		// Gets value as color
		void GetValue(Color4& value) const;

		// Gets value as UID
		void GetValue(UID& value) const;

		// Gets value as char
		void GetValue(char& value) const;

		// Gets value as char
		void GetValue(unsigned char& value) const;

		// Gets value as char
		void GetValue(wchar_t& value) const;

		// Gets value as char
		void GetValue(short& value) const;

		// Gets value as char
		void GetValue(unsigned short& value) const;

		// Gets value as char
		void GetValue(long& value) const;

		// Gets value as char
		void GetValue(unsigned long& value) const;

		// Gets value as char
		void GetValue(long long int& value) const;

		// Gets value as pointer, only for objects, based on ISerializable
		template<typename _type, typename X = typename std::enable_if<std::is_base_of<IObject, _type>::value>::type>
		void GetValue(_type*& value) const;

		// Gets value as pointer, only for objects, based on ISerializable
		template<typename _type, typename X = typename std::enable_if<std::is_base_of<IObject, _type>::value>::type>
		void GetValueRaw(_type*& value) const;

		// Gets value as vector
		template<typename _type>
		void GetValue(Vector<_type>& value) const;

		// Gets value as vector
		template<typename _type>
		void GetValueRaw(Vector<_type>& value) const;

		// Gets value as dictionary
		template<typename _key, typename _value>
		void GetValue(Dictionary<_key, _value>& value) const;

		// Gets value as enum class or IObject
		template<typename _type, typename _conv = typename ConverterSelector<_type>::type, typename X = typename std::enable_if<IsSupportsConverter<_type>::value>::type>
		void GetValue(_type& value) const;

		// Gets objects with delta from source object
		void GetValueDelta(IObject& object, const IObject& source) const;

		// Gets object from data without custom converters
		void GetValueRaw(IObject& object) const;

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

		// Registers data converter
		static void RegDataConverter(IDataNodeTypeConverter* converter);

	protected:
		static Vector<IDataNodeTypeConverter*> mDataConverters; // Data converters

		WString      mName;       // Name of node
		WString      mData;       // Node data
		DataNode*    mParent;     // Node parent
		DataNodesVec mChildNodes; // Children nodes

	protected:
		// Registers basic engine converters
		static void RegBasicConverters();

		friend class Application;
	};

	// Type and type getting forward declaration
	class Type;

	template<typename _type, typename _getter>
	const Type& GetTypeOf();
}

#include "Utils/Reflection/Reflection.h"

namespace o2
{
	template<typename _type>
	DataNode& DataNode::operator=(const _type& value)
	{
		return SetValue(value);
	}

	template<typename _type>
	DataNode::operator _type() const
	{
		_type res;
		GetValue(res);
		return res;
	}

	template<typename _type, typename X>
	DataNode& DataNode::SetValue(const _type* value)
	{
		if (value)
		{
			for (auto conv : mDataConverters)
			{
				if (conv->IsConvertsType(&GetTypeOf<_type*>()))
				{
					conv->ToData(&value, *this);
					return *this;
				}
			}

			AddNode("Type")->SetValue(value->GetType().GetName());
			AddNode("Value")->SetValue(*value);
		}

		return *this;
	}

	template<typename _type, typename X>
	DataNode& DataNode::SetValueRaw(const _type* value)
	{
		if (value)
		{
			AddNode("Type")->SetValue(value->GetType().GetName());
			AddNode("Value")->SetValueRaw(*value);
		}

		return *this;
	}

	template<typename _key, typename _value>
	DataNode& DataNode::SetValue(const Dictionary<_key, _value>& value)
	{
		Clear();

		for (auto kv : value)
		{
			DataNode* child = AddNode("Element");
			child->AddNode("Key")->SetValue(kv.Key());
			child->AddNode("Value")->SetValue(kv.Value());
		}

		return *this;
	}

	template<typename _type>
	DataNode& DataNode::SetValue(const Vector<_type>& value)
	{
		Clear();

		for (auto v : value)
			AddNode("Element")->SetValue(v);

		return *this;
	}

	template<typename _type, typename X>
	DataNode& DataNode::SetValueRaw(const Vector<_type>& value)
	{
		Clear();

		for (auto v : value)
			AddNode("Element")->SetValueRaw(v);

		return *this;
	}

	template<typename _type, typename X>
	void DataNode::GetValue(_type*& value) const
	{
		for (auto conv : mDataConverters)
		{
			if (conv->IsConvertsType(&GetTypeOf<_type*>()))
			{
				conv->FromData(&value, *this);
				return;
			}
		}

		GetValueRaw<_type>(value);
	}

	template<typename _type, typename X>
	void DataNode::GetValueRaw(_type*& value) const
	{
		if (auto typeNode = GetNode("Type"))
		{
			String type = *typeNode;

			if (auto valueNode = GetNode("Value"))
			{
				value = static_cast<_type*>(Reflection::CreateTypeSample(type));
				if (value)
					valueNode->GetValueRaw(*value);

				return;
			}
		}

		value = nullptr;
	}

	template<typename _key, typename _value>
	void DataNode::GetValue(Dictionary<_key, _value>& value) const
	{
		value.Clear();

		int count = mChildNodes.Count();
		for (auto childNode : mChildNodes)
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

	template<typename _type>
	void DataNode::GetValue(Vector<_type>& value) const
	{
		value.Clear();

		for (auto childNode : mChildNodes)
		{
			_type v = _type();
			childNode->GetValue(v);
			value.Add(v);
		}
	}

	template<typename _type>
	void DataNode::GetValueRaw(Vector<_type>& value) const
	{
		value.Clear();

		for (auto childNode : mChildNodes)
		{
			_type v = _type();
			childNode->GetValueRaw(v);
			value.Add(v);
		}
	}

	template<typename _type, typename _conv, typename X>
	DataNode& DataNode::SetValue(const _type& value)
	{
		_conv::ToData(value, *this);
		return *this;
	}

	template<typename _type, typename _conv, typename X>
	void DataNode::GetValue(_type& value) const
	{
		_conv::FromData(value, *this);
	}

	template<typename _type>
	void DataNode::EnumDataConverter<_type>::FromData(_type& object, const DataNode& data)
	{
		object = Reflection::GetEnumValue<_type>(data);
	}

	template<typename _type>
	void DataNode::EnumDataConverter<_type>::ToData(const _type& object, DataNode& data)
	{
		data = Reflection::GetEnumName<_type>(object);
	}

	template<typename _type>
	void DataNode::ObjectDataConverter<_type>::FromData(_type& object, const DataNode& data)
	{
		for (auto conv : mDataConverters)
		{
			if (conv->IsConvertsType(&object.GetType()))
			{
				conv->FromData(&object, data);
				return;
			}
		}

		data.GetValueRaw(object);
	}

	template<typename _type>
	void DataNode::ObjectDataConverter<_type>::ToData(const _type& object, DataNode& data)
	{
		for (auto conv : mDataConverters)
		{
			if (conv->IsConvertsType(&object.GetType()))
			{
				conv->ToData(&object, data);
				return;
			}
		}

		data.SetValueRaw(object);
	}

	template<typename _type>
	void DataNode::PropertyDataConverter<_type>::FromData(_type& object, const DataNode& data)
	{
		object.Set((_type::valueType)data);
	}

	template<typename _type>
	void DataNode::PropertyDataConverter<_type>::ToData(const _type& object, DataNode& data)
	{
		data.SetValue(object.Get());
	}
}
