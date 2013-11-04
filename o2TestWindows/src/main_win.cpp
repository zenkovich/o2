#include "test_app.h"

#include "util/mem_utils/test_allocator.h"
#include "util/mem_utils/std_allocator.h"
#include "util/mem_utils/pool_allocator.h"
#include "util/timer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	o2::TestApp app;
	app.launch();

	return 0;
}