#include "DataDoc.h"

#include "Utils/FileSystem/File.h"
#include "Utils/Data/XmlDataFormat.h"

namespace o2
{

	DataNode::DataNode() :
		mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name) :
		mName(name), mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name, char* value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name, int value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name, float value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name, UInt value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name, const String& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name, const Vec2F& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name, const Vec2I& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name, const RectF& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name, const RectI& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const String& name, const Color4& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const DataNode& other)
	{
		for (auto child : other.mChildNodes)
			mChildNodes.Add(new DataNode(*child));
	}

	DataNode::~DataNode()
	{
		for (auto child : mChildNodes)
			delete child;

		mChildNodes.Clear();
	}

	DataNode& DataNode::operator=(const DataNode& other)
	{
		for (auto child : mChildNodes)
			delete child;

		mChildNodes.Clear();

		for (auto child : other.mChildNodes)
			mChildNodes.Add(new DataNode(*child));

		return *this;
	}

	DataNode& DataNode::operator=(char* value)
	{
		mData = value;
		return *this;
	}

	DataNode& DataNode::operator=(int value)
	{
		mData = (String)value;
		return *this;
	}

	DataNode& DataNode::operator=(float value)
	{
		mData = (String)value;
		return *this;
	}

	DataNode& DataNode::operator=(UInt value)
	{
		mData = (String)value;
		return *this;
	}

	DataNode& DataNode::operator=(const String& value)
	{
		mData = (String)value;
		return *this;
	}

	DataNode& DataNode::operator=(const Vec2F& value)
	{
		mData = (String)value;
		return *this;
	}

	DataNode& DataNode::operator=(const Vec2I& value)
	{
		mData = (String)value;
		return *this;
	}

	DataNode& DataNode::operator=(const RectF& value)
	{
		mData = (String)value;
		return *this;
	}

	DataNode& DataNode::operator=(const RectI& value)
	{
		mData = (String)value;
		return *this;
	}

	DataNode& DataNode::operator=(const Color4& value)
	{
		mData = (String)value;
		return *this;
	}

	DataNode::operator char*() const
	{
		return mData;
	}

	DataNode::operator String() const
	{
		return mData;
	}

	DataNode::operator int() const
	{
		return (int)mData;
	}

	DataNode::operator float() const
	{
		return (float)mData;
	}

	DataNode::operator UInt() const
	{
		return (UInt)mData;
	}

	DataNode::operator Vec2F() const
	{
		return (Vec2F)mData;
	}

	DataNode::operator Vec2I() const
	{
		return (Vec2I)mData;
	}

	DataNode::operator RectF() const
	{
		return (RectF)mData;
	}

	DataNode::operator RectI() const
	{
		return (RectI)mData;
	}

	DataNode::operator Color4() const
	{
		return (Color4)mData;
	}

	DataNode* DataNode::operator[](const String& nodePath) const
	{
		return GetNode(nodePath);
	}

	bool DataNode::operator==(const DataNode& other) const
	{
		return mName == other.mName && mData == other.mData && mChildNodes == other.mChildNodes;
	}

	bool DataNode::operator!=(const DataNode& other) const
	{
		return mName != other.mName || mData != other.mData || mChildNodes != other.mChildNodes;
	}

	DataNode* DataNode::GetParent() const
	{
		return mParent;
	}

	DataNode* DataNode::GetNode(const String& nodePath) const
	{
		int delPos = nodePath.Find("/");
		String pathPart = nodePath.SubStr(0, delPos);

		if (pathPart == "..")
		{
			if (mParent)
			{
				if (delPos == -1)
					return mParent;
				else
					return mParent->GetNode(nodePath.SubStr(delPos + 1));
			}

			return NULL;
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

	DataNode* DataNode::AddNode(const String& name)
	{
		DataNode* newNode = new DataNode(name);
		newNode->mParent = this;
		mChildNodes.Add(newNode);
		return newNode;
	}

	DataNode* DataNode::AddNode(DataNode* const node)
	{
		mChildNodes.Add(node);
		node->mParent = this;
		return node;
	}

	bool DataNode::RemoveNode(DataNode* const node)
	{
		if (!mChildNodes.Contains(node))
			return false;

		mChildNodes.Remove(node);
		delete node;

		return true;
	}

	bool DataNode::RemoveNode(const String& name)
	{
		int idx = mChildNodes.FindIdx(&[&](DataNode* const x){ return x->mName == name; });
		if (idx < 0)
			return false;

		DataNode* node = mChildNodes.Get(idx);
		mChildNodes.RemoveAt(idx);
		delete node;

		return true;
	}

	String DataNode::GetName() const
	{
		return mName;
	}

	void DataNode::SetName(const String& name)
	{
		mName = name;
	}

	const DataNode::DataNodesArr& DataNode::GetChildNodes() const
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


	DataDoc::DataDoc()
	{
	}

	DataDoc::DataDoc(const String& fileName)
	{
		LoadFromFile(fileName);
	}

	DataDoc::DataDoc(const DataDoc& other)
	{
		for (auto node : other.mChildNodes)
			mChildNodes.Add(new DataNode(*node));
	}

	DataDoc::~DataDoc()
	{
		for (auto node : mChildNodes)
			delete node;

		mChildNodes.Clear();
	}

	DataDoc& DataDoc::operator=(const DataDoc& other)
	{
		for (auto node : mChildNodes)
			delete node;

		mChildNodes.Clear();

		for (auto node : other.mChildNodes)
			mChildNodes.Add(new DataNode(*node));

		return *this;
	}

	DataNode* DataDoc::operator[](const String& nodePath) const
	{
		return GetNode(nodePath);
	}

	bool DataDoc::LoadFromFile(const String& fileName)
	{
		InFile file(fileName);

		if (!file.IsOpened())
			return false;

		String data;
		data.Reserve(file.GetDataSize() + 1);
		auto sz = file.ReadFullData(data.Data());
		data[sz] = '\0';

		return LoadFromData(data);
	}

	bool DataDoc::LoadFromData(const String& data)
	{
		return XmlDataFormat::LoadDataDoc(data, *this);
	}

	bool DataDoc::SaveToFile(const String& fileName, Format format /*= Format::Xml*/) const
	{
		String data = SaveAsString(format);

		OutFile file(fileName);
		if (!file.IsOpened())
			return false;

		file.WriteData(data.Data(), data.Length());

		return false;
	}

	String DataDoc::SaveAsString(Format format /*= Format::Xml*/) const
	{
		return XmlDataFormat::SaveDataDoc(*this);
	}

	DataNode* DataDoc::GetNode(const String& nodePath) const
	{
		int delPos = nodePath.Find("/");
		String pathPart = nodePath.SubStr(0, delPos);

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

	DataNode* DataDoc::AddNode(const String& name)
	{
		DataNode* newNode = new DataNode(name);
		mChildNodes.Add(newNode);
		return newNode;
	}

	DataNode* DataDoc::AddNode(DataNode* const node)
	{
		mChildNodes.Add(node);
		node->mParent = nullptr;
		return node;
	}

	bool DataDoc::RemoveNode(DataNode* const node)
	{
		if (!mChildNodes.Contains(node))
			return false;

		mChildNodes.Remove(node);
		delete node;

		return true;
	}

	bool DataDoc::RemoveNode(const String& name)
	{
		int idx = mChildNodes.FindIdx(&[&](DataNode* const x){ return x->mName == name; });
		if (idx < 0)
			return false;

		DataNode* node = mChildNodes.Get(idx);
		mChildNodes.RemoveAt(idx);
		delete node;

		return true;
	}

	const DataNode::DataNodesArr& DataDoc::GetChildNodes() const
	{
		return mChildNodes;
	}

	DataDoc::Iterator DataDoc::Begin()
	{
		return mChildNodes.Begin();
	}

	DataDoc::ConstIterator DataDoc::Begin() const
	{
		return mChildNodes.Begin();
	}

	DataDoc::Iterator DataDoc::End()
	{
		return mChildNodes.End();
	}

	DataDoc::ConstIterator DataDoc::End() const
	{
		return mChildNodes.End();
	}

	DataDoc::Iterator DataDoc::begin()
	{
		return mChildNodes.Begin();
	}

	DataDoc::ConstIterator DataDoc::begin() const
	{
		return mChildNodes.Begin();
	}

	DataDoc::Iterator DataDoc::end()
	{
		return mChildNodes.End();
	}

	DataDoc::ConstIterator DataDoc::end() const
	{
		return mChildNodes.End();
	}

}