#pragma once

#include "AnimationWindow/KeyHandlesSheet.h"
#include "AnimationWindow/Timeline.h"
#include "AnimationWindow/TrackControls/ITrackControl.h"
#include "Core/EditorScope.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/Widget.h"
#include "Utils/Editor/DragHandle.h"

using namespace o2;

namespace Editor
{
	// -------------------------------------------------
	// Animation control track for key frames animations
	// Creates handles for each keys and updates them
	// -------------------------------------------------
	template<typename AnimatedValueType>
	class KeyFramesTrackControl: public ITrackControl
	{
	public:
		// Default constructor
		KeyFramesTrackControl();

		// Copy-constructor
		KeyFramesTrackControl(const KeyFramesTrackControl& other);

		// Destructor
		~KeyFramesTrackControl();

		// Copy-operator
		KeyFramesTrackControl& operator=(const KeyFramesTrackControl& other);

		// Draws handles with clipping
		void Draw() override;

		// Updates widget, checks value change
		void Update(float dt) override;

		// Sets animated value, updates and creates key handles
		void SetAnimatedValue(IAnimatedValue* animatedValue) override;

		// Returns animated value
		AnimatedValueType* GetAnimatedValue() const;

		// Sets timeline for calculating handles positions, and  handles sheet as selecting group for handles
		void Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet) override;

		// Updates handles position on timeline
		void UpdateHandles();

		// Returns key handles list
		KeyHandlesVec GetKeyHandles() const override;

		// Returns key handle position
		float GetKeyPosition(int idx) const override;

		// Returns value property
		IPropertyField* GetPropertyField() const override;

		// Inserts new key at time
		void InsertNewKey(float time);

		SERIALIZABLE(KeyFramesTrackControl<AnimatedValueType>);

	private:
		typedef typename AnimatedValueType::ValueType AnimatedValueTypeValueType;

		KeyHandlesVec mHandles; // List of handles, each for keys

		IPropertyField*                               mPropertyField; 
		AnimatedValueTypeValueType                    mPropertyValue = AnimatedValueTypeValueType();
		PointerValueProxy<AnimatedValueTypeValueType> mPropertyValueProxy;

		AnimatedValueTypeValueType mLastAnimatedValue = AnimatedValueTypeValueType();

		AnimatedValueType* mAnimatedValue = nullptr; // Editing animated value
		AnimationTimeline* mTimeline = nullptr;      // Timeline used for calculating handles positions
		KeyHandlesSheet*   mHandlesSheet = nullptr;  // Handles sheet, used for drawing and managing drag handles

	private:
		void InitializeProperty();
		void InitializeHandles();

		WidgetDragHandle* CreateHandle();

		void ChangeHandleIndex(int oldIndex, int newIndex);

		void OnPropertyChanged();
	};

	template<typename AnimatedValueType>
	KeyFramesTrackControl<AnimatedValueType>::KeyFramesTrackControl():
		ITrackControl()
	{
		InitializeProperty();
	}

	template<typename AnimatedValueType>
	KeyFramesTrackControl<AnimatedValueType>::KeyFramesTrackControl(const KeyFramesTrackControl& other) :
		ITrackControl(other)
	{
		InitializeProperty();
	}

	template<typename AnimatedValueType>
	KeyFramesTrackControl<AnimatedValueType>::~KeyFramesTrackControl()
	{
		for (auto handle : mHandles)
			delete handle;
	}

	template<typename AnimatedValueType>
	KeyFramesTrackControl<AnimatedValueType>& KeyFramesTrackControl<AnimatedValueType>::operator=(const KeyFramesTrackControl& other)
	{
		Widget::operator=(other);
		return *this;
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::Draw()
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

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::Update(float dt)
	{
		Widget::Update(dt);

		if (!Math::Equals(mLastAnimatedValue, mAnimatedValue->GetValue()))
		{
			mPropertyValue = mAnimatedValue->GetValue();
			mLastAnimatedValue = mPropertyValue;
			mPropertyField->Refresh();
		}
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::SetAnimatedValue(IAnimatedValue* animatedValue)
	{
		if (mAnimatedValue)
			mAnimatedValue->onKeysChanged -= THIS_FUNC(UpdateHandles);

		mAnimatedValue = dynamic_cast<AnimatedValueType*>(animatedValue);

		if (mAnimatedValue)
			mAnimatedValue->onKeysChanged += THIS_FUNC(UpdateHandles);

		InitializeHandles();
	}

	template<typename AnimatedValueType>
	AnimatedValueType* KeyFramesTrackControl<AnimatedValueType>::GetAnimatedValue() const
	{
		return mAnimatedValue;
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet)
	{
		mTimeline = timeline;
		mHandlesSheet = handlesSheet;
	}

	template<typename AnimatedValueType>
	ITrackControl::KeyHandlesVec KeyFramesTrackControl<AnimatedValueType>::GetKeyHandles() const
	{
		return mHandles;
	}

	template<typename AnimatedValueType>
	float KeyFramesTrackControl<AnimatedValueType>::GetKeyPosition(int idx) const
	{
		return mHandles[idx]->handle->GetPosition().x;
	}

	template<typename AnimatedValueType>
	IPropertyField* KeyFramesTrackControl<AnimatedValueType>::GetPropertyField() const
	{
		return mPropertyField;
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::InitializeProperty()
	{
		auto fieldProto = o2EditorProperties.GetFieldPropertyPrototype(&TypeOf(AnimatedValueType::ValueType));
		mPropertyField = dynamic_cast<IPropertyField*>(o2UI.CreateWidget(fieldProto->GetType(), "standard"));
		mPropertyValueProxy = PointerValueProxy<AnimatedValueTypeValueType>(&mPropertyValue);
		mPropertyField->SetValueProxy({ dynamic_cast<IAbstractValueProxy*>(&mPropertyValueProxy) });
		mPropertyField->onChangeCompleted = [&](const String&, const Vector<DataNode>&, const Vector<DataNode>&) { OnPropertyChanged(); };
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::InitializeHandles()
	{
		PushScopeEnterOnStack scope;

		Vector<WidgetDragHandle*> handlesCache = mHandles.Select<WidgetDragHandle*>(
			[&](const KeyHandle* x) { x->handle->SetParent(nullptr); x->handle->SetEnabled(false); return x->handle; });

		for (auto keyHandle : mHandles)
			delete keyHandle;

		mHandles.Clear();

		int idx = 0;
		for (auto& key : mAnimatedValue->GetKeys())
		{
			WidgetDragHandle* handle = nullptr;

			if (!handlesCache.IsEmpty())
				handle = handlesCache.PopBack();
			else
				handle = CreateHandle();

			handle->SetEnabled(true);
			handle->SetPosition(Vec2F(key.position, 0.0f));
			AddChild(handle);

			KeyHandle* keyhandle = mnew KeyHandle(idx++, handle);
			mHandles.Add(keyhandle);

			handle->onChangedPos = [=](const Vec2F& pos) {
				auto key = mAnimatedValue->GetKeys()[keyhandle->keyIdx];

				key.position = pos.x;
				mAnimatedValue->RemoveKeyAt(keyhandle->keyIdx);
				auto newIdx = mAnimatedValue->AddKey(key);

				if (newIdx != keyhandle->keyIdx) {
					ChangeHandleIndex(keyhandle->keyIdx, newIdx);
					keyhandle->keyIdx = newIdx;
				}
			};

		}
	}


	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::ChangeHandleIndex(int oldIndex, int newIndex)
	{
		KeyHandle* editingHandle = mHandles[oldIndex];
		mHandles.RemoveAt(oldIndex);
		mHandles.Insert(editingHandle, newIndex);

		for (int i = 0; i < mHandles.Count(); i++)
			mHandles[i]->keyIdx = i;
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::UpdateHandles()
	{
		if (!mAnimatedValue)
			return;

		int idx = 0;
		for (auto& key : mAnimatedValue->GetKeys())
		{			
			if (idx < mHandles.Count()) 
			{
				auto keyHandle = mHandles[idx];
				keyHandle->handle->SetPosition(Vec2F(key.position, 0.0f));
			}

			idx++;
		}
	}

	template<typename AnimatedValueType>
	WidgetDragHandle* KeyFramesTrackControl<AnimatedValueType>::CreateHandle()
	{
		WidgetDragHandle* handle = mnew WidgetDragHandle(mnew Sprite("ui/UI4_key.png"), 
														 mnew Sprite("ui/UI4_key_hover.png"),
														 mnew Sprite("ui/UI4_key_pressed.png"),
														 mnew Sprite("ui/UI4_selected_key.png"),
														 mnew Sprite("ui/UI4_selected_key_hover.png"),
														 mnew Sprite("ui/UI4_selected_key_pressed.png"));

		handle->cursorType = CursorType::SizeWE;
		handle->pixelPerfect = true;
		handle->SetSpritesSizePivot(Vec2F(7, 1));
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

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::InsertNewKey(float time)
	{
		mAnimatedValue->AddKey(time, mAnimatedValue->GetValue(time));
		InitializeHandles();
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::OnPropertyChanged()
	{
		auto time = mAnimatedValue->GetTime();
		int keyIdx = -1;
		int i = 0;
		for (auto& key : mAnimatedValue->GetKeys()) 
		{
			if (mTimeline->IsSameTime(key.position, time)) 
			{
				keyIdx = i;
				break;
			}
			i++;
		}

		if (keyIdx >= 0)
		{
			auto key = mAnimatedValue->GetKeys()[keyIdx];
			mAnimatedValue->RemoveKeyAt(keyIdx);
			key.value = mPropertyValue;
			mAnimatedValue->AddKey(key);
		}
		else 
		{
			mAnimatedValue->AddKey(time, mPropertyValue);
			InitializeHandles();
		}
	}
}

META_TEMPLATES(typename AnimatedValueType)
CLASS_BASES_META(Editor::KeyFramesTrackControl<AnimatedValueType>)
{
	BASE_CLASS(Editor::ITrackControl);
}
END_META;
META_TEMPLATES(typename AnimatedValueType)
CLASS_FIELDS_META(Editor::KeyFramesTrackControl<AnimatedValueType>)
{
	PRIVATE_FIELD(mHandles);
	PRIVATE_FIELD(mPropertyField);
	PRIVATE_FIELD(mPropertyValue);
	PRIVATE_FIELD(mPropertyValueProxy);
	PRIVATE_FIELD(mLastAnimatedValue);
	PRIVATE_FIELD(mAnimatedValue);
	PRIVATE_FIELD(mTimeline);
	PRIVATE_FIELD(mHandlesSheet);
}
END_META;
META_TEMPLATES(typename AnimatedValueType)
CLASS_METHODS_META(Editor::KeyFramesTrackControl<AnimatedValueType>)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetAnimatedValue, IAnimatedValue*);
	PUBLIC_FUNCTION(AnimatedValueType*, GetAnimatedValue);
	PUBLIC_FUNCTION(void, Initialize, AnimationTimeline*, KeyHandlesSheet*);
	PUBLIC_FUNCTION(void, UpdateHandles);
	PUBLIC_FUNCTION(KeyHandlesVec, GetKeyHandles);
	PUBLIC_FUNCTION(float, GetKeyPosition, int);
	PUBLIC_FUNCTION(IPropertyField*, GetPropertyField);
	PUBLIC_FUNCTION(void, InsertNewKey, float);
	PRIVATE_FUNCTION(void, InitializeProperty);
	PRIVATE_FUNCTION(void, InitializeHandles);
	PRIVATE_FUNCTION(WidgetDragHandle*, CreateHandle);
	PRIVATE_FUNCTION(void, ChangeHandleIndex, int, int);
	PRIVATE_FUNCTION(void, OnPropertyChanged);
}
END_META;
