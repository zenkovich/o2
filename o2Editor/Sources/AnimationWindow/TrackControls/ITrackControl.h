#pragma once

#include "Animation/AnimatedValue.h"
#include "AnimationWindow/Timeline.h"
#include "Scene/UI/Widget.h"

namespace Editor
{
	class ITrackControl: public Widget
	{
	public:
		virtual void SetAnimatedValue(IAnimatedValue* animatedValue) {}
		virtual void SetTimeline(AnimationTimeline* timeline) {}
		virtual void UpdateView() {}

		SERIALIZABLE(ITrackControl);
	};
}

CLASS_BASES_META(Editor::ITrackControl)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::ITrackControl)
{
}
END_META;
CLASS_METHODS_META(Editor::ITrackControl)
{

	PUBLIC_FUNCTION(void, SetAnimatedValue, IAnimatedValue*);
	PUBLIC_FUNCTION(void, SetTimeline, AnimationTimeline*);
	PUBLIC_FUNCTION(void, UpdateView);
}
END_META;
