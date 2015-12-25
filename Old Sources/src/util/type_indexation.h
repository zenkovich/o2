#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

#define UniqueType unsigned int

struct TypeIdxContainer
{
	static UniqueType index;
};

#define DEFINE_TYPE(TYPENAME) public:                               \
	static UniqueType TYPENAME ## Type;                             \
	static UniqueType GetStaticType() { return TYPENAME ## Type; }  \
	virtual UniqueType GetType() const { return TYPENAME ## Type; } \
	virtual const char* GetTypeName() { return #TYPENAME; }

#define DEFINE_TEMPLATE_TYPE(TYPENAME, T) DEFINE_TYPE(TYPENAME ## T)

#define REGIST_TYPE(TYPENAME) UniqueType TYPENAME::TYPENAME ## Type = TypeIdxContainer::index++;

CLOSE_O2_NAMESPACE