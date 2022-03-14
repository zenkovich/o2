#pragma once

#include "AnimationKeyDragHandle.h"
#include "o2/Scene/UI/Widget.h"
#include "o2Editor/AnimationWindow/AnimationKeysActions.h"
#include "o2Editor/AnimationWindow/AnimationWindow.h"
#include "o2Editor/AnimationWindow/KeyHandlesSheet.h"
#include "o2Editor/AnimationWindow/Timeline.h"
#include "o2Editor/AnimationWindow/TrackControls/ITrackControl.h"
#include "o2Editor/Core/Actions/ActionsList.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "AnimationTrackWrapper.h"

using namespace o2;

namespace Editor
{
	// Draws curve by points transformed to basis
	void DrawCurveInCoords(const ApproximationValue* points, int pointsCount, const RectF& pointsBounds, const Basis& drawBasis,
						   const Color4& color);

	// -------------------------------------------------
	// Animation control track for key frames animations
	// Creates handles for each keys and updates them
	// -------------------------------------------------
	template<typename AnimationTrackType>
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

		// Sets Animation track, updates and creates key handles
		void SetTrack(IAnimationTrack* track, IAnimationTrack::IPlayer* player, const String& path) override;

		// Returns Animation track
		AnimationTrackType* GetTrack() const;

		// Sets timeline for calculating handles positions, and  handles sheet as selecting group for handles
		void Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet) override;

		// Updates handles position on timeline
		void UpdateHandles();

		// Returns key handles list
		Vector<KeyHandle*> GetKeyHandles() const override;

		// Returns a container of controllers that are part of a tree
		Widget* GetTreePartControls() const override;

		// Sets curves edit view mode
		void SetCurveViewEnabled(bool enabled) override;

		// Sets curves view mode color
		void SetCurveViewColor(const Color4& color) override;

		// Serialize key with specified uid into data node
		void SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime) override;

		// Deserialize key from data node and paste on track
		UInt64 DeserializeKey(const DataValue& data, float relativeTime, bool generateNewUid = true) override;

		// Removes key from track
		void DeleteKey(UInt64 keyUid) override;

		// Inserts new key at time
		void InsertNewKey(float time) override;

		SERIALIZABLE(KeyFramesTrackControl<AnimationTrackType>);

	protected:
		typedef AnimationTrackWrapper<AnimationTrackType> Wrapper;
		typedef typename Wrapper::TrackPlayerType         TrackPlayerType;
		typedef typename Wrapper::ValueType               TrackValueType;

		Vector<KeyHandle*> mHandles; // List of handles, each for keys

		String mTrackPath; // Path to Animation track in animation

		Widget* mTreeControls = nullptr; // Returns a container of controllers that are part of a tree

		IPropertyField*                   mPropertyField;
		TrackValueType                    mPropertyValue = TrackValueType();
		PointerValueProxy<TrackValueType> mPropertyValueProxy;

		Button* mAddKeyDotButton = nullptr; // Dot add key button colored with curve color
		Button* mAddKeyButton = nullptr;    // Add key button, enables when available to add new key

		TrackValueType mLastValue = TrackValueType();

		AnimationTrackType* mTrack = nullptr;         // Editing Animation track
		TrackPlayerType*    mPlayer = nullptr;        // Track player
		AnimationTimeline*  mTimeline = nullptr;      // Time line used for calculating handles positions
		KeyHandlesSheet*    mHandlesSheet = nullptr;  // Handles sheet, used for drawing and managing drag handles

		bool mDisableHandlesUpdate = false;  // It is true when handles are changing and combining or updating is not available

	private:
		void InitializeControls();
		void InitializeHandles();

		virtual void OnSetTrack() {}
		virtual void OnKeysChanged() {}

		AnimationKeyDragHandle* CreateHandle();

		void CheckCanCreateKey(float time);

		void OnPropertyChanged();
	};

	template<typename AnimationTrackType>
	KeyFramesTrackControl<AnimationTrackType>::KeyFramesTrackControl():
		ITrackControl()
	{
		InitializeControls();
	}

	template<typename AnimationTrackType>
	KeyFramesTrackControl<AnimationTrackType>::KeyFramesTrackControl(const KeyFramesTrackControl& other):
		ITrackControl(other)
	{
		InitializeControls();
	}

	template<typename AnimationTrackType>
	KeyFramesTrackControl<AnimationTrackType>::~KeyFramesTrackControl()
	{
		for (auto handle : mHandles)
			delete handle;

		if (mTrack)
			mTrack->onKeysChanged -= THIS_SUBSCRIPTION(UpdateHandles, []() {});

		if (mPlayer)
			mPlayer->onUpdate -= THIS_SUBSCRIPTION(CheckCanCreateKey, []() {});
	}

	template<typename AnimationTrackType>
	KeyFramesTrackControl<AnimationTrackType>& KeyFramesTrackControl<AnimationTrackType>::operator=(const KeyFramesTrackControl& other)
	{
		Widget::operator=(other);
		return *this;
	}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::Draw()
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

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::Update(float dt)
	{
		Widget::Update(dt);

		if (mPlayer)
		{
			auto playerValue = Wrapper::GetValue(*mPlayer);
			if (!Math::Equals(mLastValue, playerValue))
			{
				mPropertyValue = playerValue;
				mLastValue = mPropertyValue;
				mPropertyField->Refresh();
			}
		}
	}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::SetTrack(IAnimationTrack* track, IAnimationTrack::IPlayer* player, const String& path)
	{
		mTrackPath = path;

		if (mTrack)
			mTrack->onKeysChanged -= THIS_SUBSCRIPTION(UpdateHandles, []() {});

		if (mPlayer)
			mPlayer->onUpdate -= THIS_SUBSCRIPTION(CheckCanCreateKey, []() {});

		mTrack = dynamic_cast<AnimationTrackType*>(track);
		mPlayer = dynamic_cast<TrackPlayerType*>(player);

		if (mTrack)
			mTrack->onKeysChanged += THIS_SUBSCRIPTION(UpdateHandles, [&]() { mTrack = nullptr; });

		if (mPlayer)
			mPlayer->onUpdate += THIS_SUBSCRIPTION(CheckCanCreateKey, [&]() { mPlayer = nullptr; });

		InitializeHandles();
		CheckCanCreateKey(mTimeline->GetTimeCursor());

		OnSetTrack();
	}

	template<typename AnimationTrackType>
	AnimationTrackType* KeyFramesTrackControl<AnimationTrackType>::GetTrack() const
	{
		return mTrack;
	}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet)
	{
		mTimeline = timeline;
		mHandlesSheet = handlesSheet;
	}

	template<typename AnimationTrackType>
	Vector<ITrackControl::KeyHandle*> KeyFramesTrackControl<AnimationTrackType>::GetKeyHandles() const
	{
		return mHandles;
	}

	template<typename AnimationTrackType>
	Widget* KeyFramesTrackControl<AnimationTrackType>::GetTreePartControls() const
	{
		return mTreeControls;
	}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::SetCurveViewColor(const Color4& color)
	{
		mAddKeyDotButton->GetLayerDrawable<Sprite>("basic/regularBack")->SetColor(color);
	}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::SetCurveViewEnabled(bool enabled)
	{}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::InitializeControls()
	{
		mTreeControls = mnew Widget();

		auto fieldProto = o2EditorProperties.GetFieldPropertyType(&TypeOf(TrackValueType));
		mPropertyField = dynamic_cast<IPropertyField*>(o2UI.CreateWidget(*fieldProto, "standard"));
		mPropertyValueProxy = PointerValueProxy<TrackValueType>(&mPropertyValue);
		mPropertyField->SetValueProxy({ dynamic_cast<IAbstractValueProxy*>(&mPropertyValueProxy) });
		mPropertyField->onChangeCompleted = [&](const String&, const Vector<DataDocument>&, const Vector<DataDocument>&) { OnPropertyChanged(); };
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

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::InitializeHandles()
	{
		PushEditorScopeOnStack scope;

		Vector<UInt64> selectedHandles;
		for (auto keyHandle : mHandles)
		{
			if (keyHandle->handle->IsSelected())
				selectedHandles.Add(keyHandle->keyUid);
		}

		Vector<AnimationKeyDragHandle*> handlesCache = mHandles.Convert<AnimationKeyDragHandle*>([&](const KeyHandle* x) {
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

		for (auto& key : Wrapper::GetKeys(*mTrack))
		{
			AnimationKeyDragHandle* handle = nullptr;

			if (!handlesCache.IsEmpty())
				handle = handlesCache.PopBack();
			else
				handle = CreateHandle();

			handle->SetEnabled(true);
			handle->SetPosition(Vec2F(key.position, 0.0f));
			handle->track = mTrack;
			handle->trackPath = mTrackPath;
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

				int keyIdx = Wrapper::FindKeyIdx(*mTrack, keyhandle->keyUid);
				auto key = Wrapper::GetKeys(*mTrack)[keyIdx];
				key.position = pos.x;

				Wrapper::RemoveKeyAt(*mTrack, keyIdx);
				Wrapper::AddKey(*mTrack, key);

				mDisableHandlesUpdate = false;
			};

		}
	}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::UpdateHandles()
	{
		if (!mTrack)
			return;

		if (mDisableHandlesUpdate)
			return;

		if (Wrapper::GetKeys(*mTrack).Count() != mHandles.Count())
		{
			InitializeHandles();
		}
		else
		{
			for (auto keyHandle : mHandles)
				keyHandle->handle->SetPosition(Vec2F(Wrapper::FindKey(*mTrack, keyHandle->keyUid).position, 0.0f));
		}
	}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::CheckCanCreateKey(float time)
	{
		time = mTimeline->GetTimeCursor();

		bool hasKeyAtTime = false;
		for (auto key : Wrapper::GetKeys(*mTrack))
		{
			if (mTimeline->IsSameTime(key.position, time))
			{
				hasKeyAtTime = true;
				break;
			}
		}

		mAddKeyButton->SetInteractable(!hasKeyAtTime);
	}

	template<typename AnimationTrackType>
	AnimationKeyDragHandle* KeyFramesTrackControl<AnimationTrackType>::CreateHandle()
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

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::InsertNewKey(float time)
	{
		typename Wrapper::KeyType key;
		key.position = time;
		key.value = Wrapper::GetValue(*mTrack, time);

		int idx = Wrapper::AddKey(*mTrack, key);

		InitializeHandles();
		mTimeline->SetTimeCursor(time);

		OnKeysChanged();

		DataDocument keyData;
		Map<String, Vector<UInt64>> keys = { { mTrackPath, { Wrapper::GetKey(*mTrack, idx).uid } } };
		mHandlesSheet->SerializeKeys(keyData, keys, 0);
		mHandlesSheet->mAnimationWindow->mActionsList.DoneAction(mnew AnimationAddKeysAction(keys, keyData, mHandlesSheet));
	}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::OnPropertyChanged()
	{
		auto time = mTimeline->GetTimeCursor();
		int keyIdx = -1;
		int i = 0;
		for (auto& key : Wrapper::GetKeys(*mTrack))
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
			auto key = Wrapper::GetKeys(*mTrack)[keyIdx];
			Wrapper::RemoveKeyAt(*mTrack, keyIdx);
			key.value = mPropertyValue;
			Wrapper::AddKey(*mTrack, key);
		}
		else
		{
			typename Wrapper::KeyType key;
			key.position = time;
			key.value = mPropertyValue;
			Wrapper::AddKey(*mTrack, key);
			InitializeHandles();
		}

		OnKeysChanged();
	}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime)
	{
		auto key = Wrapper::FindKey(*mTrack, keyUid);
		key.position -= relativeTime;
		data.Set(key);
	}

	template<typename AnimationTrackType>
	UInt64 KeyFramesTrackControl<AnimationTrackType>::DeserializeKey(const DataValue& data, float relativeTime,
																	 bool generateNewUid /*= true*/)
	{
		typename Wrapper::KeyType key;
		data.Get(key);
		key.position += relativeTime;

		if (generateNewUid)
			key.uid = Math::Random();

		Wrapper::AddKey(*mTrack, key);

		return key.uid;
	}

	template<typename AnimationTrackType>
	void KeyFramesTrackControl<AnimationTrackType>::DeleteKey(UInt64 keyUid)
	{
		int idx = Wrapper::FindKeyIdx(*mTrack, keyUid);
		if (idx >= 0)
			Wrapper::RemoveKeyAt(*mTrack, idx);
	}

}

META_TEMPLATES(typename AnimationTrackType)
CLASS_BASES_META(Editor::KeyFramesTrackControl<AnimationTrackType>)
{
	BASE_CLASS(Editor::ITrackControl);
}
END_META;
META_TEMPLATES(typename AnimationTrackType)
CLASS_FIELDS_META(Editor::KeyFramesTrackControl<AnimationTrackType>)
{
	FIELD().PROTECTED().NAME(mHandles);
	FIELD().PROTECTED().NAME(mTrackPath);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTreeControls);
	FIELD().PROTECTED().NAME(mPropertyField);
	FIELD().PROTECTED().DEFAULT_VALUE(TrackValueType()).NAME(mPropertyValue);
	FIELD().PROTECTED().NAME(mPropertyValueProxy);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAddKeyDotButton);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAddKeyButton);
	FIELD().PROTECTED().DEFAULT_VALUE(TrackValueType()).NAME(mLastValue);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTrack);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPlayer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTimeline);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHandlesSheet);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mDisableHandlesUpdate);
}
END_META;
META_TEMPLATES(typename AnimationTrackType)
CLASS_METHODS_META(Editor::KeyFramesTrackControl<AnimationTrackType>)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const KeyFramesTrackControl&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTrack, IAnimationTrack*, IAnimationTrack::IPlayer*, const String&);
	FUNCTION().PUBLIC().SIGNATURE(AnimationTrackType*, GetTrack);
	FUNCTION().PUBLIC().SIGNATURE(void, Initialize, AnimationTimeline*, KeyHandlesSheet*);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateHandles);
	FUNCTION().PUBLIC().SIGNATURE(Vector<KeyHandle*>, GetKeyHandles);
	FUNCTION().PUBLIC().SIGNATURE(Widget*, GetTreePartControls);
	FUNCTION().PUBLIC().SIGNATURE(void, SetCurveViewEnabled, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, SetCurveViewColor, const Color4&);
	FUNCTION().PUBLIC().SIGNATURE(void, SerializeKey, UInt64, DataValue&, float);
	FUNCTION().PUBLIC().SIGNATURE(UInt64, DeserializeKey, const DataValue&, float, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, DeleteKey, UInt64);
	FUNCTION().PUBLIC().SIGNATURE(void, InsertNewKey, float);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeControls);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeHandles);
	FUNCTION().PRIVATE().SIGNATURE(void, OnSetTrack);
	FUNCTION().PRIVATE().SIGNATURE(void, OnKeysChanged);
	FUNCTION().PRIVATE().SIGNATURE(AnimationKeyDragHandle*, CreateHandle);
	FUNCTION().PRIVATE().SIGNATURE(void, CheckCanCreateKey, float);
	FUNCTION().PRIVATE().SIGNATURE(void, OnPropertyChanged);
}
END_META;
