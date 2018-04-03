#include "stdafx.h"
#include "ScaleTool.h"

#include "Core/Actions/ActorsTransform.h"
#include "Core/EditorApplication.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"

namespace Editor
{
	ScaleTool::ScaleTool()
	{
		mHorDragHandle = SceneDragHandle(mnew Sprite("ui/UI2_right_scale_arrow.png"),
										 mnew Sprite("ui/UI2_right_scale_arrow_select.png"),
										 mnew Sprite("ui/UI2_right_scale_arrow_pressed.png"));

		mVerDragHandle = SceneDragHandle(mnew Sprite("ui/UI2_up_scale_arrow.png"),
										 mnew Sprite("ui/UI2_up_scale_arrow_select.png"),
										 mnew Sprite("ui/UI2_up_scale_arrow_pressed.png"));

		mBothDragHandle = SceneDragHandle(mnew Sprite("ui/UI2_scale_both.png"),
										  mnew Sprite("ui/UI2_scale_both_select.png"),
										  mnew Sprite("ui/UI2_scale_both_pressed.png"));

		mHorDragHandle.enabled = false;
		mVerDragHandle.enabled = false;
		mBothDragHandle.enabled = false;

		mHorDragHandle.onChangedPos = THIS_FUNC(OnHorDragHandleMoved);
		mVerDragHandle.onChangedPos = THIS_FUNC(OnVerDragHandleMoved);
		mBothDragHandle.onChangedPos = THIS_FUNC(OnBothDragHandleMoved);

		mHorDragHandle.onPressed = THIS_FUNC(HandlePressed);
		mVerDragHandle.onPressed = THIS_FUNC(HandlePressed);
		mBothDragHandle.onPressed = THIS_FUNC(HandlePressed);

		mHorDragHandle.onReleased = THIS_FUNC(UpdateHandlesPosition);
		mVerDragHandle.onReleased = THIS_FUNC(UpdateHandlesPosition);

		mHorDragHandle.onReleased += THIS_FUNC(HandleReleased);
		mVerDragHandle.onReleased += THIS_FUNC(HandleReleased);
		mBothDragHandle.onReleased += THIS_FUNC(HandleReleased);
	}

	ScaleTool::~ScaleTool()
	{}

	void ScaleTool::Update(float dt)
	{}

	void ScaleTool::DrawScreen()
	{
		SelectionTool::DrawScreen();

		if (!mHorDragHandle.IsPressed() && !mVerDragHandle.IsPressed() && !mBothDragHandle.IsPressed())
			UpdateHandlesPositions();

		Vec2F screenHandlesPos = o2EditorSceneScreen.SceneToScreenPoint(mSceneHandlesPos);
		Vec2F screenHorHandlePos = o2EditorSceneScreen.SceneToScreenPoint(mHorDragHandle.GetPosition());
		Vec2F screenVerHandlePos = o2EditorSceneScreen.SceneToScreenPoint(mVerDragHandle.GetPosition());
		o2Render.DrawLine(screenHandlesPos, screenHorHandlePos, Color4::Green());
		o2Render.DrawLine(screenHandlesPos, screenVerHandlePos, Color4::Red());
	}

	void ScaleTool::OnEnabled()
	{
		mHorDragHandle.enabled = true;
		mVerDragHandle.enabled = true;
		mBothDragHandle.enabled = true;
		UpdateHandlesPosition();
	}

	void ScaleTool::OnDisabled()
	{
		mHorDragHandle.enabled = false;
		mVerDragHandle.enabled = false;
		mBothDragHandle.enabled = false;
	}

	void ScaleTool::OnSceneChanged(Vector<Actor*> changedActors)
	{}

	void ScaleTool::OnActorsSelectionChanged(Vector<Actor*> actors)
	{
		UpdateHandlesPosition();
	}

	void ScaleTool::OnHorDragHandleMoved(const Vec2F& position)
	{
		Vec2F axis = Vec2F::Rotated(mHandlesAngle);
		Vec2F handlePos = (position - mSceneHandlesPos).Project(axis) + mSceneHandlesPos;
		float scale = (handlePos - mSceneHandlesPos).Length() / (mLastHorHandlePos - mSceneHandlesPos).Length();

		mLastHorHandlePos = handlePos;
		mHorDragHandle.position = handlePos;

		ScaleSelectedActors(Vec2F(scale, 1.0f));
	}

	void ScaleTool::OnVerDragHandleMoved(const Vec2F& position)
	{
		Vec2F axis = Vec2F::Rotated(mHandlesAngle).Perpendicular();
		Vec2F handlePos = (position - mSceneHandlesPos).Project(axis) + mSceneHandlesPos;
		float scale = (handlePos - mSceneHandlesPos).Length() / (mLastVerHandlePos - mSceneHandlesPos).Length();

		mLastVerHandlePos = handlePos;
		mVerDragHandle.position = handlePos;

		ScaleSelectedActors(Vec2F(1.0f, scale));
	}

	void ScaleTool::OnBothDragHandleMoved(const Vec2F& position)
	{
		float delta = o2EditorSceneScreen.SceneToScreenPoint(position).x -
			o2EditorSceneScreen.SceneToScreenPoint(mLastBothHandlePos).x;

		float scale = 1.0f + delta*bothScaleSence;
		mLastBothHandlePos = position;

		mBothDragHandle.position = mSceneHandlesPos;

		ScaleSelectedActors(Vec2F(scale, scale));
	}

	void ScaleTool::UpdateHandlesPosition()
	{
		auto selectedActors = o2EditorSceneScreen.GetSelectedActors();
		mSceneHandlesPos =
			selectedActors.Sum<Vec2F>([](auto x) { return x->transform->GetWorldPosition(); }) /
			(float)selectedActors.Count();

		if (selectedActors.Count() > 0 && !o2Input.IsKeyDown(VK_CONTROL))
		{
			Actor* lastSelectedActor = selectedActors.Last();
			UpdateHandlesAngleAndPositions(-lastSelectedActor->transform->GetRightDir().Angle(Vec2F::Right()));
		}
		else UpdateHandlesAngleAndPositions(0.0f);
	}

	void ScaleTool::UpdateHandlesAngleAndPositions(float angle)
	{
		mHandlesAngle = angle;

		mVerDragHandle.angle = mHandlesAngle;
		mHorDragHandle.angle = mHandlesAngle;
		mBothDragHandle.angle = mHandlesAngle;

		UpdateHandlesPositions();
	}

	void ScaleTool::UpdateHandlesPositions()
	{
		Vec2F handlesAxis = Vec2F::Rotated(mHandlesAngle);
		Vec2F handlesSceneSize = o2EditorSceneScreen.ScreenToScenePoint(mHandlesSize) -
			o2EditorSceneScreen.ScreenToScenePoint(Vec2F());

		mVerDragHandle.position = mSceneHandlesPos + handlesAxis.Perpendicular()*handlesSceneSize.y;
		mHorDragHandle.position = mSceneHandlesPos + handlesAxis*handlesSceneSize.x;
		mBothDragHandle.position = mSceneHandlesPos;

		mLastVerHandlePos = mVerDragHandle.position;
		mLastHorHandlePos = mHorDragHandle.position;
		mLastBothHandlePos = mBothDragHandle.position;
	}

	void ScaleTool::OnKeyPressed(const Input::Key& key)
	{
		if (!o2EditorTree.GetActorsTree()->IsFocused())
			return;

		if (key == VK_CONTROL)
			UpdateHandlesAngleAndPositions(0.0f);

		SelectionTool::OnKeyPressed(key);
	}

	void ScaleTool::OnKeyStayDown(const Input::Key& key)
	{}

	void ScaleTool::OnKeyReleased(const Input::Key& key)
	{
		if (!o2EditorTree.GetActorsTree()->IsFocused())
			return;

		if (key == VK_CONTROL)
		{
			auto selectedActors = o2EditorSceneScreen.GetSelectedActors();
			if (selectedActors.Count() > 0)
			{
				Actor* lastSelectedActor = selectedActors.Last();
				UpdateHandlesAngleAndPositions(-lastSelectedActor->transform->GetRightDir().Angle(Vec2F::Right()));
			}
		}
	}

	void ScaleTool::ScaleSelectedActors(const Vec2F& scale)
	{
		Basis transform =
			Basis::Translated(mSceneHandlesPos*-1.0f)*
			Basis::Rotated(-mHandlesAngle)*
			Basis::Scaled(scale)*
			Basis::Rotated(mHandlesAngle)*
			Basis::Translated(mSceneHandlesPos);

		for (auto actor : o2EditorSceneScreen.GetTopSelectedActors())
			actor->transform->SetWorldNonSizedBasis(actor->transform->GetWorldNonSizedBasis()*transform);
	}

	void ScaleTool::HandlePressed()
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
			[](Actor* x) { return *x->transform; });
	}

	void ScaleTool::HandleReleased()
	{
		auto action = mnew ActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::ScaleTool);
