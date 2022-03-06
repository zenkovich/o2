#pragma once

#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2Editor/AnimationWindow/TrackControls/AnimationKeyDragHandle.h"

namespace o2
{
	class AnimationClip;
	class ContextMenu;
	class Sprite;
}

using namespace o2;

namespace Editor
{
	class AnimationWindow;
	class ITrackControl;

	// -------------------------------------------
	// Handles sheet, manages selection of handles
	// -------------------------------------------
	class KeyHandlesSheet : public Widget, public SelectableDragHandlesGroup, public CursorAreaEventsListener
	{
	public:
		// Default constructor
		KeyHandlesSheet();

		// Copy-constructor
		KeyHandlesSheet(const KeyHandlesSheet& other);

		// Destructor
		~KeyHandlesSheet();

		// Copy-operator
		KeyHandlesSheet& operator=(const KeyHandlesSheet& other);

		// Sets animation. Used for batch change of keys
		void SetAnimation(AnimationClip* animation);

		// Updates selection frame
		void Update(float dt) override;

		// Draws selection
		void Draw() override;

		// Updates draw order for correct handles and sheet input processing
		void UpdateInputDrawOrder();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Registers animation track track control
		void RegTrackControl(ITrackControl* trackControl, const std::string& path);

		// Unregisters animation track track control
		void UnregTrackControl(ITrackControl* trackControl);

		// Unregisters all tracks controls
		void UnregAllTrackControls();

		// Adds selectable handle to group
		void AddHandle(DragHandle* handle) override;

		// Removes selectable handle from group
		void RemoveHandle(DragHandle* handle) override;

		// Sets selected keys handles
		void SetSelectedKeys(const Map<String, Vector<UInt64>>& keys);

		// Returns list of selected keys handles
		Map<String, Vector<UInt64>> GetSelectedKeys() const;

		// Removes keys
		void DeleteKeys(const Map<String, Vector<UInt64>>& keys, bool createAction = true);

		// Returns context menu
		ContextMenu* GetContextMenu() const;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(KeyHandlesSheet);

	private:
		RectF mSelectionFrameOffsets = RectF(-9, -3, 5, 2);
		RectF mSelectionFrameCursorOffsets = RectF(-2, -3, 2, 2);

		AnimationWindow* mAnimationWindow = nullptr; // Animation window

		Vector<ITrackControl*>               mTrackControls;    // List of actual track controls
		Vector<Pair<String, ITrackControl*>> mTrackControlsMap; // Map of actual track controls, key is Animation track path

		Map<IAnimationTrack*, Vector<AnimationKeyDragHandle*>> mHandlesGroups; // All handles grouped by Animation track, used for fast searching handles for same Animation track

		ContextMenu* mContextMenu = nullptr; // Keys context menu
		Vec2F        mContextMenuPressPoint; // Cursor position when right button were clicked. When trying to show context menu checking delta between current cursor position and this 

		bool mNeedUpdateSelectionFrame = false; // True when selection frame required to update

		bool mHandleHasMoved = false; // it is true when some handle was selected and moved, resets on handle pressing

		Sprite* mSelectionFrame = nullptr; // Selected handles frame drawing sprite
		RectF   mSelectionRect;            // Current selected handles rectangle. The right and left is minimum and maximum handles positions, top and bottom is minimum and maximum handles lines

		Vec2F   mBeginSelectPoint;         // Begin frame selection point, where x is position on timeline, y is line number
		bool    mIsFrameSelecting = false; // It is true when user selection by frame now

		Vector<DragHandle*> mBeginSelectHandles; // handles list, that were selected before frame selecting

		DragHandle mLeftFrameDragHandle;   // Left frame border drag handle, resizing selected handles rect
		DragHandle mRightFrameDragHandle;  // Right frame border drag handle, resizing selected handles rect
		DragHandle mCenterFrameDragHandle; // Center frame drag handle, moves selected handles

		DataDocument mBeforeChangeKeysData; // Serialized keys data before change

	private:
		// Initializes frame handles
		void InitializeHandles();

		// Initializes center handle, that moves selected keys on timeline
		void InitializeCenterHandle();

		// Initializes left handle, that moves selected keys on timeline relative to right selection rect position
		void InitializeLeftHandle();

		// Initializes right handle, that moves selected keys on timeline relative to left selection rect position
		void InitializeRightHandle();

		// Initializes context menu with copy, paste, delete etc
		void InitializeContextMenu();

		// Updates selection rectangle and drawing sprite
		void UpdateSelectionFrame();

		// Serialized keys into data
		void SerializeKeys(DataValue& data, const Map<String, Vector<UInt64>>& keys, float relativeTime);

		// Deserializes keys from data 
		void DeserializeKeys(const DataValue& data, Map<String, Vector<UInt64>>& keys, float relativeTime, bool generateNewUid = true);

		// Copies selected keys into buffer
		void CopyKeys();

		// Inserts keys from buffer under cursor
		void PasteKeys();

		// It is called when selection is changed - some handle was added or removed from selection
		// Updating selection frame
		void OnSelectionChanged() override;

		// It is called when selectable draggable handle was pressed, sends to track control that drag has began
		void OnHandleCursorPressed(DragHandle* handle, const Input::Cursor& cursor) override;

		// It is called when selectable draggable handle was released, sends to track control that drag has completed
		void OnHandleCursorReleased(DragHandle* handle, const Input::Cursor& cursor) override;

		// It is called when selectable handle moved, moves all selected handles position
		// Enables keys batch change
		void OnHandleMoved(DragHandle* handle, const Vec2F& cursorPos) override;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor pressed outside this
		void OnCursorPressedOutside(const Input::Cursor& cursor) override;

		// It is called when cursor released outside this(only when cursor pressed this at previous time)
		void OnCursorReleasedOutside(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when cursor double clicked
		void OnCursorDblClicked(const Input::Cursor& cursor) override;

		// It is called when right mouse button was pressed on this
		void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

		// It is called when right mouse button stay down on this
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// It is called when middle mouse button was pressed on this
		void OnCursorMiddleMousePressed(const Input::Cursor& cursor) override;

		// It is called when middle mouse button stay down on this
		void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor) override;

		// It is called when middle mouse button was released (only when middle mouse button pressed this at previous time)
		void OnCursorMiddleMouseReleased(const Input::Cursor& cursor) override;

		friend class AnimationWindow;
		friend class AnimationAddKeysAction;
		friend class AnimationDeleteKeysAction;
		friend class AnimationKeysChangeAction;

		template<typename AnimationTrackType>
		friend class KeyFramesTrackControl;
	};
}

CLASS_BASES_META(Editor::KeyHandlesSheet)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::SelectableDragHandlesGroup);
	BASE_CLASS(o2::CursorAreaEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::KeyHandlesSheet)
{
	FIELD().PRIVATE().DEFAULT_VALUE(RectF(-9, -3, 5, 2)).NAME(mSelectionFrameOffsets);
	FIELD().PRIVATE().DEFAULT_VALUE(RectF(-2, -3, 2, 2)).NAME(mSelectionFrameCursorOffsets);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mAnimationWindow);
	FIELD().PRIVATE().NAME(mTrackControls);
	FIELD().PRIVATE().NAME(mTrackControlsMap);
	FIELD().PRIVATE().NAME(mHandlesGroups);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mContextMenu);
	FIELD().PRIVATE().NAME(mContextMenuPressPoint);
	FIELD().PRIVATE().DEFAULT_VALUE(false).NAME(mNeedUpdateSelectionFrame);
	FIELD().PRIVATE().DEFAULT_VALUE(false).NAME(mHandleHasMoved);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mSelectionFrame);
	FIELD().PRIVATE().NAME(mSelectionRect);
	FIELD().PRIVATE().NAME(mBeginSelectPoint);
	FIELD().PRIVATE().DEFAULT_VALUE(false).NAME(mIsFrameSelecting);
	FIELD().PRIVATE().NAME(mBeginSelectHandles);
	FIELD().PRIVATE().NAME(mLeftFrameDragHandle);
	FIELD().PRIVATE().NAME(mRightFrameDragHandle);
	FIELD().PRIVATE().NAME(mCenterFrameDragHandle);
	FIELD().PRIVATE().NAME(mBeforeChangeKeysData);
}
END_META;
CLASS_METHODS_META(Editor::KeyHandlesSheet)
{

	typedef const Map<String, Vector<UInt64>>& _tmp1;
	typedef Map<String, Vector<UInt64>> _tmp2;
	typedef const Map<String, Vector<UInt64>>& _tmp3;
	typedef const Map<String, Vector<UInt64>>& _tmp4;
	typedef Map<String, Vector<UInt64>>& _tmp5;

	FUNCTION().PUBLIC().SIGNATURE(void, SetAnimation, AnimationClip*);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateInputDrawOrder);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(void, RegTrackControl, ITrackControl*, const std::string&);
	FUNCTION().PUBLIC().SIGNATURE(void, UnregTrackControl, ITrackControl*);
	FUNCTION().PUBLIC().SIGNATURE(void, UnregAllTrackControls);
	FUNCTION().PUBLIC().SIGNATURE(void, AddHandle, DragHandle*);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveHandle, DragHandle*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSelectedKeys, _tmp1);
	FUNCTION().PUBLIC().SIGNATURE(_tmp2, GetSelectedKeys);
	FUNCTION().PUBLIC().SIGNATURE(void, DeleteKeys, _tmp3, bool);
	FUNCTION().PUBLIC().SIGNATURE(ContextMenu*, GetContextMenu);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeHandles);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeCenterHandle);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeLeftHandle);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeRightHandle);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeContextMenu);
	FUNCTION().PRIVATE().SIGNATURE(void, UpdateSelectionFrame);
	FUNCTION().PRIVATE().SIGNATURE(void, SerializeKeys, DataValue&, _tmp4, float);
	FUNCTION().PRIVATE().SIGNATURE(void, DeserializeKeys, const DataValue&, _tmp5, float, bool);
	FUNCTION().PRIVATE().SIGNATURE(void, CopyKeys);
	FUNCTION().PRIVATE().SIGNATURE(void, PasteKeys);
	FUNCTION().PRIVATE().SIGNATURE(void, OnSelectionChanged);
	FUNCTION().PRIVATE().SIGNATURE(void, OnHandleCursorPressed, DragHandle*, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnHandleCursorReleased, DragHandle*, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnHandleMoved, DragHandle*, const Vec2F&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorPressBreak, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorPressedOutside, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorReleasedOutside, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorEnter, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorExit, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorDblClicked, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorRightMousePressed, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorRightMouseReleased, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorMiddleMousePressed, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorMiddleMouseStayDown, const Input::Cursor&);
	FUNCTION().PRIVATE().SIGNATURE(void, OnCursorMiddleMouseReleased, const Input::Cursor&);
}
END_META;
