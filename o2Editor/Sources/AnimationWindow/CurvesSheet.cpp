#include "stdafx.h"

#include "AnimationWindow/CurvesSheet.h"

#include "Animation/AnimatedFloat.h"
#include "AnimationWindow/Timeline.h"
#include "AnimationWindow/Tree.h"
#include "Scene/UI/WidgetLayout.h"

namespace Editor
{

	CurvesSheet::CurvesSheet()
	{
		InitializeControls();
	}

	CurvesSheet::CurvesSheet(const CurvesSheet& other)
	{
		InitializeControls();
	}

	CurvesSheet::~CurvesSheet()
	{}

	void CurvesSheet::Initialize(AnimationTimeline* timeline, AnimationTree* tree)
	{
		mTimeline = timeline;
		mTree = tree;
	}

	void CurvesSheet::SetAnimation(Animation* animation)
	{
		mEditor->RemoveAllEditingCurves();

		for (auto animValue : animation->GetAnimationsValues())
		{
			if (auto floatAnimValue = dynamic_cast<AnimatedValue<float>*>(animValue.animatedValue))
			{
				mEditor->AddEditingCurve(animValue.targetPath, &floatAnimValue->curve);
			}
		}
	}

	void CurvesSheet::InitializeControls()
	{
		mEditor = mnew CurveEditor();
		*mEditor->layout = WidgetLayout::BothStretch();
		AddChild(mEditor);
	}

	CurvesSheet& CurvesSheet::operator=(const CurvesSheet& other)
	{
		delete mEditor;

		Widget::operator=(other);
		InitializeControls();

		return *this;
	}

}

DECLARE_CLASS(Editor::CurvesSheet);
