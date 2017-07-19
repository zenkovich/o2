#include "FrameScrollView.h"

#include "UI/HorizontalScrollBar.h"
#include "UI/VerticalScrollBar.h"

namespace Editor
{

	UIFrameScrollView::UIFrameScrollView():
		UIScrollView()
	{
		mReady = false;

		mHorScrollbar = mnew UIHorizontalScrollBar();
		mHorScrollbar->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 20);
		mHorScrollbar->SetParent(this);
		mHorScrollbar->onUserChange = Func(this, &UIFrameScrollView::OnHorScrollScrolled);
		mChilds.Remove(mHorScrollbar);

		mVerScrollbar = mnew UIVerticalScrollBar();
		mVerScrollbar->layout = UIWidgetLayout::VerStretch(HorAlign::Right, 0, 0, 20);
		mVerScrollbar->SetParent(this);
		mVerScrollbar->onUserChange = Func(this, &UIFrameScrollView::OnVerScrollScrolled);
		mChilds.Remove(mVerScrollbar);

		mReady = true;
	}

	UIFrameScrollView::UIFrameScrollView(const UIFrameScrollView& other):
		UIScrollView(other), mHorScrollbar(other.mHorScrollbar->Clone()), mVerScrollbar(other.mVerScrollbar->Clone())
	{
		mReady = false;

		mHorScrollbar->SetParent(this);
		mHorScrollbar->onUserChange = Func(this, &UIFrameScrollView::OnHorScrollScrolled);
		mChilds.Remove(mHorScrollbar);

		mVerScrollbar->SetParent(this);
		mVerScrollbar->onUserChange = Func(this, &UIFrameScrollView::OnVerScrollScrolled);
		mChilds.Remove(mVerScrollbar);

		RetargetStatesAnimations();

		mReady = true;
	}

	UIFrameScrollView::~UIFrameScrollView()
	{
		delete mHorScrollbar;
		delete mVerScrollbar;
	}

	UIFrameScrollView& UIFrameScrollView::operator=(const UIFrameScrollView& other)
	{
		UIScrollView::operator=(other);

		delete mHorScrollbar;
		delete mVerScrollbar;

		mHorScrollbar = other.mHorScrollbar->Clone();
		mHorScrollbar->SetParent(this);
		mHorScrollbar->onUserChange = Func(this, &UIFrameScrollView::OnHorScrollScrolled);
		mChilds.Remove(mHorScrollbar);

		mVerScrollbar = other.mVerScrollbar->Clone();
		mVerScrollbar->SetParent(this);
		mVerScrollbar->onUserChange = Func(this, &UIFrameScrollView::OnVerScrollScrolled);
		mChilds.Remove(mVerScrollbar);

		return *this;
	}

	void UIFrameScrollView::Draw()
	{
		UIScrollView::Draw();

		if (!mReady)
			return;

		mHorScrollbar->Draw();
		mVerScrollbar->Draw();
	}

	void UIFrameScrollView::Update(float dt)
	{
		UIScrollView::Update(dt);

		if (!mReady)
			return;

		UpdateCameraLimits(dt);

		mHorScrollbar->Update(dt);
		mVerScrollbar->Update(dt);
	}

	void UIFrameScrollView::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		UIScrollView::UpdateLayout(forcible, withChildren);

		if (!mReady)
			return;

		mHorScrollbar->UpdateLayout(forcible, withChildren);
		mVerScrollbar->UpdateLayout(forcible, withChildren);
	}

	void UIFrameScrollView::SetHorScrollbar(UIHorizontalScrollBar* scrollbar)
	{
		delete mHorScrollbar;
		mHorScrollbar = scrollbar;
		AddChild(mHorScrollbar);
		mChilds.Remove(mHorScrollbar);
		mHorScrollbar->onUserChange = Func(this, &UIFrameScrollView::OnHorScrollScrolled);

		UpdateLayout();
	}

	void UIFrameScrollView::SetVerScrollbar(UIVerticalScrollBar* scrollbar)
	{
		delete mVerScrollbar;
		mVerScrollbar = scrollbar;
		AddChild(mVerScrollbar);
		mChilds.Remove(mVerScrollbar);
		mVerScrollbar->onUserChange = Func(this, &UIFrameScrollView::OnVerScrollScrolled);

		UpdateLayout();
	}

	void UIFrameScrollView::SetViewArea(const RectF& area)
	{
		mAvailableArea = area;
	}

	RectF UIFrameScrollView::GetViewArea() const
	{
		return mAvailableArea;
	}

	void UIFrameScrollView::UpdateCameraLimits(float dt)
	{
		if (o2Input.IsCursorDown())
			return;

		RectF camRect = mViewCamera.GetRect();
		Vec2F camSize = camRect.Size();

		mHorScrollbar->SetValueRange(Math::Min(mAvailableArea.left + camSize.x*0.5f, camRect.Center().x), 
									 Math::Max(mAvailableArea.right - camSize.x*0.5f, camRect.Center().x));
		mHorScrollbar->SetScrollHandleSize(camRect.Width());

		mVerScrollbar->SetValueRange(Math::Min(mAvailableArea.bottom + camSize.y*0.5f, camRect.Center().y),
									 Math::Max(mAvailableArea.top - camSize.y*0.5f, camRect.Center().y));

		mVerScrollbar->SetScrollHandleSize(camRect.Height());
	}

	void UIFrameScrollView::OnHorScrollScrolled(float value)
	{
		mViewCamera.SetPosition(Vec2F(value, mViewCamera.position->y));
		mViewCameraTargetPos = mViewCamera.GetPosition();
		mNeedRedraw = true;
	}

	void UIFrameScrollView::OnVerScrollScrolled(float value)
	{
		float min = mVerScrollbar->GetMinValue();
		float max = mVerScrollbar->GetMaxValue();

		float invertedValue = min + (max - min - (value - min));
		mViewCamera.SetPosition(Vec2F(mViewCamera.position->x, invertedValue));
		mViewCameraTargetPos = mViewCamera.GetPosition();
		mNeedRedraw = true;
	}

	void UIFrameScrollView::OnCameraTransformChanged()
	{
		mHorScrollbar->SetValue(mViewCamera.position->x);

		float min = mVerScrollbar->GetMinValue();
		float max = mVerScrollbar->GetMaxValue();
		mVerScrollbar->SetValue(min + (max - min - (mViewCamera.position->y - min)));
	}
}

CLASS_META(Editor::UIFrameScrollView)
{
	BASE_CLASS(Editor::UIScrollView);

	PROTECTED_FIELD(mHorScrollbar).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mVerScrollbar).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAvailableArea).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
	PUBLIC_FUNCTION(void, SetHorScrollbar, UIHorizontalScrollBar*);
	PUBLIC_FUNCTION(void, SetVerScrollbar, UIVerticalScrollBar*);
	PUBLIC_FUNCTION(void, SetViewArea, const RectF&);
	PUBLIC_FUNCTION(RectF, GetViewArea);
	PROTECTED_FUNCTION(void, UpdateCameraLimits, float);
	PROTECTED_FUNCTION(void, OnHorScrollScrolled, float);
	PROTECTED_FUNCTION(void, OnVerScrollScrolled, float);
	PROTECTED_FUNCTION(void, OnCameraTransformChanged);
}
END_META;
