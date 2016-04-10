#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Containers/Vector.h"
#include "Utils/String.h"

namespace o2
{
	class Actor;
	class Component;
	class DataNode;
	class ISerializable;
	class Type;

	// -------------------------------
	// Custom data converter interface
	// -------------------------------
	class IDataNodeTypeConverter
	{
	public:
		// Converts data to DataNode
		virtual void ToData(void* object, DataNode& data) {}

		// Converts from DataNode to object
		virtual void FromData(void*& object, const DataNode& data) {}

		// Checks that type is supports by this converter
		virtual bool CheckType(const Type* type) const { return false; }
	};

	//====================
	// Tree-like data node
	// ===================
	class DataNode
	{
	public:
		enum class Format { Xml, JSON, Binary };

		typedef Vector<DataNode*> DataNodesVec;
		typedef DataNodesVec::Iterator Iterator;
		typedef DataNodesVec::ConstIterator ConstIterator;

	protected:
		// --------------------
		// Enums data converter
		// --------------------
		template<typename _type>
		class EnumDataConverter
		{
		public:
			static void ToData(_type& object, DataNode& data);
			static void FromData(_type& object, const DataNode& data);
		};

		// -----------------------------------
		// Custom IObject types data converter
		// -----------------------------------
		template<typename _type>
		class CustomDataConverter
		{
		public:
			static void ToData(_type& object, DataNode& data);
			static void FromData(_type& object, const DataNode& data);
		};

	public:
		// Default constructor
		DataNode();

		// Copy-constructor
		DataNode(const DataNode& other);

		// Constructor with name
		DataNode(const WString& name);

		// Constructor with name and string value
		DataNode(const WString& name, char* value);

		// Constructor with name and wide characters string value
		DataNode(const WString& name, wchar_t* value);

		// Constructor with name and boolean value
		DataNode(const WString& name, bool value);

		// Constructor with name and integer value
		DataNode(const WString& name, int value);

		// Constructor with name and float value
		DataNode(const WString& name, float value);

		// Constructor with name and unsigned integer value
		DataNode(const WString& name, UInt value);

		// Constructor with name and string value
		DataNode(const WString& name, const String& value);

		// Constructor with name and wide characters string value
		DataNode(const WString& name, const WString& value);

		// Constructor with name and float vector value
		DataNode(const WString& name, const Vec2F& value);

		// Constructor with name and integer vector value
		DataNode(const WString& name, const Vec2I& value);

		// Constructor with name and float rectangle value
		DataNode(const WString& name, const RectF& value);

		// Constructor with name and integer rectangle value
		DataNode(const WString& name, const RectI& value);

		// Constructor with name and color value
		DataNode(const WString& name, const Color4& value);

		// Constructor with name and serializable value
		DataNode(const WString& name, ISerializable& value);

		// Constructor with integer value
		DataNode(int value);

		// Constructor with name and pointer value
		template<typename _type>
		DataNode(const WString& name, _type* value);

		// Constructor with name and vector value
		template<typename _type>
		DataNode(const WString& name, const Vector<_type>& value);

		// Constructor with name and dictionary value
		template<typename _key, typename _value>
		DataNode(const WString& name, const Dictionary<_key, _value>& value);


		// Destructor
		virtual ~DataNode();


		// Assign operator to other node
		DataNode& operator=(const DataNode& other);

		// Assign operator to other node
		DataNode& operator=(ISerializable& other);

		// Assign operator string value
		DataNode& operator=(char* value);

		// Assign operator wide string value
		DataNode& operator=(wchar_t* value);

		// Assign operator to integer value
		DataNode& operator=(int value);

		// Assign operator to integer value
		DataNode& operator=(unsigned long value);

		// Assign operator to integer value
		DataNode& operator=(long long int value);

		// Assign operator to integer value
		DataNode& operator=(UInt64 value);

		// Assign operator to boolean value
		DataNode& operator=(bool value);

		// Assign operator to float value
		DataNode& operator=(float value);

		// Assign operator to float value
		DataNode& operator=(double value);

		// Assign operator to unsigned integer value
		DataNode& operator=(UInt value);

		// Assign operator to string value
		DataNode& operator=(const String& value);

		// Assign operator to wide string value
		DataNode& operator=(const WString& value);

		// Assign operator to float vector value
		DataNode& operator=(const Vec2F& value);

		// Assign operator to integer vector value
		DataNode& operator=(const Vec2I& value);

		// Assign operator to float rectangle value
		DataNode& operator=(const RectF& value);

		// Assign operator to integer rectangle value
		DataNode& operator=(const RectI& value);

		// Assign operator to color value
		DataNode& operator=(const Color4& value);

		// Assign operator to pointer value
		template<typename _type,
			typename X = std::enable_if<std::is_base_of<ISerializable, _type>::value>::type>
		DataNode& operator=(_type* value);

		// Assign operator to vector value
		template<typename _type>
		DataNode& operator=(const Vector<_type>& value);

		// Assign operator to dictionary value
		template<typename _key, typename _value>
		DataNode& operator=(Dictionary<_key, _value>& value);

		// Assign operator to enum class value
		template<typename _type,
			typename _conv = std::conditional<std::is_enum<_type>::value, EnumDataConverter<_type>, CustomDataConverter<_type>>::type,
			typename X = std::enable_if<std::is_enum<_type>::value || std::is_base_of<IObject, _type>::value>::type>
			DataNode& operator=(_type& value);


		// Cast operator to wide string
		operator wchar_t*() const;

		// Cast operator to boolean
		operator bool() const;

		// Cast operator to integer
		operator int() const;

		// Cast operator to float
		operator float() const;

		// Cast operator to unsigned integer
		operator UInt() const;

		// Cast operator to unsigned integer
		operator UInt64() const;

		// Cast operator to string
		operator String() const;

		// Cast operator to wide string
		operator WString() const;

		// Cast operator to float vector
		operator Vec2F() const;

		// Cast operator to integer vector
		operator Vec2I() const;

		// Cast operator to float rectangle
		operator RectF() const;

		// Cast operator to integer rectangle
		operator RectI() const;

		// Cast operator to color
		operator Color4() const;

		// Cast operator to char
		operator char() const;

		// Cast operator to char
		operator unsigned char() const;

		// Cast operator to char
		operator wchar_t() const;

		// Cast operator to char
		operator short() const;

		// Cast operator to char
		operator unsigned short() const;

		// Cast operator to char
		operator long() const;

		// Cast operator to char
		operator unsigned long() const;

		// Cast operator to char
		operator long long int() const;

		// Cast operator to pointer
		template<typename _type,
			typename X = std::enable_if<std::is_base_of<ISerializable, _type>::value>::type>
		operator _type*() const;

		// Cast operator to vector
		template<typename _type>
		operator Vector<_type>() const;

		// Cast operator to dictionary
		template<typename _key, typename _value>
		operator Dictionary<_key, _value>() const;

		// Cast operator to enum class
		template<typename _type,
			typename _conv = std::conditional<std::is_enum<_type>::value, EnumDataConverter<_type>, CustomDataConverter<_type>>::type,
			typename X = std::enable_if<std::is_enum<_type>::value || std::is_base_of<IObject, _type>::value>::type>
			operator _type() const;


		// [] assign operator. nodePath sample: "node/node/abc/cde"
		DataNode* operator[](const WString& nodePath);

		// [] assign operator. nodePath sample: "node/node/abc/cde"
		DataNode* operator[](const char* nodePath);

		// Equals operator
		bool operator==(const DataNode& other) const;

		// Not equals operator
		bool operator!=(const DataNode& other) const;

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

		// Is DataNode supporting type trait
		template<typename T>
		struct IsSupport:
			std::integral_constant<bool,
			(std::is_fundamental<T>::value ||
			 std::is_enum<T>::value ||
			 std::is_base_of<ISerializable, T>::value ||
			 std::is_base_of<ISerializable, typename std::remove_pointer<T>::type>::value ||
			 std::is_same<Color4, T>::value ||
			 std::is_same<RectI, T>::value ||
			 std::is_same<RectF, T>::value ||
			 std::is_same<Vec2I, T>::value ||
			 std::is_same<Vec2F, T>::value ||
			 std::is_same<String, T>::value ||
			 std::is_same<WString, T>::value ||
			 std::is_same<DataNode, T>::value) && !std::is_const<T>::value>
		{};

		template<typename T2>
		struct IsSupport<Vector<T2>>: IsSupport<T2> {};

		template<typename T2, typename T3>
		struct IsSupport<Dictionary<T2, T3>>: std::integral_constant<bool, IsSupport<T2>::value && IsSupport<T3>::value> {};

	protected:
		static Vector<IDataNodeTypeConverter*> mDataConverters; // Data converters

		WString      mName;       // Name of node
		WString      mData;       // Node data
		DataNode*    mParent;     // Node parent. Nullptr if no parent
		DataNodesVec mChildNodes; // Children nodes

	protected:
		// Registers basic engine converters
		static void RegBasicConverters(); 

		friend class Application;
	};

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

		// Serializes actor to data (as object or asset)
		static void Serialize(Actor* actor, DataNode& data);

		// Deserializes param from data
		template<typename T, typename X = std::conditional<DataNode::IsSupport<T>::value, Fundamental<T>, Dummy<T>>::type>
		static void Deserialize(T& param, DataNode& data);

		// Deserializes actor form data (as object or asset)
		static void Deserialize(Actor*& actor, DataNode& data);
	};

	// Type and type getting forward declaration
	class Type;

	template<typename _type>
	const Type& _TypeOf();

	template<typename _type>
	DataNode::DataNode(const WString& name, _type* value):
		mName(name), mParent(nullptr)
	{
		*AddNode("Type") = value->GetType().Name();
		*AddNode("Value") = *value;
	}

	template<typename _type, typename X>
	DataNode::operator _type*() const
	{
		for (auto conv : mDataConverters)
		{
			void* value = nullptr;
			if (conv->CheckType(&_TypeOf<_type>()))
			{
				conv->FromData(value, *this);
				return (_type*)value;
			}
		}

		if (auto typeNode = GetNode("Type"))
		{
			String type = *typeNode;

			if (auto valueNode = GetNode("Value"))
			{
				ISerializable* value = static_cast<ISerializable*>(Reflection::CreateTypeSample(type));
				if (value)
					*value = *valueNode;

				return (_type*)value;
			}
		}

		return nullptr;
	}

	template<typename _type, typename X>
	DataNode& DataNode::operator=(_type* value)
	{
		if (value)
		{
			for (auto conv : mDataConverters)
			{
				if (conv->CheckType(&_TypeOf<_type>()))
				{
					conv->ToData(value, *this);
					return *this;
				}
			}

			*AddNode("Type") = value->GetType().Name();
			*AddNode("Value") = *value;
		}

		return *this;
	}


	template<typename _key, typename _value>
	DataNode::DataNode(const WString& name, const Dictionary<_key, _value>& value):
		mName(name), mParent(nullptr)
	{
		for (auto kv : value)
		{
			DataNode* child = AddNode("Element");
			*child->AddNode("Key") = kv.Key();
			*child->AddNode("Value") = kv.Value();
		}
	}

	template<typename _type>
	DataNode::DataNode(const WString& name, const Vector<_type>& value)
	{
		for (auto v : value)
			*AddNode("Element") = v;
	}

	template<typename _key, typename _value>
	DataNode& DataNode::operator=(Dictionary<_key, _value>& value)
	{
		Clear();

		for (auto kv : value)
		{
			DataNode* child = AddNode("Element");
			*child->AddNode("Key") = kv.Key();
			*child->AddNode("Value") = kv.Value();
		}

		return *this;
	}

	template<typename _type>
	DataNode& DataNode::operator=(const Vector<_type>& value)
	{
		Clear();

		for (auto v : value)
			*AddNode("Element") = v;

		return *this;
	}

	template<typename _key, typename _value>
	DataNode::operator Dictionary<_key, _value>() const
	{
		Dictionary<_key, _value> res;

		int count = mChildNodes.Count();
		_value v = _value();
		_key k = _key();
		for (auto childNode : mChildNodes)
		{
			auto keyNode = childNode->GetNode("Key");
			auto valueNode = childNode->GetNode("Value");

			if (keyNode && valueNode)
			{
				k = *keyNode;
				v = *valueNode;
				res.Add(k, v);
			}
		}

		return res;
	}

	template<typename _type>
	DataNode::operator Vector<_type>() const
	{
		int count = mChildNodes.Count();

		Vector<_type> res(count + 2);
		_type v = _type();
		for (auto childNode : mChildNodes)
		{
			v = *childNode;
			res.Add(v);
		}

		return res;
	}

	template<typename _type, typename _conv, typename X>
	DataNode& DataNode::operator=(_type& value)
	{
		_conv::ToData(value, *this);
		return *this;
	}

	template<typename _type, typename _conv, typename X>
	DataNode::operator _type() const
	{
		_type value;
		_conv::FromData(value, *this);
		return value;
	}

	template<typename _type>
	void DataNode::EnumDataConverter<_type>::FromData(_type& object, const DataNode& data)
	{
		object = Reflection::GetEnumValue<_type>(data);
	}

	template<typename _type>
	void DataNode::EnumDataConverter<_type>::ToData(_type& object, DataNode& data)
	{
		data = Reflection::GetEnumName<_type>(object);
	}

	template<typename _type>
	void DataNode::CustomDataConverter<_type>::FromData(_type& object, const DataNode& data)
	{
		for (auto conv : mDataConverters)
		{
			if (conv->CheckType(&_TypeOf<_type>()))
			{
				conv->FromData(&object, data);
				return;
			}
		}

		object.Deserialize(data);
	}

	template<typename _type>
	void DataNode::CustomDataConverter<_type>::ToData(_type& object, DataNode& data)
	{
		for (auto conv : mDataConverters)
		{
			if (conv->CheckType(&_TypeOf<_type>()))
			{
				conv->ToData(&object, data);
				return;
			}
		}

		data = object.Serialize();
	}

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

	template<typename T, typename X /*= SerializerSelector<T>::type*/>
	void Serializer::Deserialize(T& param, DataNode& data)
	{
		X::Deserialize(param, data);
	}
}