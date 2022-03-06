#pragma once

#include "o2/Utils/Memory/MemoryManager.h"

#if IS_SCRIPTING_SUPPORTED
#include "o2/Scripts/ScriptValue.h"
#endif

namespace o2
{
	class Type;

	template<typename _type>
	const Type& GetTypeOf();

	class ReflectionInitializationTypeProcessor;
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
		virtual IObject* Clone() const { return nullptr; };

		// Cloning as type
		template<typename _cast_type>
		_cast_type* CloneAs() const { return dynamic_cast<_cast_type*>(Clone()); }

		// Returns type
		virtual const Type& GetType() const { return *type; }

#if IS_SCRIPTING_SUPPORTED
		// Returns script value with wrapped object
		virtual ScriptValue GetScriptValue() const { return ScriptValue(*this); }
#endif

	private:
		static Type* type;

		template<typename _type>
		friend const o2::Type& o2::GetTypeOf();

		friend class ReflectionInitializationTypeProcessor;
		friend class Reflection;
	};

	template<typename _type>
	struct SafeClone
	{
		static _type* Clone(const _type& origin)
		{
			if constexpr (std::is_copy_constructible<_type>::value)
				return mnew _type(origin);
			else
				return nullptr;
		}
	};
}

// -------------------------------
// Types meta information macroses
// -------------------------------
 
#if IS_SCRIPTING_SUPPORTED
#define IOBJECT_SCRIPTING() ScriptValue GetScriptValue() const override
#else
#define IOBJECT_SCRIPTING()
#endif

#define IOBJECT_MAIN(CLASS)  							                                                        \
private:                                                                                                        \
	static o2::Type* type;							                                                            \
                                                                                                                \
    template<typename __type>                                                                                   \
	friend const o2::Type& o2::GetTypeOf();                                                                     \
                                                                                                                \
	template<typename __type>                                                                                   \
	friend class o2::TObjectType;                                                                               \
                                                                                                                \
    template<typename __type>                                                                                   \
	friend class o2::PointerValueProxy;                                                                         \
																												\
    template<typename __type>																					\
	friend class o2::IValueProxy;																			    \
                                                                                                                \
    friend class o2::ReflectionInitializationTypeProcessor;                                                     \
    friend class o2::Reflection;                                                                                \
    friend class o2::DataValue;                                                                                 \
                                                                                                                \
public:                                                                                                         \
	typedef CLASS thisclass;                                                                                    \
	o2::IObject* Clone() const override {return o2::SafeClone<CLASS>::Clone(*this); }                           \
	const o2::Type& GetType() const override { return *type; };                                                 \
	IOBJECT_SCRIPTING();                                                                                        \
                                                                                                                \
    template<typename _type_processor> static void ProcessType(CLASS* object, _type_processor& processor)       \
	{                                                                                                           \
		processor.template Start<CLASS>(object, type);                                                          \
		ProcessBaseTypes<_type_processor>(object, processor);                                                   \
		ProcessFields<_type_processor>(object, processor);                                                      \
		ProcessMethods<_type_processor>(object, processor);                                                     \
	}                                                                                                           

#define IOBJECT(CLASS)																							\
    IOBJECT_MAIN(CLASS)																							\
		                                                                                                        \
    template<typename _type_processor> static void ProcessBaseTypes(CLASS* object, _type_processor& processor); \
    template<typename _type_processor> static void ProcessFields(CLASS* object, _type_processor& processor);    \
    template<typename _type_processor> static void ProcessMethods(CLASS* object, _type_processor& processor)
