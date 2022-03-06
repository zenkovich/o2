#pragma once

#include "o2/Utils/Singleton.h"
#include "o2Editor/Core/Actions/ActionsList.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

using namespace o2;

namespace o2
{
	class AnimationClip;
	class Button;
	class EditBox;
	class HorizontalLayout;
	class HorizontalScrollBar;
	class Toggle;
	class WidgetDragHandle;
}

// Editor animation window accessor macros
#define o2EditorAnimationWindow AnimationWindow::Instance()

namespace Editor
{
	class AnimationTimeline;
	class AnimationTree;
	class CurvesSheet;
	class KeyHandlesSheet;

	// TODO
	// - Добавить вывод трека цвета, булевого, вектора
	// - Добавить assign акторов
	// - Добавить "запись" изменения полей
	// - Сделать редактор кривых для Vec2F

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
		void SetAnimation(AnimationClip* animation, AnimationPlayer* player = nullptr);

		// Sets animation editable
		void SetAnimationEditable(IEditableAnimation* editable);

		// Sets target actor
		void SetTarget(ActorRef actor);

		// Sets curves or handles mode
		void SetCurvesMode(bool enabled);

		// Returns is curves mode enabled
		bool IsCurvesMode() const;

	protected:
		float mTreeViewWidth = 325.0f;    // Width of tree area. Changed by dragable separator
		float mMinTreeViewWidth = 250.0f; // Minimal tree width

		ActorRef         mTargetActor;         // Target actor on animation
		AnimationPlayer* mPlayer = nullptr;    // Animation player
		bool             mOwnPlayer = false;   // Is player owned by this
		AnimationClip*   mAnimation = nullptr; // Editing animation

		IEditableAnimation* mAnimationEditable = nullptr; // Editable animation holder. Deactivating when editing animation

		bool mDisableTimeTracking = false; // When true animation time changes has no effect


		Widget* mUpPanel = nullptr;  // Up panel with control buttons
		Widget* mWorkArea = nullptr; // Working area with tree and time line

		HorizontalLayout* mControlsPanel = nullptr; // Panel with buttons described below

		Toggle* mPreviewToggle = nullptr;    // Preview toggle
		Toggle* mRecordToggle = nullptr;     // Record toggle
		Button* mRewindLeft = nullptr;       // Rewind animation to start button
		Button* mMoveLeft = nullptr;         // Move time one frame left
		Toggle* mPlayPauseToggle = nullptr;  // Play - pause toggle
		Button* mMoveRight = nullptr;        // Move time one frame right
		Button* mRewindRight = nullptr;      // Rewind animation to end
		Toggle* mLoopToggle = nullptr;       // Animation loop toggle
		Toggle* mCurvesToggle = nullptr;     // Toggle curves view
		Button* mAddKeyButton = nullptr;     // Add key on current time button
		Button* mPropertiesButton = nullptr; // Open properties window

		AnimationTimeline*   mTimeline = nullptr;     // Animation time line
		HorizontalScrollBar* mTimeScroll = nullptr;   // Time line horizontal scrollbar
		AnimationTree*       mTree = nullptr;         // animation tracks tree
		KeyHandlesSheet*     mHandlesSheet = nullptr; // Animation keys handles sheet
		CurvesSheet*         mCurves = nullptr;       // Animation curves sheet

		WidgetDragHandle* mTreeSeparatorHandle = nullptr; // Tree separator handle. When it moves, it changes size of all dependent widgets

		ActionsList mActionsList; // List of actions in animation editor, also injecting into curves editor

	protected:
		// It is called when editor window has closed
		void OnClosed() override;

		// Initializes window
		void InitializeWindow();

		// Initializes handles sheet
		void InitializeHandlesSheet();

		// Initializes nodes tree
		void InitializeTree();

		// Initializes timeline and scrollbar
		void InitializeTimeline();

		// Initializes curves sheet widget
		void InitializeCurvesSheet();

		// Initializes up control panel
		void InitializeUpPanel();

		// Initializes separator handle view and events
		void InitializeSeparatorHandle();

		// It is called when editing animation changed. Invokes change methods in tree, curves etc
		void OnAnimationChanged();

		// It is called when animation has updated
		void OnAnimationUpdate(float time);

		// It is called when play/pause button was pressed
		void OnPlayPauseToggled(bool play);

		// It is called when loop button was pressed
		void OnLoopToggled(bool loop);

		// It is called when search edit box text was changed
		void OnSearchEdited(const WString& search);

		// It is called when menu filter button was pressed
		void OnMenuFilterPressed();

		// It is called when menu preview button was pressed
		void OnMenuPreviewToggle(bool value);

		// It is called when menu record button was pressed
		void OnMenuRecordToggle(bool value);

		friend class AnimationTimeline;
		friend class AnimationTree;
		friend class CurvesSheet;
		friend class KeyHandlesSheet;

		template<typename AnimationTrackType>
		friend class KeyFramesTrackControl;
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
	FIELD().PROTECTED().DEFAULT_VALUE(325.0f).NAME(mTreeViewWidth);
	FIELD().PROTECTED().DEFAULT_VALUE(250.0f).NAME(mMinTreeViewWidth);
	FIELD().PROTECTED().NAME(mTargetActor);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPlayer);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mOwnPlayer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAnimation);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAnimationEditable);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mDisableTimeTracking);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mUpPanel);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mWorkArea);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mControlsPanel);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPreviewToggle);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRecordToggle);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRewindLeft);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mMoveLeft);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPlayPauseToggle);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mMoveRight);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRewindRight);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLoopToggle);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mCurvesToggle);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAddKeyButton);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPropertiesButton);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTimeline);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTimeScroll);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTree);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHandlesSheet);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mCurves);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTreeSeparatorHandle);
	FIELD().PROTECTED().NAME(mActionsList);
}
END_META;
CLASS_METHODS_META(Editor::AnimationWindow)
{

	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAnimation, AnimationClip*, AnimationPlayer*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAnimationEditable, IEditableAnimation*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTarget, ActorRef);
	FUNCTION().PUBLIC().SIGNATURE(void, SetCurvesMode, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsCurvesMode);
	FUNCTION().PROTECTED().SIGNATURE(void, OnClosed);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindow);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeHandlesSheet);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeTree);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeTimeline);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeCurvesSheet);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeUpPanel);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeSeparatorHandle);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAnimationChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAnimationUpdate, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnPlayPauseToggled, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnLoopToggled, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSearchEdited, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnMenuFilterPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnMenuPreviewToggle, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnMenuRecordToggle, bool);
}
END_META;
