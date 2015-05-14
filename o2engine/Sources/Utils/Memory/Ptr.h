#pragma once

#include "Utils/Memory/IPtr.h"

namespace o2
{
	template<typename _type>
	class Ptr : public IPtr
	{
	public:
		Ptr(_type* object = nullptr);
		Ptr(const Ptr& other);
		~Ptr();

		Ptr& operator=(const Ptr& other);
		bool operator==(const Ptr& other) const;
		bool operator!=(const Ptr& other) const;

		operator bool() const;
		operator _type*();
		operator _type* const() const;

		_type* operator->() const;
		_type& operator*();

		_type* Get();
	};


	template<typename _type>
	Ptr<_type>::Ptr(_type* object = nullptr) :
		IPtr(object)
	{}

	template<typename _type>
	Ptr<_type>::Ptr(const Ptr& other) :
		IPtr(other)
	{}

	template<typename _type>
	Ptr<_type>::~Ptr()
	{}

	template<typename _type>
	Ptr<_type>& Ptr<_type>::operator=(const Ptr& other)
	{
		IPtr::operator=(other);
		return *this;
	}

	template<typename _type>
	bool Ptr<_type>::operator==(const Ptr& other) const
	{
		return mObject == other.mObject;
	}

	template<typename _type>
	bool Ptr<_type>::operator!=(const Ptr& other) const
	{
		return mObject != other.mObject;
	}

	template<typename _type>
	Ptr<_type>::operator bool() const
	{
		return mObject != nullptr;
	}

	template<typename _type>
	Ptr<_type>::operator _type*()
	{
		return (_type*)mObject;
	}

	template<typename _type>
	Ptr<_type>::operator _type* const() const
	{
		return (_type*)mObject;
	}

	template<typename _type>
	_type* Ptr<_type>::operator->() const
	{
		return (_type*)mObject;
	}

	template<typename _type>
	_type& Ptr<_type>::operator*()
	{
		return *(_type*)mObject;
	}

	template<typename _type>
	_type* Ptr<_type>::Get()
	{
		return (_type*)mObject;
	}
}