#include "ScaleTool.h"

#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

EditorScaleTool::EditorScaleTool():
	mHorDragHandle(mnew Sprite("ui/UI2_right_scale_arrow.png"),
				   mnew Sprite("ui/UI2_right_scale_arrow_select.png"),
				   mnew Sprite("ui/UI2_right_scale_arrow_pressed.png")),
	mVerDragHandle(mnew Sprite("ui/UI2_up_scale_arrow.png"),
				   mnew Sprite("ui/UI2_up_scale_arrow_select.png"),
				   mnew Sprite("ui/UI2_up_scale_arrow_pressed.png")),
	mBothDragHandle(mnew Sprite("ui/UI2_scale_both.png"),
					mnew Sprite("ui/UI2_scale_both_select.png"),
					mnew Sprite("ui/UI2_scale_both_pressed.png"))
{
	mHorDragHandle.enabled = false;
	mVerDragHandle.enabled = false;
	mBothDragHandle.enabled = false;

	mHorDragHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorScaleTool::OnHorDragHandleMoved);
	mVerDragHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorScaleTool::OnVerDragHandleMoved);
	mBothDragHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorScaleTool::OnBothDragHandleMoved);

	mHorDragHandle.regularSprite->SetSizePivot(Vec2F(1, 5));
	mHorDragHandle.hoverSprite->SetSizePivot(Vec2F(1, 5));
	mHorDragHandle.pressedSprite->SetSizePivot(Vec2F(1, 5));

	mVerDragHandle.regularSprite->SetSizePivot(Vec2F(5, 1));
	mVerDragHandle.hoverSprite->SetSizePivot(Vec2F(5, 1));
	mVerDragHandle.pressedSprite->SetSizePivot(Vec2F(5, 1));
}

EditorScaleTool::~EditorScaleTool()
{}

void EditorScaleTool::Update(float dt)
{}

void EditorScaleTool::OnEnabled()
{
	mHorDragHandle.enabled = true;
	mVerDragHandle.enabled = true;
	mBothDragHandle.enabled = true;
	UpdateHandlesPosition();
}

void EditorScaleTool::OnDisabled()
{
	mHorDragHandle.enabled = false;
	mVerDragHandle.enabled = false;
	mBothDragHandle.enabled = false;
}

void EditorScaleTool::OnSceneChanged(Vector<Actor*> changedActors)
{}

void EditorScaleTool::OnActorsSelectionChanged(Vector<Actor*> actors)
{
	UpdateHandlesPosition();
}

void EditorScaleTool::OnHorDragHandleMoved(const Vec2F& position)
{

}

void EditorScaleTool::OnVerDragHandleMoved(const Vec2F& position)
{

}

void EditorScaleTool::OnBothDragHandleMoved(const Vec2F& position)
{

}

void EditorScaleTool::UpdateHandlesPosition()
{
	auto selectedActors = o2EditorSceneScreen.GetSelectedActors();
	mSceneHandlesPos =
		selectedActors.Sum<Vec2F>([](auto x) { return x->transform.GetWorldPosition(); }) /
		(float)selectedActors.Count();

	mVerDragHandle.position = mSceneHandlesPos;
	mHorDragHandle.position = mSceneHandlesPos;
	mBothDragHandle.position = mSceneHandlesPos;

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

void EditorScaleTool::OnKeyPressed(const Input::Key& key)
{
	if (key == VK_CONTROL)
	{
		mHandlesAngle = 0.0f;
		mVerDragHandle.angle = mHandlesAngle;
		mHorDragHandle.angle = mHandlesAngle;
		mBothDragHandle.angle = mHandlesAngle;
	}
}

void EditorScaleTool::OnKeyStayDown(const Input::Key& key)
{}

void EditorScaleTool::OnKeyReleased(const Input::Key& key)
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

void EditorScaleTool::ScaleSelectedActors(const Vec2F& scale)
{

}
