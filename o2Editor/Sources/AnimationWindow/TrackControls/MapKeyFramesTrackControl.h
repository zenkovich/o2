#pragma once

#include "../Tree.h"
#include "AnimationWindow/Timeline.h"
#include "Core/EditorScope.h"
#include "ITrackControl.h"
#include "Scene/UI/Widget.h"
#include "Utils/Editor/DragHandle.h"

using namespace o2;

namespace Editor
{
	// ---------------------------------------------------------------------
	// Animation control track for mapped keys from children animated values
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
		void Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet) override;

		// Draws handles with clipping
		void Draw() override;

		// Updates handles position on timeline
		void UpdateHandles() override;

		// Serialize key with specified uid into data node
		void SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime) override;

		// Returns key handles list
		ITrackControl::KeyHandlesVec GetKeyHandles() const override;

		// Removes key from track
		void DeleteKey(UInt64 keyUid) override;

		// Sets mapped animated values. Creates handles
		void SetMappedTracks(const AnimationTree::AnimationValueNode& valueNode);

		// Updates handles positions for specified animated value
		void UpdateHandlesForValue(IAnimatedValue* animatedValue);

		// It is called when group of keys began dragged, disables keys combining
		void BeginKeysDrag();

		// It is called when group of keys completed drag, enables keys combining
		void EndKeysDrag();

		SERIALIZABLE(MapKeyFramesTrackControl);

	private:
		struct KeyHandle: public ITrackControl::KeyHandle
		{
			IAnimatedValue*         animatedValue = nullptr;

			Function<void(KeyHandle& keyHandle)> updateFunc;

		public:
			KeyHandle();
			KeyHandle(UInt64 keyUid, AnimationKeyDragHandle* handle, IAnimatedValue* animatedValue,
					  const Function<void(KeyHandle& keyHandle)>& updateFunc);

			bool operator==(const KeyHandle& other) const;
		};
		typedef Vector<KeyHandle*> KeyHandlesVec;

		struct IHandlesGroup
		{
			String                    animatedValuePath;
			MapKeyFramesTrackControl* trackControl;
			KeyHandlesVec             handles;

		public:
			virtual ~IHandlesGroup();

			virtual void InitializeHandles(IAnimatedValue* ianimatedValue, const String& animatedValuePath) = 0;
			virtual void CreateHandles() = 0;
			virtual void OnHandleChangedPos(KeyHandle* keyHandle, const Vec2F& pos) = 0;
			virtual void UpdateHandles() = 0;
			virtual bool SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime) = 0;
			virtual void DeleteKey(UInt64 keyUid)  = 0;
			void CacheHandles();
		};

		template<typename AnimationValueType>
		struct HandlesGroup : public IHandlesGroup
		{
			AnimationValueType* animatedValue;

		public:
			~HandlesGroup();

			void InitializeHandles(IAnimatedValue* ianimatedValue, const String& animatedValuePath) override;
			void CreateHandles() override;
			void OnHandleChangedPos(KeyHandle* keyHandle, const Vec2F& pos) override;
			void UpdateHandles() override;
			bool SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime) override;
			void DeleteKey(UInt64 keyUid) override;
		};

		typedef Dictionary<IAnimatedValue*, IHandlesGroup*> AnimatedValueKeyHandlesDict;

	private:
		AnimatedValueKeyHandlesDict mHandlesGroups;          // List of handles, each for keys
		Vector<IAnimatedValue*>     mAnimatedValues;         // Editing animated values
		AnimationTimeline*          mTimeline = nullptr;     // Timeline used for calculating handles positions
		KeyHandlesSheet*            mHandlesSheet = nullptr; // Handles sheet, used for drawing and managing drag handles

		Vector<AnimationKeyDragHandle*> mHandlesCache; // Cached drag handles, can be reused

		bool mDisableHandlesUpdate = false;  // It is true when handles are changing and combining or updating is not available

	private:
		void CacheHandles();
		void InitializeNodeHandles(const AnimationTree::AnimationValueNode& valueNode);

		AnimationKeyDragHandle* CreateHandle();

		Vector<KeyHandle*> FindHandlesAtPosition(float position) const;
	};

	template<typename AnimationValueType>
	MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::~HandlesGroup()
	{
		if (animatedValue)
			animatedValue->onKeysChanged -= THIS_SUBSCRIPTION(HandlesGroup<AnimationValueType>::UpdateHandles, [&]() {});
	}

	template<typename AnimationValueType>
	void MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::InitializeHandles(IAnimatedValue* ianimatedValue, 
																					   const String& animatedValuePath)
	{
		this->animatedValuePath = animatedValuePath;

		animatedValue = dynamic_cast<AnimationValueType*>(ianimatedValue);
		animatedValue->onKeysChanged += THIS_SUBSCRIPTION(HandlesGroup<AnimationValueType>::UpdateHandles,
														  [&]() { animatedValue = nullptr; });

		trackControl->mAnimatedValues.Add(animatedValue);

		CreateHandles();
	}

	template<typename AnimationValueType>
	void MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::CreateHandles()
	{
		PushScopeEnterOnStack scope;

		for (auto& key : animatedValue->GetKeys())
		{
			AnimationKeyDragHandle* handle = nullptr;

			if (!trackControl->mHandlesCache.IsEmpty())
				handle = trackControl->mHandlesCache.PopBack();
			else
				handle = trackControl->CreateHandle();

			handle->SetEnabled(true);
			handle->SetPosition(Vec2F(key.position, 0.0f));
			handle->animatedValue = animatedValue;
			handle->animatedValuePath = animatedValuePath;
			handle->trackControl = trackControl;
			handle->keyUid = key.uid;
			handle->isMapping = true;
			handle->SetSelectionGroup(trackControl->mHandlesSheet);

			auto updatePosFunc = [=](KeyHandle& keyHandle) {
				auto& keys = animatedValue->GetKeys();
				keyHandle.handle->SetPosition(Vec2F(animatedValue->FindKey(handle->keyUid).position, 0.0f));
			};

			KeyHandle* keyHandle = mnew KeyHandle(key.uid, handle, animatedValue, updatePosFunc);
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

	template<typename AnimationValueType>
	void MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::UpdateHandles()
	{
		if (trackControl->mDisableHandlesUpdate)
			return;

		if (animatedValue->GetKeys().Count() != handles.Count())
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

	template<typename AnimationValueType>
	void MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::OnHandleChangedPos(KeyHandle* keyHandle, const Vec2F& pos)
	{
		trackControl->mDisableHandlesUpdate = true;

		int keyIdx = animatedValue->FindKeyIdx(keyHandle->keyUid);
		auto key = animatedValue->GetKeys()[keyIdx];

		key.position = pos.x;
		animatedValue->RemoveKeyAt(keyIdx);
		auto newIdx = animatedValue->AddKey(key);

		trackControl->mDisableHandlesUpdate = false;
	}

	template<typename AnimationValueType>
	bool MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime)
	{
		int idx = animatedValue->FindKeyIdx(keyUid);
		if (idx < 0)
			return false;

		auto key = animatedValue->GetKeyAt(idx);
		key.position -= relativeTime;
		data.SetValue(key);

		return true;
	}

	template<typename AnimationValueType>
	void MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::DeleteKey(UInt64 keyUid)
	{
		int idx = animatedValue->FindKeyIdx(keyUid);
		if (idx >= 0)
			animatedValue->RemoveKeyAt(idx);
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
	PRIVATE_FIELD(mAnimatedValues);
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
	PUBLIC_FUNCTION(ITrackControl::KeyHandlesVec, GetKeyHandles);
	PUBLIC_FUNCTION(void, DeleteKey, UInt64);
	PUBLIC_FUNCTION(void, SetMappedTracks, const AnimationTree::AnimationValueNode&);
	PUBLIC_FUNCTION(void, UpdateHandlesForValue, IAnimatedValue*);
	PUBLIC_FUNCTION(void, BeginKeysDrag);
	PUBLIC_FUNCTION(void, EndKeysDrag);
	PRIVATE_FUNCTION(void, CacheHandles);
	PRIVATE_FUNCTION(void, InitializeNodeHandles, const AnimationTree::AnimationValueNode&);
	PRIVATE_FUNCTION(AnimationKeyDragHandle*, CreateHandle);
	PRIVATE_FUNCTION(Vector<KeyHandle*>, FindHandlesAtPosition, float);
}
END_META;
