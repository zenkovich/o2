#include "o2/stdafx.h"
#include "AnimationTrack.h"

#include "o2/Scene/Components/AnimationComponent.h"
#include "o2/Utils/Math/Color.h"

template<>
DECLARE_CLASS_MANUAL(o2::AnimationTrack<bool>);

template<>
DECLARE_CLASS_MANUAL(o2::AnimationTrack<bool>::Key);

template<>
DECLARE_CLASS_MANUAL(o2::AnimationTrack<bool>::Player);

template<>
DECLARE_CLASS_MANUAL(o2::AnimationTrack<int>);

template<>
DECLARE_CLASS_MANUAL(o2::AnimationTrack<int>::Key);

template<>
DECLARE_CLASS_MANUAL(o2::AnimationTrack<int>::Player);

template<>
DECLARE_CLASS_MANUAL(o2::AnimationTrack<o2::Color4>);

template<>
DECLARE_CLASS_MANUAL(o2::AnimationTrack<o2::Color4>::Key);

template<>
DECLARE_CLASS_MANUAL(o2::AnimationTrack<o2::Color4>::Player);
