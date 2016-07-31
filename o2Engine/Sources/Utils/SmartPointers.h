#pragma once

#include "Utils/Containers/Vector.h"

namespace o2
{
	template<typename _type>
	class AutoPtr
	{
		_type* mObjPtr;

	public:
		AutoPtr();
		AutoPtr(_type* objPtr);
		~AutoPtr();

		bool IsValid() const;
		_type* Get();

		operator bool();
		operator _type*();

		_type* operator->();
		_type& operator*();
		AutoPtr& operator=(_type* objPtr);

	private:
		AutoPtr(const AutoPtr& other);
		AutoPtr& operator=(const AutoPtr* ptr);
	};

	template<typename _type>
	class AutoArr
	{
		_type* mObjPtr;

	public:
		AutoArr();
		AutoArr(_type* objPtr);
		~AutoArr();

		bool IsValid() const;
		_type* Get();

		operator bool();
		operator _type*();

		_type* operator->();
		_type& operator*();
		AutoArr& operator=(_type* objPtr);

	private:
		AutoArr(const AutoArr& other);
		AutoArr& operator=(const AutoArr* ptr);
	};

#pragma region AutoPtr implementation

	template<typename _type>
	AutoPtr<_type>::AutoPtr() :
		mObjPtr(nullptr)
	{
	}

	template<typename _type>
	AutoPtr<_type>::AutoPtr(_type* objPtr)
	{
		mObjPtr = objPtr;
	}

	template<typename _type>
	AutoPtr<_type>::~AutoPtr()
	{
		if (mObjPtr)
			delete mObjPtr;
	}

	template<typename _type>
	bool AutoPtr<_type>::IsValid() const
	{
		return mObjPtr != nullptr;
	}

	template<typename _type>
	_type* AutoPtr<_type>::Get()
	{
		return mObjPtr;
	}

	template<typename _type>
	AutoPtr<_type>::operator bool() {
		return mObjPtr != nullptr;
	}

	template<typename _type>
	AutoPtr<_type>::operator _type*()
	{
		return mObjPtr;
	}

	template<typename _type>
	_type* AutoPtr<_type>::operator-*()
	{
		return mObjPtr;
	}

	template<typename _type>
	_type& AutoPtr<_type>::operator*()
	{
		return *mObjPtr;
	}

	template<typename _type>
	AutoPtr<_type>& AutoPtr<_type>::operator=(_type* objPtr)
	{
		if (mObjPtr)
			delete mObjPtr;
		mObjPtr = objPtr;
		return *this;
	}

#pragma endregion AutoPtr implementation

#pragma region AutoArr implementation

	template<typename _type>
	AutoArr<_type>::AutoArr() :
		mObjPtr(nullptr)
	{
	}

	template<typename _type>
	AutoArr<_type>::AutoArr(_type* objPtr)
	{
		mObjPtr = objPtr;
	}

	template<typename _type>
	AutoArr<_type>::~AutoArr()
	{
		if (mObjPtr)
			delete[] mObjPtr;
	}

	template<typename _type>
	bool AutoArr<_type>::IsValid() const
	{
		return mObjPtr != nullptr;
	}

	template<typename _type>
	_type* AutoArr<_type>::Get()
	{
		return mObjPtr;
	}

	template<typename _type>
	AutoArr<_type>::operator bool() {
		return mObjPtr != nullptr;
	}

	template<typename _type>
	AutoArr<_type>::operator _type*()
	{
		return mObjPtr;
	}

	template<typename _type>
	_type* AutoArr<_type>::operator->()
	{
		return mObjPtr;
	}

	template<typename _type>
	_type& AutoArr<_type>::operator*()
	{
		return *mObjPtr;
	}

	template<typename _type>
	AutoArr<_type>& AutoArr<_type>::operator=(_type* objPtr)
	{
		if (mObjPtr)
			delete[] mObjPtr;

		mObjPtr = objPtr;
		return *this;
	}

#pragma endregion AutoArr implementation
	
}