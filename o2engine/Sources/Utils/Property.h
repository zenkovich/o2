#pragma once

namespace o2
{

	/** C#-like property. Uses two functions: setter and getter, and works as simple field. */
	template<typename _class_type, typename _type>
	class Property
	{
		_class_type* mObject; /** Pointer to owner object. */                        

		void (_class_type::*mSetter)(const _type&);  
		void (_class_type::*mSetterNonConst)(_type); 
		_type(_class_type::*mGetter)() const;        
		_type(_class_type::*mGetterNonConst)();      

		Property(const Property& other);

	public:
		/** ctor. */
		Property():
			mSetter(nullptr), mGetter(nullptr), mSetterNonConst(nullptr), mGetterNonConst(nullptr), mObject(nullptr)
		{
		}

		/** ctor. */
		Property(_class_type* object, void (_class_type::*setter)(const _type&), _type(_class_type::*getter)() const)
		{
			mObject = object;
			mSetter = setter;
			mGetter = getter;
		}

		/** ctor. */
		Property(_class_type* object, void (_class_type::*setter)(const _type&), _type(_class_type::*getter)())
		{
			mObject = object;
			mSetter = setter;
			mGetterNonConst = getter;
		}

		/** ctor. */
		Property(_class_type* object, void (_class_type::*setter)(_type), _type(_class_type::*getter)() const)
		{
			mObject = object;
			mSetterNonConst = setter;
			mGetter = getter;
		}

		/** ctor. */
		Property(_class_type* object, void (_class_type::*setter)(_type), _type(_class_type::*getter)())
		{
			mObject = object;
			mSetterNonConst = setter;
			mGetterNonConst = getter;
		}

		/** Getting value operator. */
		operator _type()
		{
			return Get();
		}

		/** Returns getted value. */
		_type Get() const
		{
			return mGetter != nullptr ? (mObject->*mGetter)():(mObject->*mGetterNonConst)();
		}

		/** Sets value. */
		void Set(_type& value)
		{
			if (mSetter != nullptr) (mObject->*mSetter)(value);
			else                    (mObject->*mSetterNonConst)(value);
		}

		/** Assign operator. */
		Property& operator=(_type& value)
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

#define INITIALIZE_PROPERTY(_CLASS, _PROPERTY, _SET_FUNC, _GET_FUNC) \
	_PROPERTY.Initialize(this, &_CLASS::_SET_FUNC, &_CLASS::_GET_FUNC); 

#define PROPERTY(_CLASS, _TYPE) Property<_CLASS, _TYPE>

}