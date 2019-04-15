#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"
#include "Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class Button;
	class EditBox;
	class HorizontalScrollBar;
	class Toggle;
	class WidgetDragHandle;

	class Animation;
}

// Editor animation window accessor macros
#define o2EditorAnimationWindow AnimationWindow::Instance()

namespace Editor
{
	class AnimationTimeline;
	class AnimationTree;

	class AnimationWindow : public IEditorWindow, public Singleton<AnimationWindow>
	{
		IOBJECT(AnimationWindow);

	public:
		// Default constructor
		AnimationWindow();

		// Destructor
		~AnimationWindow();


		// Updates window logic
		void Update(float dt) override;


		// Sets editing animation
		void SetAnimation(Animation* animation);

	protected:
		float mTreeViewWidth = 250.0f;

		Animation* mAnimation = nullptr;

		Widget* mUpPanel = nullptr;
		Widget* mWorkArea = nullptr;

		Widget*  mControlsPanel = nullptr;
		Toggle*  mRecordToggle = nullptr;
		Toggle*  mPlayPauseToggle = nullptr;
		Toggle*  mLoopToggle = nullptr;
		Button*  mAddKeyButton = nullptr;
		Button*  mFilterButton = nullptr;
		EditBox* mSearchEditBox = nullptr;

		AnimationTimeline* mTimeline = nullptr;
		HorizontalScrollBar* mTimeScroll = nullptr;

		AnimationTree* mTree = nullptr;

		WidgetDragHandle* mTreeSeparatorHandle = nullptr;

	protected:
		// Initializes window
		void InitializeWindow();

		// Initializes up control panel
		void InitializeUpPanel();

		// Initializes separator handle view and events
		void InitializeSeparatorHandle();

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
	BASE_CLASS(o2::Singleton<AnimationWindow>);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationWindow)
{
	PROTECTED_FIELD(mTreeViewWidth);
	PROTECTED_FIELD(mAnimation);
	PROTECTED_FIELD(mUpPanel);
	PROTECTED_FIELD(mWorkArea);
	PROTECTED_FIELD(mControlsPanel);
	PROTECTED_FIELD(mRecordToggle);
	PROTECTED_FIELD(mPlayPauseToggle);
	PROTECTED_FIELD(mLoopToggle);
	PROTECTED_FIELD(mAddKeyButton);
	PROTECTED_FIELD(mFilterButton);
	PROTECTED_FIELD(mSearchEditBox);
	PROTECTED_FIELD(mTimeline);
	PROTECTED_FIELD(mTimeScroll);
	PROTECTED_FIELD(mTree);
	PROTECTED_FIELD(mTreeSeparatorHandle);
}
END_META;
CLASS_METHODS_META(Editor::AnimationWindow)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetAnimation, Animation*);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeUpPanel);
	PROTECTED_FUNCTION(void, InitializeSeparatorHandle);
	PROTECTED_FUNCTION(void, OnSearchEdited, const WString&);
	PROTECTED_FUNCTION(void, OnMenuFilterPressed);
	PROTECTED_FUNCTION(void, OnMenuRecordPressed);
}
END_META;
