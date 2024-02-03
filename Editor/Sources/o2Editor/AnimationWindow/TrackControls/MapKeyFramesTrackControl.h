#pragma once

#include "o2/Utils/Editor/DragHandle.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/AnimationWindow/Timeline.h"
#include "o2Editor/AnimationWindow/TrackControls/ITrackControl.h"
#include "o2Editor/AnimationWindow/TrackControls/AnimationTrackWrapper.h"
#include "o2Editor/AnimationWindow/Tree.h"
#include "o2Editor/AnimationWindow/KeyHandlesSheet.h"
#include <o2/Utils/SmartPointers/Ref.h>
#include <o2/Utils/SmartPointers/WeakRef.h>

using namespace o2;

namespace Editor
{
	// ---------------------------------------------------------------------
	// Animation control track for mapped keys from children Animation tracks
	// ---------------------------------------------------------------------
	class MapKeyFramesTrackControl : public ITrackControl
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
		void Initialize(const Ref<AnimationTimeline>& timeline, const Ref<KeyHandlesSheet>& handlesSheet) override;

		// Draws handles with clipping
		void Draw() override;

		// Updates handles position on timeline
		void UpdateHandles() override;

		// Serialize key with specified uid into data node
		void SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime) override;

		// Returns key handles list
		Vector<Ref<ITrackControl::KeyHandle>> GetKeyHandles() const override;

		// Removes key from track
		void DeleteKey(UInt64 keyUid) override;

		// Sets mapped Animation tracks. Creates handles
		void SetMappedTracks(const AnimationTree::TrackNode& valueNode);

		// Updates handles positions for specified Animation track
		void UpdateHandlesForTrack(const Ref<IAnimationTrack>& track);

		// Called when group of keys began dragged, disables keys combining
		void BeginKeysDrag() override;

		// Called when group of keys completed drag, enables keys combining
		void EndKeysDrag() override;

		SERIALIZABLE(MapKeyFramesTrackControl);

	private:
		struct KeyHandle : public ITrackControl::KeyHandle
		{
			WeakRef<IAnimationTrack> track;

			Function<void(KeyHandle& keyHandle)> updateFunc;

		public:
			KeyHandle();
			KeyHandle(UInt64 keyUid, const Ref<AnimationKeyDragHandle>& handle, const Ref<IAnimationTrack>& track,
			          const Function<void(KeyHandle& keyHandle)>& updateFunc);

			bool operator==(const KeyHandle& other) const;
		};

		struct IHandlesGroup
		{
			String trackPath;
			Vector<Ref<KeyHandle>> handles;

			WeakRef<MapKeyFramesTrackControl> trackControl;

		public:
			virtual ~IHandlesGroup();

			virtual void InitializeHandles(const Ref<IAnimationTrack>& itrack, const String& trackPath) = 0;
			virtual void CreateHandles() = 0;
			virtual void OnHandleChangedPos(const Ref<KeyHandle>& keyHandle, const Vec2F& pos) = 0;
			virtual void UpdateHandles() = 0;
			virtual bool SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime) = 0;
			virtual void DeleteKey(UInt64 keyUid) = 0;
			void CacheHandles();
		};

		template<typename TrackType>
		struct HandlesGroup : public IHandlesGroup
		{
			typedef AnimationTrackWrapper<TrackType> Wrapper;

			WeakRef<TrackType> track;

		public:
			~HandlesGroup();

			void InitializeHandles(const Ref<IAnimationTrack>& itrack, const String& trackPath) override;
			void CreateHandles() override;
			void OnHandleChangedPos(const Ref<KeyHandle>& keyHandle, const Vec2F& pos) override;
			void UpdateHandles() override;
			bool SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime) override;
			void DeleteKey(UInt64 keyUid) override;
		};

	private:
		Map<WeakRef<IAnimationTrack>, Ref<IHandlesGroup>> mHandlesGroups; // List of handles, each for keys
		Vector<WeakRef<IAnimationTrack>>                  mTracks;        // Editing Animation tracks
		WeakRef<AnimationTimeline>                        mTimeline;      // Time line used for calculating handles positions
		WeakRef<KeyHandlesSheet>                          mHandlesSheet;  // Handles sheet, used for drawing and managing drag handles

		Vector<Ref<AnimationKeyDragHandle>> mHandlesCache; // Cached drag handles, can be reused

		bool mDisableHandlesUpdate = false;  // It is true when handles are changing and combining or updating is not available

	private:
		void CacheHandles();
		void InitializeNodeHandles(const AnimationTree::TrackNode& valueNode);

		Ref<AnimationKeyDragHandle> CreateHandle();

		Vector<Ref<KeyHandle>> FindHandlesAtPosition(float position) const;
	};

	template<typename TrackType>
	MapKeyFramesTrackControl::HandlesGroup<TrackType>::~HandlesGroup()
	{
		if (track)
			track->onKeysChanged -= THIS_SUBSCRIPTION(HandlesGroup<TrackType>::UpdateHandles, [&]() {});
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::InitializeHandles(const Ref<IAnimationTrack>& itrack, const String& trackPath)
	{
		trackPath = trackPath;
		trackControl = trackControl.Cast<MapKeyFramesTrackControl>();
		track = itrack.Cast<TrackType>();
		CreateHandles();
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::CreateHandles()
	{
		auto wrapper = Ref<AnimationTrackWrapper<TrackType>>::Create(track);
		for (const auto& key : wrapper->GetAllKeys())
		{
			auto handle = CreateHandle();
			keyHandles.push_back(
			    Ref<KeyHandle>::Create(key->GetUid(), handle, itrack, [&](KeyHandle& keyHandle) {
				    trackControl.Lock()->UpdateHandlesForTrack(track);
			    }));
		}
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::OnHandleChangedPos(const Ref<KeyHandle>& keyHandle, const Vec2F& pos)
	{
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::UpdateHandles()
	{
	}

	template<typename TrackType>
	bool MapKeyFramesTrackControl::HandlesGroup<TrackType>::SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime)
	{
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::DeleteKey(UInt64 keyUid)
	{
	}

	MapKeyFramesTrackControl::MapKeyFramesTrackControl() = default;

	MapKeyFramesTrackControl::MapKeyFramesTrackControl(const MapKeyFramesTrackControl& other) = default;

	MapKeyFramesTrackControl::~MapKeyFramesTrackControl() = default;

	MapKeyFramesTrackControl& MapKeyFramesTrackControl::operator=(const MapKeyFramesTrackControl& other) = default;

	void MapKeyFramesTrackControl::Initialize(const Ref<AnimationTimeline>& timeline, const Ref<KeyHandlesSheet>& handlesSheet)
	{
		mTimeline = timeline;
		mHandlesSheet = handlesSheet;
	}

	void MapKeyFramesTrackControl::Draw()
	{
	}

	void MapKeyFramesTrackControl::UpdateHandles()
	{
	}

	void MapKeyFramesTrackControl::SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime)
	{
	}

	Vector<Ref<ITrackControl::KeyHandle>> MapKeyFramesTrackControl::GetKeyHandles() const
	{
	}

	void MapKeyFramesTrackControl::DeleteKey(UInt64 keyUid)
	{
	}

	void MapKeyFramesTrackControl::SetMappedTracks(const AnimationTree::TrackNode& valueNode)
	{
		InitializeNodeHandles(valueNode);
	}

	void MapKeyFramesTrackControl::UpdateHandlesForTrack(const Ref<IAnimationTrack>& track)
	{
	}

	void MapKeyFramesTrackControl::BeginKeysDrag()
	{
	}

	void MapKeyFramesTrackControl::EndKeysDrag()
	{
	}

	void MapKeyFramesTrackControl::CacheHandles()
	{
	}

	void MapKeyFramesTrackControl::InitializeNodeHandles(const AnimationTree::TrackNode& valueNode)
	{
	}

	Ref<AnimationKeyDragHandle> MapKeyFramesTrackControl::CreateHandle()
	{
	}

	Vector<Ref<KeyHandle>> MapKeyFramesTrackControl::FindHandlesAtPosition(float position) const
	{
	}

	MapKeyFramesTrackControl::KeyHandle::KeyHandle() = default;

	MapKeyFramesTrackControl::KeyHandle::KeyHandle(UInt64 keyUid, const Ref<AnimationKeyDragHandle>& handle, const Ref<IAnimationTrack>& track,
	                                                const Function<void(KeyHandle& keyHandle)>& updateFunc)
	{
	}

	bool MapKeyFramesTrackControl::KeyHandle::operator==(const KeyHandle& other) const
	{
	}

	MapKeyFramesTrackControl::IHandlesGroup::~IHandlesGroup() = default;

	void MapKeyFramesTrackControl::IHandlesGroup::CacheHandles()
	{
	}

	MapKeyFramesTrackControl::HandlesGroup<TrackType>::~HandlesGroup() = default;

	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::InitializeHandles(const Ref<IAnimationTrack>& itrack, const String& trackPath)
	{
		trackPath = trackPath;
		trackControl = trackControl.Cast<MapKeyFramesTrackControl>();
		track = itrack.Cast<TrackType>();
		CreateHandles();
	}

	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::CreateHandles()
	{
		auto wrapper = Ref<AnimationTrackWrapper<TrackType>>::Create(track);
		for (const auto& key : wrapper->GetAllKeys())
		{
			auto handle = CreateHandle();
			keyHandles.push_back(
			    Ref<KeyHandle>::Create(key->GetUid(), handle, itrack, [&](KeyHandle& keyHandle) {
				    trackControl.Lock()->UpdateHandlesForTrack(track);
			    }));
		}
	}

	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::OnHandleChangedPos(const Ref<KeyHandle>& keyHandle, const Vec2F& pos)
	{
	}

	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::UpdateHandles()
	{
	}

	bool MapKeyFramesTrackControl::HandlesGroup<TrackType>::SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime)
	{
	}

	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::DeleteKey(UInt64 keyUid)
	{
	}
}.replace raw pointers with Ref<>, except void* pointers.Ref<> is a smart pointer like shared_ptr. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Dont replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

/nationTrack>& itrack,
																			  const String& trackPath)
	{
		this->trackPath = trackPath;
		track = DynamicCast<TrackType>(itrack);
		track.Lock()->onKeysChanged += THIS_SUBSCRIPTION(HandlesGroup<TrackType>::UpdateHandles,
														 [&]() { track = nullptr; });

		trackControl->mTracks.Add(track);

		CreateHandles();
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::CreateHandles()
	{
		PushEditorScopeOnStack scope;

		for (auto& key : Wrapper::GetKeys(*track)) {
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
			handle->SetSelectionGroup(dynamic_cast<ISelectableDragHandlesGroup*>(trackControl->mHandlesSheet));

			auto updatePosFunc = [=](KeyHandle& keyHandle) {
				keyHandle.handle->SetPosition(Vec2F(Wrapper::FindKey(*track, handle->keyUid).position, 0.0f));
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

		if (Wrapper::GetKeys(*track).Count() != handles.Count()) {
			Vector<UInt64> selectedHandles;
			for (auto keyHandle : handles) {
				if (keyHandle->handle->IsSelected())
					selectedHandles.Add(keyHandle->keyUid);
			}

			CacheHandles();
			CreateHandles();

			for (auto keyHandle : handles)
				keyHandle->handle->SetSelected(selectedHandles.Contains(keyHandle->keyUid));
		}
		else {
			for (auto keyHandle : handles)
				keyHandle->updateFunc(*keyHandle);
		}
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::OnHandleChangedPos(const Ref<KeyHandle>& keyHandle, const Vec2F& pos)
	{
		trackControl->mDisableHandlesUpdate = true;

		int keyIdx = Wrapper::FindKeyIdx(*track, keyHandle->keyUid);
		auto key = Wrapper::GetKeys(*track)[keyIdx];

		key.position = pos.x;
		Wrapper::RemoveKeyAt(*track, keyIdx);
		Wrapper::AddKey(*track, key);

		trackControl->mDisableHandlesUpdate = false;
	}

	template<typename TrackType>
	bool MapKeyFramesTrackControl::HandlesGroup<TrackType>::SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime)
	{
		int idx = Wrapper::FindKeyIdx(*track, keyUid);
		if (idx < 0)
			return false;

		auto key = Wrapper::GetKey(*track, idx);
		key.position -= relativeTime;
		data.Set(key);

		return true;
	}

	template<typename TrackType>
	void MapKeyFramesTrackControl::HandlesGroup<TrackType>::DeleteKey(UInt64 keyUid)
	{
		int idx = Wrapper::FindKeyIdx(*track, keyUid);
		if (idx >= 0)
			Wrapper::RemoveKeyAt(*track, idx);
	}
}
// --- META ---

CLASS_BASES_META(Editor::MapKeyFramesTrackControl)
{
	BASE_CLASS(Editor::ITrackControl);
}
END_META;
CLASS_FIELDS_META(Editor::MapKeyFramesTrackControl)
{
	FIELD().PRIVATE().NAME(mHandlesGroups);
	FIELD().PRIVATE().NAME(mTracks);
	FIELD().PRIVATE().NAME(mTimeline);
	FIELD().PRIVATE().NAME(mHandlesSheet);
	FIELD().PRIVATE().NAME(mHandlesCache);
	FIELD().PRIVATE().NAME(mDisableHandlesUpdate);
}
END_META;
CLASS_METHODS_META(Editor::MapKeyFramesTrackControl)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const MapKeyFramesTrackControl&);
	FUNCTION().PUBLIC().SIGNATURE(void, Initialize, const Ref<AnimationTimeline>&, const Ref<KeyHandlesSheet>&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateHandles);
	FUNCTION().PUBLIC().SIGNATURE(void, SerializeKey, UInt64, DataValue&, float);
	FUNCTION().PUBLIC().SIGNATURE(Vector<ITrackControl::KeyHandle*>, GetKeyHandles);
	FUNCTION().PUBLIC().SIGNATURE(void, DeleteKey, UInt64);
	FUNCTION().PUBLIC().SIGNATURE(void, SetMappedTracks, const Ref<AnimationTree::TrackNode>&);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateHandlesForTrack, const Ref<IAnimationTrack>&);
	FUNCTION().PUBLIC().SIGNATURE(void, BeginKeysDrag);
	FUNCTION().PUBLIC().SIGNATURE(void, EndKeysDrag);
	FUNCTION().PRIVATE().SIGNATURE(void, CacheHandles);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeNodeHandles, const Ref<AnimationTree::TrackNode>&);
	FUNCTION().PRIVATE().SIGNATURE(AnimationKey#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr) {
    return std::dynamic_pointer_cast<T>(ptr);
}

class KeyHandle
{
};

class Vector
{
public:
    Vector<KeyHandle*> FindHandlesAtPosition(float) const;
};

int main()
{
    Vector().FindHandlesAtPosition(0.0f);
    return 0;
}