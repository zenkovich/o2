#include "stdafx.h"
#include "Timeline.h"

#include "Application/Input.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/HorizontalScrollBar.h"

namespace Editor
{

	AnimationTimeline::AnimationTimeline() :
		Widget()
	{
		mTextFont = FontRef("stdFont.ttf");
		mTextFont->CheckCharacters("0123456789.,+-", 10);

		mText = mnew Text(mTextFont);
		mText->horAlign = HorAlign::Middle;
		mText->verAlign = VerAlign::Bottom;
		mText->height = 8;
	}

	AnimationTimeline::AnimationTimeline(const AnimationTimeline& other) :
		Widget(other), mTextFont(other.mTextFont), mText(other.mText->CloneAs<Text>())
	{}

	AnimationTimeline::~AnimationTimeline()
	{
		delete mText;
	}

	AnimationTimeline& AnimationTimeline::operator=(const AnimationTimeline& other)
	{
		delete mText;

		Widget::operator=(other);

		mTextFont = other.mTextFont;
		mText = other.mText->CloneAs<Text>();

		return *this;
	}

	void AnimationTimeline::Draw()
	{
		Widget::Draw();

		float beginPos = mScaleOffset - mSmoothViewScroll*mOneSecondDefaultSize*mSmoothViewZoom;

		int bigLinePeriod;
		float bigLineTimeAmount;
		ChooseScaleParams(bigLinePeriod, bigLineTimeAmount);

		float posDelta = bigLineTimeAmount * mOneSecondDefaultSize*mSmoothViewZoom;

		int lineIdx = 0;
		while (beginPos < 0)
		{
			lineIdx++;
			beginPos += posDelta;
		}

		float endPos = layout->GetWidth();

		RectF worldRect = layout->GetWorldRect();
		for (float pos = beginPos; pos < endPos; pos += posDelta, lineIdx++)
		{
			float screenPos = pos + worldRect.left;
			bool isBigLine = lineIdx%bigLinePeriod == 0;

			o2Render.DrawAALine(Vec2F(screenPos, worldRect.top - (isBigLine ? mBigLineOffset : mSmallLineOffset)),
								Vec2F(screenPos, worldRect.bottom),
								isBigLine ? mBigLineColor : mSmallLineColor);

			if (isBigLine)
			{
				mText->SetText((String)((float)lineIdx*bigLineTimeAmount));
				mText->SetPosition(Vec2F(screenPos, worldRect.top - mTextOffset));
				mText->Draw();
			}
		}
	}

	void AnimationTimeline::Update(float dt)
	{
		Widget::Update(dt);

		UpdateZooming(dt);
		UpdateScrolling(dt);
	}

	void AnimationTimeline::UpdateZooming(float dt)
	{
		if (!Math::Equals(mSmoothViewZoom, mViewZoom))
		{
			mSmoothViewZoom = Math::Lerp(mSmoothViewZoom, mViewZoom, dt*mScaleSmoothLerpCoef);
			UpdateScrollBarHandleSize();
		}

		if (IsUnderPoint(o2Input.GetCursorPos()) && !Math::Equals(0.0f, o2Input.GetMouseWheelDelta()))
			mViewZoom = Math::Clamp(mViewZoom/(1.0f - o2Input.GetMouseWheelDelta()*mScaleSense), mMinScale, mMaxScale);
	}

	void AnimationTimeline::UpdateScrollBarHandleSize()
	{
		if (mScrollBar)
			mScrollBar->SetScrollHandleSize(layout->GetWidth()/mOneSecondDefaultSize/mSmoothViewZoom);
	}

	float AnimationTimeline::LocalToWorld(float pos) const
	{
		return (pos - mSmoothViewScroll)*(mOneSecondDefaultSize*mSmoothViewZoom) + mScaleOffset + layout->GetWorldLeft();
	}

	float AnimationTimeline::WorldToLocal(float pos) const
	{
		return (pos - layout->GetWorldLeft() - mScaleOffset)/(mOneSecondDefaultSize*mSmoothViewZoom) + mSmoothViewScroll;
	}

	void AnimationTimeline::OnTransformUpdated()
	{
		UpdateScrollBarHandleSize();
		Widget::OnTransformUpdated();
	}

	void AnimationTimeline::ChooseScaleParams(int& bigLinePeriod, float& bigLineTimeAmount)
	{
		struct Cfg
		{
			int chunkSegments;
			float chunkDuration;
		};

		Cfg configs[] = { { 10, 0.1f }, { 5, 0.1f }, { 2, 0.1f }, { 5, 0.5f }, { 10, 1.0f }, { 5, 1.0f }, { 2, 1.0f }, { 5, 5.0f }, { 6, 30.0f } };

		Cfg nearestCfg;
		float nearestCfgScreenChunkSegmentSizeDiff = FLT_MAX;
		for (auto cfg : configs) 
		{
			float screenChunkSegmentSize = cfg.chunkDuration/(float)cfg.chunkSegments*mOneSecondDefaultSize*mSmoothViewZoom;
			float screenChunkSegmentSizeDiff = mPerfectScaleSegmentSize - screenChunkSegmentSize;
			if (screenChunkSegmentSizeDiff > 0.0f)
				screenChunkSegmentSizeDiff *= 2.0f;

			if (Math::Abs(screenChunkSegmentSizeDiff) < nearestCfgScreenChunkSegmentSizeDiff)
			{
				nearestCfgScreenChunkSegmentSizeDiff = screenChunkSegmentSizeDiff;
				nearestCfg = cfg;
			}
		}

		bigLinePeriod = nearestCfg.chunkSegments;
		bigLineTimeAmount = nearestCfg.chunkDuration/(float)nearestCfg.chunkSegments;
	}

	void AnimationTimeline::UpdateScrolling(float dt)
	{
		if (IsUnderPoint(o2Input.GetCursorPos()) && o2Input.IsRightMousePressed())
		{
			mBeginDragViewScrollOffset = WorldToLocal(o2Input.GetCursorPos().x);
			mDragViewScroll = true;
		}

		if (o2Input.IsRightMouseReleased() && mDragViewScroll)
		{
			mViewScrollSpeed = -o2Input.GetCursorDelta().x/dt/(mOneSecondDefaultSize*mSmoothViewZoom);
			mDragViewScroll = false;
		}

		if (mDragViewScroll)
		{
			mViewScroll -= WorldToLocal(o2Input.GetCursorPos().x) - mBeginDragViewScrollOffset;

			if (mViewScroll < 0.0f)
				mViewScroll = mViewScroll/2.0f;

			if (mViewScroll > mDuration)
				mViewScroll = Math::Lerp(mDuration, mViewScroll, 0.5f);

			mSmoothViewScroll = mViewScroll;

			if (mScrollBar)
				mScrollBar->SetValueForcible(mSmoothViewScroll);
		}

		if (!mDragViewScroll && Math::Abs(mViewScrollSpeed) > 0.01f)
		{
			mViewScroll += mViewScrollSpeed*dt;
			mSmoothViewScroll = mViewScroll;
			mViewScrollSpeed = Math::Lerp(mViewScrollSpeed, 0.0f, dt*mScrollSpeedDecreaseCoef);

			if (mScrollBar)
				mScrollBar->SetValueForcible(mSmoothViewScroll);
		}

		if (!mDragViewScroll)
		{
			if (mViewScroll < 0.0f)
				mViewScroll = Math::Lerp(mViewScroll, 0.0f, dt*mScrollBorderBounceCoef);

			if (mViewScroll > mDuration)
				mViewScroll = Math::Lerp(mSmoothViewScroll, mDuration, dt*mScrollBorderBounceCoef);
		}

		if (!Math::Equals(mViewScroll, mSmoothViewScroll, 0.01f))
		{
			mSmoothViewScroll = Math::Lerp(mSmoothViewScroll, mViewScroll, dt*mScrollSmoothCoef);

			if (mScrollBar)
				mScrollBar->SetValueForcible(mSmoothViewScroll);
		}
	}

	void AnimationTimeline::SetDuration(float duration)
	{
		mDuration = duration;

		if (mScrollBar)
		{
			mScrollBar->minValue = 0.0f;
			mScrollBar->maxValue = mDuration;
			UpdateScrollBarHandleSize();
		}
	}

	float AnimationTimeline::GetDuration() const
	{
		return mDuration;
	}

	void AnimationTimeline::SetScroll(float scroll)
	{
		mSmoothViewScroll = scroll;
	}

	float AnimationTimeline::GetScroll() const
	{
		return mSmoothViewScroll;
	}

	void AnimationTimeline::SetScale(float scale)
	{
		mViewZoom = scale;
	}

	float AnimationTimeline::GetScale() const
	{
		return mViewZoom;
	}

	Text* AnimationTimeline::GetText() const
	{
		return mText;
	}

	void AnimationTimeline::SetScrollBar(HorizontalScrollBar* scrollBar)
	{
		mScrollBar = scrollBar;
		mScrollBar->onUserChange = [&](float value) { mViewScroll = value; };

		AddChild(scrollBar);
	}

	HorizontalScrollBar* AnimationTimeline::GetScrollBar() const
	{
		return mScrollBar;
	}

}

DECLARE_CLASS(Editor::AnimationTimeline);
