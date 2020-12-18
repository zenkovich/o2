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
	FIELD().NAME(mEditWidget).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mUpPanel).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mLayersButton).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mLayersPopup).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mGizomsView).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::SceneWindow)
{

	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, PostInitializeWindow);
}
END_META;
