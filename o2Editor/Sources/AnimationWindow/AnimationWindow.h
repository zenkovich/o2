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

		Button* mRecordButton = nullptr;
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

		// It is called when menu record button was pressed
		void OnMenuRecordPressed();
	};
}

CLASS_BASES_META(Editor::AnimationWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationWindow)
{
	PROTECTED_FIELD(mTreeViewWidth);
	PROTECTED_FIELD(mRecordButton);
	PROTECTED_FIELD(mFilterButton);
	PROTECTED_FIELD(mSearchEditBox);
}
END_META;
CLASS_METHODS_META(Editor::AnimationWindow)
{

	PUBLIC_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeUpPanel);
	PROTECTED_FUNCTION(void, OnSearchEdited, const WString&);
	PROTECTED_FUNCTION(void, OnMenuFilterPressed);
	PROTECTED_FUNCTION(void, OnMenuRecordPressed);
}
END_META;
