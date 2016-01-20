#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"

class SceneWindow: public IEditorWindow
{
public:
	IOBJECT(SceneWindow);

protected:
	SceneWindow();
	~SceneWindow();
	SceneWindow(const SceneWindow& other);
};
