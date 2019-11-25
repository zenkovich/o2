#pragma once

#include "AnimationKeyDragHandle.h"
#include "AnimationWindow/AnimationKeysActions.h"
#include "AnimationWindow/AnimationWindow.h"
#include "AnimationWindow/KeyHandlesSheet.h"
#include "AnimationWindow/Timeline.h"
#include "AnimationWindow/TrackControls/ITrackControl.h"
#include "Core/Actions/ActionsList.h"
#include "Core/EditorScope.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/Widget.h"
#include "Scene/UI/Widgets/Image.h"

using namespace o2;

namespace Editor
{
	// -------------------------------------------------
	// Animation control track for key frames animations
	// Creates handles for each keys and updates them
	// -------------------------------------------------
	template<typename AnimatedValueType>
	class KeyFramesTrackControl : public ITrackControl
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
		void SetAnimatedValue(IAnimatedValue* animatedValue, const String& path) override;

		// Returns animated value
		AnimatedValueType* GetAnimatedValue() const;

		// Sets timeline for calculating handles positions, and  handles sheet as selecting group for handles
		void Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet) override;

		// Updates handles position on timeline
		void UpdateHandles();

		// Returns key handles list
		KeyHandlesVec GetKeyHandles() const override;

		// Returns a container of controllers that are part of a tree
		Widget* GetTreePartControls() const override;

		// Sets curves edit view mode
		void SetCurveViewEnabled(bool enabled) override;

		// Sets curves view mode color
		void SetCurveViewColor(const Color4& color) override;

		// Serialize key with specified uid into data node
		void SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime) override;

		// Deserialize key from data node and paste on track
		UInt64 DeserializeKey(const DataNode& data, float relativeTime, bool generateNewUid = true) override;

		// Removes key from track
		void DeleteKey(UInt64 keyUid) override;

		// Inserts new key at time
		void InsertNewKey(float time);

		SERIALIZABLE(KeyFramesTrackControl<AnimatedValueType>);

	private:
		typedef typename AnimatedValueType::ValueType AnimatedValueTypeValueType;

		KeyHandlesVec mHandles; // List of handles, each for keys

		String mAnimatedValuePath; // Path to animated value in animation

		Widget* mTreeControls = nullptr; // Returns a container of controllers that are part of a tree

		IPropertyField*                               mPropertyField;
		AnimatedValueTypeValueType                    mPropertyValue = AnimatedValueTypeValueType();
		PointerValueProxy<AnimatedValueTypeValueType> mPropertyValueProxy;

		Button* mAddKeyDotButton = nullptr; // Dot add key button colored with curve color
		Button* mAddKeyButton = nullptr;    // Add key button, enables when available to add new key

		AnimatedValueTypeValueType mLastAnimatedValue = AnimatedValueTypeValueType();

		AnimatedValueType* mAnimatedValue = nullptr; // Editing animated value
		AnimationTimeline* mTimeline = nullptr;      // Timeline used for calculating handles positions
		KeyHandlesSheet*   mHandlesSheet = nullptr;  // Handles sheet, used for drawing and managing drag handles

		bool mDisableHandlesUpdate = false;  // It is true when handles are changing and combining or updating is not available

	private:
		void InitializeControls();
		void InitializeHandles();

		AnimationKeyDragHandle* CreateHandle();

		void CheckCanCreateKey(float time);

		void OnPropertyChanged();
	};

	template<typename AnimatedValueType>
	KeyFramesTrackControl<AnimatedValueType>::KeyFramesTrackControl() :
		ITrackControl()
	{
		InitializeControls();
	}

	template<typename AnimatedValueType>
	KeyFramesTrackControl<AnimatedValueType>::KeyFramesTrackControl(const KeyFramesTrackControl& other) :
		ITrackControl(other)
	{
		InitializeControls();
	}

	template<typename AnimatedValueType>
	KeyFramesTrackControl<AnimatedValueType>::~KeyFramesTrackControl()
	{
		for (auto handle : mHandles)
			delete handle;

		if (mAnimatedValue)
		{
			auto animVal = mAnimatedValue;
			animVal->onKeysChanged -= THIS_SUBSCRIPTION(UpdateHandles, []() {});
			animVal->onUpdate -= THIS_SUBSCRIPTION(CheckCanCreateKey, []() {});
		}
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

		if (!mHandlesSheet->enabled)
			return;

		o2Render.EnableScissorTest(mTimeline->layout->GetWorldRect());

		for (auto handle : mHandles)
			handle->handle->Draw();

		o2Render.DisableScissorTest();

		OnDrawn();
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
	void KeyFramesTrackControl<AnimatedValueType>::SetAnimatedValue(IAnimatedValue* animatedValue, const String& path)
	{
		mAnimatedValuePath = path;

		if (mAnimatedValue)
		{
			auto animVal = mAnimatedValue;
			animVal->onKeysChanged -= THIS_SUBSCRIPTION(UpdateHandles, []() {});
			animVal->onUpdate -= THIS_SUBSCRIPTION(CheckCanCreateKey, []() {});
		}

		mAnimatedValue = dynamic_cast<AnimatedValueType*>(animatedValue);

		if (mAnimatedValue)
		{
			mAnimatedValue->onKeysChanged += THIS_SUBSCRIPTION(UpdateHandles, [&]() { mAnimatedValue = nullptr; });
			mAnimatedValue->onUpdate += THIS_SUBSCRIPTION(CheckCanCreateKey, [&]() { mAnimatedValue = nullptr; });
		}

		InitializeHandles();
		CheckCanCreateKey(mTimeline->GetTimeCursor());
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
	Widget* KeyFramesTrackControl<AnimatedValueType>::GetTreePartControls() const
	{
		return mTreeControls;
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::SetCurveViewColor(const Color4& color)
	{
		mAddKeyDotButton->GetLayerDrawable<Sprite>("basic/regularBack")->SetColor(color);
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::SetCurveViewEnabled(bool enabled)
	{}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::InitializeControls()
	{
		mTreeControls = mnew Widget();

		auto fieldProto = o2EditorProperties.GetFieldPropertyPrototype(&TypeOf(AnimatedValueType::ValueType));
		mPropertyField = dynamic_cast<IPropertyField*>(o2UI.CreateWidget(fieldProto->GetType(), "standard"));
		mPropertyValueProxy = PointerValueProxy<AnimatedValueTypeValueType>(&mPropertyValue);
		mPropertyField->SetValueProxy({ dynamic_cast<IAbstractValueProxy*>(&mPropertyValueProxy) });
		mPropertyField->onChangeCompleted = [&](const String&, const Vector<DataNode>&, const Vector<DataNode>&) { OnPropertyChanged(); };
		*mPropertyField->layout = WidgetLayout::BothStretch(0, 0, 20, 0);

		mAddKeyButton = o2UI.CreateWidget<Button>("add key");
		*mAddKeyButton->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F());
		mAddKeyButton->onClick = [&]() { InsertNewKey(mTimeline->GetTimeCursor()); };

		mAddKeyDotButton = o2UI.CreateWidget<Button>("add dot key");
		*mAddKeyDotButton->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 0));
		mAddKeyDotButton->GetLayerDrawable<Sprite>("basic/regularBack")->SetColor(Color4::Black());
		mAddKeyDotButton->onClick = [&]() { InsertNewKey(mTimeline->GetTimeCursor()); };
		mAddKeyDotButton->enabled = false;

		mTreeControls->AddChildren({ mPropertyField, mAddKeyButton, mAddKeyDotButton });
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::InitializeHandles()
	{
		PushEditorScopeOnStack scope;

		Vector<UInt64> selectedHandles;
		for (auto keyHandle : mHandles)
		{
			if (keyHandle->handle->IsSelected())
				selectedHandles.Add(keyHandle->keyUid);
		}

		Vector<AnimationKeyDragHandle*> handlesCache = mHandles.Select<AnimationKeyDragHandle*>([&](const KeyHandle* x) {
			x->handle->SetParent(nullptr);
			x->handle->SetEnabled(false);
			x->handle->SetSelectionGroup(nullptr);
			x->handle->SetSelected(false);
			x->handle->onChangedPos.Clear();
			return x->handle;
		});

		for (auto keyHandle : mHandles)
			delete keyHandle;

		mHandles.Clear();

		for (auto& key : mAnimatedValue->GetKeys())
		{
			AnimationKeyDragHandle* handle = nullptr;

			if (!handlesCache.IsEmpty())
				handle = handlesCache.PopBack();
			else
				handle = CreateHandle();

			handle->SetEnabled(true);
			handle->SetPosition(Vec2F(key.position, 0.0f));
			handle->animatedValue = mAnimatedValue;
			handle->animatedValuePath = mAnimatedValuePath;
			handle->trackControl = this;
			handle->keyUid = key.uid;
			handle->isMapping = false;
			handle->SetSelectionGroup(dynamic_cast<ISelectableDragHandlesGroup*>(mHandlesSheet));
			handle->SetSelected(selectedHandles.Contains(key.uid));

			AddChild(handle);

			KeyHandle* keyhandle = mnew KeyHandle(key.uid, handle);
			mHandles.Add(keyhandle);

			handle->onChangedPos = [=](const Vec2F& pos) {
				mDisableHandlesUpdate = true;

				int keyIdx = mAnimatedValue->FindKeyIdx(keyhandle->keyUid);
				auto key = mAnimatedValue->GetKeys()[keyIdx];
				key.position = pos.x;

				mAnimatedValue->RemoveKeyAt(keyIdx);
				mAnimatedValue->AddKey(key);

				mDisableHandlesUpdate = false;
			};

		}
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::UpdateHandles()
	{
		if (!mAnimatedValue)
			return;

		if (mDisableHandlesUpdate)
			return;

		if (mAnimatedValue->GetKeys().Count() != mHandles.Count())
		{
			InitializeHandles();
		}
		else
		{
			for (auto keyHandle : mHandles)
				keyHandle->handle->SetPosition(Vec2F(mAnimatedValue->FindKey(keyHandle->keyUid).position, 0.0f));
		}
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::CheckCanCreateKey(float time)
	{
		time = mTimeline->GetTimeCursor();

		bool hasKeyAtTime = false;
		for (auto key : mAnimatedValue->GetKeys())
		{
			if (mTimeline->IsSameTime(key.position, time))
			{
				hasKeyAtTime = true;
				break;
			}
		}

		mAddKeyButton->SetInteractable(!hasKeyAtTime);
	}

	template<typename AnimatedValueType>
	AnimationKeyDragHandle* KeyFramesTrackControl<AnimatedValueType>::CreateHandle()
	{
		AnimationKeyDragHandle* handle = mnew AnimationKeyDragHandle(mnew Sprite("ui/UI4_key.png"),
																	 mnew Sprite("ui/UI4_key_hover.png"),
																	 mnew Sprite("ui/UI4_key_pressed.png"),
																	 mnew Sprite("ui/UI4_selected_key.png"),
																	 mnew Sprite("ui/UI4_selected_key_hover.png"),
																	 mnew Sprite("ui/UI4_selected_key_pressed.png"));

		handle->cursorType = CursorType::SizeWE;
		handle->pixelPerfect = true;
		handle->SetSpritesSizePivot(Vec2F(7, 1));

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
		int idx = mAnimatedValue->AddKey(time, mAnimatedValue->GetValue(time));
		InitializeHandles();
		mTimeline->SetTimeCursor(time);

		DataNode keyData;
		Map<String, Vector<UInt64>> keys = { { mAnimatedValuePath, { mAnimatedValue->GetKeyAt(idx).uid } } };
		mHandlesSheet->SerializeKeys(keyData, keys, 0);
		mHandlesSheet->mAnimationWindow->mActionsList.DoneAction(mnew AnimationAddKeysAction(keys, keyData, mHandlesSheet));
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::OnPropertyChanged()
	{
		auto time = mTimeline->GetTimeCursor();
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

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::SerializeKey(UInt64 keyUid, DataNode& data, float relativeTime)
	{
		auto key = mAnimatedValue->FindKey(keyUid);
		key.position -= relativeTime;
		data.SetValue(key);
	}

	template<typename AnimatedValueType>
	UInt64 KeyFramesTrackControl<AnimatedValueType>::DeserializeKey(const DataNode& data, float relativeTime,
																	bool generateNewUid /*= true*/)
	{
		AnimatedValueType::Key key;
		data.GetValue(key);
		key.position += relativeTime;

		if (generateNewUid)
			key.uid = Math::Random();

		mAnimatedValue->AddKey(key);

		return key.uid;
	}

	template<typename AnimatedValueType>
	void KeyFramesTrackControl<AnimatedValueType>::DeleteKey(UInt64 keyUid)
	{
		int idx = mAnimatedValue->FindKeyIdx(keyUid);
		if (idx >= 0)
			mAnimatedValue->RemoveKeyAt(idx);
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
	PRIVATE_FIELD(mAnimatedValuePath);
	PRIVATE_FIELD(mTreeControls);
	PRIVATE_FIELD(mPropertyField);
	PRIVATE_FIELD(mPropertyValue);
	PRIVATE_FIELD(mPropertyValueProxy);
	PRIVATE_FIELD(mAddKeyDotButton);
	PRIVATE_FIELD(mAddKeyButton);
	PRIVATE_FIELD(mLastAnimatedValue);
	PRIVATE_FIELD(mAnimatedValue);
	PRIVATE_FIELD(mTimeline);
	PRIVATE_FIELD(mHandlesSheet);
	PRIVATE_FIELD(mDisableHandlesUpdate);
}
END_META;
META_TEMPLATES(typename AnimatedValueType)
CLASS_METHODS_META(Editor::KeyFramesTrackControl<AnimatedValueType>)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetAnimatedValue, IAnimatedValue*, const String&);
	PUBLIC_FUNCTION(AnimatedValueType*, GetAnimatedValue);
	PUBLIC_FUNCTION(void, Initialize, AnimationTimeline*, KeyHandlesSheet*);
	PUBLIC_FUNCTION(void, UpdateHandles);
	PUBLIC_FUNCTION(KeyHandlesVec, GetKeyHandles);
	PUBLIC_FUNCTION(Widget*, GetTreePartControls);
	PUBLIC_FUNCTION(void, SetCurveViewEnabled, bool);
	PUBLIC_FUNCTION(void, SetCurveViewColor, const Color4&);
	PUBLIC_FUNCTION(void, SerializeKey, UInt64, DataNode&, float);
	PUBLIC_FUNCTION(UInt64, DeserializeKey, const DataNode&, float, bool);
	PUBLIC_FUNCTION(void, DeleteKey, UInt64);
	PUBLIC_FUNCTION(void, InsertNewKey, float);
	PRIVATE_FUNCTION(void, InitializeControls);
	PRIVATE_FUNCTION(void, InitializeHandles);
	PRIVATE_FUNCTION(AnimationKeyDragHandle*, CreateHandle);
	PRIVATE_FUNCTION(void, CheckCanCreateKey, float);
	PRIVATE_FUNCTION(void, OnPropertyChanged);
}
END_META;
