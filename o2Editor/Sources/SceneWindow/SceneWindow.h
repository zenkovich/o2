#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"


namespace o2
{
	class UIWidget;
}

namespace Editor
{
	class SceneEditScreen;

	class SceneWindow: public IEditorWindow
	{
	public:
		IOBJECT(SceneWindow);

	protected:
		SceneEditScreen* mEditWidget;
		UIWidget*        mLayersView;
		UIWidget*        mGizomsView;

	public:
		SceneWindow();
		SceneWindow(const SceneWindow& other);
		~SceneWindow();

	protected:

		void InitializeWindow();
		void InitializeLayersView();

		// It is called after that all windows was created
		void PostInitializeWindow();
	};
}
