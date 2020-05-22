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
	class DataMember;

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
		enum class Format { Xml, JSON, Binary };

		// Is DataNode supporting type trait
		template<typename _type>
		struct IsSupports
		{
			static constexpr bool value = Converter<_type>::isSupported;
		};

	public:
		// Default constructor
		DataValue(DataDocument& document);

		template<typename _type>
		explicit DataValue(const _type& value, DataDocument& document);

		// Copy-constructor
		DataValue(const DataValue& other);

		// Destructor
		~DataValue();

		// Assign operator
		DataValue& operator=(const DataValue& value);

		// Equals operator
		bool operator==(const DataValue& other) const;

		// Not equals operator
		bool operator!=(const DataValue& other) const;

	public:	// Value methods
		// Checks value type
		template<typename _type>
		bool IsValue() const;

		// Cast to type operator
		template<typename _type>
		operator _type() const;

		// Assign operator
		template<typename _type>
		DataValue& operator=(const _type& value);

		// Sets value. Using DataNodeConverter specializations
		template<typename _type>
		DataValue& SetValue(const _type& value);

		// Gets value. Using DataNodeConverter specializations
		template<typename _type>
		void GetValue(_type& value) const;

		// Optimized set string
		void SetString(const wchar_t* string, int length);

	public: // Array methods
		// Checks value is array
		bool IsArray() const;

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
		DataValue& operator[](const WString& name);

		// Member access by name. Valid only for object type
		DataValue& operator[](const char* name);

		// Returns node by path: "node/node/abc/cde". Creates new nodes when required
		DataValue& GetMember(const WString& path);

		// Returns node by path: "node/node/abc/cde"
		const DataValue& GetMember(const WString& path) const;

		// Returns node by path: "node/node/abc/cde"
		DataValue* FindMember(const WString& path);

		// Returns node by path: "node/node/abc/cde"
		const DataValue* FindMember(const WString& path) const;

		// Add new node with name
		DataValue& AddMember(const WString& name);

		// Add new node with name
		DataValue& AddMember(const char* name);

		// Removes node by name
		void RemoveMember(const WString& name);

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
		// Returns is node data empty and have no children
		bool IsEmpty() const;

		// Removes all children
		void Clear();

		// Loads data structure from file
		bool LoadFromFile(const String& fileName);

		// Loads data structure from string
		bool LoadFromData(const WString& data);

		// Saves data to file with specified format
		bool SaveToFile(const String& fileName, Format format = Format::Xml) const;

		// Saves data to string
		WString SaveAsWString(Format format = Format::Xml) const;

	public:
		enum class Flags: unsigned
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

		static constexpr int DataPayloadSize = 16;
		static constexpr int DataSize = DataPayloadSize + sizeof(Flags);

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
			wchar_t* stringPtr;
			size_t stringLength;
		};

		struct ShortStringData
		{
			wchar_t stringValue[DataPayloadSize/sizeof(wchar_t) - 1];
			wchar_t stringLength;
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

		ValueData     mValue;
		DataDocument& mDocument;
	};

	class DataDocument: public DataValue
	{
	protected:
		ChunkPoolAllocator mAllocator;

		friend class DataValue;
	};

	// --------------------------------------------
	// Data object value member. Has name and value
	// --------------------------------------------
	class DataMember
	{
		DataValue name;
		DataValue value;
	};

	// -------------------------
	// Base data member iterator
	// -------------------------
	template <bool _const>
	class BaseMemberIterator
	{
		typedef std::conditional<_const, const DataMember, DataMember> DataMemberType;

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
	};

	DataValue::Flags operator&(const DataValue::Flags& a, const DataValue::Flags& b);
	DataValue::Flags operator|(const DataValue::Flags& a, const DataValue::Flags& b);
}

#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
	template<typename _type>
	void DataValue::GetValue(_type& value) const
	{
		Converter<_type>::Read(value, *this);
	}

	template<typename _type>
	DataValue& DataValue::SetValue(const _type& value)
	{
		Converter<_type>::Write(value, *this);
		return *this;
	}

	template<typename _type>
	DataValue::DataValue(const _type& value, DataDocument& document):
		mDocument(document)
	{
		SetValue(value);
	}

	template<typename _type>
	bool DataValue::IsValue() const
	{
		return mValue.flagsData.flags & Flags::Value;
	}

	template<typename _type>
	DataValue::operator _type() const
	{
		_type result;
		GetValue(result);
		return result;
	}

	template<typename _type>
	DataValue& DataValue::operator=(const _type& value)
	{
		SetValue(value);
		return *this;
	}

	template<>
	struct DataValue::Converter<char*>
	{
		static constexpr bool isSupported = true;

		using charPtr = char*;

		static void Write(const charPtr& value, DataValue& data)
		{
			using namespace rapidjson;

			StringStream source(value);
			GenericStringBuffer<UTF16<>> target;

			bool hasError = false;
			while (source.Peek() != '\0')
			{
				if (!Transcoder<UTF8<>, UTF16<>>::Transcode(source, target))
				{
					hasError = true;
					break;
				}
			}

			if (!hasError)
				data.SetValue(target.GetString());
		}

		static void Read(charPtr& value, const DataValue& data)
		{
			using namespace rapidjson;

			GenericStringStream<UTF16<>> source((wchar_t*)data);
			GenericStringBuffer<UTF8<>> target;

			bool hasError = false;
			while (source.Peek() != '\0')
			{
				if (!Transcoder<UTF16<>, UTF8<>>::Transcode(source, target))
				{
					hasError = true;
					break;
				}
			}

			if (!hasError)
				strcpy(value, target.GetString());
		}
	};

	template<>
	struct DataValue::Converter<wchar_t*>
	{
		static constexpr bool isSupported = true;

		using wcharPtr = wchar_t*;

		static void Write(const wcharPtr& value, DataValue& data)
		{
			data.SetString(value, wcslen(value));
		}

		static void Read(wcharPtr value, const DataValue& data)
		{
			Assert(data.mValue.flagsData.Is(DataValue::Flags::String), "Trying get string from not string value");

			if (data.mValue.flagsData.Is(DataValue::Flags::ShortString))
			{
				auto length = data.mValue.shortStringData.stringLength;
				memcpy(value, data.mValue.shortStringData.stringValue, sizeof(wchar_t)*length);
				value[length] = '\0';
			}
			else 
			{
				auto length = data.mValue.stringPtrData.stringLength;
				memcpy(value, data.mValue.stringPtrData.stringPtr, sizeof(wchar_t)*length);
				value[length] = '\0';
			}
		}
	};

	template<>
	struct DataValue::Converter<bool>
	{
		static constexpr bool isSupported = true;

		static void Write(const bool& value, DataValue& data)
		{
			if (value)
				data.mValue.flagsData.flags = DataValue::Flags::Bool | DataValue::Flags::BoolTrue;
			else
				data.mValue.flagsData.flags = DataValue::Flags::Bool | DataValue::Flags::BoolFalse;
		}

		static void Read(bool& value, const DataValue& data)
		{
			value = data.mValue.flagsData.Is(DataValue::Flags::Bool) && data.mValue.flagsData.Is(DataValue::Flags::BoolTrue);
		}
	};

	template<>
	struct DataValue::Converter<int>
	{
		static constexpr bool isSupported = true;

		static void Write(const int& value, DataValue& data)
		{
			data.mValue.flagsData.flags = Flags::Int & Flags::Value;
			data.mValue.intData.intValue = value;
		}

		static void Read(int& value, const DataValue& data)
		{
			if (data.mValue.flagsData.Is(Flags::Int))
				value = data.mValue.intData.intValue;
			else if (data.mValue.flagsData.Is(Flags::UInt))
				value = (int)data.mValue.intData.uintValue;
			else if (data.mValue.flagsData.Is(Flags::Int64))
				value = (int)data.mValue.int64Data.intValue;
			else if (data.mValue.flagsData.Is(Flags::UInt64))
				value = (int)data.mValue.int64Data.uintValue;
			else if (data.mValue.flagsData.Is(Flags::Double))
				value = (int)data.mValue.doubleData.value;
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
			data.mValue.flagsData.flags = Flags::UInt & Flags::Value;
			data.mValue.intData.uintValue = value;
		}

		static void Read(UInt& value, const DataValue& data)
		{
			if (data.mValue.flagsData.Is(Flags::UInt))
				value = data.mValue.intData.uintValue;
			else if (data.mValue.flagsData.Is(Flags::Int))
				value = (UInt)data.mValue.intData.intValue;
			else if (data.mValue.flagsData.Is(Flags::Int64))
				value = (UInt)data.mValue.int64Data.intValue;
			else if (data.mValue.flagsData.Is(Flags::UInt64))
				value = (UInt)data.mValue.int64Data.uintValue;
			else if (data.mValue.flagsData.Is(Flags::Double))
				value = (UInt)data.mValue.doubleData.value;
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
			data.mValue.flagsData.flags = Flags::Int64 & Flags::Value;
			data.mValue.int64Data.intValue = value;
		}

		static void Read(Int64& value, const DataValue& data)
		{
			if (data.mValue.flagsData.Is(Flags::Int64))
				value = data.mValue.int64Data.intValue;
			else if (data.mValue.flagsData.Is(Flags::UInt64))
				value = (Int64)data.mValue.int64Data.uintValue;
			else if (data.mValue.flagsData.Is(Flags::Int))
				value = (Int64)data.mValue.intData.intValue;
			else if (data.mValue.flagsData.Is(Flags::UInt))
				value = (Int64)data.mValue.intData.uintValue;
			else if (data.mValue.flagsData.Is(Flags::Double))
				value = (Int64)data.mValue.doubleData.value;
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
			data.mValue.flagsData.flags = Flags::UInt64 & Flags::Value;
			data.mValue.int64Data.uintValue = value;
		}

		static void Read(UInt64& value, const DataValue& data)
		{
			if (data.mValue.flagsData.Is(Flags::UInt64))
				value = data.mValue.int64Data.uintValue;
			else if (data.mValue.flagsData.Is(Flags::Int64))
				value = (UInt64)data.mValue.int64Data.intValue;
			else if (data.mValue.flagsData.Is(Flags::Int))
				value = (UInt64)data.mValue.intData.intValue;
			else if (data.mValue.flagsData.Is(Flags::UInt))
				value = (UInt64)data.mValue.intData.uintValue;
			else if (data.mValue.flagsData.Is(Flags::Double))
				value = (UInt64)data.mValue.doubleData.value;
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
			data.mValue.flagsData.flags = Flags::Double & Flags::Value;
			data.mValue.doubleData.value = value;
		}

		static void Read(double& value, const DataValue& data)
		{
			if (data.mValue.flagsData.Is(Flags::Double))
				value = data.mValue.doubleData.value;
			else if (data.mValue.flagsData.Is(Flags::Int))
				value = (double)data.mValue.intData.intValue;
			else if (data.mValue.flagsData.Is(Flags::UInt))
				value = (double)data.mValue.intData.uintValue;
			else if (data.mValue.flagsData.Is(Flags::Int64))
				value = (double)data.mValue.int64Data.intValue;
			else if (data.mValue.flagsData.Is(Flags::UInt64))
				value = (double)data.mValue.int64Data.uintValue;
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
			data.mValue.flagsData.flags = Flags::Double & Flags::Value;
			data.mValue.doubleData.value = (double)value;
		}

		static void Read(float& value, const DataValue& data)
		{
			if (data.mValue.flagsData.Is(Flags::Double))
				value = (float)data.mValue.doubleData.value;
			else if (data.mValue.flagsData.Is(Flags::Int))
				value = (float)data.mValue.intData.intValue;
			else if (data.mValue.flagsData.Is(Flags::UInt))
				value = (float)data.mValue.intData.uintValue;
			else if (data.mValue.flagsData.Is(Flags::Int64))
				value = (float)data.mValue.int64Data.intValue;
			else if (data.mValue.flagsData.Is(Flags::UInt64))
				value = (float)data.mValue.int64Data.uintValue;
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
			data.SetValue(value.Data());
		}

		static void Read(String& value, const DataValue& data)
		{
			using namespace rapidjson;

			GenericStringStream<UTF16<>> source((wchar_t*)data);
			GenericStringBuffer<UTF8<>> target;

			bool hasError = false;
			while (source.Peek() != '\0')
			{
				if (!Transcoder<UTF16<>, UTF8<>>::Transcode(source, target))
				{
					hasError = true;
					break;
				}
			}

			if (!hasError)
				value = target.GetString();
		}
	};

	template<>
	struct DataValue::Converter<WString>
	{
		static constexpr bool isSupported = true;

		static void Write(const WString& value, DataValue& data)
		{
			data.SetString(value.Data(), value.Length());
		}

		static void Read(WString& value, const DataValue& data)
		{
			Assert(data.mValue.flagsData.Is(DataValue::Flags::String), "Trying get string from not string value");

			if (data.mValue.flagsData.Is(DataValue::Flags::ShortString))
				value = data.mValue.shortStringData.stringValue;
			else
				value = data.mValue.stringPtrData.stringPtr;
		}
	};

	template<>
	struct DataValue::Converter<UID>
	{
		static constexpr bool isSupported = true;

		static void Write(const UID& value, DataValue& data)
		{
			data.SetValue((WString)value);
		}

		static void Read(UID& value, const DataValue& data)
		{
			WString buf;
			data.GetValue(buf);
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
				data.AddMember("Type").SetValue(value->GetType().GetName());
				data.AddMember("Value").SetValue(*value);
			}
		}

		static void Read(T& value, const DataValue& data)
		{
			if (auto typeNode = data.FindMember("Type"))
			{
				String typeName = *typeNode;

				if (auto valueNode = data.FindMember("Value"))
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
			data.Clear();

			for (auto v : value)
				data.AddMember("Element")->SetValue(v);
		}

		static void Read(Vector<T>& value, const DataValue& data)
		{
			for (auto childNode : data)
			{
				T v = T();
				childNode->GetValue(v);
				value.Add(v);
			}
		}
	};

	template<typename _key, typename _value>
	struct DataValue::Converter<Map<_key, _value>>
	{
		static constexpr bool isSupported = true;

		static void Write(const Map<_key, _value>& value, DataValue& data)
		{
			data.Clear();

			for (auto& kv : value)
			{
				DataValue& child = data.AddMember("Element");
				child.AddMember("Key").SetValue(kv.first);
				child.AddMember("Value").SetValue(kv.second);
			}
		}

		static void Read(Map<_key, _value>& value, const DataValue& data)
		{
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
	struct DataValue::Converter<T, typename std::enable_if<std::is_enum<T>::value>::type>
	{
		static constexpr bool isSupported = true;

		static void Write(const T& value, DataValue& data)
		{
			data.mData = Reflection::GetEnumName<T>(value);
		}

		static void Read(T& value, const DataValue& data)
		{
			value = Reflection::GetEnumValue<T>(data.mData);
		}
	};

	template<typename T>
	struct DataValue::Converter<T, typename std::enable_if<IsProperty<T>::value>::type>
	{
		static constexpr bool isSupported = DataValue::IsSupport<T::valueType>::value;
		using TValueType = typename T::valueType;

		static void Write(const T& value, DataValue& data)
		{
			data.SetValue(value.Get());
		}

		static void Read(T& value, const DataValue& data)
		{
			TValueType val;
			data.GetValue(val);
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

					for (auto field : type.GetFields())
					{
						auto srlzAttribute = field->GetAttribute<SerializableAttribute>();
						if (srlzAttribute)
						{
							auto fldNode = node.GetMember(field->GetName());
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

PRE_ENUM_META(o2::DataValue::Format);
