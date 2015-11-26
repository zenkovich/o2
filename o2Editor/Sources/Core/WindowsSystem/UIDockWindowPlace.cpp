#include "UIDockWindowPlace.h"

UIDockWindowPlace::UIDockWindowPlace():
	UIWidget(), mDragHandleLayout(Vec2F(), Vec2F(), Vec2F(), Vec2F()), mDragSide(Side::None)
{
	InitializeDragHandle();
	RetargetStatesAnimations();
	UpdateLayout();
}

UIDockWindowPlace::UIDockWindowPlace(const UIDockWindowPlace& other):
	UIWidget(other)
{
	InitializeDragHandle();
	RetargetStatesAnimations();
	SetResizibleSide(other.mDragSide);
	UpdateLayout();
}

UIDockWindowPlace::~UIDockWindowPlace()
{}

UIDockWindowPlace& UIDockWindowPlace::operator=(const UIDockWindowPlace& other)
{
	UIWidget::operator=(other);
	mDragHandleLayout = other.mDragHandleLayout;
	return *this;
}

void UIDockWindowPlace::Draw()
{
	UIWidget::Draw();
	mDragHandleDepth = o2Render.GetDrawingDepth();
	if (o2Input.IsKeyDown(VK_F1))
		o2Render.DrawRectFrame(mDragHandleArea);
}

bool UIDockWindowPlace::IsUnderPoint(const Vec2F& point)
{
	return layout.GetAbsoluteRect().IsInside(point);
}

float UIDockWindowPlace::Depth()
{
	return 0;
}

void UIDockWindowPlace::SetResizibleSide(Side side)
{
	mDragSide = side;

	switch (side)
	{
		case Side::Left:
		mDragHandleLayout = Layout(Vec2F(0, 0), Vec2F(0, 1), Vec2F(-5, 0), Vec2F(0, 0));
		mDragHandle.cursorType = CursorType::SizeWE;
		break;

		case Side::Right:
		mDragHandleLayout = Layout(Vec2F(1, 0), Vec2F(1, 1), Vec2F(0, 0), Vec2F(5, 0));
		mDragHandle.cursorType = CursorType::SizeWE;
		break;

		case Side::Bottom:
		mDragHandleLayout = Layout(Vec2F(0, 0), Vec2F(1, 0), Vec2F(0, -5), Vec2F(0, 0));
		mDragHandle.cursorType = CursorType::SizeNS;
		break;

		case Side::Top:
		mDragHandleLayout = Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(0, 0), Vec2F(0, 5));
		mDragHandle.cursorType = CursorType::SizeNS;
		break;
	}

	mDragHandle.interactable = side != Side::None;
	mDragHandleArea = mDragHandleLayout.Calculate(layout.GetAbsoluteRect());
}

void UIDockWindowPlace::UpdateLayout(bool forcible /*= false*/)
{
	UIWidget::UpdateLayout(forcible);
	mDragHandleArea = mDragHandleLayout.Calculate(layout.GetAbsoluteRect());
}

void UIDockWindowPlace::OnDragHandleMoved(const Vec2F& delta)
{
	Ptr<UIWidget> _this = this;
	Ptr<UIWidget> neighbor = mParent->GetChilds().FindMatch([&](auto x) { return x != _this; });
	if (!neighbor)
	{
		o2Debug.LogError("Failed to size dock");
		return;
	}

	if (mDragSide == Side::Left)
	{
		float anchorDelta = delta.x / mParent->layout.width;
		layout.anchorLeft += anchorDelta;
		neighbor->layout.anchorRight += anchorDelta;
	}

	if (mDragSide == Side::Right)
	{
		float anchorDelta = delta.x / mParent->layout.width;
		layout.anchorRight += anchorDelta;
		neighbor->layout.anchorLeft += anchorDelta;
	}

	if (mDragSide == Side::Top)
	{
		float anchorDelta = delta.y / mParent->layout.height;
		layout.anchorTop += anchorDelta;
		neighbor->layout.anchorBottom += anchorDelta;
	}
}

void UIDockWindowPlace::CheckInteractable()
{
	interactable = mChilds.CountMatch([](auto x) { return x->GetTypeId() == UIDockWindowPlace::type->ID(); }) == 0;

	for (auto child : mChilds)
	{
		if (child->GetTypeId() == UIDockWindowPlace::type->ID())
			child.Cast<UIDockWindowPlace>()->CheckInteractable();
	}
}

void UIDockWindowPlace::InitializeDragHandle()
{
	mDragHandle.getDepth = [&]() { return mDragHandleDepth; };
	mDragHandle.isUnderPoint = [&](const Vec2F& point) { return mDragHandleArea.IsInside(point); };
	mDragHandle.onMoved = [&](const Input::Cursor& cursor) { OnDragHandleMoved(cursor.mDelta); };
}
