#pragma once

#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Reflection/Type.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/Ref.h"

// User interfaces manager access macros
#define o2UI UIManager::Instance()

namespace o2
{
    class Button;
    class CustomDropDown;
    class CustomList;
    class DropDown;
    class EditBox;
    class HorizontalLayout;
    class HorizontalProgress;
    class HorizontalScrollBar;
    class Image;
    class Label;
    class List;
    class ScrollArea;
    class Toggle;
    class VerticalLayout;
    class VerticalProgress;
    class VerticalScrollBar;
    class Widget;
    class Window;

#undef CreateWindow

    // ------------------------------------------------
    // UI manager, contains all root widgets and styles
    // ------------------------------------------------
    class UIManager : public Singleton<UIManager>, public RefCounterable
    {
    public:
        // Destructor
        ~UIManager();

        // Loads widgets style
        void LoadStyle(const String& stylesPath);

        // Saves style
        void SaveStyle(const String& stylesPath);

        // Clears style widgets
        void ClearStyle();

        // Adds widget style
        void AddWidgetStyle(Widget* widget, const String& style);

        // Creates widget by style
        Widget* CreateWidget(const Type& type, const String& style = "standard");

        // Returns widget style by name
        Widget* GetWidgetStyle(const Type& type, const String& style);

        // Returns widget style by name
        template<typename _type>
        _type* GetWidgetStyle(const String& style);

        // Removes widget style
        template<typename _type>
        void RemoveWidgetStyle(const String& style);

        // Creates widget by style
        template<typename _type>
        _type* CreateWidget(const String& style = "standard");

        // Creates button by style
        Button* CreateButton(const WString& caption, const Function<void()>& onClick = Function<void()>(),
                             const String& style = "standard");

        // Creates button by style
        Window* CreateWindow(const WString& caption, const String& style = "standard");

        // Creates label by style
        Label* CreateLabel(const WString& text, const String& style = "standard");

        // Creates horizontal layout
        HorizontalLayout* CreateHorLayout();

        // Creates vertical layout
        VerticalLayout* CreateVerLayout();

        // Creates horizontal progress bar by style
        HorizontalProgress* CreateHorProgress(const String& style = "standard");

        // CreatesCreates vertical progress bar by style
        VerticalProgress* CreateVerProgress(const String& style = "standard");

        // Creates horizontal scroll bar by style
        HorizontalScrollBar* CreateHorScrollBar(const String& style = "standard");

        // Creates vertical scroll bar by style
        VerticalScrollBar* CreateVerScrollBar(const String& style = "standard");

        // Creates scroll area
        ScrollArea* CreateScrollArea(const String& style = "standard");

        // Creates edit box
        EditBox* CreateEditBox(const String& style = "standard");

        // Creates list view
        CustomList* CreateCustomList(const String& style = "standard");

        // Creates text list
        List* CreateList(const String& style = "standard");

        // Creates custom dropdown
        CustomDropDown* CreateCustomDropdown(const String& style = "standard");

        // Creates text dropdown
        DropDown* CreateDropdown(const String& style = "standard");

        // Creates toggle
        Toggle* CreateToggle(const WString& caption, const String& style = "standard");

        // Creates image
        Image* CreateImage(const String& name);

        // Sets widget as focused
        void FocusWidget(Widget* widget);

        // Returns current focused widget
        Widget* GetFocusedWidget() const;

        // Sets next widget focused
        void FocusNextWidget();

        // Draws context menus and top drawing widgets
        void Draw();

        // Checks last focused and unfocused widget
        void Update();

        // Registering widget for drawing at top of all regular widgets
        void DrawWidgetAtTop(Widget* widget);

        // Returns all styles widgets
        const Vector<ActorAssetRef>& GetWidgetStyles() const;

    protected:
        Ref<LogStream> mLog; // UI Log stream

        Ref<Widget>         mFocusedWidget;        // Current selected widget
        Vector<Ref<Widget>> mLastUnfocusedWidgets; // Widget that was unfocused on last frame
        Vector<Ref<Widget>> mLastFocusedWidgets;   // Widget that was focused on last frame
        Vector<Ref<Widget>> mFocusableWidgets;     // List of selectable widgets

        Vector<Widget*> mTopWidgets; // Top widgets, drawing after mScreenWidget 

        Vector<ActorAssetRef> mStyleSamples; // Style widgets

    protected:
        // Registers focusable widget
        static void RegisterFocusableWidget(Widget* widget);

        // Unregisters focusable widget
        static void UnregisterFocusableWidget(Widget* widget);

    protected:
        // Default constructor
        UIManager();

        // Tries to load style "ui_style.json"
        void TryLoadStyle();

        friend class Application;
        friend class BaseApplication;
        friend class CustomDropDown;
        friend class Tree;
        friend class Widget;
    };

    template<typename _type>
    _type* UIManager::GetWidgetStyle(const String& style /*= "standard"*/)
    {
        for (auto styleWidget : mStyleSamples)
        {
            if (TypeOf(_type) == styleWidget->GetActor()->GetType())
            {
                if (style == styleWidget->GetActor()->GetName())
                    return dynamic_cast<_type*>(styleWidget->GetActor());
            }
        }

        return nullptr;
    }

    template<typename _type>
    void UIManager::RemoveWidgetStyle(const String& style)
    {
        for (auto styleWidget : mStyleSamples)
        {
            if (TypeOf(_type) == styleWidget->GetActor()->GetType())
            {
                if (style == styleWidget->GetActor()->GetName())
                {
                    mStyleSamples.Remove(styleWidget);
                    break;
                }
            }
        }
    }

    template<typename _type>
    _type* UIManager::CreateWidget(const String& style /*= "standard"*/)
    {
        _type* sample = GetWidgetStyle<_type>(style);
        if (!sample)
            sample = GetWidgetStyle<_type>("standard");

        _type* res = nullptr;

        if (sample)
            res = sample->template CloneAs<_type>();
        else
            res = mnew _type();

//         if (TypeOf(_type) != TypeOf(ContextMenu))
//             res->SetEnabledForcible(true);

        return res;
    }

}
