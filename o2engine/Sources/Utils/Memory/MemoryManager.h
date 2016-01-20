#pragma once

#include <vector>
#include <map>

#include "EngineSettings.h"
#include "Utils/CommonTypes.h"

// Memory manager access macros
#define o2Memory (*MemoryManager::instance)

// Managed new macros
#define mnew new(__FILE__, __LINE__)

// Managed malloc macros
#define mmalloc(SIZE) _mmalloc(SIZE, __FILE__, __LINE__)

// Managed free macros
#define mfree(MEMORY) _mfree(MEMORY)

void* operator new(size_t size, const char* location, int line);
void  operator delete(void* allocMemory, const char* location, int line);
void  operator delete(void* allocMemory);

void* _mmalloc(size_t size, const char* location, int line);
void _mfree(void* allocMemory);

namespace o2
{
	class LogStream;

	// ------------------------------------------------------------------
	// Memory manager, using for collecting garbage, tracing memory leaks
	// ------------------------------------------------------------------
	class MemoryManager
	{
	public:
		static MemoryManager* instance; // Instance pointer

	public:
		// Constructor
		MemoryManager();

		// Destructor
		~MemoryManager();

		// Collects information about allocated memory and prints into console
		void DumpInfo();

	protected:
		struct AllocInfo
		{
			const char* source;
			int         sourceLine;
			size_t      size;
			void*       memory;
		};
		typedef std::map<void*, AllocInfo> AllocsInfosMap;

		AllocsInfosMap mAllocs;
		size_t         mTotalBytes;

	protected:
		void OnMemoryAllocate(void* memory, size_t size, const char* source, int line);
		void OnMemoryRelease(void* memory);

		friend void* ::operator new(size_t size, const char* location, int line);
		friend void  ::operator delete(void* allocMemory);
		friend void* ::_mmalloc(size_t size, const char* location, int line);
		friend void  ::_mfree(void* allocMemory);
	};
}
