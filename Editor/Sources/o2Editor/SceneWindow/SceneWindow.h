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

		// Called after that all windows was created
		void PostInitializeWindow() override;
	};
}
// --- META ---

CLASS_BASES_META(Editor::SceneWindow)
{
    BASE_CLASS(Editor::IEditorWindow);
}
END_META;
CLASS_FIELDS_META(Editor::SceneWindow)
{
    FIELD().PROTECTED().NAME(mEditWidget);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mUpPanel);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLayersButton);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLayersPopup);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mGizomsView);
}
END_META;
CLASS_METHODS_META(Editor::SceneWindow)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const SceneWindow&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindow);
    FUNCTION().PROTECTED().SIGNATURE(void, PostInitializeWindow);
}
END_META;
// --- END META ---
