#pragma once

#include "o2/Utils/Editor/DragHandle.h"
#include "o2Editor/AnimationWindow/Timeline.h"
#include "o2Editor/AnimationWindow/TrackControls/ITrackControl.h"
#include "o2Editor/AnimationWindow/Tree.h"
#include "o2Editor/Core/EditorScope.h"

using namespace o2;

namespace Editor
{
	// ---------------------------------------------------------------------
	// Animation control track for mapped keys from children Animation tracks
	// ---------------------------------------------------------------------
	class MapKeyFramesTrackControl: public ITrackControl
	{
	public:
		// Default constructor
		MapKeyFramesTrackControl();

		// Copy-constructor
		MapKeyFramesTrackControl(const MapKeyFramesTrackControl& other);

		// Destructor
		~MapKeyFramesTrackControl();

		// Copy-operator
		MapKeyFramesTrackControl& operator=(const MapKeyFramesTrackControl& other);

		// Sets timeline for calculating handles positions, and  handles sheet as selecting group for handles
		void Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet) override;

		// Draws handles with clipping
		void Draw() override;

		// Updates handles position on timeline
		void UpdateHandles() override;

		// Serialize key with specified uid into data node
		void SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime) override;

		// Returns key handles list
		Vector<ITrackControl::KeyHandle*> GetKeyHandles() const override;

		// Removes key from track
		void DeleteKey(UInt64 keyUid) override;

		// Sets mapped Animation tracks. Creates handles
		void SetMappedTracks(const AnimationTree::TrackNode& valueNode);

		// Updates handles positions for specified Animation track
		void UpdateHandlesForTrack(IAnimationTrack* track);

		// It is called when group of keys began dragged, disables keys combining
		void BeginKeysDrag();

		// It is called when group of keys completed drag, enables keys combining
		void EndKeysDrag();

		SERIALIZABLE(MapKeyFramesTrackControl);

	private:
		struct KeyHandle: public ITrackControl::KeyHandle
		{
			IAnimationTrack* track = nullptr;

			Function<void(KeyHandle& keyHandle)> updateFunc;

		public:
			KeyHandle();
			KeyHandle(UInt64 keyUid, AnimationKeyDragHandle* handle, IAnimationTrack* track,
					  const Function<void(KeyHandle& keyHandle)>& updateFunc);

			bool operator==(const KeyHandle& other) const;
		};

		struct IHandlesGroup
		{
			String                    trackPath;
			MapKeyFramesTrackControl* trackControl;
			Vector<KeyHandle*>        handles;

		public:
			virtual ~IHandlesGroup();

			virtual void InitializeHandles(IAnimationTrack* itrack, const String& trackPath) = 0;
			virtual void CreateHandles() = 0;
			virtual void OnHandleChangedPos(KeyHandle* keyHandle, const Vec2F& pos) = 0;
			virtual void UpdateHandles() = 0;
			virtual bool SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime) = 0;
			virtual void DeleteKey(UInt64 keyUid) = 0;
			void CacheHandles();
		};

		template<typename TrackType>
		struct HandlesGroup: public IHandlesGroup
		{
			TrackType* track;

		public:
			~HandlesGroup();

			void InitializeHandles(IAnimationTrack* itrack, const String& trackPath) override;
			void CreateHandles() override;
			void OnHandleChangedPos(KeyHandle* keyHandle, const Vec2F& pos) override;
			void UpdateHandles() override;
			bool SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime) override;
			void DeleteKey(UInt64 keyUid) override;
		};

	private:
		Map<IAnimationTrack*, IHandlesGroup*> mHandlesGroups;          // List of handles, each for keys
		Vector<IAnimationTrack*>              mTracks;                 // Editing Animation tracks
		AnimationTimeline*                    mTimeline = nullptr;     // Time line used for calculating handles positions
		KeyHandlesSheet*                      mHandlesSheet = nullptr; // Handles sheet, used for drawing and managing drag handles

		Vector<AnimationKeyDragHandle*> mHandlesCache; // Cached drag handles, can be reused

		bool mDisableHandlesUpdate = false;  // It is true when handles are changing and combining or updating is not available

	private:
		void CacheHandles();
		void InitializeNodeHandles(const AnimationTree::TrackNode& valueNode);

		AnimationKeyDragHandle* CreateHandle();

		Vector<KeyHandle*> FindHandlesAtPosition(float position) const;
	};

	template<typename TrackType>
	MapKeyFramesTrackControl::HandlesGroup<TrackType>::~HandlesGroup()
	{
		if (track)
			track->onKeysChanged -= THIS_SUBSCRIPTION(HandlesGroup<TrackType>::UpdateHandles, [&]() {});
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::InitializeHandles(IAnimationTrack* itrack,
																			  const String& trackPath)
	{
		this->trackPath = trackPath;

		track = dynamic_cast<TrackType*>(itrack);
		track->onKeysChanged += THIS_SUBSCRIPTION(HandlesGroup<TrackType>::UpdateHandles,
												  [&]() { track = nullptr; });

		trackControl->mTracks.Add(track);

		CreateHandles();
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::CreateHandles()
	{
		PushEditorScopeOnStack scope;

		for (auto& key : track->GetKeys())
		{
			AnimationKeyDragHandle* handle = nullptr;

			if (!trackControl->mHandlesCache.IsEmpty())
				handle = trackControl->mHandlesCache.PopBack();
			else
				handle = trackControl->CreateHandle();

			handle->SetEnabled(true);
			handle->SetPosition(Vec2F(key.position, 0.0f));
			handle->track = track;
			handle->trackPath = trackPath;
			handle->trackControl = trackControl;
			handle->keyUid = key.uid;
			handle->isMapping = true;
			handle->SetSelectionGroup(trackControl->mHandlesSheet);

			auto updatePosFunc = [=](KeyHandle& keyHandle) {
				auto& keys = track->GetKeys();
				keyHandle.handle->SetPosition(Vec2F(track->FindKey(handle->keyUid).position, 0.0f));
			};

			KeyHandle* keyHandle = mnew KeyHandle(key.uid, handle, track, updatePosFunc);
			handles.Add(keyHandle);

			handle->onChangedPos = [=](const Vec2F& pos) { OnHandleChangedPos(keyHandle, pos); };
			handle->onPressed = [=]()
			{
				if (!o2Input.IsKeyDown(VK_CONTROL))
					handle->GetSelectionGroup()->DeselectAll();

				trackControl->FindHandlesAtPosition(keyHandle->handle->GetPosition().x)
					.ForEach([](KeyHandle* keyHandle) { keyHandle->handle->SetSelected(true); });
			};
			handle->onReleased = [&]() { UpdateHandles(); };

			trackControl->AddChild(handle);
		}
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::UpdateHandles()
	{
		if (trackControl->mDisableHandlesUpdate)
			return;

		if (track->GetKeys().Count() != handles.Count())
		{
			Vector<UInt64> selectedHandles;
			for (auto keyHandle : handles)
			{
				if (keyHandle->handle->IsSelected())
					selectedHandles.Add(keyHandle->keyUid);
			}

			CacheHandles();
			CreateHandles();

			for (auto keyHandle : handles)
				keyHandle->handle->SetSelected(selectedHandles.Contains(keyHandle->keyUid));
		}
		else
		{
			for (auto keyHandle : handles)
				keyHandle->updateFunc(*keyHandle);
		}
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::OnHandleChangedPos(KeyHandle* keyHandle, const Vec2F& pos)
	{
		trackControl->mDisableHandlesUpdate = true;

		int keyIdx = track->FindKeyIdx(keyHandle->keyUid);
		auto key = track->GetKeys()[keyIdx];

		key.position = pos.x;
		track->RemoveKeyAt(keyIdx);
		auto newIdx = track->AddKey(key);

		trackControl->mDisableHandlesUpdate = false;
	}

	template<typename TrackType>
	bool MapKeyFramesTrackControl::HandlesGroup<TrackType>::SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime)
	{
		int idx = track->FindKeyIdx(keyUid);
		if (idx < 0)
			return false;

		auto key = track->GetKeyAt(idx);
		key.position -= relativeTime;
		data.SetValue(key);

		return true;
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::DeleteKey(UInt64 keyUid)
	{
		int idx = track->FindKeyIdx(keyUid);
		if (idx >= 0)
			track->RemoveKeyAt(idx);
	}
}

CLASS_BASES_META(Editor::MapKeyFramesTrackControl)
{
	BASE_CLASS(Editor::ITrackControl);
}
END_META;
CLASS_FIELDS_META(Editor::MapKeyFramesTrackControl)
{
	PRIVATE_FIELD(mHandlesGroups);
	PRIVATE_FIELD(mTracks);
	PRIVATE_FIELD(mTimeline);
	PRIVATE_FIELD(mHandlesSheet);
	PRIVATE_FIELD(mHandlesCache);
	PRIVATE_FIELD(mDisableHandlesUpdate);
}
END_META;
CLASS_METHODS_META(Editor::MapKeyFramesTrackControl)
{

	PUBLIC_FUNCTION(void, Initialize, AnimationTimeline*, KeyHandlesSheet*);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, UpdateHandles);
	PUBLIC_FUNCTION(void, SerializeKey, UInt64, DataNode&, float);
	PUBLIC_FUNCTION(Vector<ITrackControl::KeyHandle*>, GetKeyHandles);
	PUBLIC_FUNCTION(void, DeleteKey, UInt64);
	PUBLIC_FUNCTION(void, SetMappedTracks, const AnimationTree::TrackNode&);
	PUBLIC_FUNCTION(void, UpdateHandlesForTrack, IAnimationTrack*);
	PUBLIC_FUNCTION(void, BeginKeysDrag);
	PUBLIC_FUNCTION(void, EndKeysDrag);
	PRIVATE_FUNCTION(void, CacheHandles);
	PRIVATE_FUNCTION(void, InitializeNodeHandles, const AnimationTree::TrackNode&);
	PRIVATE_FUNCTION(AnimationKeyDragHandle*, CreateHandle);
	PRIVATE_FUNCTION(Vector<KeyHandle*>, FindHandlesAtPosition, float);
}
END_META;
