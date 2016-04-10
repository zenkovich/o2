#include "DataNode.h"

#include "Scene/Actor.h"
#include "Scene/Component.h"
#include "Scene/Scene.h"
#include "Utils/Data/XmlDataFormat.h"
#include "Utils/FileSystem/File.h"
#include "Utils/Memory/MemoryManager.h"
#include "Utils/Reflection/Type.h"
#include "Utils/Serializable.h"

namespace o2
{
	DataNode::DataNode():
		mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name) :
		mName(name), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, char* value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, wchar_t* value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, bool value) :
		mName(name), mData(value), mParent(nullptr)
	{}


	DataNode::DataNode(const WString& name, int value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, float value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, UInt value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, const String& value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, const WString& value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, const Vec2F& value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, const Vec2I& value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, const RectF& value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, const RectI& value) :
		mName(name), mData(value), mParent(nullptr)
	{}

	DataNode::DataNode(const WString& name, const Color4& value) :
		mName(name), mData(value), mParent(nullptr)
	{}

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

	DataNode::DataNode(const WString& name, ISerializable& value):
		mName(mName), mParent(nullptr)
	{
		*this = value.Serialize();
	}

	DataNode::DataNode(int value):
		mData(value), mParent(nullptr)
	{}

	DataNode::~DataNode()
	{
		Clear();
	}

	DataNode& DataNode::operator=(const DataNode& other)
	{
		for (auto child : mChildNodes)
			delete child;

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

	DataNode& DataNode::operator=(unsigned long value)
	{
		mData = (WString)(unsigned int)value;
		return *this;
	}

	DataNode& DataNode::operator=(long long int value)
	{
		mData = (WString)(int)value;
		return *this;
	}

	DataNode& DataNode::operator=(UInt64 value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(float value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::operator=(double value)
	{
		mData = (WString)(float)value;
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

	DataNode& DataNode::operator=(ISerializable& other)
	{
		*this = other.Serialize();
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

	DataNode::operator UInt64() const
	{
		return (UInt64)mData;
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


	DataNode::operator char() const
	{
		return (char)(int)mData;
	}

	DataNode::operator unsigned char() const
	{
		return (unsigned char)(unsigned int)mData;
	}

	DataNode::operator wchar_t() const
	{
		return (wchar_t)(int)mData;
	}

	DataNode::operator short() const
	{
		return (short)(int)mData;
	}

	DataNode::operator unsigned short() const
	{
		return (unsigned short)(unsigned int)mData;
	}

	DataNode::operator long() const
	{
		return (long)(int)mData;
	}

	DataNode::operator unsigned long() const
	{
		return (unsigned long)(unsigned int)mData;
	}

	DataNode::operator long long int() const
	{
		return (long long int)(int)mData;
	}

	DataNode* DataNode::operator[](const WString& nodePath)
	{
		auto node = GetNode(nodePath);
		if (!node)
			return AddNode(nodePath);

		return node;
	}

	DataNode* DataNode::operator[](const char* nodePath)
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
				node = mnew DataNode(namePart);
				node->mParent = this;
				mChildNodes.Add(node);
			}

			return node->AddNode(name.SubStr(delPos + 1));
		}

		DataNode* newNode = mnew DataNode(name);
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

	void DataNode::RegDataConverter(IDataNodeTypeConverter* converter)
	{
		mDataConverters.Add(converter);
	}

	WString& DataNode::Data()
	{
		return mData;
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

	void DataNode::RegBasicConverters()
	{
		RegDataConverter(new ActorDataNodeConverter());
		RegDataConverter(new ComponentDataNodeConverter());
		RegDataConverter(new LayerDataNodeConverter());
		RegDataConverter(new TagDataNodeConverter());
	}

	Vector<IDataNodeTypeConverter*> DataNode::mDataConverters;
}