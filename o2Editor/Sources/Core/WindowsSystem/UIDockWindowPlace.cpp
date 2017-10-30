#include "UIDockWindowPlace.h"

#include "Core/WindowsSystem/UIDockableWindow.h"
#include "Render/Render.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace Editor
{
	UIDockWindowPlace::UIDockWindowPlace():
		UIWidget(), DrawableCursorEventsListener(this), mDragHandleLayoutMin(Vec2F(), Vec2F(), Vec2F(), Vec2F()),
		mDragHandleLayoutMax(Vec2F(), Vec2F(), Vec2F(), Vec2F()), mNeighborMax(nullptr), mNeighborMin(nullptr)
	{
		InitializeDragHandle();
		RetargetStatesAnimations();
		UpdateLayout();
	}

	UIDockWindowPlace::UIDockWindowPlace(const UIDockWindowPlace& other):
		UIWidget(other), DrawableCursorEventsListener(this), mNeighborMax(nullptr), mNeighborMin(nullptr)
	{
		InitializeDragHandle();
		RetargetStatesAnimations();
		UpdateLayout();
	}

	UIDockWindowPlace::~UIDockWindowPlace()
	{}

	UIDockWindowPlace& UIDockWindowPlace::operator=(const UIDockWindowPlace& other)
	{
		UIWidget::operator=(other);
		mDragHandleLayoutMin = other.mDragHandleLayoutMin;
		return *this;
	}

	void UIDockWindowPlace::Draw()
	{
		UIWidget::Draw();

		mDragHandleMin.OnDrawn();
		mDragHandleMax.OnDrawn();

		if (o2Input.IsKeyDown(VK_F1))
		{
			if (mDragHandleMin.IsInteractable())
				o2Render.DrawRectFrame(mDragHandleAreaMin, Color4::Blue());

			if (mDragHandleMax.IsInteractable())
				o2Render.DrawRectFrame(mDragHandleAreaMax, Color4::Red());

			if (mNeighborMin)
			{
				Vec2F c1 = layout->GetWorldRect().Center();
				Vec2F c2 = mNeighborMin->layout->GetWorldRect().Center();
				Vec2F n = (c2 - c1).Normalized().Perpendicular()*30.0f;
				o2Render.DrawBezierCurveArrow(c1, c1 + n, c2 + n, c2, Color4::Blue());
			}

			if (mNeighborMax)
			{
				Vec2F c1 = layout->GetWorldRect().Center();
				Vec2F c2 = mNeighborMax->layout->GetWorldRect().Center();
				Vec2F n = (c2 - c1).Normalized().Perpendicular()*30.0f;
				o2Render.DrawBezierCurveArrow(c1, c1 + n, c2 + n, c2, Color4::Red());
			}
		}
	}

	bool UIDockWindowPlace::IsUnderPoint(const Vec2F& point)
	{
		return layout->GetWorldRect().IsInside(point);
	}

	void UIDockWindowPlace::SetResizibleDir(TwoDirection dir, float border,
											UIDockWindowPlace* neighborMin, UIDockWindowPlace* neighborMax)
	{
		mResizibleDir = dir;
		float border2 = border*2.0f;

		mDragHandleMin.interactable = neighborMin != nullptr;
		mNeighborMin = neighborMin;

		mNeighborMax = neighborMax;
		mDragHandleMax.interactable = neighborMax != nullptr;

		if (dir == TwoDirection::Horizontal)
		{
			mDragHandleLayoutMin = Layout(Vec2F(0, 0), Vec2F(0, 1), Vec2F(-border2, 0), Vec2F(0, 0));
			mDragHandleLayoutMax = Layout(Vec2F(1, 0), Vec2F(1, 1), Vec2F(border2, 0), Vec2F(0, 0));
			mDragHandleMin.cursorType = CursorType::SizeWE;
			mDragHandleMax.cursorType = CursorType::SizeWE;

			if (neighborMin != nullptr)
				layout->offsetLeft = border;

			if (neighborMax != nullptr)
				layout->offsetRight = -border;
		}
		else
		{
			mDragHandleLayoutMin = Layout(Vec2F(0, 0), Vec2F(1, 0), Vec2F(0, -border2), Vec2F(0, 0));
			mDragHandleLayoutMax = Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(0, 0), Vec2F(0, border2));
			mDragHandleMin.cursorType = CursorType::SizeNS;
			mDragHandleMax.cursorType = CursorType::SizeNS;

			if (neighborMin != nullptr)
				layout->offsetBottom = border;

			if (neighborMax != nullptr)
				layout->offsetTop = -border;
		}

		mDragHandleAreaMin = mDragHandleLayoutMin.Calculate(layout->GetWorldRect());
		mDragHandleAreaMax = mDragHandleLayoutMax.Calculate(layout->GetWorldRect());
	}

	TwoDirection UIDockWindowPlace::GetResizibleDir() const
	{
		return mResizibleDir;
	}

	void UIDockWindowPlace::ArrangeChildWindows()
	{
		Vector<UIDockableWindow*> windows;
		for (auto child : mChildren)
		{
			if (child->GetType() == TypeOf(UIDockableWindow))
				windows.Add(dynamic_cast<UIDockableWindow*>(child));
		}

		windows.Sort([](UIDockableWindow* a, UIDockableWindow* b) { return a->mTabPosition < b->mTabPosition; });

		if (windows.Count() == 1)
		{
			windows[0]->SetNonTabState();
		}
		else
		{
			float offset = 0;
			int pos = 0;
			for (auto window : windows)
			{
				window->SetTabState(offset, pos, window == mChildren[0]);
				offset += window->GetTabWidth();

				pos++;
			}

			if (!windows.IsEmpty())
			{
				if (!windows.Any([&](UIDockableWindow* x) { return x->mTabActive; }))
					SetActiveTab(windows[0]);
			}
		}
	}

	void UIDockWindowPlace::SetActiveTab(UIDockableWindow* window)
	{
		Vector<UIDockableWindow*> windows;
		for (auto child : mChildren)
		{
			if (child->GetType() == TypeOf(UIDockableWindow))
				windows.Add(dynamic_cast<UIDockableWindow*>(child));
		}

		for (auto window : windows)
		{
			if (window->mTabActive)
			{
				window->mTabActive = false;

				if (auto state = window->GetStateObject("tabActive"))
					state->SetState(false);

				break;
			}
		}

		if (window)
		{
			window->mTabActive = true;

			if (auto state = window->GetStateObject("tabActive"))
				state->SetState(true);
		}
	}

	void UIDockWindowPlace::UpdateLayout(bool withChildren /*= true*/)
	{
		UIWidget::UpdateLayout(withChildren);
		mDragHandleAreaMin = mDragHandleLayoutMin.Calculate(layout->GetWorldRect());
		mDragHandleAreaMax = mDragHandleLayoutMax.Calculate(layout->GetWorldRect());
	}

	void UIDockWindowPlace::OnDragHandleMinMoved(const Vec2F& delta)
	{
		if (mResizibleDir == TwoDirection::Horizontal)
		{
			float anchorDelta = delta.x / mParentWidget->layout->width;
			layout->anchorLeft += anchorDelta;

			mNeighborMin->layout->anchorRight += anchorDelta;
		}
		else
		{
			float anchorDelta = delta.y / mParentWidget->layout->height;
			layout->anchorBottom += anchorDelta;
			mNeighborMin->layout->anchorTop += anchorDelta;
		}
	}

	void UIDockWindowPlace::OnDragHandleMaxMoved(const Vec2F& delta)
	{
		if (mResizibleDir == TwoDirection::Horizontal)
		{
			float anchorDelta = delta.x / mParentWidget->layout->width;
			layout->anchorRight += anchorDelta;

			mNeighborMax->layout->anchorLeft += anchorDelta;
		}
		else
		{
			float anchorDelta = delta.y / mParentWidget->layout->height;
			layout->anchorTop += anchorDelta;
			mNeighborMax->layout->anchorBottom += anchorDelta;
		}
	}

	void UIDockWindowPlace::CheckInteractable()
	{
		interactable = mChildren.CountMatch([](auto x) { return x->GetType() == TypeOf(UIDockWindowPlace); }) == 0;

		for (auto child : mChildren)
		{
			if (child->GetType() == TypeOf(UIDockWindowPlace))
				((UIDockWindowPlace*)child)->CheckInteractable();
		}
	}

	void UIDockWindowPlace::InitializeDragHandle()
	{
		mDragHandleMin.isUnderPoint = [&](const Vec2F& point) { return mDragHandleAreaMin.IsInside(point); };
		mDragHandleMin.onMoved = [&](const Input::Cursor& cursor) { OnDragHandleMinMoved(cursor.delta); };

		mDragHandleMax.isUnderPoint = [&](const Vec2F& point) { return mDragHandleAreaMax.IsInside(point); };
		mDragHandleMax.onMoved = [&](const Input::Cursor& cursor) { OnDragHandleMaxMoved(cursor.delta); };
	}

}

CLASS_META(Editor::UIDockWindowPlace)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::DrawableCursorEventsListener);

	PROTECTED_FIELD(mResizibleDir);
	PROTECTED_FIELD(mNeighborMin);
	PROTECTED_FIELD(mDragHandleMin);
	PROTECTED_FIELD(mDragHandleLayoutMin);
	PROTECTED_FIELD(mDragHandleAreaMin);
	PROTECTED_FIELD(mNeighborMax);
	PROTECTED_FIELD(mDragHandleMax);
	PROTECTED_FIELD(mDragHandleLayoutMax);
	PROTECTED_FIELD(mDragHandleAreaMax);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetResizibleDir, TwoDirection, float, UIDockWindowPlace*, UIDockWindowPlace*);
	PUBLIC_FUNCTION(TwoDirection, GetResizibleDir);
	PUBLIC_FUNCTION(void, ArrangeChildWindows);
	PUBLIC_FUNCTION(void, SetActiveTab, UIDockableWindow*);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, UpdateLayout, bool);
	PROTECTED_FUNCTION(void, OnDragHandleMinMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, OnDragHandleMaxMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, CheckInteractable);
	PROTECTED_FUNCTION(void, InitializeDragHandle);
}
END_META;
