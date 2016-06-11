#pragma once

namespace o2
{
	// -----------------------------------------------------------------------
	// Unknown type pointer. Can be initialized and casted to any pointer type
	// -----------------------------------------------------------------------
	class UnknownPtr
	{
	public:
		// Default constructor, initializes by nullptr
		UnknownPtr():mPointer(nullptr) {}

		// Copy-constructor
		UnknownPtr(const UnknownPtr& other):mPointer(other.mPointer) {}

		// Constructor from other pointer
		template<typename T>
		UnknownPtr(T* ptr):mPointer((void*)mPointer) {}

		// Cope operator
		UnknownPtr& operator=(const UnknownPtr& other)
		{
			mPointer = other.mPointer;
			return *this;
		}

		// Copy operator from other pointer
		template<typename T>
		UnknownPtr& operator=(T* ptr)
		{
			mPointer = (void*)ptr;
			return *this;
		}

		// Specified pointer type cast operator
		template<typename T>
		operator T*() const
		{
			return (T*)mPointer;
		}

		// Boolean cast operator, returns false when pointer is null
		operator bool() const;
		{
			return mPointer;
		}

		// Check equals operator
		bool operator==(const UnknownPtr& other) const
		{
			return mPointer == other.mPointer;
		}

		// Check equals to other pointer operator
		template<typename T>
		bool operator==(T* ptr) const
		{
			return mPointer == (void*)ptr;
		}

	protected:
		void* mPointer;
	};
}