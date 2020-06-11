#pragma once

#include "o2/Utils/Memory/Allocators/ChunkPoolAllocator.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/String.h"
#include "o2/Utils/Types/UID.h"

#include "rapidjson/stream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/encodings.h"

namespace o2
{
	class DataDocument;
	struct DataMember;

	template <bool _const>
	class BaseMemberIterator;

	typedef BaseMemberIterator<false> DataMemberIterator;
	typedef BaseMemberIterator<true> ConstDataMemberIterator;

	// ----------------------------------------------
	// DOM value. Contains value, or array, or object
	// ----------------------------------------------
	class DataValue
	{
	protected:
		// Specialize this template class for your custom serialization types
		template<typename _type, typename _enable = void>
		struct Converter
		{
			static constexpr bool isSupported = false;
			using __type = typename std::conditional<std::is_same<void, _type>::value, int, _type>::type;

			static void Write(const __type& value, DataValue& data) {}
			static void Read(__type& value, const DataValue& data) {}
		};

	public:
		// Is DataValue supporting type trait
		template<typename _type>
		struct IsSupports
		{
			static constexpr bool value = Converter<_type>::isSupported;
		};

	public:
		// Default constructor
		DataValue(DataDocument& document);

		// Constructor as template value
		template<typename _type>
		DataValue(const _type& value, DataDocument& document);

		// Constructor as string value
		DataValue(const char* string, int length, bool isCopy, DataDocument& document);

		// Copy-constructor with same document
		DataValue(const DataValue& other);

		// Copy-constructor with another document
		DataValue(const DataValue& other, DataDocument& document);

		// Move-constructor
		DataValue(DataValue&& other);

		// Destructor
		~DataValue();

		// Copy operator
		DataValue& operator=(const DataValue& other);

		// Move operator
		DataValue& operator=(DataValue&& other);

		// Equals operator
		bool operator==(const DataValue& other) const;

		// Not equals operator
		bool operator!=(const DataValue& other) const;

		// Returns data document reference
		DataDocument& GetDocument();

		// Returns data document reference
		const DataDocument& GetDocument() const;

	public:	// Value methods
		// Checks is value type
		bool IsValue() const;

		// Checks is value number type
		bool IsNumber() const;

		// Checks is value string value
		bool IsString() const;

		// Checks is value boolean
		bool IsBoolean() const;

		// Checks is value null
		bool IsNull() const;

		// Cast to type operator
		template<typename _type>
		operator _type() const;

		// Assign operator
		template<typename _type>
		DataValue& operator=(const _type& value);

		// Sets value. Using DataValueConverter specializations
		template<typename _type>
		DataValue& Set(const _type& value);

		// Gets value. Using DataValueConverter specializations
		template<typename _type>
		void Get(_type& value) const;

		// Sets value as null
		void SetNull();

		// Optimized set string. When isCopy is false, string pointer can be used as reference
		void SetString(const char* string, int length, bool isCopy);

		// Returns string pointer
		const char* GetString() const;

		// Returns string length
		int GetStringLength() const;

	public: // Array methods
		// Checks value is array
		bool IsArray() const;

		// Returns array elements count
		int GetElementsCount() const;

		// Array element access by index
		DataValue& operator[](const int idx);

		// Array methods
		// Returns element by index
		DataValue& GetElement(int idx);

		// Returns element by index
		const DataValue& GetElement(int idx) const;

		// Adds element to array
		DataValue& AddElement(DataValue& value);

		// Adds element to array
		DataValue& AddElement();

		// Adds element to array
		DataValue* RemoveElement(DataValue* it);

		// Begin array iterator
		DataValue* Begin();

		// Begin array const iterator
		const DataValue* Begin() const;

		// End array iterator
		DataValue* End();

		// End array const iterator
		const DataValue* End() const;

		// Begin array iterator for range based for
		DataValue* begin();

		// Begin array const iterator for range based for
		const DataValue* begin() const;

		// End array iterator for range based for
		DataValue* end();

		// End array const iterator for range based for
		const DataValue* end() const;

	public: // Object methods		
		// Checks value is object
		bool IsObject() const;

		// Member access by name. Valid only for object type
		DataValue& operator[](const DataValue& name);

		// Member access by name. Valid only for object type
		DataValue& operator[](const char* name);

		// Member access by name. Valid only for object type
		const DataValue& operator[](const DataValue& name) const;

		// Member access by name. Valid only for object type
		const DataValue& operator[](const char* name) const;

		// Returns members count
		int GetMembersCount() const;

		// Returns node by name. Creates new nodes when required
		DataValue& GetMember(const DataValue& name);

		// Returns node by name. Creates new nodes when required
		DataValue& GetMember(const char* name);

		// Returns node by name.
		const DataValue& GetMember(const DataValue& name) const;

		// Returns node by name.
		const DataValue& GetMember(const char* name) const;

		// Returns node by name.
		DataValue* FindMember(const DataValue& name);

		// Returns node by name.
		DataValue* FindMember(const char* name);

		// Returns node by name.
		const DataValue* FindMember(const DataValue& name) const;

		// Returns node by name.
		const DataValue* FindMember(const char* name) const;

		// Add new node with name
		DataValue& AddMember(DataValue& name);

		// Add new node with name
		DataValue& AddMember(const char* name);

		// Removes node by name
		void RemoveMember(const DataValue& name);

		// Removes node by name
		void RemoveMember(const char* name);

		// Removes member
		DataMemberIterator RemoveMember(DataMemberIterator it);

		// Sets objects with delta from source object
		DataValue& SetValueDelta(const IObject& object, const IObject& source);

		// Gets objects with delta from source object
		void GetValueDelta(IObject& object, const IObject& source) const;

		// Begin array iterator
		DataMemberIterator BeginMember();

		// Begin array const iterator
		ConstDataMemberIterator BeginMember() const;

		// End array iterator
		DataMemberIterator EndMember();

		// End array const iterator
		ConstDataMemberIterator EndMember() const;

	public:
		// Returns is node data empty and have no members or elements
		bool IsEmpty() const;

		// Removes all members or elements
		void Clear();

		// Writes data to writer
		template <typename _writer>
		void Write(_writer& writer) const;

	public:
		enum class Flags
		{
			Bool = 1 << 0,
			Int = 1 << 1,
			UInt = 1 << 2,
			Int64 = 1 << 3,
			UInt64 = 1 << 4,
			Double = 1 << 5,
			Value = 1 << 6,
			String = 1 << 7,
			Object = 1 << 8,
			Array = 1 << 9,
			Null = 1 << 10,

			BoolTrue = 1 << 11,
			BoolFalse = 1 << 12,

			ShortString = 1 << 13,
			StringRef = 1 << 14,
			StringCopy = 1 << 15
		};

	protected:
		static constexpr UInt DataPayloadSize = 16;
		static constexpr UInt DataSize = DataPayloadSize + sizeof(Flags);

		static constexpr UInt ObjectInitialCapacity = 7;
		static constexpr UInt ArrayInitialCapacity = 7;

		struct IntData
		{
			int intValue;
			UInt uintValue;
			char padding[DataPayloadSize - sizeof(int)];
		};

		struct Int64Data
		{
			Int64 intValue;
			UInt64 uintValue;
			char padding[DataPayloadSize - sizeof(Int64)];
		};

		struct DoubleData
		{
			double value;
			char padding[DataPayloadSize - sizeof(double)];
		};

		struct StringPtrData
		{
			char* stringPtr;
			size_t stringLength;
		};

		struct ShortStringData
		{
			static constexpr int maxLength = DataPayloadSize/sizeof(char) - 1;
			char stringValue[maxLength + 1];
		};

		struct ValueTypeData
		{
			char padding[DataPayloadSize];
			Flags flags;

			inline bool Is(Flags f) const
			{
				return static_cast<Flags>(
					static_cast<std::underlying_type<Flags>::type>(flags) &
					static_cast<std::underlying_type<Flags>::type>(f)) == f;
			}
		};

		struct ObjectData
		{
			DataMember* members;
			UInt count;
			UInt capacity;
		};

		struct ArrayData
		{
			DataValue* elements;
			UInt count;
			UInt capacity;
		};

		union ValueData
		{
			IntData         intData;
			Int64Data       int64Data;
			DoubleData      doubleData;
			StringPtrData   stringPtrData;
			ShortStringData shortStringData;
			ObjectData      objectData;
			ArrayData       arrayData;

			ValueTypeData flagsData;
		};

		ValueData     mData;
		DataDocument* mDocument;

	protected:
		// Hidden constructor without document
		DataValue();

		// Constructor temporary string reference
		explicit DataValue(const char* stringRef);

		// Transcode wide char to char
		static bool Transcode(rapidjson::GenericStringBuffer<rapidjson::UTF8<>>& target, const wchar_t* source);

		// Transcode char to wide char
		static bool Transcode(rapidjson::GenericStringBuffer<rapidjson::UTF16<>>& target, const char* source);

		friend class JsonDataDocumentParseHandler;
		friend class TType<DataValue>;
	};

	// ------------------------------------------
	// Data values document. Must be object value
	// ------------------------------------------
	class DataDocument: public DataValue
	{
	public:
		enum class Format { Xml, JSON, Binary };

	public:
		// Default constructor
		DataDocument();

		// Copy-constructor
		DataDocument(const DataDocument& other);

		// Move-constructor
		DataDocument(DataDocument&& other);

		// Destructor
		~DataDocument();

		// Copy-operator. 
		DataDocument& operator=(const DataDocument& other);

		// Move-operator
		DataDocument& operator=(DataDocument&& other);

		// Equals operator
		bool operator==(const DataDocument& other) const;

		// Not equals operator
		bool operator!=(const DataDocument& other) const;

		// Cast to type operator
		template<typename _type>
		operator _type() const;

		// Assign operator
		template<typename _type>
		DataDocument& operator=(const _type& value);

		// Loads data structure from file
		bool LoadFromFile(const String& fileName, Format format = Format::JSON);

		// Loads data structure from string
		bool LoadFromData(const String& data, Format format = Format::JSON);

		// Saves data to file with specified format
		bool SaveToFile(const String& fileName, Format format = Format::JSON) const;

		// Saves data to string
		String SaveAsString(Format format = Format::JSON) const;

	protected:
		ChunkPoolAllocator mAllocator;

		friend class DataValue;
		friend class JsonDataDocumentParseHandler;
	};

	// --------------------------------------------
	// Data object value member. Has name and value
	// --------------------------------------------
	struct DataMember
	{
		DataValue name;
		DataValue value;

		DataMember() = default;
		DataMember(DataValue& name, DataValue& value);

		DataMember& operator=(DataMember& other);

		bool operator==(const DataMember& other) const;
		bool operator!=(const DataMember& other) const;
	};

	// -------------------------
	// Base data member iterator
	// -------------------------
	template <bool _const>
	class BaseMemberIterator
	{
		typedef typename std::conditional<_const, const DataMember, DataMember>::type DataMemberType;

	public:
		typedef DataMemberType value_type;
		typedef DataMemberType* pointer;
		typedef DataMemberType& reference;
		typedef std::ptrdiff_t difference_type;
		typedef std::random_access_iterator_tag iterator_category;

	public:
		BaseMemberIterator() {}
		BaseMemberIterator(const BaseMemberIterator<true>& other): mPointer(other.mPointer) {}

		operator bool() const { return mPointer != nullptr; }

		BaseMemberIterator<_const>& operator=(const BaseMemberIterator<true>& other) { mPointer = other.mPointer; return *this; }

		int operator-(BaseMemberIterator<true> other) const { return mPointer - other.mPointer; }

		BaseMemberIterator<_const>& operator++() { ++mPointer; return *this; }
		BaseMemberIterator<_const>& operator--() { --mPointer; return *this; }
		BaseMemberIterator<_const>  operator++(int) { Iterator old(*this); ++mPointer; return old; }
		BaseMemberIterator<_const>  operator--(int) { Iterator old(*this); --mPointer; return old; }

		BaseMemberIterator<_const> operator+(int n) const { return Iterator(mPointer+n); }
		BaseMemberIterator<_const> operator-(int n) const { return Iterator(mPointer-n); }

		BaseMemberIterator<_const>& operator+=(int n) { mPointer += n; return *this; }
		BaseMemberIterator<_const>& operator-=(int n) { mPointer -= n; return *this; }

		template <bool _const_> bool operator==(const BaseMemberIterator<_const_>& other) const { return mPointer == other.mPointer; }
		template <bool _const_> bool operator!=(const BaseMemberIterator<_const_>& other) const { return mPointer != other.mPointer; }
		template <bool _const_> bool operator<=(const BaseMemberIterator<_const_>& other) const { return mPointer <= other.mPointer; }
		template <bool _const_> bool operator>=(const BaseMemberIterator<_const_>& other) const { return mPointer >= other.mPointer; }
		template <bool _const_> bool operator< (const BaseMemberIterator<_const_>& other) const { return mPointer < other.mPointer; }
		template <bool _const_> bool operator> (const BaseMemberIterator<_const_>& other) const { return mPointer > other.mPointer; }

		DataMemberType& operator*() const { return *mPointer; }
		DataMemberType* operator->() const { return mPointer; }
		DataMemberType& operator[](int n) const { return mPointer[n]; }

		bool IsValid() const { return mPointer != nullptr; }

	private:
		explicit BaseMemberIterator(DataMemberType* p): mPointer(p) {}

	private:
		DataMemberType* mPointer = nullptr;

		friend class DataValue;
	};

	DataValue::Flags operator&(const DataValue::Flags& a, const DataValue::Flags& b);
	DataValue::Flags operator|(const DataValue::Flags& a, const DataValue::Flags& b);
}

#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
	template<typename _type>
	DataValue::DataValue(const _type& value, DataDocument& document):
		mData(), mDocument(&document)
	{
		Set(value);
	}

	template<typename _type>
	void DataValue::Get(_type& value) const
	{
		Converter<_type>::Read(value, *this);
	}

	template<typename _type>
	DataValue& DataValue::Set(const _type& value)
	{
		Converter<_type>::Write(value, *this);
		return *this;
	}

	template<typename _type>
	DataValue::operator _type() const
	{
		_type result = _type();
		Get(result);
		return result;
	}

	template<typename _type>
	DataValue& DataValue::operator=(const _type& value)
	{
		Set(value);
		return *this;
	}

	template <typename _writer>
	void DataValue::Write(_writer& writer) const
	{
		if (IsObject())
		{
			writer.StartObject();

			for (auto it = BeginMember(); it != EndMember(); ++it)
			{
				writer.Key(it->name.GetString(), it->name.GetStringLength(), it->name.mData.flagsData.Is(Flags::StringCopy));
				it->value.Write(writer);
			}

			writer.EndObject(mData.objectData.count);
		}
		else if (IsArray())
		{
			writer.StartArray();

			for (auto it = Begin(); it != End(); ++it)
				it->Write(writer);

			writer.EndArray(mData.arrayData.count);
		}
		else if (IsString())
		{
			writer.String(GetString(), GetStringLength(), mData.flagsData.Is(Flags::StringCopy));
		}
		else if (mData.flagsData.Is(Flags::Int))
		{
			writer.Int(mData.intData.intValue);
		}
		else if (mData.flagsData.Is(Flags::UInt))
		{
			writer.Uint(mData.intData.uintValue);
		}
		else if (mData.flagsData.Is(Flags::Int64))
		{
			writer.Int64(mData.int64Data.intValue);
		}
		else if (mData.flagsData.Is(Flags::UInt64))
		{
			writer.Uint64(mData.int64Data.uintValue);
		}
		else if (mData.flagsData.Is(Flags::Double))
		{
			writer.Double(mData.doubleData.value);
		}
		else if (mData.flagsData.Is(Flags::BoolTrue))
		{
			writer.Bool(true);
		}
		else if (mData.flagsData.Is(Flags::BoolFalse))
		{
			writer.Bool(false);
		}
		else
		{
			writer.Null();
		}
	}

	template<typename _type>
	DataDocument& DataDocument::operator=(const _type& value)
	{
		DataValue::operator=(value);
		return *this;
	}

	template<typename _type>
	DataDocument::operator _type() const
	{
		_type result = _type();
		Get(result);
		return result;
	}

	template<>
	struct DataValue::Converter<char*>
	{
		static constexpr bool isSupported = true;

		using charPtr = char*;

		static void Write(const charPtr& value, DataValue& data)
		{
			data.SetString(value, strlen(value), true);
		}

		static void Read(charPtr& value, const DataValue& data)
		{
			Assert(data.mData.flagsData.Is(DataValue::Flags::String), "Trying get string from not string value");

			if (data.mData.flagsData.Is(DataValue::Flags::ShortString))
			{
				strcpy(value, data.mData.shortStringData.stringValue);
			}
			else
			{
				auto length = data.mData.stringPtrData.stringLength;
				memcpy(value, data.mData.stringPtrData.stringPtr, sizeof(char)*length);
				value[length] = '\0';
			}
		}
	};

	template<UInt _size>
	struct DataValue::Converter<char[_size]>
	{
		static constexpr bool isSupported = true;

		using charPtr = char[_size];

		static void Write(const charPtr& value, DataValue& data)
		{
			data.SetString(value, _size, true);
		}

		static void Read(charPtr& value, const DataValue& data)
		{}
	};

	template<>
	struct DataValue::Converter<const char*>
	{
		static constexpr bool isSupported = true;

		using charPtr = const char*;

		static void Write(const charPtr& value, DataValue& data)
		{
			data.SetString(value, strlen(value), true);
		}

		static void Read(charPtr& value, const DataValue& data)
		{}
	};

	template<>
	struct DataValue::Converter<wchar_t*>
	{
		static constexpr bool isSupported = true;

		using wcharPtr = wchar_t*;

		static void Write(const wcharPtr& value, DataValue& data)
		{
			rapidjson::GenericStringBuffer<rapidjson::UTF8<>> target;
			if (Transcode(target, value))
				data.Set(target.GetString());
		}

		static void Read(wcharPtr value, const DataValue& data)
		{
			rapidjson::GenericStringBuffer<rapidjson::UTF16<>> target;
			if (Transcode(target, data.GetString()))
				wcscpy(value, target.GetString());
		}
	};

	template<>
	struct DataValue::Converter<const wchar_t*>
	{
		static constexpr bool isSupported = true;

		using wcharPtr = const wchar_t*;

		static void Write(const wcharPtr& value, DataValue& data)
		{
			rapidjson::GenericStringBuffer<rapidjson::UTF8<>> target;
			if (Transcode(target, value))
				data.Set(target.GetString());
		}

		static void Read(wcharPtr value, const DataValue& data)
		{}
	};

	template<>
	struct DataValue::Converter<bool>
	{
		static constexpr bool isSupported = true;

		static void Write(const bool& value, DataValue& data)
		{
			if (value)
				data.mData.flagsData.flags = DataValue::Flags::Bool | DataValue::Flags::BoolTrue;
			else
				data.mData.flagsData.flags = DataValue::Flags::Bool | DataValue::Flags::BoolFalse;
		}

		static void Read(bool& value, const DataValue& data)
		{
			value = data.mData.flagsData.Is(DataValue::Flags::Bool) && data.mData.flagsData.Is(DataValue::Flags::BoolTrue);
		}
	};

	template<>
	struct DataValue::Converter<int>
	{
		static constexpr bool isSupported = true;

		static void Write(const int& value, DataValue& data)
		{
			data.mData.flagsData.flags = Flags::Int | Flags::Value;
			data.mData.intData.intValue = value;
		}

		static void Read(int& value, const DataValue& data)
		{
			if (data.mData.flagsData.Is(Flags::Int))
				value = data.mData.intData.intValue;
			else if (data.mData.flagsData.Is(Flags::UInt))
				value = (int)data.mData.intData.uintValue;
			else if (data.mData.flagsData.Is(Flags::Int64))
				value = (int)data.mData.int64Data.intValue;
			else if (data.mData.flagsData.Is(Flags::UInt64))
				value = (int)data.mData.int64Data.uintValue;
			else if (data.mData.flagsData.Is(Flags::Double))
				value = (int)data.mData.doubleData.value;
			else
				Assert(false, "Trying to get int from not number value");
		}
	};

	template<>
	struct DataValue::Converter<UInt>
	{
		static constexpr bool isSupported = true;

		static void Write(const UInt& value, DataValue& data)
		{
			data.mData.flagsData.flags = Flags::UInt | Flags::Value;
			data.mData.intData.uintValue = value;
		}

		static void Read(UInt& value, const DataValue& data)
		{
			if (data.mData.flagsData.Is(Flags::UInt))
				value = data.mData.intData.uintValue;
			else if (data.mData.flagsData.Is(Flags::Int))
				value = (UInt)data.mData.intData.intValue;
			else if (data.mData.flagsData.Is(Flags::Int64))
				value = (UInt)data.mData.int64Data.intValue;
			else if (data.mData.flagsData.Is(Flags::UInt64))
				value = (UInt)data.mData.int64Data.uintValue;
			else if (data.mData.flagsData.Is(Flags::Double))
				value = (UInt)data.mData.doubleData.value;
			else
				Assert(false, "Trying to get unsigned int from not number value");
		}
	};

	template<>
	struct DataValue::Converter<Int64>
	{
		static constexpr bool isSupported = true;

		static void Write(const Int64& value, DataValue& data)
		{
			data.mData.flagsData.flags = Flags::Int64 | Flags::Value;
			data.mData.int64Data.intValue = value;
		}

		static void Read(Int64& value, const DataValue& data)
		{
			if (data.mData.flagsData.Is(Flags::Int64))
				value = data.mData.int64Data.intValue;
			else if (data.mData.flagsData.Is(Flags::UInt64))
				value = (Int64)data.mData.int64Data.uintValue;
			else if (data.mData.flagsData.Is(Flags::Int))
				value = (Int64)data.mData.intData.intValue;
			else if (data.mData.flagsData.Is(Flags::UInt))
				value = (Int64)data.mData.intData.uintValue;
			else if (data.mData.flagsData.Is(Flags::Double))
				value = (Int64)data.mData.doubleData.value;
			else
				Assert(false, "Trying to get int64 from not number value");
		}
	};

	template<>
	struct DataValue::Converter<UInt64>
	{
		static constexpr bool isSupported = true;

		static void Write(const UInt64& value, DataValue& data)
		{
			data.mData.flagsData.flags = Flags::UInt64 | Flags::Value;
			data.mData.int64Data.uintValue = value;
		}

		static void Read(UInt64& value, const DataValue& data)
		{
			if (data.mData.flagsData.Is(Flags::UInt64))
				value = data.mData.int64Data.uintValue;
			else if (data.mData.flagsData.Is(Flags::Int64))
				value = (UInt64)data.mData.int64Data.intValue;
			else if (data.mData.flagsData.Is(Flags::Int))
				value = (UInt64)data.mData.intData.intValue;
			else if (data.mData.flagsData.Is(Flags::UInt))
				value = (UInt64)data.mData.intData.uintValue;
			else if (data.mData.flagsData.Is(Flags::Double))
				value = (UInt64)data.mData.doubleData.value;
			else
				Assert(false, "Trying to get unsigned int64 from not number value");
		}
	};

	template<>
	struct DataValue::Converter<double>
	{
		static constexpr bool isSupported = true;

		static void Write(const double& value, DataValue& data)
		{
			data.mData.flagsData.flags = Flags::Double | Flags::Value;
			data.mData.doubleData.value = value;
		}

		static void Read(double& value, const DataValue& data)
		{
			if (data.mData.flagsData.Is(Flags::Double))
				value = data.mData.doubleData.value;
			else if (data.mData.flagsData.Is(Flags::Int))
				value = (double)data.mData.intData.intValue;
			else if (data.mData.flagsData.Is(Flags::UInt))
				value = (double)data.mData.intData.uintValue;
			else if (data.mData.flagsData.Is(Flags::Int64))
				value = (double)data.mData.int64Data.intValue;
			else if (data.mData.flagsData.Is(Flags::UInt64))
				value = (double)data.mData.int64Data.uintValue;
			else
				Assert(false, "Trying to get double from not number value");
		}
	};

	template<>
	struct DataValue::Converter<float>
	{
		static constexpr bool isSupported = true;

		static void Write(const float& value, DataValue& data)
		{
			data.mData.flagsData.flags = Flags::Double | Flags::Value;
			data.mData.doubleData.value = (double)value;
		}

		static void Read(float& value, const DataValue& data)
		{
			if (data.mData.flagsData.Is(Flags::Double))
				value = (float)data.mData.doubleData.value;
			else if (data.mData.flagsData.Is(Flags::Int))
				value = (float)data.mData.intData.intValue;
			else if (data.mData.flagsData.Is(Flags::UInt))
				value = (float)data.mData.intData.uintValue;
			else if (data.mData.flagsData.Is(Flags::Int64))
				value = (float)data.mData.int64Data.intValue;
			else if (data.mData.flagsData.Is(Flags::UInt64))
				value = (float)data.mData.int64Data.uintValue;
			else
				Assert(false, "Trying to get float from not number value");
		}
	};

	template<>
	struct DataValue::Converter<String>
	{
		static constexpr bool isSupported = true;

		static void Write(const String& value, DataValue& data)
		{
			data.SetString(value.Data(), value.Length(), true);
		}

		static void Read(String& value, const DataValue& data)
		{
			Assert(data.mData.flagsData.Is(DataValue::Flags::String), "Trying get string from not string value");

			if (data.mData.flagsData.Is(DataValue::Flags::ShortString))
				value = data.mData.shortStringData.stringValue;
			else
				value = data.mData.stringPtrData.stringPtr;
		}
	};

	template<>
	struct DataValue::Converter<WString>
	{
		static constexpr bool isSupported = true;

		static void Write(const WString& value, DataValue& data)
		{
			data.Set(value.Data());
		}

		static void Read(WString& value, const DataValue& data)
		{
			rapidjson::GenericStringBuffer<rapidjson::UTF16<>> target;
			if (Transcode(target, data.GetString()))
				value = target.GetString();
		}
	};

	template<>
	struct DataValue::Converter<UID>
	{
		static constexpr bool isSupported = true;

		static void Write(const UID& value, DataValue& data)
		{
			data.Set((WString)value);
		}

		static void Read(UID& value, const DataValue& data)
		{
			WString buf;
			data.Get(buf);
			value = buf;
		}
	};

	template<>
	struct DataValue::Converter<Vec2F>
	{
		static constexpr bool isSupported = true;

		static void Write(const Vec2F& value, DataValue& data)
		{
			data.AddMember("x") = value.x;
			data.AddMember("y") = value.y;
		}

		static void Read(Vec2F& value, const DataValue& data)
		{
			value.x = data.GetMember("x");
			value.y = data.GetMember("y");
		}
	};

	template<>
	struct DataValue::Converter<Vec2I>
	{
		static constexpr bool isSupported = true;

		static void Write(const Vec2I& value, DataValue& data)
		{
			data.AddMember("x") = value.x;
			data.AddMember("y") = value.y;
		}

		static void Read(Vec2I& value, const DataValue& data)
		{
			value.x = data.GetMember("x");
			value.y = data.GetMember("y");
		}
	};

	template<>
	struct DataValue::Converter<RectF>
	{
		static constexpr bool isSupported = true;

		static void Write(const RectF& value, DataValue& data)
		{
			data.AddMember("left") = value.left;
			data.AddMember("bottom") = value.bottom;
			data.AddMember("right") = value.right;
			data.AddMember("top") = value.top;
		}

		static void Read(RectF& value, const DataValue& data)
		{
			value.left = data.GetMember("left");
			value.bottom = data.GetMember("bottom");
			value.right = data.GetMember("right");
			value.top = data.GetMember("top");
		}
	};

	template<>
	struct DataValue::Converter<RectI>
	{
		static constexpr bool isSupported = true;

		static void Write(const RectI& value, DataValue& data)
		{
			data.AddMember("left") = value.left;
			data.AddMember("bottom") = value.bottom;
			data.AddMember("right") = value.right;
			data.AddMember("top") = value.top;
		}

		static void Read(RectI& value, const DataValue& data)
		{
			value.left = data.GetMember("left");
			value.bottom = data.GetMember("bottom");
			value.right = data.GetMember("right");
			value.top = data.GetMember("top");
		}
	};

	template<>
	struct DataValue::Converter<BorderF>
	{
		static constexpr bool isSupported = true;

		static void Write(const BorderF& value, DataValue& data)
		{
			data.AddMember("left") = value.left;
			data.AddMember("bottom") = value.bottom;
			data.AddMember("right") = value.right;
			data.AddMember("top") = value.top;
		}

		static void Read(BorderF& value, const DataValue& data)
		{
			value.left = data.GetMember("left");
			value.bottom = data.GetMember("bottom");
			value.right = data.GetMember("right");
			value.top = data.GetMember("top");
		}
	};

	template<>
	struct DataValue::Converter<BorderI>
	{
		static constexpr bool isSupported = true;

		static void Write(const BorderI& value, DataValue& data)
		{
			data.AddMember("left") = value.left;
			data.AddMember("bottom") = value.bottom;
			data.AddMember("right") = value.right;
			data.AddMember("top") = value.top;
		}

		static void Read(BorderI& value, const DataValue& data)
		{
			value.left = data.GetMember("left");
			value.bottom = data.GetMember("bottom");
			value.right = data.GetMember("right");
			value.top = data.GetMember("top");
		}
	};

	template<>
	struct DataValue::Converter<Color4>
	{
		static constexpr bool isSupported = true;

		static void Write(const Color4& value, DataValue& data)
		{
			data.AddMember("r") = value.r;
			data.AddMember("g") = value.g;
			data.AddMember("b") = value.b;
			data.AddMember("a") = value.a;
		}

		static void Read(Color4& value, const DataValue& data)
		{
			value.r = data.GetMember("r");
			value.g = data.GetMember("g");
			value.b = data.GetMember("b");
			value.a = data.GetMember("a");
		}
	};

	template<typename T>
	struct DataValue::Converter<T, typename std::enable_if<std::is_pointer<T>::value && !std::is_const<T>::value &&
		std::is_base_of<o2::IObject, typename std::remove_pointer<T>::type>::value>::type>
	{
		static constexpr bool isSupported = true;

		static void Write(const T& value, DataValue& data)
		{
			if (value)
			{
				data.AddMember("Type").Set(value->GetType().GetName());
				data.AddMember("Value").Set(*value);
			}
		}

		static void Read(T& value, const DataValue& data)
		{
			if (auto typeNode = data.FindMember("Type"))
			{
				String typeName = *typeNode;
				if (typeName == "Editor::AnimationPropertiesTreeNode")
					typeName = typeName;

				if (auto valueNode = data.FindMember("Value"))
				{
					if (value)
						delete value;

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
						valueNode->Get(*value);
				}
			}
		}
	};

	template<typename T>
	struct DataValue::Converter<T, typename std::enable_if<std::is_pointer<T>::value && !std::is_const<T>::value &&
		!std::is_base_of<o2::IObject, typename std::remove_pointer<T>::type>::value && !std::is_same<void*, T>::value>::type>
	{
		static constexpr bool isSupported = DataValue::Converter<std::remove_pointer<T>::type>::isSupported;

		static void Write(const T& value, DataValue& data)
		{
			DataValue::Converter<std::remove_pointer<T>::type>::Write(*value, data);
		}

		static void Read(T& value, const DataValue& data)
		{
			DataValue::Converter<std::remove_pointer<T>::type>::Read(*value, data);
		}
	};

	template<typename T>
	struct DataValue::Converter<Vector<T>>
	{
		static constexpr bool isSupported = true;

		static void Write(const Vector<T>& value, DataValue& data)
		{
			data.mData.flagsData.flags = Flags::Array;
			data.mData.arrayData.elements = nullptr;
			data.mData.arrayData.count = 0;
			data.mData.arrayData.capacity = 0;

			for (auto& v : value)
				data.AddElement(DataValue(v, *data.mDocument));
		}

		static void Read(Vector<T>& value, const DataValue& data)
		{
			if (data.IsArray())
			{
				value.Clear();
				for (auto& element : data)
				{
					T v = T();
					element.Get(v);
					value.Add(v);
				}
			}
		}
	};

	template<typename _key, typename _value>
	struct DataValue::Converter<Map<_key, _value>>
	{
		static constexpr bool isSupported = true;

		static void Write(const Map<_key, _value>& value, DataValue& data)
		{
			data.mData.flagsData.flags = Flags::Array;
			data.mData.arrayData.elements = nullptr;
			data.mData.arrayData.count = 0;
			data.mData.arrayData.capacity = 0;

			for (auto& kv : value)
			{
				DataValue& child = data.AddElement();
				child.AddMember("Key").Set(kv.first);
				child.AddMember("Value").Set(kv.second);
			}
		}

		static void Read(Map<_key, _value>& value, const DataValue& data)
		{
			if (data.IsArray())
			{
				value.Clear();
				for (auto& childNode : data)
				{
					auto keyNode = childNode.FindMember("Key");
					auto valueNode = childNode.FindMember("Value");

					if (keyNode && valueNode)
					{
						_value v = _value();
						_key k = _key();
						keyNode->Get(k);
						valueNode->Get(v);
						value.Add(k, v);
					}
				}
			}
		}
	};

	template<typename T>
	struct DataValue::Converter<T, typename std::enable_if<std::is_enum<T>::value>::type>
	{
		static constexpr bool isSupported = true;

		static void Write(const T& value, DataValue& data)
		{
			data.Set(Reflection::GetEnumName<T>(value));
		}

		static void Read(T& value, const DataValue& data)
		{
			String buf;
			data.Get(buf);
			value = Reflection::GetEnumValue<T>(buf);
		}
	};

	template<typename T>
	struct DataValue::Converter<T, typename std::enable_if<IsProperty<T>::value>::type>
	{
		static constexpr bool isSupported = DataValue::IsSupports<T::valueType>::value;
		using TValueType = typename T::valueType;

		static void Write(const T& value, DataValue& data)
		{
			data.Set(value.Get());
		}

		static void Read(T& value, const DataValue& data)
		{
			TValueType val;
			data.Get(val);
			value.Set(val);
		}
	};

	template<typename T>
	struct DataValue::Converter<T, typename std::enable_if<std::is_base_of<IObject, T>::value || std::is_same<IObject, T>::value>::type>
	{
		static constexpr bool isSupported = true;

		static void Write(const T& value, DataValue& data)
		{
			struct helper
			{
				static void WriteObject(void* object, const ObjectType& type, DataValue& node)
				{
					for (auto baseType : type.GetBaseTypes())
					{
						const ObjectType* baseObjectType = dynamic_cast<const ObjectType*>(baseType.type);
						if (!baseObjectType)
							continue;

						void* baseObject = (*baseType.dynamicCastUpFunc)(object);
						WriteObject(baseObject, *baseObjectType, node);
					}

					for (auto& field : type.GetFields())
					{
						auto srlzAttribute = field.GetAttribute<SerializableAttribute>();
						if (srlzAttribute)
							field.SerializeFromObject(object, node.AddMember(field.GetName()));
					}
				}
			};

			if (value.GetType().IsBasedOn(TypeOf(ISerializable)))
				dynamic_cast<const ISerializable&>(value).OnSerialize(data);

			const ObjectType& type = dynamic_cast<const ObjectType&>(value.GetType());
			void* objectPtr = type.DynamicCastFromIObject(const_cast<IObject*>(dynamic_cast<const IObject*>(&value)));

			helper::WriteObject(objectPtr, type, data);
		}

		static void Read(T& value, const DataValue& data)
		{
			struct helper
			{
				static void ReadObject(void* object, const ObjectType& type, const DataValue& node)
				{
					for (auto baseType : type.GetBaseTypes())
					{
						const ObjectType* baseObjectType = dynamic_cast<const ObjectType*>(baseType.type);
						if (!baseObjectType)
							continue;

						void* baseObject = (*baseType.dynamicCastUpFunc)(object);
						ReadObject(baseObject, *baseObjectType, node);
					}

					for (auto& field : type.GetFields())
					{
						auto srlzAttribute = field.GetAttribute<SerializableAttribute>();
						if (srlzAttribute)
						{
							auto fldNode = node.FindMember(field.GetName());
							if (fldNode)
								field.DeserializeFromObject(object, *fldNode);
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

PRE_ENUM_META(o2::DataValue::Flags);

PRE_ENUM_META(o2::DataDocument::Format);
