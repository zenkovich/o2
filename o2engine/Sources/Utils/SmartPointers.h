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

	class IPtrBase;

	class IPtr
	{
	public:
		friend class IPtrBase;

		IPtrBase* mOwner = nullptr;

		virtual void SetInvalid() = 0;
	};

	class IPtrBase
	{
	public:
		typedef Array<IPtr*> PointersArr;

		PointersArr mPointers;

		int id;

	public:
		bool IsNotFree(Array<IPtrBase*>& processed)
		{
			for (auto ptrIt = mPointers.Begin(); ptrIt != mPointers.End(); ++ptrIt)
			{
				if (!(*ptrIt)->mOwner)
					return false;

				if (processed.Contains((*ptrIt)->mOwner))
					continue;

				processed.Add((*ptrIt)->mOwner);

				if (!(*ptrIt)->mOwner->IsNotFree(processed))
					return false;
			}

			return true;
		}

		bool OnRemovePointer(IPtr* ptr) {
			mPointers.Remove(ptr);

			Array<IPtrBase*> processed;
			processed.Add(this);
			return IsNotFree(processed);
			//return mPointers.Count() == 0; //FIXME
		}
	};
	
	template<typename _type>
	class Ptr : public IPtr
	{
		friend class IPtrBase;

		_type* mObject;

		void SetInvalid()
		{
			mObject = nullptr;
		}

		void Release() {
			if (mObject)
			{
				if (mObject->OnRemovePointer(this)) {
					printf("delete %x\n", mObject);
					delete mObject;
				}
			}
		}

	public:
		Ptr(IPtrBase* owner):
			mObject(nullptr)
		{
			mOwner = owner;
		}

		Ptr(_type* object = nullptr) :
			mObject(object)
		{
			if (mObject)
				mObject->mPointers.Add(this);
		}

		Ptr(const Ptr& other) :
			mObject(other.mObject)
		{
			if (mObject)
				mObject->mPointers.Add(this);
		}

		~Ptr()
		{
			Release();
		}

		Ptr& operator=(const Ptr& other)
		{
			Release();

			mObject = other.mObject;

			if (mObject)
				mObject->mPointers.Add(this);

			return *this;
		}

		bool operator==(const Ptr& other) const
		{
			return mObject == other.mObject;
		}

		bool operator!=(const Ptr& other) const
		{
			return mObject != other.mObject;
		}

		operator bool() const
		{
			return mObject != nullptr;
		}

		operator _type*()
		{
			return mObject;
		}

		operator _type* const() const
		{
			return mObject;
		}

		_type* operator->()
		{
			return mObject;
		}

		_type& operator*()
		{
			return *mObject;
		}

		bool IsValid() const
		{
			return mObject != nullptr;
		}

		_type* Get()
		{
			return mObject;
		}
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

}