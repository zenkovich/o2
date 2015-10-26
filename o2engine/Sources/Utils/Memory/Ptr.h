#pragma once

#include "Utils/Memory/IPtr.h"

namespace o2
{
	// --------------------------------
	// Smart pointer to template object
	// --------------------------------
	template<typename _type>
	class Ptr : public ITemplPtr<_type>
	{
	public:
		// Constructor
		Ptr(_type* object = nullptr);

		// Copy-constructor
		Ptr(const Ptr& other);

		// Destructor
		~Ptr();

		// Copy operator
		Ptr& operator=(const Ptr& other);

		// Assign operator
		Ptr& operator=(_type* object);

		// Check equal operator
		bool operator==(const Ptr& other) const;

		// Check equal operator
		bool operator==(_type* other) const;

		// Check not equal operator
		bool operator!=(const Ptr& other) const;

		// Boolean cast operator. Returns true when pointer is valid
		operator bool() const;

		// Object access operator
		operator _type*();

		// Object contains access operator
		operator _type* const() const;

		template<typename _other_type>
		operator Ptr<_other_type>() const;

		// Pointer access operator
		_type* operator->() const;

		// Reference access operator
		_type& operator*();

		// Returns object pointer
		_type* Get();

		// Static cost to another type
		template<typename _other_type>
		Ptr<_other_type> Cast() const;
	};

	template<typename _type>
	Ptr<_type>::Ptr(_type* object = nullptr) :
		ITemplPtr(object)
	{}

	template<typename _type>
	Ptr<_type>::Ptr(const Ptr& other) :
		ITemplPtr(other)
	{}

	template<typename _type>
	Ptr<_type>::~Ptr()
	{}

	template<typename _type>
	Ptr<_type>& Ptr<_type>::operator=(const Ptr& other)
	{
		ITemplPtr::operator=(other);
		return *this;
	}

	template<typename _type>
	Ptr<_type>& o2::Ptr<_type>::operator=(_type* object)
	{
		ITemplPtr::operator=(object);
		return *this;
	}

	template<typename _type>
	bool Ptr<_type>::operator==(const Ptr& other) const
	{
		return mObject == other.mObject;
	}

	template<typename _type>
	bool Ptr<_type>::operator==(_type* other) const
	{
		return mObject == other;
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
	template<typename _other_type> 
	Ptr<_type>::operator Ptr<_other_type>() const
	{
		return Cast<_other_type>();
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

	template<typename _type>
	template<typename _other_type>
	Ptr<_other_type> Ptr<_type>::Cast() const
	{
		return Ptr<_other_type>(static_cast<_other_type*>(mObject));
	}
}