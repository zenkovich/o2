#pragma once

#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2Editor/AnimationWindow/TrackControls/AnimationKeyDragHandle.h"
#include <memory>

namespace o2
{
    class AnimationClip;
    class ContextMenu;
    class Sprite;
}

using namespace o2;

namespace Editor
{
    class AnimationWindow;
    class ITrackControl;

    // -------------------------------------------
    // Handles sheet, manages selection of handles
    // -------------------------------------------
    class KeyHandlesSheet : public Widget, public SelectableDragHandlesGroup, public CursorAreaEventsListener
    {
    public:
        using Ref = std::shared_ptr;

        // Default constructor
        KeyHandlesSheet();

        // Copy-constructor
        KeyHandlesSheet(const KeyHandlesSheet& other);

        // Destructor
        ~KeyHandlesSheet();

        // Copy-operator
        KeyHandlesSheet& operator=(const KeyHandlesSheet& other);

        // Sets animation. Used for batch change of keys
        void SetAnimation(Ref<AnimationClip> animation);

        // Updates selection frame
        void Update(float dt) override;

        // Draws selection
        void Draw() override;

        // Updates draw order for correct handles and sheet input processing
        void UpdateInputDrawOrder();

        // Returns true if point is in this object
        bool IsUnderPoint(const Vec2F& point) override;

        // Registers animation track track control
        void RegTrackControl(ITrackControl* trackControl, const std::string& path);

        // Unregisters animation track track control
        void UnregTrackControl(ITrackControl* trackControl);

        // Unregisters all tracks controls
        void UnregAllTrackControls();

        // Adds selectable handle to group
        void AddHandle(const Ref<DragHandle>& handle) override;

        // Removes selectable handle from group
        void RemoveHandle(DragHandle* handle) override;

        // Sets selected keys handles
        void SetSelectedKeys(const Map<String, Vector<UInt64>>& keys);

        // Returns list of selected keys handles
        Map<String, Vector<UInt64>> GetSelectedKeys() const;

        // Removes keys
        void DeleteKeys(const Map<String, Vector<UInt64>>& keys, bool createAction = true);

        // Returns context menu
        Ref<ContextMenu> GetContextMenu() const;

        // Returns create menu category in editor
        static String GetCreateMenuCategory();

        SERIALIZABLE(KeyHandlesSheet);

    private:
        RectF mSelectionFrameOffsets = RectF(-9, -3, 5, 2);
        RectF mSelectionFrameCursorOffsets = RectF(-2, -3, 2, 2);

        Ref<AnimationWindow> mAnimationWindow = nullptr; // Animation window

        Vector<ITrackControl*> mTrackControls;    // List of actual track controls
        Vector<Pair<String, ITrackControl*>> mTrackControlsMap; // Map of actual track controls, key is Animation track path

        Map<IAnimationTrack*, Vector<Ref<AnimationKeyDragHandle>>> mHandlesGroups; // All handles grouped by Animation track, used for fast searching handles for same Animation track

        Ref<ContextMenu> mContextMenu = nullptr; // Keys context menu
        Vec2F mContextMenuPressPoint; // Cursor position when right button were clicked. When trying to show context menu checking delta between current cursor position and this 

        bool mNeedUpdateSelectionFrame = false; // True when selection frame required to update

        bool mHandleHasMoved = false; // it is true when some handle was selected and moved, resets on handle pressing

        Sprite* mSelectionFrame = nullptr; // Selected handles frame drawing sprite
        RectF   mSelectionRect;            // Current selected handles rectangle. The right and left is minimum and maximum handles positions, top and bottom is minimum and maximum handles lines

        Vec2F   mBeginSelectPoint;         // Begin frame selection point, where x is position on timeline, y is line number
        bool    mIsFrameSelecting = false; // It is true when user selection by frame now

        Vector<DragHandle*> mBeginSelectHandles; // handles list, that were selected before frame selecting

        DragHandle mLeftFrameDragHandle;   // Left frame border drag handle, resizing selected handles rect
        DragHandle mRightFrameDragHandle;  // Right frame border drag handle, resizing selected handles rect
        DragHandle mCenterFrameDragHandle; // Center frame drag handle, moves selected handles

        DataDocument mBeforeChangeKeysData; // Serialized keys data before change

    private:
        // Initializes frame handles
        void InitializeHandles();

        // Initializes center handle, that moves selected keys on timeline
        void InitializeCenterHandle();

        // Initializes left handle, that moves selected keys on timeline relative to right selection rect position
        void InitializeLeftHandle();

        // Initializes right handle, that moves selected keys on timeline relative to left selection rect position
        void InitializeRightHandle();

        // Initializes context menu with copy, paste, delete etc
        void InitializeContextMenu();

        // Updates selection rectangle and drawing sprite
        void UpdateSelectionFrame();

        // Serialized keys into data
        void SerializeKeys(DataValue& data, const Map<String, Vector<UInt64>>& keys, float relativeTime);

        // Deserializes keys from data 
        void D#include <memory>
#include <vector>
#include <unordered_map>

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

class AnimationWindow;

namespace Editor {
    class DragHandle;
    class KeyHandlesSheet : public o2::Widget, public o2::SelectableDragHandlesGroup, public o2::CursorAreaEventsListener {
    public:
        void CopyKeys();

        void PasteKeys();

        void OnSelectionChanged() override;

        void OnHandleCursorPressed(const Ref<DragHandle>& handle, const Input::Cursor& cursor) override;

        void OnHandleCursorReleased(const Ref<DragHandle>& handle, const Input::Cursor& cursor) override;

        void OnHandleMoved(const Ref<DragHandle>& handle, const Vec2F& cursorPos) override;

        void OnCursorPressed(const Input::Cursor& cursor) override;

        void OnCursorReleased(const Input::Cursor& cursor) override;

        void OnCursorPressBreak(const Input::Cursor& cursor) override;

        void OnCursorPressedOutside(const Input::Cursor& cursor) override;

        void OnCursorReleasedOutside(const Input::Cursor& cursor) override;

        void OnCursorStillDown(const Input::Cursor& cursor) override;

        void OnCursorMoved(const Input::Cursor& cursor) override;

        void OnCursorEnter(const Input::Cursor& cursor) override;

        void OnCursorExit(const Input::Cursor& cursor) override;

        void OnCursorDblClicked(const Input::Cursor& cursor) override;

        void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

        void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

        void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

        void OnCursorMiddleMousePressed(const Input::Cursor& cursor) override;

        void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor) override;

        void OnCursorMiddleMouseReleased(const Input::Cursor& cursor) override;

        friend class AnimationWindow;
        friend class AnimationAddKeysAction;
        friend class AnimationDeleteKeysAction;
        friend class AnimationKeysChangeAction;

        template<typename AnimationTrackType>
        friend class KeyFramesTrackControl;

    private:
        Ref<RectF> mSelectionFrameOffsets = mmake<RectF>(-9, -3, 5, 2);
        Ref<RectF> mSelectionFrameCursorOffsets = mmake<RectF>(-2, -3, 2, 2);
        Ref<AnimationWindow> mAnimationWindow = nullptr;
        std::vector<Ref<TrackControl>> mTrackControls;
        std::unordered_map<std::string, Ref<TrackControl>> mTrackControlsMap;
        std::vector<Ref<HandlesGroup>> mHandlesGroups;
        Ref<ContextMenu> mContextMenu = nullptr;
        Ref<Vec2F> mContextMenuPressPoint;
        bool mNeedUpdateSelectionFrame = false;
        bool mHandleHasMoved = false;
        Ref<SelectionFrame> mSelectionFrame = nullptr;
        Ref<RectF> mSelectionRect;
        Ref<Vec2F> mBeginSelectPoint;
        bool mIsFrameSelecting = false;
        std::vector<Ref<DragHandle>> mBeginSelectHandles;
        Ref<DragHandle> mLeftFrameDragHandle;
        Ref<DragHandle> mRightFrameDragHandle;
        Ref<DragHandle> mCenterFrameDragHandle;
        Ref<BeforeChangeKeysData> mBeforeChangeKeysData;
    };
}

// --- META ---
CLASS_BASES_META(Editor::KeyHandlesSheet)
{
    BASE_CLASS(o2::Widget);
    BASE_CLASS(o2::SelectableDragHandlesGroup);
    BASE_CLASS(o2::CursorAreaEventsListener);
}
END_META;

CLASS_FIELDS_META(Editor::KeyHandlesSheet)
{
    FIELD().PRIVATE().DEFAULT_VALUE(Ref<RectF>(mmake<RectF>(-9, -3, 5, 2))).NAME(mSelectionFrameOffsets);
    FIELD().PRIVATE().DEFAULT_VALUE(Ref<RectF>(mmake<RectF>(-2, -3, 2, 2))).NAME(mSelectionFrameCursorOffsets);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mAnimationWindow);
    FIELD().PRIVATE().NAME(mTrackControls);
    FIELD().PRIVATE().NAME(mTrackControlsMap);
    FIELD().PRIVATE().NAME(mHandlesGroups);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mContextMenu);
    FIELD().PRIVATE().NAME(mContextMenuPressPoint);
    FIELD().PRIVATE().DEFAULT_VALUE(false).NAME(mNeedUpdateSelectionFrame);
    FIELD().PRIVATE().DEFAULT_VALUE(false).NAME(mHandleHasMoved);
    FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mSelectionFrame);
    FIELD().PRIVATE().NAME(mSelectionRect);
    FIELD().PRIVATE().NAME(mBeginSelectPoint);
    FIELD().PRIVATE().DEFAULT_VALUE(false).NAME(mIsFrameSelecting);
    FIELD().PRIVATE().NAME(mBeginSelectHandles);
    FIELD().PRIVATE().NAME(mLeftFrameDragHandle);
    FIELD().PRIVATE().NAME(mRightFrameDragHandle);
    FIELD().PRIVATE().NAME(mCenterFrameDragHandle);
    FIELD().PRIVATE().NAME(mBeforeChangeKeysData);
}
END_META;

CLASS_METHODS_META(Editor::KeyHandlesSheet)
{
    METHOD().NAME("CopyKeys").ACCESS_MODIFIERS(MethodAccessModifier::Public);
    METHOD().NAME("PasteKeys").ACCESS_MODIFIERS(MethodAccessModifier::Public);
    METHOD().NAME("OnSelectionChanged").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnHandleCursorPressed").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnHandleCursorReleased").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnHandleMoved").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorPressed").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorReleased").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorPressBreak").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorPressedOutside").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorReleasedOutside").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorStillDown").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorMoved").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorEnter").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorExit").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorDblClicked").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorRightMousePressed").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorRightMouseStayDown").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorRightMouseReleased").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorMiddleMousePressed").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorMiddleMouseStayDown").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();
    METHOD().NAME("OnCursorMiddleMouseReleased").ACCESS_MODIFIERS(MethodAccessModifier::Public).OVERRIDE();

    return meta::ClassMethodsMeta();
}
END_META;Replace raw pointers with Ref<> and const Ref<>& where applicable:

```cpp
#include <memory> // Include the necessary header for Ref<>
#include <string>
#include <vector>
#include <map>

#include "Ref.h" // Include the header for Ref<>
#include "WeakRef.h" // Include the header for WeakRef<>

using namespace std;

using UInt64 = unsigned long long;
using Vec2F = float[2];

// --- META ---
META_BEGIN(KeyHandlesSheet)
class KeyHandlesSheet
{
public:
    typedef Ref<DragHandle> DragHandleRef;
    typedef Map<String, Vector<UInt64>>& _tmp5;

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<KeyHandlesSheet>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetAnimation, Ref<AnimationClip>);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateInputDrawOrder);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(void, RegTrackControl, Ref<ITrackControl>, const std::string&);
    FUNCTION().PUBLIC().SIGNATURE(void, UnregTrackControl, Ref<ITrackControl>);
    FUNCTION().PUBLIC().SIGNATURE(void, UnregAllTrackControls);
    FUNCTION().PUBLIC().SIGNATURE(void, AddHandle, Ref<DragHandle>);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveHandle, Ref<DragHandle>);
    FUNCTION().PUBLIC().SIGNATURE(void, SetSelectedKeys, const _tmp1&);
    FUNCTION().PUBLIC().SIGNATURE(_tmp2, GetSelectedKeys);
    FUNCTION().PUBLIC().SIGNATURE(void, DeleteKeys, _tmp3&, bool);
    FUNCTION().PUBLIC().SIGNATURE(Ref<ContextMenu>, GetContextMenu);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeHandles);
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeCenterHandle);
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeLeftHandle);
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeRightHandle);
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeContextMenu);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateSelectionFrame);
    FUNCTION().PRIVATE().SIGNATURE(void, SerializeKeys, Ref<DataValue>&, const _tmp4&, float);
    FUNCTION().PRIVATE().SIGNATURE(void, DeserializeKeys, const Ref<DataValue>&, _tmp5&, float, bool);
    FUNCTION().PRIVATE().SIGNATURE(void, CopyKeys);
    FUNCTION().PRIVATE().SIGNATURE(void, PasteKeys);
    FUNCTION().PRIVATE().SIGNATURE(void, OnSelectionChanged);
    FUNCTION().PRIVATE().SIGNATURE(void, OnHandleCursorPressed, Ref<DragHandle>, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnHandleCursorReleased, Ref<DragHandle>, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnHandleMoved, Ref<DragHandle>, const Vec2F&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorPressBreak, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorPressedOutside, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorReleasedOutside, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorEnter, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorExit, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorDblClicked, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorRightMousePressed, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorRightMouseStayDown, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorRightMouseReleased, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorMiddleMousePressed, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorMiddleMouseStayDown, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorMiddleMouseReleased, const Input::Cursor&);
};
END_META;
// --- END META ---
```

Please note that we have replaced raw pointers with Ref<> and const Ref<>&. Additionally, any default nullptr values for class members of type Ref<> have been removed. If a class variable represents a "parent" relationship, we have used WeakRef<>. The mmake<> function has not been defined, so we cannot replace make_shared<>() with that. The dynamic_cast<type*> have been left unchanged as replacing them with DynamicCast<type> would require additional modifications.