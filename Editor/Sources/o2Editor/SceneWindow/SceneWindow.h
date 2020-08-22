#pragma once

#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

namespace o2
{
	class PopupWidget;
}

namespace Editor
{
	class LayersPopup;
	class SceneEditScreen;

	// --------------------
	// Scene editing window
	// --------------------
	class SceneWindow: public IEditorWindow
	{
	public:
		IOBJECT(SceneWindow);

	protected:
		SceneEditScreen* mEditWidget;

		Widget* mUpPanel = nullptr;

		Button*      mLayersButton = nullptr;
		LayersPopup* mLayersPopup = nullptr;

		PopupWidget* mGizomsView = nullptr;

	public:
		SceneWindow();
		SceneWindow(const SceneWindow& other);
		~SceneWindow();

	protected:
		// Initializes window and controls
		void InitializeWindow();

		// It is called after that all windows was created
		void PostInitializeWindow() override;
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
	PROTECTED_FIELD(mUpPanel).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mLayersButton).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mLayersPopup).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mGizomsView).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(Editor::SceneWindow)
{

	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, PostInitializeWindow);
}
END_META;
