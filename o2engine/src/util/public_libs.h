#pragma once

#if defined(PLATFORM_WIN)
#	if defined(RENDER_OGL)
#		pragma comment(lib, "opengl32.lib")
#		pragma comment(lib, "glu32.lib")
#		pragma comment(lib, "odbc32.lib")
#		pragma comment(lib, "odbccp32.lib")
#elif defined(RENDER_D3D9C)
#	pragma comment(lib, __FILE__ "/../../../dependencies/dx9c/Lib/d3d9.lib") // aww eeaaah bad ass combination
#	pragma comment(lib, __FILE__ "/../../../dependencies/dx9c/Lib/d3dx9.lib")
#endif

#	pragma comment(lib, __FILE__ "/../../../dependencies/pthreads/pthreadVC2.lib")

#endif //defined(PLATFORM_WIN)
