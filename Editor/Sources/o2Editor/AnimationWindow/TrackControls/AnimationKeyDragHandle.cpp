#include "o2Editor/stdafx.h"

#include "AnimationKeyDragHandle.h"
#include "ITrackControl.h"

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

#undef DrawText
	void AnimationKeyDragHandle::Draw()
	{
		WidgetDragHandle::Draw();

		//o2Debug.DrawText(GetScreenPosition(), String(mIsClipped));
	}

	String AnimationKeyDragHandle::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	Editor::AnimationKeyDragHandle& AnimationKeyDragHandle::operator=(const AnimationKeyDragHandle& other)
	{
		WidgetDragHandle::operator=(other);
		return *this;
	}

}
// --- META ---

DECLARE_CLASS(Editor::AnimationKeyDragHandle, Editor__AnimationKeyDragHandle);
// --- END META ---
