#pragma once

#include "o2/Utils/Singleton.h"
#include "o2Editor/Core/Actions/ActionsList.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"
#include "o2/Utils/Ref.h"
#include "o2/Utils/WeakRef.h"

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
		void SetAnimation(Ref<AnimationClip> animation, Ref<AnimationPlayer> player = nullptr);

		// Sets animation editable
		void SetAnimationEditable(Ref<IEditableAnimation> editable);

		// Sets target actor
		void SetTarget(const Ref<Actor>& actor);

		// Sets curves or handles mode
		void SetCurvesMode(bool enabled);

		// Returns is curves mode enabled
		bool IsCurvesMode() const;

	protected:
		float mTreeViewWidth = 325.0f;    // Width of tree area. Changed by dragable separator
		float mMinTreeViewWidth = 250.0f; // Minimal tree width

		WeakRef<Actor>         mTargetActor;         // Target actor on animation
		Ref<AnimationPlayer> mPlayer = nullptr;    // Animation player
		bool             mOwnPlayer = false;   // Is player owned by this
		Ref<AnimationClip>   mAnimation; // Editing animation

		Ref<IEditableAnimation> mAnimationEditable = nullptr; // Editable animation holder. Deactivating when editing animation

		bool mDisableTimeTracking = false; // When true animation time changes has no effect


		Widget* mUpPanel = nullptr;  // Up panel with control buttons
		Widget* mWorkArea = nullptr; // Working area with tree and time line

		HorizontalLayout* mControlsPanel = nullptr; // Panel with buttons described below

		Ref<Toggle> mPreviewToggle = nullptr;    // Preview toggle
		Ref<Toggle> mRecordToggle = nullptr;     // Record toggle
		Ref<Button> mRewindLeft = nullptr;       // Rewind animation to start button
		Ref<Button> mMoveLeft = nullptr;         // Move time one frame left
		Ref<Toggle> mPlayPauseToggle = nullptr;  // Play - pause toggle
		Ref<Button> mMoveRight = nullptr;        // Move time one frame right
		Ref<Button> mRewindRight = nullptr;      // Rewind animation to end
		Ref<Toggle> mLoopToggle = nullptr;       // Animation loop toggle
		Ref<Toggle> mCurvesToggle = nullptr;     // Toggle curves view
		Ref<Button> mAddKeyButton = nullptr;     // Add key on current time button
		Ref<Button> mPropertiesButton = nullptr; // Open properties window

		Ref<AnimationTimeline>   mTimeline = nullptr;     // Animation time line
		Ref<HorizontalScrollBar> mTimeScroll = nullptr;   // Time line horizontal scrollbar
		Ref<AnimationTree>       mTree = nullptr;         // animation tracks tree
		Ref<KeyHandlesSheet>     mHandlesSheet = nullptr; // Animation keys handles sheet
		Ref<CurvesSheet>         mCurves = nullptr;       // Animation curves sheet

		Ref<WidgetDragHandle> mTreeSeparatorHandle = nullptr; // Tree separator handle. When it moves, it changes size of all dependent widgets

		ActionsList mActionsList; // List of actions in animation editor, also injecting into curves editor

	protected:
		// Called when editor window has closed
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

		// Called when editing animation changed. Invokes change methods in tree, curves etc
		void OnAnimationChanged();

		// Called when animation has updated
		void OnAnimationUpdate(float time);

		// Called when play/pause button was pressed
		void OnPlayPauseToggled(bool play);

		// Called when loop button was pressed
		void OnLoopToggled(bool loop);

		// Called when search edit box text was changed
		void OnSearchEdited(const WString& search);

		// Called when menu filter button was pressed
		void OnMenuFilterPressed();

		// Called when menu preview button was pressed
		void OnMenuPreviewToggle(bool value);

		// Called when menu record button was pressed
		void OnMenuRecordToggle(bool value);

		friend class AnimationTimeline;
		friend class AnimationTree;
		friend class CurvesSheet;
		friend class KeyHandlesSheet;

		template<typename AnimationTrackType>
		friend class DynamicCast;
	};

	template<typename Derived, typename Base>
	class DynamicCast
	{
	public:
		template<typename... Args>
		static Ref<Derived> Cast(Args&&... args)
		{
			return Ref<Derived>(new Derived(std::forward<Args>(args)...));
		}
	};
}#include <memory>

template <typename T>
class Ref
{
public:
    Ref(T* ptr = nullptr) : m_ptr(ptr) {}
    ~Ref() { delete m_ptr; }
    
    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }

private:
    T* m_ptr;
};

template <typename T>
Ref<T> mmake()
{
    return Ref<T>(new T());
}

template <typename T>
class WeakRef
{
public:
    WeakRef(T* ptr = nullptr) : m_ptr(ptr) {}

    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }

private:
    T* m_ptr;
};

class KeyFramesTrackControl
{
};

// --- META ---
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
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<AnimationPlayer>>()).NAME(mPlayer);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mOwnPlayer);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<AnimationClip>>()).NAME(mAnimation);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAnimationEditable);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mDisableTimeTracking);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<UpPanel>>()).NAME(mUpPanel);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<WorkArea>>()).NAME(mWorkArea);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<ControlsPanel>>()).NAME(mControlsPanel);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<PreviewToggle>>()).NAME(mPreviewToggle);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<RecordToggle>>()).NAME(mRecordToggle);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<RewindLeft>>()).NAME(mRewindLeft);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<MoveLeft>>()).NAME(mMoveLeft);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<PlayPauseToggle>>()).NAME(mPlayPauseToggle);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<MoveRight>>()).NAME(mMoveRight);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<RewindRight>>()).NAME(mRewindRight);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<LoopToggle>>()).NAME(mLoopToggle);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<CurvesToggle>>()).NAME(mCurvesToggle);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<AddKeyButton>>()).NAME(mAddKeyButton);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<PropertiesButton>>()).NAME(mPropertiesButton);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<Timeline>>()).NAME(mTimeline);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<TimeScroll>>()).NAME(mTimeScroll);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<Tree>>()).NAME(mTree);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<HandlesSheet>>()).NAME(mHandlesSheet);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<Curves>>()).NAME(mCurves);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Ref<TreeSeparatorHandle>>()).NAME(mTreeSeparatorHandle);
    FIELD().PROTECTED().NAME(mActionsList);
}
END_META;

CLASS_METHODS_META(Editor::AnimationWindow)
{
    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, SetAnimation, const Ref<AnimationClip>&, const Ref<AnimationPlayer>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetAnimationEditable, const IEditableAnimation*);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTarget, const Ref<Actor>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetCurvesMode, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsCurvesMode) const;
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
// --- END META ---