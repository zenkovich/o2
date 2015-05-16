#pragma once

#include "Utils/Containers/Array.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/String.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Memory/IObject.h"

namespace o2
{
	class DataNode: public IObject
	{
		friend class DataDoc;

	public:
		typedef Array<Ptr<DataNode>> DataNodesArr;
		typedef DataNode::DataNodesArr::Iterator Iterator;
		typedef DataNode::DataNodesArr::ConstIterator ConstIterator;

	protected:
		WString      mName;
		WString      mData;
		DataNode*    mParent;
		DataNodesArr mChildNodes;

	public:
		DataNode();
		DataNode(const DataNode& other);
		DataNode(const WString& name);
		DataNode(const WString& name, char* value);
		DataNode(const WString& name, wchar_t* value);
		DataNode(const WString& name, bool value);
		DataNode(const WString& name, int value);
		DataNode(const WString& name, float value);
		DataNode(const WString& name, UInt value);
		DataNode(const WString& name, const String& value);
		DataNode(const WString& name, const WString& value);
		DataNode(const WString& name, const Vec2F& value);
		DataNode(const WString& name, const Vec2I& value);
		DataNode(const WString& name, const RectF& value);
		DataNode(const WString& name, const RectI& value);
		DataNode(const WString& name, const Color4& value);

		template<typename _type>
		DataNode(const WString& name, const Ptr<_type>& value);

		template<typename _type>
		DataNode(const WString& name, const Array<_type>& value);

		template<typename _key, typename _value>
		DataNode(const WString& name, const Dictionary<_key, _value>& value);

		virtual ~DataNode();

		DataNode& operator=(const DataNode& other);
		DataNode& operator=(char* value);
		DataNode& operator=(wchar_t* value);
		DataNode& operator=(int value);
		DataNode& operator=(bool value);
		DataNode& operator=(float value);
		DataNode& operator=(UInt value);
		DataNode& operator=(const String& value);
		DataNode& operator=(const WString& value);
		DataNode& operator=(const Vec2F& value);
		DataNode& operator=(const Vec2I& value);
		DataNode& operator=(const RectF& value);
		DataNode& operator=(const RectI& value);
		DataNode& operator=(const Color4& value);

		template<typename _type>
		DataNode& operator=(const Ptr<_type>& value);

		template<typename _type>
		DataNode& operator=(const Array<_type>& value);

		template<typename _key, typename _value>
		DataNode& operator=(const Dictionary<_key, _value>& value);

		operator wchar_t*() const;
		operator bool() const;
		operator int() const;
		operator float() const;
		operator UInt() const;
		operator String() const;
		operator WString() const;
		operator Vec2F() const;
		operator Vec2I() const;
		operator RectF() const;
		operator RectI() const;
		operator Color4() const;
		operator DataNode() { return *this; }

		template<typename _type>
		operator Ptr<_type>() const;

		template<typename _type>
		operator Array<_type>() const;

		template<typename _key, typename _value>
		operator Dictionary<_key, _value>() const;

		Ptr<DataNode> operator[](const WString& nodePath) const;
		Ptr<DataNode> operator[](const char* nodePath) const;
		bool operator==(const DataNode& other) const;
		bool operator!=(const DataNode& other) const;

		DataNode* CreateSample() const;

		void Clear();

		Ptr<DataNode> GetParent() const;

		Ptr<DataNode> GetNode(const WString& nodePath) const;
		Ptr<DataNode> AddNode(const WString& name);
		Ptr<DataNode> AddNode(const Ptr<DataNode>& node);
		bool RemoveNode(const Ptr<DataNode>& node);
		bool RemoveNode(const WString& name);

		WString GetName() const;
		void SetName(const WString& name);

		WString& Data();

		const DataNode::DataNodesArr& GetChildNodes() const;

		Iterator Begin();
		Iterator End();

		ConstIterator Begin() const;
		ConstIterator End() const;

		Iterator begin();
		Iterator end();

		ConstIterator begin() const;
		ConstIterator end() const;
	};

	class DataDoc : public DataNode
	{
	public:
		enum class Format { Xml, JSON, Binary };

	public:
		DataDoc();
		DataDoc(const DataNode& node);
		DataDoc(const WString& fileName);

		DataDoc& operator=(const DataNode& other);

		bool LoadFromFile(const String& fileName);
		bool LoadFromData(const WString& data);

		bool SaveToFile(const String& fileName, Format format = Format::Xml) const;
		WString SaveAsWString(Format format = Format::Xml) const;
	};
	
	template<typename _type>
	DataNode::DataNode(const WString& name, const Ptr<_type>& value):
		mName(name), mParent(nullptr)
	{
		*AddNode("type") = (String)(typeid(*value).name());
		*AddNode("value") = *value;
	}

	template<typename _type>
	DataNode::operator Ptr<_type>() const
	{
		String type;
		IObject* value = nullptr;

		if (auto typeNode = GetNode("type"))
			type = *typeNode;

		if (auto valueNode = GetNode("value"))
		{
			value = SerializableTypesSamples::CreateSample(type);
			*value = *valueNode;
		}

		return Ptr<_type>(value);
	}

	template<typename _type>
	DataNode& DataNode::operator=(const Ptr<_type>& value)
	{
		*AddNode("type") = (String)(typeid(*value).name());
		*AddNode("value") = *value;

		return *this;
	}


	template<typename _key, typename _value>
	DataNode::DataNode(const WString& name, const Dictionary<_key, _value>& value) :
		mName(name), mParent(nullptr)
	{
		for (auto kv : value)
		{
			Ptr<DataNode> child = AddNode("Element");
			*child->AddNode("Key") = kv.Key();
			*child->AddNode("Value") = kv.Value();
		}
	}

	template<typename _type>
	DataNode::DataNode(const WString& name, const Array<_type>& value)
	{
		for (auto v : value)
			*AddNode("Element") = v;
	}

	template<typename _key, typename _value>
	DataNode& DataNode::operator=(const Dictionary<_key, _value>& value)
	{
		Clear();

		for (auto kv : value)
		{
			Ptr<DataNode> child = AddNode("Element");
			*child->AddNode("Key") = kv.Key();
			*child->AddNode("Value") = kv.Value();
		}

		return *this;
	}

	template<typename _type>
	DataNode& DataNode::operator=(const Array<_type>& value)
	{
		Clear();

		for (auto v : value)
			*AddNode("Element") = v;

		return *this;
	}

	template<typename _key, typename _value>
	DataNode::operator Dictionary<_key, _value>() const
	{
		Dictionary<_key, _value> res;

		int count = mChildNodes.Count();
		for (auto childNode : mChildNodes)
		{
			auto keyNode = childNode->GetNode("Key");
			auto valueNode = childNode->GetNode("Value");

			if (keyNode && valueNode)
				res.Add((_key)*keyNode, (_value)*valueNode);
		}

		return res;
	}

	template<typename _type>
	DataNode::operator Array<_type>() const
	{
		int count = mChildNodes.Count();

		Array<_type> res(count + 2);
		_type v = _type();
		for (auto childNode : mChildNodes)
		{
			v = *childNode;
			res.Add(v);
		}

		return res;
	}
}