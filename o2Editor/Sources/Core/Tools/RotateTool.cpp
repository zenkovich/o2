#include "RotateTool.h"

#include "Core/Actions/ActorsTransform.h"
#include "Core/EditorApplication.h"
#include "Render/Mesh.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"

namespace Editor
{
	RotateTool::RotateTool():
		mPivotDragHandle(mnew Sprite("ui/UI2_pivot.png"),
						 mnew Sprite("ui/UI2_pivot_select.png"),
						 mnew Sprite("ui/UI2_pivot_pressed.png"))
	{
		mRotateRingFillMesh = new Mesh(TextureRef::Null(), mRotateRingSegs * 4, mRotateRingSegs * 2);
		mAngleMesh = new Mesh(TextureRef::Null(), mRotateRingSegs * 4, mRotateRingSegs * 2);

		mPivotDragHandle.onChangedPos += Function<void(const Vec2F&)>(this, &RotateTool::OnPivotDragHandleMoved);
		mPivotDragHandle.enabled = false;
	}

	RotateTool::~RotateTool()
	{
		delete mRotateRingFillMesh;
		delete mAngleMesh;
	}

	void RotateTool::Update(float dt)
	{}

	void RotateTool::DrawScreen()
	{
		if (o2EditorSceneScreen.GetSelectedActors().Count() > 0)
		{
			UpdateMeshes();
			mRotateRingFillMesh->Draw();
			if (mRingPressed)
				mAngleMesh->Draw();

			Vec2F screenPos = o2EditorSceneScreen.SceneToScreenPoint(mScenePivot);
			o2Render.DrawCircle(screenPos, mRotateRingInsideRadius, mRotateRingsColor, mRotateRingSegs);
			o2Render.DrawCircle(screenPos, mRotateRingOutsideRadius, mRotateRingsColor, mRotateRingSegs);
		}

		SelectionTool::DrawScreen();
	}

	void RotateTool::OnEnabled()
	{
		CalcPivotByActorsCenter();
		mPivotDragHandle.enabled = true;
	}

	void RotateTool::OnDisabled()
	{
		mPivotDragHandle.enabled = false;
	}

	void RotateTool::OnSceneChanged(Vector<Actor*> changedActors)
	{}

	void RotateTool::OnActorsSelectionChanged(Vector<Actor*> actors)
	{
		CalcPivotByActorsCenter();
	}

	void RotateTool::UpdateMeshes()
	{
		Vec2F screenPos = o2EditorSceneScreen.SceneToScreenPoint(mScenePivot);

		Color4 fillColor = mRotateRingsFillColor;
		Color4 fillColor2 = mRotateRingsFillColor2;

		const float selectionAlphaCoef = 1.2f;
		const float pressingAlphaCoef = 0.5f;

		if (mRingPressed)
		{
			fillColor.a = (int)((float)fillColor.a*pressingAlphaCoef);
			fillColor2.a = (int)((float)fillColor2.a*pressingAlphaCoef);
		}
		else if (IsPointInRotateRing(o2Input.GetCursorPos()))
		{
			fillColor.a = (int)((float)fillColor.a*selectionAlphaCoef);
			fillColor2.a = (int)((float)fillColor2.a*selectionAlphaCoef);
		}

		ULong fillColorUL = fillColor.ARGB();
		ULong fillColorUL2 = fillColor2.ARGB();
		mRotateRingFillMesh->vertexCount = mRotateRingSegs * 4;
		mRotateRingFillMesh->polyCount = mRotateRingSegs * 2;
		float segAngle = 2.0f*Math::PI() / (float)mRotateRingSegs;

		float angle = 0.0f;
		int i = 0;
		while (angle < 2.0f*Math::PI())
		{
			float angleNext = angle + segAngle;

			Vec2F pinside = Vec2F::Rotated(angle)*mRotateRingInsideRadius + screenPos;
			Vec2F poutside = Vec2F::Rotated(angle)*mRotateRingOutsideRadius + screenPos;
			Vec2F pinsideNext = Vec2F::Rotated(angleNext)*mRotateRingInsideRadius + screenPos;
			Vec2F poutsideNext = Vec2F::Rotated(angleNext)*mRotateRingOutsideRadius + screenPos;

			int vi = i * 4;
			int pi = i * 6;
			i++;

			ULong currFillColor = fillColorUL;
			if (angle < Math::PI()*0.5f || (angle >= Math::PI() - FLT_EPSILON && angle <= Math::PI()*1.5f + FLT_EPSILON))
				currFillColor = fillColorUL2;

			mRotateRingFillMesh->vertices[vi] = Vertex2(pinside, currFillColor, 0.0f, 0.0f);
			mRotateRingFillMesh->vertices[vi + 1] = Vertex2(poutside, currFillColor, 0.0f, 0.0f);
			mRotateRingFillMesh->vertices[vi + 2] = Vertex2(pinsideNext, currFillColor, 0.0f, 0.0f);
			mRotateRingFillMesh->vertices[vi + 3] = Vertex2(poutsideNext, currFillColor, 0.0f, 0.0f);

			mRotateRingFillMesh->indexes[pi] = vi;
			mRotateRingFillMesh->indexes[pi + 1] = vi + 1;
			mRotateRingFillMesh->indexes[pi + 2] = vi + 3;

			mRotateRingFillMesh->indexes[pi + 3] = vi;
			mRotateRingFillMesh->indexes[pi + 4] = vi + 3;
			mRotateRingFillMesh->indexes[pi + 5] = vi + 2;

			angle = angleNext;
		}

		mAngleMesh->vertexCount = 0;
		mAngleMesh->polyCount = 0;

		if (!mRingPressed)
			return;

		angle = mPressAngle;
		i = 0;
		float direction = Math::Sign(mCurrentRotateAngle - mPressAngle);
		ULong angleRingColor = direction > 0.0f ? mRotateMeshClockwiseColor.ABGR() : mRotateMeshCClockwiseColor.ABGR();
		int reqAngleMeshSegs = Math::CeilToInt(Math::Abs(mCurrentRotateAngle - mPressAngle) / segAngle) + 1;
		mAngleMesh->Resize(reqAngleMeshSegs * 4, reqAngleMeshSegs * 2);
		while (direction > 0.0f ? angle < mCurrentRotateAngle : angle > mCurrentRotateAngle)
		{
			float angleNext = angle + segAngle*direction;
			if (direction > 0.0f ? angleNext > mCurrentRotateAngle : angleNext < mCurrentRotateAngle)
				angleNext = mCurrentRotateAngle;

			Vec2F pinside = Vec2F::Rotated(-angle)*mRotateRingInsideRadius + screenPos;
			Vec2F poutside = Vec2F::Rotated(-angle)*mRotateRingOutsideRadius + screenPos;
			Vec2F pinsideNext = Vec2F::Rotated(-angleNext)*mRotateRingInsideRadius + screenPos;
			Vec2F poutsideNext = Vec2F::Rotated(-angleNext)*mRotateRingOutsideRadius + screenPos;

			int vi = i * 4;
			int pi = i * 6;
			i++;

			mAngleMesh->vertices[vi] = Vertex2(pinside, angleRingColor, 0.0f, 0.0f);
			mAngleMesh->vertices[vi + 1] = Vertex2(poutside, angleRingColor, 0.0f, 0.0f);
			mAngleMesh->vertices[vi + 2] = Vertex2(pinsideNext, angleRingColor, 0.0f, 0.0f);
			mAngleMesh->vertices[vi + 3] = Vertex2(poutsideNext, angleRingColor, 0.0f, 0.0f);

			mAngleMesh->indexes[pi] = vi;
			mAngleMesh->indexes[pi + 1] = vi + 1;
			mAngleMesh->indexes[pi + 2] = vi + 3;

			mAngleMesh->indexes[pi + 3] = vi;
			mAngleMesh->indexes[pi + 4] = vi + 3;
			mAngleMesh->indexes[pi + 5] = vi + 2;

			mAngleMesh->vertexCount = i * 4;
			mAngleMesh->polyCount = i * 2;

			angle = angleNext;
		}
	}

	void RotateTool::CalcPivotByActorsCenter()
	{
		auto selectedActors = o2EditorSceneScreen.GetSelectedActors();
		mScenePivot =
			selectedActors.Sum<Vec2F>([](auto x) { return x->transform.GetWorldPosition(); }) /
			(float)selectedActors.Count();

		mPivotDragHandle.position = mScenePivot;
	}

	void RotateTool::OnPivotDragHandleMoved(const Vec2F& position)
	{
		mScenePivot = position;
	}

	bool RotateTool::IsPointInRotateRing(const Vec2F& point) const
	{
		float pivotDist = (o2EditorSceneScreen.SceneToScreenPoint(mScenePivot) - point).Length();

		return pivotDist > mRotateRingInsideRadius && pivotDist < mRotateRingOutsideRadius;
	}

	void RotateTool::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (IsPointInRotateRing(cursor.position))
		{
			mRingPressed = true;
			Vec2F cursorInScene = o2EditorSceneScreen.ScreenToScenePoint(cursor.position);
			mPressAngle = Vec2F::Angle(cursorInScene - mScenePivot, Vec2F::Right());
			mCurrentRotateAngle = mPressAngle;
			mSnapAngleAccumulated = 0.0f;

			mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
				[](Actor* x) { return x->transform; });
		}
		else SelectionTool::OnCursorPressed(cursor);
	}

	void RotateTool::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mRingPressed)
		{
			mRingPressed = false;

			auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
			o2EditorApplication.DoneAction(action);
		}
		else SelectionTool::OnCursorReleased(cursor);
	}

	void RotateTool::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		if (mRingPressed)
		{
			mRingPressed = false;

			auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
			o2EditorApplication.DoneAction(action);
		}
		else SelectionTool::OnCursorPressBreak(cursor);
	}

	void RotateTool::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (!o2EditorTree.GetActorsTree()->IsFocused())
			return;

		if (mRingPressed)
		{
			if (cursor.delta != Vec2F())
			{
				Vec2F cursorInScene = o2EditorSceneScreen.ScreenToScenePoint(cursor.position);
				Vec2F lastCursorInScene = o2EditorSceneScreen.ScreenToScenePoint(cursor.position - cursor.delta);
				float angleDelta = Vec2F::SignedAngle(cursorInScene - mScenePivot, lastCursorInScene - mScenePivot);

				if (o2Input.IsKeyDown(VK_SHIFT))
				{
					float angleStepRad = Math::Deg2rad(angleSnapStep);
					mSnapAngleAccumulated += angleDelta;
					float dir = Math::Sign(mSnapAngleAccumulated);

					while (mSnapAngleAccumulated*dir > angleStepRad)
					{
						mSnapAngleAccumulated -= dir*angleStepRad;

						if (o2Input.IsKeyDown(VK_CONTROL)) RotateActorsSeparated(angleStepRad*dir);
						else RotateActors(angleStepRad*dir);
					}
				}
				else
				{
					if (o2Input.IsKeyDown(VK_CONTROL)) RotateActorsSeparated(angleDelta);
					else RotateActors(angleDelta);
				}

				mCurrentRotateAngle += angleDelta;
			}
		}
		else SelectionTool::OnCursorStillDown(cursor);
	}

	void RotateTool::OnKeyPressed(const Input::Key& key)
	{
		if (!o2EditorTree.GetActorsTree()->IsFocused())
			return;

		float angle = o2Input.IsKeyDown(VK_SHIFT) ? angleSnapStep : 1.0f;

		if (key == VK_LEFT || key == VK_DOWN)
		{
			if (o2Input.IsKeyDown(VK_CONTROL)) RotateActorsSeparatedWithAction(Math::Deg2rad(-angle));
			else RotateActorsWithAction(Math::Deg2rad(-angle));
		}

		if (key == VK_RIGHT || key == VK_UP)
		{
			if (o2Input.IsKeyDown(VK_CONTROL)) RotateActorsSeparatedWithAction(Math::Deg2rad(-angle));
			else RotateActorsWithAction(Math::Deg2rad(angle));
		}

		SelectionTool::OnKeyPressed(key);
	}

	void RotateTool::OnKeyStayDown(const Input::Key& key)
	{
		if (key.pressedTime < 0.3f)
			return;

		float angle = o2Input.IsKeyDown(VK_SHIFT) ? angleSnapStep : 1.0f;

		if (key == VK_LEFT || key == VK_DOWN)
		{
			if (o2Input.IsKeyDown(VK_CONTROL)) RotateActorsSeparatedWithAction(Math::Deg2rad(-angle));
			else RotateActorsWithAction(Math::Deg2rad(-angle));
		}

		if (key == VK_RIGHT || key == VK_UP)
		{
			if (o2Input.IsKeyDown(VK_CONTROL)) RotateActorsSeparatedWithAction(Math::Deg2rad(-angle));
			else RotateActorsWithAction(Math::Deg2rad(angle));
		}
	}

	void RotateTool::RotateActors(float angleDelta)
	{
		Basis transform = Basis::Translated(mScenePivot*-1.0f)*Basis::Rotated(-angleDelta)*Basis::Translated(mScenePivot);
		for (auto actor : o2EditorSceneScreen.GetTopSelectedActors())
		{
			actor->transform.SetWorldNonSizedBasis(actor->transform.GetWorldNonSizedBasis()*transform);
		}
	}

	void RotateTool::RotateActorsSeparated(float angleDelta)
	{
		for (auto actor : o2EditorSceneScreen.GetTopSelectedActors())
			actor->transform.angle -= angleDelta;
	}

	void RotateTool::RotateActorsWithAction(float angleDelta)
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
			[](Actor* x) { return x->transform; });

		RotateActors(angleDelta);

		auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
		o2EditorApplication.DoneAction(action);
	}

	void RotateTool::RotateActorsSeparatedWithAction(float angleDelta)
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
			[](Actor* x) { return x->transform; });

		RotateActorsSeparated(angleDelta);

		auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
		o2EditorApplication.DoneAction(action);
	}

}
 
CLASS_META(Editor::RotateTool)
{
	BASE_CLASS(Editor::SelectionTool);

	PUBLIC_FIELD(angleSnapStep);
	PROTECTED_FIELD(mRotateRingInsideRadius);
	PROTECTED_FIELD(mRotateRingOutsideRadius);
	PROTECTED_FIELD(mRotateRingSegs);
	PROTECTED_FIELD(mRotateRingsColor);
	PROTECTED_FIELD(mRotateRingsFillColor);
	PROTECTED_FIELD(mRotateRingsFillColor2);
	PROTECTED_FIELD(mRotateMeshClockwiseColor);
	PROTECTED_FIELD(mRotateMeshCClockwiseColor);
	PROTECTED_FIELD(mRotateRingFillMesh);
	PROTECTED_FIELD(mAngleMesh);
	PROTECTED_FIELD(mScenePivot);
	PROTECTED_FIELD(mPivotDragHandle);
	PROTECTED_FIELD(mPressAngle);
	PROTECTED_FIELD(mCurrentRotateAngle);
	PROTECTED_FIELD(mRingPressed);
	PROTECTED_FIELD(mSnapAngleAccumulated);
	PROTECTED_FIELD(mBeforeTransforms);

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, DrawScreen);
	PUBLIC_FUNCTION(void, OnEnabled);
	PUBLIC_FUNCTION(void, OnDisabled);
	PUBLIC_FUNCTION(void, OnSceneChanged, Vector<Actor*>);
	PUBLIC_FUNCTION(void, OnActorsSelectionChanged, Vector<Actor*>);
	PUBLIC_FUNCTION(void, UpdateMeshes);
	PUBLIC_FUNCTION(void, CalcPivotByActorsCenter);
	PUBLIC_FUNCTION(void, OnPivotDragHandleMoved, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsPointInRotateRing, const Vec2F&);
	PUBLIC_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PUBLIC_FUNCTION(void, OnKeyStayDown, const Input::Key&);
	PUBLIC_FUNCTION(void, RotateActors, float);
	PUBLIC_FUNCTION(void, RotateActorsSeparated, float);
	PUBLIC_FUNCTION(void, RotateActorsWithAction, float);
	PUBLIC_FUNCTION(void, RotateActorsSeparatedWithAction, float);
}
END_META;
 