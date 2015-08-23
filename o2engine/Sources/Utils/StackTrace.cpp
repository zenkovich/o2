#include "StackTrace.h"

namespace o2
{
	String GetStackTrace()
	{
		return o2StackWalker::GetStackTrace();
	}
}