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
		virtual Type& GetType() const = 0;

	private:
		static Type type;

		template<typename _type>
		friend const Type& o2::_TypeOf();
	};

	// IObject header definition
#define IOBJECT(CLASS)  							   \
private:                                               \
	static Type type;								   \
                                                       \
	friend struct o2::Type::TypeCreator<CLASS>;        \
                                                       \
    template<typename _type>                           \
	friend const Type& o2::_TypeOf();                  \
    friend class o2::TypeInitializer;                  \
    friend class o2::Reflection;                       \
    template<typename _type>                           \
    friend struct o2::Type::TypeCreator;               \
    friend class o2::DataNode;                         \
                                                       \
public:                                                \
	CLASS* Clone() const { return mnew CLASS(*this); } \
	Type& GetType() const { return type; };	           \
	static void InitializeType(CLASS* sample)   
}
