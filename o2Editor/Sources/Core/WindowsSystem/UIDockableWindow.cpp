#include "UIDockableWindow.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Core/WindowsSystem/UIDockWindowPlace.h"
#include "Events/CursorEventsListener.h"
#include "Events/EventSystem.h"
#include "UI/UIManager.h"

namespace Editor
{
	UIDockableWindow::UIDockableWindow():
		UIWindow()
	{
		InitializeDragHandles();
		SetDocked(false);
		mDockingFrameSample = mnew Sprite();

		RetargetStatesAnimations();
	}

	UIDockableWindow::UIDockableWindow(const UIDockableWindow& other):
		UIWindow(other)
	{
		InitializeDragHandles();
		SetDocked(false);
		mDockingFrameSample = other.mDockingFrameSample->Clone();
		InitializeDockFrameAppearanceAnim();

		if (mVisibleState)
			mVisibleState->onStateFullyFalse += THIS_FUNC(Undock);

		RetargetStatesAnimations();
	}

	UIDockableWindow::~UIDockableWindow()
	{
		delete mDockingFrameSample;
	}

	UIDockableWindow& UIDockableWindow::operator=(const UIDockableWindow& other)
	{
		UIWindow::operator=(other);

		mDockingFrameSample = other.mDockingFrameSample->Clone();

		if (mVisibleState)
			mVisibleState->onStateFullyFalse += THIS_FUNC(Undock);

		return *this;
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

	void UIDockableWindow::SetDocked(bool docked)
	{
		mDocked = docked;

		auto dockedState = state["docked"];
		if (dockedState)
			*dockedState = docked;

		mTopDragHandle.interactable         = !docked;
		mBottomDragHandle.interactable      = !docked;
		mLeftDragHandle.interactable        = !docked;
		mRightDragHandle.interactable       = !docked;
		mLeftTopDragHandle.interactable     = !docked;
		mLeftBottomDragHandle.interactable  = !docked;
		mRightTopDragHandle.interactable    = !docked;
		mRightBottomDragHandle.interactable = !docked;
	}

	bool UIDockableWindow::IsDocked() const
	{
		return mDocked;
	}

	Sprite* UIDockableWindow::GetDockingFrameSample() const
	{
		return mDockingFrameSample;
	}

	void UIDockableWindow::OnVisibleChanged()
	{
		UIWindow::OnVisibleChanged();

		if (!mResVisible)
			Undock();
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
		mHeadDragHandle.onMoved          = THIS_FUNC(OnMoved);
		mHeadDragHandle.onDblClicked     = THIS_FUNC(OnHeadDblCKicked);
		mHeadDragHandle.onCursorPressed  = THIS_FUNC(OnMoveBegin);
		mHeadDragHandle.onCursorReleased = THIS_FUNC(OnMoveCompleted);
	}

	void UIDockableWindow::OnHeadDblCKicked(const Input::Cursor& cursor)
	{
		if (IsDocked())
			Undock();
		else
		{
			Vec2F cursorPos = o2Input.cursorPos;
			auto listenersUnderCursor = o2Events.GetAllCursorListenersUnderCursor(0);
			auto dockPlaceListener = listenersUnderCursor.FindMatch([](CursorAreaEventsListener* x) {
				return dynamic_cast<UIDockWindowPlace*>(x) != nullptr;
			});

			if (dockPlaceListener)
				PlaceDock(dynamic_cast<UIDockWindowPlace*>(dockPlaceListener));
		}
	}

	void UIDockableWindow::OnMoved(const Input::Cursor& cursor)
	{
		if (mDocked)
		{
			if (!layout.GetAbsoluteRect().IsInside(cursor.position))
				Undock();

			return;
		}

		layout.position += cursor.delta;

		UIDockWindowPlace* targetDock;
		Side dockPosition = Side::None;
		RectF dockZoneRect;

		if (!TraceDock(targetDock, dockPosition, dockZoneRect))
			return;

		if (dockPosition != Side::None)
		{
			mDockingFrameAppearance.PlayForward();
			mDockingFrameTarget = dockZoneRect;
		}
		else
		{
			mDockingFrameAppearance.PlayBack();
			mDockingFrameTarget = layout.GetAbsoluteRect();
		}
	}

	void UIDockableWindow::OnMoveCompleted(const Input::Cursor& cursro)
	{
		UIDockWindowPlace* targetDock = nullptr;
		Side dockPosition = Side::None;
		RectF dockZoneRect;

		if (!TraceDock(targetDock, dockPosition, dockZoneRect) || targetDock == nullptr)
			return;

		bool allOnLine = targetDock->mParent && targetDock->mParent->GetChilds().All([&](auto x) {

			if (x->GetType() != TypeOf(UIDockWindowPlace))
				return false;

			UIDockWindowPlace* dock = (UIDockWindowPlace*)x;
			TwoDirection rd = dock->GetResizibleDir();

			if (dockPosition == Side::Left || dockPosition == Side::Right)
				return rd == TwoDirection::Horizontal;

			return rd == TwoDirection::Vertical;
		});

		if (allOnLine)
			PlaceLineDock(targetDock, dockPosition, dockZoneRect);
		else
			PlaceNonLineDock(targetDock, dockPosition);
	}

	void UIDockableWindow::OnMoveBegin(const Input::Cursor& cursor)
	{
		OnFocused();

		if (mDocked)
			mDragOffset = (Vec2F)o2Input.cursorPos - layout.absLeftTop;
	}

	bool UIDockableWindow::TraceDock(UIDockWindowPlace*& targetDock, Side& dockPosition, RectF& dockZoneRect)
	{
		Vec2F cursorPos = o2Input.cursorPos;
		auto listenersUnderCursor = o2Events.GetAllCursorListenersUnderCursor(0);
		auto dockPlaceListener = listenersUnderCursor.FindMatch([](CursorAreaEventsListener* x) {
			return dynamic_cast<UIDockWindowPlace*>(x) != nullptr;
		});

		if (dockPlaceListener)
		{
			auto dockPlace = dynamic_cast<UIDockWindowPlace*>(dockPlaceListener);

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
				dockPosition = Side::Bottom;
				dockZoneRect = bottomZone;
			}
			else if (rightZone.IsInside(cursorPos))
			{
				targetDock = dockPlace;
				dockPosition = Side::Right;
				dockZoneRect = rightZone;
			}
			else if (leftZone.IsInside(cursorPos))
			{
				targetDock = dockPlace;
				dockPosition = Side::Left;
				dockZoneRect = leftZone;
			}
		}

		return targetDock != nullptr;
	}

	void UIDockableWindow::PlaceDock(UIDockWindowPlace* targetDock)
	{
		mNonDockSize = layout.size;

		targetDock->AddChild(this);
		layout = UIWidgetLayout::BothStretch();
		SetDocked(true);

		mDockingFrameAppearance.PlayBack();
		mDockingFrameTarget = layout.GetAbsoluteRect();
	}

	void UIDockableWindow::PlaceNonLineDock(UIDockWindowPlace* targetDock, Side dockPosition)
	{
		mNonDockSize = layout.size;
		RectF dockPlaceRect = targetDock->layout.GetAbsoluteRect();

		UIDockWindowPlace* windowDock = mnew UIDockWindowPlace();
		windowDock->name = "window dock";
		windowDock->layout = UIWidgetLayout::BothStretch();

		UIDockWindowPlace* windowNeighborDock = mnew UIDockWindowPlace();
		windowNeighborDock->name = "empty dock";
		windowNeighborDock->layout = UIWidgetLayout::BothStretch();

		for (auto child : targetDock->GetChilds())
			windowNeighborDock->AddChild(child);

		targetDock->AddChild(windowNeighborDock);
		targetDock->AddChild(windowDock);
		targetDock->interactable = false;

		if (dockPosition == Side::Bottom)
		{
			windowNeighborDock->layout.anchorBottom = mDockSizeCoef;
			windowDock->layout.anchorTop = mDockSizeCoef;
			windowDock->layout.offsetTop = -mDockBorder;

			windowDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, nullptr, windowNeighborDock);
			windowNeighborDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, windowDock, nullptr);
		}
		else if (dockPosition == Side::Right)
		{
			windowNeighborDock->layout.anchorRight = 1.0f - mDockSizeCoef;
			windowDock->layout.anchorLeft = 1.0f - mDockSizeCoef;
			windowDock->layout.offsetLeft = mDockBorder;

			windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock, nullptr);
			windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nullptr, windowDock);
		}
		else if (dockPosition == Side::Left)
		{
			windowNeighborDock->layout.anchorLeft = mDockSizeCoef;
			windowDock->layout.anchorRight = mDockSizeCoef;
			windowDock->layout.offsetRight = -mDockBorder;

			windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nullptr, windowNeighborDock);
			windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowDock, nullptr);
		}

		windowDock->AddChild(this);
		layout = UIWidgetLayout::BothStretch();
		SetDocked(true);

		mDockingFrameAppearance.PlayBack();
		mDockingFrameTarget = layout.GetAbsoluteRect();
	}

	void UIDockableWindow::PlaceLineDock(UIDockWindowPlace* targetDock, Side dockPosition, RectF dockZoneRect)
	{
		mNonDockSize = layout.size;
		RectF dockPlaceRect = targetDock->layout.GetAbsoluteRect();

		UIDockWindowPlace* windowDock = mnew UIDockWindowPlace();
		windowDock->name = "window dock";
		windowDock->layout = UIWidgetLayout::BothStretch();

		UIDockWindowPlace* windowNeighborDock = targetDock;
		targetDock->mParent->AddChild(windowDock);

		if (dockPosition == Side::Bottom)
		{
			windowDock->layout.anchorBottom = windowNeighborDock->layout.anchorBottom;

			windowNeighborDock->layout.anchorBottom +=
				windowNeighborDock->layout.height*mDockSizeCoef / windowNeighborDock->mParent->layout.height;

			windowDock->layout.anchorTop = windowNeighborDock->layout.anchorBottom;

			if (targetDock->mNeighborMin)
			{
				auto nmin = windowNeighborDock->mNeighborMin;

				nmin->SetResizibleDir(TwoDirection::Vertical, mDockBorder, nmin->mNeighborMin, windowDock);
				windowDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, nmin, windowNeighborDock);
				windowNeighborDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, windowDock, windowNeighborDock->mNeighborMax);
			}
			else
			{
				windowDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, nullptr, windowNeighborDock);
				windowNeighborDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, windowDock, windowNeighborDock->mNeighborMax);
			}
		}
		else if (dockPosition == Side::Right)
		{
			windowDock->layout.anchorRight = windowNeighborDock->layout.anchorRight;

			windowNeighborDock->layout.anchorRight -=
				windowNeighborDock->layout.width*mDockSizeCoef / windowNeighborDock->mParent->layout.width;

			windowDock->layout.anchorLeft = windowNeighborDock->layout.anchorRight;

			if (targetDock->mNeighborMax)
			{
				auto nmax = windowNeighborDock->mNeighborMax;

				nmax->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowDock, nmax->mNeighborMax);
				windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock, nmax);
				windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock->mNeighborMin, windowDock);
			}
			else
			{
				windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock, nullptr);
				windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock->mNeighborMin, windowDock);
			}
		}
		else if (dockPosition == Side::Left)
		{
			windowDock->layout.anchorLeft = windowNeighborDock->layout.anchorLeft;

			windowNeighborDock->layout.anchorLeft +=
				windowNeighborDock->layout.width*mDockSizeCoef / windowNeighborDock->mParent->layout.width;

			windowDock->layout.anchorRight = windowNeighborDock->layout.anchorLeft;

			if (targetDock->mNeighborMin)
			{
				auto nmin = windowNeighborDock->mNeighborMin;

				nmin->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nmin->mNeighborMin, windowDock);
				windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nmin, windowNeighborDock);
				windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowDock, windowNeighborDock->mNeighborMax);
			}
			else
			{
				windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nullptr, windowNeighborDock);
				windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowDock, windowNeighborDock->mNeighborMax);
			}
		}

		windowDock->AddChild(this);
		layout = UIWidgetLayout::BothStretch();
		SetDocked(true);

		mDockingFrameAppearance.PlayBack();
		mDockingFrameTarget = layout.GetAbsoluteRect();
	}

	void UIDockableWindow::Undock()
	{
		if (!IsDocked())
			return;

		auto topDock = dynamic_cast<UIDockWindowPlace*>(mParent->GetParent());

		if (topDock)
		{
			o2UI.AddWidget(this); 
			SetDocked(false);
			layout.absLeftTop = o2Input.GetCursorPos() + Vec2F(-30, 10);
			layout.absRightBottom = layout.absLeftTop + mNonDockSize.InvertedY();

			return;
		}

		auto parent = dynamic_cast<UIDockWindowPlace*>(mParent);
		auto parentNeighbors = topDock->GetChilds().FindAll([&](auto x) { return x != parent; })
			.Select<UIDockWindowPlace*>([](auto x) { return (UIDockWindowPlace*)x; });

		o2UI.AddWidget(this);

		if (parent->mNeighborMin)
		{
			parent->mNeighborMin->SetResizibleDir(parent->mNeighborMin->mResizibleDir, mDockBorder,
												  parent->mNeighborMin->mNeighborMin, parent->mNeighborMax);
		}

		if (parent->mNeighborMax)
		{
			parent->mNeighborMax->SetResizibleDir(parent->mNeighborMax->mResizibleDir, mDockBorder,
												  parent->mNeighborMin, parent->mNeighborMax->mNeighborMax);
		}

		if (parent->mResizibleDir == TwoDirection::Horizontal)
		{
			if (parent->mNeighborMin && parent->mNeighborMax)
			{
				float anchor = (parent->mNeighborMin->layout.anchorRight + parent->mNeighborMax->layout.anchorLeft) / 2.0f;
				parent->mNeighborMin->layout.anchorRight = anchor;
				parent->mNeighborMax->layout.anchorLeft = anchor;
			}
			else if (parent->mNeighborMin && !parent->mNeighborMax)
				parent->mNeighborMin->layout.anchorRight = 1.0f;
			else if (!parent->mNeighborMin && parent->mNeighborMax)
				parent->mNeighborMax->layout.anchorLeft = 0.0f;
		}

		if (parent->mResizibleDir == TwoDirection::Vertical)
		{
			if (parent->mNeighborMin && parent->mNeighborMax)
			{
				float anchor = (parent->mNeighborMin->layout.anchorTop + parent->mNeighborMax->layout.anchorBottom) / 2.0f;
				parent->mNeighborMin->layout.anchorTop = anchor;
				parent->mNeighborMax->layout.anchorBottom = anchor;
			}
			else if (parent->mNeighborMin && !parent->mNeighborMax)
				parent->mNeighborMin->layout.anchorTop = 1.0f;
			else if (!parent->mNeighborMin && parent->mNeighborMax)
				parent->mNeighborMax->layout.anchorBottom = 0.0f;
		}

		topDock->RemoveChild(parent);

		if (parentNeighbors.Count() == 1)
		{
			for (auto child : parentNeighbors[0]->GetChilds())
				topDock->AddChild(child);

			topDock->RemoveChild(parentNeighbors[0]);
		}

		// 
		// 	for (auto child : parentNeighbor->GetChilds())
		// 		topDock->AddChild(child);
		// 
		// 	topDock->RemoveChild(parent);
		// 	topDock->RemoveChild(parentNeighbor);
		topDock->CheckInteractable();

		SetDocked(false);
		layout.absLeftTop = o2Input.GetCursorPos() + Vec2F(-30, 10);
		layout.absRightBottom = layout.absLeftTop + mNonDockSize.InvertedY();
	}

}

CLASS_META(Editor::UIDockableWindow)
{
	BASE_CLASS(o2::UIWindow);

	PROTECTED_FIELD(mDockSizeCoef);
	PROTECTED_FIELD(mDockBorder);
	PROTECTED_FIELD(mDocked);
	PROTECTED_FIELD(mDockingFrameSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mDockingFrameAppearance);
	PROTECTED_FIELD(mDockingFrameCurrent);
	PROTECTED_FIELD(mDockingFrameTarget);
	PROTECTED_FIELD(mNonDockSize);
	PROTECTED_FIELD(mDragOffset);

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsDocked);
	PUBLIC_FUNCTION(Sprite*, GetDockingFrameSample);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(void, InitializeDockFrameAppearanceAnim);
	PROTECTED_FUNCTION(void, InitializeDragHandles);
	PROTECTED_FUNCTION(void, OnHeadDblCKicked, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnMoveCompleted, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnMoveBegin, const Input::Cursor&);
	PROTECTED_FUNCTION(bool, TraceDock, UIDockWindowPlace*&, Side&, RectF&);
	PROTECTED_FUNCTION(void, PlaceDock, UIDockWindowPlace*);
	PROTECTED_FUNCTION(void, PlaceNonLineDock, UIDockWindowPlace*, Side);
	PROTECTED_FUNCTION(void, PlaceLineDock, UIDockWindowPlace*, Side, RectF);
	PROTECTED_FUNCTION(void, Undock);
	PROTECTED_FUNCTION(void, SetDocked, bool);
}
END_META;
