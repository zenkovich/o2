#pragma once

#include "O2.h"
using namespace o2;

// ------------------------------------
// Dockable windows place for attaching
// ------------------------------------
class UIDockWindowPlace: public UIWidget, public CursorEventsListener
{
public:
	// Default constructor
	UIDockWindowPlace();

	// Copy-constructor
	UIDockWindowPlace(const UIDockWindowPlace& other);

	// Destructor
	~UIDockWindowPlace();

	// Copy-operator
	UIDockWindowPlace& operator=(const UIDockWindowPlace& other);

	// Draws widget
	void Draw();

	// Returns true if point is in this object
	bool IsUnderPoint(const Vec2F& point);

	// Returns depth (event system will catch listener with highest depth)
	float Depth();

	// Sets resizible side and configures drag handle
	void SetResizibleSide(Side side);

	SERIALIZABLE_IMPL(UIDockWindowPlace);
	IOBJECT(UIDockWindowPlace)
	{
		BASE_CLASS(UIWidget);
	}

protected:
	Side                 mDragSide;         // Draggable side
	FunctionalDragHandle mDragHandle; 		// Separator drag handle
	Layout               mDragHandleLayout;	// Separator drag handle layout
	RectF                mDragHandleArea;	// Separator drag handle area calculated from mDragHandleLayout
	float                mDragHandleDepth;  // Drag handle drawing depth

protected:
	// Updates layout
	void UpdateLayout(bool forcible = false);

	// Calls when cursor drag handle was moved
	void OnDragHandleMoved(const Vec2F& delta);

	// Checks interactable. If childs more than 0 this will be not interactable
	void CheckInteractable();

	// Initializes drag handle
	void InitializeDragHandle();

	friend class UIDockableWindow;
};