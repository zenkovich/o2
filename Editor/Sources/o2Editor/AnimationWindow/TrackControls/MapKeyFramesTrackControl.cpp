Replace raw pointers with Ref<>, except void* pointers. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Don't replace void* pointers. Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

```
#include "o2Editor/stdafx.h"
#include "MapKeyFramesTrackControl.h"

#include "o2/Scene/ActorTransform.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2Editor/AnimationWindow/KeyHandlesSheet.h"

namespace Editor
{
	MapKeyFramesTrackControl::MapKeyFramesTrackControl() :
		ITrackControl()
	{}

	MapKeyFramesTrackControl::MapKeyFramesTrackControl(const MapKeyFramesTrackControl& other) :
		ITrackControl(other)
	{}

	MapKeyFramesTrackControl::~MapKeyFramesTrackControl()
	{
		for (auto& kv : mHandlesGroups)
			delete kv.second;
	}

	MapKeyFramesTrackControl& MapKeyFramesTrackControl::operator=(const MapKeyFramesTrackControl& other)
	{
		Widget::operator=(other);
		return *this;
	}

#undef DrawText
	void MapKeyFramesTrackControl::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		if (!mHandlesSheet->enabled)
			return;

		OnDrawn();

		o2Render.EnableScissorTest(mTimeline->layout->GetWorldRect());

		for (const auto& child : mChildrenInheritedDepth)
			child->Draw();

		o2Render.DisableScissorTest();

		DrawDebugFrame();
	}

	void MapKeyFramesTrackControl::SetMappedTracks(const AnimationTree::TrackNode& valueNode)
	{
		CacheHandles();
		mTracks.Clear();

		InitializeNodeHandles(valueNode);
	}

	void MapKeyFramesTrackControl::Initialize(const Ref<AnimationTimeline>& timeline, const Ref<KeyHandlesSheet>& handlesSheet)
	{
		mTimeline = timeline;
		mHandlesSheet = handlesSheet;
	}

	void MapKeyFramesTrackControl::CacheHandles()
	{
		for (auto& kv : mHandlesGroups)
		{
			kv.second->CacheHandles();
			delete kv.second;
		}

		mHandlesGroups.Clear();
	}

	void MapKeyFramesTrackControl::InitializeNodeHandles(const AnimationTree::TrackNode& valueNode)
	{
		if (valueNode.track)
		{
			IHandlesGroup* newGroup = nullptr;

			if (auto animatedValue = DynamicCast<AnimationTrack<float>>(valueNode.track))
				newGroup = mmake<HandlesGroup<AnimationTrack<float>>>();
			else if (auto animatedValue = DynamicCast<AnimationTrack<bool>>(valueNode.track))
				newGroup = mmake<HandlesGroup<AnimationTrack<bool>>>();
			else if (auto animatedValue = DynamicCast<AnimationTrack<Vec2F>>(valueNode.track))
				newGroup = mmake<HandlesGroup<AnimationTrack<Vec2F>>>();
			else if (auto animatedValue = DynamicCast<AnimationTrack<Color4>>(valueNode.track))
				newGroup = mmake<HandlesGroup<AnimationTrack<Color4>>>();

			if (newGroup)
			{
				newGroup->trackControl = this;
				newGroup->InitializeHandles(valueNode.track, valueNode.path);

				mHandlesGroups.Add(valueNode.track, newGroup);
			}
		}

		for (const auto& childNode : valueNode.children)
			InitializeNodeHandles(*childNode);
	}

	void MapKeyFramesTrackControl::UpdateHandles()
	{
		if (mDisableHandlesUpdate)
			return;

		for (const auto& kv : mHandlesGroups)
			kv.second->UpdateHandles();
	}

	void MapKeyFramesTrackControl::SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime) const
	{
		for (const auto& kv : mHandlesGroups)
		{
			if (kv.second->SerializeKey(keyUid, data, relativeTime))
				break;
		}
	}

	Vector<ITrackControl::KeyHandle*> MapKeyFramesTrackControl::GetKeyHandles() const
	{
		Vector<ITrackControl::KeyHandle*> res;
		for (const auto& kv : mHandlesGroups) {
			res.Add(kv.second->handles.Cast<ITrackControl::KeyHandle*>());
		}

		return res;
	}

	void MapKeyFramesTrackControl::DeleteKey(UInt64 keyUid)
	{
		for (const auto& kv : mHandlesGroups)
			kv.second->DeleteKey(keyUid);
	}

	void MapKeyFramesTrackControl::UpdateHandlesForTrack(IAnimationTrack* animatedValue)
	{
		if (mHandlesGroups.ContainsKey(animatedValue))
			mHandlesGroups[animatedValue]->UpdateHandles();
	}

	void MapKeyFramesTrackControl::BeginKeysDrag()
	{}

	void MapKeyFramesTrackControl::EndKeysDrag()
	{}

	AnimationKeyDragHandle* MapKeyFramesTrackControl::CreateHandle()
	{
		AnimationKeyDragHandle* handle = mmake<AnimationKeyDragHandle>(mmake<Sprite>("ui/UI4_map_key.png"),
																	 mmake<Sprite>("ui/UI4_map_key_hover.png"),
																	 mmake<Sprite>("ui/UI4_map_key_pressed.png"),
																	 mmake<Sprite>("ui/UI4_selected_map_key.png"),
																	 mmake<Sprite>("ui/UI4_selected_map_key_hover.png"),
																	 mmake<Sprite>("ui/UI4_selected_map_key_pressed.png"));

		handle->cursorType = CursorType::SizeWE;
		handle->pixelPerfect = true;
		handle->SetDrawablesSizePivot(Vec2F(7, 2));

		handle->checkPositionFunc = [&](const Vec2F& pos) {
			float position = pos.x;
			if (position < 0.0f)
				position = 0.0f;

			return Vec2F(position, layout->GetHeight() * 0.5f);
		};

		handle->localToWidgetOffsetTransformFunc = [&](const Vec2F& pos) {
			float worldXPos = mTimeline->LocalToWorld(pos.x);
			float localXPos = worldXPos - layout->GetWorldLeft();

			return Vec2F(localXPos, 0);
		};

		handle->widgetOffsetToLocalTransformFunc = [&](const Vec2F& pos) {
			float worldXPos = layout->GetWorldLeft() + pos.x;
			float localXPos = mTimeline->WorldToLocal(worldXPos);

			return Vec2F(localXPos, 0);
		};

		return handle;
	}

	Vector<MapKeyFramesTrackControl::KeyHandle*> MapKeyFramesTrackControl::FindHandlesAtPosition(float pos)
	{
		Vector<KeyHandle*> res;
		for (const auto& kv : mHandlesGroups)
		{
			Vector<KeyHandle*> handles = kv.second->FindHandlesAtPosition(pos);
			res.Append(handles);
		}

		return res;
	}
}
```#include <memory>
#include <functional>
#include <vector>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
	return std::dynamic_pointer_cast<T>(ptr);
}

namespace Editor
{
	class AnimationKeyDragHandle {};
	class IAnimationTrack {};
	class ITrackControl
	{
	public:
		class KeyHandle
		{
		public:
			KeyHandle();
			KeyHandle(UInt64 keyUid, AnimationKeyDragHandle* handle, IAnimationTrack* track,
					  const std::function<void(KeyHandle& keyHandle)>& updateFunc);

			bool operator==(const KeyHandle& other) const;

			Ref<AnimationKeyDragHandle> handle;
			Ref<IAnimationTrack> track;
			std::function<void(KeyHandle& keyHandle)> updateFunc;
		};

		virtual ~ITrackControl() = default;

		std::vector<Ref<KeyHandle>> FindKeyHandlesAtPosition(float position) const
		{
			std::vector<Ref<KeyHandle>> res;

			for (auto& kv : mHandlesGroups)
			{
				for (const Ref<KeyHandle>& keyHandle : kv.second->handles)
				{
					if (mTimeline->IsSameTime(keyHandle->handle->GetPosition().x, position))
						res.push_back(keyHandle);
				}
			}

			return res;
		}

		class IHandlesGroup
		{
		public:
			virtual ~IHandlesGroup();

			virtual void CacheHandles();

			Ref<ITrackControl> trackControl;
			std::vector<Ref<KeyHandle>> handles;
		};

		std::map<std::string, Ref<IHandlesGroup>> mHandlesGroups;
		std::vector<Ref<AnimationKeyDragHandle>> mHandlesCache;
		Ref<IAnimationTrack> mTimeline;
	};
}

// --- META ---

DECLARE_CLASS(Editor::MapKeyFramesTrackControl, Editor__MapKeyFramesTrackControl);
// --- END META ---