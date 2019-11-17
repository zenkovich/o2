#pragma once

#include "Animation/AnimatedValue.h"
#include "Core/Properties/IPropertyField.h"
#include "Scene/UI/Widget.h"
#include "AnimationKeyDragHandle.h"

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

		// Returns a container of controllers that are part of a tree
		virtual Widget* GetTreePartControls() const;

		// Sets curves edit view mode
		virtual void SetCurveViewEnabled(bool enabled);

		// Sets curves view mode color
		virtual void SetCurveViewColor(const Color4& color);

		// Inserts new key at time
		virtual void InsertNewKey(float time);

		// It is called when group of keys began drag
		virtual void BeginKeysDrag();

		// It is called when group of keys completed drag
		virtual void EndKeysDrag();

		// Serialize key with specified uid into data node
		virtual void SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime);

		// Deserialize key from data node and paste on track, returns key uid
		virtual UInt64 DeserializeKey(const DataNode& data, float relativeTime, bool generateNewUid = true);

		// Removes key from track
		virtual void DeleteKey(UInt64 keyUid);

		SERIALIZABLE(ITrackControl);
	};
}

CLASS_BASES_META(Editor::ITrackControl)
{
	BASE_CLASS(Widget);
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
	PUBLIC_FUNCTION(Widget*, GetTreePartControls);
	PUBLIC_FUNCTION(void, SetCurveViewEnabled, bool);
	PUBLIC_FUNCTION(void, SetCurveViewColor, const Color4&);
	PUBLIC_FUNCTION(void, InsertNewKey, float);
	PUBLIC_FUNCTION(void, BeginKeysDrag);
	PUBLIC_FUNCTION(void, EndKeysDrag);
	PUBLIC_FUNCTION(void, SerializeKey, UInt64, DataNode&, float);
	PUBLIC_FUNCTION(UInt64, DeserializeKey, const DataNode&, float, bool);
	PUBLIC_FUNCTION(void, DeleteKey, UInt64);
}
END_META;
