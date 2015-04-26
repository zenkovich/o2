#pragma once

#include "Utils/Containers/Array.h"
#include "Utils/String.h"

namespace o2
{
	class DataNode
	{
		friend class DataDoc;

	public:
		typedef Array<DataNode*> DataNodesArr;

	protected:
		WString      mName;
		WString      mData;
		DataNode*    mParent;
		DataNodesArr mChildNodes;

	public:
		DataNode();
		DataNode(const WString& name);
		DataNode(const WString& name, char* value);
		DataNode(const WString& name, int value);
		DataNode(const WString& name, float value);
		DataNode(const WString& name, UInt value);
		DataNode(const WString& name, const WString& value);
		DataNode(const WString& name, const Vec2F& value);
		DataNode(const WString& name, const Vec2I& value);
		DataNode(const WString& name, const RectF& value);
		DataNode(const WString& name, const RectI& value);
		DataNode(const WString& name, const Color4& value);
		~DataNode();

		DataNode& operator=(char* value);
		DataNode& operator=(int value);
		DataNode& operator=(float value);
		DataNode& operator=(UInt value);
		DataNode& operator=(const WString& value);
		DataNode& operator=(const Vec2F& value);
		DataNode& operator=(const Vec2I& value);
		DataNode& operator=(const RectF& value);
		DataNode& operator=(const RectI& value);
		DataNode& operator=(const Color4& value);

		explicit operator char*() const;
		explicit operator WString() const;
		explicit operator int() const;
		explicit operator float() const;
		explicit operator UInt() const;
		explicit operator Vec2F() const;
		explicit operator Vec2I() const;
		explicit operator RectF() const;
		explicit operator RectI() const;
		explicit operator Color4() const;

		DataNode operator[](const WString& nodePath) const;

		DataNode& operator=(const DataNode& other);
		bool operator==(const DataNode& other) const;
		bool operator!=(const DataNode& other) const;

		DataNode* GetParent() const;

		DataNode* GetNode(const WString& nodePath) const;
		DataNode* AddNode(const WString& name);
		bool RemoveNode(const DataNode* node);
		bool RemoveNode(const WString& name);

		WString GetName() const;
		void SetName(const WString& name);
	};

	class DataDoc
	{
		friend class DataNode;

	public:
		enum class Format { Xml, JSON, Binary };

	protected:
		DataNode::DataNodesArr mChildNodes;

	public:
		DataDoc(Format format = Format::Xml);
		DataDoc(const WString& fileName, Format format = Format::Xml);
		DataDoc(const DataDoc& other);
		~DataDoc();

		DataDoc& operator=(const DataDoc& other);

		DataNode operator[](const WString& nodePath) const;

		bool LoadFromFile(const WString& fileName);
		bool LoadFromData(const WString& data);

		bool SaveToFile(const WString& fileName) const;
		WString SaveAsWString() const;

		DataNode GetNode(const WString& nodePath) const;
		DataNode AddNode(const WString& name);
		bool RemoveNode(const DataNode& node);
		bool RemoveNode(const WString& name);
	};
}