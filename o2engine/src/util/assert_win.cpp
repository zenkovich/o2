#include "engine_settings.h"
#ifdef PLATFORM_WIN

#include <string>
#include <windows.h>

#include "assert.h"
OPEN_O2_NAMESPACE

void errorMessage( const char* desc, const char* file, long line )
{
	char message[1024];
	sprintf(message, "Error at\n%s : %i\nDescription:\n%s", file, line, desc);

	//*gLog << message.c_str();

	MessageBox(NULL, message, "Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
}

CLOSE_O2_NAMESPACE


#endif //platform_win