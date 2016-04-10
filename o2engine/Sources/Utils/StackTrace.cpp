#include "StackTrace.h"

namespace o2
{
	String GetStackTrace()
	{
		return o2StackWalker::GetStackTrace();
	}

	o2StackWalker::o2StackWalker(DWORD dwProcessId, HANDLE hProcess):
		StackWalker(dwProcessId, hProcess)
	{}

	o2StackWalker::o2StackWalker():
		StackWalker()
	{}

	o2StackWalker& o2StackWalker::Instance()
	{
		return *mInstance;
	}

	void o2StackWalker::Initialize()
	{
		mInstance = new o2StackWalker();
	}

	o2::String o2StackWalker::GetStackTrace()
	{
		mInstance->mRes = "";
		mInstance->ShowCallstack();
		return mInstance->mRes;
	}

	void o2StackWalker::OnOutput(LPCSTR szText)
	{
		mRes += szText;
	}
}