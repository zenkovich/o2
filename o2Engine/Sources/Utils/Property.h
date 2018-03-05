#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Delegates.h"

namespace o2
{
	template<typename T>
	class TString;

	typedef TString<wchar_t> WString;
	typedef TString<char> String;

	// -----------------------------
	// Get function overriding class
	// -----------------------------
	template<typename _type>
	class Getter
	{
		IFunction<_type()>* mGetter;
		_type               mTempValue;

	public:
		// Default constructor
		Getter():
			mGetter(nullptr)
		{
		}

		// Constructor from constant object function
		template<typename _class_type>
		Getter(_class_type* object, _type(_class_type::*getter)() const):
			Getter()
		{
			Initialize(object, getter);
		}

		// Constructor from object function
		template<typename _class_type>
		Getter(_class_type* object, _type(_class_type::*getter)()):
			Getter()
		{
			Initialize(object, getter);
		}

		// Constructor from static function pointer
		Getter(_type(*getter)()):
			Getter()
		{
			Initialize(object, getter);
		}

		// Constructor from function
		Getter(IFunction<_type()>* getter):
			Getter()
		{
			mGetter = getter;
		}

		// Destructor
		virtual ~Getter()
		{
			delete mGetter;
		}

		// Initialize from object function
		template<typename _class_type>
		void Initialize(_class_type* object, _type(_class_type::*getter)())
		{
			if (mGetter) delete mGetter;
			mGetter = mnew ObjFunctionPtr<_class_type, _type>(object, getter);
		}

		// Initialize from constant object function
		template<typename _class_type>
		void Initialize(_class_type* object, _type(_class_type::*getter)() const)
		{
			if (mGetter) delete mGetter;
			mGetter = mnew ObjConstFunctionPtr<_class_type, _type>(object, getter);
		}

		// Initialize by static getter function
		void Initialize(_type(*getter)())
		{
			if (mGetter) delete mGetter;
			mGetter = mnew FunctionPtr<_type()>(getter);
		}

		// Getting value operator
		operator _type()
		{
			return Get();
		}

		// Pointer access operator
		_type* operator->()
		{
			mTempValue = Get();
			return &mTempValue;
		}

		// Returns value from get function
		_type Get() const
		{
			return (*mGetter)();
		}
	};

	// -----------------------------
	// Set function overriding class
	// -----------------------------
	template<typename _type>
	class Setter
	{
		IFunction<void(const _type&)>* mSetter;

	public:
		// Default constructor
		Setter():
			mSetter(nullptr)
		{
		}

		// Constructor from object function with constant reference parameter
		template<typename _class_type>
		Setter(_class_type* object, void(_class_type::*setter)(const _type&)):
			Setter()
		{
			Initialize(object, setter);
		}

		// Constructor from object function with reference parameter
		template<typename _class_type>
		Setter(_class_type* object, void(_class_type::*setter)(_type&)):
			Setter()
		{
			Initialize(object, setter);
		}

		// Constructor from object function with regular parameter
		template<typename _class_type>
		Setter(_class_type* object, void(_class_type::*setter)(_type)):
			Setter()
		{
			Initialize(object, setter);
		}

		// Constructor from static function pointer with constant reference parameter
		Setter(void(*setter)(const _type&)):
			Setter()
		{
			Initialize(setter);
		}

		// Constructor from static function pointer with reference parameter
		Setter(void(*setter)(_type&)):
			Setter()
		{
			Initialize(setter);
		}

		// Constructor from static function pointer with regular parameter
		Setter(void(*setter)(_type)):
			Setter()
		{
			Initialize(setter);
		}

		// Constructor from function
		Setter(IFunction<void(const _type&)>* setter):
			mSetter(setter)
		{
		}

		// Destructor
		virtual ~Setter()
		{
			delete mSetter;
		}

		// Initialize from object function with constant reference parameter
		template<typename _class_type>
		void Initialize(_class_type* object, void(_class_type::*setter)(const _type&))
		{
			if (mSetter) delete mSetter;
			mSetter = mnew ObjFunctionPtr<_class_type, void, const _type&>(object, setter);
		}

		// Initialize from object function with reference parameter
		template<typename _class_type>
		void Initialize(_class_type* object, void(_class_type::*setter)(_type&))
		{
			if (mSetter) delete mSetter;
			auto lambda = [=](const _type& value) { (object->*setter)(const_cast<_type&>(value)); };
			mSetter = mnew SharedLambda<void(const _type&)>(lambda);
		}

		// Initialize from object function with regular parameter
		template<typename _class_type>
		void Initialize(_class_type* object, void(_class_type::*setter)(_type))
		{
			if (mSetter) delete mSetter;
			auto lambda = [=](const _type& value) { (object->*setter)(value); };
			mSetter = mnew SharedLambda<void(const _type&)>(lambda);
		}

		// Initialize by static function with constant reference parameter
		void Initialize(void(*setter)(const _type&))
		{
			if (mSetter) delete mSetter;
			mSetter = mnew FunctionPtr<void(const _type&)>(setter);
		}

		// Initialize by static function with reference parameter
		void Initialize(void(*setter)(_type&))
		{
			if (mSetter) delete mSetter;
			auto lambda = [=](const _type& value) { setter(const_cast<_type&>(value)); };
			mSetter = mnew SharedLambda<void(const _type&)>(lambda);
		}

		// Initialize by static function with regular parameter
		void Initialize(void(*setter)(_type))
		{
			if (mSetter) delete mSetter;
			auto lambda = [=](const _type& value) { setter(value); };
			mSetter = mnew SharedLambda<void(const _type&)>(lambda);
		}

		// Invokes set function
		void Set(const _type& value)
		{
			(*mSetter)(value);
		}

		// Assign operator
		Setter& operator=(const _type& value)
		{
			Set(value);
			return *this;
		}
	};

	// ----------------------------------------------------------------------------------
	// C#-like property. Uses two functions: setter and getter, and works as simple field
	// ----------------------------------------------------------------------------------
	template<typename _type>
	class Property: public Setter<_type>, public Getter<_type>
	{
	public:
		using Setter::Initialize;
		using Getter::Initialize;

		// Getting value operator
		operator _type()
		{
			return Get();
		}

		// Assign operator
		Property& operator=(const _type& value)
		{
			Set(value);
			return *this;
		}

		// Assign operator
		Property& operator=(const Property& other)
		{
			Set(other.Get());
			return *this;
		}

		// Increment operator
		Property& operator+=(const _type& value)
		{
			Set(Get() + value);
			return *this;
		}

		// Increment operator
		_type operator+(const _type& value)
		{
			return Get() + value;
		}

		// Decrement operator
		Property& operator-=(const _type& value)
		{
			Set(Get() - value);
			return *this;
		}

		// Decrement operator
		_type operator-(const _type& value)
		{
			return Get() - value;
		}

		// Multiplication operator
		Property& operator*=(const _type& value)
		{
			Set(Get()*value);
			return *this;
		}

		// Multiplication operator
		_type operator*(const _type& value)
		{
			return Get()*value;
		}

		// Divide operator
		Property& operator/=(const _type& value)
		{
			Set(Get()/value);
			return *this;
		}

		// Divide operator
		_type operator/(const _type& value)
		{
			return Get()/value;
		}

		// Check equals operator
		bool operator==(const _type& other) const
		{
			return Get() == other;
		}

		// Check not equals operator
		bool operator!=(const _type& other) const
		{
			return Get() != other;
		}
	};

	// -----------------------
	// Array accessor operator
	// -----------------------
	template<typename _res_type, typename _key_type>
	class Accessor
	{
	public:
		typedef Dictionary<String, _res_type> AllRes;

	protected:
		IFunction<_res_type(_key_type)>* mAccessFunc;
		IFunction<AllRes()>*             mAllAccessFunc;

	public:
		// Default constructor
		Accessor():
			mAccessFunc(nullptr), mAllAccessFunc(nullptr)
		{
		}

		// Constructor from constant object function
		template<typename _class_type>
		Accessor(_class_type* object, _res_type(_class_type::*getter)(_key_type) const):
			Accessor()
		{
			Initialize(object, getter);
		}

		// Constructor from object function
		template<typename _class_type>
		Accessor(_class_type* object, _res_type(_class_type::*getter)(_key_type)):
			Accessor()
		{
			Initialize(object, getter);
		}

		// Constructor from static function pointer
		Accessor(_res_type(*getter)(_key_type)):
			Accessor()
		{
			Initialize(object, getter);
		}

		// Constructor from function
		Accessor(IFunction<_res_type(_key_type)>* getter):
			Accessor()
		{
			mAccessFunc = getter;
		}

		// Destructor
		virtual ~Accessor()
		{
			if (mAccessFunc)
				delete mAccessFunc;

			if (mAllAccessFunc)
				delete mAllAccessFunc;
		}

		// Initialize from object function
		template<typename _class_type>
		void Initialize(_class_type* object, _res_type(_class_type::*getter)(_key_type))
		{
			if (mAccessFunc) delete mAccessFunc;
			mAccessFunc = mnew ObjFunctionPtr<_class_type, _res_type, _key_type>(object, getter);
		}

		// Initialize from constant object function
		template<typename _class_type>
		void Initialize(_class_type* object, _res_type(_class_type::*getter)(_key_type) const)
		{
			if (mAccessFunc) delete mAccessFunc;
			mAccessFunc = mnew ObjConstFunctionPtr<_class_type, _res_type, _key_type>(object, getter);
		}

		// Initialize by static getter function
		void Initialize(_res_type(*getter)(_key_type))
		{
			if (mAccessFunc) delete mAccessFunc;
			mAccessFunc = mnew FunctionPtr<_res_type(_key_type)>(getter);
		}

		// Access operator
		_res_type operator[](_key_type key)
		{
			return mAccessFunc->Invoke(key);
		}

		// Initialize from object function
		template<typename _class_type>
		void SetAllAccessFunc(_class_type* object, AllRes(_class_type::*getter)())
		{
			if (mAllAccessFunc) delete mAllAccessFunc;
			mAllAccessFunc = mnew ObjFunctionPtr<_class_type, AllRes>(object, getter);
		}

		Dictionary<String, _res_type> GetAll()
		{
			if (mAllAccessFunc)
				return mAllAccessFunc->Invoke();

			return Dictionary<String, _res_type>();
		}
	};

	// Initialize property helping macros
#define INITIALIZE_PROPERTY(_CLASS, _PROPERTY, _SET_FUNC, _GET_FUNC) \
_PROPERTY.Initialize(this, &_CLASS::_SET_FUNC); _PROPERTY.Initialize(this, &_CLASS::_GET_FUNC)

// Initialize property macros by static functions
#define INITIALIZE_STATIC_PROPERTY(_CLASS, _PROPERTY, _SET_FUNC, _GET_FUNC) \
_PROPERTY.Initialize(&_CLASS::_SET_FUNC); _PROPERTY.Initialize(&_CLASS::_GET_FUNC)

//Initialize setter macros 
#define INITIALIZE_SETTER(_CLASS, _SETTER, _SET_FUNC) \
_SETTER.Initialize(this, &_CLASS::_SET_FUNC)

//Initialize setter macros by static function
#define INITIALIZE_STATIC_SETTER(_CLASS, _SETTER, _SET_FUNC) \
_SETTER.Initialize(&_CLASS::_SET_FUNC)

//Initialize getter macros 
#define INITIALIZE_GETTER(_CLASS, _GETTER, _GET_FUNC) \
_GETTER.Initialize(this, &_CLASS::_GET_FUNC)

//Initialize getter macros by static function
#define INITIALIZE_STATIC_GETTER(_CLASS, _GETTER, _GET_FUNC) \
_GETTER.Initialize(&_CLASS::_GET_FUNC)

	// Initialize accessor macros 
#define INITIALIZE_ACCESSOR(_CLASS, _ACCESSOR, _FUNC) \
_ACCESSOR.Initialize(this, &_CLASS::_FUNC)

}
