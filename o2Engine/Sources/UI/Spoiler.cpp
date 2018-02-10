#include "stdafx.h"
#include "Spoiler.h"

#include "Animation/AnimatedFloat.h"
#include "Render/Render.h"
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
		UIVerticalLayout(other)
	{
		mExpandState = GetStateObject("expand");
		if (!mExpandState)
			mExpandState = AddState("expand", Animation::EaseInOut(this, &mExpandCoef, 0.0f, 1.0f, 0.2f));

		mExpandState->animation.onUpdate = THIS_FUNC(UpdateExpanding);
		mExpandState->SetState(false);
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

		mTargetHeight = UIVerticalLayout::GetMinHeightWithChildren();
	}

	bool UISpoiler::IsExpanded() const
	{
		return mExpandState ? mExpandState->GetState() : false;
	}

#undef DrawText
	void UISpoiler::Draw()
	{
		if (mFullyDisabled || mIsClipped)
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

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		o2Debug.DrawText(layout->GetWorldLeftTop(), (String)layout->GetSize());

		DrawDebugFrame();
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
		UpdateExpanding(0);
	}

	void UISpoiler::UpdateExpanding(float dt)
	{
		layout->SetDirty(false);
	}

	void UISpoiler::UpdateTransform(bool withChildren /*= true*/)
	{
		UpdateLayoutParametres();
		ExpandSizeByChilds();

		layout->Update();

		if (withChildren)
			RearrangeChilds();
	}

	float UISpoiler::GetMinHeightWithChildren() const
	{
		return UIVerticalLayout::GetMinHeightWithChildren()*Math::Clamp01(mExpandCoef);
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
