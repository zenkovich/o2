#pragma once

#include "Utils/Memory/MemoryManager.h"

namespace o2
{
	struct AllocObjectInfo;

	// -----------------------
	// Basic pointer interface
	// -----------------------
	class IPtr
	{
	public:
		enum class TreePosition { Unknown, Root, Child };

		// Returns object allocation info pointer
		AllocObjectInfo* GetAllocObjectInfo() const
		{
			return mObjectInfo;
		}

		// Sets pointer position as root in memory tree
		void SetupAsRoot()
		{
			mMemTreePosition = TreePosition::Root;
		}

	protected:
		AllocObjectInfo* mObjectInfo;      // Object info pointer
		TreePosition     mMemTreePosition; // Using for GC. True when pointer is on top of memory hierarchy
		int              mIdx;

	protected:
		// Default constructor
		IPtr(): 
			mObjectInfo(nullptr), mMemTreePosition(TreePosition::Unknown) 
		{}

		// Copy-constructor
		IPtr(const IPtr& other):
			mObjectInfo(other.mObjectInfo), mMemTreePosition(TreePosition::Unknown) 
		{}

		// Virtual destructor
		virtual ~IPtr() 
		{}

		// Assign operator
		IPtr& operator=(const IPtr& other)
		{
			mObjectInfo = other.mObjectInfo;
			return *this;
		}

		// Returns position in memory tree
		TreePosition GetMemoryTreePosition() const
		{
			return mMemTreePosition;
		}

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
	public:
		// Returns true, if object is valid
		bool IsValid() const;

		// Releases object
		void Release();

	protected:
		_type* mObject; // Pointer to object

	protected:
		// Constructor
		ITemplPtr(_type* object = nullptr);

		// Copy-constructor
		ITemplPtr(const ITemplPtr& other);

		// Virtual destructor
		virtual ~ITemplPtr();

		// Copy operator
		ITemplPtr& operator=(const ITemplPtr& other);

		// Assign operator
		ITemplPtr& operator=(_type* object);

		// Call when object was released
		void ObjectReleased();

		// Returns pointer to object
		void* ObjectPtr() const;

		friend class MemoryManager;
		friend struct AllocObjectInfo;
	};

	template<typename _type>
	ITemplPtr<_type>::ITemplPtr(_type* object /*= nullptr*/) :
		mObject(object)
	{
		MemoryManager::OnPtrCreating(this);
	}

	template<typename _type>
	ITemplPtr<_type>::ITemplPtr(const ITemplPtr& other) :
		mObject(other.mObject)
	{
		MemoryManager::OnPtrCreating(this);
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
		mObjectInfo = other.mObjectInfo;

		return *this;
	}

	template<typename _type>
	ITemplPtr<_type>& ITemplPtr<_type>::operator=(_type* object)
	{
		mObject = object;
		mObjectInfo = nullptr;

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