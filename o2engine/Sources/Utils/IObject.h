#pragma once

#include "Utils/Reflection/Type.h"

namespace o2
{
	// ----------------------------------------------------
	// Basic object interface with type information support
	// ----------------------------------------------------
	class IObject
	{
	public:
		// Virtual destructor
		virtual ~IObject() {}

		// Cloning interface
		virtual IObject* Clone() const = 0;

		// Returns type
		virtual const Type& GetType() const = 0;
	};

	// IObject header definition
#define IOBJECT(CLASS)  							     \
	CLASS* Clone() const { return mnew CLASS(*this); }   \
	static Type* type;								     \
	const Type& GetType() const { return *type; };	     \
	static void InitializeType(CLASS* sample)   

	// Field registration in type macros
#define FIELD(NAME) \
	type->RegField(#NAME, (char*)(&sample->NAME) - (char*)sample, sample->NAME)

}
