#pragma once

#include "Events/CursorEventsListener.h"
#include "Render/IDrawable.h"
#include "Utils/Delegates.h"
#include "Utils/Property.h"
#include "Utils/Serialization/Serializable.h"

namespace o2
{
	class Sprite;

	// -----------
	// Drag handle
	// -----------
	class DragHandle: public IDrawable, public CursorAreaEventsListener, public ISerializable
	{
	public:
		PROPERTIES(DragHandle);
		PROPERTY(float, angle, SetAngle, GetAngle);          // Handle angle in radians property
		PROPERTY(Vec2F, position, SetPosition, GetPosition); // Current position property
		PROPERTY(bool, enabled, SetEnabled, IsEnabled);      // Is handle enabled property. Disabled handle don't drawn and interact

		CursorType cursorType;          // Cursor type when hovering and dragging
		bool       pixelPerfect = true; // Is handle draws pixel perfect

		Function<void(const Vec2F&)> onChangedPos;        // On position changed event
		Function<void()>             onPressed;           // Pressed cursor on handle event
		Function<void()>             onReleased;          // Released cursor event
		Function<void()>             onChangeCompleted;   // Change completed event

		Function<bool(const Vec2F&)> isPointInside; // Checking point inside function. When this is empty, using sprite inside checking function

		Function<Vec2F(const Vec2F&)> screenToLocalTransformFunc;  // Screen position to local transformation function
		Function<Vec2F(const Vec2F&)> localToScreenTransformFunc;  // Local position to screen transformation function
		Function<Vec2F(const Vec2F&)> checkPositionFunc;           // Position constraints checking function

		Function<void(const Input::Cursor&)> onRightButtonPressed;  // Right mouse button pressed event
		Function<void(const Input::Cursor&)> onRightButtonReleased; // Right mouse button released event

		// Default constructor
		DragHandle();

		// Constructor with views
		DragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr);

		// Copy-constructor
		DragHandle(const DragHandle& other);

		// Destructor
		virtual ~DragHandle();

		// Copy-operator
		DragHandle& operator=(const DragHandle& other);

		// Draws handle
		void Draw();

		// Returns true if point is above this
		bool IsUnderPoint(const Vec2F& point);

		// Sets position
		void SetPosition(const Vec2F& position);

		// Reuturns handle screen position, transformed from position with localToScreenTransformFunc
		const Vec2F& GetScreenPosition() const;

		// Sets drag position of handle, updates handle final position after position checking
		void SetDragPosition(const Vec2F& position);

		// Returns position
		Vec2F GetPosition() const;

		// Returns handle dragging offset to cursor
		Vec2F GetDraggingOffset() const;

		// Returns position at beginning of dragging
		Vec2F GetDraggingBeginPosition() const;

		// Set handle enabled. Disabled handle don't drawn and interact
		virtual void SetEnabled(bool enabled);

		// Returns is handle enabled. Disabled handle don't drawn and interact
		bool IsEnabled() const;

		// Sets handle rotation angle in radians
		virtual void SetAngle(float rad);

		// Return handle rotation angle in radians
		float GetAngle() const;

		// Sets regular sprite
		void SetRegularSprite(Sprite* sprite);

		// Returns regular sprite
		Sprite* GetRegularSprite() const;

		// Sets hovered sprite
		void SetHoverSprite(Sprite* sprite);

		// Returns hovered sprite
		Sprite* GetHoverSprite() const;

		// Sets pressed sprite
		void SetPressedSprite(Sprite* sprite);

		// Returns pressed sprite
		Sprite* GetPressedSprite() const;

		SERIALIZABLE(DragHandle);

	protected:
		bool    mEnabled = true;           // Is handle enabled. Disabled handle don't drawn and interact

		Sprite* mRegularSprite = nullptr;  // Regular view sprite @SERIALIZABLE
		Sprite* mHoverSprite = nullptr;    // Hovered view sprite @SERIALIZABLE
		Sprite* mPressedSprite = nullptr;  // Pressed view sprite @SERIALIZABLE

		Vec2F  mPosition;                  // Current handle position, checked by checkPositionFunc
		Vec2F  mScreenPosition;            // Handle screen position, transformed from mPosition with localToScreenTransformFunc
		float  mAngle = 0.0f;              // Handle rotation angle in radians
		Vec2F  mDragOffset;                // Dragging offset from cursor in local space to center
		Vec2F  mDragPosition;              // Current drag handle position
		Vec2F  mDragBeginPosition;         // Position at beginning dragging

		int    mPressedCursorId;           // Id of pressed cursor
		bool   mIsDragging = false;        // Is handle in dragging mode
		bool   mIsHovered = false;         // Is handle under cursor, used for hover sprite appearing

	protected:
		// Converts point from screen to local space
		virtual Vec2F ScreenToLocal(const Vec2F& point);

		// Converts point from local to screen space
		virtual Vec2F LocalToScreen(const Vec2F& point);

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// It is called when right mouse button was pressed on this, calls onRightButtonPressed event
		void OnCursorRightMousePressed(const Input::Cursor& cursor);

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time), calls onRightButtonReleased event
		void OnCursorRightMouseReleased(const Input::Cursor& cursor);

		// Updates screen position from position with localToScreenTransformFunc
		void UpdateScreenPosition();
	};

	class SelectableDragHandle;

	// --------------------------------------------
	// Selectable draggable handles group interface
	// --------------------------------------------
	class ISelectableDragHandlesGroup
	{
	public:
		typedef Vector<SelectableDragHandle*> SelectableDragHandlesVec;

	public:
		// Returns selected handles in group
		virtual SelectableDragHandlesVec GetSelectedHandles() const = 0;

		// Returns all handles in group 
		virtual SelectableDragHandlesVec GetAllHandles() const = 0;

		// Selects handle
		virtual void SelectHandle(SelectableDragHandle* handle) = 0;

		// Deselects handle
		virtual void DeselectHandle(SelectableDragHandle* handle) = 0;

		// Adds selectable handle to group
		virtual void AddHandle(SelectableDragHandle* handle) = 0;

		// Removes selectable handle from group
		virtual void RemoveHandle(SelectableDragHandle* handle) = 0;

		// Deselects all in group
		void DeselectAll();

		// Selects all in group
		void SelectAll();

	protected:
		// It is called when selectable draggable handle was pressed
		virtual void OnHandleCursorPressed(SelectableDragHandle* handle, const Input::Cursor& cursor) {}

		// It is called when selectable draggable handle was released
		virtual void OnHandleCursorReleased(SelectableDragHandle* handle, const Input::Cursor& cursor) {}

		// It is called when selectable handle was began to drag
		virtual void OnHandleBeganDragging(SelectableDragHandle* handle) {}

		// It is called when selectable handle moved, moves all selected handles position
		virtual void OnHandleMoved(SelectableDragHandle* handle, const Input::Cursor& cursor) {}

		// It is called when selectable handle completed changing
		virtual void OnHandleCompletedChange(SelectableDragHandle* handle) {}

		// Sets handle selected state without adding to selected handles array
		void SetHandleSelectedState(SelectableDragHandle* handle, bool selected);

		friend class SelectableDragHandle;
	};


	// ----------------------------------
	// Selectable draggable handles group
	// ----------------------------------
	class SelectableDragHandlesGroup: public ISelectableDragHandlesGroup
	{
	public:
		typedef Vector<SelectableDragHandle*> SelectableDragHandlesVec;

	public:
		// Destructor
		~SelectableDragHandlesGroup();

		// Returns selected handles in group
		SelectableDragHandlesVec GetSelectedHandles() const;

		// Returns all handles in group 
		SelectableDragHandlesVec GetAllHandles() const;

		// Selects handle
		void SelectHandle(SelectableDragHandle* handle);

		// Deselects handle
		void DeselectHandle(SelectableDragHandle* handle);

		// Adds selectable handle to group
		void AddHandle(SelectableDragHandle* handle);

		// Removes selectable handle from group
		void RemoveHandle(SelectableDragHandle* handle);

	protected:
		SelectableDragHandlesVec mSelectedHandles;
		SelectableDragHandlesVec mHandles;

	protected:
		// It is called when selectable draggable handle was pressed
		void OnHandleCursorPressed(SelectableDragHandle* handle, const Input::Cursor& cursor);

		// It is called when selectable draggable handle was released
		void OnHandleCursorReleased(SelectableDragHandle* handle, const Input::Cursor& cursor);

		// It is called when selectable handle was began to drag
		void OnHandleBeganDragging(SelectableDragHandle* handle);

		// It is called when selectable handle moved, moves all selected handles position
		void OnHandleMoved(SelectableDragHandle* handle, const Input::Cursor& cursor);

		friend class SelectableDragHandle;
	};

	// -------------------------------------
	// Selectable draggable handle interface
	// -------------------------------------
	class SelectableDragHandle: public DragHandle
	{
	public:
		// Default constructor
		SelectableDragHandle();

		// Constructor with views
		SelectableDragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr,
							 Sprite* selected = nullptr);

		// Copy-constructor
		SelectableDragHandle(const SelectableDragHandle& other);

		// Virtual destructor. Removes himself from selectable group, if need
		virtual ~SelectableDragHandle();

		// Copy-operator
		SelectableDragHandle& operator=(const SelectableDragHandle& other);

		// Returns is this selected
		bool IsSelected() const;

		// Sets is this selected
		virtual void SetSelected(bool selected);

		// Draws handle with selection
		void Draw();

		// Selects this
		void Select();

		// Deselects this
		void Deselect();

		// Sets selection group
		void SetSelectionGroup(ISelectableDragHandlesGroup* group);

		// Returns selection group
		ISelectableDragHandlesGroup* GetSelectionGroup() const;

		// Sets handle rotation angle in radians
		void SetAngle(float rad);

		SERIALIZABLE(SelectableDragHandle);

	protected:
		Sprite*                      mSelectedSprite = nullptr; // Selected view sprite @SERIALIZABLE

		bool                         mIsSelected  = false;      // Is this selected
		ISelectableDragHandlesGroup* mSelectGroup = nullptr;    // Selection group

		float                        mDragDistanceThreshold = 3.0f; // Drag distance threshold: object starts dragging when cursor moves more tan this distance
		Vec2F                        mPressedCursorPos;             // Cursor pressed position

	protected:
		// It is called when cursor pressed on this
		virtual void OnCursorPressed(const Input::Cursor& cursor);

		// It is called when cursor moved on this (or moved outside when this was pressed)
		virtual void OnCursorStillDown(const Input::Cursor& cursor);

		// It is called when cursor released (only when cursor pressed this at previous time)
		virtual void OnCursorReleased(const Input::Cursor& cursor);

		// It is called when cursor released outside this(only when cursor pressed this at previous time)
		virtual void OnCursorReleasedOutside(const Input::Cursor& cursor);

		// It is called when this was selected
		virtual void OnSelected();

		// It is called when this was unselected
		virtual void OnDeselected();

		friend class ISelectableDragHandlesGroup;
		friend class SelectableDragHandlesGroup;
	};
}

CLASS_BASES_META(o2::DragHandle)
{
	BASE_CLASS(o2::IDrawable);
	BASE_CLASS(o2::CursorAreaEventsListener);
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::DragHandle)
{
	PUBLIC_FIELD(angle);
	PUBLIC_FIELD(position);
	PUBLIC_FIELD(enabled);
	PUBLIC_FIELD(cursorType);
	PUBLIC_FIELD(pixelPerfect);
	PUBLIC_FIELD(onChangedPos);
	PUBLIC_FIELD(onPressed);
	PUBLIC_FIELD(onReleased);
	PUBLIC_FIELD(onChangeCompleted);
	PUBLIC_FIELD(isPointInside);
	PUBLIC_FIELD(screenToLocalTransformFunc);
	PUBLIC_FIELD(localToScreenTransformFunc);
	PUBLIC_FIELD(checkPositionFunc);
	PUBLIC_FIELD(onRightButtonPressed);
	PUBLIC_FIELD(onRightButtonReleased);
	PROTECTED_FIELD(mEnabled);
	PROTECTED_FIELD(mRegularSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHoverSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPressedSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPosition);
	PROTECTED_FIELD(mScreenPosition);
	PROTECTED_FIELD(mAngle);
	PROTECTED_FIELD(mDragOffset);
	PROTECTED_FIELD(mDragPosition);
	PROTECTED_FIELD(mDragBeginPosition);
	PROTECTED_FIELD(mPressedCursorId);
	PROTECTED_FIELD(mIsDragging);
	PROTECTED_FIELD(mIsHovered);
}
END_META;
CLASS_METHODS_META(o2::DragHandle)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, SetPosition, const Vec2F&);
	PUBLIC_FUNCTION(const Vec2F&, GetScreenPosition);
	PUBLIC_FUNCTION(void, SetDragPosition, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPosition);
	PUBLIC_FUNCTION(Vec2F, GetDraggingOffset);
	PUBLIC_FUNCTION(Vec2F, GetDraggingBeginPosition);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(void, SetAngle, float);
	PUBLIC_FUNCTION(float, GetAngle);
	PUBLIC_FUNCTION(void, SetRegularSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetRegularSprite);
	PUBLIC_FUNCTION(void, SetHoverSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetHoverSprite);
	PUBLIC_FUNCTION(void, SetPressedSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetPressedSprite);
	PROTECTED_FUNCTION(Vec2F, ScreenToLocal, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, LocalToScreen, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, UpdateScreenPosition);
}
END_META;

CLASS_BASES_META(o2::SelectableDragHandle)
{
	BASE_CLASS(o2::DragHandle);
}
END_META;
CLASS_FIELDS_META(o2::SelectableDragHandle)
{
	PROTECTED_FIELD(mSelectedSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIsSelected);
	PROTECTED_FIELD(mSelectGroup);
	PROTECTED_FIELD(mDragDistanceThreshold);
	PROTECTED_FIELD(mPressedCursorPos);
}
END_META;
CLASS_METHODS_META(o2::SelectableDragHandle)
{

	PUBLIC_FUNCTION(bool, IsSelected);
	PUBLIC_FUNCTION(void, SetSelected, bool);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Deselect);
	PUBLIC_FUNCTION(void, SetSelectionGroup, ISelectableDragHandlesGroup*);
	PUBLIC_FUNCTION(ISelectableDragHandlesGroup*, GetSelectionGroup);
	PUBLIC_FUNCTION(void, SetAngle, float);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleasedOutside, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnSelected);
	PROTECTED_FUNCTION(void, OnDeselected);
}
END_META;
