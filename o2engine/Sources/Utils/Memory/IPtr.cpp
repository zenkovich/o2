#include "IPtr.h"

#include "Utils/Memory/IObject.h"
#include "Utils/Memory/MemoryManager.h"

namespace o2
{
	void IPtr::Release()
	{
		if (mObject)
		{
			for (auto ptr : mObject->mPointers)
				if (ptr != this)
					ptr->mObject = nullptr;

			delete mObject;
		}
	}

	bool IPtr::IsValid() const
	{
		return mObject != nullptr;
	}

	IPtr& IPtr::operator=(const IPtr& other)
	{
		if (mObject)
			mObject->mPointers.Remove(this);

		mObject = other.mObject;

		if (mObject)
			mObject->mPointers.Add(this);

		return *this;
	}

	IPtr::~IPtr()
	{
		if (mObject)
			mObject->mPointers.Remove(this);

		if (mStatic)
			MemoryManager::OnPtrRemoving(this);
	}

	IPtr::IPtr(const IPtr& other) :
		IPtr(other.mObject)
	{

	}

	IPtr::IPtr(IObject* object /*= nullptr*/) :
		mObject(object), mStatic(false)
	{
		MemoryManager::OnPtrCreating(this);

		if (mObject)
			mObject->mPointers.Add(this);
	}
}