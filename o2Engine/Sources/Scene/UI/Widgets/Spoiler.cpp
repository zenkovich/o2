#include "stdafx.h"
#include "Spoiler.h"

#include "Animation/AnimatedFloat.h"
#include "Render/Render.h"
#include "Scene/UI/Button.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"

namespace o2
{
	UISpoiler::UISpoiler():
		UIVerticalLayout()
	{
		mBaseCorner = BaseCorner::Top;
		mFitByChildren = true;
		mExpandWidth = true;
		mExpandHeight = false;

		//Parametric(begin, end, duration, 0.0f, 0.4f, 1.0f, 0.6f);
		CreateExpandAnimation();
		mExpandState->animation.onUpdate = THIS_FUNC(UpdateExpanding);
		mExpandState->SetState(false);
		UpdateExpanding(0);
	}

	UISpoiler::UISpoiler(const UISpoiler& other):
		UIVerticalLayout(other), caption(this), headHeight(this), expanded(this), mHeadHeight(other.mHeadHeight)
	{
		mExpandState = GetStateObject("expand");
		if (!mExpandState)
			CreateExpandAnimation();

		mExpandState->animation.onUpdate = THIS_FUNC(UpdateExpanding);
		mExpandState->SetState(false);

		InitializeControls();
		RetargetStatesAnimations();
		UpdateExpanding(0);
	}

	UISpoiler& UISpoiler::operator=(const UISpoiler& other)
	{
		UIVerticalLayout::operator=(other);
		return *this;
	}

	void UISpoiler::Expand()
	{
		SetExpanded(true);
	}

	void UISpoiler::Collapse()
	{
		SetExpanded(false);
	}

	void UISpoiler::SetExpanded(bool expand)
	{
		if (mExpandState)
			mExpandState->SetState(expand);

		if (expand)
			onExpand();

		auto expandBtn = FindExpandButton();
		if (expandBtn)
			expandBtn->SetState("expanded", expand);

		mTargetHeight = UIVerticalLayout::GetMinHeightWithChildren();
	}

	bool UISpoiler::IsExpanded() const
	{
		return mExpandState ? mExpandState->GetState() : false;
	}

#undef DrawText
	void UISpoiler::Draw()
	{
		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		OnDrawn();

		if (!IsFullyCollapsed())
		{
			bool clipping = !IsFullyExpanded();
			if (clipping)
				o2Render.EnableScissorTest(mBounds);

			for (auto child : mDrawingChildren)
				child->Draw();

			if (clipping)
				o2Render.DisableScissorTest();
		}

		for (auto child : mInternalWidgets)
			child->Draw();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		DrawDebugFrame();
	}

	void UISpoiler::SetCaption(const WString& caption)
	{
		auto textLayer = GetLayerDrawable<Text>("caption");
		if (textLayer)
			textLayer->text = caption;
	}

	WString UISpoiler::GetCaption() const
	{
		auto textLayer = GetLayerDrawable<Text>("caption");
		if (textLayer)
			return textLayer->text;

		return "";
	}

	void UISpoiler::SetHeadHeight(float height)
	{
		mHeadHeight = height;
	}

	float UISpoiler::GetHeadHeight() const
	{
		return mHeadHeight;
	}

	void UISpoiler::CopyData(const Actor& otherActor)
	{
		const UISpoiler& other = dynamic_cast<const UISpoiler&>(otherActor);

		UIVerticalLayout::CopyData(other);

		mExpandState = GetStateObject("expand");
		if (!mExpandState)
			CreateExpandAnimation();

		mExpandState->animation.onUpdate = THIS_FUNC(UpdateExpanding);
		mExpandState->SetState(false);

		mHeadHeight = other.mHeadHeight;

		InitializeControls();
		UpdateExpanding(0);
	}

	void UISpoiler::RearrangeChilds()
	{
		float borderTop = mBorder.top;
		mBorder.top = borderTop + mHeadHeight;

		UIVerticalLayout::RearrangeChilds();

		mBorder.top = borderTop;
	}

	void UISpoiler::UpdateExpanding(float dt)
	{
		layout->SetDirty(false);
	}

	void UISpoiler::CreateExpandAnimation()
	{
		mExpandState = AddState("expand", Animation::Parametric(this, &mExpandCoef, 0.0f, 1.0f, 0.4f, 0.0f, 0.4f, 1.0f, 1.0f));
	}

	float UISpoiler::GetMinHeightWithChildren() const
	{
		if (!mFitByChildren)
			return UIWidget::GetMinHeightWithChildren();

		float res = Math::Max(mChildWidgets.Count() - 1, 0)*mSpacing + mBorder.top + mBorder.bottom;
		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
				res += child->GetMinHeightWithChildren();
		}

		res = res*Math::Clamp01(mExpandCoef) + mHeadHeight;
		res = Math::Max(res, layout->mData->minSize.y);

		return res;
	}

	void UISpoiler::UpdateLayoutParametres()
	{
		if (IsFullyExpanded())
			UIVerticalLayout::UpdateLayoutParametres();
		else
		{
			layout->mData->weight.y = 1;
			layout->mData->minSize.y = 0;
		}
	}

	void UISpoiler::InitializeControls()
	{
		auto textLayer = GetLayerDrawable<Text>("caption");
		auto expandBtn = FindExpandButton();

		if (expandBtn)
		{
			expandBtn->onClick = [&]() { SetExpanded(!IsExpanded()); };
			expandBtn->isPointInside = [=](const Vec2F& point) {
				return expandBtn->layout->IsPointInside(point) || (textLayer && textLayer->IsPointInside(point));
			};
		}
	}

	UIButton* UISpoiler::FindExpandButton() const
	{
		auto expandBtn = mInternalWidgets.FindMatch(
			[](UIWidget* x) { return x->GetName() == "expand" && x->GetType() == TypeOf(UIButton); });

		if (expandBtn)
			return dynamic_cast<UIButton*>(expandBtn);

		return nullptr;
	}

	bool UISpoiler::IsFullyExpanded() const
	{
		if (!mExpandState)
			return true;

		return mExpandState->GetState() && !mExpandState->animation.IsPlaying();
	}

	bool UISpoiler::IsFullyCollapsed() const
	{
		if (!mExpandState)
			return false;

		return !mExpandState->GetState() && !mExpandState->animation.IsPlaying();
	}
}

DECLARE_CLASS(o2::UISpoiler);
