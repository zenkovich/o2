#include "o2/stdafx.h"
#include "ScriptValue.h"

namespace o2
{
	String ScriptValue::Dump(const String& tab /*= ""*/) const
	{
		String res;

		auto valType = GetValueType();
		if (valType == ValueType::Array)
		{
			res += "[ " + (String)GetLength() + "\n";

			for (int i = 0; i < GetLength(); i++)
				res += (*this)[i].Dump(tab + "  ") + ",\n";

			res += tab + "]";
		}
		else if (valType == ValueType::Bool)
		{
			res += (GetValue<bool>() ? String("true") : String("false"));
		}
		else if (valType == ValueType::Error)
		{
			res += "Error: " + GetError();
		}
		else if (valType == ValueType::Function)
		{
#if SCRIPTING_BACKEND_JERRYSCRIPT
			void* ptr = nullptr;
			jerry_get_object_native_pointer(jvalue, &ptr, &GetDataDeleter().info);
			res += ptr ? "c-function" : "function";
#else
			res += "function";
#endif
		}
		else if (valType == ValueType::None)
		{
			res += "none";
		}
		else if (valType == ValueType::Null)
		{
			res += "null";
		}
		else if (valType == ValueType::Number)
		{
			res += (String)GetValue<float>();
		}
		else if (valType == ValueType::String)
		{
			res += GetValue<String>();
		}
		else if (valType == ValueType::Undefined)
		{
			res += "Undefined";
		}
		else if (valType == ValueType::Object)
		{
			res += "{\n";

			ForEachProperties([&](const ScriptValue& name, const ScriptValue& value) {
				res += tab + "  " + name.Dump(tab + "  ") + " : " + value.Dump(tab + "  ") + ",\n";
				return true;
			});

			res += tab + "}";
		}

		return res;
	}

	ScriptValue ScriptValue::GetProperty(const char* name) const
	{
		return GetProperty(ScriptValue(name));
	}

	void ScriptValue::SetProperty(const char* name, const ScriptValue& value)
	{
		SetProperty(ScriptValue(name), value);
	}

	bool ScriptValue::IsObject() const
	{
		return GetValueType() == ValueType::Object;
	}

	bool ScriptValue::IsArray() const
	{
		return GetValueType() == ValueType::Array;
	}

	bool ScriptValue::IsFunction() const
	{
		return GetValueType() == ValueType::Function;
	}

	bool ScriptValueProperty::operator==(const ScriptValueProperty& other) const
	{
		return object == other.object && name == other.name;
	}

	ScriptValue ScriptValueProperty::Get() const
	{
		return object.GetProperty(name);
	}

	void ScriptValueProperty::Set(const ScriptValue& value)
	{
		object.SetProperty(name, value);
	}
}

ENUM_META(o2::ScriptValue::ValueType)
{
	ENUM_ENTRY(Array);
	ENUM_ENTRY(BigInt);
	ENUM_ENTRY(Bool);
	ENUM_ENTRY(Constructor);
	ENUM_ENTRY(Error);
	ENUM_ENTRY(Function);
	ENUM_ENTRY(None);
	ENUM_ENTRY(Null);
	ENUM_ENTRY(Number);
	ENUM_ENTRY(Object);
	ENUM_ENTRY(String);
	ENUM_ENTRY(Symbol);
	ENUM_ENTRY(Undefined);
}
END_ENUM_META;
