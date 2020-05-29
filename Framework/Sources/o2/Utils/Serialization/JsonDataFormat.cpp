#include "o2/stdafx.h"
#include "JsonDataFormat.h"

#include "rapidjson/reader.h"

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
			(DataValue&)document = *handler.stack.Pop<DataValue>();
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

		top->mValue.flagsData.flags = DataValue::Flags::Object;
		if (memberCount != 0)
		{
			size_t size = sizeof(DataMember)*memberCount;
			top->mValue.objectData.members = (DataMember*)document.mAllocator.Allocate(size);
			memcpy(top->mValue.objectData.members, members, size);
		}
		else
			top->mValue.objectData.members = nullptr;

		top->mValue.objectData.count = memberCount;

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

		top->mValue.flagsData.flags = DataValue::Flags::Array;
		if (elementCount != 0)
		{
			size_t size = sizeof(DataValue)*elementCount;
			top->mValue.arrayData.elements = (DataValue*)document.mAllocator.Allocate(size);
			memcpy(top->mValue.arrayData.elements, elements, size);
		}
		else
			top->mValue.arrayData.elements = nullptr;

		top->mValue.arrayData.count = elementCount;

		return true;
	}

}