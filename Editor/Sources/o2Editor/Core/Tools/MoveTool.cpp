#include "o2Editor/stdafx.h"
#include "MoveTool.h"

#include "o2Editor/Core/Actions/Transform.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2/Render/Sprite.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/SceneTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"
#include "o2/Utils/Editor/SceneEditableObject.h"

namespace Editor
{
	MoveTool::MoveTool()
	{
		mHorDragHandle = SceneDragHandle(mnew Sprite("ui/UI2_right_move_arrow.png"),
										 mnew Sprite("ui/UI2_right_move_arrow_select.png"),
										 mnew Sprite("ui/UI2_right_move_arrow_pressed.png"));

		mVerDragHandle = SceneDragHandle(mnew Sprite("ui/UI2_up_move_arrow.png"),
										 mnew Sprite("ui/UI2_up_move_arrow_select.png"),
										 mnew Sprite("ui/UI2_up_move_arrow_pressed.png"));

		mBothDragHandle = SceneDragHandle(mnew Sprite("ui/UI2_move_tool_center.png"),
										  mnew Sprite("ui/UI2_move_tool_center_select.png"),
										  mnew Sprite("ui/UI2_move_tool_center_pressed.png"));

		mHorDragHandle.enabled = false;
		mVerDragHandle.enabled = false;
		mBothDragHandle.enabled = false;

		mHorDragHandle.onChangedPos = THIS_FUNC(OnHorDragHandleMoved);
		mVerDragHandle.onChangedPos = THIS_FUNC(OnVerDragHandleMoved);
		mBothDragHandle.onChangedPos = THIS_FUNC(OnBothDragHandleMoved);

		mHorDragHandle.onPressed = THIS_FUNC(HandlePressed);
		mVerDragHandle.onPressed = THIS_FUNC(HandlePressed);
		mBothDragHandle.onPressed = THIS_FUNC(HandlePressed);

		mHorDragHandle.onReleased = THIS_FUNC(HandleReleased);
		mVerDragHandle.onReleased = THIS_FUNC(HandleReleased);
		mBothDragHandle.onReleased = THIS_FUNC(HandleReleased);

		mHorDragHandle.GetRegularSprite()->SetSizePivot(Vec2F(1, 5));
		mHorDragHandle.GetHoverSprite()->SetSizePivot(Vec2F(1, 5));
		mHorDragHandle.GetPressedSprite()->SetSizePivot(Vec2F(1, 5));

		mVerDragHandle.GetRegularSprite()->SetSizePivot(Vec2F(5, 1));
		mVerDragHandle.GetHoverSprite()->SetSizePivot(Vec2F(5, 1));
		mVerDragHandle.GetPressedSprite()->SetSizePivot(Vec2F(5, 1));

		mBothDragHandle.GetRegularSprite()->SetSizePivot(Vec2F(1, 1));
		mBothDragHandle.GetHoverSprite()->SetSizePivot(Vec2F(1, 1));
		mBothDragHandle.GetPressedSprite()->SetSizePivot(Vec2F(1, 1));
	}

	MoveTool::~MoveTool()
	{}

	void MoveTool::Update(float dt)
	{}

	void MoveTool::OnEnabled()
	{
		mHorDragHandle.enabled = true;
		mVerDragHandle.enabled = true;
		mBothDragHandle.enabled = true;
		UpdateHandlesPosition();
	}

	void MoveTool::OnDisabled()
	{
		mHorDragHandle.enabled = false;
		mVerDragHandle.enabled = false;
		mBothDragHandle.enabled = false;
	}

	void MoveTool::OnSceneChanged(Vector<SceneEditableObject*> changedObjects)
	{
		UpdateHandlesPosition();
	}

	void MoveTool::OnObjectsSelectionChanged(Vector<SceneEditableObject*> objects)
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
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedObjects().Select<Basis>(
			[](SceneEditableObject* x) { return x->GetTransform(); });

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

			mHorDragHandle.position = mLastSceneHandlesPos;
			mVerDragHandle.position = mLastSceneHandlesPos;
			mBothDragHandle.position = mLastSceneHandlesPos;
		}
		else
		{
			Vec2F newHandlesPos = mLastSceneHandlesPos + delta;
			mLastSceneHandlesPos = newHandlesPos;
			mHorDragHandle.position = newHandlesPos;
			mVerDragHandle.position = newHandlesPos;
			mBothDragHandle.position = newHandlesPos;

			MoveSelectedObjects(delta);
		}
	}

	void MoveTool::UpdateHandlesPosition()
	{
		auto selectedObjects = o2EditorSceneScreen.GetSelectedObjects();
		mLastSceneHandlesPos =
			selectedObjects.Sum<Vec2F>([](auto x) { return x->GetTransform().origin; }) /
			(float)selectedObjects.Count();

		mVerDragHandle.position = mLastSceneHandlesPos;
		mHorDragHandle.position = mLastSceneHandlesPos;
		mBothDragHandle.position = mLastSceneHandlesPos;

		if (selectedObjects.Count() > 0 && !o2Input.IsKeyDown(VK_CONTROL))
		{
			SceneEditableObject* lastSelectedObject = selectedObjects.Last();
			mHandlesAngle = -lastSelectedObject->GetTransform().xv.Normalized().Angle(Vec2F::Right());

			mVerDragHandle.angle = mHandlesAngle;
			mHorDragHandle.angle = mHandlesAngle;
			mBothDragHandle.angle = mHandlesAngle;
		}
		else
		{
			mHandlesAngle = 0.0f;
			mVerDragHandle.angle = mHandlesAngle;
			mHorDragHandle.angle = mHandlesAngle;
			mBothDragHandle.angle = mHandlesAngle;
		}
	}

	void MoveTool::OnKeyPressed(const Input::Key& key)
	{
		if (!o2EditorTree.GetSceneTree()->IsFocused())
			return;

		float delta = o2Input.IsKeyDown(VK_SHIFT) ? snapStep : 1.0f;

		if (key == VK_LEFT)
			MoveSelectedObjectsWithAction(Vec2F::Left()*delta);

		if (key == VK_RIGHT)
			MoveSelectedObjectsWithAction(Vec2F::Right()*delta);

		if (key == VK_UP)
			MoveSelectedObjectsWithAction(Vec2F::Up()*delta);

		if (key == VK_DOWN)
			MoveSelectedObjectsWithAction(Vec2F::Down()*delta);

		if (key == VK_CONTROL)
		{
			mHandlesAngle = 0.0f;
			mVerDragHandle.angle = mHandlesAngle;
			mHorDragHandle.angle = mHandlesAngle;
			mBothDragHandle.angle = mHandlesAngle;
		}

		SelectionTool::OnKeyPressed(key);
	}

	void MoveTool::OnKeyStayDown(const Input::Key& key)
	{
		if (!o2EditorTree.GetSceneTree()->IsFocused())
			return;

		float delta = o2Input.IsKeyDown(VK_SHIFT) ? snapStep : 1.0f;

		if (key.pressedTime < 0.3f)
			return;

		if (key == VK_LEFT)
			MoveSelectedObjectsWithAction(Vec2F::Left()*delta);

		if (key == VK_RIGHT)
			MoveSelectedObjectsWithAction(Vec2F::Right()*delta);

		if (key == VK_UP)
			MoveSelectedObjectsWithAction(Vec2F::Up()*delta);

		if (key == VK_DOWN)
			MoveSelectedObjectsWithAction(Vec2F::Down()*delta);
	}

	void MoveTool::OnKeyReleased(const Input::Key& key)
	{
		if (key == VK_CONTROL)
		{
			auto selectedObjects = o2EditorSceneScreen.GetSelectedObjects();
			if (selectedObjects.Count() > 0)
			{
				SceneEditableObject* lastSelectedObject = selectedObjects.Last();
				mHandlesAngle = -lastSelectedObject->GetTransform().xv.Normalized().Angle(Vec2F::Right());

				mVerDragHandle.angle = mHandlesAngle;
				mHorDragHandle.angle = mHandlesAngle;
				mBothDragHandle.angle = mHandlesAngle;
			}
		}
	}

	void MoveTool::MoveSelectedObjects(const Vec2F& delta)
	{
		auto selectedObjects = o2EditorSceneScreen.GetTopSelectedObjects();
		for (auto object : selectedObjects)
		{
			Basis basis = object->GetTransform();
			basis.origin += delta;
			object->SetTransform(basis);
			object->UpdateTransform();
		}

		UpdateHandlesPosition();
	}

	void MoveTool::MoveSelectedObjectsWithAction(const Vec2F& delta)
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedObjects().Select<Basis>(
			[](SceneEditableObject* x) { return x->GetTransform(); });

		mTransformAction = mnew TransformAction(o2EditorSceneScreen.GetTopSelectedObjects());

		MoveSelectedObjects(delta);

		mTransformAction->Completed();
		o2EditorApplication.DoneAction(mTransformAction);
		mTransformAction = nullptr;
	}

}

DECLARE_CLASS(Editor::MoveTool);
