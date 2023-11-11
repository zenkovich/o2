#pragma once

#include "AnimationKeyDragHandle.h"
#include "o2/Animation/Tracks/AnimationTrack.h"
#include "o2/Scene/UI/Widget.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

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

	public:
		// Sets timeline for calculating handles positions, and  handles sheet as selecting group for handles
		virtual void Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet);

		// Sets Animation track, updates and creates key handles
		virtual void SetTrack(IAnimationTrack* track, IAnimationTrack::IPlayer* player, const String& path);

		// Updates handles position on timeline
		virtual void UpdateHandles();

		// Returns key handles list
		virtual Vector<KeyHandle*> GetKeyHandles() const;

		// Returns a container of controllers that are part of a tree
		virtual Widget* GetTreePartControls() const;

		// Sets curves edit view mode
		virtual void SetCurveViewEnabled(bool enabled);

		// Sets curves view mode color
		virtual void SetCurveViewColor(const Color4& color);

		// Sets track active when user selects it
		virtual void SetActive(bool active);

		// Inserts new key at time
		virtual void InsertNewKey(float time);

		// Called when group of keys began drag
		virtual void BeginKeysDrag();

		// Called when group of keys completed drag
		virtual void EndKeysDrag();

		// Serialize key with specified uid into data node
		virtual void SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime);

		// Deserialize key from data node and paste on track, returns key uid
		virtual UInt64 DeserializeKey(const DataValue& data, float relativeTime, bool generateNewUid = true);

		// Removes key from track
		virtual void DeleteKey(UInt64 keyUid);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(ITrackControl);
	};
}
// --- META ---

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

    FUNCTION().PUBLIC().SIGNATURE(void, Initialize, AnimationTimeline*, KeyHandlesSheet*);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTrack, IAnimationTrack*, IAnimationTrack::IPlayer*, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateHandles);
    FUNCTION().PUBLIC().SIGNATURE(Vector<KeyHandle*>, GetKeyHandles);
    FUNCTION().PUBLIC().SIGNATURE(Widget*, GetTreePartControls);
    FUNCTION().PUBLIC().SIGNATURE(void, SetCurveViewEnabled, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, SetCurveViewColor, const Color4&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetActive, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, InsertNewKey, float);
    FUNCTION().PUBLIC().SIGNATURE(void, BeginKeysDrag);
    FUNCTION().PUBLIC().SIGNATURE(void, EndKeysDrag);
    FUNCTION().PUBLIC().SIGNATURE(void, SerializeKey, UInt64, DataValue&, float);
    FUNCTION().PUBLIC().SIGNATURE(UInt64, DeserializeKey, const DataValue&, float, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, DeleteKey, UInt64);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
}
END_META;
// --- END META ---
