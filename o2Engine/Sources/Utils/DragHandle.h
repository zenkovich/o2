#pragma once

#include "Events/CursorEventsListener.h"
#include "Render/IDrawable.h"
#include "Utils/Delegates.h"
#include "Utils/Property.h"
#include "Utils/Serializable.h"

namespace o2
{
	class Sprite;

	// -----------
	// Drag handle
	// -----------
	class DragHandle: public IDrawable, public CursorAreaEventsListener, public ISerializable
	{
	public:
		Function<void(const Vec2F&)> onChangedPos;   // On position changed event
		Property<Vec2F>              position;       // Current position property
		Property<bool>               enabled;        // Is handle enabled property. Disabled handle don't drawn and interact

		Function<Vec2F(const Vec2F&)> screenToLocalTransformFunc; // Screen position to local transformation function
		Function<Vec2F(const Vec2F&)> localToScreenTransformFunc; // Local position to screen transformation function
		Function<Vec2F(const Vec2F&)> checkPositionFunc;          // Position constraints checking function

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

		// Returns position
		Vec2F GetPosition() const;

		// Set handle enabled. Disabled handle don't drawn and interact
		void SetEnabled(bool enabled);

		// Returns is handle enabled. Disabled handle don't drawn and interact
		bool IsEnabled() const;

		SERIALIZABLE(DragHandle);

	protected:
		bool    mEnabled = true;           // Is handle enabled. Disabled handle don't drawn and interact

		Sprite* mRegularSprite = nullptr;  // Regular view sprite @SERIALIZABLE
		Sprite* mHoverSprite = nullptr;    // Hovered view sprite @SERIALIZABLE
		Sprite* mPressedSprite = nullptr;  // Pressed view sprite @SERIALIZABLE

		Vec2F  mPosition;                  // Current handle position, checked by checkPositionFunc
		Vec2F  mDragOffset;                // Dragging offset from cursor in local space to center
		Vec2F  mDragPosition;              // Current drag handle position
			   						       
		bool   mIsHovered = false;         // Is handle under cursor, used for hover sprite appearing
		bool   mIsPressed = false;         // Is handle pressed, used for pressed sprite appearing

		int    mPressedCursorId;           // Id of pressed cursor
		bool   mIsDragging = false;        // Is handle in dragging mode

	protected:
		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Initializes properties
		void InitializeProperties();
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
		virtual SelectableDragHandlesVec GetSelectedDragHandles() const = 0;

		// Returns all handles in group 
		virtual SelectableDragHandlesVec GetAllHandles() const = 0;

		// Selects handle
		virtual void Select(SelectableDragHandle* handle) = 0;

		// Deselects handle
		virtual void Deselect(SelectableDragHandle* handle) = 0;

		// Adds selectable handle to group
		virtual void AddSelectableHandle(SelectableDragHandle* handle) = 0;

		// Removes selectable handle from group
		virtual void RemoveSelectableHandle(SelectableDragHandle* handle) = 0;

		// Deselects all in group
		void DeselectAll();

		// Selects all in group
		void SelectAll();

	protected:
		// Calls when selectable draggable handle was pressed
		virtual void OnSelectableHandleCursorPressed(SelectableDragHandle* handle, const Input::Cursor& cursor);

		// Calls when selectable draggable handle was released
		virtual void OnSelectableHandleCursorReleased(SelectableDragHandle* handle, const Input::Cursor& cursor) {}

		// Calls when selectable handle was began to drag
		virtual void OnSelectableHandleBeganDragging(SelectableDragHandle* handle) {}

		// Calls when selectable handle moved, moves all selected handles position
		virtual void OnSelectableHandleMoved(SelectableDragHandle* handle, const Input::Cursor& cursor);

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

		SERIALIZABLE(SelectableDragHandle);

	protected:
		Sprite*                      mSelectedSprite = nullptr; // Selected view sprite @SERIALIZABLE
									 
		bool                         mIsSelected  = false;      // Is this selected
		ISelectableDragHandlesGroup* mSelectGroup = nullptr;    // Selection group

		float                        mDragDistanceThreshold = 3.0f; // Drag distance threshold: object starts dragging when cursor moves more tan this distance
		Vec2F                        mPressedCursorPos;             // Cursor pressed position

	protected:
		// Calls when cursor pressed on this
		virtual void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		virtual void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		virtual void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor released outside this(only when cursor pressed this at previous time)
		virtual void OnCursorReleasedOutside(const Input::Cursor& cursor);

		// Calls when this was selected
		virtual void OnSelected();

		// Calls when this was unselected
		virtual void OnDeselected();

		friend class ISelectableDragHandlesGroup;
	};
}
