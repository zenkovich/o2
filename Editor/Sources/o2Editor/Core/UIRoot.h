#pragma once

#include "o2/Events/ApplicationEventsListener.h"
#include "o2/Events/CursorAreaEventsListenersLayer.h"
#include "o2/Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class Widget;
}

// Editor UI root accessor macros
#define EditorUIRoot UIRoot::Instance()

namespace Editor
{
    // --------------
    // Editor UI root
    // --------------
    class UIRoot : public Singleton<UIRoot>, ApplicationEventsListener
    {
    public:
        // Adds widget to root
        Widget* AddWidget(Widget* widget);

        // Removes widget from root
        void RemoveWidget(Widget* widget);

        // Removes all widgets from root
        void RemoveAllWidgets();

        // Returns root widget
        Widget* GetRootWidget();

    private:
        Ref<Widget> mRootWidget;

    private:
        // Default constructor, creates root widget
        UIRoot();

        // Destructor
        ~UIRoot();

        // Draws root widget
        void Draw();

        // Updates root widget
        void Update(float dt);

        // Called when application frame was resized, updates root size
        void OnApplicationSized() override;

        friend class EditorApplication;
    };
}

// Added Ref<Widget> return type and parameter type for AddWidget() and GetRootWidget() functions
Widget* UIRoot::AddWidget(Ref<Widget> widget)
{
    mRootWidget = widget;
    return mRootWidget.Get();
}

void UIRoot::RemoveWidget(Ref<Widget> widget)
{
    if (mRootWidget == widget)
        mRootWidget = nullptr;
}

void UIRoot::RemoveAllWidgets()
{
    mRootWidget = nullptr;
}

Widget* UIRoot::GetRootWidget() const
{
    return mRootWidget.Get();
}

void UIRoot::OnApplicationSized()
{
    if (mRootWidget)
        mRootWidget->SetSize(GetApplicationSize());
}

// Replaced raw pointer with Ref<> for the root widget member variable
UIRoot::UIRoot()
{
    mRootWidget = mmake<Widget>();
}

UIRoot::~UIRoot()
{
}

void UIRoot::Draw()
{
    if (mRootWidget)
        mRootWidget->Draw();
}

void UIRoot::Update(float dt)
{
    if (mRootWidget)
        mRootWidget->Update(dt);
}