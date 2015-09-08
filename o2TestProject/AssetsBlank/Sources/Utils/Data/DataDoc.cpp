#include "DataDoc.h"

#include "Utils/Data/XmlDataFormat.h"
#include "Utils/FileSystem/File.h"
#include "Utils/Serialization.h"

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

	DataNode::DataNode(const WString& name, const String& value) :
		mName(name), mData(value), mParent(nullptr)
	{
	}

	DataNode::DataNode(const WString& name, const WString& value) :
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

	DataNode::DataNode(const DataNode& other) :
		mName(other.mName), mData(other.mData), mParent(nullptr)
	{
		for (auto child : other.mChildNodes)
		{
			Ptr<DataNode> newNode = mnew DataNode(*child);
			newNode->mParent = this;
			mChildNodes.Add(newNode);
		}
	}

	DataNode::DataNode(const WString& name, ISerializable& value) :
		mName(mName), mParent(nullptr)
	{
		*this = value.Serialize();
	}

	DataNode::DataNode(int value):
		mData(value), mParent(nullptr)
	{
	}

	DataNode::~DataNode()
	{
		Clear();
	}

	DataNode& DataNode::operator=(const DataNode& other)
	{
		for (auto child : mChildNodes)
			child.Release();

		mChildNodes.Clear();

		for (auto child : other.mChildNodes)
			mChildNodes.Add(mnew DataNode(*child));

		//mName = other.mName;
		mData = other.mData;

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


	DataNode& DataNode::operator=(bool value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(const String& value)
	{
		mData = value;
		return *this;
	}

	DataNode& DataNode::operator=(const WString& value)
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

	DataNode::operator wchar_t*() const
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

	DataNode::operator String() const
	{
		return mData;
	}

	DataNode::operator WString() const
	{
		return mData;
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


	Ptr<DataNode> DataNode::operator[](const WString& nodePath)
	{
		auto node = GetNode(nodePath);
		if (!node)
			return AddNode(nodePath);

		return node;
	}

 	Ptr<DataNode> DataNode::operator[](const char* nodePath)
 	{
		return operator[]((WString)nodePath);
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

	Ptr<DataNode> DataNode::AddNode(const WString& name)
	{
		int delPos = name.Find("/");
		if (delPos >= 0)
		{
			WString namePart = name.SubStr(0, delPos);

			Ptr<DataNode> node = GetNode(namePart);
			if (!node)
			{
				node = mnew DataNode(namePart);
				node->mParent = this;
				mChildNodes.Add(node);
			}

			return node->AddNode(name.SubStr(delPos + 1));
		}

		Ptr<DataNode> newNode = mnew DataNode(name);
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

	bool DataNode::RemoveNode(Ptr<DataNode>& node)
	{
		if (!mChildNodes.Contains(node))
			return false;

		mChildNodes.Remove(node);
		node.Release();

		return true;
	}

	bool DataNode::RemoveNode(const WString& name)
	{
		int idx = mChildNodes.FindIdx([&](const Ptr<DataNode>& x) { return x->mName == name; });
		if (idx < 0)
			return false;

		Ptr<DataNode> node = mChildNodes.Get(idx);
		mChildNodes.RemoveAt(idx);
		node.Release();

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

	const DataNode::DataNodesVec& DataNode::GetChildNodes() const
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

	DataNode* DataNode::CreateSample() const
	{
		return new DataNode();
	}

	void DataNode::Clear()
	{
		mData.Clear();

		for (auto child : mChildNodes)
			child.Release();

		mChildNodes.Clear();
	}

	bool DataNode::LoadFromFile(const String& fileName)
	{
		InFile file(fileName);

		if (!file.IsOpened())
			return false;

		WString data;
		data.Reserve(file.GetDataSize() + 1);
		auto sz = file.ReadFullData(data.Data());
		data[sz / sizeof(wchar_t)] = '\0';

		return LoadFromData(data);
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