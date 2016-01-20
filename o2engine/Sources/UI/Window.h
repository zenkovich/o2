#pragma once

#include "Events/CursorEventsListener.h"
#include "Render/Sprite.h"
#include "UI/ScrollArea.h"
#include "Utils/Editor/FunctionalDragHandle.h"

namespace o2
{
	class Text;

	// ----------------------------------------------------
	// Window with caption, icon, options and close buttons
	// ----------------------------------------------------
	class UIWindow: public UIScrollArea, public CursorEventsListener
	{
	public:
		Property<WString> caption; // Window caption property
		Property<Sprite*> icon;    // Window icon sprite property

		// Default constructor
		UIWindow();

		// Copy-constructor
		UIWindow(const UIWindow& other);

		// Destructor
		~UIWindow();

		// Copy-operator
		UIWindow& operator=(const UIWindow& other);

		// Updates drawables, states and widget
		void Update(float dt);

		// Draws widget
		void Draw();

		// Adds window element widget
		UIWidget* AddWindowElement(UIWidget* widget);

		// Removes window element widget
		void RemoveWindowElement(UIWidget* widget);

		// Removes all window elements widgets
		void RemoveAllWindowElements();

		// Sets icon sprite
		void SetIcon(Sprite* icon);

		// Returns icon sprite
		Sprite* GetIcon() const;

		// Sets caption text
		void SetCaption(const WString& caption);

		// Returns caption text
		WString GetCaption() const;

		// Sets drag handle areas layouts
		void SetDragAreaLayouts(const Layout& head, const Layout& top, const Layout&bottom, const Layout&left,
								const Layout& right, const Layout& leftTop, const Layout& rightTop, const Layout& leftBottom,
								const Layout& rightBottom);

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		// Returns is this widget can be selected
		bool IsSelectable() const;

		SERIALIZABLE(UIWindow);

	protected:
		Sprite*              mIconDrawable;              // Icon drawable from layer "icon"
		Text*                mCaptionDrawable;           // text caption from layer "caption"
		WidgetsVec           mWindowElements;            // Windows elements widgets @SERIALIZABLE
		float                mDrawingDepth;              // Window's drawing depth

		FunctionalDragHandle mHeadDragHandle;			 // Head drag handle, for moving window
		Layout               mHeadDragAreaLayout;		 // Head drag handle layout @SERIALIZABLE
		RectF                mHeadDragAreaRect;			 // Head drag handle rect
														 
		FunctionalDragHandle mTopDragHandle;			 //	Top drag handle, for resizing window
		Layout               mTopDragAreaLayout;		 //	Top drag handle layout @SERIALIZABLE
		RectF                mTopDragAreaRect;			 //	Top drag handle rect
														 
		FunctionalDragHandle mBottomDragHandle;			 //	Bottom drag handle, for resizing window
		Layout               mBottomDragAreaLayout;		 //	Bottom drag handle layout @SERIALIZABLE
		RectF                mBottomDragAreaRect;		 //	Bottom drag handle rect
														 
		FunctionalDragHandle mLeftDragHandle;			 //	Left drag handle, for resizing window
		Layout               mLeftDragAreaLayout;		 //	Left drag handle layout @SERIALIZABLE
		RectF                mLeftDragAreaRect;			 //	Left drag handle rect
														 
		FunctionalDragHandle mRightDragHandle;			 //	Right drag handle, for resizing window
		Layout               mRightDragAreaLayout;		 //	Right drag handle layout @SERIALIZABLE
		RectF                mRightDragAreaRect;		 //	Right drag handle rect
														 
		FunctionalDragHandle mLeftTopDragHandle;		 //	Left Top drag handle, for resizing window
		Layout               mLeftTopDragAreaLayout;	 //	Left Top drag handle layout @SERIALIZABLE
		RectF                mLeftTopDragAreaRect;		 //	Left Top drag handle rect
														 
		FunctionalDragHandle mRightTopDragHandle;		 //	Right Top drag handle, for resizing window
		Layout               mRightTopDragAreaLayout;	 //	Right Top drag handle layout @SERIALIZABLE
		RectF                mRightTopDragAreaRect;		 //	Right Top drag handle rect
														 
		FunctionalDragHandle mLeftBottomDragHandle;		 //	Left Bottom drag handle, for resizing window
		Layout               mLeftBottomDragAreaLayout;	 //	Left Bottom drag handle layout @SERIALIZABLE
		RectF                mLeftBottomDragAreaRect;	 //	Left Bottom drag handle rect
														 
		FunctionalDragHandle mRightBottomDragHandle;	 //	Right Bottom drag handle, for resizing window
		Layout               mRightBottomDragAreaLayout; //	Right Bottom drag handle layout @SERIALIZABLE
		RectF                mRightBottomDragAreaRect;   //	Right Bottom drag handle rect

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Updates transparency for this and children widgets
		void UpdateTransparency();

		// Calls when layer added and updates drawing sequence
		void OnLayerAdded(UIWidgetLayer* layer);

		// Initializes handles
		void InitializeHandles();

		// Sets drag handles interactable. Using when changing visibility
		void SetHandlesInteractable(bool interactable);

		// Binds all drag handles interactable parameter to window visibility
		void BindHandlesInteractableToVisibility();

		// Calls when widget was selected
		void OnSelected();

		// Calls when child widget was selected
		void OnChildSelected(UIWidget* child);

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when widget state was added
		void OnStateAdded(UIWidgetState* state);

		// Calls when visible was changed
		void OnVisibleChanged();

		// Initializes properties
		void InitializeProperties();
	};
}
