#pragma once

#include "Core/UI/CurveEditor/CurveEditor.h"

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

		SERIALIZABLE(CurvesSheet);

	private:
		CurveEditor* mEditor; // Curves editor

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
	PRIVATE_FIELD(mEditor);
	PRIVATE_FIELD(mAnimationWindow);
	PRIVATE_FIELD(mEditorViewLock);
}
END_META;
CLASS_METHODS_META(Editor::CurvesSheet)
{

	PUBLIC_FUNCTION(void, SetAnimation, Animation*);
	PRIVATE_FUNCTION(void, InitializeControls);
	PRIVATE_FUNCTION(void, SetCameraAsTimelineView);
	PRIVATE_FUNCTION(void, OnEditorViewChanged);
}
END_META;
