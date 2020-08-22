#include "o2/stdafx.h"
#include "PopupWidget.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"

namespace o2
{
	PopupWidget::PopupWidget():
		ScrollArea()
	{
		SetEnableForcible(false);
	}

	PopupWidget::PopupWidget(const PopupWidget& other):
		ScrollArea(other), mFitSizeMin(other.mFitSizeMin)
	{
		SetEnableForcible(false);
	}

	PopupWidget::~PopupWidget()
	{}

	PopupWidget& PopupWidget::operator=(const PopupWidget& other)
	{
		ScrollArea::operator=(other);
		return *this;
	}

	void PopupWidget::Update(float dt)
	{
		if (!mResEnabledInHierarchy)
			return;

		ScrollArea::Update(dt);

		bool cursorPressed = o2Input.IsCursorPressed() || o2Input.IsRightMousePressed();
		if (cursorPressed)
		{
			if (!mChildPopup && (cursorPressed || Math::Abs(o2Input.GetMouseWheelDelta()) > 0.1f) &&
				!layout->IsPointInside(o2Input.GetCursorPos()) && !mShownAtFrame && mEnabled)
			{
				HideWithParent();
			}
		}

		mShownAtFrame = false;
	}

	void PopupWidget::Draw()
	{}

	void PopupWidget::Show(PopupWidget* parent, const Vec2F& position /*= o2Input.GetCursorPos()*/)
	{
		if (parent)
		{
			mParentPopup = parent;
			parent->mChildPopup = this;
		}
		else mVisiblePopup = this;

		Widget::Show();

		FitSizeAndPosition(position);
		UpdateSelfTransform();
		UpdateChildrenTransforms();

		mShownAtFrame = true;
	}

	void PopupWidget::Show(const Vec2F& position /*= o2Input.GetCursorPos()*/)
	{
		Show(nullptr, position);
	}

	void PopupWidget::OnEnableInHierarchyChanged()
	{
		interactable = mResEnabled;
	}

	void PopupWidget::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		HideWithParent();
		HideWithChild();
	}


	void PopupWidget::OnCursorReleasedOutside(const Input::Cursor& cursor)
	{
		HideWithParent();
		HideWithChild();
	}

	void PopupWidget::OnKeyPressed(const Input::Key& key)
	{
		if (key == VK_ESCAPE)
		{
			HideWithParent();
			HideWithChild();
		}
	}

	void PopupWidget::HideWithParent()
	{
		Hide();

		if (mParentPopup)
			mParentPopup->HideWithParent();

		mParentPopup = nullptr;
		mChildPopup = nullptr;
	}

	void PopupWidget::HideWithChild()
	{
		Hide();

		if (mChildPopup)
			mChildPopup->HideWithChild();

		mChildPopup = nullptr;
	}

	void PopupWidget::SetMinFitSize(float size)
	{
		mFitSizeMin = size;
	}

	bool PopupWidget::IsScrollable() const
	{
		return true;
	}

	PopupWidget* PopupWidget::mVisiblePopup = nullptr;

	bool PopupWidget::IsInputTransparent() const
	{
		return false;
	}

	void PopupWidget::CopyData(const Actor& otherActor)
	{
		const PopupWidget& other = dynamic_cast<const PopupWidget&>(otherActor);

		ScrollArea::CopyData(other);

		mFitSizeMin = other.mFitSizeMin;

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void PopupWidget::CheckClipping(const RectF& clipArea)
	{
		mIsClipped = false;

		Vec2F resolution = o2Render.GetCurrentResolution();
		RectF fullScreenRect(resolution*0.5f, resolution*(-0.5f));
		for (auto child : mChildWidgets)
			child->CheckClipping(fullScreenRect);
	}

	void PopupWidget::FitSizeAndPosition(const Vec2F& position)
	{
		UpdateTransform();

		Vec2F size = fitByChildren ? GetContentSize() : layout->GetSize();
		FitPosition(position, size);
	}

	Vec2F PopupWidget::GetContentSize() const
	{
		Vec2F size = layout->GetMinimalSize();

		for (auto child : mChildWidgets)
		{
			size.x = Math::Max(size.x, child->GetMinWidthWithChildren());
			size.y = Math::Max(size.y, child->GetMinHeightWithChildren());
		}

		size.x += mViewAreaLayout.offsetMin.x - mViewAreaLayout.offsetMax.x;
		size.y += mViewAreaLayout.offsetMin.y - mViewAreaLayout.offsetMax.y;

		return size;
	}

	void PopupWidget::FitPosition(const Vec2F& position, Vec2F size)
	{
		size.x = Math::Min(size.x, (float)o2Render.GetResolution().x);
		size.y = Math::Min(size.y, (float)o2Render.GetResolution().y);

		RectF thisRect(position.x, position.y, position.x + size.x, position.y - size.y);
		RectF screenRect(-o2Render.GetResolution().x*0.5f, o2Render.GetResolution().y*0.5f,
						 o2Render.GetResolution().x*0.5f, -o2Render.GetResolution().y*0.5f);

		if (thisRect.left < screenRect.left)
			thisRect += Vec2F(screenRect.left - thisRect.left, 0);

		if (thisRect.right > screenRect.right)
			thisRect += Vec2F(screenRect.right - thisRect.right, 0);

		if (thisRect.top > screenRect.top)
			thisRect += Vec2F(0, screenRect.top - thisRect.top);

		if (thisRect.bottom < screenRect.bottom)
			thisRect += Vec2F(0, screenRect.bottom - thisRect.bottom);

		thisRect.left = Math::Round(thisRect.left);
		thisRect.right = Math::Round(thisRect.right);
		thisRect.top = Math::Round(thisRect.top);
		thisRect.bottom = Math::Round(thisRect.bottom);

		layout->worldRect = thisRect;
	}

	void PopupWidget::SpecialDraw()
	{
		if (!mResEnabledInHierarchy)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		for (auto child : mChildWidgets)
			child->Draw();

		o2Render.DisableScissorTest();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (mOwnHorScrollBar)
			mHorScrollBar->Draw();

		if (mOwnVerScrollBar)
			mVerScrollBar->Draw();

		DrawDebugFrame();

		if (mChildPopup)
			mChildPopup->SpecialDraw();
	}
}

DECLARE_CLASS(o2::PopupWidget);
