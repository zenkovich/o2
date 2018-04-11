#include "stdafx.h"
#include "Spoiler.h"

#include "Animation/AnimatedFloat.h"
#include "Render/Render.h"
#include "UI/Button.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace o2
{
	UISpoiler::UISpoiler():
		UIVerticalLayout()
	{
		mBaseCorner = BaseCorner::Top;
		mFitByChildren = true;
		mExpandWidth = true;
		mExpandHeight = false;

		mExpandState = AddState("expand", Animation::EaseInOut(this, &mExpandCoef, 0.0f, 1.0f, 0.2f));
		mExpandState->animation.onUpdate = THIS_FUNC(UpdateExpanding);
		mExpandState->SetState(false);
		UpdateExpanding(0);
	}

	UISpoiler::UISpoiler(const UISpoiler& other):
		UIVerticalLayout(other), caption(this), headHeight(this), expanded(this)
	{
		mExpandState = GetStateObject("expand");
		if (!mExpandState)
			mExpandState = AddState("expand", Animation::EaseInOut(this, &mExpandCoef, 0.0f, 1.0f, 0.2f));

		mExpandState->animation.onUpdate = THIS_FUNC(UpdateExpanding);
		mExpandState->SetState(false);

		CheckExpandButton();
		RetargetStatesAnimations();
		UpdateExpanding(0);
	}

	UISpoiler& UISpoiler::operator=(const UISpoiler& other)
	{
		CopyData(other);
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
		SetBorderTop(height);
	}

	float UISpoiler::GetHeadHeight() const
	{
		return GetBorderTop();
	}

	void UISpoiler::CopyData(const Actor& otherActor)
	{
		const UISpoiler& other = dynamic_cast<const UISpoiler&>(otherActor);

		UIVerticalLayout::CopyData(other);

		mExpandState = GetStateObject("expand");
		if (!mExpandState)
			mExpandState = AddState("expand", Animation::EaseInOut(this, &mExpandCoef, 0.0f, 1.0f, 0.2f));

		mExpandState->animation.onUpdate = THIS_FUNC(UpdateExpanding);
		mExpandState->SetState(false);

		CheckExpandButton();
		UpdateExpanding(0);
	}

	void UISpoiler::UpdateExpanding(float dt)
	{
		layout->SetDirty(false);
	}

	float UISpoiler::GetMinHeightWithChildren() const
	{
		if (!mFitByChildren)
			return UIWidget::GetMinHeightWithChildren();

		float res = Math::Max(mChildWidgets.Count() - 1, 0)*mSpacing;
		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
				res += child->GetMinHeightWithChildren();
		}

		res = res*Math::Clamp01(mExpandCoef) + mBorder.top + mBorder.bottom;
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

	void UISpoiler::CheckExpandButton()
	{
		auto expandBtn = FindExpandButton();

		if (expandBtn)
			expandBtn->onClick = [&]() { SetExpanded(!IsExpanded()); };
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
