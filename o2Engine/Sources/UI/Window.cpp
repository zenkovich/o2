#include "Window.h"

#include "Application/Application.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "Utils/Debug.h"

namespace o2
{
	UIWindow::UIWindow():
		UIScrollArea(), DrawableCursorEventsListener(this)
	{
		layout.mAnchorMin = Vec2F(0.5f, 0.5f);
		layout.mAnchorMax = Vec2F(0.5f, 0.5f);
		layout.mOffsetMin = Vec2F(-100, -100);
		layout.mOffsetMax = Vec2F(100, 100);

		InitializeHandles();
		InitializeProperties();
	}

	UIWindow::UIWindow(const UIWindow& other):
		UIScrollArea(other), DrawableCursorEventsListener(this), mHeadDragAreaLayout(other.mHeadDragAreaLayout),
		mTopDragAreaLayout(other.mTopDragAreaLayout), mBottomDragAreaLayout(other.mBottomDragAreaLayout),
		mLeftDragAreaLayout(other.mLeftDragAreaLayout), mRightDragAreaLayout(other.mRightDragAreaLayout),
		mLeftTopDragAreaLayout(other.mLeftTopDragAreaLayout), mRightTopDragAreaLayout(other.mRightTopDragAreaLayout),
		mLeftBottomDragAreaLayout(other.mLeftBottomDragAreaLayout),
		mRightBottomDragAreaLayout(other.mRightBottomDragAreaLayout)
	{
		for (auto elem : other.mWindowElements)
		{
			auto newElem = elem->Clone();
			newElem->mParent = this;
			mWindowElements.Add(newElem);
		}

		UIButton* closeBtn = (UIButton*)mWindowElements.FindMatch(
			[](UIWidget* x) { return x->GetName() == "closeButton" && x->GetType() == TypeOf(UIButton); });

		if (closeBtn)
			closeBtn->onClick += [&]() { Hide(); };

		RetargetStatesAnimations();
		UpdateLayout();

		InitializeHandles();
		InitializeProperties();
	}

	UIWindow::~UIWindow()
	{
		for (auto elem : mWindowElements)
			delete elem;
	}

	UIWindow& UIWindow::operator=(const UIWindow& other)
	{
		for (auto elem : mWindowElements)
			delete elem;

		UIScrollArea::operator=(other);

		for (auto elem : other.mWindowElements)
		{
			auto newElem = elem->Clone();
			newElem->mParent = this;
			mWindowElements.Add(newElem);
		}

		UIButton* closeBtn = (UIButton*)mWindowElements.FindMatch(
			[](UIWidget* x) { return x->GetName() == "closeButton" && x->GetType() == TypeOf(UIButton); });

		if (closeBtn)
			closeBtn->onClick += [&]() { Hide(); };

		mHeadDragAreaLayout        = other.mHeadDragAreaLayout;
		mTopDragAreaLayout         = other.mTopDragAreaLayout;
		mBottomDragAreaLayout      = other.mBottomDragAreaLayout;
		mLeftDragAreaLayout        = other.mLeftDragAreaLayout;
		mRightDragAreaLayout       = other.mRightDragAreaLayout;
		mLeftTopDragAreaLayout     = other.mLeftTopDragAreaLayout;
		mRightTopDragAreaLayout    = other.mRightTopDragAreaLayout;
		mLeftBottomDragAreaLayout  = other.mLeftBottomDragAreaLayout;
		mRightBottomDragAreaLayout = other.mRightBottomDragAreaLayout;

		RetargetStatesAnimations();
		BindHandlesInteractableToVisibility();
		UpdateLayout();

		return *this;
	}

	void UIWindow::Update(float dt)
	{
		if (mFullyDisabled)
			return;

		UIScrollArea::Update(dt);

		for (auto elem : mWindowElements)
			elem->Update(dt);
	}

	void UIWindow::Draw()
	{
		if (mFullyDisabled)
			return;

		mBackCursorArea.OnDrawn();

		UIScrollArea::Draw();

		mHeadDragHandle.OnDrawn();
		mTopDragHandle.OnDrawn();
		mBottomDragHandle.OnDrawn();
		mLeftDragHandle.OnDrawn();
		mRightDragHandle.OnDrawn();
		mLeftTopDragHandle.OnDrawn();
		mRightTopDragHandle.OnDrawn();
		mLeftBottomDragHandle.OnDrawn();
		mRightBottomDragHandle.OnDrawn();

		for (auto elem : mWindowElements)
			elem->Draw();

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

	void UIWindow::ShowModal()
	{
		Show();
		SetModal(true);
	}

	UIWidget* UIWindow::AddWindowElement(UIWidget* widget)
	{
		widget->mParent = this;
		mWindowElements.Add(widget);
		UpdateLayout();

		return widget;
	}

	void UIWindow::RemoveWindowElement(UIWidget* widget)
	{
		mWindowElements.Remove(widget);
		delete widget;
		UpdateLayout();
	}

	void UIWindow::RemoveAllWindowElements()
	{
		for (auto elem : mWindowElements)
			delete elem;

		UpdateLayout();
	}

	void UIWindow::SetIcon(Sprite* icon)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
		{
			if (iconLayer->drawable)
				delete iconLayer->drawable;

			iconLayer->drawable = icon;
		}
	}

	Sprite* UIWindow::GetIcon() const
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			return dynamic_cast<Sprite*>(iconLayer->drawable);
		
		return nullptr;
	}

	void UIWindow::SetIconLayout(const Layout& layout)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			iconLayer->layout = layout;
	}

	Layout UIWindow::GetIconLayout() const
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			return iconLayer->layout;

		return Layout();
	}

	void UIWindow::SetCaption(const WString& caption)
	{
		auto captionLayer = GetLayer(mCaptionLayerPath);
		if (captionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(captionLayer->drawable))
				textDrawable->SetText(caption);
		}
	}

	WString UIWindow::GetCaption() const
	{
		auto captionLayer = GetLayer(mCaptionLayerPath);
		if (captionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(captionLayer->drawable))
				return textDrawable->GetText();
		}

		return WString();
	}

	void UIWindow::SetDragAreaLayouts(const Layout& head, const Layout& top, const Layout&bottom, const Layout&left,
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

	bool UIWindow::IsFocusable() const
	{
		return true;
	}

	void UIWindow::SetModal(bool isModal)
	{
		mBackCursorArea.interactable = isModal;
	}

	bool UIWindow::IsModal() const
	{
		return mBackCursorArea.IsInteractable();
	}

	void UIWindow::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		UIScrollArea::UpdateLayout(forcible, withChildren);

		RectF _mChildsAbsRect = mChildrenWorldRect;
		mChildrenWorldRect = layout.mAbsoluteRect;

		for (auto elem : mWindowElements)
			elem->UpdateLayout(true);

		mChildrenWorldRect = _mChildsAbsRect;

		mHeadDragAreaRect        = mHeadDragAreaLayout.Calculate(layout.mAbsoluteRect);
		mTopDragAreaRect         = mTopDragAreaLayout.Calculate(layout.mAbsoluteRect);
		mBottomDragAreaRect      = mBottomDragAreaLayout.Calculate(layout.mAbsoluteRect);
		mLeftDragAreaRect        = mLeftDragAreaLayout.Calculate(layout.mAbsoluteRect);
		mRightDragAreaRect       = mRightDragAreaLayout.Calculate(layout.mAbsoluteRect);
		mLeftTopDragAreaRect     = mLeftTopDragAreaLayout.Calculate(layout.mAbsoluteRect);
		mRightTopDragAreaRect    = mRightTopDragAreaLayout.Calculate(layout.mAbsoluteRect);
		mLeftBottomDragAreaRect  = mLeftBottomDragAreaLayout.Calculate(layout.mAbsoluteRect);
		mRightBottomDragAreaRect = mRightBottomDragAreaLayout.Calculate(layout.mAbsoluteRect);
	}

	CursorEventsArea& UIWindow::GetBackCursorListener()
	{
		return mBackCursorArea;
	}

	void UIWindow::UpdateTransparency()
	{
		UIScrollArea::UpdateTransparency();

		for (auto elem : mWindowElements)
			elem->UpdateTransparency();
	}

	void UIWindow::InitializeHandles()
	{
		mBackCursorArea.isUnderPoint = [&](const Vec2F& point) { return true; };
		mBackCursorArea.interactable = false;

		mHeadDragHandle.isUnderPoint = [&](const Vec2F& point) { return mHeadDragAreaRect.IsInside(point); };
		mHeadDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout.position += cursor.delta; };
		mHeadDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };

		mTopDragHandle.isUnderPoint = [&](const Vec2F& point) { return mTopDragAreaRect.IsInside(point); };
		mTopDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout.absTop += cursor.delta.y; };
		mTopDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mTopDragHandle.cursorType = CursorType::SizeNS;

		mBottomDragHandle.isUnderPoint = [&](const Vec2F& point) { return mBottomDragAreaRect.IsInside(point); };
		mBottomDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout.absBottom += cursor.delta.y; };
		mBottomDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mBottomDragHandle.cursorType = CursorType::SizeNS;

		mLeftDragHandle.isUnderPoint = [&](const Vec2F& point) { return mLeftDragAreaRect.IsInside(point); };
		mLeftDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout.absLeft += cursor.delta.x; };
		mLeftDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mLeftDragHandle.cursorType = CursorType::SizeWE;

		mRightDragHandle.isUnderPoint = [&](const Vec2F& point) { return mRightDragAreaRect.IsInside(point); };
		mRightDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout.absRight += cursor.delta.x; };
		mRightDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mRightDragHandle.cursorType = CursorType::SizeWE;

		mLeftTopDragHandle.isUnderPoint = [&](const Vec2F& point) { return mLeftTopDragAreaRect.IsInside(point); };
		mLeftTopDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout.absLeftTop += cursor.delta; };
		mLeftTopDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mLeftTopDragHandle.cursorType = CursorType::SizeNwSe;

		mLeftBottomDragHandle.isUnderPoint = [&](const Vec2F& point) { return mLeftBottomDragAreaRect.IsInside(point); };
		mLeftBottomDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout.absLeftBottom += cursor.delta; };
		mLeftBottomDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mLeftBottomDragHandle.cursorType = CursorType::SizeNeSw;

		mRightTopDragHandle.isUnderPoint = [&](const Vec2F& point) { return mRightTopDragAreaRect.IsInside(point); };
		mRightTopDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout.absRightTop += cursor.delta; };
		mRightTopDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mRightTopDragHandle.cursorType = CursorType::SizeNeSw;

		mRightBottomDragHandle.isUnderPoint = [&](const Vec2F& point) { return mRightBottomDragAreaRect.IsInside(point); };
		mRightBottomDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout.absRightBottom += cursor.delta; };
		mRightBottomDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mRightBottomDragHandle.cursorType = CursorType::SizeNwSe;

		BindHandlesInteractableToVisibility();
	}

	void UIWindow::SetHandlesInteractable(bool interactable)
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

	void UIWindow::BindHandlesInteractableToVisibility()
	{
		if (mVisibleState)
		{
			mVisibleState->onStateBecomesTrue += [&]() { SetHandlesInteractable(true); };
			mVisibleState->onStateFullyFalse += [&]() { SetHandlesInteractable(false); };
		}
	}

	void UIWindow::OnFocused()
	{
		if (mParent)
		{
			mParent->mChildren.Remove(this);
			mParent->mChildren.Add(this);
		}

		onFocused();
	}

	void UIWindow::OnChildFocused(UIWidget* child)
	{
		OnFocused();
	}

	void UIWindow::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(this);
	}

	void UIWindow::OnStateAdded(UIWidgetState* state)
	{
		BindHandlesInteractableToVisibility();
	}

	void UIWindow::OnVisibleChanged()
	{
		interactable = mResVisible;
		
		if (mResVisible)
			Focus();
	}

	void UIWindow::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIWindow, icon, SetIcon, GetIcon);
		INITIALIZE_PROPERTY(UIWindow, caption, SetCaption, GetCaption);
	}
}

CLASS_META(o2::UIWindow)
{
	BASE_CLASS(o2::UIScrollArea);
	BASE_CLASS(o2::DrawableCursorEventsListener);

	PUBLIC_FIELD(caption);
	PUBLIC_FIELD(icon);
	PROTECTED_FIELD(mIconLayerPath);
	PROTECTED_FIELD(mCaptionLayerPath);
	PROTECTED_FIELD(mWindowElements).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mBackCursorArea);
	PROTECTED_FIELD(mHeadDragHandle);
	PROTECTED_FIELD(mHeadDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHeadDragAreaRect);
	PROTECTED_FIELD(mTopDragHandle);
	PROTECTED_FIELD(mTopDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTopDragAreaRect);
	PROTECTED_FIELD(mBottomDragHandle);
	PROTECTED_FIELD(mBottomDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mBottomDragAreaRect);
	PROTECTED_FIELD(mLeftDragHandle);
	PROTECTED_FIELD(mLeftDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLeftDragAreaRect);
	PROTECTED_FIELD(mRightDragHandle);
	PROTECTED_FIELD(mRightDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mRightDragAreaRect);
	PROTECTED_FIELD(mLeftTopDragHandle);
	PROTECTED_FIELD(mLeftTopDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLeftTopDragAreaRect);
	PROTECTED_FIELD(mRightTopDragHandle);
	PROTECTED_FIELD(mRightTopDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mRightTopDragAreaRect);
	PROTECTED_FIELD(mLeftBottomDragHandle);
	PROTECTED_FIELD(mLeftBottomDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLeftBottomDragAreaRect);
	PROTECTED_FIELD(mRightBottomDragHandle);
	PROTECTED_FIELD(mRightBottomDragAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mRightBottomDragAreaRect);

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, ShowModal);
	PUBLIC_FUNCTION(UIWidget*, AddWindowElement, UIWidget*);
	PUBLIC_FUNCTION(void, RemoveWindowElement, UIWidget*);
	PUBLIC_FUNCTION(void, RemoveAllWindowElements);
	PUBLIC_FUNCTION(void, SetIcon, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetIcon);
	PUBLIC_FUNCTION(void, SetIconLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetIconLayout);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(void, SetDragAreaLayouts, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(void, SetModal, bool);
	PUBLIC_FUNCTION(bool, IsModal);
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
	PUBLIC_FUNCTION(CursorEventsArea&, GetBackCursorListener);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, InitializeHandles);
	PROTECTED_FUNCTION(void, SetHandlesInteractable, bool);
	PROTECTED_FUNCTION(void, BindHandlesInteractableToVisibility);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnChildFocused, UIWidget*);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnStateAdded, UIWidgetState*);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
