#pragma once

#include "Utils/Containers/Array.h"
#include "Utils/String.h"

namespace o2
{
	class DataNode;

	class DataDoc
	{
		friend class DataNode;

	public:
		enum class Format { Xml, JSON, Binary };

	protected:
		struct INodeProvider
		{
			virtual char*   AsCharPtr() const = 0;
			virtual TString AsString() const = 0;
			virtual int     AsInt() const = 0;
			virtual float   AsFloat() const = 0;
			virtual UInt    AsUInt() const = 0;
			virtual Vec2F   AsVec2F() const = 0;
			virtual Vec2I   AsVec2I() const = 0;
			virtual RectF   AsRectF() const = 0;
			virtual RectI   AsRectI() const = 0;
			virtual Color4  AsColor4() const = 0;

			virtual void SetValue(const char* value) = 0;
			virtual void SetValue(int value) = 0;
			virtual void SetValue(float value) = 0;
			virtual void SetValue(UInt value) = 0;
			virtual void SetValue(const TString& value) = 0;
			virtual void SetValue(const Vec2F& value) = 0;
			virtual void SetValue(const Vec2I& value) = 0;
			virtual void SetValue(const RectF& value) = 0;
			virtual void SetValue(const RectI& value) = 0;
			virtual void SetValue(const Color4& value) = 0;

			virtual INodeProvider* GetNode(const TString& nodePath) = 0;
			virtual INodeProvider* AddNode(const TString& name) = 0;
			virtual bool RemoveNode(const TString& name) = 0;

			virtual bool Equals(INodeProvider* other) const = 0;
		};

		struct IDocProvider
		{
			virtual bool Load(const TString& data) = 0;
			virtual TString Save() const = 0;
			virtual INodeProvider* GetNode(const TString& nodePath) = 0;
			virtual INodeProvider* AddNode(const TString& name) = 0;
			virtual bool RemoveNode(const TString& name) = 0;
			virtual IDocProvider* Clone() const = 0;
		};

		IDocProvider* mDocProvider;

	public:
		DataDoc(Format format = Format::Xml);
		DataDoc(const TString& fileName, Format format = Format::Xml);
		DataDoc(const DataDoc& other);
		~DataDoc();

		DataDoc& operator=(const DataDoc& other);

		DataNode& operator[](const TString& nodePath) const;

		bool LoadFromFile(const TString& fileName);
		bool LoadFromData(const TString& data);

		bool SaveToFile(const TString& fileName) const;
		TString SaveAsString() const;

		DataNode& GetNode(const TString& nodePath) const;
		DataNode& AddNode(const TString& name);
		bool RemoveNode(const DataNode& node);
		bool RemoveNode(const TString& name);
	};

	class DataNode
	{
		DataDoc::INodeProvider* mNodeProvider;

	public:
		DataNode();
		DataNode(const TString& name);
		DataNode(const TString& name, char* value);
		DataNode(const TString& name, int value);
		DataNode(const TString& name, float value);
		DataNode(const TString& name, UInt value);
		DataNode(const TString& name, const TString& value);
		DataNode(const TString& name, const Vec2F& value);
		DataNode(const TString& name, const Vec2I& value);
		DataNode(const TString& name, const RectF& value);
		DataNode(const TString& name, const RectI& value);
		DataNode(const TString& name, const Color4& value);
		~DataNode();

		DataNode& operator=(char* value);
		DataNode& operator=(int value);
		DataNode& operator=(float value);
		DataNode& operator=(UInt value);
		DataNode& operator=(const TString& value);
		DataNode& operator=(const Vec2F& value);
		DataNode& operator=(const Vec2I& value);
		DataNode& operator=(const RectF& value);
		DataNode& operator=(const RectI& value);
		DataNode& operator=(const Color4& value);

		explicit operator char*() const;
		explicit operator TString() const;
		explicit operator int() const;
		explicit operator float() const;
		explicit operator UInt() const;
		explicit operator Vec2F() const;
		explicit operator Vec2I() const;
		explicit operator RectF() const;
		explicit operator RectI() const;
		explicit operator Color4() const;

		DataNode& operator[](const TString& nodePath) const;

		DataNode& operator=(const DataNode& other);
		bool operator==(const DataNode& other) const;
		bool operator!=(const DataNode& other) const;

		DataNode& GetParent() const;

		DataNode& GetNode(const TString& nodePath) const;
		DataNode& AddNode(const TString& name);
		bool RemoveNode(const DataNode& node);
		bool RemoveNode(const TString& name);

		TString GetName() const;
		void SetName(const TString& name);
	};
}