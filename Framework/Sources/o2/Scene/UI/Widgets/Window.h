#pragma once

#include "o2/Render/Sprite.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"
#include "o2/Events/CursorEventsArea.h"

namespace o2
{
	class Text;
	class ContextMenu;

	// ----------------------------------------------------
	// Window with caption, icon, options and close buttons
	// ----------------------------------------------------
	class Window: public ScrollArea
	{
	public:
		PROPERTIES(Window);
		PROPERTY(WString, caption, SetCaption, GetCaption); // Window caption property
		PROPERTY(Sprite*, icon, SetIcon, GetIcon);          // Window icon sprite property

		Function<void()> onOpened; // On window opened delegate
		Function<void()> onClosed; // On window closed delegate

	public:
		// Default constructor
		Window();

		// Copy-constructor
		Window(const Window& other);

		// Destructor
		~Window();

		// Copy-operator
		Window& operator=(const Window& other);

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
		ContextMenu* GetOptionsMenu() const;

		// Sets drag handle areas layouts
		void SetDragAreaLayouts(const Layout& head, const Layout& top, const Layout& bottom, const Layout& left,
								const Layout& right, const Layout& leftTop, const Layout& rightTop, const Layout& leftBottom,
								const Layout& rightBottom);

		// Returns is this widget can be selected
		bool IsFocusable() const override;

		// Sets is window modal
		void SetModal(bool isModal);

		// Returns is window modal
		bool IsModal() const;

		// Updates layout
		void UpdateSelfTransform() override;

		// Returns back cursor events listener
		CursorEventsArea& GetBackCursorListener();

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(Window);

	protected:
		const char* mIconLayerPath = "icon";
		const char* mCaptionLayerPath = "caption";

		ContextMenu* mOptionsMenu; // Window options context menu

		CursorEventsArea mBackCursorArea; // Cursor area listener at back of window, for catching events

		CursorEventsArea mHeadDragHandle;	  // Head drag handle, for moving window
		Layout           mHeadDragAreaLayout; // Head drag handle layout @SERIALIZABLE
		RectF            mHeadDragAreaRect;	  // Head drag handle rect

		CursorEventsArea mTopDragHandle;	 //	Top drag handle, for resizing window
		Layout           mTopDragAreaLayout; //	Top drag handle layout @SERIALIZABLE
		RectF            mTopDragAreaRect;	 //	Top drag handle rect

		CursorEventsArea mBottomDragHandle;		// Bottom drag handle, for resizing window
		Layout           mBottomDragAreaLayout;	// Bottom drag handle layout @SERIALIZABLE
		RectF            mBottomDragAreaRect;	// Bottom drag handle rect

		CursorEventsArea mLeftDragHandle;	  // Left drag handle, for resizing window
		Layout           mLeftDragAreaLayout; // Left drag handle layout @SERIALIZABLE
		RectF            mLeftDragAreaRect;	  // Left drag handle rect

		CursorEventsArea mRightDragHandle;		// Right drag handle, for resizing window
		Layout           mRightDragAreaLayout;	// Right drag handle layout @SERIALIZABLE
		RectF            mRightDragAreaRect;	// Right drag handle rect

		CursorEventsArea mLeftTopDragHandle;	// Left Top drag handle, for resizing window
		Layout           mLeftTopDragAreaLayout;// Left Top drag handle layout @SERIALIZABLE
		RectF            mLeftTopDragAreaRect;	// Left Top drag handle rect
													 
		CursorEventsArea mRightTopDragHandle;	  // Right Top drag handle, for resizing window
		Layout           mRightTopDragAreaLayout; // Right Top drag handle layout @SERIALIZABLE
		RectF            mRightTopDragAreaRect;	  // Right Top drag handle rect
													
		CursorEventsArea mLeftBottomDragHandle;		// Left Bottom drag handle, for resizing window
		Layout           mLeftBottomDragAreaLayout;	// Left Bottom drag handle layout @SERIALIZABLE
		RectF            mLeftBottomDragAreaRect;	// Left Bottom drag handle rect
													 
		CursorEventsArea mRightBottomDragHandle;	 //	Right Bottom drag handle, for resizing window
		Layout           mRightBottomDragAreaLayout; //	Right Bottom drag handle layout @SERIALIZABLE
		RectF            mRightBottomDragAreaRect;   //	Right Bottom drag handle rect

	protected:
		// It is called when widget was selected
		void OnFocused() override;

		// Is is called when actor enabled in hierarchy, calls onOpened
		void OnEnabled() override;

		// It is called when actor disabled in hierarchy, calls onCLosed
		void OnDisabled()override;

		// It is called when widget state was added
		void OnStateAdded(WidgetState* state) override;

		// It is called when visible was changed
		void OnEnableInHierarchyChanged() override;

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
		void OnChildFocused(Widget* child) override;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;
	};
}

CLASS_BASES_META(o2::Window)
{
	BASE_CLASS(o2::ScrollArea);
}
END_META;
CLASS_FIELDS_META(o2::Window)
{
	FIELD().NAME(caption).PUBLIC();
	FIELD().NAME(icon).PUBLIC();
	FIELD().NAME(onOpened).PUBLIC();
	FIELD().NAME(onClosed).PUBLIC();
	FIELD().DEFAULT_VALUE("icon").NAME(mIconLayerPath).PROTECTED();
	FIELD().DEFAULT_VALUE("caption").NAME(mCaptionLayerPath).PROTECTED();
	FIELD().NAME(mOptionsMenu).PROTECTED();
	FIELD().NAME(mBackCursorArea).PROTECTED();
	FIELD().NAME(mHeadDragHandle).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mHeadDragAreaLayout).PROTECTED();
	FIELD().NAME(mHeadDragAreaRect).PROTECTED();
	FIELD().NAME(mTopDragHandle).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mTopDragAreaLayout).PROTECTED();
	FIELD().NAME(mTopDragAreaRect).PROTECTED();
	FIELD().NAME(mBottomDragHandle).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mBottomDragAreaLayout).PROTECTED();
	FIELD().NAME(mBottomDragAreaRect).PROTECTED();
	FIELD().NAME(mLeftDragHandle).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mLeftDragAreaLayout).PROTECTED();
	FIELD().NAME(mLeftDragAreaRect).PROTECTED();
	FIELD().NAME(mRightDragHandle).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mRightDragAreaLayout).PROTECTED();
	FIELD().NAME(mRightDragAreaRect).PROTECTED();
	FIELD().NAME(mLeftTopDragHandle).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mLeftTopDragAreaLayout).PROTECTED();
	FIELD().NAME(mLeftTopDragAreaRect).PROTECTED();
	FIELD().NAME(mRightTopDragHandle).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mRightTopDragAreaLayout).PROTECTED();
	FIELD().NAME(mRightTopDragAreaRect).PROTECTED();
	FIELD().NAME(mLeftBottomDragHandle).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mLeftBottomDragAreaLayout).PROTECTED();
	FIELD().NAME(mLeftBottomDragAreaRect).PROTECTED();
	FIELD().NAME(mRightBottomDragHandle).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mRightBottomDragAreaLayout).PROTECTED();
	FIELD().NAME(mRightBottomDragAreaRect).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::Window)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, ShowModal);
	PUBLIC_FUNCTION(void, SetIcon, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetIcon);
	PUBLIC_FUNCTION(void, SetIconLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetIconLayout);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(ContextMenu*, GetOptionsMenu);
	PUBLIC_FUNCTION(void, SetDragAreaLayouts, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(void, SetModal, bool);
	PUBLIC_FUNCTION(bool, IsModal);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_FUNCTION(CursorEventsArea&, GetBackCursorListener);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnStateAdded, WidgetState*);
	PROTECTED_FUNCTION(void, OnEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, InitializeContextMenu);
	PROTECTED_FUNCTION(void, InitializeContextItems);
	PROTECTED_FUNCTION(void, RestoreControls);
	PROTECTED_FUNCTION(void, InitializeHandles);
	PROTECTED_FUNCTION(void, SetHandlesInteractable, bool);
	PROTECTED_FUNCTION(void, BindHandlesInteractableToVisibility);
	PROTECTED_FUNCTION(void, OnChildFocused, Widget*);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
}
END_META;
