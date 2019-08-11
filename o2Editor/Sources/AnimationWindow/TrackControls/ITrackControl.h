#pragma once

#include "Animation/AnimatedValue.h"
#include "Core/Properties/IPropertyField.h"
#include "Scene/UI/Widget.h"
#include "KeyHandles.h"

namespace o2
{
	class Button;
}

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
			UInt64 keyUid = 0;
			AnimationKeyDragHandle* handle = nullptr;

			KeyHandle() { }
			KeyHandle(UInt64 keyUid, AnimationKeyDragHandle* handle): keyUid(keyUid), handle(handle) { }

			bool operator==(const KeyHandle& other) const;
		};
		typedef Vector<KeyHandle*> KeyHandlesVec;

	public:
		// Sets timeline for calculating handles positions, and  handles sheet as selecting group for handles
		virtual void Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet);

		// Sets animated value, updates and creates key handles
		virtual void SetAnimatedValue(IAnimatedValue* animatedValue, const String& path);

		// Updates handles position on timeline
		virtual void UpdateHandles();

		// Returns key handles list
		virtual KeyHandlesVec GetKeyHandles() const;

		// Returns value property
		virtual IPropertyField* GetPropertyField() const;

		// Returns add button
		virtual Button* GetAddKeyButton() const;

		// Inserts new key at time
		virtual void InsertNewKey(float time);

		// It is called when group of keys began drag
		virtual void BeginKeysDrag();

		// It is called when group of keys completed drag
		virtual void EndKeysDrag();

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
	PUBLIC_FUNCTION(void, SetAnimatedValue, IAnimatedValue*, const String&);
	PUBLIC_FUNCTION(void, UpdateHandles);
	PUBLIC_FUNCTION(KeyHandlesVec, GetKeyHandles);
	PUBLIC_FUNCTION(IPropertyField*, GetPropertyField);
	PUBLIC_FUNCTION(Button*, GetAddKeyButton);
	PUBLIC_FUNCTION(void, InsertNewKey, float);
	PUBLIC_FUNCTION(void, BeginKeysDrag);
	PUBLIC_FUNCTION(void, EndKeysDrag);
}
END_META;
