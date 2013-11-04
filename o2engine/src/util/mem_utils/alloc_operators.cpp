#include "alloc_operators.h"

#include "allocator_interface.h"

#ifdef MEMORY_TRACE

	#ifndef ENGINE_NAMESPACE_NAME
		#define ENGINE_NAMESPACE_NAME
		#define FAKE_ENGINE_NAMESPACE_DEF
	#endif

	void* operator new(uint32 size, const char* location, int line)
	{
		#ifdef BASIC_MEMORY_ALLOCATOR
			return ENGINE_NAMESPACE_NAME::cMemoryManager::instance().mBasicAllocator->allocs(size, location, (unsigned int)line);
		#else
			void* res = malloc(size);
			ENGINE_NAMESPACE_NAME::cMemoryManager::instance().registAlloc(res, size, location, line, NULL);
			return res;
		#endif
	}

	void* operator new[](uint32 size, const char* location, int line)
	{
		#ifdef BASIC_MEMORY_ALLOCATOR
			return ENGINE_NAMESPACE_NAME::cMemoryManager::instance().mBasicAllocator->allocs(size, location, (unsigned int)line);
		#else
			void* res = malloc(size);
			ENGINE_NAMESPACE_NAME::cMemoryManager::instance().registAlloc(res, size, location, line, NULL);
			return res;
		#endif
	}

	void operator delete(void* ptr)
	{
		#ifdef BASIC_MEMORY_ALLOCATOR
			ENGINE_NAMESPACE_NAME::cMemoryManager::instance().mBasicAllocator->frees(ptr);
		#else
			//ENGINE_NAMESPACE_NAME::cMemoryManager::instance().unregistAlloc(ptr);
			free(ptr);
		#endif
	}

	void operator delete[](void* ptr)
	{
		#ifdef BASIC_MEMORY_ALLOCATOR
			ENGINE_NAMESPACE_NAME::cMemoryManager::instance().mBasicAllocator->frees(ptr);
		#else
			//ENGINE_NAMESPACE_NAME::cMemoryManager::instance().unregistAlloc(ptr);
			free(ptr);
		#endif
	}

	void operator delete(void* ptr, const char* location, int line)
	{
		#ifdef BASIC_MEMORY_ALLOCATOR
			ENGINE_NAMESPACE_NAME::cMemoryManager::instance().mBasicAllocator->frees(ptr);
		#else
			ENGINE_NAMESPACE_NAME::cMemoryManager::instance().unregistAlloc(ptr);
			free(ptr);
		#endif
	}

	void operator delete[](void* ptr, const char* location, int line)
	{
		#ifdef BASIC_MEMORY_ALLOCATOR
			ENGINE_NAMESPACE_NAME::cMemoryManager::instance().mBasicAllocator->frees(ptr);
		#else
			ENGINE_NAMESPACE_NAME::cMemoryManager::instance().unregistAlloc(ptr);
			free(ptr);
		#endif
	}

	#ifdef FAKE_ENGINE_NAMESPACE_DEF
		#undef ENGINE_NAMESPACE_NAME
		#undef FAKE_ENGINE_NAMESPACE_DEF
	#endif

#else

	#ifdef BASIC_MEMORY_ALLOCATOR

		void operator delete(void* ptr)
		{		
			ENGINE_NAMESPACE_NAME::cMemoryManager::instance().mBasicAllocator->free(ptr);
		}

		void operator delete[](void* ptr)
		{
			ENGINE_NAMESPACE_NAME::cMemoryManager::instance().mBasicAllocator->free(ptr);
		}

	#endif
	
#endif //MEMORY TRACE

#ifdef BASIC_MEMORY_ALLOCATOR

	void* operator new(uint32 size)
	{	
		return ENGINE_NAMESPACE_NAME::cMemoryManager::instance().mBasicAllocator->alloc(size);
	}

	void* operator new[](uint32 size)
	{
		return ENGINE_NAMESPACE_NAME::cMemoryManager::instance().mBasicAllocator->alloc(size);
	}

#endif
