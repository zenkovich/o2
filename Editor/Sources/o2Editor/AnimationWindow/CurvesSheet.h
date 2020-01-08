#pragma once

#include "o2Editor/Core/UI/CurveEditor/CurvesEditor.h"

using namespace o2;

namespace Editor
{
	class AnimationWindow;

	class CurvesSheet : public Widget
	{
	public:
		CurvesSheet();
		CurvesSheet(const CurvesSheet& other);
		~CurvesSheet();

		CurvesSheet& operator=(const CurvesSheet& other);

		// Sets animation and updates tree structure
		void SetAnimation(Animation* animation);

		// Generates new color for curves and transfers to the tree
		void UpdateCurvesColors();

		// It is called when animation changed, checks count of animation values, updates curves
		void OnAnimationChanged();

		SERIALIZABLE(CurvesSheet);

	private:
		CurvesEditor* mCurvesEditor; // Curves editor

		AnimationWindow* mAnimationWindow = nullptr; // Animation window

		bool mEditorViewLock = false; // It is used to prevent handling editor's camera, when timeline view changing 

	private:
		// Initializes curves edit widget
		void InitializeControls();

		// It is called when timeline view has changed, updates horizontal view position and scale
		void SetCameraAsTimelineView();

		// It is called when curve editor view has changed, updates timeline scroll position
		void OnEditorViewChanged();

		friend class AnimationWindow;
	};
}

CLASS_BASES_META(Editor::CurvesSheet)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::CurvesSheet)
{
	PRIVATE_FIELD(mCurvesEditor);
	PRIVATE_FIELD(mAnimationWindow);
	PRIVATE_FIELD(mEditorViewLock);
}
END_META;
CLASS_METHODS_META(Editor::CurvesSheet)
{

	PUBLIC_FUNCTION(void, SetAnimation, Animation*);
	PUBLIC_FUNCTION(void, UpdateCurvesColors);
	PUBLIC_FUNCTION(void, OnAnimationChanged);
	PRIVATE_FUNCTION(void, InitializeControls);
	PRIVATE_FUNCTION(void, SetCameraAsTimelineView);
	PRIVATE_FUNCTION(void, OnEditorViewChanged);
}
END_META;
