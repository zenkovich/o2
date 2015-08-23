#pragma once

#include "Dependencies/StackWalker/StackWalker.h"
#include "Utils/String.h"

namespace o2
{
	class o2StackWalker: public StackWalker
	{
		String                mRes;

	public:
		static o2StackWalker* mInstance;

		o2StackWalker(): StackWalker() {}
		o2StackWalker(DWORD dwProcessId, HANDLE hProcess): StackWalker(dwProcessId, hProcess) {}

		static String GetStackTrace()
		{
			mInstance->mRes = "";
			mInstance->ShowCallstack();
			return mInstance->mRes;
		}

	protected:
		void OnOutput(LPCSTR szText) {
			mRes += szText;
		}
	};

	String GetStackTrace();
}
