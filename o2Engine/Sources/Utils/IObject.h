#pragma once

#include "Utils/Memory/MemoryManager.h"
#include "Utils/Reflection/Type.h"

namespace o2
{
	class IClassMetaProcessor;

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
		virtual Type& GetType() const = 0;

	private:
		static Type type;

		template<typename _type>
		friend const Type& o2::GetTypeOf();
	};

#define metaclass class

	// IObject header definition
#define IOBJECT(CLASS)  							   \
private:                                               \
	static o2::Type* type;							   \
                                                       \
    template<typename _type>                           \
	friend const o2::Type& o2::GetTypeOf();            \
                                                       \
    template<typename _type>                           \
    friend struct o2::Type::TypeCreator;               \
                                                       \
    friend class o2::TypeInitializer;                  \
    friend class o2::Reflection;                       \
    friend class o2::DataNode;                         \
                                                       \
public:                                                \
	CLASS* Clone() const { return mnew CLASS(*this); } \
	o2::Type& GetType() const { return *type; };       \
                                                       \
private:                                               \
	static void InitializeType(o2::Type* type)                       
}
