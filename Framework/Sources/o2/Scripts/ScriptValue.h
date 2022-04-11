#pragma once

#if IS_SCRIPTING_SUPPORTED

#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "o2/Scripts/JerryScript/ScriptValueBase.h"
#endif
#include "o2/Utils/Types/String.h"

namespace o2
{
	// ------------------------------------------------------------------------------
	// Script value. Can contain script object, engine reflected object or Function<>
	// ------------------------------------------------------------------------------
	class ScriptValue: public ScriptValueBase
	{
	public:
		enum class ValueType { None = 0, Undefined, Null, Bool, Number, String, Object, Function, Constructor, Error, Symbol, BigInt, Array };

	public:
		template<typename _type, typename _enable = void>
		struct Converter
		{
			static constexpr bool isSupported = true;
			using __type = typename std::conditional<std::is_same<void, _type>::value, int, _type>::type;

			static void Write(const __type& value, ScriptValue& data);
			static void Read(__type& value, const ScriptValue& data);
		};

	public:
		// Default constructor. Creates undefined value
		ScriptValue();

		// Constructor from value
		template<typename _type>
		explicit ScriptValue(const _type& value);

		// Copy constructor. Copies reference on value
		ScriptValue(const ScriptValue& other);

		// Check equals operator
		bool operator==(const ScriptValue& other) const;

		// Check not equals operator
		bool operator!=(const ScriptValue& other) const;

		// Cast to type operator
		template<typename _type>
		operator _type() const;

		// Property accessor operator
		ScriptValue operator [](const ScriptValue& name) const;

		// Array index operator
		ScriptValue operator [](int idx) const;

		// Copy-operator. Copies reference on value
		ScriptValue& operator=(const ScriptValue& other);

		// Assign value operator
		template<typename _type>
		ScriptValue& operator=(const _type& value);

		// Returns type of value
		ValueType GetValueType() const;

		// Returns error description, if value type is error
		String GetError() const;

		// Dumps data into string
		String Dump(const String& tab = "") const;

	// Object methods
		// Creates empty object
		static ScriptValue EmptyObject();

		// Returns is object memory is handling GC. When it's true, containing object will be destroyed by GC. 
		bool IsObjectOwner() const;

		// Sets containing object ownership. When it's true, containing object will be destroyed by GC. 
		void SetObjectOwnership(bool own);

		// Constructs object from this
		ScriptValue Construct(const Vector<ScriptValue>& args);

		// Invokes function
		template<typename ... _args>
		ScriptValue Construct(_args ... args) const;

		// Iterates properties in object
		void ForEachProperties(const Function<bool(const ScriptValue& name, const ScriptValue& value)>& func) const;

		// Returns property value
		ScriptValue GetProperty(const ScriptValue& name) const;

		// Returns property value
		ScriptValue GetProperty(const char* name) const;

		// Sets property value
		void SetProperty(const ScriptValue& name, const ScriptValue& value);

		// Sets property value
		void SetProperty(const char* name, const ScriptValue& value);

		// Sets property value
		template<typename _type>
		void SetProperty(const char* name, const _type& value);

		// Sets property wrapper. Uses value reference to get/set value from script
		template<typename _type>
		void SetPropertyWrapper(const ScriptValue& name, _type& value);

		// Sets property wrapper. Uses setter and getter functions to get/set value from script
		template<typename _type>
		void SetPropertyWrapper(const ScriptValue& name, const Function<void(const _type& value)>& setter,
								const Function<_type()>& getter);

		// Removes property
		void RemoveProperty(const ScriptValue& name);

		// Sets prototype object
		void SetPrototype(const ScriptValue& proto);

	// Array methods
		// Returns length of array
		int GetLength() const;

		// Sets array index
		void SetElement(const ScriptValue& value, int idx);

		// Adds array element at end
		void AddElement(const ScriptValue& value);

		// Removes array element
		void RemoveElement(int idx);

	//Value methods
		// Boolean cast
		bool ToBool() const;

		// Number cast
		float ToNumber() const;

		// String cast
		String ToString() const;

		// Returns value. Trying to cast value, returns default if cant
		template<typename _type>
		_type GetValue() const;

		// Sets value
		template<typename _type>
		void SetValue(const _type& value);

	// Function methods
		// Invokes function
		ScriptValue InvokeRaw(const Vector<ScriptValue>& args) const;

		// Invokes member function
		ScriptValue InvokeRaw(const ScriptValue& thisValue, const Vector<ScriptValue>& args) const;

		// Invokes function
		template<typename _res_type, typename ... _args>
		_res_type Invoke(_args ... args) const;

		// Invokes member function
		template<typename _res_type, typename ... _args>
		_res_type Invoke(const ScriptValue& thisValue, _args ... args) const;

	private:
		template<typename ... _args>
		static void PackArgs(Vector<ScriptValue>& argsValues, _args ... args)
		{
			([&](auto& arg) { argsValues.Add(ScriptValue(arg)); } (args), ...);
		}
	};
}

#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "o2/Scripts/JerryScript/ScriptValueImpl.h"
#include "o2/Scripts/JerryScript/ScriptValueConverters.h"
#endif

#endif // IS_SCRIPTING_SUPPORTED

PRE_ENUM_META(o2::ScriptValue::ValueType);
