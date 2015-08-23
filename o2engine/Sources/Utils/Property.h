#pragma once

#include "Utils/Delegates.h"

namespace o2
{
	/** Get function overriding class. */
	template<typename _type>
	class Getter
	{
		IFunction<_type()>* mGetter;

	public:
		/** ctor. */
		Getter():
			mGetter(nullptr)
		{
		}

		/** ctor. */
		template<typename _class_type>
		Getter(_class_type* object, _type(_class_type::*getter)() const):
			Getter()
		{
			Initialize(object, getter);
		}

		/** ctor. */
		template<typename _class_type>
		Getter(_class_type* object, _type(_class_type::*getter)()):
			Getter()
		{
			Initialize(object, getter);
		}

		/** ctor. */
		Getter(IFunction<_type()>* getter):
			Getter()
		{
			mGetter = getter;
		}

		/** dtor.*/
		virtual ~Getter()
		{
			delete mGetter;
		}

		/** Initialize. */
		template<typename _class_type>
		void Initialize(_class_type* object, _type(_class_type::*getter)())
		{
			mGetter = new ObjFunctionPtr<_class_type, _type>(object, getter);
		}

		/** Initialize. */
		template<typename _class_type>
		void Initialize(_class_type* object, _type(_class_type::*getter)() const)
		{
			mGetter = new ObjConstFunctionPtr<_class_type, _type>(object, getter);
		}

		/** Getting value operator. */
		operator _type()
		{
			return Get();
		}

		/** Returns value from get function. */
		_type Get() const
		{
			return (*mGetter)();
		}
	};

	/** Set function overriding class. */
	template<typename _type>
	class Setter
	{
		IFunction<void(const _type&)>* mSetter;

	public:
		/** ctor. */
		Setter():
			mSetter(nullptr)
		{
		}

		/** ctor. */
		template<typename _class_type>
		Setter(_class_type* object, void(_class_type::*setter)(const _type&)):
			Setter()
		{
			Initialize(object, setter);
		}

		/** ctor. */
		template<typename _class_type>
		Setter(_class_type* object, void(_class_type::*setter)(_type&)):
			Setter()
		{
			Initialize(object, setter);
		}

		/** ctor. */
		template<typename _class_type>
		Setter(_class_type* object, void(_class_type::*setter)(_type)):
			Setter()
		{
			Initialize(object, setter);
		}

		/** ctor. */
		Setter(IFunction<void(const _type&)>* setter):
			mSetter(setter)
		{
		}

		/** dtor.*/
		virtual ~Setter()
		{
			delete mSetter;
		}

		/** Initialize. */
		template<typename _class_type>
		void Initialize(_class_type* object, void(_class_type::*setter)(const _type&))
		{
			mSetter = new ObjFunctionPtr<_class_type, void, const _type&>(object, setter);
		}

		/** Initialize. */
		template<typename _class_type>
		void Initialize(_class_type* object, void(_class_type::*setter)(_type&))
		{
			auto lambda = [=](const _type& value) { (object->*setter)(const_cast<_type&>(value)); };
			mSetter = new SharedLambda<void(const _type&)>(lambda);
		}

		/** Initialize. */
		template<typename _class_type>
		void Initialize(_class_type* object, void(_class_type::*setter)(_type))
		{
			auto lambda = [=](const _type& value) { (object->*setter)(value); };
			mSetter = new SharedLambda<void(const _type&)>(lambda);
		}

		void Set(const _type& value)
		{
			(*mSetter)(value);
		}

		/** Assign operator. */
		Setter& operator=(const _type& value)
		{
			Set(value);
			return *this;
		}
	};

	/** C#-like property. Uses two functions: setter and getter, and works as simple field. */
	template<typename _type>
	class Property: public Setter<_type>, public Getter<_type>
	{
	public:
		using Setter::Initialize;
		using Getter::Initialize;

		/** Getting value operator. */
		operator _type()
		{
			return Get();
		}

		/** Assign operator. */
		Property& operator=(const _type& value)
		{
			Set(value);
			return *this;
		}

		/** Assign operator. */
		Property& operator=(const Property& other)
		{
			Set(other.Get());
			return *this;
		}

		/** Increment operator. */
		Property& operator+=(const _type& value)
		{
			Set(Get() + value);
			return *this;
		}

		/** Increment operator. */
		_type operator+(const _type& value)
		{
			return Get() + value;
		}

		/** Decrement operator. */
		Property& operator-=(const _type& value)
		{
			Set(Get() - value);
			return *this;
		}

		/** Decrement operator. */
		_type operator-(const _type& value)
		{
			return Get() - value;
		}

		/** Multiplication operator. */
		Property& operator*=(const _type& value)
		{
			Set(Get()*value);
			return *this;
		}

		/** Multiplication operator. */
		_type operator*(const _type& value)
		{
			return Get()*value;
		}

		/** Divide operator. */
		Property& operator/=(const _type& value)
		{
			Set(Get()/value);
			return *this;
		}

		/** Divide operator. */
		_type operator/(const _type& value)
		{
			return Get()/value;
		}
	};

/** Initialize property helping macros. */
#define INITIALIZE_PROPERTY(_CLASS, _PROPERTY, _SET_FUNC, _GET_FUNC) \
	_PROPERTY.Initialize(this, &_CLASS::_SET_FUNC); _PROPERTY.Initialize(this, &_CLASS::_GET_FUNC)

}