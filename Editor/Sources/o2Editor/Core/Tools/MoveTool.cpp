#include "o2Editor/stdafx.h"
#include "MoveTool.h"

#include "o2/Render/Sprite.h"
#include "o2/Utils/Editor/SceneEditableObject.h"
#include "o2Editor/Core/Actions/Transform.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/SceneHierarchyTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

namespace Editor
{
	MoveTool::MoveTool()
	{
		mHorDragHandle = Ref<SceneDragHandle>(mnew SceneDragHandle(mnew Sprite("ui/UI2_right_move_arrow.png"),
										 mnew Sprite("ui/UI2_right_move_arrow_select.png"),
										 mnew Sprite("ui/UI2_right_move_arrow_pressed.png")));

		mVerDragHandle = Ref<SceneDragHandle>(mnew SceneDragHandle(mnew Sprite("ui/UI2_up_move_arrow.png"),
										 mnew Sprite("ui/UI2_up_move_arrow_select.png"),
										 mnew Sprite("ui/UI2_up_move_arrow_pressed.png")));

		mBothDragHandle = Ref<SceneDragHandle>(mnew SceneDragHandle(mnew Sprite("ui/UI2_move_tool_center.png"),
										  mnew Sprite("ui/UI2_move_tool_center_select.png"),
										  mnew Sprite("ui/UI2_move_tool_center_pressed.png")));

		mHorDragHandle->enabled = false;
		mVerDragHandle->enabled = false;
		mBothDragHandle->enabled = false;

		mHorDragHandle->onChangedPos = THIS_FUNC(OnHorDragHandleMoved);
		mVerDragHandle->onChangedPos = THIS_FUNC(OnVerDragHandleMoved);
		mBothDragHandle->onChangedPos = THIS_FUNC(OnBothDragHandleMoved);

		mHorDragHandle->onPressed = THIS_FUNC(HandlePressed);
		mVerDragHandle->onPressed = THIS_FUNC(HandlePressed);
		mBothDragHandle->onPressed = THIS_FUNC(HandlePressed);

		mHorDragHandle->onReleased = THIS_FUNC(HandleReleased);
		mVerDragHandle->onReleased = THIS_FUNC(HandleReleased);
		mBothDragHandle->onReleased = THIS_FUNC(HandleReleased);

		mHorDragHandle->GetRegularDrawable()->SetSizePivot(Vec2F(1, 5));
		mHorDragHandle->GetHoverDrawable()->SetSizePivot(Vec2F(1, 5));
		mHorDragHandle->GetPressedDrawable()->SetSizePivot(Vec2F(1, 5));

		mVerDragHandle->GetRegularDrawable()->SetSizePivot(Vec2F(5, 1));
		mVerDragHandle->GetHoverDrawable()->SetSizePivot(Vec2F(5, 1));
		mVerDragHandle->GetPressedDrawable()->SetSizePivot(Vec2F(5, 1));

		mBothDragHandle->GetRegularDrawable()->SetSizePivot(Vec2F(1, 1));
		mBothDragHandle->GetHoverDrawable()->SetSizePivot(Vec2F(1, 1));
		mBothDragHandle->GetPressedDrawable()->SetSizePivot(Vec2F(1, 1));
	}

	MoveTool::~MoveTool()
	{}

	void MoveTool::Update(float dt)
	{}

	void MoveTool::OnEnabled()
	{
		mHorDragHandle->enabled = true;
		mVerDragHandle->enabled = true;
		mBothDragHandle->enabled = true;
		UpdateHandlesPosition();
	}

	void MoveTool::OnDisabled()
	{
		mHorDragHandle->enabled = false;
		mVerDragHandle->enabled = false;
		mBothDragHandle->enabled = false;
	}

	void MoveTool::OnSceneChanged(Vector<Ref<SceneEditableObject>>& changedObjects)
	{
		UpdateHandlesPosition();
	}

	void MoveTool::OnObjectsSelectionChanged(Vector<Ref<SceneEditableObject>>& objects)
	{
		UpdateHandlesPosition();
	}

	void MoveTool::OnHorDragHandleMoved(const Vec2F& position)
	{
		Vec2F axis = Vec2F::Rotated(mHandlesAngle);
		Vec2F delta = position - mLastSceneHandlesPos;
		Vec2F axisDelta = delta.Project(axis);

		HandlesMoved(axisDelta, o2Input.IsKeyDown(VK_SHIFT), false);
	}

	void MoveTool::OnVerDragHandleMoved(const Vec2F& position)
	{
		Vec2F axis = Vec2F::Rotated(mHandlesAngle + Math::PI()*0.5f);
		Vec2F delta = position - mLastSceneHandlesPos;
		Vec2F axisDelta = delta.Project(axis);

		HandlesMoved(axisDelta, false, o2Input.IsKeyDown(VK_SHIFT));
	}

	void MoveTool::OnBothDragHandleMoved(const Vec2F& position)
	{
		bool snap = o2Input.IsKeyDown(VK_SHIFT);
		HandlesMoved(position - mLastSceneHandlesPos, snap, snap);
	}

	void MoveTool::HandlePressed()
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedObjects().Convert<Basis>(
			[](Ref<SceneEditableObject>& x) { return x->GetTransform(); });

		mTransformAction = mnew TransformAction(o2EditorSceneScreen.GetTopSelectedObjects());
	}

	void MoveTool::HandleReleased()
	{
		mTransformAction->Completed();
		o2EditorApplication.DoneAction(mTransformAction);
		mTransformAction = nullptr;
	}

	void MoveTool::HandlesMoved(const Vec2F& delta, bool snapHor /*= false*/, bool spanVer /*= false*/)
	{
		if (spanVer || snapHor)
		{
			mSnapPosition = mLastSceneHandlesPos + delta;
			Vec2F roundedSnap(snapHor ? Math::Round(mSnapPosition.x / snapStep)*snapStep : mSnapPosition.x,
							  spanVer ? Math::Round(mSnapPosition.y / snapStep)*snapStep : mSnapPosition.y);

			if ((roundedSnap - mLastSceneHandlesPos).Length() > FLT_EPSILON)
			{
				Vec2F roundDelta = roundedSnap - mLastSceneHandlesPos;
				mLastSceneHandlesPos = roundedSnap;
				MoveSelectedObjects(roundDelta);
			}

			mHorDragHandle->position = mLastSceneHandlesPos;
			mVerDragHandle->position = mLastSceneHandlesPos;
			mBothDragHandle->position = mLastSceneHandlesPos;
		}
		else
		{
			Vec2F newHandlesPos = mLastSceneHandlesPos + delta;
			mLastSceneHandlesPos = newHandlesPos;
			mHorDragHandle->position = newHandlesPos;
			mVerDragHandle->position = newHandlesPos;
			mBothDragHandle->position = newHandlesPos;

			MoveSelectedObjects(roundDelta);
		}
	}

    void MoveTool::MoveSelectedObjects(const Vec2F& delta)
    {
        Vector<Ref<SceneEditableObject>>& selectedObjects = o2EditorSceneScreen.GetTopSelectedObjects();

        for (Ref<SceneEditableObject>& obj : selectedObjects)
        {
            obj->MoveBy(delta);
        }
    }
}#include <memory>
#include <type_traits>

template <typename T>
class Ref
{
public:
    Ref() : mPtr(nullptr) {}

    explicit Ref(T* ptr) : mPtr(ptr) {}

    template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value, U>::type>
    Ref(const Ref<U>& ref) : mPtr(ref.get()) {}

    template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value, U>::type>
    Ref& operator=(const Ref<U>& ref)
    {
        mPtr = ref.get();
        return *this;
    }

    Ref(const Ref& ref) : mPtr(ref.mPtr) {}

    Ref& operator=(const Ref& ref)
    {
        mPtr = ref.mPtr;
        return *this;
    }

    T* get() const { return mPtr; }

    T* operator->() const { return mPtr; }

    operator bool() const { return mPtr != nullptr; }

private:
    T* mPtr;
};

template <typename T>
class WeakRef
{
public:
    WeakRef() : mPtr(nullptr) {}

    explicit WeakRef(T* ptr) : mPtr(ptr) {}

    template<typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value, U>::type>
    WeakRef(const Ref<U>& ref) : mPtr(ref.get()) {}

    WeakRef(const WeakRef& ref) : mPtr(ref.mPtr) {}

    WeakRef& operator=(const WeakRef& ref)
    {
        mPtr = ref.mPtr;
        return *this;
    }

    WeakRef& operator=(const Ref<T>& ref)
    {
        mPtr = ref.get();
        return *this;
    }

    T* get() const { return mPtr; }

    T* operator->() const { return mPtr; }

    operator bool() const { return mPtr != nullptr; }

private:
    T* mPtr;
};

template <typename T, typename... Args>
Ref<T> make(const Args&... args)
{
    return Ref<T>(new T(args...));
}

#include <utility>

template <typename Base, typename Derived>
Ref<Base> DynamicCast(const Ref<Derived>& ref)
{
    return Ref<Base>(dynamic_cast<Base*>(ref.get()));
}

namespace Input {
    enum Key {
        VK_LEFT,
        VK_RIGHT,
        VK_UP,
        VK_DOWN
    };
}

namespace Editor {

class MoveTool : public SelectionTool
{
public:
    MoveTool()
        : mTransformAction(nullptr)
    {
        mVerDragHandle = mmake<Handle>();
        mHorDragHandle = mmake<Handle>();
        mBothDragHandle = mmake<Handle>();
    }

    ~MoveTool()
    {
        // mVerDragHandle, mHorDragHandle, mBothDragHandle will be deleted automatically
    }

    void OnActivated() override {}

    void OnDeactivated() override {}

    void RenderHandles() override {}

    void Update() override {}

    void OnSceneObjectDeleted(SceneEditableObject* /*object*/) override {}

    void OnSceneObjectSelected(SceneEditableObject* /*object*/) override {}

    void OnSceneObjectDeselected(SceneEditableObject* /*object*/) override {}

    void OnSceneObjectAdded(SceneEditableObject* /*object*/) override {}

    void OnMouseDoubleClick(const Vec2F& /*position*/, uint32_t /*mouseIndex*/) override {}

    void OnMouseMove(const Vec2F& /*position*/, uint32_t /*mouseIndex*/) override {}

    void OnMouseButtonDown(const Vec2F& /*position*/, uint32_t /*buttonIndex*/, uint32_t /*mouseIndex*/) override {}

    void OnMouseButtonUp(const Vec2F& /*position*/, uint32_t /*buttonIndex*/, uint32_t /*mouseIndex*/) override {}

    void MoveSelectedObjects(const Vec2F& delta)
    {
        auto selectedObjects = o2EditorSceneScreen.GetTopSelectedObjects();
        for (const Ref<SceneEditableObject>& object : selectedObjects)
        {
            Basis basis = object->GetTransform();
            basis.origin += delta;
            object->SetTransform(basis);
            object->UpdateTransform();
        }

        UpdateHandlesPosition();
    }

    void MoveSelectedObjectsWithAction(const Vec2F& delta)
    {
        mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedObjects().Convert<Basis>(
            [](const Ref<SceneEditableObject>& x) { return x->GetTransform(); });

        mTransformAction = make<TransformAction>(o2EditorSceneScreen.GetTopSelectedObjects());

        MoveSelectedObjects(delta);

        mTransformAction->Completed();
        o2EditorApplication.DoneAction(mTransformAction);
        mTransformAction = nullptr;
    }

    void OnKeyPressed(const Input::Key& key)
    {
        if (!o2EditorTree.IsTreeFocused())
            return;

        float delta = o2Input.IsKeyDown(VK_SHIFT) ? snapStep : 1.0f;

        if (key == Input::VK_LEFT)
            MoveSelectedObjectsWithAction(Vec2F::Left() * delta);

        if (key == Input::VK_RIGHT)
            MoveSelectedObjectsWithAction(Vec2F::Right() * delta);

        if (key == Input::VK_UP)
            MoveSelectedObjectsWithAction(Vec2F::Up() * delta);

        if (key == Input::VK_DOWN)
            MoveSelectedObjectsWithAction(Vec2F::Down() * delta);

        if (key == Input::VK_CONTROL)
        {
            mHandlesAngle = 0.0f;
            mVerDragHandle->angle = mHandlesAngle;
            mHorDragHandle->angle = mHandlesAngle;
            mBothDragHandle->angle = mHandlesAngle;
        }

        SelectionTool::OnKeyPressed(key);
    }

    void OnKeyStayDown(const Input::Key& key)
    {
        if (!o2EditorTree.IsTreeFocused())
            return;

        float delta = o2Input.IsKeyDown(VK_SHIFT) ? snapStep : 1.0f;

        if (key.pressedTime < 0.3f)
            return;

        if (key == Input::VK_LEFT)
            MoveSelectedObjectsWithAction(Vec2F::Left() * delta);

        if (key == Input::VK_RIGHT)
            MoveSelectedObjectsWithAction(Vec2F::Right() * delta);

        if (key == Input::VK_UP)
            MoveSelectedObjectsWithAction(Vec2F::Up() * delta);

        if (key == Input::VK_DOWN)
            MoveSelectedObjectsWithAction(Vec2F::Down() * delta);
    }

    void OnKeyReleased(const Input::Key& key)
    {
        if (key == Input::VK_CONTROL)
        {
            auto selectedObjects = o2EditorSceneScreen.GetSelectedObjects();
            if (selectedObjects.Count() > 0)
            {
                SceneEditableObject* lastSelectedObject = selectedObjects.Last().get();
                mHandlesAngle = -lastSelectedObject->GetTransform().xv.Normalized().Angle(Vec2F::Right());

                mVerDragHandle->angle = mHandlesAngle;
                mHorDragHandle->angle = mHandlesAngle;
                mBothDragHandle->angle = mHandlesAngle;
            }
        }
    }

    String GetPanelIcon() const override
    {
        return "ui/UI4_move_tool.png";
    }

    ShortcutKeys GetShortcut() const override
    {
        return ShortcutKeys('W');
    }

private:
    void UpdateHandlesPosition()
    {
        auto selectedObjects = o2EditorSceneScreen.GetSelectedObjects();
        mLastSceneHandlesPos =
            selectedObjects.Sum<Vec2F>([](const Ref<SceneEditableObject>& x) { return x->GetPivot(); }) /
            static_cast<float>(selectedObjects.Count());

        mVerDragHandle->position = mLastSceneHandlesPos;
        mHorDragHandle->position = mLastSceneHandlesPos;
        mBothDragHandle->position = mLastSceneHandlesPos;

        if (selectedObjects.Count() > 0 && !o2Input.IsKeyDown(Input::VK_CONTROL))
        {
            SceneEditableObject* lastSelectedObject = selectedObjects.Last().get();
            mHandlesAngle = -lastSelectedObject->GetTransform().xv.Normalized().Angle(Vec2F::Right());

            mVerDragHandle->angle = mHandlesAngle;
            mHorDragHandle->angle = mHandlesAngle;
            mBothDragHandle->angle = mHandlesAngle;
        }
        else
        {
            mHandlesAngle = 0.0f;
            mVerDragHandle->angle = mHandlesAngle;
            mHorDragHandle->angle = mHandlesAngle;
            mBothDragHandle->angle = mHandlesAngle;
        }
    }

    Ref<Handle> mVerDragHandle;
    Ref<Handle> mHorDragHandle;
    Ref<Handle> mBothDragHandle;

    float mHandlesAngle;
    Vec2F mLastSceneHandlesPos;

    TransformAction* mTransformAction;
    Vector<Basis> mBeforeTransforms;
};
}
// --- META ---

DECLARE_CLASS(Editor::MoveTool, Editor__MoveTool);
// --- END META ---