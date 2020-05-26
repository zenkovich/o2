#include "o2/stdafx.h"
#include "DataValue.h"

#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/Utils/Serialization/JsonDataFormat.h"

namespace o2
{
	DataValue::DataValue(DataDocument& document):
		mDocument(document), mValue()
	{
		mValue.flagsData.flags = Flags::Null;
	}

	DataValue::DataValue(DataValue& other):
		mDocument(other.mDocument), mValue(other.mValue)
	{
		other.mValue.flagsData.flags = Flags::Null;
	}

	DataValue::DataValue():
		mValue(), mDocument(*(DataDocument*)nullptr)
	{
		mValue.flagsData.flags = Flags::Null;
	}

	DataValue::DataValue(rapidjson::GenericStringBuffer<rapidjson::UTF16<>>& buffer, const char* str):
		mValue(), mDocument(*(DataDocument*)nullptr)
	{
		Transcode(buffer, str);
		SetString(buffer.GetString(), buffer.GetLength(), false);
	}

	DataValue::DataValue(const wchar_t* string, int length, bool isCopy, DataDocument& document):
		mValue(), mDocument(document)
	{
		SetString(string, length, isCopy);
	}

	bool DataValue::Transcode(rapidjson::GenericStringBuffer<rapidjson::UTF8<>>& target, const wchar_t* source)
	{
		rapidjson::GenericStringStream<rapidjson::UTF16<>> sourceStream(source);

		bool hasError = false;
		while (sourceStream.Peek() != '\0')
		{
			if (!rapidjson::Transcoder<rapidjson::UTF16<>, rapidjson::UTF8<>>::Transcode(sourceStream, target))
			{
				hasError = true;
				break;
			}
		}

		return !hasError;
	}

	bool DataValue::Transcode(rapidjson::GenericStringBuffer<rapidjson::UTF16<>>& target, const char* source)
	{
		rapidjson::GenericStringStream<rapidjson::UTF8<>> sourceStream(source);

		bool hasError = false;
		while (sourceStream.Peek() != '\0')
		{
			if (!rapidjson::Transcoder<rapidjson::UTF8<>, rapidjson::UTF16<>>::Transcode(sourceStream, target))
			{
				hasError = true;
				break;
			}
		}

		return !hasError;
	}

	DataValue::~DataValue()
	{
		// Not required, document's allocator does not require freeing memory
	}

	DataDocument& DataValue::GetDocument()
	{
		return mDocument;
	}

	const DataDocument& DataValue::GetDocument() const
	{
		return mDocument;
	}

	bool DataValue::IsValue() const
	{
		return mValue.flagsData.Is(Flags::Value);
	}

	bool DataValue::IsNumber() const
	{
		return mValue.flagsData.Is(Flags::Int) || mValue.flagsData.Is(Flags::UInt) ||
			mValue.flagsData.Is(Flags::Int64) || mValue.flagsData.Is(Flags::UInt64) ||
			mValue.flagsData.Is(Flags::Double);
	}

	bool DataValue::IsString() const
	{
		return mValue.flagsData.Is(Flags::String);
	}

	bool DataValue::IsBoolean() const
	{
		return mValue.flagsData.Is(Flags::Bool);
	}

	bool DataValue::IsNull() const
	{
		return mValue.flagsData.Is(Flags::Null);
	}

	void DataValue::SetNull()
	{
		mValue.flagsData.flags = Flags::Null;
	}

	void DataValue::SetString(const wchar_t* string, int length, bool isCopy)
	{
		if (isCopy)
		{
			if (length <= ShortStringData::maxLength)
			{
				memcpy(mValue.shortStringData.stringValue, string, (length + 1)*sizeof(wchar_t));
				mValue.flagsData.flags = Flags::String & Flags::ShortString;
			}
			else
			{
				UInt64 size = length*sizeof(wchar_t);
				mValue.stringPtrData.stringPtr = (wchar_t*)mDocument.mAllocator.Allocate(size);
				memcpy(mValue.stringPtrData.stringPtr, string, size);
				mValue.stringPtrData.stringLength = (size_t)length;
				mValue.flagsData.flags = Flags::String & Flags::StringCopy;
			}
		}
		else
		{
			mValue.stringPtrData.stringPtr = const_cast<wchar_t*>(string);
			mValue.stringPtrData.stringLength = length;
			mValue.flagsData.flags = Flags::String & Flags::StringRef;
		}
	}

	const wchar_t* DataValue::GetString() const
	{
		if (mValue.flagsData.Is(Flags::ShortString))
			return mValue.shortStringData.stringValue;
	}

	bool DataValue::IsArray() const
	{
		return mValue.flagsData.Is(Flags::Array);
	}

	bool DataValue::IsObject() const
	{
		return mValue.flagsData.Is(Flags::Object);
	}

	int DataValue::GetElementsCount() const
	{
		if (!IsArray())
			return -1;

		return mValue.arrayData.count;
	}

	bool DataValue::operator!=(const DataValue& other) const
	{
		return !(*this == other);
	}

	bool DataValue::operator==(const DataValue& other) const
	{
		if (IsArray())
		{
			if (!other.IsArray())
				return false;

			if (GetElementsCount() != other.GetElementsCount())
				return false;

			for (int i = 0; i < GetElementsCount(); i++)
			{
				if (GetElement(i) != other.GetElement(i))
					return false;
			}

			return true;
		}

		if (IsObject())
		{
			if (!other.IsObject())
				return false;

			if (mValue.objectData.count != other.mValue.objectData.count)
				return false;

			for (auto memberIt = BeginMember(); memberIt != EndMember(); ++memberIt)
			{
				auto otherMember = other.FindMember(memberIt->name);
				if (!otherMember || *otherMember != memberIt->value)
					return false;
			}

			return true;
		}

		if (IsString())
		{
			if (!other.IsString())
				return false;

			if (mValue.flagsData.Is(Flags::StringRef) && other.mValue.flagsData.Is(Flags::StringRef))
				return mValue.stringPtrData.stringPtr == mValue.stringPtrData.stringPtr;

			return wcscmp(GetString(), other.GetString()) == 0;
		}

		if (IsNumber())
		{
			if (mValue.flagsData.Is(Flags::Double) || mValue.flagsData.Is(Flags::Double))
			{
				double a, b;
				Get(a); other.Get(b);
				return a >= b && a <= b;
			}
			else if (mValue.flagsData.Is(Flags::Int))
				return (int)(*this) == (int)other;
			else if (mValue.flagsData.Is(Flags::UInt))
				return (UInt)(*this) == (UInt)other;
			else if (mValue.flagsData.Is(Flags::Int64))
				return (Int64)(*this) == (Int64)other;
			else if (mValue.flagsData.Is(Flags::UInt64))
				return (UInt64)(*this) == (UInt64)other;

			return false;
		}

		if (IsBoolean())
			return other.IsBoolean() && (bool)(*this) == (bool)other;

		if (IsNull())
			return other.IsNull();

		return false;
	}

	DataValue& DataValue::operator=(DataValue& other)
	{
		mValue = other.mValue;
		other.mValue.flagsData.flags = Flags::Null;
		return *this;
	}

	DataValue& DataValue::operator[](const DataValue& name)
	{
		return GetMember(name);
	}

	DataValue& DataValue::operator[](const char* name)
	{
		return GetMember(name);
	}

	const DataValue& DataValue::operator[](const DataValue& name) const
	{
		return GetMember(name);
	}

	const DataValue& DataValue::operator[](const char* name) const
	{
		return GetMember(name);
	}

	int DataValue::GetMembersCount() const
	{
		if (!IsObject())
			return 0;

		return mValue.objectData.count;
	}

	DataValue& DataValue::GetMember(const DataValue& name)
	{
		if (auto res = FindMember(name))
			return *res;

		return AddMember(name);
	}

	const DataValue& DataValue::GetMember(const DataValue& name) const
	{
		if (auto res = FindMember(name))
			return *res;

		Assert(false, "Can't access data member");

		static DataValue empty;
		return empty;
	}

	DataValue& DataValue::GetMember(const char* name)
	{
		rapidjson::GenericStringBuffer<rapidjson::UTF16<>> buf;
		return GetMember(DataValue(buf, name));
	}

	const DataValue& DataValue::GetMember(const char* name) const
	{
		rapidjson::GenericStringBuffer<rapidjson::UTF16<>> buf;
		return GetMember(DataValue(buf, name));
	}

	DataValue* DataValue::FindMember(const DataValue& name)
	{
		Assert(IsObject(), "Trying get member, but value isn't object");

		for (auto memberIt = BeginMember(); memberIt != EndMember(); ++memberIt)
		{
			if (memberIt->name == name)
				return &memberIt->value;
		}

		return nullptr;
	}

	const DataValue* DataValue::FindMember(const DataValue& name) const
	{
		Assert(IsObject(), "Trying get member, but value isn't object");

		for (auto memberIt = BeginMember(); memberIt != EndMember(); ++memberIt)
		{
			if (memberIt->name == name)
				return &memberIt->value;
		}

		return nullptr;
	}

	DataValue* DataValue::FindMember(const char* name)
	{
		rapidjson::GenericStringBuffer<rapidjson::UTF16<>> buf;
		return FindMember(DataValue(buf, name));
	}

	const DataValue* DataValue::FindMember(const char* name) const
	{
		rapidjson::GenericStringBuffer<rapidjson::UTF16<>> buf;
		return FindMember(DataValue(buf, name));
	}

	DataValue& DataValue::AddMember(DataValue& name)
	{
		if (!IsObject())
		{
			mValue.flagsData.flags = Flags::Object;

			mValue.objectData.members = (DataMember*)mDocument.mAllocator.Allocate(sizeof(DataMember)*ObjectInitialCapacity);
			mValue.objectData.capacity = ObjectInitialCapacity;
			mValue.objectData.count = 0;
		}

		if (mValue.objectData.count == mValue.objectData.capacity)
		{
			if (mValue.objectData.members)
			{
				UInt newCapacity = mValue.objectData.capacity*2;
				mValue.objectData.members = (DataMember*)mDocument.mAllocator.Reallocate(
					mValue.objectData.members, sizeof(DataMember)*mValue.objectData.capacity,
					sizeof(DataMember)*newCapacity);

				mValue.objectData.capacity = newCapacity;
			}
			else
			{
				mValue.objectData.members = (DataMember*)mDocument.mAllocator.Allocate(sizeof(DataMember)*ObjectInitialCapacity);
				mValue.objectData.capacity = ObjectInitialCapacity;
			}
		}

		DataMember* newMember = 
			new (mValue.objectData.members + mValue.objectData.count) DataMember(name, DataValue(mDocument));

		return newMember->value;
	}

	DataValue& DataValue::AddMember(const char* name)
	{
		return AddMember(DataValue(name, mDocument));
	}

	void DataValue::RemoveMember(const DataValue& name)
	{
		Assert(IsObject(), "Trying remove member, but value isn't object");

		for (auto memberIt = BeginMember(); memberIt != EndMember(); ++memberIt)
		{
			if (memberIt->name == name)
			{
				*memberIt = *(mValue.objectData.members + mValue.objectData.count - 1);
				mValue.objectData.count--;
			}
		}
	}

	DataMemberIterator DataValue::RemoveMember(DataMemberIterator it)
	{
		Assert(IsObject(), "Trying remove member, but value isn't object");
		Assert(it >= BeginMember() && it < EndMember(), "Iterator is invalid");

		*it = *(mValue.objectData.members + mValue.objectData.count - 1);
		mValue.objectData.count--;

		return it;
	}

	void DataValue::RemoveMember(const char* name)
	{
		rapidjson::GenericStringBuffer<rapidjson::UTF16<>> buf;
		RemoveMember(DataValue(buf, name));
	}

	DataMemberIterator DataValue::BeginMember()
	{
		Assert(IsObject(), "Trying get member iterator, but value isn't object");
		return DataMemberIterator(mValue.objectData.members);
	}

	ConstDataMemberIterator DataValue::BeginMember() const
	{
		Assert(IsObject(), "Trying get member iterator, but value isn't object");
		return ConstDataMemberIterator(mValue.objectData.members);
	}

	DataMemberIterator DataValue::EndMember()
	{
		Assert(IsObject(), "Trying get member iterator, but value isn't object");
		return DataMemberIterator(mValue.objectData.members + mValue.objectData.count);
	}

	ConstDataMemberIterator DataValue::EndMember() const
	{
		Assert(IsObject(), "Trying get member iterator, but value isn't object");
		return ConstDataMemberIterator(mValue.objectData.members + mValue.objectData.count);
	}

	DataValue& DataValue::operator[](const int idx)
	{
		return GetElement(idx);
	}

	DataValue& DataValue::GetElement(int idx)
	{
		Assert(IsArray(), "Trying get element, but value isn't array");
		Assert(idx >= 0 && idx < mValue.arrayData.count, "Index is out of range");
		return mValue.arrayData.elements[idx];
	}

	const DataValue& DataValue::GetElement(int idx) const
	{
		Assert(IsArray(), "Trying get element, but value isn't array");
		Assert(idx >= 0 && idx < mValue.arrayData.count, "Index is out of range");
		return mValue.arrayData.elements[idx];
	}

	DataValue& DataValue::AddElement(DataValue& value)
	{
		if (!IsArray())
		{
			mValue.flagsData.flags = Flags::Array;

			mValue.arrayData.elements = (DataValue*)mDocument.mAllocator.Allocate(sizeof(DataValue)*ArrayInitialCapacity);
			mValue.arrayData.capacity = ArrayInitialCapacity;
			mValue.arrayData.count = 0;
		}

		if (mValue.arrayData.count == mValue.arrayData.capacity)
		{
			UInt newCapacity = mValue.arrayData.capacity*2;
			mValue.arrayData.elements = (DataValue*)mDocument.mAllocator.Reallocate(
				mValue.arrayData.elements, sizeof(DataValue)*mValue.arrayData.capacity,
				sizeof(DataValue)*newCapacity);
			mValue.arrayData.capacity = newCapacity;
		}

		DataValue* newElement =
			new (mValue.arrayData.elements + mValue.arrayData.count) DataValue(mDocument);

		return *newElement;
	}

	DataValue* DataValue::RemoveElement(DataValue* it)
	{
		Assert(IsArray(), "Trying get element, but value isn't array");
		Assert(it >= Begin() && it < End(), "Iterator is invalid");

		*it = *(mValue.arrayData.elements + mValue.arrayData.count - 1);
		mValue.arrayData.count--;

		return it;
	}

	DataValue* DataValue::Begin()
	{
		Assert(IsArray(), "Trying get element iterator, but value isn't array");
		return mValue.arrayData.elements;
	}

	const DataValue* DataValue::Begin() const
	{
		Assert(IsArray(), "Trying get element iterator, but value isn't array");
		return mValue.arrayData.elements;
	}

	DataValue* DataValue::End()
	{
		Assert(IsArray(), "Trying get element iterator, but value isn't array");
		return mValue.arrayData.elements + mValue.arrayData.count;
	}

	const DataValue* DataValue::End() const
	{
		Assert(IsArray(), "Trying get element iterator, but value isn't array");
		return mValue.arrayData.elements + mValue.arrayData.count;
	}

	DataValue* DataValue::begin()
	{
		Assert(IsArray(), "Trying get element iterator, but value isn't array");
		return mValue.arrayData.elements;
	}

	const DataValue* DataValue::begin() const
	{
		Assert(IsArray(), "Trying get element iterator, but value isn't array");
		return mValue.arrayData.elements;
	}

	DataValue* DataValue::end()
	{
		Assert(IsArray(), "Trying get element iterator, but value isn't array");
		return mValue.arrayData.elements + mValue.arrayData.count;
	}

	const DataValue* DataValue::end() const
	{
		Assert(IsArray(), "Trying get element iterator, but value isn't array");
		return mValue.arrayData.elements + mValue.arrayData.count;
	}

	DataValue& DataValue::SetValueDelta(const IObject& object, const IObject& source)
	{
		struct helper
		{
			static void WriteObject(void* object, void* source, const ObjectType& type, DataValue& node)
			{
				for (auto baseType : type.GetBaseTypes())
				{
					const ObjectType* baseObjectType = dynamic_cast<const ObjectType*>(baseType.type);
					if (!baseObjectType)
						continue;

					void* baseObject = (*baseType.dynamicCastUpFunc)(object);
					void* baseSourceObject = (*baseType.dynamicCastUpFunc)(source);
					WriteObject(baseObject, baseSourceObject, *baseObjectType, node);
				}

				for (auto field : type.GetFields())
				{
					if (!field->GetAttribute<SerializableAttribute>())
						continue;

					if (field->GetType()->IsBasedOn(TypeOf(IObject)))
					{
						DataValue& newFieldNode = node.AddMember(field->GetName());

						newFieldNode.SetValueDelta(*(IObject*)field->GetValuePtr(object),
													*(IObject*)field->GetValuePtr(source));

						if (newFieldNode.IsEmpty())
							node.RemoveMember(field->GetName());

						continue;
					}

					if (!field->IsValueEquals(object, source))
					{
						DataValue& newFieldNode = node.AddMember(field->GetName());

						field->SerializeFromObject(object, newFieldNode);

						if (newFieldNode.IsEmpty())
							node.RemoveMember(field->GetName());
					}
				}
			}
		};

		if (!object.GetType().IsBasedOn(source.GetType()) && !source.GetType().IsBasedOn(object.GetType()))
			return Set(object);

		if (object.GetType().IsBasedOn(TypeOf(ISerializable)))
			((ISerializable&)object).OnSerialize(*this);

		const ObjectType& type = dynamic_cast<const ObjectType&>(object.GetType());
		void* objectPtr = type.DynamicCastFromIObject(const_cast<IObject*>(&object));
		void* sourcePtr = type.DynamicCastFromIObject(const_cast<IObject*>(&source));

		helper::WriteObject(objectPtr, sourcePtr, type, *this);

		return *this;
	}

	void DataValue::GetValueDelta(IObject& object, const IObject& source) const
	{
		struct helper
		{
			static void ReadObject(void* object, void* source, const ObjectType& type, const DataValue& node)
			{
				for (auto baseType : type.GetBaseTypes())
				{
					const ObjectType* baseObjectType = dynamic_cast<const ObjectType*>(baseType.type);
					if (!baseObjectType)
						continue;

					void* baseObject = (*baseType.dynamicCastUpFunc)(object);
					void* baseSourceObject = (*baseType.dynamicCastUpFunc)(source);
					ReadObject(baseObject, baseSourceObject, *baseObjectType, node);
				}

				for (auto field : type.GetFields())
				{
					if (!field->GetAttribute<SerializableAttribute>())
						continue;

					auto fldNode = node.FindMember(field->GetName());
					if (fldNode)
					{
						if (field->GetType()->IsBasedOn(TypeOf(IObject)))
						{
							fldNode->GetValueDelta(*(IObject*)field->GetValuePtr(object),
												   *(IObject*)field->GetValuePtr(source));
						}
						else field->DeserializeFromObject(object, *fldNode);
					}
					else field->CopyValue(object, source);
				}
			}
		};

		if (!object.GetType().IsBasedOn(source.GetType()) && !source.GetType().IsBasedOn(object.GetType()))
		{
			Get(object);
			return;
		}

		const ObjectType& type = dynamic_cast<const ObjectType&>(object.GetType());
		void* objectPtr = type.DynamicCastFromIObject(const_cast<IObject*>(&object));
		void* sourcePtr = type.DynamicCastFromIObject(const_cast<IObject*>(&source));
		helper::ReadObject(objectPtr, sourcePtr, type, *this);

		if (object.GetType().IsBasedOn(TypeOf(ISerializable)))
			((ISerializable&)object).OnDeserialized(*this);
	}

	bool DataValue::IsEmpty() const
	{
		if (IsObject())
			return mValue.objectData.count == 0;

		if (IsArray())
			return mValue.arrayData.count == 0;

		return true;
	}

	void DataValue::Clear()
	{
		if (IsObject())
			mValue.objectData.count = 0;
		else if (IsArray())
			mValue.arrayData.count = 0;
		else
			mValue.flagsData.flags = Flags::Null;
	}

	DataDocument::DataDocument():
		DataValue(*this), mAllocator()
	{}

	DataDocument::DataDocument(DataDocument& other):
		DataValue(other), mAllocator(other.mAllocator)
	{}

	DataDocument::~DataDocument()
	{
		mAllocator.Clear();
	}

	bool DataDocument::operator!=(const DataDocument& other) const
	{

	}

	bool DataDocument::operator==(const DataDocument& other) const
	{

	}

	DataDocument& DataDocument::operator=(DataDocument& other)
	{
		DataValue::operator=(other);
		mAllocator = other.mAllocator;
		return *this;
	}

	bool DataDocument::LoadFromFile(const String& fileName)
	{
		InFile file(fileName);

		if (!file.IsOpened())
			return false;

		auto sz = file.GetDataSize();
		auto length = sz/sizeof(wchar_t) + 1;
		wchar_t* data = (wchar_t*)mAllocator.Allocate(sizeof(wchar_t)*length);
		file.ReadData(data, sz);
		data[length] = '\0';

		auto extension = FileSystem::GetFileExtension(fileName);
		if (extension == "json")
			return ParseJsonInplace(data, *this);

		return false;
	}

	bool DataDocument::LoadFromData(const WString& data, Format format /*= Format::JSON*/)
	{
		if (format == Format::JSON)
			return ParseJson(data.Data(), *this);

		return false;
	}

	bool DataDocument::SaveToFile(const String& fileName, Format format /*= Format::JSON*/) const
	{
		WString data = SaveAsWString(format);

		OutFile file(fileName);
		if (!file.IsOpened())
			return false;

		file.WriteData(data.Data(), data.Length()*sizeof(wchar_t));

		return false;
	}

	WString DataDocument::SaveAsWString(Format format /*= Format::Xml*/) const
	{
		return "";
		//return XmlDataFormat::SaveDataDoc(*this);
	}

	DataValue::Flags operator&(const DataValue::Flags& a, const DataValue::Flags& b)
	{
		return static_cast<DataValue::Flags>(
			static_cast<std::underlying_type<DataValue::Flags>::type>(a) &
			static_cast<std::underlying_type<DataValue::Flags>::type>(b));
	}

	DataValue::Flags operator|(const DataValue::Flags& a, const DataValue::Flags& b)
	{
		return static_cast<DataValue::Flags>(
			static_cast<std::underlying_type<DataValue::Flags>::type>(a) |
			static_cast<std::underlying_type<DataValue::Flags>::type>(b));
	}

	DataMember::DataMember(DataValue& name, DataValue& value):
		name(name), value(value)
	{}

	DataMember& DataMember::operator=(DataMember& other)
	{
		name = other.name;
		value = other.value;
		return *this;
	}

	bool DataMember::operator==(const DataMember& other) const
	{
		return name == other.name && value == other.value;
	}

	bool DataMember::operator!=(const DataMember& other) const
	{
		return !(*this == other);
	}

}

ENUM_META(o2::DataValue::Flags)
{
	ENUM_ENTRY(Bool);
}
END_ENUM_META;

ENUM_META(o2::DataDocument::Format)
{
	ENUM_ENTRY(Binary);
	ENUM_ENTRY(JSON);
	ENUM_ENTRY(Xml);
}
END_ENUM_META;
