#include "o2/stdafx.h"
#include "JsonDataFormat.h"

#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

namespace o2
{
	bool ParseJsonInplace(char* str, DataDocument& document)
	{
		JsonDataDocumentParseHandler handler(document);
		rapidjson::Reader reader;
		rapidjson::InsituStringStream stream(str);
		auto result = reader.Parse<rapidjson::kParseInsituFlag | rapidjson::kParseStopWhenDoneFlag>(stream, handler);
		if (!result.IsError())
		{
			(DataValue&)document = std::move(*handler.stack.Pop<DataValue>());
			return true;
		}

		return false;
	}

	bool ParseJson(const char* str, DataDocument& document)
	{
		JsonDataDocumentParseHandler handler(document);
		rapidjson::Reader reader;
		rapidjson::StringStream stream(str);
		auto result = reader.Parse(stream, handler);
		if (!result.IsError())
		{
			(DataValue&)document = std::move(*handler.stack.Pop<DataValue>());
			return true;
		}

		return false;
	}

	void WriteJson(String& str, const DataDocument& document)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter writer(buffer);
		document.Write(writer);
		str = buffer.GetString();
	}

	JsonDataDocumentParseHandler::JsonDataDocumentParseHandler(DataDocument& document):
		document(document), stack(sizeof(DataValue)*16)
	{}

	bool JsonDataDocumentParseHandler::Null()
	{
		new (stack.template Push<DataValue>()) DataValue(document);
		return true;
	}

	bool JsonDataDocumentParseHandler::Bool(bool value)
	{
		new (stack.template Push<DataValue>()) DataValue(value, document);
		return true;
	}

	bool JsonDataDocumentParseHandler::Int(int value)
	{
		new (stack.template Push<DataValue>()) DataValue(value, document);
		return true;
	}

	bool JsonDataDocumentParseHandler::Uint(unsigned value)
	{
		new (stack.template Push<DataValue>()) DataValue(value, document);
		return true;
	}

	bool JsonDataDocumentParseHandler::Int64(int64_t value)
	{
		new (stack.template Push<DataValue>()) DataValue(value, document);
		return true;
	}

	bool JsonDataDocumentParseHandler::Uint64(uint64_t value)
	{
		new (stack.template Push<DataValue>()) DataValue(value, document);
		return true;
	}

	bool JsonDataDocumentParseHandler::Double(double value)
	{
		new (stack.template Push<DataValue>()) DataValue(value, document);
		return true;
	}

	bool JsonDataDocumentParseHandler::String(const char* str, unsigned length, bool copy)
	{
		new (stack.template Push<DataValue>()) DataValue(str, length, copy, document);
		return true;
	}

	bool JsonDataDocumentParseHandler::RawNumber(const char* str, unsigned length, bool copy)
	{
		return String(str, length, copy);
	}

	bool JsonDataDocumentParseHandler::StartObject()
	{
		new (stack.template Push<DataValue>()) DataValue(document);
		return true;
	}

	bool JsonDataDocumentParseHandler::Key(const char* str, unsigned length, bool copy)
	{
		String(str, length, copy);
		return true;
	}

	bool JsonDataDocumentParseHandler::EndObject(unsigned memberCount)
	{
		DataMember* members = stack.template Pop<DataMember>(memberCount);
		DataValue* top = stack.template Top<DataValue>();

		top->mData.flagsData.flags = DataValue::Flags::Object;
		if (memberCount != 0)
		{
			size_t size = sizeof(DataMember)*memberCount;
			top->mData.objectData.members = (DataMember*)document.mAllocator.Allocate(size);
			memcpy(top->mData.objectData.members, members, size);
		}
		else
			top->mData.objectData.members = nullptr;

		top->mData.objectData.count = memberCount;
		top->mData.objectData.capacity = memberCount;

		return true;
	}

	bool JsonDataDocumentParseHandler::StartArray()
	{
		new (stack.template Push<DataValue>()) DataValue(document);
		return true;
	}

	bool JsonDataDocumentParseHandler::EndArray(unsigned elementCount)
	{
		DataValue* elements = stack.template Pop<DataValue>(elementCount);
		DataValue* top = stack.template Top<DataValue>();

		top->mData.flagsData.flags = DataValue::Flags::Array;
		if (elementCount != 0)
		{
			size_t size = sizeof(DataValue)*elementCount;
			top->mData.arrayData.elements = (DataValue*)document.mAllocator.Allocate(size);
			memcpy(top->mData.arrayData.elements, elements, size);
		}
		else
			top->mData.arrayData.elements = nullptr;

		top->mData.arrayData.count = elementCount;
		top->mData.arrayData.capacity = elementCount;

		return true;
	}

}