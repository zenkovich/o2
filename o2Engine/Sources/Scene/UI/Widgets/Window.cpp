#include "stdafx.h"
#include "Window.h"

#include "Application/Application.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"
#include "Scene/UI/Widgets/Button.h"
#include "Utils/Debug/Debug.h"

namespace o2
{
	Window::Window():
		ScrollArea(), DrawableCursorEventsListener(this)
	{
		InitializeHandles();
		InitializeContextMenu();
	}

	Window::Window(const Window& other):
		ScrollArea(other), DrawableCursorEventsListener(this), mHeadDragAreaLayout(other.mHeadDragAreaLayout),
		mTopDragAreaLayout(other.mTopDragAreaLayout), mBottomDragAreaLayout(other.mBottomDragAreaLayout),
		mLeftDragAreaLayout(other.mLeftDragAreaLayout), mRightDragAreaLayout(other.mRightDragAreaLayout),
		mLeftTopDragAreaLayout(other.mLeftTopDragAreaLayout), mRightTopDragAreaLayout(other.mRightTopDragAreaLayout),
		mLeftBottomDragAreaLayout(other.mLeftBottomDragAreaLayout), mRightBottomDragAreaLayout(other.mRightBottomDragAreaLayout),
		caption(this), icon(this)
	{
		RestoreControls();
		RetargetStatesAnimations();
		SetLayoutDirty();
		InitializeHandles();
	}

	Window::~Window()
	{}

	Window& Window::operator=(const Window& other)
	{
		ScrollArea::operator=(other);
		return *this;
	}

	void Window::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		mBackCursorArea.OnDrawn();

		ScrollArea::Draw();

		mHeadDragHandle.OnDrawn();
		mTopDragHandle.OnDrawn();
		mBottomDragHandle.OnDrawn();
		mLeftDragHandle.OnDrawn();
		mRightDragHandle.OnDrawn();
		mLeftTopDragHandle.OnDrawn();
		mRightTopDragHandle.OnDrawn();
		mLeftBottomDragHandle.OnDrawn();
		mRightBottomDragHandle.OnDrawn();

//  		int clr = 0;
//  		o2Render.DrawRectFrame(mHeadDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mTopDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mBottomDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mLeftDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mRightDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mLeftTopDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mRightTopDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mLeftBottomDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mRightBottomDragAreaRect, Color4::SomeColor(clr++));
	}

	void Window::ShowModal()
	{
		Show();
		SetModal(true);
	}

	void Window::SetIcon(Sprite* icon)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
		{
			if (iconLayer->GetDrawable())
				delete iconLayer->GetDrawable();

			iconLayer->SetDrawable(icon);
		}
	}

	Sprite* Window::GetIcon() const
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			return dynamic_cast<Sprite*>(iconLayer->GetDrawable());

		return nullptr;
	}

	void Window::SetIconLayout(const Layout& layout)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			iconLayer->layout = layout;
	}

	Layout Window::GetIconLayout() const
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			return iconLayer->layout;

		return Layout();
	}

	void Window::SetCaption(const WString& caption)
	{
		auto captionLayer = GetLayer(mCaptionLayerPath);
		if (captionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(captionLayer->GetDrawable()))
				textDrawable->SetText(caption);
		}
	}

	WString Window::GetCaption() const
	{
		auto captionLayer = GetLayer(mCaptionLayerPath);
		if (captionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(captionLayer->GetDrawable()))
				return textDrawable->GetText();
		}

		return WString();
	}

	UIContextMenu* Window::GetOptionsMenu() const
	{
		return mOptionsMenu;
	}

	void Window::SetDragAreaLayouts(const Layout& head, const Layout& top, const Layout&bottom, const Layout&left,
									  const Layout& right, const Layout& leftTop, const Layout& rightTop,
									  const Layout& leftBottom, const Layout& rightBottom)
	{
		mHeadDragAreaLayout = head;
		mTopDragAreaLayout = top;
		mBottomDragAreaLayout = bottom;
		mLeftDragAreaLayout = left;
		mRightDragAreaLayout = right;
		mLeftTopDragAreaLayout = leftTop;
		mRightTopDragAreaLayout = rightTop;
		mLeftBottomDragAreaLayout = leftBottom;
		mRightBottomDragAreaLayout = rightBottom;
	}

	bool Window::IsFocusable() const
	{
		return true;
	}

	void Window::SetModal(bool isModal)
	{
		mBackCursorArea.interactable = isModal;
	}

	bool Window::IsModal() const
	{
		return mBackCursorArea.IsInteractable();
	}

	void Window::UpdateSelfTransform()
	{
		ScrollArea::UpdateSelfTransform();

		mHeadDragAreaRect        = mHeadDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mTopDragAreaRect         = mTopDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mBottomDragAreaRect      = mBottomDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mLeftDragAreaRect        = mLeftDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mRightDragAreaRect       = mRightDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mLeftTopDragAreaRect     = mLeftTopDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mRightTopDragAreaRect    = mRightTopDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mLeftBottomDragAreaRect  = mLeftBottomDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mRightBottomDragAreaRect = mRightBottomDragAreaLayout.Calculate(layout->mData->worldRectangle);
	}

	CursorEventsArea& Window::GetBackCursorListener()
	{
		return mBackCursorArea;
	}

	void Window::CopyData(const Actor& otherActor)
	{
		const Window& other = dynamic_cast<const Window&>(otherActor);

		ScrollArea::CopyData(other);

		mHeadDragAreaLayout        = other.mHeadDragAreaLayout;
		mTopDragAreaLayout         = other.mTopDragAreaLayout;
		mBottomDragAreaLayout      = other.mBottomDragAreaLayout;
		mLeftDragAreaLayout        = other.mLeftDragAreaLayout;
		mRightDragAreaLayout       = other.mRightDragAreaLayout;
		mLeftTopDragAreaLayout     = other.mLeftTopDragAreaLayout;
		mRightTopDragAreaLayout    = other.mRightTopDragAreaLayout;
		mLeftBottomDragAreaLayout  = other.mLeftBottomDragAreaLayout;
		mRightBottomDragAreaLayout = other.mRightBottomDragAreaLayout;

		RestoreControls();
		RetargetStatesAnimations();
		BindHandlesInteractableToVisibility();
		SetLayoutDirty();
	}

	void Window::InitializeHandles()
	{
		mBackCursorArea.isUnderPoint = [&](const Vec2F& point) { return true; };
		mBackCursorArea.interactable = false;

		mHeadDragHandle.isUnderPoint = [&](const Vec2F& point) { return mHeadDragAreaRect.IsInside(point); };
		mHeadDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->position += cursor.delta; };
		mHeadDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };

		mTopDragHandle.isUnderPoint = [&](const Vec2F& point) { return mTopDragAreaRect.IsInside(point); };
		mTopDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->worldTop += cursor.delta.y; };
		mTopDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mTopDragHandle.cursorType = CursorType::SizeNS;

		mBottomDragHandle.isUnderPoint = [&](const Vec2F& point) { return mBottomDragAreaRect.IsInside(point); };
		mBottomDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->worldBottom += cursor.delta.y; };
		mBottomDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mBottomDragHandle.cursorType = CursorType::SizeNS;

		mLeftDragHandle.isUnderPoint = [&](const Vec2F& point) { return mLeftDragAreaRect.IsInside(point); };
		mLeftDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->left += cursor.delta.x; };
		mLeftDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mLeftDragHandle.cursorType = CursorType::SizeWE;

		mRightDragHandle.isUnderPoint = [&](const Vec2F& point) { return mRightDragAreaRect.IsInside(point); };
		mRightDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->right += cursor.delta.x; };
		mRightDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mRightDragHandle.cursorType = CursorType::SizeWE;

		mLeftTopDragHandle.isUnderPoint = [&](const Vec2F& point) { return mLeftTopDragAreaRect.IsInside(point); };
		mLeftTopDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->worldLeftTop += cursor.delta; };
		mLeftTopDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mLeftTopDragHandle.cursorType = CursorType::SizeNwSe;

		mLeftBottomDragHandle.isUnderPoint = [&](const Vec2F& point) { return mLeftBottomDragAreaRect.IsInside(point); };
		mLeftBottomDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->leftBottom += cursor.delta; };
		mLeftBottomDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mLeftBottomDragHandle.cursorType = CursorType::SizeNeSw;

		mRightTopDragHandle.isUnderPoint = [&](const Vec2F& point) { return mRightTopDragAreaRect.IsInside(point); };
		mRightTopDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->rightTop += cursor.delta; };
		mRightTopDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mRightTopDragHandle.cursorType = CursorType::SizeNeSw;

		mRightBottomDragHandle.isUnderPoint = [&](const Vec2F& point) { return mRightBottomDragAreaRect.IsInside(point); };
		mRightBottomDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->rightBottom += cursor.delta; };
		mRightBottomDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mRightBottomDragHandle.cursorType = CursorType::SizeNwSe;

		BindHandlesInteractableToVisibility();
	}

	void Window::SetHandlesInteractable(bool interactable)
	{
		mHeadDragHandle.interactable        = interactable;
		mTopDragHandle.interactable         = interactable;
		mBottomDragHandle.interactable      = interactable;
		mLeftDragHandle.interactable        = interactable;
		mRightDragHandle.interactable       = interactable;
		mLeftTopDragHandle.interactable     = interactable;
		mRightTopDragHandle.interactable    = interactable;
		mLeftBottomDragHandle.interactable  = interactable;
		mRightBottomDragHandle.interactable = interactable;
	}

	void Window::BindHandlesInteractableToVisibility()
	{
		if (mVisibleState)
		{
			mVisibleState->onStateBecomesTrue += [&]() { SetHandlesInteractable(true); };
			mVisibleState->onStateFullyFalse += [&]() { SetHandlesInteractable(false); };
		}
	}

	void Window::OnFocused()
	{
		if (mParent)
			SetIndexInSiblings(mParent->GetChildren().Count() - 1);

		SetLastOnCurrentDepth();

		onFocused();
	}

	void Window::OnChildFocused(Widget* child)
	{
		OnFocused();
	}

	void Window::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(this);
	}

	void Window::OnStateAdded(WidgetState* state)
	{
		BindHandlesInteractableToVisibility();
	}

	void Window::OnResEnableInHierarchyChanged()
	{
		interactable = mResEnabled;

		if (mResEnabled)
			Focus();
	}

	void Window::InitializeContextMenu()
	{
		mOptionsMenu = o2UI.CreateWidget<UIContextMenu>("standard");

		if (!mOptionsMenu)
			return;

		mOptionsMenu->name = "options context";
		InitializeContextItems();
		AddInternalWidget(mOptionsMenu);

		Button* optionsBtn = dynamic_cast<Button*>(mInternalWidgets.FindMatch(
			[](Widget* x) { return x->GetName() == "optionsButton" && x->GetType() == TypeOf(Button); }));

		if (optionsBtn)
			optionsBtn->onClick += [=]() { mOptionsMenu->Show(optionsBtn->transform->worldCenter); };
	}

	void Window::InitializeContextItems()
	{
		mOptionsMenu->AddItem("Close", [&]() { Hide(); });
	}

	void Window::RestoreControls()
	{
		Button* closeBtn = dynamic_cast<Button*>(mInternalWidgets.FindMatch(
			[](Widget* x) { return x->GetName() == "closeButton" && x->GetType() == TypeOf(Button); }));

		if (closeBtn)
			closeBtn->onClick += [&]() { Hide(); };

		for (auto element : mInternalWidgets)
		{
			if (element->GetName() == "options context" && element->GetType() == TypeOf(UIContextMenu))
			{
				mInternalWidgets.Remove(element);
				delete element;
				break;
			}
		}

		InitializeContextMenu();
	}
}

DECLARE_CLASS(o2::Window);
