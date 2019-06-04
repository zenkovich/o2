#pragma once

#include "Utils/Editor/DragHandle.h"

using namespace o2;

namespace Editor
{
	class AnimationKeyDragHandle : public WidgetDragHandle
	{ 
	public:
		IAnimatedValue* animatedValue = nullptr;
		int             keyIdx = 0;
		bool            isMapping = false;

	public:
		// Default constructor
		AnimationKeyDragHandle();

		// Constructor with views
		AnimationKeyDragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr,
						       Sprite* selected = nullptr, Sprite* selectedHovered = nullptr,
						       Sprite* selectedPressed = nullptr);

		// Copy-constructor
		AnimationKeyDragHandle(const AnimationKeyDragHandle& other);

		// Destructor
		~AnimationKeyDragHandle();

		// Copy-operator
		AnimationKeyDragHandle& operator=(const AnimationKeyDragHandle& other);

		SERIALIZABLE(AnimationKeyDragHandle);
	};
}

CLASS_BASES_META(Editor::AnimationKeyDragHandle)
{
	BASE_CLASS(o2::WidgetDragHandle);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationKeyDragHandle)
{
	PUBLIC_FIELD(animatedValue);
	PUBLIC_FIELD(keyIdx);
	PUBLIC_FIELD(isMapping);
}
END_META;
CLASS_METHODS_META(Editor::AnimationKeyDragHandle)
{
}
END_META;
