#pragma once

#include "Utils/Singleton.h"
#include "Utils/Containers/Array.h"
#include "Utils/Memory/IPtr.h"
#include "Utils/Memory/IObject.h"
#include "Utils/Memory/AllocOperators.h"
#include "EngineSettings.h"

namespace o2
{
	class MemoryManager : public Singleton<MemoryManager>
	{
		friend class IPtr;
		friend class IObject;
		friend void* ::operator new(size_t size, bool managed, const char* location, int line);
		friend void ::operator delete(void* obj, bool managed, const char* location, int line);

	public:
		typedef Array<IPtr*> PointersArr;
		typedef Array<IObject*> ObjectsArr;

	protected:
		ObjectsArr  mObjects;
		PointersArr mPointers;
		bool        mCurrentMark;

		static void OnObjectCreating(IObject* object);
		static void OnObjectRemoving(IObject* object);

		static void OnPtrCreating(IPtr* ptr);
		static void OnPtrRemoving(IPtr* ptr);

	public:
		static void CollectGarbage();
	};
}