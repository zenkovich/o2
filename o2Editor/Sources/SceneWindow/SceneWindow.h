#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"

class SceneEditWidget;

class SceneWindow: public IEditorWindow
{
public:
	IOBJECT(SceneWindow);

protected:
	SceneEditWidget* mEditWidget;

protected:
	SceneWindow();
	SceneWindow(const SceneWindow& other);
	~SceneWindow();

	void InitializeWindow();
};
