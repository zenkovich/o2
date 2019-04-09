#include "stdafx.h"
#include "Timeline.h"

#include "Application/Input.h"
#include "Scene/UI/WidgetLayout.h"

namespace Editor
{

	AnimationTimeline::AnimationTimeline():
		Widget()
	{
		mTextFont = FontRef("stdFont.ttf");
		mTextFont->CheckCharacters("0123456789.,+-", 10);

		mText = mnew Text(mTextFont);
		mText->horAlign = HorAlign::Middle;
		mText->verAlign = VerAlign::Bottom;
		mText->height = 8;
	}

	AnimationTimeline::AnimationTimeline(const AnimationTimeline& other):
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

		float beginPos = mScaleOffset - mScroll*mOneSecondDefaultSize*mSmoothScale;
		float posDelta = mOneSecondDefaultSize*mSmoothScale/10.0f;

		int bigLinePeriod = 10.0f;
		float bigLineTimeAmount = 0.1f;
		if (posDelta < 5.0f)
		{
			bigLinePeriod = 5.0f;
			bigLineTimeAmount = 1.0f;
			posDelta *= 10.0f;
		}

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

		if (!Math::Equals(mSmoothScale, mScale))
			mSmoothScale = Math::Lerp(mSmoothScale, mScale, dt*mScaleSmoothLerpCoef);

		if (IsUnderPoint(o2Input.GetCursorPos()) && !Math::Equals(0.0f, o2Input.GetMouseWheelDelta()))
			mScale = Math::Clamp(mScale/(1.0f - o2Input.GetMouseWheelDelta()*mScaleSense), mMinScale, mMaxScale);
	}

	void AnimationTimeline::SetScroll(float scroll)
	{
		mScroll = scroll;
	}

	float AnimationTimeline::GetScroll() const
	{
		return mScroll;
	}

	void AnimationTimeline::SetScale(float scale)
	{
		mScale = scale;
	}

	float AnimationTimeline::GetScale() const
	{
		return mScale;
	}

	Text* AnimationTimeline::GetText() const
	{
		return mText;
	}
}

DECLARE_CLASS(Editor::AnimationTimeline);
