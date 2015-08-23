#pragma once

#include "Utils/Memory/MemoryManager.h"
#include "Utils/String.h"

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
		bool        mIsOnTop;    /** Using for GC. True when pointer haven't parent pointer. */

		IPtr(): mObjectInfo(nullptr) {}

		virtual ~IPtr() {}

		virtual void ObjectReleased() = 0;
		virtual void* ObjectPtr() const = 0;
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

		/** Assign operator. */
		ITemplPtr& operator=(_type* object);

		/** Call when object is released. */
		void ObjectReleased();

		void* ObjectPtr() const;

	public:
		/** Returns true, if mObject is valid. */
		bool IsValid() const;

		/** Releases object. */
		void Release();
	};

	template<typename _type>
	ITemplPtr<_type>::ITemplPtr(_type* object /*= nullptr*/) :
		mObject(object)
	{
		MemoryManager::OnPtrCreating(this);

		if (mObject)
			mObjectInfo = MemoryManager::GetObjectInfo(object);
	}

	template<typename _type>
	ITemplPtr<_type>::ITemplPtr(const ITemplPtr& other) :
		ITemplPtr(other.mObject)
	{
	}

	template<typename _type>
	ITemplPtr<_type>::~ITemplPtr()
	{
		MemoryManager::OnPtrDestroying(this);
	}

	template<typename _type>
	ITemplPtr<_type>& ITemplPtr<_type>::operator=(const ITemplPtr& other)
	{
		mObject = other.mObject;

		if (mObject)
			mObjectInfo = MemoryManager::GetObjectInfo(mObject);

		return *this;
	}

	template<typename _type>
	ITemplPtr<_type>& o2::ITemplPtr<_type>::operator=(_type* object)
	{
		mObject = object;

		if (mObject)
			mObjectInfo = MemoryManager::GetObjectInfo(mObject);

		return *this;
	}

	template<typename _type>
	void ITemplPtr<_type>::ObjectReleased()
	{
		mObject = nullptr;
		mObjectInfo = nullptr;
	}

	template<typename _type>
	void* ITemplPtr<_type>::ObjectPtr() const
	{
		return mObject;
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
			delete mObject;

		ObjectReleased();
	}
}