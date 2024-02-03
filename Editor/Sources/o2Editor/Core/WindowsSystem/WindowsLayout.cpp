#include "o2Editor/stdafx.h"
#include "WindowsLayout.h"

#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/WindowsSystem/DockWindowPlace.h"
#include "o2Editor/Core/WindowsSystem/DockableWindow.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"

namespace Editor
{
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T>
    using WeakRef = std::weak_ptr<T>;

    void WindowsLayout::WindowDockPlace::RetrieveLayout(const Ref<Widget>& widget)
    {
        if (widget->GetType() == TypeOf(DockWindowPlace))
        {
            anchors.Set(widget->layout->GetAnchorLeft(), widget->layout->GetAnchorTop(),
                widget->layout->GetAnchorRight(), widget->layout->GetAnchorBottom());

            for (const auto& child : widget->GetChildWidgets())
            {
                if (child->GetType() == TypeOf(DockWindowPlace))
                {
                    childs.emplace_back(WindowDockPlace());
                    childs.back().RetrieveLayout(child);
                }
                else if (child->GetType() == TypeOf(DockableWindow) && child->IsEnabled())
                {
                    windows.emplace_back(child->name);

                    if (Ref<DockableWindow> dockableWindow = DynamicCast<DockableWindow>(child))
                    {
                        if (dockableWindow->IsTabActive())
                            active = child->name;
                    }
                }
            }
        }
    }

    bool WindowsLayout::WindowDockPlace::operator==(const WindowDockPlace& other) const
    {
        return anchors == other.anchors && childs == other.childs && windows == other.windows;
    }

    bool WindowsLayout::operator==(const WindowsLayout& other) const
    {
        return mainDock == other.mainDock && windows == other.windows;
    }

    void WindowsLayout::RestoreDock(Ref<WindowDockPlace> dockDef, Ref<DockWindowPlace> dockWidget)
    {
        PushEditorScopeOnStack scope;

        Vector<Ref<DockWindowPlace>> childDockWidgets;
        for (const auto& child : dockDef->childs)
        {
            dockWidget->interactable = false;

            Ref<DockWindowPlace> newDock = mmake<DockWindowPlace>();
            newDock->name = "dock place";
            dockWidget->AddChild(newDock);

            *newDock->layout = WidgetLayout::BothStretch();
            newDock->layout->anchorMin = child.anchors.LeftBottom();
            newDock->layout->anchorMax = child.anchors.RightTop();
            childDockWidgets.emplace_back(newDock);
        }

        int idx = 0;
        for (const auto& child : dockDef->childs)
        {
            Ref<DockWindowPlace> newDock = childDockWidgets[idx++];

            if ((child.anchors.left < 1.0f && child.anchors.left > 0.0f) ||
                (child.anchors.right < 1.0f && child.anchors.right > 0.0f))
            {
                Ref<DockWindowPlace> neighborMin = childDockWidgets.FindOrDefault([&](const Ref<DockWindowPlace>& x) {
                    return Math::Equals(x->layout->GetAnchorRight(), newDock->layout->GetAnchorLeft()) && x != newDock;
                });

                Ref<DockWindowPlace> neighborMax = childDockWidgets.FindOrDefault([&](const Ref<DockWindowPlace>& x) {
                    return Math::Equals(x->layout->GetAnchorLeft(), newDock->layout->GetAnchorRight()) && x != newDock;
                });

                newDock->SetResizibleDir(TwoDirection::Horizontal, 1.5f, neighborMin, neighborMax);

                if (neighborMin)
                    neighborMin->SetResizibleDir(TwoDirection::Horizontal, 1.5f, neighborMin->mNeighborMin, newDock);

                if (neighborMax)
                    neighborMax->SetResizibleDir(TwoDirection::Horizontal, 1.5f, newDock, neighborMax->mNeighborMax);
            }

            if ((child.anchors.top < 1.0f && child.anchors.top > 0.0f) ||
                (child.anchors.bottom < 1.0f && child.anchors.bottom > 0.0f))
            {
                Ref<DockWindowPlace> neighborMin = childDockWidgets.FindOrDefault([&](const Ref<DockWindowPlace>& x) {
                    return Math::Equals(x->layout->GetAnchorTop(), newDock->layout->GetAnchorBottom()) && x != newDock;
                });

                Ref<DockWindowPlace> neighborMax = childDockWidgets.FindOrDefault([&](const Ref<DockWindowPlace>& x) {
                    return Math::Equals(x->layout->GetAnchorBottom(), newDock->layout->GetAnchorTop()) && x != newDock;
                });

                newDock->SetResizibleDir(TwoDirection::Vertical, 1.5f, neighborMin, neighborMax);

                if (neighborMin)
                    neighborMin->SetResizibleDir(TwoDirection::Vertical, 1.5f, neighborMin->mNeighborMin, newDock);

                if (neighborMax)
                    neighborMax->SetResizibleDir(TwoDirection::Vertical, 1.5f, newDock, neighborMax->mNeighborMax);
            }
        }

        idx = 0;
        for (const auto& child : dockDef->childs)
        {
            Ref<DockWindowPlace> newDock = childDockWidgets[idx++];
            RestoreDock(Ref<WindowDockPlace>(&child), newDock);
        }

        Ref<DockableWindow> activeTabWindow = nullptr;
        for (const auto& wnd : dockDef->windows)
        {
            if (const auto window = o2EditorWindows.mEditorWindows.FindOrDefault([&](const Ref<IEditorWindow>& x) {
                return x->mWindow->GetName() == wnd;
            }))
            {
                if (Ref<DockableWindow> dockWnd = DynamicCast<DockableWindow>(window->mWindow))
                {
                    dockWnd->mNonDockSize = dockWnd->layout->size;
                    dockWidget->AddChild(dockWnd);
                    *dockWnd->layout = WidgetLayout::BothStretch();
                    dockWnd->SetDocked(true);
                    dockWnd->Show();

                    if (dockDef->active == wnd)
                        activeTabWindow = dockWnd;
                }
            }
        }

        dockWidget->ArrangeChildWindows();

        if (activeTabWindow)
            activeTabWindow->SetTabActive();

        CleanEmptyDocks(dockWidget);
    }

    void WindowsLayout::CleanEmptyDocks(Ref<DockWindowPlace> dockPlace)
    {
        PushEditorScopeOnStack scope;

        const auto& childs = dockPlace->GetChildWidgets();
        for (const auto& child : childs)
        {
            if (const auto& dockChild = DynamicCast<DockWindowPlace>(child))
            {
                if (dockChild->GetChildWidgets().empty())
                {
                    dockPlace->RemoveChild(child);
                }
                else
                {
                    CleanEmptyDocks(dockChild);
                }
            }
        }
    }
}#include <Ref.hpp>
#include <WeakRef.hpp>

using Ref = shared_ptr;
using WeakRef = weak_ptr;

// Function declaration
void CleanEmptyDocks(const Ref<Editor__WindowsLayout__WindowDockPlace>& dockPlace);

// Class declaration
class Editor__WindowsLayout {};

class Editor__WindowsLayout__WindowDockPlace {
public:
    // Class members with Ref<>
    Ref<Editor__WindowsLayout__WindowDockPlace> m_parent;
    Ref<const Editor__WindowsLayout__WindowDockPlace> m_child;
    
    // Function with const Ref<>
    void RemoveChild(const Ref<const Editor__WindowsLayout__WindowDockPlace>& child) {}
    
    const Ref<Editor__WindowsLayout__WindowDockPlace>& GetChildWidgets() const {}
};

// Function definition
void CleanEmptyDocks(const Ref<Editor__WindowsLayout__WindowDockPlace>& dockPlace) {
    if (dockPlace->m_parent) {
        for (const auto& child : dockPlace->m_child->GetChildWidgets()) {
            CleanEmptyDocks(child);
            
            if (child->GetChildWidgets().empty()) {
                dockPlace->RemoveChild(child);
            }
        }
    }
}

// --- META ---

DECLARE_CLASS(Editor__WindowsLayout, Editor__WindowsLayout);

DECLARE_CLASS(Editor__WindowsLayout__WindowDockPlace, Editor__WindowsLayout__WindowDockPlace);
// --- END META ---