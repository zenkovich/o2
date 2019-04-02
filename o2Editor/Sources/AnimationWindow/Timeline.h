#pragma once

#include "Scene/UI/Widget.h"

using namespace o2;

namespace Editor
{
	class AnimationTimeline : public Widget
	{
	public:
		SERIALIZABLE(AnimationTimeline);
	};
}

CLASS_BASES_META(Editor::AnimationTimeline)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationTimeline)
{
}
END_META;
CLASS_METHODS_META(Editor::AnimationTimeline)
{
}
END_META;
