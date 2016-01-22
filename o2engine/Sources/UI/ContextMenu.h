#pragma once

#include "Events/CursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "UI/ScrollArea.h"
#include "Utils/Delegates.h"
#include "Utils/ShortcutKeys.h"

namespace o2
{
	class Sprite;
	class UIButton;
	class UIVerticalLayout;

	// -----------------
	// Context menu item
	// -----------------
	class UIContextMenuItem: public UIWidget
	{
	public:
		Function<void()> onClick;  // Click function
		ShortcutKeys     shortcut; // Shortuct keys

		// Default constructor
		UIContextMenuItem();

		// Copy-constructor
		UIContextMenuItem(const UIContextMenuItem& other);

		// Destructor
		~UIContextMenuItem();

		// Copy-operator
		UIContextMenuItem& operator=(const UIContextMenuItem& other);

		// Returns sub context menu
		UIContextMenu* GetSubMenu() const;

		SERIALIZABLE(UIContextMenuItem);

	protected:
		UIContextMenu* mSubMenu; // Context sub menu

	protected:
		// Calls when child widget was added
		void OnChildAdded(UIWidget* child);

		friend class UIContextMenu;
	};

	// -----------------------
	// Context menu ui element
	// -----------------------
	class UIContextMenu: public UIScrollArea, public CursorEventsListener, public KeyboardEventsListener
	{
	public:
		typedef Function<void()> ClickFunc;

	public:
		// ---------
		// Menu item
		// ---------
		class Item:public ISerializable
		{
		public:
			WString      text;	   // @SERIALIZABLE
			ShortcutKeys shortcut; // @SERIALIZABLE
			ImageAsset*  icon;	   // @SERIALIZABLE
			Vector<Item> subItems; // @SERIALIZABLE
			ClickFunc    onClick;  // On click event	

			Item();

			Item(const WString& text, Vector<Item> subItems, ImageAsset* icon = nullptr);

			Item(const WString& text, const Function<void()> onClick, ImageAsset* icon = nullptr,
				 const ShortcutKeys& shortcut = ShortcutKeys());

			~Item();

			static Item Separator();

			bool operator==(const Item& other) const;

			SERIALIZABLE(Item);
		};

	public:
		// Default constructor
		UIContextMenu();

		// Constructor from items
		UIContextMenu(Vector<Item> items);

		// Copy-constructor
		UIContextMenu(const UIContextMenu& other);

		// Destructor
		~UIContextMenu();

		// Copy operator
		UIContextMenu& operator=(const UIContextMenu& other);

		// Updates drawables, states and widget
		void Update(float dt);

		// Draws widget
		void Draw();

		// Show from parent context
		void Show(UIContextMenu* parent, const Vec2F& position = o2Input.GetCursorPos());

		// Shows context
		void Show(const Vec2F& position = o2Input.GetCursorPos());

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
		UIVerticalLayout* GetItemsLayout() const;

		// Returns item sample
		UIContextMenuItem* GetItemSample() const;

		// Returns items separator sample
		UIWidget* GetSeparatorSample() const;

		// Returns selection drawable
		Sprite* GetSelectionDrawable() const;

		// Sets selection drawable layout
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		// Sets minimal fitting size
		void SetMinFitSize(float size);

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		// Returns is listener scrollable
		bool IsScrollable() const;

		SERIALIZABLE(UIContextMenu);

	protected:
		static UIContextMenu* mVisibleContextMenu; // Current visible context menu
		const float mOpenSubMenuDelay = 0.8f;

		float              mFitSizeMin = 40.0f;     // Minimal fitting size @SERIALIZABLE
						   
		UIContextMenu*     mParentContextMenu;	   // Parent visible context menu
		UIContextMenu*     mChildContextMenu;	   // Child visible context menu
						   
		UIVerticalLayout*  mLayout;                 // Items layout
		UIContextMenuItem* mItemSample;             // Item sample @SERIALIZABLE
		UIWidget*          mSeparatorSample;        // Items separator sample @SERIALIZABLE
		Sprite*            mSelectionDrawable;      // Selection sprite @SERIALIZABLE
		Layout             mSelectionLayout;        // Selection layout, result selection area depends on selected item @SERIALIZABLE
						   
		RectF              mCurrentSelectionRect;   // Current selection rectangle (for smoothing)
		RectF              mTargetSelectionRect;    // Target selection rectangle (over selected item)
		Vec2F              mLastSelectCheckCursor;  // Last cursor position on selection check
						   
		UIContextMenuItem* mSelectedItem;           // Index of selected item
		float              mSelectSubContextTime;   // Time to appearing selected sub context

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Fits size by items
		void FitSize();

		// Fits position in screen
		void FitPosition();

		// Special drawing for contexts
		void SpecialDraw();

		// Creates item widget
		UIContextMenuItem* CreateItem(const Item& item);

		// Returns item info
		Item GetItemDef(int idx) const;

		// Calls when visible was changed
		void OnVisibleChanged();

		// Returns item widget under point and stores index in idxPtr, if not null
		UIContextMenuItem* GetItemUnderPoint(const Vec2F& point);

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

		// Calls when key was released
		void OnKeyPressed(const Input::Key& key);

		// Hides context with his parent
		void HideWithParent();

		// Hides context with his child
		void HideWithChild();

		friend class UIManager;
		friend class UIMenuPanel;
	};
}
