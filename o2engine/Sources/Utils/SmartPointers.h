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

		PtrBase<_type>* mObject;
		bool            mIsHolder;

	public:
		Ptr(_type* object = nullptr):
			mObject(object), mIsHolder(true)
		{
			if (mObject)
			{
				for (auto ptr:mObject->mPointers)
				{
					if (ptr->mIsHolder)
					{
						mIsHolder = false;
						break;
					}
				}

				mObject->mPointers.Add(this);
			}
		}

		Ptr(const Ptr& other, bool asHolder = false):
			mObject(other.mObject), mIsHolder(asHolder)
		{
			if (mObject)
			{
				if (asHolder)
				{
					for (auto ptr:mObject->mPointers)
					{
						if (ptr->mIsHolder)
						{
							ptr->mIsHolder = false;
							break;
						}
					}
				}

				mObject->mPointers.Add(this);
			}
		}

		~Ptr()
		{
			if (mObject)
			{
				if (mIsHolder)
				{
					for (auto ptr:mObject->mPointers)
						ptr->mObject = nullptr;

					delete mObject;
				}
				else
				{
					mObject->mPointers.Remove(this);
				}
			}
		}

		Ptr& operator=(const Ptr& other)
		{
			if (mObject)
			{
				if (mIsHolder)
				{
					for (auto ptr:mObject->mPointers)
						ptr->mObject = nullptr;

					delete mObject;
				}
				else
				{
					mObject->mPointers.Remove(this);
				}
			}

			mObject = other.mObject;
			mIsHolder = false;

			if (mObject)
				mObject->mPointers.Add(this);
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

		Ptr& SetAsHolder()
		{
			for (auto ptr:mObject->mPointers)
			{
				if (ptr->mIsHolder)
				{
					ptr->mIsHolder = false;
					break;
				}
			}

			mIsHolder = true;

			return *this;
		}

		bool IsHolder() const
		{
			return mIsHolder;
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