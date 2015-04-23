#include "DataDoc.h"

#include "Utils/FileSystem/File.h"

namespace o2
{

	DataDoc::DataDoc(Format format /*= Format::Xml*/)
	{
		//create  Doc Provider by type
	}

	DataDoc::DataDoc(const TString& fileName, Format format /*= Format::Xml*/):
		DataDoc(format)
	{
		LoadFromFile(fileName);
	}

	DataDoc::DataDoc(const DataDoc& other) :
		mDocProvider(other.mDocProvider->Clone())
	{
	}

	DataDoc::~DataDoc()
	{
		delete mDocProvider;
	}

	DataDoc& DataDoc::operator=(const DataDoc& other)
	{
		delete mDocProvider;
		mDocProvider = other.mDocProvider->Clone();
		return *this;
	}

	DataNode& DataDoc::operator[](const TString& nodePath) const
	{
		return GetNode(nodePath);
	}

	bool DataDoc::LoadFromFile(const TString& fileName)
	{
		InFile file(fileName);

		if (!file.IsOpened())
			return false;

		UInt dataSize = file.GetDataSize();
		TString data;
		data.Reserve(dataSize + 1);
		file.ReadFullData(data.Data());

		return mDocProvider->Load(data);
	}

	bool DataDoc::LoadFromData(const TString& data)
	{
		return mDocProvider->Load(data);
	}

	bool DataDoc::SaveToFile(const TString& fileName) const
	{
		OutFile file(fileName);
		TString data = mDocProvider->Save();
		file.WriteData(data.Data(), data.Length()*sizeof(wchar_t));
		return true;
	}

	TString DataDoc::SaveAsString() const
	{
		return mDocProvider->Save();
	}

	DataNode& DataDoc::GetNode(const TString& nodePath) const
	{

	}

	DataNode& DataDoc::AddNode(const TString& name)
	{

	}

	bool DataDoc::RemoveNode(const DataNode& node)
	{

	}

	bool DataDoc::RemoveNode(const TString& name)
	{

	}


	DataNode::DataNode()
	{

	}

	DataNode::DataNode(const TString& name)
	{

	}

	DataNode::DataNode(const TString& name, char* value)
	{

	}

	DataNode::DataNode(const TString& name, int value)
	{

	}

	DataNode::DataNode(const TString& name, float value)
	{

	}

	DataNode::DataNode(const TString& name, UInt value)
	{

	}

	DataNode::DataNode(const TString& name, const TString& value)
	{

	}

	DataNode::DataNode(const TString& name, const Vec2F& value)
	{

	}

	DataNode::DataNode(const TString& name, const Vec2I& value)
	{

	}

	DataNode::DataNode(const TString& name, const RectF& value)
	{

	}

	DataNode::DataNode(const TString& name, const RectI& value)
	{

	}

	DataNode::DataNode(const TString& name, const Color4& value)
	{

	}

	DataNode::~DataNode()
	{

	}

	DataNode& DataNode::operator=(char* value)
	{

	}

	DataNode& DataNode::operator=(int value)
	{

	}

	DataNode& DataNode::operator=(float value)
	{

	}

	DataNode& DataNode::operator=(UInt value)
	{

	}

	DataNode& DataNode::operator=(const TString& value)
	{

	}

	DataNode& DataNode::operator=(const Vec2F& value)
	{

	}

	DataNode& DataNode::operator=(const Vec2I& value)
	{

	}

	DataNode& DataNode::operator=(const RectF& value)
	{

	}

	DataNode& DataNode::operator=(const RectI& value)
	{

	}

	DataNode& DataNode::operator=(const Color4& value)
	{

	}

	DataNode& DataNode::operator=(const DataNode& other)
	{

	}

	DataNode::operator char*() const
	{

	}

	DataNode::operator TString() const
	{

	}

	DataNode::operator int() const
	{

	}

	DataNode::operator float() const
	{

	}

	DataNode::operator UInt() const
	{

	}

	DataNode::operator Vec2F() const
	{

	}

	DataNode::operator Vec2I() const
	{

	}

	DataNode::operator RectF() const
	{

	}

	DataNode::operator RectI() const
	{

	}

	DataNode::operator Color4() const
	{

	}

	DataNode& DataNode::operator[](const TString& nodePath) const
	{

	}

	bool DataNode::operator==(const DataNode& other) const
	{

	}

	bool DataNode::operator!=(const DataNode& other) const
	{

	}

	DataNode& DataNode::GetParent() const
	{

	}

	DataNode& DataNode::GetNode(const TString& nodePath) const
	{

	}

	DataNode& DataNode::AddNode(const TString& name)
	{

	}

	bool DataNode::RemoveNode(const DataNode& node)
	{

	}

	bool DataNode::RemoveNode(const TString& name)
	{

	}

	TString DataNode::GetName() const
	{

	}

	void DataNode::SetName(const TString& name)
	{

	}

}