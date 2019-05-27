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

		// Sets mapped animated values. Creates handles
		void SetMappedTracks(const AnimationTree::AnimationValueNode& valueNode);

		// Updates handles position on timeline
		void UpdateHandles() override;

		// Updates handles positions for specified animated value
		void UpdateHandlesForValue(IAnimatedValue* animatedValue);

		SERIALIZABLE(MapKeyFramesTrackControl);

	private:
		struct KeyHandle
		{
			int               keyIdx = 0;
			WidgetDragHandle* handle = nullptr;
			IAnimatedValue*   animatedValue = nullptr;

			Vector<KeyHandle*> combinedHandles;
			bool               combining = true;

			Function<void(KeyHandle& keyHandle)> updateFunc;

		public:
			KeyHandle();
			KeyHandle(int keyIdx, WidgetDragHandle* handle, IAnimatedValue* animatedValue, 
					  const Function<void(KeyHandle& keyHandle)>& updateFunc);

			bool operator==(const KeyHandle& other) const;
		};
		typedef Vector<KeyHandle*> KeyHandlesVec;

		struct IHandlesGroup
		{
			MapKeyFramesTrackControl* trackControl;
			KeyHandlesVec             handles;

		public:
			virtual ~IHandlesGroup();

			virtual void InitializeHandles(IAnimatedValue* ianimatedValue) = 0;
			virtual void CreateHandles() = 0;
			virtual void ChangeHandleIndex(int oldIndex, int newIndex) = 0;
			virtual void OnHandleChangedPos(KeyHandle* keyHandle, const Vec2F& pos) = 0;
			virtual void UpdateHandles() = 0;
			void CacheHandles();
		};

		template<typename AnimationValueType>
		struct HandlesGroup: public IHandlesGroup
		{
			AnimationValueType* animatedValue;

		public:
			~HandlesGroup();

			void InitializeHandles(IAnimatedValue* ianimatedValue) override;
			void CreateHandles() override;
			void ChangeHandleIndex(int oldIndex, int newIndex) override;
			void OnHandleChangedPos(KeyHandle* keyHandle, const Vec2F& pos) override;
			void UpdateHandles() override;
		};

		typedef Dictionary<IAnimatedValue*, IHandlesGroup*> AnimatedValueKeyHandlesDict;

	private:
		AnimatedValueKeyHandlesDict mHandles;                // List of handles, each for keys
		Vector<IAnimatedValue*>     mAnimatedValues;         // Editing animated values
		AnimationTimeline*          mTimeline = nullptr;     // Timeline used for calculating handles positions
		KeyHandlesSheet*            mHandlesSheet = nullptr; // Handles sheet, used for drawing and managing drag handles
													 
		Vector<WidgetDragHandle*> mHandlesCache; // Cached drag handles, can be reused

		bool mDisableHandlesUpdate = false; // It is true when handles are changing and combining or updating is not available

	private:
		void CacheHandles();
		void InitializeNodeHandles(const AnimationTree::AnimationValueNode& valueNode);

		WidgetDragHandle* CreateHandle();

		void UpdateHandlesCombine();
		Vector<KeyHandle*> FindHandlesAtPosition(float position) const;
	};

	template<typename AnimationValueType>
	MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::~HandlesGroup()
	{
		animatedValue->onKeysChanged -= THIS_FUNC(HandlesGroup<AnimationValueType>::UpdateHandles);
	}

	template<typename AnimationValueType>
	void MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::InitializeHandles(IAnimatedValue* ianimatedValue)
	{
		animatedValue = dynamic_cast<AnimationValueType*>(ianimatedValue);
		animatedValue->onKeysChanged += THIS_FUNC(HandlesGroup<AnimationValueType>::UpdateHandles);

		trackControl->mAnimatedValues.Add(animatedValue);

		CreateHandles();
	}

	template<typename AnimationValueType>
	void MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::CreateHandles()
	{
		PushScopeEnterOnStack scope;

		int idx = 0;
		for (auto& key : animatedValue->GetKeys()) {
			WidgetDragHandle* handle = nullptr;

			if (!trackControl->mHandlesCache.IsEmpty())
				handle = trackControl->mHandlesCache.PopBack();
			else
				handle = trackControl->CreateHandle();

			handle->SetEnabled(true);
			handle->SetPosition(Vec2F(key.position, 0.0f));

			auto updatePosFunc = [=](KeyHandle& keyHandle) {
				auto& keys = animatedValue->GetKeys();
				if (keyHandle.keyIdx < keys.Count())
					keyHandle.handle->SetPosition(Vec2F(keys[keyHandle.keyIdx].position, 0.0f));
			};

			KeyHandle* keyHandle = mnew KeyHandle(idx++, handle, animatedValue, updatePosFunc);
			handles.Add(keyHandle);

			handle->onChangedPos = [=](const Vec2F& pos) { OnHandleChangedPos(keyHandle, pos); };
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
			CacheHandles();
			CreateHandles();
		}
		else {
			for (auto keyHandle : handles)
				keyHandle->updateFunc(*keyHandle);
		}

		trackControl->UpdateHandlesCombine();
	}

	template<typename AnimationValueType>
	void MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::OnHandleChangedPos(KeyHandle* keyHandle, const Vec2F& pos)
	{
		trackControl->mDisableHandlesUpdate = true;

		auto key = animatedValue->GetKeys()[keyHandle->keyIdx];

		key.position = pos.x;
		animatedValue->RemoveKeyAt(keyHandle->keyIdx);
		auto newIdx = animatedValue->AddKey(key);

		if (newIdx != keyHandle->keyIdx) {
			ChangeHandleIndex(keyHandle->keyIdx, newIdx);
			keyHandle->keyIdx = newIdx;
		}

		if (keyHandle->combining) {
			for (auto attachedHandle : keyHandle->combinedHandles) {
				attachedHandle->combining = false;

				attachedHandle->handle->SetPosition(pos);
				attachedHandle->handle->onChangedPos(pos);

				attachedHandle->combining = true;
			}
		}

		trackControl->mDisableHandlesUpdate = false;
	}

	template<typename AnimationValueType>
	void MapKeyFramesTrackControl::HandlesGroup<AnimationValueType>::ChangeHandleIndex(int oldIndex, int newIndex)
	{
		IAnimatedValue* animatedValueBasic = animatedValue;
		KeyHandle* editingHandle = handles[oldIndex];
		handles.RemoveAt(oldIndex);
		handles.Insert(editingHandle, newIndex);

		for (int i = 0; i < handles.Count(); i++)
			handles[i]->keyIdx = i;
	}
}

CLASS_BASES_META(Editor::MapKeyFramesTrackControl)
{
	BASE_CLASS(Editor::ITrackControl);
}
END_META;
CLASS_FIELDS_META(Editor::MapKeyFramesTrackControl)
{
	PRIVATE_FIELD(mHandles);
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
	PUBLIC_FUNCTION(void, SetMappedTracks, const AnimationTree::AnimationValueNode&);
	PUBLIC_FUNCTION(void, UpdateHandles);
	PUBLIC_FUNCTION(void, UpdateHandlesForValue, IAnimatedValue*);
	PRIVATE_FUNCTION(void, CacheHandles);
	PRIVATE_FUNCTION(void, InitializeNodeHandles, const AnimationTree::AnimationValueNode&);
	PRIVATE_FUNCTION(WidgetDragHandle*, CreateHandle);
	PRIVATE_FUNCTION(void, UpdateHandlesCombine);
	PRIVATE_FUNCTION(Vector<KeyHandle*>, FindHandlesAtPosition, float);
}
END_META;
