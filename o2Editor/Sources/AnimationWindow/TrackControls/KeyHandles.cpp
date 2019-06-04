#include "stdafx.h"
#include "KeyHandles.h"

namespace Editor
{

	AnimationKeyDragHandle::AnimationKeyDragHandle()
	{}

	AnimationKeyDragHandle::AnimationKeyDragHandle(Sprite* regular, Sprite* hover /*= nullptr*/, Sprite* pressed /*= nullptr*/, Sprite* selected /*= nullptr*/, 
												   Sprite* selectedHovered /*= nullptr*/, Sprite* selectedPressed /*= nullptr*/):
		WidgetDragHandle(regular, hover, pressed, selected, selectedHovered, selectedPressed)
	{}

	AnimationKeyDragHandle::AnimationKeyDragHandle(const AnimationKeyDragHandle& other):
		WidgetDragHandle(other)
	{}

	AnimationKeyDragHandle::~AnimationKeyDragHandle()
	{}

	Editor::AnimationKeyDragHandle& AnimationKeyDragHandle::operator=(const AnimationKeyDragHandle& other)
	{
		WidgetDragHandle::operator=(other);
		return *this;
	}

}

DECLARE_CLASS(Editor::AnimationKeyDragHandle);
