#include "MathTests.h"

#include <stdio.h>
#include <conio.h>

#include "Utils/Log/LogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/ConsoleLogStream.h"
#include "Utils/Debug.h"

int main(char** lpCmdLine, int nCmdShow)
{
	TestMath();

	Debug::Log("Some log");
	Debug::LogWarning("Some log warning");
	Debug::LogError("Some log error");

	printf("All tests completed!");
	_getch();

	return 0;
}