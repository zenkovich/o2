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

	DataNode::DataNode(const WString& name):
		mName(name), mParent(nullptr)
	{}

	DataNode::~DataNode()
	{
		Clear();
	}

	DataNode& DataNode::operator=(const DataNode& value)
	{
		return SetValue(value);
	}

	DataNode& DataNode::SetValue(const DataNode& other)
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

	DataNode& DataNode::SetValue(char* value)
	{
		mData = value;
		return *this;
	}

	DataNode& DataNode::SetValue(wchar_t* value)
	{
		mData = value;
		return *this;
	}

	DataNode& DataNode::SetValue(int value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(unsigned long value)
	{
		mData = (WString)(unsigned int)value;
		return *this;
	}

	DataNode& DataNode::SetValue(long long int value)
	{
		mData = (WString)(int)value;
		return *this;
	}

	DataNode& DataNode::SetValue(UInt64 value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(float value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(double value)
	{
		mData = (WString)(float)value;
		return *this;
	}

	DataNode& DataNode::SetValue(UInt value)
	{
		mData = (WString)value;
		return *this;
	}


	DataNode& DataNode::SetValue(bool value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(const String& value)
	{
		mData = value;
		return *this;
	}

	DataNode& DataNode::SetValue(const WString& value)
	{
		mData = value;
		return *this;
	}

	DataNode& DataNode::SetValue(const Vec2F& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(const Vec2I& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(const RectF& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(const RectI& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(const BorderF& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(const BorderI& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(const Color4& value)
	{
		mData = (WString)value;
		return *this;
	}

	DataNode& DataNode::SetValue(ISerializable& other)
	{
		*this = other.Serialize();
		return *this;
	}

	DataNode& DataNode::SetValue(const UID& value)
	{
		mData = (String)value;
		return *this;
	}

	void DataNode::DataNode::GetValue(wchar_t*& value) const
	{
		memcpy(value, mData.Data(), sizeof(wchar_t)*mData.Length() + 1);
	}

	void DataNode::GetValue(bool& value) const
	{
		value = (bool)mData;
	}

	void DataNode::GetValue(int& value) const
	{
		value = (int)mData;
	}

	void DataNode::GetValue(float& value) const
	{
		value = (float)mData;
	}

	void DataNode::GetValue(UInt& value) const
	{
		value = (UInt)mData;
	}

	void DataNode::GetValue(UInt64& value) const
	{
		value = (UInt64)mData;
	}

	void DataNode::GetValue(String& value) const
	{
		value = mData;
	}

	void DataNode::GetValue(WString& value) const
	{
		value = mData;
	}

	void DataNode::GetValue(Vec2F& value) const
	{
		value = (Vec2F)mData;
	}

	void DataNode::GetValue(Vec2I& value) const
	{
		value = (Vec2I)mData;
	}

	void DataNode::GetValue(RectF& value) const
	{
		value = (RectF)mData;
	}

	void DataNode::GetValue(RectI& value) const
	{
		value = (RectI)mData;
	}

	void DataNode::GetValue(BorderF& value) const
	{
		value = (BorderF)mData;
	}

	void DataNode::GetValue(BorderI& value) const
	{
		value = (BorderI)mData;
	}

	void DataNode::GetValue(Color4& value) const
	{
		value = (Color4)mData;
	}


	void DataNode::GetValue(char& value) const
	{
		value = (char)(int)mData;
	}

	void DataNode::GetValue(unsigned char& value) const
	{
		value = (unsigned char)(unsigned int)mData;
	}

	void DataNode::GetValue(wchar_t& value) const
	{
		value = (wchar_t)(int)mData;
	}

	void DataNode::GetValue(short& value) const
	{
		value = (short)(int)mData;
	}

	void DataNode::GetValue(unsigned short& value) const
	{
		value = (unsigned short)(unsigned int)mData;
	}

	void DataNode::GetValue(long& value) const
	{
		value = (long)(int)mData;
	}

	void DataNode::GetValue(unsigned long& value) const
	{
		value = (unsigned long)(unsigned int)mData;
	}

	void DataNode::GetValue(long long int& value) const
	{
		value = (long long int)(int)mData;
	}

	void DataNode::GetValue(DataNode& other) const
	{
		other = *this;
	}

	void DataNode::GetValue(UID& value) const
	{
		value = mData;
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
 
ENUM_META_(o2::DataNode::Format, Format)
{
	ENUM_ENTRY(Binary);
	ENUM_ENTRY(JSON);
	ENUM_ENTRY(Xml);
}
END_ENUM_META;
 