#include "stdafx.h"
#include "MapKeyFramesTrackControl.h"

#include "AnimationWindow/KeyHandlesSheet.h"
#include "Scene/ActorTransform.h"
#include "Scene/UI/WidgetLayout.h"

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
		for (auto kv : mHandlesGroups)
			delete kv.Value();
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

		OnDrawn();

		o2Render.EnableScissorTest(mTimeline->layout->GetWorldRect());

		for (auto child : mDrawingChildren)
			child->Draw();

		o2Render.DisableScissorTest();

		DrawDebugFrame();
	}

	void MapKeyFramesTrackControl::SetMappedTracks(const AnimationTree::AnimationValueNode& valueNode)
	{
		CacheHandles();
		mAnimatedValues.Clear();

		InitializeNodeHandles(valueNode);
	}

	void MapKeyFramesTrackControl::Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet)
	{
		mTimeline = timeline;
		mHandlesSheet = handlesSheet;
	}

	void MapKeyFramesTrackControl::CacheHandles()
	{
		for (auto kv : mHandlesGroups)
		{
			kv.Value()->CacheHandles();
			delete kv.Value();
		}

		mHandlesGroups.Clear();
	}

	void MapKeyFramesTrackControl::InitializeNodeHandles(const AnimationTree::AnimationValueNode& valueNode)
	{
		if (valueNode.animatedValue)
		{
			IHandlesGroup* newGroup = nullptr;

			if (auto animatedValue = dynamic_cast<AnimatedValue<float>*>(valueNode.animatedValue))
				newGroup = mnew HandlesGroup<AnimatedValue<float>>();
			else if (auto animatedValue = dynamic_cast<AnimatedValue<bool>*>(valueNode.animatedValue))
				newGroup = mnew HandlesGroup<AnimatedValue<bool>>();
			else if (auto animatedValue = dynamic_cast<AnimatedValue<Vec2F>*>(valueNode.animatedValue))
				newGroup = mnew HandlesGroup<AnimatedValue<Vec2F>>();
			else if (auto animatedValue = dynamic_cast<AnimatedValue<Color4>*>(valueNode.animatedValue))
				newGroup = mnew HandlesGroup<AnimatedValue<Color4>>();

			if (newGroup)
			{
				newGroup->trackControl = this;
				newGroup->InitializeHandles(valueNode.animatedValue, valueNode.path);

				mHandlesGroups.Add(valueNode.animatedValue, newGroup);
			}
		}

		for (auto childNode : valueNode.children)
			InitializeNodeHandles(*childNode);
	}

	void MapKeyFramesTrackControl::UpdateHandles()
	{
		if (mDisableHandlesUpdate)
			return;

		for (auto kv : mHandlesGroups)
			kv.Value()->UpdateHandles();
	}

	void MapKeyFramesTrackControl::SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime)
	{
		for (auto kv : mHandlesGroups)
		{
			if (kv.Value()->SerializeKey(keyUid, data, relativeTime))
				break;
		}
	}

	void MapKeyFramesTrackControl::DeleteKey(UInt64 keyUid)
	{
		for (auto kv : mHandlesGroups)
			kv.Value()->DeleteKey(keyUid);
	}

	void MapKeyFramesTrackControl::UpdateHandlesForValue(IAnimatedValue* animatedValue)
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
		AnimationKeyDragHandle* handle = mnew AnimationKeyDragHandle(mnew Sprite("ui/UI4_map_key.png"),
																	 mnew Sprite("ui/UI4_map_key_hover.png"),
																	 mnew Sprite("ui/UI4_map_key_pressed.png"),
																	 mnew Sprite("ui/UI4_selected_map_key.png"),
																	 mnew Sprite("ui/UI4_selected_map_key_hover.png"),
																	 mnew Sprite("ui/UI4_selected_map_key_pressed.png"));

		handle->cursorType = CursorType::SizeWE;
		handle->pixelPerfect = true;
		handle->SetSpritesSizePivot(Vec2F(7, 2));

		handle->checkPositionFunc = [&](const Vec2F& pos) {
			float position = pos.x;
			if (position < 0.0f)
				position = 0.0f;

			return Vec2F(position, layout->GetHeight()*0.5f);
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

	Vector<MapKeyFramesTrackControl::KeyHandle*> MapKeyFramesTrackControl::FindHandlesAtPosition(float position) const
	{
		Vector<KeyHandle*> res;

		for (auto kv : mHandlesGroups)
		{
			for (auto keyHandle : kv.Value()->handles)
			{
				if (mTimeline->IsSameTime(keyHandle->handle->GetPosition().x, position))
					res.Add(keyHandle);
			}
		}

		return res;
	}

	MapKeyFramesTrackControl::KeyHandle::KeyHandle()
	{}

	MapKeyFramesTrackControl::KeyHandle::KeyHandle(UInt64 keyUid, AnimationKeyDragHandle* handle, IAnimatedValue* animatedValue,
												   const Function<void(KeyHandle& keyHandle)>& updateFunc) :
		keyUid(keyUid), handle(handle), animatedValue(animatedValue), updateFunc(updateFunc)
	{}

	bool MapKeyFramesTrackControl::KeyHandle::operator==(const KeyHandle& other) const
	{
		return handle == other.handle;
	}

	MapKeyFramesTrackControl::IHandlesGroup::~IHandlesGroup()
	{
		for (auto handle : handles)
			delete handle;
	}

	void MapKeyFramesTrackControl::IHandlesGroup::CacheHandles()
	{
		for (auto keyHandle : handles)
		{
			keyHandle->handle->SetParent(nullptr);
			keyHandle->handle->SetEnabled(false);
			keyHandle->handle->SetSelected(false);
			keyHandle->handle->SetSelectionGroup(nullptr);
			trackControl->mHandlesCache.Add(keyHandle->handle);
			delete keyHandle;
		}

		handles.clear();
	}

}

DECLARE_CLASS(Editor::MapKeyFramesTrackControl);
