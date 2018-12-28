#include "stdafx.h"
#include "UIDockableWindow.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Core/WindowsSystem/UIDockWindowPlace.h"
#include "Events/CursorEventsListener.h"
#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "Render/Text.h"
#include "../UIRoot.h"

namespace Editor
{
	const char* UIDockableWindow::mTabLayerPath = "tab/main";
	const char* UIDockableWindow::mTabIconLayerPath = "tab/main/icon";
	const char* UIDockableWindow::mTabCaptionLayerPath = "tab/main/caption";
	const char* UIDockableWindow::mIconLayerPath = "back/icon";
	const char* UIDockableWindow::mCaptionLayerPath = "back/caption";

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
		mDockingFrameSample = other.mDockingFrameSample->CloneAs<Sprite>();
		InitializeDockFrameAppearanceAnim();

		if (mVisibleState)
			mVisibleState->onStateFullyFalse += THIS_FUNC(Undock);

		RetargetStatesAnimations();
	}

	UIDockableWindow::~UIDockableWindow()
	{
		if (mDockingFrameSample)
			delete mDockingFrameSample;
	}

	UIDockableWindow& UIDockableWindow::operator=(const UIDockableWindow& other)
	{
		UIWindow::operator=(other);
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
		if (!mResEnabledInHierarchy)
			return;

		mBackCursorArea.OnDrawn();

		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		UIScrollArea::OnDrawn();

		if (!mTabState || mTabActive)
		{
			o2Render.EnableScissorTest(mAbsoluteClipArea);

			for (auto child : mDrawingChildren)
				child->Draw();

			o2Render.DisableScissorTest();

			mTopDragHandle.OnDrawn();
			mBottomDragHandle.OnDrawn();
			mLeftDragHandle.OnDrawn();
			mRightDragHandle.OnDrawn();
			mLeftTopDragHandle.OnDrawn();
			mRightTopDragHandle.OnDrawn();
			mLeftBottomDragHandle.OnDrawn();
			mRightBottomDragHandle.OnDrawn();
			mHeadDragHandle.OnDrawn();

			for (auto child : mInternalWidgets)
				child->Draw();

			for (auto layer : mTopDrawingLayers)
				layer->Draw();

			if (mOwnHorScrollBar)
				mHorScrollBar->Draw();

			if (mOwnVerScrollBar)
				mVerScrollBar->Draw();
		}
		else
			mHeadDragHandle.OnDrawn();

		DrawDebugFrame();

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

	void UIDockableWindow::RecalculateTabWidth()
	{
		float width = 0;
		float expand = 15;
		if (auto textLayer = GetLayer(mTabCaptionLayerPath))
		{
			if (auto textDrawable = dynamic_cast<Text*>(textLayer->drawable))
			{
				Text::SymbolsSet symbolsSet;
				symbolsSet.Initialize(textDrawable->GetFont(), textDrawable->GetText(), textDrawable->GetHeight(),
									  Vec2F(), Vec2F(), HorAlign::Left, VerAlign::Bottom, false, false, 1.0f, 1.0f);

				SetTabWidth(symbolsSet.mRealSize.x + textLayer->layout.offsetMin.x - textLayer->layout.offsetMax.x + expand);
			}
		}
	}

	bool UIDockableWindow::IsDocked() const
	{
		return mDocked;
	}

	Sprite* UIDockableWindow::GetDockingFrameSample() const
	{
		return mDockingFrameSample;
	}

	void UIDockableWindow::SetIcon(Sprite* icon)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
		{
			if (iconLayer->drawable)
				delete iconLayer->drawable;

			iconLayer->drawable = icon;
		}

		auto tabIconLayer = GetLayer(mTabIconLayerPath);
		if (tabIconLayer)
		{
			if (tabIconLayer->drawable)
				delete tabIconLayer->drawable;

			tabIconLayer->drawable = icon->CloneAs<Sprite>();
		}
	}

	Sprite* UIDockableWindow::GetIcon() const
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
		{
			if (iconLayer->drawable)
				delete iconLayer->drawable;

			return dynamic_cast<Sprite*>(iconLayer->drawable);
		}

		return nullptr;
	}

	void UIDockableWindow::SetIconLayout(const Layout& layout)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			iconLayer->layout = layout;

		auto tabIconLayer = GetLayer(mTabIconLayerPath);
		if (tabIconLayer)
			tabIconLayer->layout = layout;
	}

	Layout UIDockableWindow::GetIconLayout() const
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			return iconLayer->layout;

		return Layout();
	}

	void UIDockableWindow::SetCaption(const WString& caption)
	{
		auto captionLayer = GetLayer(mCaptionLayerPath);
		if (captionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(captionLayer->drawable))
				textDrawable->SetText(caption);
		}

		auto tabCaptionLayer = GetLayer(mTabCaptionLayerPath);
		if (tabCaptionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(tabCaptionLayer->drawable))
				textDrawable->SetText(caption);
		}

		if (mAutoCalculateTabWidth)
			RecalculateTabWidth();
	}

	WString UIDockableWindow::GetCaption() const
	{
		auto captionLayer = GetLayer(mCaptionLayerPath);
		if (captionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(captionLayer->drawable))
				return textDrawable->GetText();
		}

		return WString();
	}

	void UIDockableWindow::SetTabWidth(float width)
	{
		mTabWidth = width;

		if (auto tabLayer = GetLayer(mTabLayerPath))
			tabLayer->layout.offsetMax.x = tabLayer->layout.offsetMin.x + width;
	}

	float UIDockableWindow::GetTabWidth() const
	{
		return mTabWidth;
	}

	void UIDockableWindow::SetAutoCalcuclatingTabWidth(bool enable)
	{
		mAutoCalculateTabWidth = enable;

		if (mAutoCalculateTabWidth)
			RecalculateTabWidth();
	}

	bool UIDockableWindow::IsAutoCalcuclatingTabWidth() const
	{
		return mAutoCalculateTabWidth;
	}

	void UIDockableWindow::UpdateSelfTransform()
	{
		UIWindow::UpdateSelfTransform();

		if (auto tabLayer = GetLayer(mTabLayerPath))
			mHeadDragAreaRect = mHeadDragAreaLayout.Calculate(tabLayer->GetRect());
	}

	bool UIDockableWindow::IsUnderPoint(const Vec2F& point)
	{
		return !mTabState && UIWidget::IsUnderPoint(point);
	}

	void UIDockableWindow::CopyData(const Actor& otherActor)
	{
		const UIDockableWindow& other = dynamic_cast<const UIDockableWindow&>(otherActor);

		UIWindow::CopyData(other);

		mDockingFrameSample = other.mDockingFrameSample->CloneAs<Sprite>();

		if (mVisibleState)
			mVisibleState->onStateFullyFalse += THIS_FUNC(Undock);
	}

	void UIDockableWindow::OnResEnableInHierarchyChanged()
	{
		UIWindow::OnResEnableInHierarchyChanged();

		if (!mResEnabled)
			Undock();
	}

	void UIDockableWindow::OnFocused()
	{
		UIWindow::OnFocused();

		if (mTabState)
		{
			if (auto parentDock = dynamic_cast<UIDockWindowPlace*>(mParent))
				parentDock->SetActiveTab(this);
		}
	}

	void UIDockableWindow::InitializeDockFrameAppearanceAnim()
	{
		mDockingFrameAppearance.SetTarget(this);
		*mDockingFrameAppearance.AddAnimationValue(&mDockingFrameSample->transparency) =
			AnimatedValue<float>::EaseInOut(0, 1, 0.3f);

		*mDockingFrameAppearance.AddAnimationValue(&transparency) =
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
		{
			Undock();

			Vec2F size = layout->GetSize();

			if (auto headLayer = GetLayer(mTabLayerPath))
				layout->worldLeftTop = o2Input.GetCursorPos() - headLayer->GetRect().Size().InvertedY()*0.5f;
			else
				layout->worldLeftTop = o2Input.GetCursorPos();

			layout->worldRightBottom = layout->worldLeftTop + size.InvertedY();
		}
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
			if (!layout->IsPointInside(cursor.position))
			{
				Undock();

				SetLayoutDirty();

				Vec2F anchor = (layout->worldLeftTop + layout->worldRightTop.Get())*0.5f;

				if (auto headLayer = GetLayer(mTabLayerPath))
					anchor.y -= headLayer->GetRect().Height()*0.5f;

				layout->worldPosition += o2Input.GetCursorPos() - anchor;

				mDragOffset = Vec2F();
			}

			return;
		}

		layout->worldPosition += cursor.delta;

		UIDockWindowPlace* targetDock;
		Side dockPosition = Side::None;
		RectF dockZoneRect;

		bool tracedDock = TraceDock(targetDock, dockPosition, dockZoneRect);

		if (dockPosition != Side::None && tracedDock)
		{
			mDockingFrameAppearance.PlayForward();
			mDockingFrameTarget = dockZoneRect;
		}
		else
		{
			mDockingFrameAppearance.PlayBack();
			mDockingFrameTarget = layout->worldRect;
		}
	}

	void UIDockableWindow::OnMoveCompleted(const Input::Cursor& cursor)
	{
		if (mDocked)
			return;

		UIDockWindowPlace* targetDock = nullptr;
		Side dockPosition = Side::None;
		RectF dockZoneRect;

		if (!TraceDock(targetDock, dockPosition, dockZoneRect) || targetDock == nullptr)
			return;

		bool allOnLine = targetDock->mParent && targetDock->mParentWidget->GetChildWidgets().All([&](auto x) {

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
			mDragOffset = (Vec2F)o2Input.cursorPos - layout->worldLeftTop;
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

			RectF dockPlaceRect = dockPlace->layout->worldRect;

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
		mNonDockSize = layout->size;

		mTabPosition = targetDock->mChildren.Count();

		targetDock->AddChild(this);
		*layout = UIWidgetLayout::BothStretch();
		SetDocked(true);

		mDockingFrameAppearance.PlayBack();
		mDockingFrameTarget = layout->GetWorldRect();

		targetDock->ArrangeChildWindows();
		targetDock->SetLayoutDirty();
	}

	void UIDockableWindow::PlaceNonLineDock(UIDockWindowPlace* targetDock, Side dockPosition)
	{
		mNonDockSize = layout->size;
		RectF dockPlaceRect = targetDock->layout->GetWorldRect();

		UIDockWindowPlace* windowDock = mnew UIDockWindowPlace();
		windowDock->name = "window dock";
		*windowDock->layout = UIWidgetLayout::BothStretch();

		UIDockWindowPlace* windowNeighborDock = mnew UIDockWindowPlace();
		windowNeighborDock->name = "empty dock";
		*windowNeighborDock->layout = UIWidgetLayout::BothStretch();

		for (auto child : targetDock->GetChildWidgets())
			windowNeighborDock->AddChild(child);

		targetDock->AddChild(windowNeighborDock);
		targetDock->AddChild(windowDock);
		targetDock->interactable = false;

		if (dockPosition == Side::Bottom)
		{
			windowNeighborDock->layout->anchorBottom = mDockSizeCoef;
			windowDock->layout->anchorTop = mDockSizeCoef;
			windowDock->layout->offsetTop = -mDockBorder;

			windowDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, nullptr, windowNeighborDock);
			windowNeighborDock->SetResizibleDir(TwoDirection::Vertical, mDockBorder, windowDock, nullptr);
		}
		else if (dockPosition == Side::Right)
		{
			windowNeighborDock->layout->anchorRight = 1.0f - mDockSizeCoef;
			windowDock->layout->anchorLeft = 1.0f - mDockSizeCoef;
			windowDock->layout->offsetLeft = mDockBorder;

			windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowNeighborDock, nullptr);
			windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nullptr, windowDock);
		}
		else if (dockPosition == Side::Left)
		{
			windowNeighborDock->layout->anchorLeft = mDockSizeCoef;
			windowDock->layout->anchorRight = mDockSizeCoef;
			windowDock->layout->offsetRight = -mDockBorder;

			windowDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, nullptr, windowNeighborDock);
			windowNeighborDock->SetResizibleDir(TwoDirection::Horizontal, mDockBorder, windowDock, nullptr);
		}

		windowDock->AddChild(this);
		*layout = UIWidgetLayout::BothStretch();
		SetDocked(true);

		mDockingFrameAppearance.PlayBack();
		mDockingFrameTarget = layout->GetWorldRect();
	}

	void UIDockableWindow::PlaceLineDock(UIDockWindowPlace* targetDock, Side dockPosition, RectF dockZoneRect)
	{
		mNonDockSize = layout->size;
		RectF dockPlaceRect = targetDock->layout->GetWorldRect();

		UIDockWindowPlace* windowDock = mnew UIDockWindowPlace();
		windowDock->name = "window dock";
		*windowDock->layout = UIWidgetLayout::BothStretch();

		UIDockWindowPlace* windowNeighborDock = targetDock;
		targetDock->mParent->AddChild(windowDock);

		if (dockPosition == Side::Bottom)
		{
			windowDock->layout->anchorBottom = windowNeighborDock->layout->anchorBottom;

			windowNeighborDock->layout->anchorBottom +=
				windowNeighborDock->layout->height*mDockSizeCoef / windowNeighborDock->mParentWidget->layout->height;

			windowDock->layout->anchorTop = windowNeighborDock->layout->anchorBottom;

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
			windowDock->layout->anchorRight = windowNeighborDock->layout->anchorRight;

			windowNeighborDock->layout->anchorRight -=
				windowNeighborDock->layout->width*mDockSizeCoef / windowNeighborDock->mParentWidget->layout->width;

			windowDock->layout->anchorLeft = windowNeighborDock->layout->anchorRight;

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
			windowDock->layout->anchorLeft = windowNeighborDock->layout->anchorLeft;

			windowNeighborDock->layout->anchorLeft +=
				windowNeighborDock->layout->width*mDockSizeCoef / windowNeighborDock->mParentWidget->layout->width;

			windowDock->layout->anchorRight = windowNeighborDock->layout->anchorLeft;

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
		*layout = UIWidgetLayout::BothStretch();
		SetDocked(true);

		mDockingFrameAppearance.PlayBack();
		mDockingFrameTarget = layout->GetWorldRect();
	}

	void UIDockableWindow::SetTabState(float offset, int position, bool isFirst)
	{
		SetStateForcible("tab", true);

		if (mAutoCalculateTabWidth)
			RecalculateTabWidth();

		if (auto tabFirstState = GetStateObject("tabFirst"))
			tabFirstState->SetState(isFirst);

		mTabPosition = position;

		if (auto tabMainLayer = GetLayer(mTabLayerPath))
		{
			tabMainLayer->layout.offsetMin.x = offset;
			tabMainLayer->layout.offsetMax.x = offset + mTabWidth;
			tabMainLayer->layout.anchorMin.x = 0;
			tabMainLayer->layout.anchorMax.x = 0;
		}

		mTabState = true;
	}

	void UIDockableWindow::SetNonTabState()
	{
		SetStateForcible("tab", false);

		mTabActive = false;

		if (auto state = GetStateObject("tabActive"))
			state->SetState(false);

		if (auto tabMainLayer = GetLayer(mTabLayerPath))
		{
			tabMainLayer->layout.offsetMin.x = 0;
			tabMainLayer->layout.offsetMax.x = 0;
			tabMainLayer->layout.anchorMin.x = 0;
			tabMainLayer->layout.anchorMax.x = 1;
		}

		mTabState = false;
	}

	void UIDockableWindow::SetActiveTab()
	{
		if (!mTabState)
			return;

		mTabActive = true;

		if (auto state = GetStateObject("tabActive"))
			state->SetState(true);
	}

	void UIDockableWindow::Undock()
	{
		if (!IsDocked())
			return;

		auto topDock = dynamic_cast<UIDockWindowPlace*>(mParent->GetParent());

		if (!topDock)
		{
			EditorUIRoot.AddWidget(this);
		}
		else
		{
			auto parent = dynamic_cast<UIDockWindowPlace*>(mParent);
			auto parentNeighbors = topDock->GetChildWidgets().FindAll([&](auto x) { return x != parent; })
				.Select<UIDockWindowPlace*>([](auto x) { return (UIDockWindowPlace*)x; });

			EditorUIRoot.AddWidget(this);

			if (!parent->GetChildWidgets().IsEmpty())
			{
				parent->ArrangeChildWindows();
				parent->SetLayoutDirty();
			}
			else
			{
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
						float anchor = (parent->mNeighborMin->layout->anchorRight + parent->mNeighborMax->layout->anchorLeft) / 2.0f;
						parent->mNeighborMin->layout->anchorRight = anchor;
						parent->mNeighborMax->layout->anchorLeft = anchor;
					}
					else if (parent->mNeighborMin && !parent->mNeighborMax)
						parent->mNeighborMin->layout->anchorRight = 1.0f;
					else if (!parent->mNeighborMin && parent->mNeighborMax)
						parent->mNeighborMax->layout->anchorLeft = 0.0f;
				}

				if (parent->mResizibleDir == TwoDirection::Vertical)
				{
					if (parent->mNeighborMin && parent->mNeighborMax)
					{
						float anchor = (parent->mNeighborMin->layout->anchorTop + parent->mNeighborMax->layout->anchorBottom) / 2.0f;
						parent->mNeighborMin->layout->anchorTop = anchor;
						parent->mNeighborMax->layout->anchorBottom = anchor;
					}
					else if (parent->mNeighborMin && !parent->mNeighborMax)
						parent->mNeighborMin->layout->anchorTop = 1.0f;
					else if (!parent->mNeighborMin && parent->mNeighborMax)
						parent->mNeighborMax->layout->anchorBottom = 0.0f;
				}

				topDock->RemoveChild(parent);

				if (parentNeighbors.Count() == 1)
				{
					for (auto child : parentNeighbors[0]->GetChildWidgets())
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
			}
		}

		SetDocked(false);
		SetNonTabState();

		Vec2F leftTop = layout->worldLeftTop;
		layout->anchorMin = Vec2F(); layout->anchorMax = Vec2F();
		layout->worldLeftTop = leftTop;
		layout->worldRightBottom = layout->worldLeftTop + mNonDockSize.InvertedY();
	}

}

DECLARE_CLASS(Editor::UIDockableWindow);
