#pragma once

#include "Events/CursorEventsListener.h"
#include "UI/ScrollArea.h"

namespace o2
{
	class Sprite;
	class UIButton;
	class UIVerticalLayout;

	// -----------------------
	// Context menu ui element
	// -----------------------
	class UIContextMenu: public UIScrollArea, public CursorEventsListener
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
			WString          text;	   // @SERIALIZABLE
			WString          shortcut; // @SERIALIZABLE
			Ptr<ImageAsset>  icon;	   // @SERIALIZABLE
			Vector<Item>     subItems; // @SERIALIZABLE
			ClickFunc        onClick;  // On click event	

			Item();

			Item(const WString& text, Vector<Item> subItems, Ptr<ImageAsset> icon = nullptr);

			Item(const WString& text, const Function<void()> onClick, const WString& shortcut = "",
				 Ptr<ImageAsset> icon = nullptr);

			~Item();

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
		void Show(Ptr<UIContextMenu> parent, const Vec2F& position = o2Input.GetCursorPos());

		// Shows context
		void Show(const Vec2F& position = o2Input.GetCursorPos());

		// Add item
		Ptr<UIWidget> AddItem(const Item& item);

		// Inserts item at position
		Ptr<UIWidget> InsertItem(const Item& item, int position);

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

		// Removes all items
		void RemoveAllItems();

		// Returns items vertical layout
		Ptr<UIVerticalLayout> GetItemsLayout() const;

		// Returns item sample
		Ptr<UIWidget> GetItemSample() const;

		// Returns selection drawable
		Ptr<Sprite> GetSelectionDrawable() const;

		// Sets selection drawable layout
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		// Returns is listener scrollable
		bool IsScrollable() const;

		SERIALIZABLE(UIContextMenu);

	protected:
		static Ptr<UIContextMenu> mVisibleContextMenu; // Current visible context menu
		const float mOpenSubMenuDelay = 0.8f;

		Ptr<UIContextMenu>    mParentContextMenu;	   // Parent visible context menu
		Ptr<UIContextMenu>    mChildContextMenu;	   // Child visible context menu

		Ptr<UIVerticalLayout> mLayout;                 // Items layout
		Vector<ClickFunc>     mClickFunctions;         // Items click functions
		Ptr<UIWidget>         mItemSample;             // Item sample
		Ptr<Sprite>           mSelectionDrawable;      // Selection sprite @SERIALIZABLE
		Layout                mSelectionLayout;        // Selection layout, result selection area depends on selected item @SERIALIZABLE

		RectF                 mCurrentSelectionRect;   // Current selection rectangle (for smoothing)
		RectF                 mTargetSelectionRect;    // Target selection rectangle (over selected item)
		Vec2F                 mLastSelectCheckCursor;  // Last cursor position on selection check

		int                   mSelectedItem;           // Index of selected item
		int                   mPressedItem;            // Index of pressed item
		float                 mSelectSubContextTime;   // Time to appearing selected sub context

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
		Ptr<UIWidget> CreateItem(const Item& item);

		// Returns item info
		Item GetItemDef(int idx) const;

		// Calls when visible was changed
		void OnVisibleChanged();

		// Returns item widget under point and stores index in idxPtr, if not null
		Ptr<UIWidget> GetItemUnderPoint(const Vec2F& point, int* idxPtr);

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

		// Hides context with his parent
		void HideWithParent();

		// Hides context with his child
		void HideWithChild();

		friend class UIManager;
	};
}
