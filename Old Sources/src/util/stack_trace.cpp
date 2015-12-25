#include "stack_trace.h"

#include "stack_walker/StackWalker.h"

OPEN_O2_NAMESPACE

class MyStackWalker: public StackWalker
{
public:
	std::string res;
	MyStackWalker(): StackWalker() {}
	MyStackWalker(DWORD dwProcessId, HANDLE hProcess): StackWalker(dwProcessId, hProcess) {}
	virtual void OnOutput(LPCSTR szText) {
		res += szText;
		//printf(szText);
		/*StackWalker::OnOutput(szText);*/
	}
};


MyStackWalker ____sw;

std::string GetStackTraceAsStr()
{
	____sw.res = "";
	____sw.ShowCallstack();
	return ____sw.res;
}

CLOSE_O2_NAMESPACE