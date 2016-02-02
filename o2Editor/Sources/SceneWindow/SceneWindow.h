#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"

class SceneEditWidget;

namespace o2
{
	class UIWidget;
}

class SceneWindow: public IEditorWindow
{
public:
	IOBJECT(SceneWindow);

protected:
	SceneEditWidget* mEditWidget;
	UIWidget*        mLayersView;
	UIWidget*        mGizomsView;

protected:
	SceneWindow();
	SceneWindow(const SceneWindow& other);
	~SceneWindow();

	void InitializeWindow();
	void InitializeLayersView();

	// Calls after that all windows was created
	void PostInitializeWindow();
};
