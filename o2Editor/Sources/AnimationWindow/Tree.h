#pragma once

#include "Scene/UI/Widgets/Tree.h"

using namespace o2;

namespace Editor
{
	class AnimationTree : public o2::Tree
	{
	public:
		SERIALIZABLE(AnimationTree);
	};
}

CLASS_BASES_META(Editor::AnimationTree)
{
	BASE_CLASS(o2::Tree);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationTree)
{
}
END_META;
CLASS_METHODS_META(Editor::AnimationTree)
{
}
END_META;
