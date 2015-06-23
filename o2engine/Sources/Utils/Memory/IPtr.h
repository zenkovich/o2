#pragma once

#include "Utils/Memory/MemoryManager.h"

namespace o2
{
	struct ObjectInfo;

	class IPtr
	{
		friend class MemoryManager;
		friend struct ObjectInfo;

		template<typename _type>
		friend class ITemplPtr;

	protected:
		ObjectInfo* mObjectInfo; /** Object info pointer. */
		bool        mStatic;     /** True, if object is static, false if object is child of another object. */

		IPtr(ObjectInfo* objectInfo, bool isStatic) :
			mObjectInfo(objectInfo), mStatic(isStatic) {}

		virtual ~IPtr() {}

		virtual void ObjectReleased() = 0;
	};

	template<typename _type>
	class ITemplPtr : public IPtr
	{
		friend class MemoryManager;
		friend struct ObjectInfo;

	protected:
		_type* mObject; /** Pointer to object. */

		/** ctor. */
		ITemplPtr(_type* object = nullptr);

		/** copy-ctor. */
		ITemplPtr(const ITemplPtr& other);

		/** virtual dtor. */
		~ITemplPtr();

		/** Copy operator. */
		ITemplPtr& operator=(const ITemplPtr& other);

		/** Call when object is released. */
		void ObjectReleased();

	public:
		/** Returns true, if mObject is valid. */
		bool IsValid() const;

		/** Releases object. */
		void Release();
	};

	template<typename _type>
	ITemplPtr<_type>::ITemplPtr(_type* object /*= nullptr*/) :
		mObject(object), IPtr(nullptr, false)
	{
		MemoryManager::OnPtrCreating(this);

		if (mObject)
		{
			mObjectInfo = MemoryManager::GetObjectInfo(object);
			mObjectInfo->mPointers.Add(this);
		}
	}

	template<typename _type>
	ITemplPtr<_type>::ITemplPtr(const ITemplPtr& other) :
		ITemplPtr(other.mObject)
	{
	}

	template<typename _type>
	ITemplPtr<_type>::~ITemplPtr()
	{
		if (mObject)
			mObjectInfo->mPointers.Remove(this);

		if (mStatic)
			MemoryManager::OnPtrDestroying(this);
	}

	template<typename _type>
	ITemplPtr<_type>& ITemplPtr<_type>::operator=(const ITemplPtr& other)
	{
		if (mObject)
			mObjectInfo->mPointers.Remove(this);

		mObject = other.mObject;

		if (mObject)
		{
			mObjectInfo = MemoryManager::GetObjectInfo(mObject);
			mObjectInfo->mPointers.Add(this);
		}

		return *this;
	}

	template<typename _type>
	void ITemplPtr<_type>::ObjectReleased()
	{
		mObject = nullptr;
		mObjectInfo = nullptr;
	}

	template<typename _type>
	bool ITemplPtr<_type>::IsValid() const
	{
		return mObject != nullptr;
	}

	template<typename _type>
	void ITemplPtr<_type>::Release()
	{
		if (mObject)
		{
			_type* obj = mObject;

			for (auto ptr : mObjectInfo->mPointers)
				ptr->ObjectReleased();

			delete obj;
		}
	}
}