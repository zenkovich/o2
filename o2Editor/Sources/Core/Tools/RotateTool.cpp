#include "RotateTool.h"

#include "Render/Mesh.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

EditorRotateTool::EditorRotateTool():
	mPivotDragHandle(mnew Sprite("ui/UI2_pivot.png"),
					 mnew Sprite("ui/UI2_pivot_select.png"),
					 mnew Sprite("ui/UI2_pivot_pressed.png"))
{
	mRotateRingFillMesh = new Mesh(TextureRef::Null(), mRotateRingSegs * 4, mRotateRingSegs * 2);
	mAngleMesh = new Mesh(TextureRef::Null(), mRotateRingSegs * 4, mRotateRingSegs * 2);

	mPivotDragHandle.onChangedPos += Function<void(const Vec2F&)>(this, &EditorRotateTool::OnPivotDragHandleMoved);
	mPivotDragHandle.enabled = false;
}

EditorRotateTool::~EditorRotateTool()
{
	delete mRotateRingFillMesh;
	delete mAngleMesh;
}

void EditorRotateTool::Update(float dt)
{

}

void EditorRotateTool::DrawScreen()
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

	EditorSelectionTool::DrawScreen();
}

void EditorRotateTool::OnEnabled()
{
	CalcPivotByActorsCenter();
	mPivotDragHandle.enabled = true;
}

void EditorRotateTool::OnDisabled()
{
	mPivotDragHandle.enabled = false;
}

void EditorRotateTool::OnSceneChanged(Vector<Actor*> changedActors)
{}

void EditorRotateTool::OnActorsSelectionChanged(Vector<Actor*> actors)
{
	CalcPivotByActorsCenter();
}

void EditorRotateTool::UpdateMeshes()
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

void EditorRotateTool::CalcPivotByActorsCenter()
{
	auto selectedActors = o2EditorSceneScreen.GetSelectedActors();
	mScenePivot =
		selectedActors.Sum<Vec2F>([](auto x) { return x->transform.GetWorldPosition(); }) /
		(float)selectedActors.Count();

	mPivotDragHandle.position = mScenePivot;
}

void EditorRotateTool::OnPivotDragHandleMoved(const Vec2F& position)
{
	mScenePivot = position;
}

bool EditorRotateTool::IsPointInRotateRing(const Vec2F& point) const
{
	float pivotDist = (o2EditorSceneScreen.SceneToScreenPoint(mScenePivot) - point).Length();

	return pivotDist > mRotateRingInsideRadius && pivotDist < mRotateRingOutsideRadius;
}

void EditorRotateTool::OnCursorPressed(const Input::Cursor& cursor)
{
	if (IsPointInRotateRing(cursor.mPosition))
	{
		mRingPressed = true;
		Vec2F cursorInScene = o2EditorSceneScreen.ScreenToScenePoint(cursor.mPosition);
		mPressAngle = Vec2F::Angle(cursorInScene - mScenePivot, Vec2F::Right());
		mCurrentRotateAngle = mPressAngle;
		mSnapAngleAccumulated = 0.0f;
	}
	else EditorSelectionTool::OnCursorPressed(cursor);
}

void EditorRotateTool::OnCursorReleased(const Input::Cursor& cursor)
{
	if (mRingPressed)
	{
		mRingPressed = false;
	}
	else EditorSelectionTool::OnCursorReleased(cursor);
}

void EditorRotateTool::OnCursorPressBreak(const Input::Cursor& cursor)
{
	if (mRingPressed)
	{
		mRingPressed = false;
	}
	else EditorSelectionTool::OnCursorPressBreak(cursor);
}

void EditorRotateTool::OnCursorStillDown(const Input::Cursor& cursor)
{
	if (mRingPressed)
	{
		if (cursor.mDelta != Vec2F())
		{
			Vec2F cursorInScene = o2EditorSceneScreen.ScreenToScenePoint(cursor.mPosition);
			Vec2F lastCursorInScene = o2EditorSceneScreen.ScreenToScenePoint(cursor.mPosition - cursor.mDelta);
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
	else EditorSelectionTool::OnCursorStillDown(cursor);
}

void EditorRotateTool::OnKeyPressed(const Input::Key& key)
{
	float angle = o2Input.IsKeyDown(VK_SHIFT) ? angleSnapStep : 1.0f;

	if (key == VK_LEFT || key == VK_DOWN)
	{
		if (o2Input.IsKeyDown(VK_CONTROL)) RotateActorsSeparated(Math::Deg2rad(-angle));
		else RotateActors(Math::Deg2rad(-angle));
	}

	if (key == VK_RIGHT || key == VK_UP)
	{
		if (o2Input.IsKeyDown(VK_CONTROL)) RotateActorsSeparated(Math::Deg2rad(-angle));
		else RotateActors(Math::Deg2rad(angle));
	}
}

void EditorRotateTool::OnKeyStayDown(const Input::Key& key)
{
	if (key.mPressedTime < 0.3f)
		return;

	float angle = o2Input.IsKeyDown(VK_SHIFT) ? angleSnapStep : 1.0f;

	if (key == VK_LEFT || key == VK_DOWN)
	{
		if (o2Input.IsKeyDown(VK_CONTROL)) RotateActorsSeparated(Math::Deg2rad(-angle));
		else RotateActors(Math::Deg2rad(-angle));
	}

	if (key == VK_RIGHT || key == VK_UP)
	{
		if (o2Input.IsKeyDown(VK_CONTROL)) RotateActorsSeparated(Math::Deg2rad(-angle));
		else RotateActors(Math::Deg2rad(angle));
	}
}

void EditorRotateTool::RotateActors(float angleDelta)
{
	Basis transform = Basis::Translated(mScenePivot*-1.0f)*Basis::Rotated(-angleDelta)*Basis::Translated(mScenePivot);
	for (auto actor : o2EditorSceneScreen.GetTopSelectedActors())
	{
		actor->transform.SetWorldNonSizedBasis(actor->transform.GetWorldNonSizedBasis()*transform);
	}
}

void EditorRotateTool::RotateActorsSeparated(float angleDelta)
{
	for (auto actor : o2EditorSceneScreen.GetTopSelectedActors())
		actor->transform.angle -= angleDelta;
}
