#pragma once

#include "Core/UI/CurveEdit.h"

using namespace o2;

namespace Editor
{
	class AnimationTree;
	class AnimationTimeline;

	class CurvesSheet : public Widget
	{
	public:
		CurvesSheet();
		CurvesSheet(const CurvesSheet& other);
		~CurvesSheet();

		CurvesSheet& operator=(const CurvesSheet& other);

		// Sets timeline and tree
		void Initialize(AnimationTimeline* timeline, AnimationTree* tree);

		// Sets animation and updates tree structure
		void SetAnimation(Animation* animation);

		SERIALIZABLE(CurvesSheet);

	private:
		CurveEditor* mEditor;

		AnimationTimeline* mTimeline = nullptr; // Timeline pointer, used for calculation world and local timeline positions
		AnimationTree*     mTree = nullptr;     // Animated values tree pointer, used for calculation handles lines numbers

	private:
		void InitializeControls();
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
	PRIVATE_FIELD(mTimeline);
	PRIVATE_FIELD(mTree);
}
END_META;
CLASS_METHODS_META(Editor::CurvesSheet)
{

	PUBLIC_FUNCTION(void, Initialize, AnimationTimeline*, AnimationTree*);
	PUBLIC_FUNCTION(void, SetAnimation, Animation*);
	PRIVATE_FUNCTION(void, InitializeControls);
}
END_META;
