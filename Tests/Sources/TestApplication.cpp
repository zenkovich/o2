#include "TestApplication.h"

#include "Tests/Prototypes.h"

void TestApplication::OnStarted()
{
	TestPrototypes();

	Editor::EditorApplication::OnStarted();
}