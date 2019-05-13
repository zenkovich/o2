#pragma once

#include "Animation/AnimatedValue.h"
#include "Scene/UI/Widget.h"
#include "Utils/Editor/DragHandle.h"

using namespace o2;

namespace Editor
{
	class AnimationTimeline;
	class KeyHandlesSheet;

	class ITrackControl: public Widget
	{
	public:
		struct KeyHandle
		{
			int keyIdx = 0;
			WidgetDragHandle* handle = nullptr;

			KeyHandle() { }
			KeyHandle(int keyIdx, WidgetDragHandle* handle): keyIdx(keyIdx), handle(handle) { }

			bool operator==(const KeyHandle& other) const;
		};
		typedef Vector<KeyHandle*> KeyHandlesVec;

	public:
		virtual void Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet);
		virtual void SetAnimatedValue(IAnimatedValue* animatedValue);

		virtual void UpdateHandles();

		virtual KeyHandlesVec GetKeyHandles() const;
		virtual float GetKeyPosition(int idx) const;

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

	PUBLIC_FUNCTION(void, Initialize, AnimationTimeline*, KeyHandlesSheet*);
	PUBLIC_FUNCTION(void, SetAnimatedValue, IAnimatedValue*);
	PUBLIC_FUNCTION(void, UpdateHandles);
	PUBLIC_FUNCTION(KeyHandlesVec, GetKeyHandles);
	PUBLIC_FUNCTION(float, GetKeyPosition, int);
}
END_META;
