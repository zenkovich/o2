#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"


namespace o2
{
	class UIWidget;
}

namespace Editor
{
	class SceneEditWidget;

	class SceneWindow: public IEditorWindow
	{
	public:
		IOBJECT(SceneWindow);

	protected:
		SceneEditWidget* mEditWidget;
		UIWidget*        mLayersView;
		UIWidget*        mGizomsView;

	public:
		SceneWindow();
		SceneWindow(const SceneWindow& other);
		~SceneWindow();

	protected:

		void InitializeWindow();
		void InitializeLayersView();

		// Calls after that all windows was created
		void PostInitializeWindow();
	};
}