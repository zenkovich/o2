#pragma once

#include "../Tree.h"
#include "AnimationWindow/Timeline.h"
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


		// Draws handles with scissor rect
		void Draw() override;


		// Sets mapped animated values. Creates handles
		void SetMappedTracks(const AnimationTree::AnimationValueNode& valueNode);


		// Updates handles position on timeline
		void UpdateHandles() override;


		// Sets timeline for calculating handles positions
		void SetTimeline(AnimationTimeline* timeline) override;

		SERIALIZABLE(MapKeyFramesTrackControl);

	private:
		struct KeyHandle
		{
			int keyIdx = 0;
			WidgetDragHandle* handle = nullptr;
			IAnimatedValue* animatedValue = nullptr;

			Function<void(KeyHandle& keyHandle)> updateFunc;

			bool operator==(const KeyHandle& other) const { return handle == other.handle; }
		};
		typedef Vector<KeyHandle> KeyHandlesVec;

	private:
		KeyHandlesVec           mHandles;            // List of handles, each for keys
		Vector<IAnimatedValue*> mAnimatedValues;     // Editing animated values
		AnimationTimeline*      mTimeline = nullptr; // Timeline used for calculating handles positions
													 
		Vector<WidgetDragHandle*> mHandlesCache;     // Cached drag handles, can be reused

	private:
		void CacheHandles();
		void InitializeNodeHandles(const AnimationTree::AnimationValueNode& valueNode);

		WidgetDragHandle* CreateHandle();

		template<typename _animatedValueType>
		void InitializeAnimatedValueHandles(_animatedValueType* animatedValue);
	};

	template<typename _animatedValueType>
	void MapKeyFramesTrackControl::InitializeAnimatedValueHandles(_animatedValueType* animatedValue)
	{
		animatedValue->onKeysChanged += THIS_FUNC(UpdateHandles);

		mAnimatedValues.Add(animatedValue);

		int idx = 0;
		for (auto& key : animatedValue->GetKeys())
		{
			WidgetDragHandle* handle = nullptr;

			if (!mHandlesCache.IsEmpty())
				handle = mHandlesCache.PopBack();
			else
				handle = CreateHandle();

			handle->SetPosition(Vec2F(key.position, 0.0f));

			mHandles.Add({ idx++, handle, animatedValue, [=](KeyHandle& keyHandle) { 
				auto& keys = animatedValue->GetKeys();
				if (keyHandle.keyIdx < keys.Count())
					keyHandle.handle->SetPosition(Vec2F(keys[keyHandle.keyIdx].position, 0.0f));
			} });

			AddChild(handle);
		}
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
	PRIVATE_FIELD(mHandlesCache);
}
END_META;
CLASS_METHODS_META(Editor::MapKeyFramesTrackControl)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetMappedTracks, const AnimationTree::AnimationValueNode&);
	PUBLIC_FUNCTION(void, UpdateHandles);
	PUBLIC_FUNCTION(void, SetTimeline, AnimationTimeline*);
	PRIVATE_FUNCTION(void, CacheHandles);
	PRIVATE_FUNCTION(void, InitializeNodeHandles, const AnimationTree::AnimationValueNode&);
	PRIVATE_FUNCTION(WidgetDragHandle*, CreateHandle);
}
END_META;
