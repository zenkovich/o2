#pragma once

#include "Utils/CommonTypes.h"

namespace o2
{
	// ---------------------------------------------------------
	// Unsigned integer type identificator, uses for custom RTTI
	// ---------------------------------------------------------
	typedef UInt TypeId;

	// -------------------------------------------
	// Last given type id container, uses for RTTI
	// -------------------------------------------
	struct TypeIdContainer
	{
		static TypeId lastId;
	};

// Definition type macros, using in classes body, giving some useful functional. !!! Must be in public section!
#define DEFINE_TYPE(TYPENAME) public:                                \
	static TypeId _ ## TYPENAME ## Type;                             \
	static TypeId GetStaticType() { return _ ## TYPENAME ## Type; }  \
	virtual TypeId GetType() const { return _ ## TYPENAME ## Type; } \
	virtual const char* GetTypeName() { return #TYPENAME; }

// Definition of template type macros, using in template classes body, giving some useful functional
#define DEFINE_TEMPLATE_TYPE(TYPENAME, T) DEFINE_TYPE(TYPENAME ## T)

// Registering type macros, using in .cpp sources
#define REGIST_TYPE(TYPENAME) TypeId TYPENAME::_ ## TYPENAME ## Type = TypeIdContainer::lastId++;
}