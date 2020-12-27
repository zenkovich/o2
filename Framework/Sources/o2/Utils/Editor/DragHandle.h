#pragma once

#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Render/IDrawable.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Function.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class Sprite;
	class ISelectableDragHandlesGroup;

	// -----------
	// Drag handle
	// -----------
	class DragHandle: public IDrawable, public CursorAreaEventsListener, virtual public ISerializable
	{
	public:
		PROPERTIES(DragHandle);
		PROPERTY(float, angle, SetAngle, GetAngle);          // Handle angle in radians property
		PROPERTY(Vec2F, position, SetPosition, GetPosition); // Current position property
		PROPERTY(bool, enabled, SetEnabled, IsEnabled);      // Is handle enabled property. Disabled handle don't drawn and interact
		PROPERTY(bool, selected, SetSelected, IsSelected);   // Is handle selected property

	public:
		CursorType  cursorType = CursorType::Arrow; // Cursor type when hovering and dragging
		bool        pixelPerfect = true;            // Is handle draws pixel perfect
		KeyboardKey snappingKey = VK_CONTROL;       // Snapping key, when it pressed handle uses checkSnappingFunc to correct position

	public:
		Function<void(const Vec2F&)> onChangedPos;      // On position changed event
		Function<void()>             onPressed;         // Pressed cursor on handle event
		Function<void()>             onReleased;        // Released cursor event
		Function<void()>             onBeganDragging;   // Drag begin event
		Function<void()>             onChangeCompleted; // Change completed event

		Function<bool(const Vec2F&)> isPointInside; // Checking point inside function. When this is empty, using sprite inside checking function

		Function<Vec2F(const Vec2F&)> screenToLocalTransformFunc; // Screen position to local transformation function
		Function<Vec2F(const Vec2F&)> localToScreenTransformFunc; // Local position to screen transformation function
		Function<Vec2F(const Vec2F&)> checkPositionFunc;          // Position constraints checking function
		Function<Vec2F(const Vec2F&)> checkSnappingFunc;          // Checking snapping position when dragging with pressed snapping key

		Function<void(const Input::Cursor&)> onRightButtonPressed;  // Right mouse button pressed event
		Function<void(const Input::Cursor&)> onRightButtonReleased; // Right mouse button released event

	public:
		// Default constructor
		DragHandle();

		// Constructor with views
		DragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr,
				   Sprite* selected = nullptr, Sprite* selectedHovered = nullptr, Sprite* selectedPressed = nullptr);

		// Copy-constructor
		DragHandle(const DragHandle& other);

		// Destructor
		virtual ~DragHandle();

		// Copy-operator
		DragHandle& operator=(const DragHandle& other);

		// Draws handle
		void Draw();

		// Draws handle with clipping check
		void Draw(const RectF& screenClipRect);

		// Returns true if point is above this
		bool IsUnderPoint(const Vec2F& point);

		// Sets position
		void SetPosition(const Vec2F& position);

		// Returns handle screen position, transformed from position with localToScreenTransformFunc
		const Vec2F& GetScreenPosition() const;

		// Updates screen position from position with localToScreenTransformFunc
		virtual void UpdateScreenPosition();

		// Sets drag position of handle, updates handle final position after position checking
		void SetDragPosition(const Vec2F& position);

		// Returns drag position
		Vec2F GetDragPosition() const;

		// Returns position
		Vec2F GetPosition() const;

		// Returns handle dragging offset to cursor
		Vec2F GetDraggingOffset() const;

		// Returns position at beginning of dragging
		Vec2F GetDraggingBeginPosition() const;

		// It is called when handle beginning to drag from outside, for example from selection group. Updates drag position and drag offset
		void BeginDrag(const Vec2F& cursor);

		// Sets is this selected
		virtual void SetSelected(bool selected);

		// Returns is this selected
		bool IsSelected() const;

		// Selects this
		void Select();

		// Deselects this
		void Deselect();

		// Sets selection group
		void SetSelectionGroup(ISelectableDragHandlesGroup* group);

		// Returns selection group
		ISelectableDragHandlesGroup* GetSelectionGroup() const;

		// Set handle enabled. Disabled handle don't drawn and interact
		virtual void SetEnabled(bool enabled);

		// Returns is handle enabled. Disabled handle don't drawn and interact
		bool IsEnabled() const;

		// Sets handle rotation angle in radians
		virtual void SetAngle(float rad);

		// Return handle rotation angle in radians
		float GetAngle() const;

		// Converts point from screen to local space
		virtual Vec2F ScreenToLocal(const Vec2F& point);

		// Converts point from local to screen space
		virtual Vec2F LocalToScreen(const Vec2F& point);

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

		// Sets selected sprite
		void SetSelectedSprite(Sprite* sprite);

		// Returns selected sprite
		Sprite* GetSelectedSprite() const;

		// Sets selected hovered sprite
		void SetSelectedHoveredSprite(Sprite* sprite);

		// Returns selected hovered sprite
		Sprite* GetSelectedHoveredSprite() const;

		// Sets selected pressed sprite
		void SetSelectedPressedSprite(Sprite* sprite);

		// Returns selected pressed sprite
		Sprite* GetSelectedPressedSprite() const;

		// Sets size to all available sprites
		void SetSpritesSize(const Vec2F& size);

		// Sets color to all sprites
		void SetSpritesColor(const Color4& color);

		// Sets size to all available sprites
		void SetSpritesSizePivot(const Vec2F& pivot);

		// Returns is listener scrollable
		bool IsScrollable() const override;

		SERIALIZABLE(DragHandle);

	protected:
		Sprite* mRegularSprite = nullptr;  // Regular view sprite @SERIALIZABLE
		Sprite* mHoverSprite = nullptr;    // Hovered view sprite @SERIALIZABLE
		Sprite* mPressedSprite = nullptr;  // Pressed view sprite @SERIALIZABLE

		Sprite* mSelectedSprite = nullptr;        // Selected view sprite @SERIALIZABLE
		Sprite* mSelectedHoverSprite = nullptr;   // Selected hovered view sprite @SERIALIZABLE
		Sprite* mSelectedPressedSprite = nullptr; // Selected pressed view sprite @SERIALIZABLE

		bool   mEnabled = true; // Is handle enabled. Disabled handle don't drawn and interact

		Vec2F  mPosition;                      // Current handle position, checked by checkPositionFunc
		Vec2F  mScreenPosition;                // Handle screen position, transformed from mPosition with localToScreenTransformFunc
		Vec2F  mLastDrawnScreenPosition;       // Handle screen position on last draw
		int    mLastScreenPosUpdateFrame = -1; // Last screen position update frame index
		float  mAngle = 0.0f;                  // Handle rotation angle in radians

		Vec2F  mDragOffset;        // Dragging offset from cursor in local space to center
		Vec2F  mDragPosition;      // Current drag handle position
		Vec2F  mDragBeginPosition; // Position at beginning dragging

		int    mPressedCursorId;    // Id of pressed cursor
		Vec2F  mPressedCursorPos;   // Cursor pressed position
		bool   mIsDragging = false; // Is handle in dragging mode
		bool   mIsHovered = false;  // Is handle under cursor, used for hover sprite appearing

		bool                         mIsSelected = false;    // Is this selected
		ISelectableDragHandlesGroup* mSelectGroup = nullptr; // Selection group

		float  mDragDistanceThreshold = 3.0f; // Drag distance threshold: object starts dragging when cursor moves more tan this distance

	protected:
		// Draws internal sprites with calculated screen position
		void DrawInternal();

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor released outside this(only when cursor pressed this at previous time)
		void OnCursorReleasedOutside(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when right mouse button was pressed on this, calls onRightButtonPressed event
		void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time), calls onRightButtonReleased event
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// Updates sprites positions by handle screen position
		virtual void UpdateSpritesPositions();

		// It is called when this was selected
		virtual void OnSelected();

		// It is called when this was unselected
		virtual void OnDeselected();

		friend class ISelectableDragHandlesGroup;
		friend class SelectableDragHandlesGroup;
	};

	// ---------------------------------------------------------
	// Widget drag handle. Can be attached to widget as children
	// ---------------------------------------------------------
	class WidgetDragHandle: public DragHandle, public Widget
	{
	public:
		Function<Vec2F(const Vec2F&)> widgetOffsetToLocalTransformFunc; // Widget offset relative to parent to local transformation function
		Function<Vec2F(const Vec2F&)> localToWidgetOffsetTransformFunc; // Local position to widget offset relative to parent transformation function

		Function<void()> onLayoutUpdated; // It is called when this layout were updated

	public:
		// Default constructor
		WidgetDragHandle();

		// Constructor with views
		WidgetDragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr,
						 Sprite* selected = nullptr, Sprite* selectedHovered = nullptr,
						 Sprite* selectedPressed = nullptr);

		// Copy-constructor
		WidgetDragHandle(const WidgetDragHandle& other);

		// Destructor
		~WidgetDragHandle();

		// Copy-operator
		WidgetDragHandle& operator=(const WidgetDragHandle& other);

		// Draws handle
		void Draw() override;

		// Set handle enabled. Disabled handle don't drawn and interact
		void SetEnabled(bool enabled);

		// Returns is handle enabled. Disabled handle don't drawn and interact
		bool IsEnabled() const;

		// Converts point from screen to local space
		Vec2F ScreenToLocal(const Vec2F& point) override;

		// Converts point from local to screen space
		Vec2F LocalToScreen(const Vec2F& point) override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(WidgetDragHandle);

	protected:
		// Hide public functions
		using DragHandle::screenToLocalTransformFunc;
		using DragHandle::localToScreenTransformFunc;

		// Updates layers layouts, calls after updating widget layout
		void UpdateLayersLayouts() override;

		// Beginning serialization callback
		void OnSerialize(DataValue& node) const override;

		// It is called when deserialized
		void OnDeserialized(const DataValue& node) override;
	};

	// --------------------------------------------
	// Selectable draggable handles group interface
	// --------------------------------------------
	class ISelectableDragHandlesGroup
	{
	public:
		// Returns selected handles in group
		virtual Vector<DragHandle*> GetSelectedHandles() const = 0;

		// Returns all handles in group 
		virtual Vector<DragHandle*> GetAllHandles() const = 0;

		// Selects handle
		virtual void SelectHandle(DragHandle* handle) = 0;

		// Deselects handle
		virtual void DeselectHandle(DragHandle* handle) = 0;

		// Adds selectable handle to group
		virtual void AddHandle(DragHandle* handle) = 0;

		// Removes selectable handle from group
		virtual void RemoveHandle(DragHandle* handle) = 0;

		// Deselects all in group
		virtual void DeselectAll();

		// Selects all in group
		virtual void SelectAll();

	protected:
		// It is called when selectable draggable handle was pressed
		virtual void OnHandleCursorPressed(DragHandle* handle, const Input::Cursor& cursor) { }

		// It is called when selectable draggable handle was released
		virtual void OnHandleCursorReleased(DragHandle* handle, const Input::Cursor& cursor) { }

		// It is called when selectable handle was began to drag
		virtual void OnHandleBeganDragging(DragHandle* handle) { }

		// It is called when selectable handle moved, moves all selected handles position
		virtual void OnHandleMoved(DragHandle* handle, const Vec2F& cursorPos) { }

		// It is called when selectable handle completed changing
		virtual void OnHandleCompletedChange(DragHandle* handle) { }

		// Sets handle selected state without adding to selected handles array
		void SetHandleSelectedState(DragHandle* handle, bool selected);

		friend class DragHandle;
	};


	// ----------------------------------
	// Selectable draggable handles group
	// ----------------------------------
	class SelectableDragHandlesGroup: public ISelectableDragHandlesGroup
	{
	public:
		// Destructor
		~SelectableDragHandlesGroup();

		// Returns selected handles in group
		Vector<DragHandle*> GetSelectedHandles() const;

		// Returns all handles in group 
		Vector<DragHandle*> GetAllHandles() const;

		// Selects handle
		void SelectHandle(DragHandle* handle) override;

		// Deselects handle
		void DeselectHandle(DragHandle* handle) override;

		// Adds selectable handle to group
		void AddHandle(DragHandle* handle) override;

		// Removes selectable handle from group
		void RemoveHandle(DragHandle* handle) override;

		// Deselects all in group
		void DeselectAll() override;

		// Selects all in group
		void SelectAll() override;

	protected:
		Vector<DragHandle*> mSelectedHandles;
		Vector<DragHandle*> mHandles;

	protected:
		// It is called when selection is changed - some handle was added or removed from selection
		virtual void OnSelectionChanged();

		// It is called when selectable draggable handle was pressed
		void OnHandleCursorPressed(DragHandle* handle, const Input::Cursor& cursor) override;

		// It is called when selectable draggable handle was released
		void OnHandleCursorReleased(DragHandle* handle, const Input::Cursor& cursor) override;

		// It is called when selectable handle was began to drag
		void OnHandleBeganDragging(DragHandle* handle) override;

		// It is called when selectable handle moved, moves all selected handles position
		void OnHandleMoved(DragHandle* handle, const Vec2F& cursorPos) override;

		friend class DragHandle;
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
	FIELD().NAME(angle).PUBLIC();
	FIELD().NAME(position).PUBLIC();
	FIELD().NAME(enabled).PUBLIC();
	FIELD().NAME(selected).PUBLIC();
	FIELD().DEFAULT_VALUE(CursorType::Arrow).NAME(cursorType).PUBLIC();
	FIELD().DEFAULT_VALUE(true).NAME(pixelPerfect).PUBLIC();
	FIELD().DEFAULT_VALUE(VK_CONTROL).NAME(snappingKey).PUBLIC();
	FIELD().NAME(onChangedPos).PUBLIC();
	FIELD().NAME(onPressed).PUBLIC();
	FIELD().NAME(onReleased).PUBLIC();
	FIELD().NAME(onBeganDragging).PUBLIC();
	FIELD().NAME(onChangeCompleted).PUBLIC();
	FIELD().NAME(isPointInside).PUBLIC();
	FIELD().NAME(screenToLocalTransformFunc).PUBLIC();
	FIELD().NAME(localToScreenTransformFunc).PUBLIC();
	FIELD().NAME(checkPositionFunc).PUBLIC();
	FIELD().NAME(checkSnappingFunc).PUBLIC();
	FIELD().NAME(onRightButtonPressed).PUBLIC();
	FIELD().NAME(onRightButtonReleased).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mRegularSprite).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mHoverSprite).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mPressedSprite).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mSelectedSprite).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mSelectedHoverSprite).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mSelectedPressedSprite).PROTECTED();
	FIELD().DEFAULT_VALUE(true).NAME(mEnabled).PROTECTED();
	FIELD().NAME(mPosition).PROTECTED();
	FIELD().NAME(mScreenPosition).PROTECTED();
	FIELD().NAME(mLastDrawnScreenPosition).PROTECTED();
	FIELD().DEFAULT_VALUE(-1).NAME(mLastScreenPosUpdateFrame).PROTECTED();
	FIELD().DEFAULT_VALUE(0.0f).NAME(mAngle).PROTECTED();
	FIELD().NAME(mDragOffset).PROTECTED();
	FIELD().NAME(mDragPosition).PROTECTED();
	FIELD().NAME(mDragBeginPosition).PROTECTED();
	FIELD().NAME(mPressedCursorId).PROTECTED();
	FIELD().NAME(mPressedCursorPos).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mIsDragging).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mIsHovered).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mIsSelected).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSelectGroup).PROTECTED();
	FIELD().DEFAULT_VALUE(3.0f).NAME(mDragDistanceThreshold).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::DragHandle)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Draw, const RectF&);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, SetPosition, const Vec2F&);
	PUBLIC_FUNCTION(const Vec2F&, GetScreenPosition);
	PUBLIC_FUNCTION(void, UpdateScreenPosition);
	PUBLIC_FUNCTION(void, SetDragPosition, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetDragPosition);
	PUBLIC_FUNCTION(Vec2F, GetPosition);
	PUBLIC_FUNCTION(Vec2F, GetDraggingOffset);
	PUBLIC_FUNCTION(Vec2F, GetDraggingBeginPosition);
	PUBLIC_FUNCTION(void, BeginDrag, const Vec2F&);
	PUBLIC_FUNCTION(void, SetSelected, bool);
	PUBLIC_FUNCTION(bool, IsSelected);
	PUBLIC_FUNCTION(void, Select);
	PUBLIC_FUNCTION(void, Deselect);
	PUBLIC_FUNCTION(void, SetSelectionGroup, ISelectableDragHandlesGroup*);
	PUBLIC_FUNCTION(ISelectableDragHandlesGroup*, GetSelectionGroup);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(void, SetAngle, float);
	PUBLIC_FUNCTION(float, GetAngle);
	PUBLIC_FUNCTION(Vec2F, ScreenToLocal, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, LocalToScreen, const Vec2F&);
	PUBLIC_FUNCTION(void, SetRegularSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetRegularSprite);
	PUBLIC_FUNCTION(void, SetHoverSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetHoverSprite);
	PUBLIC_FUNCTION(void, SetPressedSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetPressedSprite);
	PUBLIC_FUNCTION(void, SetSelectedSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetSelectedSprite);
	PUBLIC_FUNCTION(void, SetSelectedHoveredSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetSelectedHoveredSprite);
	PUBLIC_FUNCTION(void, SetSelectedPressedSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetSelectedPressedSprite);
	PUBLIC_FUNCTION(void, SetSpritesSize, const Vec2F&);
	PUBLIC_FUNCTION(void, SetSpritesColor, const Color4&);
	PUBLIC_FUNCTION(void, SetSpritesSizePivot, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PROTECTED_FUNCTION(void, DrawInternal);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleasedOutside, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, UpdateSpritesPositions);
	PROTECTED_FUNCTION(void, OnSelected);
	PROTECTED_FUNCTION(void, OnDeselected);
}
END_META;

CLASS_BASES_META(o2::WidgetDragHandle)
{
	BASE_CLASS(o2::DragHandle);
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(o2::WidgetDragHandle)
{
	FIELD().NAME(widgetOffsetToLocalTransformFunc).PUBLIC();
	FIELD().NAME(localToWidgetOffsetTransformFunc).PUBLIC();
	FIELD().NAME(onLayoutUpdated).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::WidgetDragHandle)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(Vec2F, ScreenToLocal, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, LocalToScreen, const Vec2F&);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, OnSerialize, DataValue&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
}
END_META;
