#include "stdafx.h"
#include "KeyFramesTrackControl.h"


template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimatedValue<float>>);

template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimatedValue<bool>>);

template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimatedValue<Color4>>);

template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimatedValue<Vec2F>>);