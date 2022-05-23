#include "o2/stdafx.h"
#include "ScriptValueDef.h"

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
