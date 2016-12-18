#include "MoveTool.h"

#include "Core/Actions/ActorsTransform.h"
#include "Core/EditorApplication.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"

namespace Editor
{
	MoveTool::MoveTool():
		mHorDragHandle(mnew Sprite("ui/UI2_right_move_arrow.png"),
					   mnew Sprite("ui/UI2_right_move_arrow_select.png"),
					   mnew Sprite("ui/UI2_right_move_arrow_pressed.png")),
		mVerDragHandle(mnew Sprite("ui/UI2_up_move_arrow.png"),
					   mnew Sprite("ui/UI2_up_move_arrow_select.png"),
					   mnew Sprite("ui/UI2_up_move_arrow_pressed.png")),
		mBothDragHandle(mnew Sprite("ui/UI2_move_tool_center.png"),
						mnew Sprite("ui/UI2_move_tool_center_select.png"),
						mnew Sprite("ui/UI2_move_tool_center_pressed.png"))
	{
		mHorDragHandle.enabled = false;
		mVerDragHandle.enabled = false;
		mBothDragHandle.enabled = false;

		mHorDragHandle.onChangedPos = Function<void(const Vec2F&)>(this, &MoveTool::OnHorDragHandleMoved);
		mVerDragHandle.onChangedPos = Function<void(const Vec2F&)>(this, &MoveTool::OnVerDragHandleMoved);
		mBothDragHandle.onChangedPos = Function<void(const Vec2F&)>(this, &MoveTool::OnBothDragHandleMoved);

		mHorDragHandle.onPressed = Function<void()>(this, &MoveTool::HandlePressed);
		mVerDragHandle.onPressed = Function<void()>(this, &MoveTool::HandlePressed);
		mBothDragHandle.onPressed = Function<void()>(this, &MoveTool::HandlePressed);

		mHorDragHandle.onReleased = Function<void()>(this, &MoveTool::HandleReleased);
		mVerDragHandle.onReleased = Function<void()>(this, &MoveTool::HandleReleased);
		mBothDragHandle.onReleased = Function<void()>(this, &MoveTool::HandleReleased);

		mHorDragHandle.regularSprite->SetSizePivot(Vec2F(1, 5));
		mHorDragHandle.hoverSprite->SetSizePivot(Vec2F(1, 5));
		mHorDragHandle.pressedSprite->SetSizePivot(Vec2F(1, 5));

		mVerDragHandle.regularSprite->SetSizePivot(Vec2F(5, 1));
		mVerDragHandle.hoverSprite->SetSizePivot(Vec2F(5, 1));
		mVerDragHandle.pressedSprite->SetSizePivot(Vec2F(5, 1));

		mBothDragHandle.regularSprite->SetSizePivot(Vec2F(1, 1));
		mBothDragHandle.hoverSprite->SetSizePivot(Vec2F(1, 1));
		mBothDragHandle.pressedSprite->SetSizePivot(Vec2F(1, 1));
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

	void MoveTool::OnSceneChanged(Vector<Actor*> changedActors)
	{
		UpdateHandlesPosition();
	}

	void MoveTool::OnActorsSelectionChanged(Vector<Actor*> actors)
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
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
			[](Actor* x) { return x->transform; });
	}

	void MoveTool::HandleReleased()
	{
		auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
		o2EditorApplication.DoneAction(action);
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
				MoveSelectedActors(roundDelta);
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

			MoveSelectedActors(delta);
		}
	}

	void MoveTool::UpdateHandlesPosition()
	{
		auto selectedActors = o2EditorSceneScreen.GetSelectedActors();
		mLastSceneHandlesPos =
			selectedActors.Sum<Vec2F>([](auto x) { return x->transform.GetWorldPosition(); }) /
			(float)selectedActors.Count();

		mVerDragHandle.position = mLastSceneHandlesPos;
		mHorDragHandle.position = mLastSceneHandlesPos;
		mBothDragHandle.position = mLastSceneHandlesPos;

		if (selectedActors.Count() > 0 && !o2Input.IsKeyDown(VK_CONTROL))
		{
			Actor* lastSelectedActor = selectedActors.Last();
			mHandlesAngle = -lastSelectedActor->transform.right->Angle(Vec2F::Right());

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
		if (!o2EditorTree.GetActorsTree()->IsFocused())
			return;

		float delta = o2Input.IsKeyDown(VK_SHIFT) ? snapStep : 1.0f;

		if (key == VK_LEFT)
			MoveSelectedActorsWithAction(Vec2F::Left()*delta);

		if (key == VK_RIGHT)
			MoveSelectedActorsWithAction(Vec2F::Right()*delta);

		if (key == VK_UP)
			MoveSelectedActorsWithAction(Vec2F::Up()*delta);

		if (key == VK_DOWN)
			MoveSelectedActorsWithAction(Vec2F::Down()*delta);

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
		if (!o2EditorTree.GetActorsTree()->IsFocused())
			return;

		float delta = o2Input.IsKeyDown(VK_SHIFT) ? snapStep : 1.0f;

		if (key.pressedTime < 0.3f)
			return;

		if (key == VK_LEFT)
			MoveSelectedActorsWithAction(Vec2F::Left()*delta);

		if (key == VK_RIGHT)
			MoveSelectedActorsWithAction(Vec2F::Right()*delta);

		if (key == VK_UP)
			MoveSelectedActorsWithAction(Vec2F::Up()*delta);

		if (key == VK_DOWN)
			MoveSelectedActorsWithAction(Vec2F::Down()*delta);
	}

	void MoveTool::OnKeyReleased(const Input::Key& key)
	{
		if (key == VK_CONTROL)
		{
			auto selectedActors = o2EditorSceneScreen.GetSelectedActors();
			if (selectedActors.Count() > 0)
			{
				Actor* lastSelectedActor = selectedActors.Last();
				mHandlesAngle = -lastSelectedActor->transform.right->Angle(Vec2F::Right());

				mVerDragHandle.angle = mHandlesAngle;
				mHorDragHandle.angle = mHandlesAngle;
				mBothDragHandle.angle = mHandlesAngle;
			}
		}
	}

	void MoveTool::MoveSelectedActors(const Vec2F& delta)
	{
		auto selectedActors = o2EditorSceneScreen.GetTopSelectedActors();
		for (auto actor : selectedActors)
			actor->transform.SetWorldPosition(actor->transform.GetWorldPosition() + delta);
	}

	void MoveTool::MoveSelectedActorsWithAction(const Vec2F& delta)
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
			[](Actor* x) { return x->transform; });

		MoveSelectedActors(delta);

		auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
		o2EditorApplication.DoneAction(action);
	}

}
 
CLASS_META(Editor::MoveTool)
{
	BASE_CLASS(Editor::SelectionTool);

	PUBLIC_FIELD(snapStep);
	PROTECTED_FIELD(mHorDragHandle);
	PROTECTED_FIELD(mVerDragHandle);
	PROTECTED_FIELD(mBothDragHandle);
	PROTECTED_FIELD(mLastSceneHandlesPos);
	PROTECTED_FIELD(mSnapPosition);
	PROTECTED_FIELD(mHandlesAngle);
	PROTECTED_FIELD(mBeforeTransforms);

	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnSceneChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, OnActorsSelectionChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, OnHorDragHandleMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, OnVerDragHandleMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, OnBothDragHandleMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, HandlePressed);
	PROTECTED_FUNCTION(void, HandleReleased);
	PROTECTED_FUNCTION(void, HandlesMoved, const Vec2F&, bool, bool);
	PROTECTED_FUNCTION(void, UpdateHandlesPosition);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyStayDown, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, MoveSelectedActors, const Vec2F&);
	PROTECTED_FUNCTION(void, MoveSelectedActorsWithAction, const Vec2F&);
}
END_META;
 