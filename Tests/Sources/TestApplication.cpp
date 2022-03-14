#include "o2/stdafx.h"
#include "TestApplication.h"

#include "Tests/Prototypes.h"
#include "Tests/Scripts.h"

void TestApplication::OnStarted()
{
	Editor::EditorApplication::OnStarted();
	TestPrototypes();
	TestScripts();
}