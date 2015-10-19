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

		// Returns type id
		virtual Type::Id GetTypeId() const = 0;
	};

	class UnknownObject
	{
	public:
		static Type type;
	};

	// IObject header definition
#define IOBJECT(CLASS)  							     \
	CLASS* Clone() const { return mnew CLASS(*this); }   \
	static Type type;								     \
	const Type& GetType() const { return type; };	     \
	Type::Id GetTypeId() const { return type.ID(); };    \
	static void InitializeType(CLASS* sample)   

	// IObject source file implementation
#define IOBJECT_CPP(CLASS) Type CLASS::type;

	// Field registration in type macros
#define FIELD(NAME) \
	type.RegField(#NAME, (char*)(&sample->NAME) - (char*)sample, sample->NAME)

	// Field registration in type macros
#define PROPERTY(NAME) \
	type.RegField(#NAME, (char*)(&sample->NAME) - (char*)sample, sample->NAME)

	// Base class specialization macros
#define BASE_CLASS(CLASS) \
Type::SetupBaseType(type, &CLASS::type)

}
