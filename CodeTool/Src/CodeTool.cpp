#include "CodeToolApp.h"


int main(int nargs, char** agrs)
{

	CodeToolApplication app;
	app.SetArguments(agrs, nargs);
	app.Process();

    return 0;
}

