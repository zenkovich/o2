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
		delete mCurvesEditor;

		Widget::operator=(other);
		InitializeControls();

		return *this;
	}

	void CurvesSheet::SetAnimation(Animation* animation)
	{
		mCurvesEditor->RemoveAllCurves();

		for (auto animValue : animation->GetAnimationsValues())
		{
			if (auto floatAnimValue = dynamic_cast<AnimatedValue<float>*>(animValue.animatedValue))
				mCurvesEditor->AddCurve(animValue.targetPath, &floatAnimValue->curve);
		}

		UpdateCurvesColors();

		mCurvesEditor->actionsListDelegate = &mAnimationWindow->mActionsList;
	}

	void CurvesSheet::UpdateCurvesColors()
	{
		Color4 curveColor(44, 62, 80);
		int colorIdx = 0;
		for (auto animValue : mAnimationWindow->mAnimation->GetAnimationsValues())
		{
			if (auto floatAnimValue = dynamic_cast<AnimatedValue<float>*>(animValue.animatedValue))
			{
				mAnimationWindow->mTree->SetAnimationValueColor(animValue.targetPath, curveColor);
				mCurvesEditor->SetCurveColor(&floatAnimValue->curve, curveColor);
				curveColor = Color4::SomeColor(colorIdx++);
			}
		}
	}

	void CurvesSheet::OnAnimationChanged()
	{
		bool changed = false;

		// Check new curves
		auto currentCurves = mCurvesEditor->GetCurves();
		Vector<Curve*> animCurves;
		for (auto animValue : mAnimationWindow->mAnimation->GetAnimationsValues())
		{
			if (auto floatAnimValue = dynamic_cast<AnimatedValue<float>*>(animValue.animatedValue))
			{
				animCurves.Add(&floatAnimValue->curve);

				if (!currentCurves.ContainsValue(&floatAnimValue->curve))
				{
					mCurvesEditor->AddCurve(animValue.targetPath, &floatAnimValue->curve);
					changed = true;
				}
			}
		}

		// Check removed curves
		for (auto curve : mCurvesEditor->GetCurves())
		{
			if (!animCurves.Contains(curve.second))
			{
				mCurvesEditor->RemoveCurve(curve.second);
				changed = true;
			}
		}

		if (changed)
			UpdateCurvesColors();
	}

	void CurvesSheet::InitializeControls()
	{
		mCurvesEditor = mnew CurveEditor();
		*mCurvesEditor->layout = WidgetLayout::BothStretch();

		auto horScroll = o2UI.CreateHorScrollBar();
		*horScroll->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 10, 10, 10, 0);
		mCurvesEditor->SetHorScrollbar(horScroll);

		auto verScroll = o2UI.CreateVerScrollBar();
		*verScroll->layout = WidgetLayout::VerStretch(HorAlign::Right, 10, 10, 10, 0);
		mCurvesEditor->SetVerScrollbar(verScroll);

		mCurvesEditor->SetTextBorder(BorderF(0, 10, 10, 0));

		mCurvesEditor->SetMainHandleImages(ImageAssetRef("ui/CurveHandle.png"),
										   ImageAssetRef("ui/CurveHandleHover.png"),
										   ImageAssetRef("ui/CurveHandlePressed.png"),
										   ImageAssetRef("ui/CurveHandleSelected.png"));

		mCurvesEditor->SetSupportHandleImages(ImageAssetRef("ui/CurveSupportHandle.png"),
											  ImageAssetRef("ui/CurveSupportHandleHover.png"),
											  ImageAssetRef("ui/CurveSupportHandlePressed.png"),
											  ImageAssetRef("ui/CurveSupportHandleSelected.png"));

		mCurvesEditor->SetSelectionSpriteImage(ImageAssetRef("ui/UI_Window_place.png"));

		mCurvesEditor->verGridEnabled = false;
		mCurvesEditor->onViewChanged += THIS_FUNC(OnEditorViewChanged);

		mCurvesEditor->GetContextMenu()->AddItem("---");
		mCurvesEditor->GetContextMenu()->AddToggleItem("Normalize curves", true, [&](bool val) { 
			mCurvesEditor->SetAdjustCurvesScale(val);
		});

		AddChild(mCurvesEditor);
	}

	void CurvesSheet::SetCameraAsTimelineView()
	{
		Camera camera = mCurvesEditor->GetCamera();
		RectF camRect = camera.GetRect();
		camRect.left = mAnimationWindow->mTimeline->WorldToLocal(mAnimationWindow->mTimeline->layout->worldLeft);
		camRect.right = mAnimationWindow->mTimeline->WorldToLocal(mAnimationWindow->mTimeline->layout->worldRight);

		camera.SetRect(camRect, false);
		RectF checkRect = camera.GetRect();

		mEditorViewLock = true;
		mCurvesEditor->SetCamera(camera);
		mEditorViewLock = false;
	}

	void CurvesSheet::OnEditorViewChanged()
	{
		if (mEditorViewLock || !IsEnabled())
			return;

		mAnimationWindow->mTimeline->SetViewRange(mCurvesEditor->GetCamera().GetRect().left, mCurvesEditor->GetCamera().GetRect().right);
	}

}

DECLARE_CLASS(Editor::CurvesSheet);
