#include "o2/stdafx.h"
#include "DataValue.h"

namespace o2
{
	DataValue::DataValue()
	{

	}

	DataValue::DataValue(const DataValue& other)
	{

	}

	DataValue::~DataValue()
	{

	}

	void DataValue::SetString(const wchar_t* string, int length)
	{

	}

	bool DataValue::IsArray() const
	{

	}

	bool DataValue::operator!=(const DataValue& other) const
	{

	}

	bool DataValue::operator==(const DataValue& other) const
	{

	}

	DataValue& DataValue::operator=(const DataValue& value)
	{

	}

	DataValue& DataValue::operator[](const int idx)
	{

	}

	DataValue& DataValue::operator[](const WString& name)
	{

	}

	DataValue& DataValue::operator[](const char* name)
	{

	}

	DataValue& DataValue::GetMember(const WString& path)
	{

	}

	const DataValue& DataValue::GetMember(const WString& path) const
	{

	}

	DataValue* DataValue::FindMember(const WString& path)
	{

	}

	const DataValue* DataValue::FindMember(const WString& path) const
	{

	}

	DataValue& DataValue::AddMember(const WString& name)
	{

	}

	DataValue::DataValue& DataValue::AddMember(const char* name)
	{

	}

	DataValue& DataValue::GetElement(int idx)
	{

	}

	const DataValue& DataValue::GetElement(int idx) const
	{

	}

	DataValue& DataValue::AddElement(DataValue& value)
	{

	}

	DataValue* DataValue::RemoveElement(DataValue* it)
	{

	}

	DataValue* DataValue::Begin()
	{

	}

	const DataValue* DataValue::Begin() const
	{

	}

	DataValue* DataValue::End()
	{

	}

	const DataValue* DataValue::End() const
	{

	}

	DataValue* DataValue::begin()
	{

	}

	const DataValue* DataValue::begin() const
	{

	}

	DataValue* DataValue::end()
	{

	}

	const DataValue* DataValue::end() const
	{

	}

	bool DataValue::IsObject() const
	{

	}

	void DataValue::RemoveMember(const WString& name)
	{

	}

	DataMemberIterator DataValue::RemoveMember(DataMemberIterator it)
	{

	}

	void DataValue::RemoveMember(const char* name)
	{

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
						DataValue* newFieldNode = mnew DataValue();
						newFieldNode->SetName(field->GetName());

						newFieldNode->SetValueDelta(*(IObject*)field->GetValuePtr(object),
													*(IObject*)field->GetValuePtr(source));

						if (!newFieldNode->IsEmpty())
							node.AddNode(newFieldNode);
						else
							delete newFieldNode;

						continue;
					}

					if (!field->IsValueEquals(object, source))
					{
						DataValue* newFieldNode = mnew DataValue();
						newFieldNode->SetName(field->GetName());

						field->SerializeFromObject(object, *newFieldNode);

						if (!newFieldNode->IsEmpty())
							node.AddNode(newFieldNode);
						else
							delete newFieldNode;
					}
				}
			}
		};

		if (!object.GetType().IsBasedOn(source.GetType()) && !source.GetType().IsBasedOn(object.GetType()))
			return SetValue(object);

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

					auto fldNode = node.GetMember(field->GetName());
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
			GetValue(object);
			return;
		}

		const ObjectType& type = dynamic_cast<const ObjectType&>(object.GetType());
		void* objectPtr = type.DynamicCastFromIObject(const_cast<IObject*>(&object));
		void* sourcePtr = type.DynamicCastFromIObject(const_cast<IObject*>(&source));
		helper::ReadObject(objectPtr, sourcePtr, type, *this);

		if (object.GetType().IsBasedOn(TypeOf(ISerializable)))
			((ISerializable&)object).OnDeserialized(*this);
	}

	DataMemberIterator DataValue::BeginMember()
	{

	}

	ConstDataMemberIterator DataValue::BeginMember() const
	{

	}

	DataMemberIterator DataValue::EndMember()
	{

	}

	ConstDataMemberIterator DataValue::EndMember() const
	{

	}

	bool DataValue::IsEmpty() const
	{

	}

	void DataValue::Clear()
	{

	}

	bool DataValue::LoadFromFile(const String& fileName)
	{
		InFile file(fileName);

		if (!file.IsOpened())
			return false;

		auto sz = file.GetDataSize();
		wchar_t* data = mnew wchar_t[sz + 1];
		file.ReadData(data, sz);
		data[sz/sizeof(wchar_t)] = '\0';

		bool res = LoadFromData(data);
		delete data;

		return res;
	}

	bool DataValue::LoadFromData(const WString& data)
	{
		return XmlDataFormat::LoadDataDoc(data, *this);
	}

	bool DataValue::SaveToFile(const String& fileName, Format format /*= Format::Xml*/) const
	{
		WString data = SaveAsWString(format);

		OutFile file(fileName);
		if (!file.IsOpened())
			return false;

		file.WriteData(data.Data(), data.Length()*sizeof(wchar_t));

		return false;
	}

	WString DataValue::SaveAsWString(Format format /*= Format::Xml*/) const
	{
		return XmlDataFormat::SaveDataDoc(*this);
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

}

ENUM_META(DataValue::Format)
{
	ENUM_ENTRY(Binary);
	ENUM_ENTRY(JSON);
	ENUM_ENTRY(Xml);
}
END_ENUM_META;
