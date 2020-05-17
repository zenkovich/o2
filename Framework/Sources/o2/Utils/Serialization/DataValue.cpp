#include "o2/stdafx.h"
#include "DataValue.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/ActorDataNodeConverter.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Scene.h"
#include "o2/Utils/FileSystem/File.h"
#include "o2/Utils/Memory/MemoryManager.h"
#include "o2/Utils/Reflection/Type.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Serialization/XmlDataFormat.h"

namespace o2
{
	DataValue::DataValue():
		mParent(nullptr)
	{}

	DataValue::DataValue(const WString& name) :
		mName(name), mParent(nullptr)
	{ }

	DataValue::DataValue(const DataValue& other) :
		mName(other.mName), mData(other.mData), mParent(nullptr)
	{
		for (auto child : other.mChildNodes)
		{
			DataValue* newNode = mnew DataValue(*child);
			newNode->mParent = this;
			mChildNodes.Add(newNode);
		}
	}

	DataValue::~DataValue()
	{
		Clear();
	}

	DataValue& DataValue::operator=(const DataValue& value)
	{
		return SetValue(value);
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

	DataValue& DataValue::operator[](const WString& nodePath)
	{
		if (auto node = GetMember(nodePath))
			return *node;

		return *AddNode(nodePath);
	}

	DataValue& DataValue::operator[](const char* nodePath)
	{
		return operator[]((WString)nodePath);
	}

	bool DataValue::operator==(const DataValue& other) const
	{
		if (mName != other.mName || mData != other.mData || mChildNodes.Count() != other.mChildNodes.Count())
			return false;
		
		for (int i = 0; i < mChildNodes.Count(); i++)
		{
			if (*mChildNodes[i] != *other.mChildNodes[i])
				return false;
		}

		return true;
	}

	bool DataValue::operator!=(const DataValue& other) const
	{
		return !(*this == other);
	}

	DataValue* DataValue::GetParent() const
	{
		return mParent;
	}

	DataValue* DataValue::GetMember(const WString& nodePath) const
	{
		int delPos = nodePath.Find("/");
		WString pathPart = nodePath.SubStr(0, delPos);

		if (pathPart == "..")
		{
			if (mParent)
			{
				if (delPos == -1)
					return mParent;
				else
					return mParent->GetNode(nodePath.SubStr(delPos + 1));
			}

			return nullptr;
		}

		for (auto child : mChildNodes)
		{
			if (child->mName == pathPart)
			{
				if (delPos == -1)
					return child;
				else
					return child->GetNode(nodePath.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	DataValue* DataValue::AddNode(const WString& name)
	{
		int delPos = name.Find("/");
		if (delPos >= 0)
		{
			WString namePart = name.SubStr(0, delPos);

			DataValue* node = GetMember(namePart);
			if (!node)
			{
				node = mnew DataValue();
				node->SetName(namePart);
				node->mParent = this;
				mChildNodes.Add(node);
			}

			return node->AddNode(name.SubStr(delPos + 1));
		}

		DataValue* newNode = mnew DataValue();
		newNode->SetName(name);
		newNode->mParent = this;
		mChildNodes.Add(newNode);

		return newNode;
	}

	DataValue* DataValue::AddNode(DataValue* node)
	{
		mChildNodes.Add(node);
		node->mParent = this;
		return node;
	}

	bool DataValue::RemoveNode(DataValue* node)
	{
		if (!mChildNodes.Contains(node))
			return false;

		mChildNodes.Remove(node);
		delete node;

		return true;
	}

	bool DataValue::RemoveNode(const WString& name)
	{
		int idx = mChildNodes.FindIdx([&](DataValue* x) { return x->mName == name; });
		if (idx < 0)
			return false;

		DataValue* node = mChildNodes.Get(idx);
		mChildNodes.RemoveAt(idx);
		delete node;

		return true;
	}

	WString DataValue::GetName() const
	{
		return mName;
	}

	void DataValue::SetName(const WString& name)
	{
		mName = name;
	}

	const Vector<DataValue*>& DataValue::GetChildNodes() const
	{
		return mChildNodes;
	}

	DataValue::Iterator DataValue::Begin()
	{
		return mChildNodes.Begin();
	}

	DataValue::ConstIterator DataValue::Begin() const
	{
		return mChildNodes.Begin();
	}

	DataValue::Iterator DataValue::End()
	{
		return mChildNodes.End();
	}

	DataValue::ConstIterator DataValue::End() const
	{
		return mChildNodes.End();
	}

	DataValue::Iterator DataValue::begin()
	{
		return mChildNodes.Begin();
	}

	DataValue::ConstIterator DataValue::begin() const
	{
		return mChildNodes.Begin();
	}

	DataValue::Iterator DataValue::end()
	{
		return mChildNodes.End();
	}

	DataValue::ConstIterator DataValue::end() const
	{
		return mChildNodes.End();
	}

	WString& DataValue::Data()
	{
		return mData;
	}

	const WString& DataValue::Data() const
	{
		return mData;
	}

	bool DataValue::IsEmpty() const
	{
		return mData.IsEmpty() && mChildNodes.IsEmpty();
	}

	void DataValue::Clear()
	{
		mData.Clear();

		for (auto child : mChildNodes)
			delete child;

		mChildNodes.Clear();
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
}

ENUM_META(o2::DataValue::Format)
{
	ENUM_ENTRY(Binary);
	ENUM_ENTRY(JSON);
	ENUM_ENTRY(Xml);
}
END_ENUM_META;
