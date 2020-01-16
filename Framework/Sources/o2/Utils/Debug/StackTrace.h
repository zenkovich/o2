#pragma once

#ifdef PLATFORM_WINDOWS

#include "3rdPartyLibs/StackWalker/StackWalker.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	// ---------------
	// o2 stack walker
	// ---------------
	class o2StackWalker: public StackWalker
	{
	public:
		// Returns stack walker instance
		static o2StackWalker& Instance();

		// Initializes stack walker
		static void Initialize();

		// Returns stack trace as text
		static String GetStackTrace();

	protected:
		static o2StackWalker* mInstance; // Static instance pointer
		String mRes; // Here puts result of stack trace

		// Default constructor
		o2StackWalker();

		// Constructor for process
		o2StackWalker(DWORD dwProcessId, HANDLE hProcess);

		void OnOutput(LPCSTR szText);
	};

	// Returns stack trace as text
	String GetStackTrace();
}

#endif //PLATFORM_WINDOWS
