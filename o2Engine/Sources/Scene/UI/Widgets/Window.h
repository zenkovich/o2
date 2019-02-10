#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Render/Sprite.h"
#include "Scene/UI/Widgets/ScrollArea.h"
#include "Events/CursorEventsArea.h"

namespace o2
{
	class Text;
	class UIContextMenu;

	// ----------------------------------------------------
	// Window with caption, icon, options and close buttons
	// ----------------------------------------------------
	class UIWindow: public UIScrollArea, public DrawableCursorEventsListener
	{
	public:
		PROPERTIES(UIWindow);
		PROPERTY(WString, caption, SetCaption, GetCaption); // Window caption property
		PROPERTY(Sprite*, icon, SetIcon, GetIcon);          // Window icon sprite property

		// Default constructor
		UIWindow();

		// Copy-constructor
		UIWindow(const UIWindow& other);

		// Destructor
		~UIWindow();

		// Copy-operator
		UIWindow& operator=(const UIWindow& other);

		// Draws widget
		void Draw() override;

		// Shows window as modal
		void ShowModal();

		// Sets icon sprite
		virtual void SetIcon(Sprite* icon);

		// Returns icon sprite
		virtual Sprite* GetIcon() const;

		// Sets icon layer layout
		virtual void SetIconLayout(const Layout& layout);

		// Returns icon layer layout
		virtual Layout GetIconLayout() const;

		// Sets caption text
		virtual void SetCaption(const WString& caption);

		// Returns caption text
		virtual WString GetCaption() const;

		// Returns options context menu
		UIContextMenu* GetOptionsMenu() const;

		// Sets drag handle areas layouts
		void SetDragAreaLayouts(const Layout& head, const Layout& top, const Layout& bottom, const Layout& left,
								const Layout& right, const Layout& leftTop, const Layout& rightTop, const Layout& leftBottom,
								const Layout& rightBottom);

		// Returns is this widget can be selected
		bool IsFocusable() const;

		// Sets is window modal
		void SetModal(bool isModal);

		// Returns is window modal
		bool IsModal() const;

		// Updates layout
		void UpdateSelfTransform();

		// Returns back cursor events listener
		CursorEventsArea& GetBackCursorListener();

		SERIALIZABLE(UIWindow);

	protected:
		const char* mIconLayerPath = "icon";
		const char* mCaptionLayerPath = "caption";

		UIContextMenu*   mOptionsMenu;               // Window options context menu

		CursorEventsArea mBackCursorArea;            // Cursor area listener at back of window, for catching events

		CursorEventsArea mHeadDragHandle;			 // Head drag handle, for moving window
		Layout           mHeadDragAreaLayout;		 // Head drag handle layout @SERIALIZABLE
		RectF            mHeadDragAreaRect;			 // Head drag handle rect
													 
		CursorEventsArea mTopDragHandle;			 //	Top drag handle, for resizing window
		Layout           mTopDragAreaLayout;		 //	Top drag handle layout @SERIALIZABLE
		RectF            mTopDragAreaRect;			 //	Top drag handle rect
													 
		CursorEventsArea mBottomDragHandle;			 //	Bottom drag handle, for resizing window
		Layout           mBottomDragAreaLayout;		 //	Bottom drag handle layout @SERIALIZABLE
		RectF            mBottomDragAreaRect;		 //	Bottom drag handle rect
													 
		CursorEventsArea mLeftDragHandle;			 //	Left drag handle, for resizing window
		Layout           mLeftDragAreaLayout;		 //	Left drag handle layout @SERIALIZABLE
		RectF            mLeftDragAreaRect;			 //	Left drag handle rect
													 
		CursorEventsArea mRightDragHandle;			 //	Right drag handle, for resizing window
		Layout           mRightDragAreaLayout;		 //	Right drag handle layout @SERIALIZABLE
		RectF            mRightDragAreaRect;		 //	Right drag handle rect
													 
		CursorEventsArea mLeftTopDragHandle;		 //	Left Top drag handle, for resizing window
		Layout           mLeftTopDragAreaLayout;	 //	Left Top drag handle layout @SERIALIZABLE
		RectF            mLeftTopDragAreaRect;		 //	Left Top drag handle rect
													 
		CursorEventsArea mRightTopDragHandle;		 //	Right Top drag handle, for resizing window
		Layout           mRightTopDragAreaLayout;	 //	Right Top drag handle layout @SERIALIZABLE
		RectF            mRightTopDragAreaRect;		 //	Right Top drag handle rect
													 
		CursorEventsArea mLeftBottomDragHandle;		 //	Left Bottom drag handle, for resizing window
		Layout           mLeftBottomDragAreaLayout;	 //	Left Bottom drag handle layout @SERIALIZABLE
		RectF            mLeftBottomDragAreaRect;	 //	Left Bottom drag handle rect
													 
		CursorEventsArea mRightBottomDragHandle;	 //	Right Bottom drag handle, for resizing window
		Layout           mRightBottomDragAreaLayout; //	Right Bottom drag handle layout @SERIALIZABLE
		RectF            mRightBottomDragAreaRect;   //	Right Bottom drag handle rect

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when widget was selected
		void OnFocused() override;

		// It is called when widget state was added
		void OnStateAdded(UIWidgetState* state) override;

		// It is called when visible was changed
		void OnResEnableInHierarchyChanged() override;

		// Initializes context menu and options button
		void InitializeContextMenu();

		// Initializes context menu items - close window
		virtual void InitializeContextItems();

		// Restores close button, options button and context menu from window elements
		void RestoreControls();

		// Initializes handles
		void InitializeHandles();

		// Sets drag handles interactable. Using when changing visibility
		void SetHandlesInteractable(bool interactable);

		// Binds all drag handles interactable parameter to window visibility
		void BindHandlesInteractableToVisibility();

		// It is called when child widget was selected
		void OnChildFocused(UIWidget* child) override;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;
	};
}

CLASS_BASES_META(o2::UIWindow)
{
	BASE_CLASS(o2::UIScrollArea);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::UIWindow)
{
	PUBLIC_FIELD(caption);
	PUBLIC_FIELD(icon);
	PROTECTED_FIELD(mIconLayerPath);
	PROTECTED_FIELD(mCaptionLayerPath);
	PROTECTED_FIELD(mOptionsMenu);
	PROTECTED_FIELD(mBackCursorArea);
	PROTECTED_FIELD(mHeadDragHandle);
	PROTECTED_FIELD(mHeadDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHeadDragAreaRect);
	PROTECTED_FIELD(mTopDragHandle);
	PROTECTED_FIELD(mTopDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTopDragAreaRect);
	PROTECTED_FIELD(mBottomDragHandle);
	PROTECTED_FIELD(mBottomDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mBottomDragAreaRect);
	PROTECTED_FIELD(mLeftDragHandle);
	PROTECTED_FIELD(mLeftDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLeftDragAreaRect);
	PROTECTED_FIELD(mRightDragHandle);
	PROTECTED_FIELD(mRightDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mRightDragAreaRect);
	PROTECTED_FIELD(mLeftTopDragHandle);
	PROTECTED_FIELD(mLeftTopDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLeftTopDragAreaRect);
	PROTECTED_FIELD(mRightTopDragHandle);
	PROTECTED_FIELD(mRightTopDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mRightTopDragAreaRect);
	PROTECTED_FIELD(mLeftBottomDragHandle);
	PROTECTED_FIELD(mLeftBottomDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLeftBottomDragAreaRect);
	PROTECTED_FIELD(mRightBottomDragHandle);
	PROTECTED_FIELD(mRightBottomDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mRightBottomDragAreaRect);
}
END_META;
CLASS_METHODS_META(o2::UIWindow)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, ShowModal);
	PUBLIC_FUNCTION(void, SetIcon, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetIcon);
	PUBLIC_FUNCTION(void, SetIconLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetIconLayout);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(UIContextMenu*, GetOptionsMenu);
	PUBLIC_FUNCTION(void, SetDragAreaLayouts, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(void, SetModal, bool);
	PUBLIC_FUNCTION(bool, IsModal);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_FUNCTION(CursorEventsArea&, GetBackCursorListener);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnStateAdded, UIWidgetState*);
	PROTECTED_FUNCTION(void, OnResEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, InitializeContextMenu);
	PROTECTED_FUNCTION(void, InitializeContextItems);
	PROTECTED_FUNCTION(void, RestoreControls);
	PROTECTED_FUNCTION(void, InitializeHandles);
	PROTECTED_FUNCTION(void, SetHandlesInteractable, bool);
	PROTECTED_FUNCTION(void, BindHandlesInteractableToVisibility);
	PROTECTED_FUNCTION(void, OnChildFocused, UIWidget*);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
}
END_META;
