#include "WindowsLayout.h"

#include "Core/WindowsSystem/IEditorWindow.h"
#include "Core/WindowsSystem/UIDockWindowPlace.h"
#include "Core/WindowsSystem/UIDockableWindow.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "UI/Widget.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	void WindowsLayout::WindowDockPlace::RetrieveLayout(UIWidget* widget)
	{
		if (widget->GetType() == UIDockWindowPlace::type)
		{
			anchors.Set(widget->layout.GetAnchorLeft(), widget->layout.GetAnchorTop(),
						widget->layout.GetAnchorRight(), widget->layout.GetAnchorBottom());

			for (auto child : widget->GetChilds())
			{
				if (child->GetType() == UIDockWindowPlace::type)
				{
					childs.Add(WindowDockPlace());
					childs[childs.Count() - 1].RetrieveLayout(child);
				}
				else if (child->GetType() == UIDockableWindow::type)
				{
					windows.Add(child->name);
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

	void WindowsLayout::RestoreDock(WindowDockPlace* dockDef, UIDockWindowPlace* dockWidget)
	{
		Vector<UIDockWindowPlace*> childDockWidgets;
		for (auto child : dockDef->childs)
		{
			dockWidget->interactable = false;

			UIDockWindowPlace* newDock = mnew UIDockWindowPlace();
			newDock->name = "dockPlace";
			dockWidget->AddChild(newDock);

			newDock->layout = UIWidgetLayout::BothStretch();
			newDock->layout.anchorMin = child.anchors.LeftBottom();
			newDock->layout.anchorMax = child.anchors.RightTop();
			childDockWidgets.Add(newDock);
		}

		int idx = 0;
		for (auto child : dockDef->childs)
		{
			UIDockWindowPlace* newDock = childDockWidgets[idx++];

			if ((child.anchors.left < 1.0f && child.anchors.left > 0.0f) ||
				(child.anchors.right < 1.0f && child.anchors.right > 0.0f))
			{
				UIDockWindowPlace* neighborMin = childDockWidgets.FindMatch([&](UIDockWindowPlace* x) {
					return Math::Equals(x->layout.GetAnchorRight(), newDock->layout.GetAnchorLeft());
				});

				UIDockWindowPlace* neighborMax = childDockWidgets.FindMatch([&](UIDockWindowPlace* x) {
					return Math::Equals(x->layout.GetAnchorLeft(), newDock->layout.GetAnchorRight());
				});

				newDock->SetResizibleDir(TwoDirection::Horizontal, 1.5f, neighborMin, neighborMax);

				if (neighborMin)
					neighborMin->SetResizibleDir(TwoDirection::Horizontal, 1.5f, neighborMin->mNeighborMin, newDock);

				if (neighborMax)
					neighborMax->SetResizibleDir(TwoDirection::Horizontal, 1.5f, newDock, newDock->mNeighborMax);
			}

			if ((child.anchors.top < 1.0f && child.anchors.top > 0.0f) ||
				(child.anchors.bottom < 1.0f && child.anchors.bottom > 0.0f))
			{
				UIDockWindowPlace* neighborMin = childDockWidgets.FindMatch([&](UIDockWindowPlace* x) {
					return Math::Equals(x->layout.GetAnchorTop(), newDock->layout.GetAnchorBottom());
				});

				UIDockWindowPlace* neighborMax = childDockWidgets.FindMatch([&](UIDockWindowPlace* x) {
					return Math::Equals(x->layout.GetAnchorBottom(), newDock->layout.GetAnchorTop());
				});

				newDock->SetResizibleDir(TwoDirection::Vertical, 1.5f, neighborMin, neighborMax);

				if (neighborMin)
					neighborMin->SetResizibleDir(TwoDirection::Vertical, 1.5f, neighborMin->mNeighborMin, newDock);

				if (neighborMax)
					neighborMax->SetResizibleDir(TwoDirection::Vertical, 1.5f, newDock, neighborMax->mNeighborMax);
			}
		}

		idx = 0;
		for (auto child : dockDef->childs)
		{
			UIDockWindowPlace* newDock = childDockWidgets[idx++];
			RestoreDock(&child, newDock);
		}

		for (auto wnd : dockDef->windows)
		{
			auto window =
				o2EditorWindows.mEditorWindows.FindMatch([&](IEditorWindow* x) { return x->mWindow->GetName() == wnd; });

			if (window)
			{
				UIDockableWindow* dockWnd = (UIDockableWindow*)window->mWindow;

				dockWnd->mNonDockSize = dockWnd->layout.size;
				dockWidget->AddChild(dockWnd);
				dockWnd->layout = UIWidgetLayout::BothStretch();
				dockWnd->SetDocked(true);
			}
		}
	}

}