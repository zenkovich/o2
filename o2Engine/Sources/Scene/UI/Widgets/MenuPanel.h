#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Scene/UI/ContextMenu.h"
#include "Scene/UI/Widget.h"
#include "Utils/Delegates.h"

namespace o2
{
	class UIHorizontalLayout;

	// -------------
	// UI Menu panel
	// -------------
	class UIMenuPanel: public UIWidget, public DrawableCursorEventsListener
	{
	public:
		class Item: public ISerializable
		{
		public:
			WString                     text;     // @SERIALIZABLE
			Vector<UIContextMenu::Item> subItems; // @SERIALIZABLE

			Function<void()>            onClick;

			Item();
			Item(const WString& text, Vector<UIContextMenu::Item> subItems);
			Item(const WString& text, const Function<void()> onClick);

			bool operator==(const Item& other) const;

			SERIALIZABLE(Item);
		};

	public:
		// Default constructor
		UIMenuPanel();

		// Copy-constructor
		UIMenuPanel(const UIMenuPanel& other);

		// Destructor
		~UIMenuPanel();

		// Copy-operator
		UIMenuPanel& operator=(const UIMenuPanel& other);

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Draws widget
		void Draw() override;

		// Add item
		UIWidget* AddItem(const Item& item);

		// Adds item by path ("node/sub node/target")
		UIWidget* AddItem(const WString& path, const Function<void()>& 
						  clickFunc = Function<void()>(),
						  const ImageAssetRef& icon = ImageAssetRef(), 
						  const ShortcutKeys& shortcut = ShortcutKeys());

		// Adds toggle item by path ("node/sub node/target")
		UIWidget* AddToggleItem(const WString& path, bool value, 
								const Function<void(bool)>& clickFunc = Function<void(bool)>(),
								const ImageAssetRef& icon = ImageAssetRef(), 
								const ShortcutKeys& shortcut = ShortcutKeys());

			  // Inserts item at position
		UIWidget* InsertItem(const Item& item, int position);

		// Adds array of items
		void AddItems(Vector<Item> items);

		// Inserts array of items at position
		void InsertItems(Vector<Item> items, int position);

		// Returns item at position
		Item GetItem(int position);

		// Returns array of all items
		Vector<Item> GetItems() const;

		// Removes item at position
		void RemoveItem(int position);

		// Removes item by path
		void RemoveItem(const WString& path);

		// Removes all items
		void RemoveAllItems();

		// Returns items vertical layout
		UIHorizontalLayout* GetItemsLayout() const;

		// Returns item sample
		UIWidget* GetItemSample() const;

		// Sets item sample
		void SetItemSample(UIWidget* sample);

		// Returns selection drawable
		Sprite* GetSelectionDrawable() const;

		// Sets selection drawable layout
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		SERIALIZABLE(UIMenuPanel);

	protected:
		UIHorizontalLayout*      mLayout = nullptr;             // Items layout

		UIWidget*                mItemSample = nullptr;         // Item sample @SERIALIZABLE

		Vector<Function<void()>> mClickFunctions;               // Items click functions

		Sprite*                  mSelectionDrawable = nullptr;  // Selection sprite @SERIALIZABLE
		Layout                   mSelectionLayout;              // Selection layout, result selection area depends on selected item @SERIALIZABLE

		RectF                    mCurrentSelectionRect;         // Current selection rectangle (for smoothing)
		RectF                    mTargetSelectionRect;          // Target selection rectangle (over selected item)
		Vec2F                    mLastSelectCheckCursor;        // Last cursor position on selection check

		int                      mSelectedItem = -1;            // Index of selected item
		float                    mSelectSubContextTime = -1.0f; // Time to appearing selected sub context
		UIContextMenu*           mOpenedContext = nullptr;      // Last opened context in menu

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when visible was changed
		void OnResEnableInHierarchyChanged() override;

		// Creates sub context menus by path
		UIContextMenu* CreateSubContext(WString& path);

		// Creates item widget
		UIWidget* CreateItem(const Item& item);

		// Returns item info
		Item GetItemDef(int idx) const;

		// Returns item widget under point and stores index in idxPtr, if not null
		UIWidget* GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;
	};
}

CLASS_BASES_META(o2::UIMenuPanel)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::UIMenuPanel)
{
	PROTECTED_FIELD(mLayout);
	PROTECTED_FIELD(mItemSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mClickFunctions);
	PROTECTED_FIELD(mSelectionDrawable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelectionLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCurrentSelectionRect);
	PROTECTED_FIELD(mTargetSelectionRect);
	PROTECTED_FIELD(mLastSelectCheckCursor);
	PROTECTED_FIELD(mSelectedItem);
	PROTECTED_FIELD(mSelectSubContextTime);
	PROTECTED_FIELD(mOpenedContext);
}
END_META;
CLASS_METHODS_META(o2::UIMenuPanel)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(UIWidget*, AddItem, const Item&);
	PUBLIC_FUNCTION(UIWidget*, AddItem, const WString&, const Function<void()>&, const ImageAssetRef&, const ShortcutKeys&);
	PUBLIC_FUNCTION(UIWidget*, AddToggleItem, const WString&, bool, const Function<void(bool)>&, const ImageAssetRef&, const ShortcutKeys&);
	PUBLIC_FUNCTION(UIWidget*, InsertItem, const Item&, int);
	PUBLIC_FUNCTION(void, AddItems, Vector<Item>);
	PUBLIC_FUNCTION(void, InsertItems, Vector<Item>, int);
	PUBLIC_FUNCTION(Item, GetItem, int);
	PUBLIC_FUNCTION(Vector<Item>, GetItems);
	PUBLIC_FUNCTION(void, RemoveItem, int);
	PUBLIC_FUNCTION(void, RemoveItem, const WString&);
	PUBLIC_FUNCTION(void, RemoveAllItems);
	PUBLIC_FUNCTION(UIHorizontalLayout*, GetItemsLayout);
	PUBLIC_FUNCTION(UIWidget*, GetItemSample);
	PUBLIC_FUNCTION(void, SetItemSample, UIWidget*);
	PUBLIC_FUNCTION(Sprite*, GetSelectionDrawable);
	PUBLIC_FUNCTION(void, SetSelectionDrawableLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetSelectionDrawableLayout);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnResEnableInHierarchyChanged);
	PROTECTED_FUNCTION(UIContextMenu*, CreateSubContext, WString&);
	PROTECTED_FUNCTION(UIWidget*, CreateItem, const Item&);
	PROTECTED_FUNCTION(Item, GetItemDef, int);
	PROTECTED_FUNCTION(UIWidget*, GetItemUnderPoint, const Vec2F&, int*);
	PROTECTED_FUNCTION(void, UpdateHover, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
}
END_META;

CLASS_BASES_META(o2::UIMenuPanel::Item)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::UIMenuPanel::Item)
{
	PUBLIC_FIELD(text).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(subItems).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(onClick);
}
END_META;
CLASS_METHODS_META(o2::UIMenuPanel::Item)
{
}
END_META;
