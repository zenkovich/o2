#include "DataDoc.h"

#include "Utils/FileSystem/File.h"
#include "Utils/Data/XmlDataFormat.h"

namespace o2
{

	DataNode::DataNode() :
		mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name) :
		mName(name), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, char* value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, wchar_t* value) :
		mName(name), mData(value), mParent(nullptr)
	{

	}

	DataNode::DataNode(const WString& name, bool value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}


	DataNode::DataNode(const WString& name, int value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, float value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, UInt value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const WString& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const String& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const Vec2F& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const Vec2I& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const RectF& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const RectI& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const Color4& value) :
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
			child.Release();

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

	DataNode& DataNode::operator=(wchar_t* value)
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
		mData = value;
		return *this;
	}

	DataNode& DataNode::operator=(const String& value)
	{
		mData = value;
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

	DataNode& DataNode::operator=(bool value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode::operator wchar_t*() const
	{
		return mData;
	}

	DataNode::operator WString() const
	{
		return mData;
	}

	DataNode::operator String() const
	{
		return mData;
	}

	DataNode::operator bool() const
	{
		return (bool)mData;
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

	Ptr<DataNode> DataNode::operator[](const WString& nodePath) const
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

	Ptr<DataNode> DataNode::GetParent() const
	{
		return mParent;
	}

	Ptr<DataNode> DataNode::GetNode(const WString& nodePath) const
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

	Ptr<DataNode> DataNode::AddNode(const WString& name)
	{
		DataNode* newNode = new DataNode(name);
		newNode->mParent = this;
		mChildNodes.Add(newNode);
		return newNode;
	}

	Ptr<DataNode> DataNode::AddNode(const Ptr<DataNode>& node)
	{
		mChildNodes.Add(node);
		node->mParent = this;
		return node;
	}

	bool DataNode::RemoveNode(const Ptr<DataNode>& node)
	{
		if (!mChildNodes.Contains(node))
			return false;

		mChildNodes.Remove(node);
		delete node;

		return true;
	}

	bool DataNode::RemoveNode(const WString& name)
	{
		int idx = mChildNodes.FindIdx([&](const Ptr<DataNode>& x){ return x->mName == name; });
		if (idx < 0)
			return false;

		Ptr<DataNode> node = mChildNodes.Get(idx);
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

	WString& DataNode::Data()
	{
		return mData;
	}


	DataDoc::DataDoc()
	{
	}

	DataDoc::DataDoc(const WString& fileName)
	{
		LoadFromFile(fileName);
	}

	bool DataDoc::LoadFromFile(const String& fileName)
	{
		InFile file(fileName);

		if (!file.IsOpened())
			return false;

		WString data;
		data.Reserve(file.GetDataSize() + 1);
		auto sz = file.ReadFullData(data.Data());
		data[sz/sizeof(wchar_t)] = '\0';

		return LoadFromData(data);
	}

	bool DataDoc::LoadFromData(const WString& data)
	{
		return XmlDataFormat::LoadDataDoc(data, *this);
	}

	bool DataDoc::SaveToFile(const String& fileName, Format format /*= Format::Xml*/) const
	{
		WString data = SaveAsWString(format);

		OutFile file(fileName);
		if (!file.IsOpened())
			return false;

		file.WriteData(data.Data(), data.Length()*sizeof(wchar_t));

		return false;
	}

	WString DataDoc::SaveAsWString(Format format /*= Format::Xml*/) const
	{
		return XmlDataFormat::SaveDataDoc(*this);
	}
}