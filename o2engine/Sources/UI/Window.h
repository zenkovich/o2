#pragma once

#include "UI/ScrollArea.h"
#include "Utils/Editor/FunctionalDragHandle.h"

namespace o2
{
	class Text;
	class Sprite;

	// ----------------------------------------------------
	// Window with caption, icon, options and close buttons
	// ----------------------------------------------------
	class UIWindow: public UIScrollArea
	{
	public:
		Property<WString>     caption; // Window caption property
		Property<Ptr<Sprite>> icon;    // Window icon sprite property

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
		Ptr<UIWidget> AddWindowElement(Ptr<UIWidget> widget);

		// Removes window element widget
		void RemoveWindowElement(Ptr<UIWidget> widget);

		// Removes all window elements widgets
		void RemoveAllWindowElements();

		// Sets icon sprite
		void SetIcon(Ptr<Sprite> icon);

		// Returns icon sprite
		Ptr<Sprite> GetIcon() const;

		// Sets caption text
		void SetCaption(const WString& caption);

		// Returns caption text
		WString GetCaption() const;

		// Sets drag handle areas layouts
		void SetDragAreaLayouts(const Layout& head, const Layout& top, const Layout&bottom, const Layout&left,
								const Layout& right, const Layout& leftTop, const Layout& rightTop, const Layout& leftBottom,
								const Layout& rightBottom);

		SERIALIZABLE_IMPL(UIWindow);

		IOBJECT(UIWindow)
		{
			BASE_CLASS(UIScrollArea);

			FIELD(caption);
			FIELD(icon);

			SRLZ_FIELD(mWindowElements);
			SRLZ_FIELD(mHeadDragAreaLayout);
			SRLZ_FIELD(mTopDragAreaLayout);
			SRLZ_FIELD(mBottomDragAreaLayout);
			SRLZ_FIELD(mLeftDragAreaLayout);
			SRLZ_FIELD(mRightDragAreaLayout);
			SRLZ_FIELD(mLeftTopDragAreaLayout);
			SRLZ_FIELD(mRightTopDragAreaLayout);
			SRLZ_FIELD(mLeftBottomDragAreaLayout);
			SRLZ_FIELD(mRightBottomDragAreaLayout);
		}

	protected:
		Ptr<Sprite>          mIconDrawable;              // Icon drawable from layer "icon"
		Ptr<Text>            mCaptionDrawable;           // text caption from layer "caption"
		WidgetsVec           mWindowElements;            // Windows elements widgets
		float                mDrawingDepth;              // Window's drawing depth

		FunctionalDragHandle mHeadDragHandle;			 // Head drag handle, for moving window
		Layout               mHeadDragAreaLayout;		 // Head drag handle layout
		RectF                mHeadDragAreaRect;			 // Head drag handle rect
														 
		FunctionalDragHandle mTopDragHandle;			 //	Top drag handle, for resizing window
		Layout               mTopDragAreaLayout;		 //	Top drag handle layout
		RectF                mTopDragAreaRect;			 //	Top drag handle rect
														 
		FunctionalDragHandle mBottomDragHandle;			 //	Bottom drag handle, for resizing window
		Layout               mBottomDragAreaLayout;		 //	Bottom drag handle layout
		RectF                mBottomDragAreaRect;		 //	Bottom drag handle rect
														 
		FunctionalDragHandle mLeftDragHandle;			 //	Left drag handle, for resizing window
		Layout               mLeftDragAreaLayout;		 //	Left drag handle layout
		RectF                mLeftDragAreaRect;			 //	Left drag handle rect
														 
		FunctionalDragHandle mRightDragHandle;			 //	Right drag handle, for resizing window
		Layout               mRightDragAreaLayout;		 //	Right drag handle layout
		RectF                mRightDragAreaRect;		 //	Right drag handle rect
														 
		FunctionalDragHandle mLeftTopDragHandle;		 //	Left Top drag handle, for resizing window
		Layout               mLeftTopDragAreaLayout;	 //	Left Top drag handle layout
		RectF                mLeftTopDragAreaRect;		 //	Left Top drag handle rect
														 
		FunctionalDragHandle mRightTopDragHandle;		 //	Right Top drag handle, for resizing window
		Layout               mRightTopDragAreaLayout;	 //	Right Top drag handle layout
		RectF                mRightTopDragAreaRect;		 //	Right Top drag handle rect
														 
		FunctionalDragHandle mLeftBottomDragHandle;		 //	Left Bottom drag handle, for resizing window
		Layout               mLeftBottomDragAreaLayout;	 //	Left Bottom drag handle layout
		RectF                mLeftBottomDragAreaRect;	 //	Left Bottom drag handle rect
														 
		FunctionalDragHandle mRightBottomDragHandle;	 //	Right Bottom drag handle, for resizing window
		Layout               mRightBottomDragAreaLayout; //	Right Bottom drag handle layout
		RectF                mRightBottomDragAreaRect;   //	Right Bottom drag handle rect

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Updates transparency for this and children widgets
		void UpdateTransparency();

		// Calls when layer added and updates drawing sequence
		void OnLayerAdded(Ptr<UIWidgetLayer> layer);

		// Initializes handles
		void InitializeHandles();

		// Sets drag handles interactable. Using when changing visibility
		void SetHandlesInteractable(bool interactable);

		// Binds all drag handles interactable parameter to window visibility
		void BindHandlesInteractableToVisibility();

		// Initializes properties
		void InitializeProperties();
	};
}
