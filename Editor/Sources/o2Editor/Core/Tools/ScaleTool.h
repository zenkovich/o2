#pragma once

#include "o2/Utils/Math/Basis.h"
#include "o2Editor/Core/Tools/SelectionTool.h"
#include "o2Editor/SceneWindow/SceneDragHandle.h"
#include "o2/Utils/Ref.h"

namespace Editor
{
    class TransformAction;

    // -------------------------
    // Scale objects editor tool
    // -------------------------
    class ScaleTool : public SelectionTool
    {
    public:
        float bothScaleSence = 0.01f;

    public:
        // Default constructor
        ScaleTool();

        // Destructor
        ~ScaleTool();

        IOBJECT(ScaleTool);

    protected:
        SceneDragHandle mHorDragHandle;  // Horizontal scale drag handle
        SceneDragHandle mVerDragHandle;  // Vertical scale drag handle
        SceneDragHandle mBothDragHandle; // Bot axis scale drag handle

        float mHandlesAngle = 0.0f;			  // Handles angle in radians
        Vec2F mSceneHandlesPos;				  // Scene space handles position
        Vec2F mHandlesSize = Vec2F(100, 100); // Handles size in screen space

        Vec2F mLastHorHandlePos;  // Last horizontal handle position
        Vec2F mLastVerHandlePos;  // Last vertical handle position
        Vec2F mLastBothHandlePos; // Last both axis handle position

        Vector<Basis> mBeforeTransforms;		 // Array of objects' transformations before changing
        TransformAction* mTransformAction = nullptr; // Current transform action. Creates when transform started

    protected:
        // Returns toggle in menu panel icon name
        String GetPanelIcon() const override;

        // Returns shortcut keys for toggle
        ShortcutKeys GetShortcut() const override;

        // Updates tool
        void Update(float dt) override;

        // Draws screen
        void DrawScreen() override;

        // Called when tool was enabled
        void OnEnabled() override;

        // Called when tool was disabled
        void OnDisabled() override;

        // Called when scene objects was changed
        void OnSceneChanged(const Ref<Vector<SceneEditableObject*>>& changedObjects) override;

        // Called when objects selection was changed
        void OnObjectsSelectionChanged(const Ref<Vector<SceneEditableObject*>>& objects) override;

        // Called when horizontal drag handle was moved
        void OnHorDragHandleMoved(const Ref<Vec2F>& position);

        // Called when horizontal drag handle was moved
        void OnVerDragHandleMoved(const Ref<Vec2F>& position);

        // Called when horizontal drag handle was moved
        void OnBothDragHandleMoved(const Ref<Vec2F>& position);

        // Updates handles position
        void UpdateHandlesPosition();

        // Updates handles angle and position
        void UpdateHandlesAngleAndPositions(float angle);

        // Updates handles position
        void UpdateHandlesPositions();

        // Called when key was pressed
        void OnKeyPressed(const Ref<Input::Key>& key) override;

        // Called when key stay down during frame
        void OnKeyStayDown(const Ref<Input::Key>& key) override;

        // Called when key was pressed
        void OnKeyReleased(const Ref<Input::Key>& key) override;

        // Moves selected objects on delta
        void ScaleSelectedObjects(const Ref<Vec2F>& scale);

        // Called when some handle was pressed, stores before transformations
        void HandlePressed();

        // Called when handle was released, completes transformation action
        void HandleReleased();
    };

}

// --- META ---

CLASS_BASES_META(Editor::ScaleTool)
{
    BASE_CLASS(Editor::SelectionTool);
}
END_META;

CLASS_FIELDS_META(Editor::ScaleTool)
{
    FIELD().PUBLIC().DEFAULT_VALUE(0.01f).NAME(bothScaleSence);
    FIELD().PROTECTED().NAME(mHorDragHandle);
    FIELD().PROTECTED().NAME(mVerDragHandle);
    FIELD().PROTECTED().NAME(mBothDragHandle);
    FIELD().PROTECTED().DEFAULT_VALUE(0.0f).NAME(mHandlesAngle);
    FIELD().PROTECTED().NAME(mSceneHandlesPos);
    FIELD().PROTECTED().DEFAULT_VALUE(Vec2F(100, 100)).NAME(mHandlesSize);
    FIELD().PROTECTED().NAME(mLastHorHandlePos);
    FIELD().PROTECTED().NAME(mLastVerHandlePos);
    FIELD().PROTECTED().NAME(mLastBothHandlePos);
    FIELD().PROTECTED().NAME(mBeforeTransforms);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTransformAction);
}
END_META;

CLASS_METHODS_META(Editor::ScaleTool)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PROTECTED().SIGNATURE(String, GetPanelIcon);
    FUNCTION().PROTECTED().SIGNATURE(ShortcutKeys, GetShortcut);
    FUNCTION().PROTECTED().SIGNATURE(void, Update, const Ref<float>&);
    FUNCTION().PROTECTED().SIGNATURE(void, DrawScreen);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSceneChanged, const Ref<Vector<SceneEditableObject*>>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnObjectsSelectionChanged, const Ref<Vector<SceneEditableObject*>>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnHorDragHandleMoved, const Ref<Vec2F>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnVerDragHandleMoved, const Ref<Vec2F>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnBothDragHandleMoved, const Ref<Vec2F>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyPressed, const Ref<Input::Key>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyStayDown, const Ref<Input::Key>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyReleased, const Ref<Input::Key>&);
}
END_META;#include <memory>

template<typename T>
class Ref
{
public:
    Ref() noexcept : m_ptr(nullptr) {}
    explicit Ref(T* ptr) noexcept : m_ptr(ptr) {}
    Ref(const Ref& other) noexcept : m_ptr(other.m_ptr) {}
    Ref& operator=(const Ref& other) noexcept { m_ptr = other.m_ptr; return *this; }
    Ref(Ref&& other) noexcept : m_ptr(std::exchange(other.m_ptr, nullptr)) {}
    Ref& operator=(Ref&& other) noexcept { std::swap(m_ptr, other.m_ptr); return *this; }
    ~Ref() noexcept { delete m_ptr; }

    T* operator->() const noexcept { return m_ptr; }
    T& operator*() const noexcept { return *m_ptr; }

    explicit operator bool() const noexcept { return m_ptr != nullptr; }
    bool operator==(const Ref& other) const noexcept { return m_ptr == other.m_ptr; }
    bool operator!=(const Ref& other) const noexcept { return !(*this == other); }

private:
    T* m_ptr;
};

template<typename T>
Ref<T> mmake()
{
    return Ref<T>(new T());
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return Ref<T>(dynamic_cast<T*>(ptr.operator->()));
}

template<typename T>
class WeakRef
{
public:
    WeakRef() noexcept : m_ptr(nullptr) {}
    explicit WeakRef(T* ptr) noexcept : m_ptr(ptr), m_weakCount(new unsigned int(1)) {}
    WeakRef(const WeakRef& other) noexcept : m_ptr(other.m_ptr), m_weakCount(other.m_weakCount) { (*m_weakCount)++; }
    WeakRef& operator=(const WeakRef& other) noexcept
    {
        if (this != &other)
        {
            m_ptr = other.m_ptr;
            m_weakCount = other.m_weakCount;
            (*m_weakCount)++;
        }
        return *this;
    }
    WeakRef(WeakRef&& other) noexcept : m_ptr(std::exchange(other.m_ptr, nullptr)), m_weakCount(std::exchange(other.m_weakCount, nullptr)) {}
    WeakRef& operator=(WeakRef&& other) noexcept
    {
        std::swap(m_ptr, other.m_ptr);
        std::swap(m_weakCount, other.m_weakCount);
        return *this;
    }
    ~WeakRef() noexcept
    {
        if (m_weakCount != nullptr && --(*m_weakCount) == 0)
        {
            delete m_weakCount;
            delete m_ptr;
        }
    }

    Ref<T> Lock() const noexcept
    {
        return Ref<T>(m_ptr);
    }

private:
    T* m_ptr;
    unsigned int* m_weakCount;
};

class Input
{
public:
    class Key {};
};

class Vec2F {};

class Class
{
public:
    Class() noexcept = default;
    Class(const Ref<Class>& parent) noexcept : m_parent(parent) {}

    void HandleMoved(const Vec2F&) {}
    void UpdateHandlesPosition() {}
    void UpdateHandlesAngleAndPositions(float) {}
    void UpdateHandlesPositions() {}
    void OnKeyPressed(const Input::Key&) {}
    void OnKeyStayDown(const Input::Key&) {}
    void OnKeyReleased(const Input::Key&) {}
    void ScaleSelectedObjects(const Vec2F&) {}
    void HandlePressed() {}
    void HandleReleased() {}

private:
    WeakRef<Class> m_parent;
};

// --- META ---
BEGIN_META(Class)
{
    FUNCTION().PROTECTED().SIGNATURE(void, HandleMoved, const Vec2F&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateHandlesPosition);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateHandlesAngleAndPositions, float);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateHandlesPositions);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyPressed, const Input::Key&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyStayDown, const Input::Key&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyReleased, const Input::Key&);
    FUNCTION().PROTECTED().SIGNATURE(void, ScaleSelectedObjects, const Vec2F&);
    FUNCTION().PROTECTED().SIGNATURE(void, HandlePressed);
    FUNCTION().PROTECTED().SIGNATURE(void, HandleReleased);
}
END_META;
// --- END META ---