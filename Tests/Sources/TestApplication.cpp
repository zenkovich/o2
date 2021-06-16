#include "TestApplication.h"

#include "Tests/Prototypes.h"

void TestApplication::OnStarted()
{
	Editor::EditorApplication::OnStarted();
	TestPrototypes();
}