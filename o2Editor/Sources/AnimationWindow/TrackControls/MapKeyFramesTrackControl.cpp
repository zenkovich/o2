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
		for (auto kv : mHandles)
		{
			for (auto keyHandle : kv.Value())
				delete keyHandle;
		}
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

		for (auto animValue : mAnimatedValues)
			animValue->onKeysChanged -= THIS_FUNC(UpdateHandles);

		mAnimatedValues.Clear();

		InitializeNodeHandles(valueNode);
		UpdateHandlesCombine();
	}

	void MapKeyFramesTrackControl::Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet)
	{
		mTimeline = timeline;
		mHandlesSheet = handlesSheet;
	}

	void MapKeyFramesTrackControl::CacheHandles()
	{
		for (auto kv : mHandles)
		{
			for (auto keyHandle : kv.Value())
			{
				keyHandle->handle->SetParent(nullptr);
				keyHandle->handle->SetEnabled(false);
				mHandlesCache.Add(keyHandle->handle);
				delete keyHandle;
			}
		}

		mHandles.Clear();
	}

	void MapKeyFramesTrackControl::InitializeNodeHandles(const AnimationTree::AnimationValueNode& valueNode)
	{
		if (valueNode.animatedValue)
		{
			if (auto animatedValue = dynamic_cast<AnimatedValue<float>*>(valueNode.animatedValue))
				InitializeAnimatedValueHandles(animatedValue);
			else if (auto animatedValue = dynamic_cast<AnimatedValue<bool>*>(valueNode.animatedValue))
				InitializeAnimatedValueHandles(animatedValue);
			else if (auto animatedValue = dynamic_cast<AnimatedValue<Vec2F>*>(valueNode.animatedValue))
				InitializeAnimatedValueHandles(animatedValue);
			else if (auto animatedValue = dynamic_cast<AnimatedValue<Color4>*>(valueNode.animatedValue))
				InitializeAnimatedValueHandles(animatedValue);
		}

		for (auto childNode : valueNode.children)
			InitializeNodeHandles(*childNode);
	}

	void MapKeyFramesTrackControl::UpdateHandles()
	{
		if (mDisableHandlesUpdate)
			return;

		for (auto kv : mHandles)
		{
			for (auto keyHandle : kv.Value())
				keyHandle->updateFunc(*keyHandle);
		}

		UpdateHandlesCombine();
	}

	void MapKeyFramesTrackControl::UpdateHandlesForValue(IAnimatedValue* animatedValue)
	{
		for (auto keyHandle : mHandles[animatedValue])
			keyHandle->updateFunc(*keyHandle);

		UpdateHandlesCombine();
	}

	WidgetDragHandle* MapKeyFramesTrackControl::CreateHandle()
	{
		WidgetDragHandle* handle = mnew WidgetDragHandle(mnew Sprite("ui/UI4_map_key.png"),
														 mnew Sprite("ui/UI4_map_key_hover.png"),
														 mnew Sprite("ui/UI4_map_key_pressed.png"),
														 mnew Sprite("ui/UI4_selected_map_key.png"),
														 mnew Sprite("ui/UI4_selected_map_key_hover.png"),
														 mnew Sprite("ui/UI4_selected_map_key_pressed.png"));

		handle->cursorType = CursorType::SizeWE;
		handle->pixelPerfect = true;
		handle->SetSpritesSizePivot(Vec2F(7, 2));
		handle->SetSelectionGroup(dynamic_cast<ISelectableDragHandlesGroup*>(mHandlesSheet));

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

	void MapKeyFramesTrackControl::UpdateHandlesCombine()
	{
		auto handlesCopy = mHandles;
		mHandles.Clear();

		for (auto kv : handlesCopy)
		{
			for (auto keyHandle : kv.Value())
				keyHandle->combinedHandles.Clear();
		}

		for (auto kv : handlesCopy)
		{
			mHandles.Add(kv.Key(), KeyHandlesVec());

			for (auto keyHandle : kv.Value())
			{
				keyHandle->combinedHandles = FindHandlesAtPosition(keyHandle->handle->GetPosition().x);

				for (auto combinedHandle : keyHandle->combinedHandles)
					combinedHandle->combinedHandles.Add(keyHandle);

				mHandles[kv.Key()].Add(keyHandle);
			}
		}
	}

	Vector<MapKeyFramesTrackControl::KeyHandle*> MapKeyFramesTrackControl::FindHandlesAtPosition(float position) const
	{
		Vector<KeyHandle*> res;

		float delta = mTimeline->WorldToLocal(1.5f) - mTimeline->WorldToLocal(0);

		for (auto kv : mHandles)
		{
			for (auto keyHandle : kv.Value())
			{
				if (Math::Equals(keyHandle->handle->GetPosition().x, position, delta))
					res.Add(keyHandle);
			}
		}

		return res;
	}

	MapKeyFramesTrackControl::KeyHandle::KeyHandle()
	{}

	MapKeyFramesTrackControl::KeyHandle::KeyHandle(int keyIdx, WidgetDragHandle* handle, IAnimatedValue* animatedValue, const Function<void(KeyHandle& keyHandle)>& updateFunc) :
		keyIdx(keyIdx), handle(handle), animatedValue(animatedValue), updateFunc(updateFunc)
	{}

	bool MapKeyFramesTrackControl::KeyHandle::operator==(const KeyHandle& other) const
	{
		return handle == other.handle;
	}

}

DECLARE_CLASS(Editor::MapKeyFramesTrackControl);
