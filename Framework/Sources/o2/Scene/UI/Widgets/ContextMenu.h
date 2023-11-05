#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Events/ShortcutKeysListener.h"
#include "o2/Scene/UI/Widgets/PopupWidget.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"
#include "o2/Utils/System/ShortcutKeys.h"
#include "o2/Render/Sprite.h"

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

            WString       text;        // @SERIALIZABLE
            WString       group;    // @SERIALIZABLE
            ImageAssetRef icon;        // @SERIALIZABLE

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

        // Called when cursor pressed on this
        void OnCursorPressed(const Input::Cursor& cursor) override;

        // Called when cursor stay down during frame
        void OnCursorStillDown(const Input::Cursor& cursor) override;

        // Called when cursor released (only when cursor pressed this at previous time)
        void OnCursorReleased(const Input::Cursor& cursor) override;

        // Called when cursor moved on this (or moved outside when this was pressed)
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
        bool IsEnabled() const OPTIONAL_OVERRIDE;

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
        // Called when child widget was added
        void OnChildAdded(Widget* child) override;

        friend class ContextMenu;
    };
}
// --- META ---

CLASS_BASES_META(o2::ContextMenu)
{
    BASE_CLASS(o2::PopupWidget);
}
END_META;
CLASS_FIELDS_META(o2::ContextMenu)
{
    FIELD().PROTECTED().DEFAULT_VALUE(0.8f).NAME(mOpenSubMenuDelay);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(100).NAME(mMaxVisibleItems);
    FIELD().PROTECTED().NAME(mItems);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mItemsLayout);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mItemsBuilt);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mItemSample);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mSeparatorSample);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mSelectionDrawable);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mSelectionLayout);
    FIELD().PROTECTED().NAME(mCurrentSelectionRect);
    FIELD().PROTECTED().NAME(mTargetSelectionRect);
    FIELD().PROTECTED().NAME(mLastSelectCheckCursor);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSelectedItem);
    FIELD().PROTECTED().DEFAULT_VALUE(-1.0f).NAME(mSelectSubContextTime);
}
END_META;
CLASS_METHODS_META(o2::ContextMenu)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const Vector<Item*>&);
    FUNCTION().PUBLIC().CONSTRUCTOR(const ContextMenu&);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, Show, PopupWidget*, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(void, Show, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(void, AddItem, Item*);
    FUNCTION().PUBLIC().SIGNATURE(void, AddItem, const WString&, const Function<void()>&, const ImageAssetRef&, const ShortcutKeys&);
    FUNCTION().PUBLIC().SIGNATURE(void, AddToggleItem, const WString&, bool, const Function<void(bool)>&, const ImageAssetRef&, const ShortcutKeys&);
    FUNCTION().PUBLIC().SIGNATURE(void, InsertItem, Item*, int);
    FUNCTION().PUBLIC().SIGNATURE(void, AddItems, const Vector<Item*>&);
    FUNCTION().PUBLIC().SIGNATURE(void, InsertItems, const Vector<Item*>&, int);
    FUNCTION().PUBLIC().SIGNATURE(ContextMenuItem*, GetItemWidget, int);
    FUNCTION().PUBLIC().SIGNATURE(int, FindItem, const WString&);
    FUNCTION().PUBLIC().SIGNATURE(ContextMenuItem*, FindItemWidget, const WString&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetItem, int, Item*);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Item*>&, GetItems);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveItem, int);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveItem, const WString&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllItems);
    FUNCTION().PUBLIC().SIGNATURE(void, SetItemChecked, int, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsItemChecked, int);
    FUNCTION().PUBLIC().SIGNATURE(VerticalLayout*, GetItemsLayout);
    FUNCTION().PUBLIC().SIGNATURE(ContextMenuItem*, GetItemSample);
    FUNCTION().PUBLIC().SIGNATURE(Widget*, GetSeparatorSample);
    FUNCTION().PUBLIC().SIGNATURE(Sprite*, GetSelectionDrawable);
    FUNCTION().PUBLIC().SIGNATURE(void, SetSelectionDrawableLayout, const Layout&);
    FUNCTION().PUBLIC().SIGNATURE(Layout, GetSelectionDrawableLayout);
    FUNCTION().PUBLIC().SIGNATURE(void, SetMaxItemsVisible, int);
    FUNCTION().PUBLIC().SIGNATURE(void, SetItemsMaxPriority);
    FUNCTION().PUBLIC().SIGNATURE(void, SetItemsMinPriority);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuGroup);
    FUNCTION().PROTECTED().SIGNATURE(void, FitSizeAndPosition, const Vec2F&);
    FUNCTION().PROTECTED().SIGNATURE(void, HideWithParent);
    FUNCTION().PROTECTED().SIGNATURE(void, HideWithChild);
    FUNCTION().PROTECTED().SIGNATURE(void, SpecialDraw);
    FUNCTION().PROTECTED().SIGNATURE(void, RebuildItems);
    FUNCTION().PROTECTED().SIGNATURE(Vector<Item*>*, CreateItemsByPath, WString&);
    FUNCTION().PROTECTED().SIGNATURE(Item*, GetItem, int);
    FUNCTION().PROTECTED().SIGNATURE(ContextMenuItem*, GetItemUnderPoint, const Vec2F&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateHover, const Vec2F&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
}
END_META;

CLASS_BASES_META(o2::ContextMenuItem)
{
    BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(o2::ContextMenuItem)
{
    FIELD().PUBLIC().NAME(text);
    FIELD().PUBLIC().NAME(onClick);
    FIELD().PUBLIC().NAME(onChecked);
    FIELD().PROTECTED().NAME(mSubMenu);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mChecked);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mCheckable);
}
END_META;
CLASS_METHODS_META(o2::ContextMenuItem)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const ContextMenuItem&);
    FUNCTION().PUBLIC().SIGNATURE(void, Setup, ContextMenu::Item*);
    FUNCTION().PUBLIC().SIGNATURE(void, SetText, const WString&);
    FUNCTION().PUBLIC().SIGNATURE(WString, GetText);
    FUNCTION().PUBLIC().SIGNATURE(ContextMenu*, GetSubMenu);
    FUNCTION().PUBLIC().SIGNATURE(void, SetChecked, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsChecked);
    FUNCTION().PUBLIC().SIGNATURE(void, SetCheckable, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsCheckable);
    FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabled);
    FUNCTION().PUBLIC().SIGNATURE(void, Enable);
    FUNCTION().PUBLIC().SIGNATURE(void, Disable);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuGroup);
    FUNCTION().PROTECTED().SIGNATURE(void, OnChildAdded, Widget*);
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
    FIELD().PUBLIC().NAME(shortcut);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(text);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(group);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(icon);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(subItems);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(checked);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(checkable);
    FIELD().PUBLIC().NAME(onClick);
    FIELD().PUBLIC().NAME(onChecked);
    FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(widget);
    FIELD().PRIVATE().SERIALIZABLE_ATTRIBUTE().NAME(mShortcut);
}
END_META;
CLASS_METHODS_META(o2::ContextMenu::Item)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const WString&, const Vector<Item*>&, const WString&, const ImageAssetRef&);
    FUNCTION().PUBLIC().CONSTRUCTOR(const WString&, const Function<void()>, const WString&, const ImageAssetRef&, const ShortcutKeys&);
    FUNCTION().PUBLIC().CONSTRUCTOR(const WString&, bool, Function<void(bool)>, const WString&, const ImageAssetRef&, const ShortcutKeys&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetShortcut, const ShortcutKeys&);
    FUNCTION().PUBLIC().SIGNATURE(const ShortcutKeys&, GetShortcut);
    FUNCTION().PUBLIC().SIGNATURE(void, SetMaxPriority);
    FUNCTION().PUBLIC().SIGNATURE(void, SetMinPriority);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Item*, Separator);
    FUNCTION().PRIVATE().SIGNATURE(void, OnShortcutPressed);
}
END_META;
// --- END META ---
