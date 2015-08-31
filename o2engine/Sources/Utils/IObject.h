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

	// IObject header definition
#define IOBJECT(CLASS)  							     \
	IObject* Clone() const { return mnew CLASS(*this); } \
	static Type type;								     \
	const Type& GetType() const { return type; };	     \
	Type::Id GetTypeId() const { return type.ID(); };    \
	static void InitializeType(CLASS* sample)   

	// IObject source file implementation
#define IOBJECT_CPP(CLASS)										\
	Type CLASS::type;											\
	Types::Registrator<CLASS> CLASS ## reg(CLASS::type, #CLASS)	

	// IObject source file implementation for inherited types
#define IOBJECT_INH_CPP(CLASS, BASE_CLASS)						\
	Type BASE_CLASS::CLASS::type;								\
	Types::Registrator<BASE_CLASS::CLASS> BASE_CLASS ## CLASS ## reg(BASE_CLASS::CLASS::type, #BASE_CLASS ## #CLASS)	

	// FIeld registration in type macros
#define FIELD(NAME) \
type.RegField(#NAME, (char*)(&sample->NAME) - (char*)sample)

	// Base class specialization macros
#define BASE_CLASS(CLASS) \
Type::SetupBaseType(type, &CLASS::type)

}
