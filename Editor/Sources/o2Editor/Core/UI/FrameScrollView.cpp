#include "o2Editor/stdafx.h"
#include "FrameScrollView.h"

#include "o2/Scene/UI/Widgets/HorizontalScrollBar.h"
#include "o2/Scene/UI/Widgets/VerticalScrollBar.h"
#include "o2/Scene/UI/WidgetLayout.h"

namespace Editor
{

	FrameScrollView::FrameScrollView():
		ScrollView()
	{
		mReady = false;

		mHorScrollbar = mnew HorizontalScrollBar();
		*mHorScrollbar->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 20);
		mHorScrollbar->SetInternalParent(this);
		mHorScrollbar->onUserChange = THIS_FUNC(OnHorScrollScrolled);

		mVerScrollbar = mnew VerticalScrollBar();
		*mVerScrollbar->layout = WidgetLayout::VerStretch(HorAlign::Right, 0, 0, 20);
		mVerScrollbar->SetInternalParent(this);
		mVerScrollbar->onUserChange = THIS_FUNC(OnVerScrollScrolled);

		mReady = true;
	}

	FrameScrollView::FrameScrollView(const FrameScrollView& other):
		ScrollView(other), 
		mHorScrollbar(other.mHorScrollbar->CloneAs<HorizontalScrollBar>()), 
		mVerScrollbar(other.mVerScrollbar->CloneAs<VerticalScrollBar>())
	{
		mReady = false;

		mHorScrollbar->SetInternalParent(this);
		mHorScrollbar->onUserChange = THIS_FUNC(OnHorScrollScrolled);

		mVerScrollbar->SetInternalParent(this);
		mVerScrollbar->onUserChange = THIS_FUNC(OnVerScrollScrolled);

		RetargetStatesAnimations();

		mReady = true;
	}

	FrameScrollView::~FrameScrollView()
	{
		if (mHorScrollbar)
			delete mHorScrollbar;

		if (mVerScrollbar)
			delete mVerScrollbar;
	}

	FrameScrollView& FrameScrollView::operator=(const FrameScrollView& other)
	{
		ScrollView::operator=(other);
		return *this;
	}

	void FrameScrollView::Draw()
	{
		ScrollView::Draw();

		if (!mReady)
			return;

		mHorScrollbar->Draw();
		mVerScrollbar->Draw();
	}

	void FrameScrollView::Update(float dt)
	{
		ScrollView::Update(dt);

		if (!mReady)
			return;

		UpdateCameraLimits(dt);

		mHorScrollbar->Update(dt);
		mVerScrollbar->Update(dt);
	}

	void FrameScrollView::UpdateSelfTransform()
{
		ScrollView::UpdateSelfTransform();

		if (!mReady)
			return;

		mHorScrollbar->UpdateSelfTransform();
		mVerScrollbar->UpdateSelfTransform();
	}

	void FrameScrollView::SetHorScrollbar(HorizontalScrollBar* scrollbar)
	{
		delete mHorScrollbar;
		mHorScrollbar = scrollbar;
		mHorScrollbar->SetInternalParent(this);
		mHorScrollbar->onUserChange = THIS_FUNC(OnHorScrollScrolled);

		SetLayoutDirty();
	}

	void FrameScrollView::SetVerScrollbar(VerticalScrollBar* scrollbar)
	{
		delete mVerScrollbar;
		mVerScrollbar = scrollbar;
		mVerScrollbar->SetInternalParent(this);
		mVerScrollbar->onUserChange = THIS_FUNC(OnVerScrollScrolled);

		SetLayoutDirty();
	}

	void FrameScrollView::SetViewArea(const RectF& area)
	{
		mAvailableArea = area;
	}

	RectF FrameScrollView::GetViewArea() const
	{
		return mAvailableArea;
	}

	void FrameScrollView::CopyData(const Actor& otherActor)
	{
		const FrameScrollView& other = dynamic_cast<const FrameScrollView&>(otherActor);

		ScrollView::CopyData(other);

		delete mHorScrollbar;
		delete mVerScrollbar;

		mHorScrollbar = other.mHorScrollbar->CloneAs<HorizontalScrollBar>();
		mHorScrollbar->SetInternalParent(this);
		mHorScrollbar->onUserChange = THIS_FUNC(OnHorScrollScrolled);

		mVerScrollbar = other.mVerScrollbar->CloneAs<VerticalScrollBar>();
		mVerScrollbar->SetInternalParent(this);
		mVerScrollbar->onUserChange = THIS_FUNC(OnVerScrollScrolled);
	}

	void FrameScrollView::UpdateCameraLimits(float dt)
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

	void FrameScrollView::OnHorScrollScrolled(float value)
	{
		mViewCamera.SetPosition(Vec2F(value, mViewCamera.GetPosition().y));
		mViewCameraTargetPos = mViewCamera.GetPosition();
		mNeedRedraw = true;
	}

	void FrameScrollView::OnVerScrollScrolled(float value)
	{
		float min = mVerScrollbar->GetMinValue();
		float max = mVerScrollbar->GetMaxValue();

		float invertedValue = min + (max - min - (value - min));
		mViewCamera.SetPosition(Vec2F(mViewCamera.GetPosition().x, invertedValue));
		mViewCameraTargetPos = mViewCamera.GetPosition();
		mNeedRedraw = true;
	}

	void FrameScrollView::OnCameraTransformChanged()
	{
		mHorScrollbar->SetValue(mViewCamera.GetPosition().x);

		float min = mVerScrollbar->GetMinValue();
		float max = mVerScrollbar->GetMaxValue();
		mVerScrollbar->SetValue(min + (max - min - (mViewCamera.GetPosition().y - min)));

		onViewChanged();
	}
}

DECLARE_CLASS(Editor::FrameScrollView);
