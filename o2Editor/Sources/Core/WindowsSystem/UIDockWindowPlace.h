#pragma once

#include "Events/CursorEventsListener.h"
#include "UI/Widget.h"
#include "Utils/Editor/FunctionalDragHandle.h"
#include "Utils/Math/Layout.h"
#include "Utils/Math/Rect.h"

using namespace o2;

// ------------------------------------
// Dockable windows place for attaching
// ------------------------------------
class UIDockWindowPlace: public o2::UIWidget, public CursorEventsListener
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

	// Sets resizible side and configures drag handle when draggable is true
	void SetResizibleDir(TwoDirection dir, float border,
						 Ptr<UIDockWindowPlace> neighborMin, Ptr<UIDockWindowPlace> neighborMax);

	// Returns resizible side
	TwoDirection GetResizibleDir() const;

	SERIALIZABLE(UIDockWindowPlace);

protected:
	TwoDirection           mResizibleDir;       // Resizible dragable side

	Ptr<UIDockWindowPlace> mNeighborMin; // Resizing neighbor, using when dragging this side
	FunctionalDragHandle   mDragHandleMin;       // Separator drag handle
	Layout                 mDragHandleLayoutMin; // Separator drag handle layout
	RectF                  mDragHandleAreaMin;	 // Separator drag handle area calculated from mDragHandleLayout

	Ptr<UIDockWindowPlace> mNeighborMax; // Resizing neighbor, using when dragging this side
	FunctionalDragHandle   mDragHandleMax;       // Separator drag handle
	Layout                 mDragHandleLayoutMax; // Separator drag handle layout
	RectF                  mDragHandleAreaMax;	 // Separator drag handle area calculated from mDragHandleLayout

	float                  mDragHandleDepth;     // Drag handle drawing depth

protected:
	// Updates layout
	void UpdateLayout(bool forcible = false);

	// Calls when cursor drag handle was moved
	void OnDragHandleMinMoved(const Vec2F& delta);

	// Calls when cursor drag handle was moved
	void OnDragHandleMaxMoved(const Vec2F& delta);

	// Checks interactable. If childs more than 0 this will be not interactable
	void CheckInteractable();

	// Initializes drag handle
	void InitializeDragHandle();

	friend class UIDockableWindow;
};