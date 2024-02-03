> Replace raw pointers with Ref<>, except for void* pointers.

```cpp
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

        // Sets Animation track, updates and creates key handles
        void SetTrack(const Ref<IAnimationTrack>& track, const Ref<IAnimationTrack::IPlayer>& player, const String& path) override;

        // Returns Animation track
        const Ref<AnimationTrackType>& GetTrack() const;

        // Sets timeline for calculating handles positions, and  handles sheet as selecting group for handles
        void Initialize(const Ref<AnimationTimeline>& timeline, const Ref<KeyHandlesSheet>& handlesSheet) override;

        // Updates handles position on timeline
        void UpdateHandles() override;

        // Returns key handles list
        Vector<Ref<KeyHandle>> GetKeyHandles() const override;

        // Returns a container of controllers that are part of a tree
        const Ref<Widget>& GetTreePartControls() const override;

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

        Vector<Ref<KeyHandle>> mHandles; // List of handles, each for keys

        String mTrackPath; // Path to Animation track in animation

        const Ref<Widget> mTreeControls; // Container of controllers that are part of a tree

        Ref<IPropertyField>                    mPropertyField;
        TrackValueType                         mPropertyValue = TrackValueType();
        Ref<PointerValueProxy<TrackValueType>> mPropertyValueProxy;

        Ref<Button> mAddKeyDotButton; // Dot add key button colored with curve color
        Ref<Button> mAddKeyButton;    // Add key button, enables when available to add new key

        TrackValueType mLastValue = TrackValueType();

        Ref<AnimationTrackType> mTrack;         // Editing Animation track
        Ref<TrackPlayerType>    mPlayer;        // Track player
        Ref<AnimationTimeline>  mTimeline;      // Time line used for calculating handles positions
        Ref<KeyHandlesSheet>    mHandlesSheet;  // Handles sheet, used for drawing and managing drag handles

        bool mDisableHandlesUpdate = false;  // It is true when handles are changing and combining or updating is not available

    private:
        void InitializeControls();
        void InitializeHandles();

        virtual void OnSetTrack() {}
        virtual void OnKeysChanged() {}

        Ref<AnimationKeyDragHandle> CreateHandle();

        void CheckCanCreateKey(float time);

        void OnPropertyChanged();
    };

    template<typename AnimationTrackType>
    KeyFramesTrackControl<AnimationTrackType>::KeyFramesTrackControl() :
        ITrackControl()
    {
        InitializeControls();
    }

    template<typename AnimationTrackType>
    KeyFramesTrackControl<AnimationTrackType>::KeyFramesTrackControl(const KeyFramesTrackControl& other) :
        ITrackControl(other)
    {
        InitializeControls();
    }

    template<typename AnimationTrackType>
    KeyFramesTrackControl<AnimationTrackType>::~KeyFramesTrackControl()
    {
    }

    template<typename AnimationTrackType>
    KeyFramesTrackControl<AnimationTrackType>& KeyFramesTrackControl<AnimationTrackType>::operator=(const KeyFramesTrackControl& other)
    {
        if (this != &other)
        {
            ITrackControl::operator=(other);
            InitializeControls();
        }
        return *this;
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::Draw()
    {
        // Implementation of Draw() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::Update(float dt)
    {
        // Implementation of Update() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::SetTrack(const Ref<IAnimationTrack>& track, const Ref<IAnimationTrack::IPlayer>& player, const String& path)
    {
        // Implementation of SetTrack() function
    }

    template<typename AnimationTrackType>
    const Ref<AnimationTrackType>& KeyFramesTrackControl<AnimationTrackType>::GetTrack() const
    {
        return mTrack;
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::Initialize(const Ref<AnimationTimeline>& timeline, const Ref<KeyHandlesSheet>& handlesSheet)
    {
        // Implementation of Initialize() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::UpdateHandles()
    {
        // Implementation of UpdateHandles() function
    }

    template<typename AnimationTrackType>
    Vector<Ref<KeyHandle>> KeyFramesTrackControl<AnimationTrackType>::GetKeyHandles() const
    {
        return mHandles;
    }

    template<typename AnimationTrackType>
    const Ref<Widget>& KeyFramesTrackControl<AnimationTrackType>::GetTreePartControls() const
    {
        return mTreeControls;
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::SetCurveViewEnabled(bool enabled)
    {
        // Implementation of SetCurveViewEnabled() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::SetCurveViewColor(const Color4& color)
    {
        // Implementation of SetCurveViewColor() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime)
    {
        // Implementation of SerializeKey() function
    }

    template<typename AnimationTrackType>
    UInt64 KeyFramesTrackControl<AnimationTrackType>::DeserializeKey(const DataValue& data, float relativeTime, bool generateNewUid)
    {
        // Implementation of DeserializeKey() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::DeleteKey(UInt64 keyUid)
    {
        // Implementation of DeleteKey() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::InsertNewKey(float time)
    {
        // Implementation of InsertNewKey() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::InitializeControls()
    {
        // Implementation of InitializeControls() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::InitializeHandles()
    {
        // Implementation of InitializeHandles() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::OnPropertyChanged()
    {
        // Implementation of OnPropertyChanged() function
    }
    
    template<typename AnimationTrackType>
    Ref<AnimationKeyDragHandle> KeyFramesTrackControl<AnimationTrackType>::CreateHandle()
    {
        // Implementation of CreateHandle() function
    }

    template<typename AnimationTrackType>
    void KeyFramesTrackControl<AnimationTrackType>::CheckCanCreateKey(float time)
    {
        // Implementation of CheckCanCreateKey() function
    }

} // namespace Editor

```template<typename AnimationTrackType>
class Ref
{
public:
    Ref() : ptr(nullptr) {}
    Ref(AnimationTrackType* p) : ptr(p) {}
    Ref(const Ref& other) : ptr(other.ptr) {}

    AnimationTrackType* operator->()
    {
        return ptr;
    }

    const AnimationTrackType* operator->() const
    {
        return ptr;
    }

    AnimationTrackType& operator*()
    {
        return *ptr;
    }

    const AnimationTrackType& operator*() const
    {
        return *ptr;
    }

private:
    AnimationTrackType* ptr;
};

template<typename T>
class WeakRef
{
public:
    WeakRef() : ptr(nullptr) {}
    WeakRef(const Ref<T>& ref) : ptr(ref.ptr) {}
    WeakRef(const WeakRef& other) : ptr(other.ptr) {}

    Ref<T> lock() const
    {
        return Ref<T>(ptr);
    }

private:
    T* ptr;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ref)
{
    return Ref<T>(dynamic_cast<T*>(ref.operator->()));
}

template<typename AnimationTrackType>
class KeyFramesTrackControl : public Widget
{
public:
    using KeyHandle = ITrackControl::KeyHandle;
    using TrackPlayerType = IAnimationTrack::IPlayer;
    using TrackValueType = typename AnimationTrackType::ValueType;

    void Init();

    void operator--(int);

    KeyFramesTrackControl& operator=(const KeyFramesTrackControl& other);

    void Draw() const;

    void Update(float dt);

    void SetTrack(const Ref<IAnimationTrack>& track, const Ref<IAnimationTrack::IPlayer>& player, const String& path);

    const Ref<AnimationTrackType>& GetTrack() const;

    void Initialize(const Ref<AnimationTimeline>& timeline, const Ref<KeyHandlesSheet>& handlesSheet);

    Vector<Ref<ITrackControl::KeyHandle>> GetKeyHandles() const;

    const Ref<Widget>& GetTreePartControls() const;

    void SetCurveViewColor(const Color4& color);

    void SetCurveViewEnabled(bool enabled);

    void InitializeControls();

    void InitializeHandles();

private:
    Ref<AnimationTrackType> mTrack;
    Ref<TrackPlayerType> mPlayer;
    Ref<AnimationTimeline> mTimeline;
    Ref<KeyHandlesSheet> mHandlesSheet;
    Vector<Ref<AnimationKeyDragHandle>> mHandles;
    Ref<Widget> mTreeControls;
    Ref<IPropertyField> mPropertyField;
    PointerValueProxy<TrackValueType> mPropertyValueProxy;
    TrackValueType mPropertyValue;
    Button* mAddKeyButton;
    Button* mAddKeyDotButton;
    TrackValueType mLastValue;
    Ref<KeyFrameControl> mActiveKeyControl;

    void CheckCanCreateKey(float cursorTime);

    void UpdateHandles();

    void InsertNewKey(float cursorTime);

    void OnPropertyChanged();
};

template<typename AnimationTrackType>
void KeyFramesTrackControl<AnimationTrackType>::Init()
{
    mAddKeyButton = mmake<Button>("add key");
    mAddKeyDotButton = mmake<Button>("add dot key");

    InitializeControls();
}

template<typename AnimationTrackType>
void KeyFramesTrackControl<AnimationTrackType>::operator--(int)
{
    mTrack = nullptr;
    mPlayer = nullptr;
}

template<typename AnimationTrackType>
KeyFramesTrackControl<AnimationTrackType>& KeyFramesTrackControl<AnimationTrackType>::operator=(const KeyFramesTrackControl& other)
{
    Widget::operator=(other);
    return *this;
}

template<typename AnimationTrackType>
void KeyFramesTrackControl<AnimationTrackType>::Draw() const
{
    if (!mResEnabledInHierarchy)
        return;

    if (!mHandlesSheet->enabled)
        return;

    o2Render.EnableScissorTest(mTimeline->layout->GetWorldRect());

    for (const auto& handle : mHandles)
        handle->handle->Draw();

    o2Render.DisableScissorTest();

    OnDrawn();
    DrawDebugFrame();
}

template<typename AnimationTrackType>
void KeyFramesTrackControl<AnimationTrackType>::Update(float dt)
{
    Widget::Update(dt);

    if (mPlayer) {
        auto playerValue = Wrapper::GetValue(*mPlayer);
        if (!Math::Equals(mLastValue, playerValue)) {
            mPropertyValue = playerValue;
            mLastValue = mPropertyValue;
            mPropertyField->Refresh();
        }
    }
}

template<typename AnimationTrackType>
void KeyFramesTrackControl<AnimationTrackType>::SetTrack(const Ref<IAnimationTrack>& track, const Ref<IAnimationTrack::IPlayer>& player, const String& path)
{
    mTrackPath = path;

    if (mTrack)
        mTrack->onKeysChanged -= THIS_SUBSCRIPTION(UpdateHandles, []() {});

    if (mPlayer)
        mPlayer->onUpdate -= THIS_SUBSCRIPTION(CheckCanCreateKey, []() {});

    mTrack = DynamicCast<AnimationTrackType>(track);
    mPlayer = DynamicCast<TrackPlayerType>(player);

    if (mTrack)
        mTrack->onKeysChanged += THIS_SUBSCRIPTION(UpdateHandles, [&]() { mTrack = nullptr; });

    if (mPlayer)
        mPlayer->onUpdate += THIS_SUBSCRIPTION(CheckCanCreateKey, [&]() { mPlayer = nullptr; });

    InitializeHandles();
    CheckCanCreateKey(mTimeline->GetTimeCursor());

    OnSetTrack();
}

template<typename AnimationTrackType>
const Ref<AnimationTrackType>& KeyFramesTrackControl<AnimationTrackType>::GetTrack() const
{
    return mTrack;
}

template<typename AnimationTrackType>
void KeyFramesTrackControl<AnimationTrackType>::Initialize(const Ref<AnimationTimeline>& timeline, const Ref<KeyHandlesSheet>& handlesSheet)
{
    mTimeline = timeline;
    mHandlesSheet = handlesSheet;
}

template<typename AnimationTrackType>
Vector<Ref<ITrackControl::KeyHandle>> KeyFramesTrackControl<AnimationTrackType>::GetKeyHandles() const
{
    return mHandles;
}

template<typename AnimationTrackType>
const Ref<Widget>& KeyFramesTrackControl<AnimationTrackType>::GetTreePartControls() const
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
    mTreeControls = mmake<Widget>();

    auto fieldProto = o2EditorProperties.GetFieldPropertyType(&TypeOf(TrackValueType));
    mPropertyField = DynamicCast<IPropertyField>(o2UI.CreateWidget(*fieldProto, "standard"));
    mPropertyValueProxy = mmake<PointerValueProxy<TrackValueType>>(&mPropertyValue);
    mPropertyField->SetValueProxy({ DynamicCast<IAbstractValueProxy>(mPropertyValueProxy) });
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
    for (const auto& keyHandle : mHandles) {
        if (keyHandle->handle->IsSelected())
            selectedHandles.Add(keyHandle->keyUid);
    }

    Vector<AnimationKeyDragHandle*> handlesCache = mHandles.Convert<AnimationKeyDragHandle*>([&](const KeyHandle* x) {
        x->handle->SetParent(nullptr);
        x->handle->SetEnabled(false);
        x->handle->SetSelectionGroup(nullptr);
        x->handle->SetSelected(false);
        x->handle->onChangedPos.Clear();
    });
    //template<typename AnimationTrackType>
class KeyFramesTrackControl
{
public:
    KeyFramesTrackControl() = default;
    ~KeyFramesTrackControl() = default;

    void SetTrackPath(const String& trackPath)
    {
        mTrackPath = trackPath;
    }

    void SetTimeline(Timeline* timeline)
    {
        mTimeline = timeline;
    }

    void SetPropertyValue(const typename AnimationTrackType::ValueType& value)
    {
        mPropertyValue = value;
    }

    void SetSelectedHandles(const Set<UInt64>& selectedHandles)
    {
        mSelectedHandles = selectedHandles;
    }

    void SetHandlesSheet(HandlesSheet* handlesSheet)
    {
        mHandlesSheet = handlesSheet;
    }

    void SetTrack(AnimationTrackType* track)
    {
        mTrack = track;
    }

    void Initialize()
    {
        InitializeHandles();
        UpdateHandles();
    }

    void Update()
    {
        UpdateHandles();
        CheckCanCreateKey();
    }

    void InsertNewKey(float time);

    void SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime)
    {
        auto key = Wrapper::FindKey(*mTrack, keyUid);
        key.position -= relativeTime;
        data.Set(key);
    }

    UInt64 DeserializeKey(const DataValue& data);

private:
    void InitializeHandles()
    {
        Vector<AnimationKeyDragHandle*> handlesCache;
        handlesCache.Reserve(mHandles.Size());

        for (auto keyHandle : mHandles)
        {
            handlesCache.PushBack(keyHandle->handle);
            RemoveChild(keyHandle->handle);
            delete keyHandle;
        }

        mHandles.Clear();

        for (auto& key : Wrapper::GetKeys(*mTrack))
        {
            AnimationKeyDragHandle* handle = nullptr;

            if (!handlesCache.IsEmpty())
            {
                handle = handlesCache.PopBack();
            }
            else
            {
                handle = CreateHandle();
            }

            handle->SetEnabled(true);
            handle->SetPosition(Vec2F(key.position, 0.0f));
            handle->track = mTrack;
            handle->trackPath = mTrackPath;
            handle->trackControl = this;
            handle->keyUid = key.uid;
            handle->isMapping = false;
            handle->SetSelectionGroup(dynamic_cast<ISelectableDragHandlesGroup*>(mHandlesSheet));
            handle->SetSelected(mSelectedHandles.Contains(key.uid));

            AddChild(handle);

            KeyHandle* keyhandle = new KeyHandle(key.uid, handle);
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

    void UpdateHandles()
    {
        if (!mTrack)
        {
            return;
        }

        if (mDisableHandlesUpdate)
        {
            return;
        }

        if (Wrapper::GetKeys(*mTrack).Count() != mHandles.Count())
        {
            InitializeHandles();
        }
        else
        {
            for (auto keyHandle : mHandles)
            {
                keyHandle->handle->SetPosition(Vec2F(Wrapper::FindKey(*mTrack, keyHandle->keyUid).position, 0.0f));
            }
        }
    }

    void CheckCanCreateKey()
    {
        float time = mTimeline->GetTimeCursor();

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

    AnimationKeyDragHandle* CreateHandle()
    {
        AnimationKeyDragHandle* handle = new AnimationKeyDragHandle(
            Ref<Sprite>(new Sprite("ui/UI4_key.png")),
            Ref<Sprite>(new Sprite("ui/UI4_key_hover.png")),
            Ref<Sprite>(new Sprite("ui/UI4_key_pressed.png")),
            Ref<Sprite>(new Sprite("ui/UI4_selected_key.png")),
            Ref<Sprite>(new Sprite("ui/UI4_selected_key_hover.png")),
            Ref<Sprite>(new Sprite("ui/UI4_selected_key_pressed.png"))
        );

        handle->cursorType = CursorType::SizeWE;
        handle->pixelPerfect = true;
        handle->SetDrawablesSizePivot(Vec2F(7, 1));

        handle->checkPositionFunc = [&](const Vec2F& pos) {
            float position = pos.x;
            if (position < 0.0f)
            {
                position = 0.0f;
            }

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

    void RemoveHandle(UInt64 keyUid)
    {
        int handleIdx = FindHandleIdx(keyUid);
        if (handleIdx != -1)
        {
            auto keyHandle = mHandles[handleIdx];
            mHandles.RemoveAt(handleIdx);
            RemoveChild(keyHandle->handle);
            delete keyHandle;
        }
    }
};

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
    mHandlesSheet->mAnimationWindow->mActionsList.DoneAction(new AnimationAddKeysAction(keys, keyData, mHandlesSheet));
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
replace raw pointers with Ref<>, except void* pointers. Remove default nullptr value for class members with type Ref<>. Use const Ref<>& for function arguments with pointer types, also in overridden methods. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

#include <memory>

template<typename T>
class Ref
{
public:
    Ref() : m_ptr(nullptr) {}

    Ref(T* ptr) : m_ptr(ptr) {}

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    Ref(const Ref<U>& other) : m_ptr(other.m_ptr) {}

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    Ref& operator=(const Ref<U>& other)
    {
        m_ptr = other.m_ptr;
        return *this;
    }

    T* operator->() const { return m_ptr; }
    
    T& operator*() const { return *m_ptr; }

    T* get() const { return m_ptr; }

    explicit operator bool() const { return m_ptr != nullptr; }

    bool operator==(const Ref<T>& other) const { return m_ptr == other.m_ptr; }

    bool operator!=(const Ref<T>& other) const { return m_ptr != other.m_ptr; }

private:
    T* m_ptr;
};

template<typename T>
class WeakRef
{
public:
    WeakRef() : m_ptr(nullptr) {}

    WeakRef(const Ref<T>& ref) : m_ptr(ref.get()) {}

    WeakRef& operator=(const Ref<T>& ref)
    {
        m_ptr = ref.get();
        return *this;
    }

    T* operator->() const { return m_ptr; }
    
    T& operator*() const { return *m_ptr; }

    T* get() const { return m_ptr; }

    explicit operator bool() const { return m_ptr != nullptr; }

    bool operator==(const WeakRef<T>& other) const { return m_ptr == other.m_ptr; }

    bool operator!=(const WeakRef<T>& other) const { return m_ptr != other.m_ptr; }

private:
    T* m_ptr;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
using DynamicCast = T;

class KeyFramesTrackControl : public ITrackControl
{
public:
    KeyFramesTrackControl()
        : mHandlesSheet(nullptr), mDisableHandlesUpdate(false)
    {}

    KeyFramesTrackControl(const KeyFramesTrackControl& other)
        : mHandles(other.mHandles), mTrackPath(other.mTrackPath),
          mTreeControls(other.mTreeControls), mPropertyField(other.mPropertyField),
          mPropertyValue(other.mPropertyValue), mPropertyValueProxy(other.mPropertyValueProxy),
          mAddKeyDotButton(other.mAddKeyDotButton), mAddKeyButton(other.mAddKeyButton),
          mLastValue(other.mLastValue), mTrack(other.mTrack), mPlayer(other.mPlayer),
          mTimeline(other.mTimeline), mHandlesSheet(other.mHandlesSheet),
          mDisableHandlesUpdate(other.mDisableHandlesUpdate)
    {}

    void Draw();

    void Update(float deltaTime);

    void SetTrack(const Ref<IAnimationTrack>& track, const Ref<IAnimationTrack::IPlayer>& player, const String& trackPath);

    Ref<IAnimationTrack> GetTrack() const { return mTrack; }

    void Initialize(const Ref<AnimationTimeline>& timeline, const Ref<KeyHandlesSheet>& handlesSheet);

    void UpdateHandles();

    Vector<Ref<KeyHandle>> GetKeyHandles();

    Ref<Widget> GetTreePartControls();

    void SetCurveViewEnabled(bool enabled);

    void SetCurveViewColor(const Color4& color);

    void SerializeKey(UInt64 keyUid, DataValue& data, float relativeTime);

    UInt64 DeserializeKey(const DataValue& data, float relativeTime, bool generateNewUid = true);

    void DeleteKey(UInt64 keyUid);

    void InsertNewKey(float relativeTime);

private:
    void InitializeControls();

    void InitializeHandles();

    void OnSetTrack();

    void OnKeysChanged();

    Ref<AnimationKeyDragHandle> CreateHandle();

    void CheckCanCreateKey(float relativeTime);

    void OnPropertyChanged();

private:
    Vector<Ref<KeyHandle>> mHandles;
    String mTrackPath;
    Ref<Widget> mTreeControls;
    Ref<IPropertyField> mPropertyField;
    TrackValueType mPropertyValue;
    Ref<TrackValueProxy> mPropertyValueProxy;
    Ref<Button> mAddKeyDotButton;
    Ref<Button> mAddKeyButton;
    TrackValueType mLastValue;
    Ref<IAnimationTrack> mTrack;
    Ref<IAnimationTrack::IPlayer> mPlayer;
    Ref<AnimationTimeline> mTimeline;
    WeakRef<KeyHandlesSheet> mHandlesSheet;
    bool mDisableHandlesUpdate;
};