#include "o2/stdafx.h"
#include "DataNode.h"

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
	DataNode::DataNode():
		mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name) :
		mName(name), mParent(nullptr)
	{ }

	DataNode::DataNode(const DataNode& other) :
		mName(other.mName), mData(other.mData), mParent(nullptr)
	{
		for (auto child : other.mChildNodes)
		{
			DataNode* newNode = mnew DataNode(*child);
			newNode->mParent = this;
			mChildNodes.Add(newNode);
		}
	}

	DataNode::~DataNode()
	{
		Clear();
	}

	DataNode& DataNode::operator=(const DataNode& value)
	{
		return SetValue(value);
	}

	DataNode& DataNode::SetValueDelta(const IObject& object, const IObject& source)
	{
		struct helper
		{
			static void WriteObject(void* object, void* source, const ObjectType& type, DataNode& node)
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
						DataNode* newFieldNode = mnew DataNode();
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
						DataNode* newFieldNode = mnew DataNode();
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

	void DataNode::GetValueDelta(IObject& object, const IObject& source) const
	{
		struct helper
		{
			static void ReadObject(void* object, void* source, const ObjectType& type, const DataNode& node)
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

					auto fldNode = node.GetNode(field->GetName());
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

	DataNode& DataNode::operator[](const WString& nodePath)
	{
		if (auto node = GetNode(nodePath))
			return *node;

		return *AddNode(nodePath);
	}

	DataNode& DataNode::operator[](const char* nodePath)
	{
		return operator[]((WString)nodePath);
	}

	bool DataNode::operator==(const DataNode& other) const
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

	bool DataNode::operator!=(const DataNode& other) const
	{
		return !(*this == other);
	}

	DataNode* DataNode::GetParent() const
	{
		return mParent;
	}

	DataNode* DataNode::GetNode(const WString& nodePath) const
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

	DataNode* DataNode::AddNode(const WString& name)
	{
		int delPos = name.Find("/");
		if (delPos >= 0)
		{
			WString namePart = name.SubStr(0, delPos);

			DataNode* node = GetNode(namePart);
			if (!node)
			{
				node = mnew DataNode();
				node->SetName(namePart);
				node->mParent = this;
				mChildNodes.Add(node);
			}

			return node->AddNode(name.SubStr(delPos + 1));
		}

		DataNode* newNode = mnew DataNode();
		newNode->SetName(name);
		newNode->mParent = this;
		mChildNodes.Add(newNode);

		return newNode;
	}

	DataNode* DataNode::AddNode(DataNode* node)
	{
		mChildNodes.Add(node);
		node->mParent = this;
		return node;
	}

	bool DataNode::RemoveNode(DataNode* node)
	{
		if (!mChildNodes.Contains(node))
			return false;

		mChildNodes.Remove(node);
		delete node;

		return true;
	}

	bool DataNode::RemoveNode(const WString& name)
	{
		int idx = mChildNodes.FindIdx([&](DataNode* x) { return x->mName == name; });
		if (idx < 0)
			return false;

		DataNode* node = mChildNodes.Get(idx);
		mChildNodes.RemoveAt(idx);
		delete node;

		return true;
	}

	WString DataNode::GetName() const
	{
		return mName;
	}

	void DataNode::SetName(const WString& name)
	{
		mName = name;
	}

	const Vector<DataNode*>& DataNode::GetChildNodes() const
	{
		return mChildNodes;
	}

	DataNode::Iterator DataNode::Begin()
	{
		return mChildNodes.Begin();
	}

	DataNode::ConstIterator DataNode::Begin() const
	{
		return mChildNodes.Begin();
	}

	DataNode::Iterator DataNode::End()
	{
		return mChildNodes.End();
	}

	DataNode::ConstIterator DataNode::End() const
	{
		return mChildNodes.End();
	}

	DataNode::Iterator DataNode::begin()
	{
		return mChildNodes.Begin();
	}

	DataNode::ConstIterator DataNode::begin() const
	{
		return mChildNodes.Begin();
	}

	DataNode::Iterator DataNode::end()
	{
		return mChildNodes.End();
	}

	DataNode::ConstIterator DataNode::end() const
	{
		return mChildNodes.End();
	}

	WString& DataNode::Data()
	{
		return mData;
	}

	const WString& DataNode::Data() const
	{
		return mData;
	}

	bool DataNode::IsEmpty() const
	{
		return mData.IsEmpty() && mChildNodes.IsEmpty();
	}

	void DataNode::Clear()
	{
		mData.Clear();

		for (auto child : mChildNodes)
			delete child;

		mChildNodes.Clear();
	}

	bool DataNode::LoadFromFile(const String& fileName)
	{
		InFile file(fileName);

		if (!file.IsOpened())
			return false;

		auto sz = file.GetDataSize();
		wchar_t* data = mnew wchar_t[sz];
		file.ReadData(data, file.GetDataSize());
		data[sz/sizeof(wchar_t)] = '\0';

		bool res = LoadFromData(data);
		delete data;

		return res;
	}

	bool DataNode::LoadFromData(const WString& data)
	{
		return XmlDataFormat::LoadDataDoc(data, *this);
	}

	bool DataNode::SaveToFile(const String& fileName, Format format /*= Format::Xml*/) const
	{
		WString data = SaveAsWString(format);

		OutFile file(fileName);
		if (!file.IsOpened())
			return false;

		file.WriteData(data.Data(), data.Length()*sizeof(wchar_t));

		return false;
	}

	WString DataNode::SaveAsWString(Format format /*= Format::Xml*/) const
	{
		return XmlDataFormat::SaveDataDoc(*this);
	}
}

ENUM_META_(o2::DataNode::Format, Format)
{
	ENUM_ENTRY(Binary);
	ENUM_ENTRY(JSON);
	ENUM_ENTRY(Xml);
}
END_ENUM_META;
