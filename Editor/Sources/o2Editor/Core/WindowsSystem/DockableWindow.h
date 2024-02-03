#pragma once

#include "o2/Scene/UI/Widgets/Window.h"
#include "o2/Core/Ref.h"
#include "o2/Core/WeakRef.h"
#include "o2/Graphics/Sprite.h"
#include "o2/Scene/UI/Layout.h"
#include "o2/Animation/AnimationPlayer.h"
#include "o2/Input/Cursor.h"
#include "o2/Math/Vec2.h"
#include "o2/Math/Rect.h"
#include "o2/Core/String.h"
#include "o2/Core/WString.h"

using namespace o2;

namespace Editor
{
    class DockWindowPlace;

    // -----------------------------
    // Dockable editor window widget
    // -----------------------------
    class DockableWindow: public o2::Window
    {
    public:
        // Default constructor
        DockableWindow();

        // Copy-constructor
        DockableWindow(const DockableWindow& other);

        // Destructor
        ~DockableWindow();

        // Copy-operator
        DockableWindow& operator=(const DockableWindow& other);

        // Updates drawables, states and widget
        void Update(float dt) override;

        // Draws widget
        void Draw() override;

        // Returns is window docked
        bool IsDocked() const;

        // Return pointer to docking sample sprite
        Ref<Sprite> GetDockingFrameSample() const; 

        // Sets icon sprite
        void SetIcon(Ref<Sprite> icon) override;

        // Returns icon sprite
        Ref<Sprite> GetIcon() const override;

        // Sets icon layer layout
        void SetIconLayout(const Layout& layout) override;

        // Returns icon layer layout
        Layout GetIconLayout() const override;

        // Sets caption text
        void SetCaption(const WString& caption) override;

        // Returns caption text
        WString GetCaption() const override;

        // Sets tab size
        void SetTabWidth(float width);

        // Sets tab width
        float GetTabWidth() const;

        // Sets current window tab as active tab
        void SetTabActive();

        // Returns is this window active and tabbed and tab is active
        bool IsTabActive() const;

        // Sets Automatically calculating tab width when changing caption
        void SetAutoCalcuclatingTabWidth(bool enable);

        // Is Automatically calculating tab width when changing caption
        bool IsAutoCalcuclatingTabWidth() const;

        // Places this into empty dock
        void PlaceDock(Ref<DockWindowPlace> targetDock);

        // Undocks this window, using when window moved out from dock
        void Undock();

        // Updates layout
        void UpdateSelfTransform() override;

        // Returns true if point is under drawable
        bool IsUnderPoint(const Vec2F& point) override;

        // Returns create menu category in editor
        static String GetCreateMenuCategory();

        SERIALIZABLE(DockableWindow);

    protected:
        static const char* mTabLayerPath;
        static const char* mTabIconLayerPath;
        static const char* mTabCaptionLayerPath;
        static const char* mIconLayerPath;
        static const char* mCaptionLayerPath;

        const float mDockSizeCoef = 0.2f;
        const float mDockBorder = 1.5f;

        bool    mDocked = false;               // Is window docked
        Ref<Sprite> mDockingFrameSample; // Docking frame sample @SERIALIZABLE
        RectF   mDockingFrameCurrent;          // Docking sample current frame
        RectF   mDockingFrameTarget;           // Docking sample target  frame

        Ref<AnimationPlayer> mDockingFrameAppearance = mmake<AnimationPlayer>(); // Docking frame appearance tween

        Vec2F mNonDockSize; // Size of widget before docking
        Vec2F mDragOffset;  // Offset from cursor to dragging anchor point

        bool  mTabState = false;             // Is window as tab
        int   mTabPosition = 0;              // Tab index
        bool  mTabActive = false;            // Is active tab
        float mTabWidth = 150.0f;            // Width of tab layer "tab/main"
        bool  mAutoCalculateTabWidth = true; // Automatically calculating tab width when changing caption

    protected:
        // Called when visible was changed
        void OnEnabled() override;

        // Called when visible was changed, undocks
        void OnDisabled() override;

        // Called when widget was selected, enables active tab
        void OnFocused() override;

        //Initialize animation for frame appearance
        void InitializeDockFrameAppearanceAnim();

        // Initializes drag handles
        void InitializeDragHandles();

        // Called when head handle double clicked, trying to dock window
        void OnHeadDblCKicked(const Input::Cursor& cursor);

        // Called when window was moved
        void OnMoved(const Input::Cursor& cursor);

        // Called when window moving was completed
        void OnMoveCompleted(const Input::Cursor&);

        // Called when window going to move
        void OnMoveBegin(const Input::Cursor&);

        // Searches dock place under cursor and returns parameters of them
        bool TraceDock(Ref<DockWindowPlace>& targetDock, Side& dockPosition, RectF& dockZoneRect);

        // Places this into target dock as non line arranged
        void PlaceNonLineDock(Ref<DockWindowPlace> targetDock, Side dockPosition);

        // Places this into target dock as line arranged
        void PlaceLineDock(Ref<DockWindowPlace> targetDock, Side dockPosition, RectF dockZoneRect);

        // Sets window as one of tab
        void SetTabState(float offset, int position, bool isFirst);

        // Sets window regular state, not as tab
        void SetNonTabState();

        // Sets tab as active
        void SetActiveTab();

        // Sets window docked
        void SetDocked(bool docked);

        // Recalculates tab width by caption size
        void RecalculateTabWidth();

        friend class WindowsLayout;
        friend class DockWindowPlace;
    };
}

// --- META ---
CLASS_BASES_META(Editor::DockableWindow)
{
    BASE_CLASS(o2::Window);
}
END_META;
CLASS_FIELDS_META(
    (Ref<Sprite>)mDockingFrameSample,
    (Ref<AnimationPlayer>)mDockingFrameAppearance
)
END_META;#include <memory>

template<typename T>
class Ref {
public:
    Ref(T* ptr) : m_ptr(ptr) {}
    ~Ref() {
        if (m_ptr) {
            delete m_ptr;
        }
    }
    T* get() {
        return m_ptr;
    }
private:
    T* m_ptr;
};

template<typename T>
class WeakRef {
public:
    WeakRef(T* ptr) : m_ptr(ptr) {}
    ~WeakRef() {
        m_ptr = nullptr;
    }
    T* get() {
        return m_ptr;
    }
private:
    T* m_ptr;
};

template<typename T>
Ref<T> mmake() {
    return Ref<T>(new T());
}

template<typename T>
class DynamicCast {
public:
    static T cast(const Ref<void>& ptr) {
        return dynamic_cast<T>(ptr.get());
    }
};

class DockableWindow {
    FIELD().PROTECTED().DEFAULT_VALUE(0.2f).NAME(mDockSizeCoef);
    FIELD().PROTECTED().DEFAULT_VALUE(1.5f).NAME(mDockBorder);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mDocked);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mDockingFrameSample);
    FIELD().PROTECTED().NAME(mDockingFrameCurrent);
    FIELD().PROTECTED().NAME(mDockingFrameTarget);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<AnimationPlayer>()).NAME(mDockingFrameAppearance);
    FIELD().PROTECTED().NAME(mNonDockSize);
    FIELD().PROTECTED().NAME(mDragOffset);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mTabState);
    FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mTabPosition);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mTabActive);
    FIELD().PROTECTED().DEFAULT_VALUE(150.0f).NAME(mTabWidth);
    FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mAutoCalculateTabWidth);

public:
    DockableWindow() {}

    DockableWindow(const DockableWindow&) {}

    void Update(float) {}

    void Draw() {}

    bool IsDocked() {}

    Sprite* GetDockingFrameSample() {}

    void SetIcon(Sprite*) {}

    Sprite* GetIcon() {}

    void SetIconLayout(const Layout&) {}

    Layout GetIconLayout() {}

    void SetIconLayout(const WString&) {}

    WString GetCaption() {}

    void SetTabWidth(float) {}

    float GetTabWidth() {}

    void SetTabActive() {}

    bool IsTabActive() {}

    void SetAutoCalcuclatingTabWidth(bool) {}

    bool IsAutoCalcuclatingTabWidth() {}

    void PlaceDock(DockWindowPlace*) {}

    void Undock() {}

    void UpdateSelfTransform() {}

    bool IsUnderPoint(const Vec2F&) {}

    static String GetCreateMenuCategory() {}

protected:
    void OnEnabled() {}

    void OnDisabled() {}

    void OnFocused() {}

    void InitializeDockFrameAppearanceAnim() {}

    void InitializeDragHandles() {}

    void OnHeadDblCKicked(const Input::Cursor&) {}

    void OnMoved(const Input::Cursor&) {}

    void OnMoveCompleted(const Input::Cursor&) {}

    void OnMoveBegin(const Input::Cursor&) {}

    bool TraceDock(DockWindowPlace*&, Side&, RectF&) {}

    void PlaceNonLineDock(DockWindowPlace*, Side) {}

    void PlaceLineDock(DockWindowPlace*, Side, RectF) {}

    void SetTabState(float, int, bool) {}

    void SetNonTabState() {}

    void SetActiveTab() {}

    void SetDocked(bool) {}

    void RecalculateTabWidth() {}

};

CLASS_METHODS_META(Editor::DockableWindow) {
    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<DockableWindow>&);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsDocked);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Sprite>, GetDockingFrameSample);
    FUNCTION().PUBLIC().SIGNATURE(void, SetIcon, Ref<Sprite>);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Sprite>, GetIcon);
    FUNCTION().PUBLIC().SIGNATURE(void, SetIconLayout, const Ref<Layout>&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Layout>, GetIconLayout);
    FUNCTION().PUBLIC().SIGNATURE(void, SetIconLayout, const Ref<WString>&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<WString>, GetCaption);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTabWidth, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetTabWidth);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTabActive);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsTabActive);
    FUNCTION().PUBLIC().SIGNATURE(void, SetAutoCalcuclatingTabWidth, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsAutoCalcuclatingTabWidth);
    FUNCTION().PUBLIC().SIGNATURE(void, PlaceDock, const Ref<DockWindowPlace>&);
    FUNCTION().PUBLIC().SIGNATURE(void, Undock);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateSelfTransform);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Ref<Vec2F>&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Ref<String>, GetCreateMenuCategory);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnFocused);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeDockFrameAppearanceAnim);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeDragHandles);
    FUNCTION().PROTECTED().SIGNATURE(void, OnHeadDblCKicked, const Ref<Input::Cursor>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMoved, const Ref<Input::Cursor>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMoveCompleted, const Ref<Input::Cursor>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMoveBegin, const Ref<Input::Cursor>&);
    FUNCTION().PROTECTED().SIGNATURE(bool, TraceDock, const Ref<DockWindowPlace>&, Ref<Side>&, Ref<RectF>&);
    FUNCTION().PROTECTED().SIGNATURE(void, PlaceNonLineDock, const Ref<DockWindowPlace>&, Ref<Side>);
    FUNCTION().PROTECTED().SIGNATURE(void, PlaceLineDock, const Ref<DockWindowPlace>&, Ref<Side>, const Ref<RectF>&);
    FUNCTION().PROTECTED().SIGNATURE(void, SetTabState, float, int, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, SetNonTabState);
    FUNCTION().PROTECTED().SIGNATURE(void, SetActiveTab);
    FUNCTION().PROTECTED().SIGNATURE(void, SetDocked, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, RecalculateTabWidth);
};