#pragma once

namespace o2
{
	template<typename _type>
	class IPtr
	{
		friend class MemoryManager;
		friend struct ObjectInfo;

	protected:
		_type*      mObject;     /** Pointer to object. */
		ObjectInfo* mObjectInfo; /** Object info pointer. */
		bool        mStatic;     /** True, if object is static, false if object is child of another object. */

		/** ctor. */
		IPtr(_type* object = nullptr);

		/** copy-ctor. */
		IPtr(const IPtr& other);

		/** virtual dtor. */
		virtual ~IPtr();

		/** Copy operator. */
		IPtr& operator=(const IPtr& other);

	public:
		/** Returns true, if mObject is valid. */
		bool IsValid() const;

		/** Releases object. */
		void Release();
	};

	template<typename _type>
	IPtr<_type>::IPtr(_type* object /*= nullptr*/) :
		mObject(object), mStatic(false), mObjectInfo(nullptr)
	{
		MemoryManager::OnPtrCreating(this);

		if (mObject)
		{
			mObjectInfo = MemoryManager::GetObjectInfo(object);
			mObjectInfo->mPointers.Add(this);
		}
	}

	template<typename _type>
	IPtr<_type>::IPtr(const IPtr& other) :
		IPtr(other.mObject)
	{
	}

	template<typename _type>
	IPtr<_type>::~IPtr()
	{
		if (mObject)
			mObjectInfo->mPointers.Remove(this);

		if (mStatic)
			MemoryManager::OnPtrDestroying(this);
	}

	template<typename _type>
	IPtr<_type>& IPtr<_type>::operator=(const IPtr& other)
	{
		if (mObject)
			mObjectInfo->mPointers.Remove(this);

		mObject = other.mObject;

		if (mObject)
		{
			mObjectInfo = MemoryManager::GetObjectInfo(object);
			mObjectInfo->mPointers.Add(this);
		}

		return *this;
	}

	template<typename _type>
	bool IPtr<_type>::IsValid() const
	{
		return mObject != nullptr;
	}

	template<typename _type>
	void IPtr<_type>::Release()
	{
		if (mObject)
		{
			_type* obj = mObject;

			for (auto ptr : mObject->mObjectInfo->mPointers)
				ptr->mObject = nullptr;

			MemoryManager::OnObjectDestroying(obj);
			delete obj;
		}
	}
}