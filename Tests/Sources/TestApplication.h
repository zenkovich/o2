#pragma once
#include "o2Editor/Core/EditorApplication.h"

class TestApplication: public Editor::EditorApplication
{
protected:
	// Calling when application is starting; runs tests
	void OnStarted() override;
};