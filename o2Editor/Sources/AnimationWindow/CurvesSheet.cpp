#include "stdafx.h"

#include "AnimationWindow/CurvesSheet.h"

#include "Animation/AnimatedFloat.h"
#include "AnimationWindow/AnimationWindow.h"
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

	CurvesSheet& CurvesSheet::operator=(const CurvesSheet& other)
	{
		delete mEditor;

		Widget::operator=(other);
		InitializeControls();

		return *this;
	}

	void CurvesSheet::SetAnimation(Animation* animation)
	{
		mEditor->RemoveAllEditingCurves();

		for (auto animValue : animation->GetAnimationsValues())
		{
			if (auto floatAnimValue = dynamic_cast<AnimatedValue<float>*>(animValue.animatedValue))
				mEditor->AddEditingCurve(animValue.targetPath, &floatAnimValue->curve, Color4(44, 62, 80));
		}
	}

	void CurvesSheet::InitializeControls()
	{
		mEditor = mnew CurveEditor();
		*mEditor->layout = WidgetLayout::BothStretch();

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
		mEditor->actionsListDelegate = &mAnimationWindow->mActionsList;
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
