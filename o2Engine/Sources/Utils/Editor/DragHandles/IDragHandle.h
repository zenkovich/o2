#pragma once

#include "Application/Input.h"
#include "Utils/Delegates.h"
#include "Utils/Property.h"
#include "Utils/Serialization/Serializable.h"

namespace o2
{
	// ---------------------
	// Drag handle interface
	// ---------------------
	class IDragHandle
	{
	public:
		PROPERTIES(IDragHandle);
		PROPERTY(Vec2F, position, SetPosition, GetPosition); // Current position property

		CursorType  cursorType = CursorType::Arrow; // Cursor type when hovering and dragging

		KeyboardKey snappingKey = VK_CONTROL;       // Snapping key, when it pressed handle uses checkSnappingFunc to correct position

		Function<void(const Vec2F&)> onChangedPos;        // On position changed event
		Function<void()>             onPressed;           // Pressed cursor on handle event
		Function<void()>             onReleased;          // Released cursor event
		Function<void()>             onChangeCompleted;   // Change completed event

		Function<bool(const Vec2F&)> isPointInside; // Checking point inside function. When this is empty, using sprite inside checking function

		Function<Vec2F(const Vec2F&)> checkPositionFunc;           // Position constraints checking function
		Function<Vec2F(const Vec2F&)> checkSnappingFunc;           // Checking snapping positiong when dragging with pressed snapping key

		Function<void(const Input::Cursor&)> onRightButtonPressed;  // Right mouse button pressed event
		Function<void(const Input::Cursor&)> onRightButtonReleased; // Right mouse button released event

		// Default constructor
		IDragHandle();

		// Copy-constructor
		IDragHandle(const IDragHandle& other);

		// Destructor
		virtual ~IDragHandle();

		// Copy-operator
		IDragHandle& operator=(const IDragHandle& other);

		// Sets position
		void SetPosition(const Vec2F& position);

		// Sets drag position of handle, updates handle final position after position checking
		void SetDragPosition(const Vec2F& position);

		// Returns position
		Vec2F GetPosition() const;

		// Returns handle dragging offset to cursor
		Vec2F GetDraggingOffset() const;

		// Returns position at beginning of dragging
		Vec2F GetDraggingBeginPosition() const;

		SERIALIZABLE(IDragHandle);

	protected:
		Vec2F  mPosition;                  // Current handle position, checked by checkPositionFunc
		Vec2F  mDragOffset;                // Dragging offset from cursor in local space to center
		Vec2F  mDragPosition;              // Current drag handle position
		Vec2F  mDragBeginPosition;         // Position at beginning dragging

		int    mPressedCursorId;           // Id of pressed cursor
		bool   mIsDragging = false;        // Is handle in dragging mode
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
	PUBLIC_FIELD(snappingKey);
	PUBLIC_FIELD(onChangedPos);
	PUBLIC_FIELD(onPressed);
	PUBLIC_FIELD(onReleased);
	PUBLIC_FIELD(onChangeCompleted);
	PUBLIC_FIELD(isPointInside);
	PUBLIC_FIELD(screenToLocalTransformFunc);
	PUBLIC_FIELD(localToScreenTransformFunc);
	PUBLIC_FIELD(checkPositionFunc);
	PUBLIC_FIELD(checkSnappingFunc);
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
