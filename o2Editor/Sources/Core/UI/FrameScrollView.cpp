#include "FrameScrollView.h"

#include "UI/HorizontalScrollBar.h"
#include "UI/VerticalScrollBar.h"
#include "UI/WidgetLayout.h"

namespace Editor
{

	UIFrameScrollView::UIFrameScrollView():
		UIScrollView()
	{
		mReady = false;

		mHorScrollbar = mnew UIHorizontalScrollBar();
		*mHorScrollbar->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 20);
		mHorScrollbar->SetParent(this);
		mHorScrollbar->onUserChange = THIS_FUNC(OnHorScrollScrolled);
		mChildren.Remove(mHorScrollbar);

		mVerScrollbar = mnew UIVerticalScrollBar();
		*mVerScrollbar->layout = UIWidgetLayout::VerStretch(HorAlign::Right, 0, 0, 20);
		mVerScrollbar->SetParent(this);
		mVerScrollbar->onUserChange = THIS_FUNC(OnVerScrollScrolled);
		mChildren.Remove(mVerScrollbar);

		mReady = true;
	}

	UIFrameScrollView::UIFrameScrollView(const UIFrameScrollView& other):
		UIScrollView(other), 
		mHorScrollbar(other.mHorScrollbar->CloneAs<UIHorizontalScrollBar>()), 
		mVerScrollbar(other.mVerScrollbar->CloneAs<UIVerticalScrollBar>())
	{
		mReady = false;

		mHorScrollbar->SetParent(this);
		mHorScrollbar->onUserChange = THIS_FUNC(OnHorScrollScrolled);
		mChildren.Remove(mHorScrollbar);

		mVerScrollbar->SetParent(this);
		mVerScrollbar->onUserChange = THIS_FUNC(OnVerScrollScrolled);
		mChildren.Remove(mVerScrollbar);

		RetargetStatesAnimations();

		mReady = true;
	}

	UIFrameScrollView::~UIFrameScrollView()
	{
		if (mHorScrollbar)
			delete mHorScrollbar;

		if (mVerScrollbar)
			delete mVerScrollbar;
	}

	UIFrameScrollView& UIFrameScrollView::operator=(const UIFrameScrollView& other)
	{
		CopyData(other);
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

	void UIFrameScrollView::UpdateLayout(bool withChildren /*= true*/)
	{
		UIScrollView::UpdateLayout(withChildren);

		if (!mReady)
			return;

		mHorScrollbar->UpdateLayout(withChildren);
		mVerScrollbar->UpdateLayout(withChildren);
	}

	void UIFrameScrollView::SetHorScrollbar(UIHorizontalScrollBar* scrollbar)
	{
		delete mHorScrollbar;
		mHorScrollbar = scrollbar;
		AddChild(mHorScrollbar);
		mChildren.Remove(mHorScrollbar);
		mHorScrollbar->onUserChange = THIS_FUNC(OnHorScrollScrolled);

		UpdateLayout();
	}

	void UIFrameScrollView::SetVerScrollbar(UIVerticalScrollBar* scrollbar)
	{
		delete mVerScrollbar;
		mVerScrollbar = scrollbar;
		AddChild(mVerScrollbar);
		mChildren.Remove(mVerScrollbar);
		mVerScrollbar->onUserChange = THIS_FUNC(OnVerScrollScrolled);

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

	void UIFrameScrollView::CopyData(const Actor& otherActor)
	{
		const UIFrameScrollView& other = dynamic_cast<const UIFrameScrollView&>(otherActor);

		UIScrollView::CopyData(other);

		delete mHorScrollbar;
		delete mVerScrollbar;

		mHorScrollbar = other.mHorScrollbar->CloneAs<UIHorizontalScrollBar>();
		mHorScrollbar->SetParent(this);
		mHorScrollbar->onUserChange = THIS_FUNC(OnHorScrollScrolled);
		mChildren.Remove(mHorScrollbar);

		mVerScrollbar = other.mVerScrollbar->CloneAs<UIVerticalScrollBar>();
		mVerScrollbar->SetParent(this);
		mVerScrollbar->onUserChange = THIS_FUNC(OnVerScrollScrolled);
		mChildren.Remove(mVerScrollbar);
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

DECLARE_CLASS(Editor::UIFrameScrollView);
