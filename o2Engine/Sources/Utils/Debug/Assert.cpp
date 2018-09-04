#include "stdafx.h"
#include "Assert.h"

#include <string>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace o2
{
	void ErrorMessage(const char* desc, const char* file, long line)
	{
		char message[1024];
		sprintf(message, "Error at\n%s : %i\nDescription:\n%s", file, (int)line, desc);

#ifdef PLATFORM_WINDOWS
		MessageBox(nullptr, message, "Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#endif
	}
}