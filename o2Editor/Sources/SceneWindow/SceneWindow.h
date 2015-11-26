#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"

class SceneWindow: public IEditorWindow
{
public:
	SceneWindow();

	SceneWindow(const SceneWindow& other);

	~SceneWindow();

	SERIALIZABLE_IMPL(SceneWindow);

	IOBJECT(SceneWindow)
	{
		BASE_CLASS(IEditorWindow);
	}
};
