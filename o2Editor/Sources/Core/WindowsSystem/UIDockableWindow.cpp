#include "UIDockableWindow.h"

#include "Core/WindowsSystem/UIDockWindowPlace.h"
#include "Events/CursorEventsListener.h"

UIDockableWindow::UIDockableWindow():
	UIWindow(), mDocked(false)
{
	InitializeDragHandles();
	SetDocked(false);
	mDockingFrameSample = mnew Sprite();

	RetargetStatesAnimations();
}

UIDockableWindow::UIDockableWindow(const UIDockableWindow& other):
	UIWindow(other), mDocked(false)
{
	InitializeDragHandles();
	SetDocked(false);
	mDockingFrameSample = other.mDockingFrameSample->Clone();
	InitializeDockFrameAppearanceAnim();

	RetargetStatesAnimations();
}

UIDockableWindow::~UIDockableWindow()
{
	mDockingFrameSample.Release();
}

void UIDockableWindow::Update(float dt)
{
	UIWindow::Update(dt);

	const float lerpFrameCoef = 10.0f;
	if (mDockingFrameTarget != mDockingFrameCurrent)
	{
		mDockingFrameCurrent = Math::Lerp(mDockingFrameCurrent, mDockingFrameTarget, dt*lerpFrameCoef);
		mDockingFrameSample->rect = mDockingFrameCurrent;
	}
	mDockingFrameAppearance.Update(dt);
}

void UIDockableWindow::Draw()
{
	UIWindow::Draw();
	mDockingFrameSample->Draw();
}

UIDockableWindow& UIDockableWindow::operator=(const UIDockableWindow& other)
{
	UIWindow::operator=(other);

	mDockingFrameSample = other.mDockingFrameSample->Clone();

	return *this;
}

void UIDockableWindow::SetDocked(bool docked)
{
	mDocked = docked;

	auto dockedState = state["docked"];
	if (dockedState)
		*dockedState = docked;

	mTopDragHandle.interactable = !docked;
	mBottomDragHandle.interactable = !docked;
	mLeftDragHandle.interactable = !docked;
	mRightDragHandle.interactable = !docked;
	mLeftTopDragHandle.interactable = !docked;
	mLeftBottomDragHandle.interactable = !docked;
	mRightTopDragHandle.interactable = !docked;
	mRightBottomDragHandle.interactable = !docked;
}

bool UIDockableWindow::IsDocked() const
{
	return mDocked;
}

Ptr<Sprite> UIDockableWindow::GetDockingFrameSample() const
{
	return mDockingFrameSample;
}

void UIDockableWindow::InitializeDockFrameAppearanceAnim()
{
	mDockingFrameAppearance.SetTarget(this);
	*mDockingFrameAppearance.AddAnimationValue<float>(&mDockingFrameSample->transparency) =
		AnimatedValue<float>::EaseInOut(0, 1, 0.3f);

	*mDockingFrameAppearance.AddAnimationValue<float>(&transparency) =
		AnimatedValue<float>::EaseInOut(1, 0, 0.15f);

	mDockingFrameAppearance.GoToBegin();
}

void UIDockableWindow::InitializeDragHandles()
{
	mHeadDragHandle.onMoved = Function<void(const Input::Cursor&)>(this, &UIDockableWindow::OnMoved);
	mHeadDragHandle.onCursorPressed = Function<void()>(this, &UIDockableWindow::OnMoveBegin);
	mHeadDragHandle.onCursorReleased = Function<void()>(this, &UIDockableWindow::OnMoveCompleted);
}

void UIDockableWindow::OnMoved(const Input::Cursor& cursor)
{
	if (mDocked)
	{
		if (layout.GetAbsoluteRect().IsInside(cursor.mPosition))
			return;

		auto topDock = mParent->GetParent().Cast<UIDockWindowPlace>();
		auto parent = mParent;
		auto parentNeighbor = topDock->GetChilds().FindMatch([&](auto x) { return x != parent; });
		bool enableTopDock = parentNeighbor->GetChilds().Count() == 0;

		o2UI.AddWidget(this);

		for (auto child : parentNeighbor->GetChilds())
			topDock->AddChild(child);

		topDock->RemoveChild(parent);
		topDock->RemoveChild(parentNeighbor);
		topDock->CheckInteractable();

		SetDocked(false);
		layout.absLeftTop = cursor.mPosition + Vec2F(-30, 10);
		layout.absRightBottom = layout.absLeftTop + mNonDockSize.InvertedY();
		return;
	}

	layout.position += cursor.mDelta;

	auto listenersUnderCursor = o2Events.GetAllCursorListenersUnderCursor(cursor.mId);
	auto dockPlaceListener = listenersUnderCursor.FindMatch([](const Ptr<CursorEventsListener>& x) { 
		return dynamic_cast<const UIDockWindowPlace*>(x.Get()) != nullptr;
	});

	if (dockPlaceListener)
	{
		auto dockPlace = dynamic_cast<UIDockWindowPlace*>(dockPlaceListener.Get());

		RectF dockPlaceRect = dockPlace->layout.GetAbsoluteRect();

		RectF leftZone(dockPlaceRect.left, dockPlaceRect.bottom,
					   Math::Lerp(dockPlaceRect.left, dockPlaceRect.right, mDockSizeCoef), dockPlaceRect.top);

		RectF rightZone(Math::Lerp(dockPlaceRect.right, dockPlaceRect.left, mDockSizeCoef), dockPlaceRect.bottom,
						dockPlaceRect.right, dockPlaceRect.top);

		RectF bottomZone(dockPlaceRect.left, dockPlaceRect.bottom,
						dockPlaceRect.right, Math::Lerp(dockPlaceRect.bottom, dockPlaceRect.top, mDockSizeCoef));

		if (bottomZone.IsInside(cursor.mPosition))
		{
			mDockingFrameAppearance.PlayForward();
			mDockingFrameTarget = bottomZone;
		}
		else if (rightZone.IsInside(cursor.mPosition))
		{
			mDockingFrameAppearance.PlayForward();
			mDockingFrameTarget = rightZone;
		} 
		else if (leftZone.IsInside(cursor.mPosition))
		{
			mDockingFrameAppearance.PlayForward();
			mDockingFrameTarget = leftZone;
		}
		else
		{
			mDockingFrameAppearance.PlayBack();
			mDockingFrameTarget = layout.GetAbsoluteRect();
		}
	}
}

void UIDockableWindow::OnMoveCompleted()
{
	Vec2F cursorPos = o2Input.cursorPos;
	auto listenersUnderCursor = o2Events.GetAllCursorListenersUnderCursor(0);
	auto dockPlaceListener = listenersUnderCursor.FindMatch([](const Ptr<CursorEventsListener>& x) {
		return dynamic_cast<const UIDockWindowPlace*>(x.Get()) != nullptr;
	});

	Ptr<UIDockWindowPlace> targetDock;
	int dockPosition = -1;
	RectF dockZoneRect;

	if (dockPlaceListener)
	{
		auto dockPlace = dynamic_cast<UIDockWindowPlace*>(dockPlaceListener.Get());

		RectF dockPlaceRect = dockPlace->layout.GetAbsoluteRect();

		RectF leftZone(dockPlaceRect.left, dockPlaceRect.bottom,
					   Math::Lerp(dockPlaceRect.left, dockPlaceRect.right, mDockSizeCoef), dockPlaceRect.top);

		RectF rightZone(Math::Lerp(dockPlaceRect.right, dockPlaceRect.left, mDockSizeCoef), dockPlaceRect.bottom,
						dockPlaceRect.right, dockPlaceRect.top);

		RectF bottomZone(dockPlaceRect.left, dockPlaceRect.bottom,
						 dockPlaceRect.right, Math::Lerp(dockPlaceRect.bottom, dockPlaceRect.top, mDockSizeCoef));

		if (bottomZone.IsInside(cursorPos))
		{
			targetDock = dockPlace;
			dockPosition = 0;
			dockZoneRect = bottomZone;
		}
		else if (rightZone.IsInside(cursorPos))
		{
			targetDock = dockPlace;
			dockPosition = 1;
			dockZoneRect = rightZone;
		}
		else if (leftZone.IsInside(cursorPos))
		{
			targetDock = dockPlace;
			dockPosition = 2;
			dockZoneRect = leftZone;
		}
	}

	if (targetDock)
	{
		mNonDockSize = layout.size;
		RectF dockPlaceRect = targetDock->layout.GetAbsoluteRect();

		Ptr<UIDockWindowPlace> emptyDock = mnew UIDockWindowPlace();
		Ptr<UIDockWindowPlace> windowDock = mnew UIDockWindowPlace();

		emptyDock->name = "empty";
		windowDock->name = "window";

		emptyDock->layout = UIWidgetLayout::Both();
		windowDock->layout = UIWidgetLayout::Both();

		for (auto child : targetDock->GetChilds())
			emptyDock->AddChild(child);

		targetDock->AddChild(emptyDock);
		targetDock->AddChild(windowDock);
		targetDock->interactable = false;

		if (dockPosition == 0) //bottom
		{
			emptyDock->layout.anchorBottom = mDockSizeCoef;
			windowDock->layout.anchorTop = mDockSizeCoef;
			windowDock->layout.offsetTop = -mDockBorder;
			windowDock->SetResizibleSide(Side::Top);
		}
		else if (dockPosition == 1) //right
		{
			emptyDock->layout.anchorRight = 1.0f - mDockSizeCoef;
			windowDock->layout.anchorLeft = 1.0f - mDockSizeCoef;
			windowDock->layout.offsetLeft = mDockBorder;
			windowDock->SetResizibleSide(Side::Left);
		}
		else if (dockPosition == 2) //left
		{
			emptyDock->layout.anchorLeft = mDockSizeCoef;
			windowDock->layout.anchorRight = mDockSizeCoef;
			windowDock->layout.offsetRight = -mDockBorder;
			windowDock->SetResizibleSide(Side::Right);
		}

		windowDock->AddChild(this);
		layout = UIWidgetLayout::Both();
		SetDocked(true);

		mDockingFrameAppearance.PlayBack();
		mDockingFrameTarget = layout.GetAbsoluteRect();
	}
}

void UIDockableWindow::OnMoveBegin()
{
	if (mDocked)
		mDragOffset = (Vec2F)o2Input.cursorPos - layout.absLeftTop;
}
