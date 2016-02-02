#pragma once

#include "Utils/Memory/MemoryManager.h"
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

		static Type type;
	};

	// IObject header definition
#define IOBJECT(CLASS)  							   \
	CLASS* Clone() const { return mnew CLASS(*this); } \
	static Type type;								   \
	const Type& GetType() const { return type; };	   \
	friend struct o2::Type::TypeCreator<CLASS>;        \
	static void InitializeType(CLASS* sample)   

}
