#include "stdafx.h"

#include "AnimationWindow/CurvesSheet.h"

#include "Animation/AnimatedFloat.h"
#include "AnimationWindow/AnimationWindow.h"
#include "AnimationWindow/Timeline.h"
#include "AnimationWindow/Tree.h"
#include "Scene/UI/UIManager.h"
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

	CurvesSheet& CurvesSheet::operator=(const CurvesSheet& other)
	{
		delete mEditor;

		Widget::operator=(other);
		InitializeControls();

		return *this;
	}

	void CurvesSheet::SetAnimation(Animation* animation)
	{
		mEditor->RemoveAllCurves();

		for (auto animValue : animation->GetAnimationsValues())
		{
			if (auto floatAnimValue = dynamic_cast<AnimatedValue<float>*>(animValue.animatedValue))
				mEditor->AddCurve(animValue.targetPath, &floatAnimValue->curve, Color4(44, 62, 80));
		}

		mEditor->actionsListDelegate = &mAnimationWindow->mActionsList;
	}

	void CurvesSheet::OnAnimationChanged()
	{
		// Check new curves
		auto currentCurves = mEditor->GetCurves();
		Vector<Curve*> animCurves;
		for (auto animValue : mAnimationWindow->mAnimation->GetAnimationsValues())
		{
			if (auto floatAnimValue = dynamic_cast<AnimatedValue<float>*>(animValue.animatedValue))
			{
				animCurves.Add(&floatAnimValue->curve);
				if (!currentCurves.ContainsValue(&floatAnimValue->curve))
					mEditor->AddCurve(animValue.targetPath, &floatAnimValue->curve, Color4(44, 62, 80));
			}
		}

		// Check removed curves
		for (auto curve : mEditor->GetCurves())
		{
			if (!animCurves.Contains(curve.Value()))
				mEditor->RemoveCurve(curve.Value());
		}
	}

	void CurvesSheet::InitializeControls()
	{
		mEditor = mnew CurveEditor();
		*mEditor->layout = WidgetLayout::BothStretch();

		auto verScroll = o2UI.CreateVerScrollBar();
		*verScroll->layout = WidgetLayout::VerStretch(HorAlign::Right, 0, 0, 10, 0);
		mEditor->SetVerScrollbar(verScroll);

		mEditor->SetMainHandleImages(ImageAssetRef("ui/CurveHandle.png"),
										   ImageAssetRef("ui/CurveHandleHover.png"),
										   ImageAssetRef("ui/CurveHandlePressed.png"),
										   ImageAssetRef("ui/CurveHandleSelected.png"));

		mEditor->SetSupportHandleImages(ImageAssetRef("ui/CurveSupportHandle.png"),
											  ImageAssetRef("ui/CurveSupportHandleHover.png"),
											  ImageAssetRef("ui/CurveSupportHandlePressed.png"),
											  ImageAssetRef("ui/CurveSupportHandleSelected.png"));

		mEditor->SetSelectionSpriteImage(ImageAssetRef("ui/UI_Window_place.png"));

		mEditor->verGridEnabled = false;
		mEditor->onViewChanged += THIS_FUNC(OnEditorViewChanged);

		AddChild(mEditor);
	}

	void CurvesSheet::SetCameraAsTimelineView()
	{
		Camera camera = mEditor->GetCamera();
		RectF camRect = camera.GetRect();
		camRect.left = mAnimationWindow->mTimeline->WorldToLocal(mAnimationWindow->mTimeline->layout->worldLeft);
		camRect.right = mAnimationWindow->mTimeline->WorldToLocal(mAnimationWindow->mTimeline->layout->worldRight);

		camera.SetRect(camRect, false);
		RectF checkRect = camera.GetRect();

		mEditorViewLock = true;
		mEditor->SetCamera(camera);
		mEditorViewLock = false;
	}

	void CurvesSheet::OnEditorViewChanged()
	{
		if (mEditorViewLock || !IsEnabled())
			return;

		mAnimationWindow->mTimeline->SetViewRange(mEditor->GetCamera().GetRect().left, mEditor->GetCamera().GetRect().right);
	}

}

DECLARE_CLASS(Editor::CurvesSheet);
