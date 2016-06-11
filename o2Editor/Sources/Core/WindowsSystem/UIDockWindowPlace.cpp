#include "UIDockWindowPlace.h"
#include "Render\Render.h"

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
				Vec2F c1 = layout.GetAbsoluteRect().Center();
				Vec2F c2 = mNeighborMin->layout.GetAbsoluteRect().Center();
				Vec2F n = (c2 - c1).Normalized().Perpendicular()*30.0f;
				o2Render.DrawBezierCurveArrow(c1, c1 + n, c2 + n, c2, Color4::Blue());
			}

			if (mNeighborMax)
			{
				Vec2F c1 = layout.GetAbsoluteRect().Center();
				Vec2F c2 = mNeighborMax->layout.GetAbsoluteRect().Center();
				Vec2F n = (c2 - c1).Normalized().Perpendicular()*30.0f;
				o2Render.DrawBezierCurveArrow(c1, c1 + n, c2 + n, c2, Color4::Red());
			}
		}
	}

	bool UIDockWindowPlace::IsUnderPoint(const Vec2F& point)
	{
		return layout.GetAbsoluteRect().IsInside(point);
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
				layout.offsetLeft = border;

			if (neighborMax != nullptr)
				layout.offsetRight = -border;
		}
		else
		{
			mDragHandleLayoutMin = Layout(Vec2F(0, 0), Vec2F(1, 0), Vec2F(0, -border2), Vec2F(0, 0));
			mDragHandleLayoutMax = Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(0, 0), Vec2F(0, border2));
			mDragHandleMin.cursorType = CursorType::SizeNS;
			mDragHandleMax.cursorType = CursorType::SizeNS;

			if (neighborMin != nullptr)
				layout.offsetBottom = border;

			if (neighborMax != nullptr)
				layout.offsetTop = -border;
		}

		mDragHandleAreaMin = mDragHandleLayoutMin.Calculate(layout.GetAbsoluteRect());
		mDragHandleAreaMax = mDragHandleLayoutMax.Calculate(layout.GetAbsoluteRect());
	}

	TwoDirection UIDockWindowPlace::GetResizibleDir() const
	{
		return mResizibleDir;
	}

	void UIDockWindowPlace::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		UIWidget::UpdateLayout(forcible, withChildren);
		mDragHandleAreaMin = mDragHandleLayoutMin.Calculate(layout.GetAbsoluteRect());
		mDragHandleAreaMax = mDragHandleLayoutMax.Calculate(layout.GetAbsoluteRect());
	}

	void UIDockWindowPlace::OnDragHandleMinMoved(const Vec2F& delta)
	{
		if (mResizibleDir == TwoDirection::Horizontal)
		{
			float anchorDelta = delta.x / mParent->layout.width;
			layout.anchorLeft += anchorDelta;

			mNeighborMin->layout.anchorRight += anchorDelta;
		}
		else
		{
			float anchorDelta = delta.y / mParent->layout.height;
			layout.anchorBottom += anchorDelta;
			mNeighborMin->layout.anchorTop += anchorDelta;
		}
	}

	void UIDockWindowPlace::OnDragHandleMaxMoved(const Vec2F& delta)
	{
		if (mResizibleDir == TwoDirection::Horizontal)
		{
			float anchorDelta = delta.x / mParent->layout.width;
			layout.anchorRight += anchorDelta;

			mNeighborMax->layout.anchorLeft += anchorDelta;
		}
		else
		{
			float anchorDelta = delta.y / mParent->layout.height;
			layout.anchorTop += anchorDelta;
			mNeighborMax->layout.anchorBottom += anchorDelta;
		}
	}

	void UIDockWindowPlace::CheckInteractable()
	{
		interactable = mChilds.CountMatch([](auto x) { return x->GetType() == UIDockWindowPlace::type; }) == 0;

		for (auto child : mChilds)
		{
			if (child->GetType() == UIDockWindowPlace::type)
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