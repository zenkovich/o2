#include "stdafx.h"
#include "DockableWindow.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Core/WindowsSystem/DockWindowPlace.h"
#include "Events/CursorEventsListener.h"
#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"
#include "Render/Text.h"
#include "../UIRoot.h"

namespace Editor
{
	const char* DockableWindow::mTabLayerPath = "tab/main";
	const char* DockableWindow::mTabIconLayerPath = "tab/main/icon";
	const char* DockableWindow::mTabCaptionLayerPath = "tab/main/caption";
	const char* DockableWindow::mIconLayerPath = "back/icon";
	const char* DockableWindow::mCaptionLayerPath = "back/caption";

	DockableWindow::DockableWindow():
		Window()
	{
		InitializeDragHandles();
		SetDocked(false);
		mDockingFrameSample = mnew Sprite();

		RetargetStatesAnimations();
	}

	DockableWindow::DockableWindow(const DockableWindow& other):
		Window(other)
	{
		InitializeDragHandles();
		SetDocked(false);
		mDockingFrameSample = other.mDockingFrameSample->CloneAs<Sprite>();
		InitializeDockFrameAppearanceAnim();

		if (mVisibleState)
			mVisibleState->onStateFullyFalse += THIS_FUNC(Undock);

		RetargetStatesAnimations();
	}

	DockableWindow::~DockableWindow()
	{
		if (mDockingFrameSample)
			delete mDockingFrameSample;
	}

	DockableWindow& DockableWindow::operator=(const DockableWindow& other)
	{
		Window::operator=(other);
		return *this;
	}

	void DockableWindow::Update(float dt)
	{
		Window::Update(dt);

		const float lerpFrameCoef = 10.0f;
		if (mDockingFrameTarget != mDockingFrameCurrent)
		{
			mDockingFrameCurrent = Math::Lerp(mDockingFrameCurrent, mDockingFrameTarget, dt*lerpFrameCoef);
			mDockingFrameSample->rect = mDockingFrameCurrent;
		}
		mDockingFrameAppearance.Update(dt);
	}

	void DockableWindow::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		mBackCursorArea.OnDrawn();

		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		ScrollArea::OnDrawn();

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
		}
		else
			mHeadDragHandle.OnDrawn();

		DrawDebugFrame();

		mDockingFrameSample->Draw();
	}

	void DockableWindow::SetDocked(bool docked)
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

	void DockableWindow::RecalculateTabWidth()
	{
		float width = 0;
		float expand = 35;
		if (auto textLayer = GetLayer(mTabCaptionLayerPath))
		{
			if (auto textDrawable = dynamic_cast<Text*>(textLayer->GetDrawable()))
			{
				Text::SymbolsSet symbolsSet;
				symbolsSet.Initialize(textDrawable->GetFont(), textDrawable->GetText(), textDrawable->GetHeight(),
									  Vec2F(), Vec2F(), HorAlign::Left, VerAlign::Bottom, false, false, 1.0f, 1.0f);

				SetTabWidth(symbolsSet.mRealSize.x + textLayer->layout.offsetLeft - textLayer->layout.offsetRight + expand);
			}
		}
	}

	bool DockableWindow::IsDocked() const
	{
		return mDocked;
	}

	Sprite* DockableWindow::GetDockingFrameSample() const
	{
		return mDockingFrameSample;
	}

	void DockableWindow::SetIcon(Sprite* icon)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
		{
			if (iconLayer->GetDrawable())
				delete iconLayer->GetDrawable();

			iconLayer->SetDrawable(icon);
		}

		auto tabIconLayer = GetLayer(mTabIconLayerPath);
		if (tabIconLayer)
		{
			if (tabIconLayer->GetDrawable())
				delete tabIconLayer->GetDrawable();

			tabIconLayer->SetDrawable(icon->CloneAs<Sprite>());
		}
	}

	Sprite* DockableWindow::GetIcon() const
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
		{
			if (iconLayer->GetDrawable())
				return dynamic_cast<Sprite*>(iconLayer->GetDrawable());
		}

		return nullptr;
	}

	void DockableWindow::SetIconLayout(const Layout& layout)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			iconLayer->layout = layout;

		auto tabIconLayer = GetLayer(mTabIconLayerPath);
		if (tabIconLayer)
			tabIconLayer->layout = layout;
	}

	Layout DockableWindow::GetIconLayout() const
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			return iconLayer->layout;

		return Layout();
	}

	void DockableWindow::SetCaption(const WString& caption)
	{
		auto captionLayer = GetLayer(mCaptionLayerPath);
		if (captionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(captionLayer->GetDrawable()))
				textDrawable->SetText(caption);
		}

		auto tabCaptionLayer = GetLayer(mTabCaptionLayerPath);
		if (tabCaptionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(tabCaptionLayer->GetDrawable()))
				textDrawable->SetText(caption);
		}

		if (mAutoCalculateTabWidth)
			RecalculateTabWidth();
	}

	WString DockableWindow::GetCaption() const
	{
		auto captionLayer = GetLayer(mCaptionLayerPath);
		if (captionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(captionLayer->GetDrawable()))
				return textDrawable->GetText();
		}

		return WString();
	}

	void DockableWindow::SetTabWidth(float width)
	{
		mTabWidth = width;

		if (auto tabLayer = GetLayer(mTabLayerPath))
			tabLayer->layout.offsetRight = tabLayer->layout.offsetLeft + width;
	}

	float DockableWindow::GetTabWidth() const
	{
		return mTabWidth;
	}

	void DockableWindow::SetAutoCalcuclatingTabWidth(bool enable)
	{
		mAutoCalculateTabWidth = enable;

		if (mAutoCalculateTabWidth)
			RecalculateTabWidth();
	}

	bool DockableWindow::IsAutoCalcuclatingTabWidth() const
	{
		return mAutoCalculateTabWidth;
	}

	void DockableWindow::UpdateSelfTransform()
	{
		Window::UpdateSelfTransform();

		if (auto tabLayer = GetLayer(mTabLayerPath))
			mHeadDragAreaRect = mHeadDragAreaLayout.Calculate(tabLayer->GetRect());
	}

	bool DockableWindow::IsUnderPoint(const Vec2F& point)
	{
		return !mTabState && Widget::IsUnderPoint(point);
	}

	void DockableWindow::CopyData(const Actor& otherActor)
	{
		const DockableWindow& other = dynamic_cast<const DockableWindow&>(otherActor);

		Window::CopyData(other);

		mDockingFrameSample = other.mDockingFrameSample->CloneAs<Sprite>();

		if (mVisibleState)
			mVisibleState->onStateFullyFalse += THIS_FUNC(Undock);
	}

	void DockableWindow::OnResEnableInHierarchyChanged()
	{
		Window::OnResEnableInHierarchyChanged();

		if (!mResEnabled)
			Undock();
	}

	void DockableWindow::OnFocused()
	{
		Window::OnFocused();

		if (mTabState)
		{
			if (auto parentDock = dynamic_cast<DockWindowPlace*>(mParent))
				parentDock->SetActiveTab(this);
		}
	}

	void DockableWindow::InitializeDockFrameAppearanceAnim()
	{
		mDockingFrameAppearance.SetTarget(this);
		*mDockingFrameAppearance.AddAnimationValue(&mDockingFrameSample->transparency) =
			AnimatedValue<float>::EaseInOut(0, 1, 0.3f);

		*mDockingFrameAppearance.AddAnimationValue(&transparency) =
			AnimatedValue<float>::EaseInOut(1, 0, 0.15f);

		mDockingFrameAppearance.GoToBegin();
	}

	void DockableWindow::InitializeDragHandles()
	{
		mHeadDragHandle.onMoved          = THIS_FUNC(OnMoved);
		mHeadDragHandle.onDblClicked     = THIS_FUNC(OnHeadDblCKicked);
		mHeadDragHandle.onCursorPressed  = THIS_FUNC(OnMoveBegin);
		mHeadDragHandle.onCursorReleased = THIS_FUNC(OnMoveCompleted);
	}

	void DockableWindow::OnHeadDblCKicked(const Input::Cursor& cursor)
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
				return dynamic_cast<DockWindowPlace*>(x) != nullptr;
			});

			if (dockPlaceListener)
				PlaceDock(dynamic_cast<DockWindowPlace*>(dockPlaceListener));
		}
	}

	void DockableWindow::OnMoved(const Input::Cursor& cursor)
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

		DockWindowPlace* targetDock;
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

	void DockableWindow::OnMoveCompleted(const Input::Cursor& cursor)
	{
		if (mDocked)
			return;

		DockWindowPlace* targetDock = nullptr;
		Side dockPosition = Side::None;
		RectF dockZoneRect;

		if (!TraceDock(targetDock, dockPosition, dockZoneRect) || targetDock == nullptr)
			return;

		bool allOnLine = targetDock->mParent && targetDock->mParentWidget->GetChildWidgets().All([&](auto x) {

			if (x->GetType() != TypeOf(DockWindowPlace))
				return false;

			DockWindowPlace* dock = (DockWindowPlace*)x;
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

	void DockableWindow::OnMoveBegin(const Input::Cursor& cursor)
	{
		OnFocused();

		if (mDocked)
			mDragOffset = (Vec2F)o2Input.cursorPos - layout->worldLeftTop;
	}

	bool DockableWindow::TraceDock(DockWindowPlace*& targetDock, Side& dockPosition, RectF& dockZoneRect)
	{
		Vec2F cursorPos = o2Input.cursorPos;
		auto listenersUnderCursor = o2Events.GetAllCursorListenersUnderCursor(0);
		auto dockPlaceListener = listenersUnderCursor.FindMatch([](CursorAreaEventsListener* x) {
			return dynamic_cast<DockWindowPlace*>(x) != nullptr;
		});

		if (dockPlaceListener)
		{
			auto dockPlace = dynamic_cast<DockWindowPlace*>(dockPlaceListener);

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

	void DockableWindow::PlaceDock(DockWindowPlace* targetDock)
	{
		mNonDockSize = layout->size;

		mTabPosition = targetDock->mChildren.Count();

		targetDock->AddChild(this);
		*layout = WidgetLayout::BothStretch();
		SetDocked(true);

		mDockingFrameAppearance.PlayBack();
		mDockingFrameTarget = layout->GetWorldRect();

		targetDock->ArrangeChildWindows();
		targetDock->SetLayoutDirty();
	}

	void DockableWindow::PlaceNonLineDock(DockWindowPlace* targetDock, Side dockPosition)
	{
		mNonDockSize = layout->size;
		RectF dockPlaceRect = targetDock->layout->GetWorldRect();

		DockWindowPlace* windowDock = mnew DockWindowPlace();
		windowDock->name = "window dock";
		*windowDock->layout = WidgetLayout::BothStretch();

		DockWindowPlace* windowNeighborDock = mnew DockWindowPlace();
		windowNeighborDock->name = "empty dock";
		*windowNeighborDock->layout = WidgetLayout::BothStretch();

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
		*layout = WidgetLayout::BothStretch();
		SetDocked(true);

		mDockingFrameAppearance.PlayBack();
		mDockingFrameTarget = layout->GetWorldRect();
	}

	void DockableWindow::PlaceLineDock(DockWindowPlace* targetDock, Side dockPosition, RectF dockZoneRect)
	{
		mNonDockSize = layout->size;
		RectF dockPlaceRect = targetDock->layout->GetWorldRect();

		DockWindowPlace* windowDock = mnew DockWindowPlace();
		windowDock->name = "window dock";
		*windowDock->layout = WidgetLayout::BothStretch();

		DockWindowPlace* windowNeighborDock = targetDock;
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
		*layout = WidgetLayout::BothStretch();
		SetDocked(true);

		mDockingFrameAppearance.PlayBack();
		mDockingFrameTarget = layout->GetWorldRect();
	}

	void DockableWindow::SetTabState(float offset, int position, bool isFirst)
	{
		SetStateForcible("tab", true);

		if (mAutoCalculateTabWidth)
			RecalculateTabWidth();

		if (auto tabFirstState = GetStateObject("tabFirst"))
			tabFirstState->SetState(isFirst);

		mTabPosition = position;

		if (auto tabMainLayer = GetLayer(mTabLayerPath))
		{
			tabMainLayer->layout.offsetLeft = offset;
			tabMainLayer->layout.offsetRight = offset + mTabWidth;
			tabMainLayer->layout.anchorLeft = 0;
			tabMainLayer->layout.anchorRight = 0;
		}

		mTabState = true;
	}

	void DockableWindow::SetNonTabState()
	{
		SetStateForcible("tab", false);

		mTabActive = false;

		if (auto state = GetStateObject("tabActive"))
			state->SetState(false);

		if (auto tabMainLayer = GetLayer(mTabLayerPath))
		{
			tabMainLayer->layout.offsetLeft = 0;
			tabMainLayer->layout.offsetRight = 0;
			tabMainLayer->layout.anchorLeft = 0;
			tabMainLayer->layout.anchorRight = 1;
		}

		mTabState = false;
	}

	void DockableWindow::SetActiveTab()
	{
		if (!mTabState)
			return;

		mTabActive = true;

		if (auto state = GetStateObject("tabActive"))
			state->SetState(true);
	}

	void DockableWindow::Undock()
	{
		if (!IsDocked())
			return;

		auto topDock = dynamic_cast<DockWindowPlace*>(mParent->GetParent());

		if (!topDock)
		{
			EditorUIRoot.AddWidget(this);
		}
		else
		{
			auto parent = dynamic_cast<DockWindowPlace*>(mParent);
			auto parentNeighbors = topDock->GetChildWidgets().FindAll([&](auto x) { return x != parent; })
				.Select<DockWindowPlace*>([](auto x) { return (DockWindowPlace*)x; });

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

DECLARE_CLASS(Editor::DockableWindow);
