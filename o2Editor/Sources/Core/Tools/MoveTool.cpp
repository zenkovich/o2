#include "MoveTool.h"

#include "Core/Actions/ActorsTransform.h"
#include "Core/EditorApplication.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

EditorMoveTool::EditorMoveTool():
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

	mHorDragHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorMoveTool::OnHorDragHandleMoved);
	mVerDragHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorMoveTool::OnVerDragHandleMoved);
	mBothDragHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorMoveTool::OnBothDragHandleMoved);

	mHorDragHandle.onPressed = Function<void()>(this, &EditorMoveTool::HandlePressed);
	mVerDragHandle.onPressed = Function<void()>(this, &EditorMoveTool::HandlePressed);
	mBothDragHandle.onPressed = Function<void()>(this, &EditorMoveTool::HandlePressed);

	mHorDragHandle.onReleased = Function<void()>(this, &EditorMoveTool::HandleReleased);
	mVerDragHandle.onReleased = Function<void()>(this, &EditorMoveTool::HandleReleased);
	mBothDragHandle.onReleased = Function<void()>(this, &EditorMoveTool::HandleReleased);

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

EditorMoveTool::~EditorMoveTool()
{}

void EditorMoveTool::Update(float dt)
{}

void EditorMoveTool::OnEnabled()
{
	mHorDragHandle.enabled = true;
	mVerDragHandle.enabled = true;
	mBothDragHandle.enabled = true;
	UpdateHandlesPosition();
}

void EditorMoveTool::OnDisabled()
{
	mHorDragHandle.enabled = false;
	mVerDragHandle.enabled = false;
	mBothDragHandle.enabled = false;
}

void EditorMoveTool::OnSceneChanged(Vector<Actor*> changedActors)
{
	UpdateHandlesPosition();
}

void EditorMoveTool::OnActorsSelectionChanged(Vector<Actor*> actors)
{
	UpdateHandlesPosition();
}

void EditorMoveTool::OnHorDragHandleMoved(const Vec2F& position)
{
	Vec2F axis = Vec2F::Rotated(mHandlesAngle);
	Vec2F delta = position - mLastSceneHandlesPos;
	Vec2F axisDelta = delta.Project(axis);

	HandlesMoved(axisDelta, o2Input.IsKeyDown(VK_SHIFT), false);
}

void EditorMoveTool::OnVerDragHandleMoved(const Vec2F& position)
{
	Vec2F axis = Vec2F::Rotated(mHandlesAngle + Math::PI()*0.5f);
	Vec2F delta = position - mLastSceneHandlesPos;
	Vec2F axisDelta = delta.Project(axis);

	HandlesMoved(axisDelta, false, o2Input.IsKeyDown(VK_SHIFT));
}

void EditorMoveTool::OnBothDragHandleMoved(const Vec2F& position)
{
	bool snap = o2Input.IsKeyDown(VK_SHIFT);
	HandlesMoved(position - mLastSceneHandlesPos, snap, snap);
}

void EditorMoveTool::HandlePressed()
{
	mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
		[](Actor* x) { return x->transform; });
}

void EditorMoveTool::HandleReleased()
{
	auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
	o2EditorApplication.DoneAction(action);
}

void EditorMoveTool::HandlesMoved(const Vec2F& delta, bool snapHor /*= false*/, bool spanVer /*= false*/)
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

void EditorMoveTool::UpdateHandlesPosition()
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

void EditorMoveTool::OnKeyPressed(const Input::Key& key)
{
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

	EditorSelectionTool::OnKeyPressed(key);
}

void EditorMoveTool::OnKeyStayDown(const Input::Key& key)
{
	float delta = o2Input.IsKeyDown(VK_SHIFT) ? snapStep : 1.0f;

	if (key.mPressedTime < 0.3f)
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

void EditorMoveTool::OnKeyReleased(const Input::Key& key)
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

void EditorMoveTool::MoveSelectedActors(const Vec2F& delta)
{
	auto selectedActors = o2EditorSceneScreen.GetTopSelectedActors();
	for (auto actor : selectedActors)
		actor->transform.SetWorldPosition(actor->transform.GetWorldPosition() + delta);
}

void EditorMoveTool::MoveSelectedActorsWithAction(const Vec2F& delta)
{
	mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
		[](Actor* x) { return x->transform; });

	MoveSelectedActors(delta);

	auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
	o2EditorApplication.DoneAction(action);
}
