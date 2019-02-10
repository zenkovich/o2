#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"


namespace o2
{
	class Widget;
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
		Widget*        mLayersView;
		Widget*        mGizomsView;

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

CLASS_BASES_META(Editor::SceneWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
}
END_META;
CLASS_FIELDS_META(Editor::SceneWindow)
{
	PROTECTED_FIELD(mEditWidget);
	PROTECTED_FIELD(mLayersView);
	PROTECTED_FIELD(mGizomsView);
}
END_META;
CLASS_METHODS_META(Editor::SceneWindow)
{

	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeLayersView);
	PROTECTED_FUNCTION(void, PostInitializeWindow);
}
END_META;
