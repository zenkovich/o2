#pragma once

#include "Utils/Memory/MemoryManager.h"

namespace o2
{
	class Type;

	template<typename _type, typename _getter>
	const Type& GetTypeOf();

	template<typename T>
	struct RegularTypeGetter;

	template<typename T, typename X>
	struct GetTypeHelper;

	template<typename _type>
	struct TypeSampleCreator;

	class TypeInitializer;
	class Reflection;

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

		// Cloning as type
		template<typename Type>
		Type* CloneAs() const { return dynamic_cast<Type*>(Clone()); }

		// Returns type
		virtual const Type& GetType() const = 0;

	private:
		static Type* type;

		template<typename _type, typename _getter>
		friend const Type& o2::GetTypeOf();

		template<typename T>
		friend struct RegularTypeGetter;

		template<typename T, typename X>
		friend struct o2::GetTypeHelper;

		friend class TypeInitializer;
		friend class Reflection;
	};
}

// -------------------------------
// Types meta information macroses
// -------------------------------

#define IOBJECT(CLASS)  							                                                            \
private:                                                                                                        \
	static o2::Type* type;							                                                            \
                                                                                                                \
    template<typename _type, typename _getter>                                                                  \
	friend const o2::Type& o2::GetTypeOf();                                                                     \
                                                                                                                \
	template<typename T>                                                                                        \
	friend struct o2::RegularTypeGetter;                                                                        \
                                                                                                                \
	template<typename T, typename X>                                                                            \
	friend struct o2::GetTypeHelper;                                                                            \
                                                                                                                \
    template<typename _type>                                                                                    \
    friend struct o2::TypeSampleCreator;                                                                        \
                                                                                                                \
    friend class o2::TypeInitializer;                                                                           \
    friend class o2::Reflection;                                                                                \
    friend class o2::DataNode;                                                                                  \
                                                                                                                \
public:                                                                                                         \
	typedef CLASS thisclass;                                                                                    \
	IObject* Clone() const { return mnew CLASS(*this); }                                                        \
	const o2::Type& GetType() const { return *type; };                                                          \
                                                                                                                \
    template<typename _type_processor> void ProcessType(_type_processor& processor)                             \
    {                                                                                                           \
        ProcessType<_type_processor>(this, processor);                                                          \
    }                                                                                                           \
                                                                                                                \
    template<typename _type_processor> void ProcessBaseTypes(_type_processor& processor)                        \
    {                                                                                                           \
        ProcessBaseTypes<_type_processor>(this, processor);                                                     \
    }                                                                                                           \
                                                                                                                \
    template<typename _type_processor> void ProcessFields(_type_processor& processor)                           \
    {                                                                                                           \
        ProcessFields<_type_processor>(this, processor);                                                        \
    }                                                                                                           \
                                                                                                                \
    template<typename _type_processor> void ProcessMethods(_type_processor& processor)                          \
    {                                                                                                           \
        ProcessMethods<_type_processor>(this, processor);                                                       \
    }                                                                                                           \
                                                                                                                \
private:                                                                                                        \
    template<typename _type_processor> static void ProcessType(CLASS* object, _type_processor& processor)       \
	{                                                                                                           \
		processor.Start<CLASS>(object);                                                                         \
		ProcessBaseTypes<_type_processor>(object, processor);                                                   \
		ProcessFields<_type_processor>(object, processor);                                                      \
		ProcessMethods<_type_processor>(object, processor);                                                     \
	}                                                                                                           \
		                                                                                                        \
    template<typename _type_processor> static void ProcessBaseTypes(CLASS* object, _type_processor& processor); \
    template<typename _type_processor> static void ProcessFields(CLASS* object, _type_processor& processor);    \
    template<typename _type_processor> static void ProcessMethods(CLASS* object, _type_processor& processor)
