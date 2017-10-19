#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Render/Sprite.h"
#include "UI/ScrollArea.h"
#include "Utils/CursorEventsArea.h"

namespace o2
{
	class Text;

	// ----------------------------------------------------
	// Window with caption, icon, options and close buttons
	// ----------------------------------------------------
	class UIWindow: public UIScrollArea, public DrawableCursorEventsListener
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
		void Update(float dt) override;

		// Draws widget
		void Draw() override;

		// Shows window as modal
		void ShowModal();

		// Adds window element widget
		UIWidget* AddWindowElement(UIWidget* widget);

		// Removes window element widget
		void RemoveWindowElement(UIWidget* widget);

		// Removes all window elements widgets
		void RemoveAllWindowElements();

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
		void UpdateLayout(bool withChildren = true);

		// Returns back cursor events listener
		CursorEventsArea& GetBackCursorListener();

		SERIALIZABLE(UIWindow);

	protected:
		const char* mIconLayerPath = "icon";
		const char* mCaptionLayerPath = "caption";

		WidgetsVec           mWindowElements;            // Windows elements widgets @SERIALIZABLE

		CursorEventsArea     mBackCursorArea;            // Cursor area listener at back of window, for catching events

		CursorEventsArea     mHeadDragHandle;			 // Head drag handle, for moving window
		Layout               mHeadDragAreaLayout;		 // Head drag handle layout @SERIALIZABLE
		RectF                mHeadDragAreaRect;			 // Head drag handle rect
														 
		CursorEventsArea     mTopDragHandle;			 //	Top drag handle, for resizing window
		Layout               mTopDragAreaLayout;		 //	Top drag handle layout @SERIALIZABLE
		RectF                mTopDragAreaRect;			 //	Top drag handle rect
														 
		CursorEventsArea     mBottomDragHandle;			 //	Bottom drag handle, for resizing window
		Layout               mBottomDragAreaLayout;		 //	Bottom drag handle layout @SERIALIZABLE
		RectF                mBottomDragAreaRect;		 //	Bottom drag handle rect
														 
		CursorEventsArea     mLeftDragHandle;			 //	Left drag handle, for resizing window
		Layout               mLeftDragAreaLayout;		 //	Left drag handle layout @SERIALIZABLE
		RectF                mLeftDragAreaRect;			 //	Left drag handle rect
														 
		CursorEventsArea     mRightDragHandle;			 //	Right drag handle, for resizing window
		Layout               mRightDragAreaLayout;		 //	Right drag handle layout @SERIALIZABLE
		RectF                mRightDragAreaRect;		 //	Right drag handle rect
														 
		CursorEventsArea     mLeftTopDragHandle;		 //	Left Top drag handle, for resizing window
		Layout               mLeftTopDragAreaLayout;	 //	Left Top drag handle layout @SERIALIZABLE
		RectF                mLeftTopDragAreaRect;		 //	Left Top drag handle rect
														 
		CursorEventsArea     mRightTopDragHandle;		 //	Right Top drag handle, for resizing window
		Layout               mRightTopDragAreaLayout;	 //	Right Top drag handle layout @SERIALIZABLE
		RectF                mRightTopDragAreaRect;		 //	Right Top drag handle rect
														 
		CursorEventsArea     mLeftBottomDragHandle;		 //	Left Bottom drag handle, for resizing window
		Layout               mLeftBottomDragAreaLayout;	 //	Left Bottom drag handle layout @SERIALIZABLE
		RectF                mLeftBottomDragAreaRect;	 //	Left Bottom drag handle rect
														 
		CursorEventsArea     mRightBottomDragHandle;	 //	Right Bottom drag handle, for resizing window
		Layout               mRightBottomDragAreaLayout; //	Right Bottom drag handle layout @SERIALIZABLE
		RectF                mRightBottomDragAreaRect;   //	Right Bottom drag handle rect

	protected:
		// Updates transparency for this and children widgets
		void UpdateTransparency() override;

		// It is called when widget was selected
		void OnFocused() override;

		// It is called when widget state was added
		void OnStateAdded(UIWidgetState* state) override;

		// It is called when visible was changed
		void OnVisibleChanged() override;

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

		// Initializes properties
		void InitializeProperties();
	};
}
