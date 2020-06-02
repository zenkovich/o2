#include "o2/stdafx.h"
#include "JsonDataFormat.h"

#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

namespace o2
{
	bool ParseJsonInplace(wchar_t* str, DataDocument& document)
	{
		JsonDataDocumentParseHandler handler(document);
		rapidjson::GenericReader<rapidjson::UTF16<>, rapidjson::UTF16<>> reader;
		rapidjson::GenericInsituStringStream<rapidjson::UTF16<>> stream(str);
		auto result = reader.Parse<rapidjson::kParseInsituFlag>(stream, handler);
		if (!result.IsError())
		{
			(DataValue&)document = std::move(*handler.stack.Pop<DataValue>());
			return true;
		}

		return false;
	}

	bool ParseJson(const wchar_t* str, DataDocument& document)
	{
		JsonDataDocumentParseHandler handler(document);
		rapidjson::GenericReader<rapidjson::UTF16<>, rapidjson::UTF16<>> reader;
		rapidjson::GenericStringStream<rapidjson::UTF16<>> stream(str);
		auto result = reader.Parse(stream, handler);
		if (!result.IsError())
		{
			(DataValue&)document = *handler.stack.Pop<DataValue>();
			return true;
		}

		return false;
	}

	void WriteJson(WString& str, const DataDocument& document)
	{
		rapidjson::GenericStringBuffer<rapidjson::UTF16<>> buffer;
		rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF16<>>, rapidjson::UTF16<>, rapidjson::UTF16<>> writer(buffer);
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

	bool JsonDataDocumentParseHandler::String(const wchar_t* str, unsigned length, bool copy)
	{
		new (stack.template Push<DataValue>()) DataValue(str, length, copy, document);
		return true;
	}

	bool JsonDataDocumentParseHandler::RawNumber(const wchar_t* str, unsigned length, bool copy)
	{
		return String(str, length, copy);
	}

	bool JsonDataDocumentParseHandler::StartObject()
	{
		new (stack.template Push<DataValue>()) DataValue(document);
		return true;
	}

	bool JsonDataDocumentParseHandler::Key(const wchar_t* str, unsigned length, bool copy)
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