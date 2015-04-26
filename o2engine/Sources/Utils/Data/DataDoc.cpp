#include "DataDoc.h"

#include "Utils/FileSystem/File.h"

namespace o2
{

	DataNode::DataNode():
		mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name):
		mName(name), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, char* value):
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, int value):
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, float value):
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, UInt value):
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const WString& value):
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const Vec2F& value):
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const Vec2I& value):
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const RectF& value):
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const RectI& value):
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const Color4& value):
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::~DataNode()
	{
		if (mParent)
			mParent->mChildNodes.Remove(this);
	}

	DataNode& DataNode::operator=(char* value)
	{
		mData = value;
		return *this;
	}

	DataNode& DataNode::operator=(int value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(float value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(UInt value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(const WString& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(const Vec2F& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(const Vec2I& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(const RectF& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(const RectI& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(const Color4& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(const DataNode& other)
	{
		mChildNodes = other.mChildNodes;
		mData = other.mData;
		mName = other.mName;
		return *this;
	}

	DataNode::operator char*() const
	{
		return (String)mData;
	}

	DataNode::operator WString() const
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

	DataNode DataNode::operator[](const WString& nodePath) const
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

			return NULL;
		}

		for (auto child:mChildNodes)
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
		DataNode* newNode = new DataNode(name);
		newNode->mParent = this;
		mChildNodes.Add(newNode);
		return newNode;
	}

	bool DataNode::RemoveNode(const DataNode* node)
	{
		if (!mChildNodes.Contains(node))
			return false;

		mChildNodes.Remove(node);
		delete node;

		return true;
	}

	bool DataNode::RemoveNode(const WString& name)
	{
		return true;
	}

	WString DataNode::GetName() const
	{
		return WString();
	}

	void DataNode::SetName(const WString& name)
	{

	}


	DataDoc::DataDoc(Format format /*= Format::Xml*/)
	{

	}

	DataDoc::DataDoc(const WString& fileName, Format format /*= Format::Xml*/)
	{

	}

	DataDoc::DataDoc(const DataDoc& other)
	{

	}

	DataDoc::~DataDoc()
	{

	}

	DataDoc& DataDoc::operator=(const DataDoc& other)
	{

	}

	DataNode DataDoc::operator[](const WString& nodePath) const
	{

	}

	bool DataDoc::LoadFromFile(const WString& fileName)
	{

	}

	bool DataDoc::LoadFromData(const WString& data)
	{

	}

	bool DataDoc::SaveToFile(const WString& fileName) const
	{

	}

	WString DataDoc::SaveAsWString() const
	{

	}

	DataNode DataDoc::GetNode(const WString& nodePath) const
	{

	}

	DataNode DataDoc::AddNode(const WString& name)
	{

	}

	bool DataDoc::RemoveNode(const DataNode& node)
	{

	}

	bool DataDoc::RemoveNode(const WString& name)
	{

	}

}