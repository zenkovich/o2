#include "Assert.h"

#include <string>
#include <windows.h>

namespace o2
{
	void ErrorMessage(const char* desc, const char* file, long line)
	{
		char message[1024];
		sprintf(message, "Error at\n%s : %i\nDescription:\n%s", file, line, desc);

		MessageBox(NULL, message, "Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
}