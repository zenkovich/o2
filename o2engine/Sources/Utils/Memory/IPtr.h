#pragma once

#include "Utils/Memory/MemoryManager.h"
#include "Utils/String.h"

namespace o2
{
	struct AllocObjectInfo;

	// -----------------------
	// Basic pointer interface
	// -----------------------
	class IPtr
	{
	public: 
		// Returns object allocation info pointer
		AllocObjectInfo* GetAllocObjectInfo() const
		{
			return mObjectInfo;
		}

	protected:
		AllocObjectInfo* mObjectInfo; // Object info pointer
		bool             mIsOnTop;    // Using for GC. True when pointer is on top of memory hierarchy

	protected:
		// Default constructor
		IPtr(): mObjectInfo(nullptr), mIsOnTop(false) {}

		// Virtual destructor
		virtual ~IPtr() {}

		// Invoking when object was released
		virtual void ObjectReleased() = 0;

		// Returns pointer to object
		virtual void* ObjectPtr() const = 0;

		friend class MemoryManager;
		friend struct AllocObjectInfo;

		template<typename _type>
		friend class ITemplPtr;
	};

	// --------------------------
	// Template pointer interface
	// --------------------------
	template<typename _type>
	class ITemplPtr : public IPtr
	{
	protected:
		_type* mObject; // Pointer to object

	protected:
		// Constructor
		ITemplPtr(_type* object = nullptr);

		// Copy-constructor
		ITemplPtr(const ITemplPtr& other);

		// Virtual destructor
		~ITemplPtr();

		// Copy operator
		ITemplPtr& operator=(const ITemplPtr& other);

		// Assign operator
		ITemplPtr& operator=(_type* object);

		// Call when object was released
		void ObjectReleased();

		// Returns pointer to object
		void* ObjectPtr() const;

	public:
		// Returns true, if object is valid
		bool IsValid() const;

		// Releases object
		void Release();

		friend class MemoryManager;
		friend struct AllocObjectInfo;
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
		{
			delete mObject;
			ObjectReleased();
		}
	}
}