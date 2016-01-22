#pragma once

#include "Events/CursorEventsListener.h"
#include "UI/ContextMenu.h"
#include "UI/Widget.h"
#include "Utils/Delegates.h"

namespace o2
{
	class UIHorizontalLayout;

	// -------------
	// UI Menu panel
	// -------------
	class UIMenuPanel: public UIWidget, public CursorEventsListener
	{
	public:
		typedef Function<void()> ClickFunc;

		// ---------
		// Menu item
		// ---------
		class Item: public ISerializable
		{
		public:
			WString                     text;     // @SERIALIZABLE
			Vector<UIContextMenu::Item> subItems; // @SERIALIZABLE
			ClickFunc                   onClick;  // On click event	

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
		void Update(float dt);

		// Draws widget
		void Draw();

		// Add item
		UIWidget* AddItem(const Item& item);

		// Adds item by path ("node/sub node/target")
		UIWidget* AddItem(const WString& path, const Function<void()>& clickFunc = Function<void()>(), 
						  ImageAsset* icon = nullptr, const ShortcutKeys& shortcut = ShortcutKeys());

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

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		SERIALIZABLE(UIMenuPanel);

	protected:
		UIHorizontalLayout* mLayout;                 // Items layout
		UIWidget*           mItemSample;             // Item sample @SERIALIZABLE
		Vector<ClickFunc>   mClickFunctions;         // Items click functions
		Sprite*             mSelectionDrawable;      // Selection sprite @SERIALIZABLE
		Layout              mSelectionLayout;        // Selection layout, result selection area depends on selected item @SERIALIZABLE
							 
		RectF               mCurrentSelectionRect;   // Current selection rectangle (for smoothing)
		RectF               mTargetSelectionRect;    // Target selection rectangle (over selected item)
		Vec2F               mLastSelectCheckCursor;  // Last cursor position on selection check
							 
		int                 mSelectedItem;           // Index of selected item
		float               mSelectSubContextTime;   // Time to appearing selected sub context
		UIContextMenu*      mOpenedContext;          // Last opened context in menu

	protected:
		// Creates item widget
		UIWidget* CreateItem(const Item& item);

		// Returns item info
		Item GetItemDef(int idx) const;

		// Calls when visible was changed
		void OnVisibleChanged();

		// Returns item widget under point and stores index in idxPtr, if not null
		UIWidget* GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);
	};
}
