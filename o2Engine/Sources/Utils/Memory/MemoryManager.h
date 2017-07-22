#pragma once

#include <vector>
#include <map>

#include "EngineSettings.h"
#include "Utils/CommonTypes.h"

// Memory manager access macros
#define o2Memory MemoryManager::Instance()

// Managed new macros
#define mnew new(__FILE__, __LINE__)

// Managed malloc macros
#define mmalloc(SIZE) _mmalloc(SIZE, __FILE__, __LINE__)

// Managed free macros
#define mfree(MEMORY) _mfree(MEMORY)

// Overloaded managed new operator with source and line arguments
void* operator new(size_t size, const char* location, int line);

// Delete operator with source and line arguments
void  operator delete(void* allocMemory, const char* location, int line);

// Overloaded delete operator
void  operator delete(void* allocMemory);

// Managed malloc() with source and line arguments
void* _mmalloc(size_t size, const char* location, int line);

// Managed free()
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
		// Constructor
		MemoryManager();

		// Destructor
		~MemoryManager();

		// Returns instance of memory manager
		static MemoryManager& Instance();

		// Initializes memory manager
		static void Initialize();

		// Collects information about allocated memory and prints into console
		void DumpInfo();

	protected:
		// ----------------------
		// Allocation information
		// ----------------------
		struct AllocInfo
		{
			const char* source;     // Allocation source code file
			int         sourceLine; // Allocation source code line
			size_t      size;       // Allocated size in bytes
			void*       memory;     // Pointer to allocated memory
		};
		typedef std::map<void*, AllocInfo> AllocsInfosMap;

		static MemoryManager* mInstance; // Instance pointer

		AllocsInfosMap mAllocs;          // Allocations info
		size_t         mTotalBytes;      // Total managed allocated bytes

	protected:
		// It is called when memory was allocated and registers allocation
		void OnMemoryAllocate(void* memory, size_t size, const char* source, int line);

		// It is called when memory releasing, unregisters allocation
		void OnMemoryRelease(void* memory);

		friend void* ::operator new(size_t size, const char* location, int line);
		friend void  ::operator delete(void* allocMemory);
		friend void* ::_mmalloc(size_t size, const char* location, int line);
		friend void  ::_mfree(void* allocMemory);
	};
}
