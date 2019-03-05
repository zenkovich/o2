#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"

using namespace o2;

namespace o2
{
	class Button;
	class EditBox;
}

namespace Editor
{
	class AnimationWindow : public IEditorWindow
	{
		IOBJECT(AnimationWindow);

	public:
		// Default constructor
		AnimationWindow();

		// Destructor
		~AnimationWindow();

		// Updates window logic
		void Update(float dt) override;

	protected:
		float mTreeViewWidth = 250.0f;

		Button* mFilterButton = nullptr;
		EditBox* mSearchEditBox = nullptr;

	protected:
		// Initializes window
		void InitializeWindow();

		// Initializes up control panel
		void InitializeUpPanel();

		// It is called when search edit box text was changed
		void OnSearchEdited(const WString& search);

		// It is called when menu filter button was pressed
		void OnMenuFilterPressed();
	};
}

CLASS_BASES_META(Editor::AnimationWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationWindow)
{}
END_META;
CLASS_METHODS_META(Editor::AnimationWindow)
{

	PUBLIC_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, InitializeWindow);
}
END_META;