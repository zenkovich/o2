#pragma once

#include "Utils/CommonTypes.h"

namespace o2
{
	typedef UInt TypeId;

	struct TypeIdContainer
	{
		static TypeId lastId;
	};

#define DEFINE_TYPE(TYPENAME) public:                           \
	static TypeId TYPENAME ## Type;                             \
	static TypeId GetStaticType() { return TYPENAME ## Type; }  \
	virtual TypeId GetType() const { return TYPENAME ## Type; } \
	virtual const char* GetTypeName() { return #TYPENAME; }

#define DEFINE_TEMPLATE_TYPE(TYPENAME, T) DEFINE_TYPE(TYPENAME ## T)

#define REGIST_TYPE(TYPENAME) TypeId TYPENAME::TYPENAME ## Type = TypeIdContainer::lastId++;
}