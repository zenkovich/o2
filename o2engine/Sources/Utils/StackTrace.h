#pragma once

#include "Dependencies/StackWalker/StackWalker.h"
#include "Utils/String.h"

namespace o2
{
	// ---------------
	// o2 stack walker
	// ---------------
	class o2StackWalker: public StackWalker
	{
		String mRes; // Here puts result of stack trace

	public:
		static o2StackWalker* mInstance; // Static instance pointer

		// Default constructor
		o2StackWalker();

		// Constructor for process
		o2StackWalker(DWORD dwProcessId, HANDLE hProcess);

		// Returns stack trace as text
		static String GetStackTrace();

	protected:
		void OnOutput(LPCSTR szText);
	};

	// Returns stack trace as text
	String GetStackTrace();
}
