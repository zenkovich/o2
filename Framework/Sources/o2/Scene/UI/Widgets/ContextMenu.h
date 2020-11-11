#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Events/ShortcutKeysListener.h"
#include "o2/Scene/UI/Widgets/PopupWidget.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"
#include "o2/Utils/System/ShortcutKeys.h"

namespace o2
{
	class Sprite;
	class Button;
	class ContextMenu;
	class VerticalLayout;
	class ContextMenuItem;

	// -----------------------
	// Context menu ui element
	// -----------------------
	class ContextMenu: public PopupWidget
	{
	public:
		// ---------
		// Menu item
		// ---------
		class Item: public ISerializable, ShortcutKeysListener
		{
		public:
			static WString separatorText;

		public:
			PROPERTIES(Item);
			PROPERTY(ShortcutKeys, shortcut, SetShortcut, GetShortcut);

			WString       text;	    // @SERIALIZABLE
			WString       group;    // @SERIALIZABLE
			ImageAssetRef icon;	    // @SERIALIZABLE

			Vector<Item*> subItems;  // @SERIALIZABLE

			bool checked;   // @SERIALIZABLE
			bool checkable; // @SERIALIZABLE

			Function<void()>     onClick;   // On click event	
			Function<void(bool)> onChecked; // On checked event	

			ContextMenuItem* widget = nullptr;

		public:
			Item();

			Item(const WString& text, const Vector<Item*>& subItems, const WString& group = "",
				 const ImageAssetRef& icon = ImageAssetRef());

			Item(const WString& text, const Function<void()> onClick, const WString& group = "",
				 const ImageAssetRef& icon = ImageAssetRef(), const ShortcutKeys& shortcut = ShortcutKeys());

			Item(const WString& text, bool checked, Function<void(bool)> onChecked = Function<void(bool)>(),
				 const WString& group = "", const ImageAssetRef& icon = ImageAssetRef(),
				 const ShortcutKeys& shortcut = ShortcutKeys());

			~Item();

			bool operator==(const Item& other) const;

			void SetShortcut(const ShortcutKeys& shortcut);
			const ShortcutKeys& GetShortcut() const;

			void SetMaxPriority();
			void SetMinPriority();

			static Item* Separator();

			SERIALIZABLE(Item);

		private:
			ShortcutKeys mShortcut; // @SERIALIZABLE

		private:
			// This event calling when shortcut hit and this listener has max priority
			void OnShortcutPressed() override;
		};

	public:
		// Default constructor
		ContextMenu();

		// Constructor from items
		ContextMenu(const Vector<Item*>& items);

		// Copy-constructor
		ContextMenu(const ContextMenu& other);

		// Destructor
		~ContextMenu();

		// Copy operator
		ContextMenu& operator=(const ContextMenu& other);

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Show from parent context
		void Show(PopupWidget* parent, const Vec2F& position = o2Input.GetCursorPos()) override;

		// Shows context
		void Show(const Vec2F& position = o2Input.GetCursorPos());

		// Add item
		void AddItem(Item* item);

		// Adds item by path ("node/sub node/target")
		void AddItem(const WString& path, const Function<void()>& clickFunc = Function<void()>(),
					 const ImageAssetRef& icon = ImageAssetRef(), const ShortcutKeys& shortcut = ShortcutKeys());

		// Adds item by path ("node/sub node/target")
		void AddToggleItem(const WString& path, bool value,
						   const Function<void(bool)>& clickFunc = Function<void(bool)>(),
						   const ImageAssetRef& icon = ImageAssetRef(),
						   const ShortcutKeys& shortcut = ShortcutKeys());

		// Inserts item at position
		void InsertItem(Item* item, int position);

		// Adds array of items
		void AddItems(const Vector<Item*>& items);

		// Inserts array of items at position
		void InsertItems(const Vector<Item*>& items, int position);

		// Returns item's widget at position
		ContextMenuItem* GetItemWidget(int position);

		// Returns index of item by path
		int FindItem(const WString& text) const;

		// Returns item's widget by path
		ContextMenuItem* FindItemWidget(const WString& path) const;

		// Sets item at position
		void SetItem(int position, Item* item);

		// Returns array of all items
		const Vector<Item*>& GetItems() const;

		// Removes item at position
		void RemoveItem(int position);

		// Removes item by path
		void RemoveItem(const WString& path);

		// Removes all items
		void RemoveAllItems();

		// Sets item at position checked (if it checkable)
		void SetItemChecked(int position, bool checked);

		// Returns is item checked
		bool IsItemChecked(int position) const;

		// Returns items vertical layout
		VerticalLayout* GetItemsLayout() const;

		// Returns item sample
		ContextMenuItem* GetItemSample() const;

		// Returns items separator sample
		Widget* GetSeparatorSample() const;

		// Returns selection drawable
		Sprite* GetSelectionDrawable() const;

		// Sets selection drawable layout
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		// Sets maximum visible items count
		void SetMaxItemsVisible(int count);

		// Sets context menu items shortcut maximum priority
		void SetItemsMaxPriority();

		// Sets context menu items shortcut minimum priority
		void SetItemsMinPriority();

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(ContextMenu);

	protected:
		const float mOpenSubMenuDelay = 0.8f; // Sub menu opening delay when cursor hover it

		int mMaxVisibleItems = 100; // Maximum visible items @SERIALIZABLE

		Vector<Item*>   mItems;                 // Items list
		VerticalLayout* mItemsLayout = nullptr; // Items layout; builds when opening context
		bool            mItemsBuilt = false;    // Is items layout is actual and built

		ContextMenuItem* mItemSample = nullptr;      // Item sample @SERIALIZABLE
		Widget*          mSeparatorSample = nullptr; // Items separator sample @SERIALIZABLE

		Sprite* mSelectionDrawable = nullptr; // Selection sprite @SERIALIZABLE
		Layout  mSelectionLayout;             // Selection layout, result selection area depends on selected item @SERIALIZABLE

		RectF mCurrentSelectionRect;  // Current selection rectangle (for smoothing)
		RectF mTargetSelectionRect;   // Target selection rectangle (over selected item)
		Vec2F mLastSelectCheckCursor; // Last cursor position on selection check

		ContextMenuItem* mSelectedItem = nullptr;       // Index of selected item
		float            mSelectSubContextTime = -1.0f; // Time to appearing selected sub context

	protected:
		// Fits size by items
		void FitSizeAndPosition(const Vec2F& position) override;

		// Hides context with his parent
		void HideWithParent() override;

		// Hides context with his child
		void HideWithChild() override;

		// Special drawing for contexts
		void SpecialDraw() override;

		// Build items layout
		void RebuildItems();

		// Creates context items by path ("node/sub node/target")
		Vector<Item*>* CreateItemsByPath(WString& path);

		// Returns item info
		Item* GetItem(int idx) const;

		// Returns item widget under point and stores index in idxPtr, if not null
		ContextMenuItem* GetItemUnderPoint(const Vec2F& point);

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		friend class UIManager;
		friend class MenuPanel;
	};

	// -----------------
	// Context menu item
	// -----------------
	class ContextMenuItem: public Widget
	{
	public:
		PROPERTIES(ContextMenuItem);
		PROPERTY(WString, text, SetText, GetText); // Text

	public:
		Function<void()>     onClick;   // Click function
		Function<void(bool)> onChecked; // Checked function, calls when check was changed and item is checkable

	public:
		// Default constructor
		ContextMenuItem();

		// Copy-constructor
		ContextMenuItem(const ContextMenuItem& other);

		// Destructor
		~ContextMenuItem();

		// Copy-operator
		ContextMenuItem& operator=(const ContextMenuItem& other);

		// Sets item
		void Setup(ContextMenu::Item* item);

		// Sets text
		void SetText(const WString& text);

		// Returns text
		WString GetText() const;

		// Returns sub context menu
		ContextMenu* GetSubMenu() const;

		// Sets checked icon
		void SetChecked(bool checked);

		// Returns is menu checked
		bool IsChecked() const;

		// Sets item checkable
		void SetCheckable(bool checkable);

		// Returns is menu item can be checked
		bool IsCheckable() const;

		// Sets item enabled
		void SetEnabled(bool enabled) override;

		// Returns is item enabled
		bool IsEnabled() const override;

		// Enables item
		void Enable();

		// DIsables item
		void Disable();

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(ContextMenuItem);

	protected:
		ContextMenu* mSubMenu; // Context sub menu

		bool mChecked = false;   // Is menu item checked
		bool mCheckable = false; // Is menu item can be checked

	protected:
		// It is called when child widget was added
		void OnChildAdded(Widget* child) override;

		friend class ContextMenu;
	};
}

CLASS_BASES_META(o2::ContextMenu)
{
	BASE_CLASS(o2::PopupWidget);
}
END_META;
CLASS_FIELDS_META(o2::ContextMenu)
{
	PROTECTED_FIELD(mOpenSubMenuDelay).DEFAULT_VALUE(0.8f);
	PROTECTED_FIELD(mMaxVisibleItems).DEFAULT_VALUE(100).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mItems);
	PROTECTED_FIELD(mItemsLayout).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mItemsBuilt).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mItemSample).DEFAULT_VALUE(nullptr).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSeparatorSample).DEFAULT_VALUE(nullptr).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelectionDrawable).DEFAULT_VALUE(nullptr).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelectionLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCurrentSelectionRect);
	PROTECTED_FIELD(mTargetSelectionRect);
	PROTECTED_FIELD(mLastSelectCheckCursor);
	PROTECTED_FIELD(mSelectedItem).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mSelectSubContextTime).DEFAULT_VALUE(-1.0f);
}
END_META;
CLASS_METHODS_META(o2::ContextMenu)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Show, PopupWidget*, const Vec2F&);
	PUBLIC_FUNCTION(void, Show, const Vec2F&);
	PUBLIC_FUNCTION(void, AddItem, Item*);
	PUBLIC_FUNCTION(void, AddItem, const WString&, const Function<void()>&, const ImageAssetRef&, const ShortcutKeys&);
	PUBLIC_FUNCTION(void, AddToggleItem, const WString&, bool, const Function<void(bool)>&, const ImageAssetRef&, const ShortcutKeys&);
	PUBLIC_FUNCTION(void, InsertItem, Item*, int);
	PUBLIC_FUNCTION(void, AddItems, const Vector<Item*>&);
	PUBLIC_FUNCTION(void, InsertItems, const Vector<Item*>&, int);
	PUBLIC_FUNCTION(ContextMenuItem*, GetItemWidget, int);
	PUBLIC_FUNCTION(int, FindItem, const WString&);
	PUBLIC_FUNCTION(ContextMenuItem*, FindItemWidget, const WString&);
	PUBLIC_FUNCTION(void, SetItem, int, Item*);
	PUBLIC_FUNCTION(const Vector<Item*>&, GetItems);
	PUBLIC_FUNCTION(void, RemoveItem, int);
	PUBLIC_FUNCTION(void, RemoveItem, const WString&);
	PUBLIC_FUNCTION(void, RemoveAllItems);
	PUBLIC_FUNCTION(void, SetItemChecked, int, bool);
	PUBLIC_FUNCTION(bool, IsItemChecked, int);
	PUBLIC_FUNCTION(VerticalLayout*, GetItemsLayout);
	PUBLIC_FUNCTION(ContextMenuItem*, GetItemSample);
	PUBLIC_FUNCTION(Widget*, GetSeparatorSample);
	PUBLIC_FUNCTION(Sprite*, GetSelectionDrawable);
	PUBLIC_FUNCTION(void, SetSelectionDrawableLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetSelectionDrawableLayout);
	PUBLIC_FUNCTION(void, SetMaxItemsVisible, int);
	PUBLIC_FUNCTION(void, SetItemsMaxPriority);
	PUBLIC_FUNCTION(void, SetItemsMinPriority);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, FitSizeAndPosition, const Vec2F&);
	PROTECTED_FUNCTION(void, HideWithParent);
	PROTECTED_FUNCTION(void, HideWithChild);
	PROTECTED_FUNCTION(void, SpecialDraw);
	PROTECTED_FUNCTION(void, RebuildItems);
	PROTECTED_FUNCTION(Vector<Item*>*, CreateItemsByPath, WString&);
	PROTECTED_FUNCTION(Item*, GetItem, int);
	PROTECTED_FUNCTION(ContextMenuItem*, GetItemUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, UpdateHover, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
}
END_META;

CLASS_BASES_META(o2::ContextMenuItem)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(o2::ContextMenuItem)
{
	PUBLIC_FIELD(text);
	PUBLIC_FIELD(onClick);
	PUBLIC_FIELD(onChecked);
	PROTECTED_FIELD(mSubMenu);
	PROTECTED_FIELD(mChecked).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mCheckable).DEFAULT_VALUE(false);
}
END_META;
CLASS_METHODS_META(o2::ContextMenuItem)
{

	PUBLIC_FUNCTION(void, Setup, ContextMenu::Item*);
	PUBLIC_FUNCTION(void, SetText, const WString&);
	PUBLIC_FUNCTION(WString, GetText);
	PUBLIC_FUNCTION(ContextMenu*, GetSubMenu);
	PUBLIC_FUNCTION(void, SetChecked, bool);
	PUBLIC_FUNCTION(bool, IsChecked);
	PUBLIC_FUNCTION(void, SetCheckable, bool);
	PUBLIC_FUNCTION(bool, IsCheckable);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(void, Enable);
	PUBLIC_FUNCTION(void, Disable);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, OnChildAdded, Widget*);
}
END_META;

CLASS_BASES_META(o2::ContextMenu::Item)
{
	BASE_CLASS(o2::ISerializable);
	BASE_CLASS(o2::ShortcutKeysListener);
}
END_META;
CLASS_FIELDS_META(o2::ContextMenu::Item)
{
	PUBLIC_FIELD(shortcut);
	PUBLIC_FIELD(text).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(group).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(icon).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(subItems).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(checked).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(checkable).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(onClick);
	PUBLIC_FIELD(onChecked);
	PUBLIC_FIELD(widget).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mShortcut).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::ContextMenu::Item)
{

	PUBLIC_FUNCTION(void, SetShortcut, const ShortcutKeys&);
	PUBLIC_FUNCTION(const ShortcutKeys&, GetShortcut);
	PUBLIC_FUNCTION(void, SetMaxPriority);
	PUBLIC_FUNCTION(void, SetMinPriority);
	PUBLIC_STATIC_FUNCTION(Item*, Separator);
	PRIVATE_FUNCTION(void, OnShortcutPressed);
}
END_META;
