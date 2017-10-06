#include "Spoiler.h"

#include "Animation\AnimatedFloat.h"
#include "Render\Render.h"

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
		UIVerticalLayout::operator=(other);

		mExpandState = GetStateObject("expand");
		if (!mExpandState)
			mExpandState = AddState("expand", Animation::EaseInOut(this, &mExpandCoef, 0.0f, 1.0f, 0.2f));

		mExpandState->animation.onUpdate = THIS_FUNC(UpdateExpanding);
		mExpandState->SetState(false);
		UpdateExpanding(0);

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

			for (auto child : mChildren)
				child->Draw();

			if (clipping)
				o2Render.DisableScissorTest();
		}

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		DrawDebugFrame();
	}

	void UISpoiler::UpdateExpanding(float dt)
	{
		UpdateLayout();
	}

	void UISpoiler::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (CheckIsLayoutDrivenByParent(forcible))
			return;

		UpdateLayoutParametres();
		ExpandSizeByChilds();
		RecalculateAbsRect();
		UpdateLayersLayouts();

		if (withChildren)
			RearrangeChilds();
	}

	float UISpoiler::GetMinHeightWithChildren() const
	{
		return UIVerticalLayout::GetMinHeightWithChildren()*Math::Clamp01(mExpandCoef);
	}

// 	void UISpoiler::ExpandSizeByChilds()
// 	{
// 		const static Vec2F relativePivots[] ={
// 			Vec2F(0.0f, 0.5f), // Left
// 			Vec2F(1.0f, 0.5f), // Right
// 			Vec2F(0.5f, 1.0f), // Top
// 			Vec2F(0.5f, 0.0f), // Bottom
// 			Vec2F(0.5f, 0.5f), // Center
// 			Vec2F(0.0f, 0.0f), // LeftBottom
// 			Vec2F(0.0f, 1.0f), // LeftTop
// 			Vec2F(1.0f, 0.0f), // RightBottom
// 			Vec2F(1.0f, 1.0f)  // RightTop
// 		};
// 
// 		Vec2F relativePivot = relativePivots[(int)mBaseCorner];
// 		Vec2F size(Math::Max(GetLayoutWidth(), layout.mMinSize.x), Math::Max(GetLayoutHeight(), layout.mMinSize.y));
// 
// 		Vec2F szDelta = size - layout.mLocalRect.Size();
// 
// 		if (mExpandWidth)
// 			szDelta.x = 0;
// 
// 		if (mExpandHeight)
// 			szDelta.y = 0;
// 
// 		layout.mOffsetMax += szDelta*(Vec2F::One() - relativePivot);
// 		layout.mOffsetMin -= szDelta*relativePivot;
// 	}

	void UISpoiler::UpdateLayoutParametres()
	{
		if (IsFullyExpanded())
			UIVerticalLayout::UpdateLayoutParametres();
		else
		{
			layout.mWeight.y = 1;
			layout.mMinSize.y = 0;
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

CLASS_META(o2::UISpoiler)
{
	BASE_CLASS(o2::UIVerticalLayout);

	PROTECTED_FIELD(mExpandState);
	PROTECTED_FIELD(mExpandCoef);
	PROTECTED_FIELD(mTargetHeight);

	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
	PROTECTED_FUNCTION(void, UpdateExpanding, float);
	PROTECTED_FUNCTION(float, GetMinHeightWithChildren);
	PROTECTED_FUNCTION(void, UpdateLayoutParametres);
	PROTECTED_FUNCTION(bool, IsFullyExpanded);
	PROTECTED_FUNCTION(bool, IsFullyCollapsed);
}
END_META;
