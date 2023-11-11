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

		// Called when animation changed, checks count of animation tracks, updates curves
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

		// Called when timeline view has changed, updates horizontal view position and scale
		void SetCameraAsTimelineView();

		// Called when curve editor view has changed, updates timeline scroll position
		void OnEditorViewChanged();

		friend class AnimationWindow;
	};
}
// --- META ---

CLASS_BASES_META(Editor::CurvesSheet)
{
    BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::CurvesSheet)
{
    FIELD().PRIVATE().NAME(mCurvesEditor);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mAnimationWindow);
    FIELD().PRIVATE().DEFAULT_VALUE(false).NAME(mEditorViewLock);
}
END_META;
CLASS_METHODS_META(Editor::CurvesSheet)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const CurvesSheet&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetAnimation, AnimationClip*);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateCurvesColors);
    FUNCTION().PUBLIC().SIGNATURE(void, OnAnimationChanged);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeControls);
    FUNCTION().PRIVATE().SIGNATURE(void, SetCameraAsTimelineView);
    FUNCTION().PRIVATE().SIGNATURE(void, OnEditorViewChanged);
}
END_META;
// --- END META ---
