#pragma once

#include "Utils/Containers/Array.h"

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

	template<typename _type>
	class Ptr;

	template<typename _type>
	class PtrBase
	{
		friend class Ptr<_type>;
		typedef Array<Ptr<_type>*> PointersArr;

		PointersArr mPointers;
	};

	template<typename _type>
	class Ptr
	{
		friend class PtrBase<_type>;

		_type* mObject;

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

		bool IsValid() const;
		_type* Get();

		void Release();
		void ReleaseArr();
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
	_type* AutoPtr<_type>::operator->()
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

#pragma region Ptr implementation

	template<typename _type>
	Ptr<_type>::Ptr(_type* object = nullptr) :
		mObject(object)
	{
		if (mObject)
			mObject->mPointers.Add(this);
	}

	template<typename _type>
	Ptr<_type>::Ptr(const Ptr& other) :
		mObject(other.mObject)
	{
		if (mObject)
			mObject->mPointers.Add(this);
	}

	template<typename _type>
	Ptr<_type>::~Ptr()
	{
		mObject->mPointers.Remove(this);
	}

	template<typename _type>
	Ptr<_type>& Ptr<_type>::operator=(const Ptr& other)
	{
		if (mObject)
			mObject->mPointers.Remove(this);

		mObject = other.mObject;

		if (mObject)
			mObject->mPointers.Add(this);

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
		return mObject;
	}

	template<typename _type>
	Ptr<_type>::operator _type* const() const
	{
		return mObject;
	}

	template<typename _type>
	_type* Ptr<_type>::operator->() const
	{
		return mObject;
	}

	template<typename _type>
	_type& Ptr<_type>::operator*()
	{
		return *mObject;
	}

	template<typename _type>
	bool Ptr<_type>::IsValid() const
	{
		return mObject != nullptr;
	}

	template<typename _type>
	_type* Ptr<_type>::Get()
	{
		return mObject;
	}

	template<typename _type>
	void Ptr<_type>::Release()
	{
		if (!mObject)
			return;

		for (auto ptr : mObject->mPointers)
			ptr->mObject = nullptr;

		delete mObject;
	}

	template<typename _type>
	void Ptr<_type>::ReleaseArr()
	{
		if (!mObject)
			return;

		for (auto ptr : mObject->mPointers)
			ptr.mObject = nullptr;

		delete[] mObject;
	}

#pragma endregion Ptr implementation

}