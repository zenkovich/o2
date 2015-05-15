#pragma once

namespace o2
{
	template<typename _class_type, typename _type>
	class Property
	{
		_class_type* mObject;                        

		void (_class_type::*mSetter)(const _type&);  
		void (_class_type::*mSetterNonConst)(_type); 
		_type(_class_type::*mGetter)() const;        
		_type(_class_type::*mGetterNonConst)();      

		Property(const Property& other);

	public:
		Property():
			mSetter(nullptr), mGetter(nullptr), mSetterNonConst(nullptr), mGetterNonConst(nullptr), mObject(nullptr)
		{
		}

		Property(_class_type* object, void (_class_type::*setter)(const _type&), _type(_class_type::*getter)() const)
		{
			mObject = object;
			mSetter = setter;
			mGetter = getter;
		}

		Property(_class_type* object, void (_class_type::*setter)(const _type&), _type(_class_type::*getter)())
		{
			mObject = object;
			mSetter = setter;
			mGetterNonConst = getter;
		}

		Property(_class_type* object, void (_class_type::*setter)(_type), _type(_class_type::*getter)() const)
		{
			mObject = object;
			mSetterNonConst = setter;
			mGetter = getter;
		}

		Property(_class_type* object, void (_class_type::*setter)(_type), _type(_class_type::*getter)())
		{
			mObject = object;
			mSetterNonConst = setter;
			mGetterNonConst = getter;
		}

		operator _type()
		{
			return Get();
		}

		_type Get() const
		{
			return mGetter != nullptr ? (mObject->*mGetter)():(mObject->*mGetterNonConst)();
		}

		void Set(_type& value)
		{
			if (mSetter != nullptr) (mObject->*mSetter)(value);
			else                    (mObject->*mSetterNonConst)(value);
		}

		Property& operator=(_type& value)
		{
			Set(value);
			return *this;
		}

		Property& operator=(const Property& other)
		{
			Set(other.Get());
			return *this;
		}

		Property& operator+=(const _type& value)
		{
			Set(Get() + value);
			return *this;
		}

		_type operator+(const _type& value)
		{
			return Get() + value;
		}

		Property& operator-=(const _type& value)
		{
			Set(Get() - value);
			return *this;
		}

		_type operator-(const _type& value)
		{
			return Get() - value;
		}

		Property& operator*=(const _type& value)
		{
			Set(Get()*value);
			return *this;
		}

		_type operator*(const _type& value)
		{
			return Get()*value;
		}

		Property& operator/=(const _type& value)
		{
			Set(Get()/value);
			return *this;
		}

		_type operator/(const _type& value)
		{
			return Get()/value;
		}
	};

#define INITIALIZE_PROPERTY(_CLASS, _PROPERTY, _SET_FUNC, _GET_FUNC) \
	_PROPERTY.Initialize(this, &_CLASS::_SET_FUNC, &_CLASS::_GET_FUNC); 

#define PROPERTY(_CLASS, _TYPE) Property<_CLASS, _TYPE>

}