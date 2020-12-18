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
		void SetAnimation(AnimationClip* animation);

		// Generates new color for curves and transfers to the tree
		void UpdateCurvesColors();

		// It is called when animation changed, checks count of animation tracks, updates curves
		void OnAnimationChanged();

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

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
	FIELD().NAME(mCurvesEditor).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAnimationWindow).PRIVATE();
	FIELD().DEFAULT_VALUE(false).NAME(mEditorViewLock).PRIVATE();
}
END_META;
CLASS_METHODS_META(Editor::CurvesSheet)
{

	PUBLIC_FUNCTION(void, SetAnimation, AnimationClip*);
	PUBLIC_FUNCTION(void, UpdateCurvesColors);
	PUBLIC_FUNCTION(void, OnAnimationChanged);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PRIVATE_FUNCTION(void, InitializeControls);
	PRIVATE_FUNCTION(void, SetCameraAsTimelineView);
	PRIVATE_FUNCTION(void, OnEditorViewChanged);
}
END_META;
