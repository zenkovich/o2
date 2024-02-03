#include "o2Editor/stdafx.h"

#include "AnimationKeyDragHandle.h"
#include "ITrackControl.h"

namespace Editor
{

    AnimationKeyDragHandle::AnimationKeyDragHandle()
    {}

    AnimationKeyDragHandle::AnimationKeyDragHandle(const Ref<Sprite>& regular, const Ref<Sprite>& hover/*= nullptr*/,
                                                   const Ref<Sprite>& pressed/*= nullptr*/,
                                                   const Ref<Sprite>& selected/*= nullptr*/,
                                                   const Ref<Sprite>& selectedHovered/*= nullptr*/,
                                                   const Ref<Sprite>& selectedPressed/*= nullptr*/) :
        WidgetDragHandle(regular, hover, pressed, selected, selectedHovered, selectedPressed)
    {}

    AnimationKeyDragHandle::AnimationKeyDragHandle(const AnimationKeyDragHandle& other) :
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

    String AnimationKeyDragHandle::GetCreateMenuCategory() const
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