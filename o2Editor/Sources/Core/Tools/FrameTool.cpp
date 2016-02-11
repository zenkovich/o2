#include "FrameTool.h"

#include "Core/Actions/ActorsTransform.h"
#include "Core/EditorApplication.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

EditorFrameTool::EditorFrameTool()
{
	Vec2F rotateHandleSize = Vec2F(mHandlesRotateSize, mHandlesRotateSize);
	Vec2F frameHandleSize = Vec2F(mFrameHandlesSize, mFrameHandlesSize);
	mLeftTopRotateHandle.regularSprite = mnew Sprite(Color4(0, 0, 0, 20));
	mLeftTopRotateHandle.hoverSprite = mnew Sprite(Color4(0, 0, 0, 10));
	mLeftTopRotateHandle.pressedSprite = mnew Sprite(Color4(0, 0, 0, 50));
	mLeftTopRotateHandle.regularSprite->size = rotateHandleSize;
	mLeftTopRotateHandle.hoverSprite->size = rotateHandleSize;
	mLeftTopRotateHandle.pressedSprite->size = rotateHandleSize;
	mLeftTopRotateHandle.regularSprite->szPivot = frameHandleSize*0.5f;
	mLeftTopRotateHandle.hoverSprite->szPivot = frameHandleSize*0.5f;
	mLeftTopRotateHandle.pressedSprite->szPivot = frameHandleSize*0.5f;

	mLeftBottomRotateHandle = mLeftTopRotateHandle;
	mRightTopRotateHandle = mLeftTopRotateHandle;
	mRightBottomRotateHandle = mLeftTopRotateHandle;

	mLeftTopHandle.regularSprite = mnew Sprite(mHandleRegularColor);
	mLeftTopHandle.hoverSprite = mnew Sprite(mHandleSelectColor);
	mLeftTopHandle.pressedSprite = mnew Sprite(mHandlePressedColor);
	mLeftTopHandle.regularSprite->size = frameHandleSize;
	mLeftTopHandle.hoverSprite->size = frameHandleSize;
	mLeftTopHandle.pressedSprite->size = frameHandleSize;

	mLeftHandle = mLeftTopHandle;
	mLeftBottomHandle = mLeftTopHandle;
	mTopHandle = mLeftTopHandle;
	mBottomHandle = mLeftTopHandle;
	mRightTopHandle = mLeftTopHandle;
	mRightHandle = mLeftTopHandle;
	mRightBottomHandle = mLeftTopHandle;

	mPivotHandle.regularSprite = mnew Sprite("ui/UI2_pivot.png");
	mPivotHandle.hoverSprite = mnew Sprite("ui/UI2_pivot_select.png");
	mPivotHandle.pressedSprite = mnew Sprite("ui/UI2_pivot_pressed.png");

	mLeftTopHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnLeftTopHandle);
	mLeftHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnLeftHandle);
	mLeftBottomHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnLeftBottomHandle);
	mTopHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnTopHandle);
	mBottomHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnBottomHandle);
	mRightTopHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnRightTopHandle);
	mRightHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnRightHandle);
	mRightBottomHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnRightBottomHandle);
	mPivotHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnPivotHandle);
	mLeftTopRotateHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnLeftTopRotateHandle);
	mLeftBottomRotateHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnLeftBottomRotateHandle);
	mRightTopRotateHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnRightTopRotateHandle);
	mRightBottomRotateHandle.onChangedPos = Function<void(const Vec2F&)>(this, &EditorFrameTool::OnRightBottomRotateHandle);

	mLeftTopHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mLeftHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mLeftBottomHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mTopHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mBottomHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mRightTopHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mRightHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mRightBottomHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mPivotHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mLeftTopRotateHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mLeftBottomRotateHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mRightTopRotateHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);
	mRightBottomRotateHandle.onPressed = Function<void()>(this, &EditorFrameTool::HandlePressed);

	mLeftTopHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mLeftHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mLeftBottomHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mTopHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mBottomHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mRightTopHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mRightHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mRightBottomHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mPivotHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mLeftTopRotateHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mLeftBottomRotateHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mRightTopRotateHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);
	mRightBottomRotateHandle.onReleased = Function<void()>(this, &EditorFrameTool::HandleReleased);

	SetHandlesEnable(false);
}

EditorFrameTool::~EditorFrameTool()
{}

void EditorFrameTool::DrawScene()
{
	EditorSelectionTool::DrawScene();

	if (o2EditorSceneScreen.GetSelectedActors().Count() > 0)
	{
		o2Render.DrawBasis(mFrame, mHandleRegularColor, mHandleRegularColor, mHandleRegularColor);
	}
}

void EditorFrameTool::OnEnabled()
{
	SetHandlesEnable(true);
	UpdateSelectionFrame();
}

void EditorFrameTool::OnDisabled()
{
	SetHandlesEnable(false);
}

void EditorFrameTool::OnSceneChanged(Vector<Actor*> changedActors)
{
	mNeedRedraw = true;
	if (mChangedFromThis)
	{
		mChangedFromThis = false;
	}
	else UpdateSelectionFrame();
}

void EditorFrameTool::OnActorsSelectionChanged(Vector<Actor*> actors)
{
	UpdateSelectionFrame();
}

void EditorFrameTool::OnKeyPressed(const Input::Key& key)
{
	if (key == VK_LEFT)
		TransformActorsWithAction(Basis::Translated(Vec2F::Left()));

	if (key == VK_RIGHT)
		TransformActorsWithAction(Basis::Translated(Vec2F::Right()));

	if (key == VK_UP)
		TransformActorsWithAction(Basis::Translated(Vec2F::Up()));

	if (key == VK_DOWN)
		TransformActorsWithAction(Basis::Translated(Vec2F::Down()));

	EditorSelectionTool::OnKeyPressed(key);
}

void EditorFrameTool::OnKeyStayDown(const Input::Key& key)
{
	if (key.mPressedTime < 0.3f)
		return;

	if (key == VK_LEFT)
		TransformActorsWithAction(Basis::Translated(Vec2F::Left()));

	if (key == VK_RIGHT)
		TransformActorsWithAction(Basis::Translated(Vec2F::Right()));

	if (key == VK_UP)
		TransformActorsWithAction(Basis::Translated(Vec2F::Up()));

	if (key == VK_DOWN)
		TransformActorsWithAction(Basis::Translated(Vec2F::Down()));
}

void EditorFrameTool::OnKeyReleased(const Input::Key& key)
{
}

void EditorFrameTool::TransformActors(const Basis& transform)
{
	for (auto actor : o2EditorSceneScreen.GetTopSelectedActors())
	{
		if (actor->transform.size != Vec2F())
			actor->transform.SetWorldBasis(actor->transform.GetWorldBasis()*transform);
		else
			actor->transform.SetWorldNonSizedBasis(actor->transform.GetWorldNonSizedBasis()*transform);
	}

	mFrame = mFrame*transform;
	mPivotHandle.position = mPivotHandle.position*transform;
	mChangedFromThis = true;
	UdateHandlesTransform();
}

void EditorFrameTool::TransformActorsWithAction(const Basis& transform)
{
	mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
		[](Actor* x) { return x->transform; });

	TransformActors(transform);

	auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
	o2EditorApplication.DoneAction(action);
}

void EditorFrameTool::UpdateSelectionFrame()
{
	auto selectedActors = o2EditorSceneScreen.GetSelectedActors();

	SetHandlesEnable(true);

	if (selectedActors.Count() == 1)
	{
		mFrame = selectedActors[0]->transform.GetWorldBasis();
		mPivotHandle.position = selectedActors[0]->transform.GetWorldPivot();
	}
	else if (selectedActors.Count() > 0)
	{
		Basis frameBasis = selectedActors.Last()->transform.GetWorldNonSizedBasis();
		Vec2F frameOrigin = frameBasis.offs;
		Vec2F xAxis = frameBasis.xv.Normalized();
		Vec2F yAxis = frameBasis.yv.Normalized();
		Vec2F sx, sy;

		const Vec2F cp[4] = { Vec2F(0, 0), Vec2F(0, 1), Vec2F(1, 0), Vec2F(1, 1) };
		for (auto actor : selectedActors)
		{
			Basis actorTransform = actor->transform.GetWorldBasis();
			for (int i = 0; i < 4; i++)
			{
				Vec2F wp = cp[i] * actorTransform;
				float px = (wp - frameOrigin).Dot(xAxis);
				float py = (wp - frameOrigin).Dot(yAxis);

				sx.x = Math::Min(sx.x, px);
				sx.y = Math::Max(sx.y, px);
				sy.x = Math::Min(sy.x, py);
				sy.y = Math::Max(sy.y, py);
			}
		}

		mFrame.Set(frameOrigin + xAxis*sx.x + yAxis*sy.x, xAxis*(sx.y - sx.x), yAxis*(sy.y - sy.x));
		mPivotHandle.position = mFrame.offs + mFrame.xv*0.5f + mFrame.yv*0.5f;
	}
	else
	{
		SetHandlesEnable(false);
		return;
	}

	UdateHandlesTransform();
}

void EditorFrameTool::OnCursorPressed(const Input::Cursor& cursor)
{
	if (o2EditorSceneScreen.GetSelectedActors().Count() > 0)
	{
		if (mFrame.IsPointInside(o2EditorSceneScreen.ScreenToScenePoint(cursor.mPosition)))
		{
			mIsDragging = true;
			SetHandlesEnable(false);
			HandlePressed();
			return;
		}
	}

	EditorSelectionTool::OnCursorPressed(cursor);
}

void EditorFrameTool::OnCursorReleased(const Input::Cursor& cursor)
{
	if (mIsDragging)
	{
		mIsDragging = false;
		SetHandlesEnable(true);
		HandleReleased();
	}
	else EditorSelectionTool::OnCursorReleased(cursor);
}

void EditorFrameTool::OnCursorPressBreak(const Input::Cursor& cursor)
{
	if (mIsDragging)
	{
		mIsDragging = false;
		SetHandlesEnable(true);
	}
	else EditorSelectionTool::OnCursorPressBreak(cursor);
}

void EditorFrameTool::OnCursorStillDown(const Input::Cursor& cursor)
{
	if (mIsDragging)
	{
		TransformActors(Basis::Translated(o2EditorSceneScreen.ScreenToSceneVector(cursor.mDelta)));
	}
	else EditorSelectionTool::OnCursorStillDown(cursor);
}

void EditorFrameTool::OnLeftTopHandle(const Vec2F& position)
{
	Basis transformedFrame = mFrame;
	Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mFrame;
	Vec2F delta = position - lastHandleCoords;
	Vec2F frameDeltaX = delta.Project(mFrame.xv);
	Vec2F frameDeltaY = delta.Project(mFrame.yv);

	transformedFrame.offs += frameDeltaX;
	transformedFrame.xv -= frameDeltaX;
	transformedFrame.yv += frameDeltaY;

	TransformActors(mFrame.Inverted()*transformedFrame);
}

void EditorFrameTool::OnLeftHandle(const Vec2F& position)
{
	Basis transformedFrame = mFrame;
	Vec2F lastHandleCoords = Vec2F(0.0f, 0.5f)*mFrame;
	Vec2F delta = position - lastHandleCoords;
	Vec2F frameDeltaX = delta.Project(mFrame.xv);

	transformedFrame.offs += frameDeltaX;
	transformedFrame.xv -= frameDeltaX;

	TransformActors(mFrame.Inverted()*transformedFrame);
}

void EditorFrameTool::OnLeftBottomHandle(const Vec2F& position)
{
	Basis transformedFrame = mFrame;
	Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mFrame;
	Vec2F delta = position - lastHandleCoords;
	Vec2F frameDeltaX = delta.Project(mFrame.xv);
	Vec2F frameDeltaY = delta.Project(mFrame.yv);

	transformedFrame.offs += frameDeltaX + frameDeltaY;
	transformedFrame.xv -= frameDeltaX;
	transformedFrame.yv -= frameDeltaY;

	TransformActors(mFrame.Inverted()*transformedFrame);
}

void EditorFrameTool::OnTopHandle(const Vec2F& position)
{
	Basis transformedFrame = mFrame;
	Vec2F lastHandleCoords = Vec2F(0.5f, 1.0f)*mFrame;
	Vec2F delta = position - lastHandleCoords;
	Vec2F frameDeltaY = delta.Project(mFrame.yv);

	transformedFrame.yv += frameDeltaY;

	TransformActors(mFrame.Inverted()*transformedFrame);
}

void EditorFrameTool::OnBottomHandle(const Vec2F& position)
{
	Basis transformedFrame = mFrame;
	Vec2F lastHandleCoords = Vec2F(0.5f, 0.0f)*mFrame;
	Vec2F delta = position - lastHandleCoords;
	Vec2F frameDeltaY = delta.Project(mFrame.yv);

	transformedFrame.offs += frameDeltaY;
	transformedFrame.yv -= frameDeltaY;

	TransformActors(mFrame.Inverted()*transformedFrame);
}

void EditorFrameTool::OnRightTopHandle(const Vec2F& position)
{
	Basis transformedFrame = mFrame;
	Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mFrame;
	Vec2F delta = position - lastHandleCoords;
	Vec2F frameDeltaX = delta.Project(mFrame.xv);
	Vec2F frameDeltaY = delta.Project(mFrame.yv);

	transformedFrame.xv += frameDeltaX;
	transformedFrame.yv += frameDeltaY;

	TransformActors(mFrame.Inverted()*transformedFrame);
}

void EditorFrameTool::OnRightHandle(const Vec2F& position)
{
	Basis transformedFrame = mFrame;
	Vec2F lastHandleCoords = Vec2F(1.0f, 0.5f)*mFrame;
	Vec2F delta = position - lastHandleCoords;
	Vec2F frameDeltaX = delta.Project(mFrame.xv);

	transformedFrame.xv += frameDeltaX;

	TransformActors(mFrame.Inverted()*transformedFrame);
}

void EditorFrameTool::OnRightBottomHandle(const Vec2F& position)
{
	Basis transformedFrame = mFrame;
	Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mFrame;
	Vec2F delta = position - lastHandleCoords;
	Vec2F frameDeltaX = delta.Project(mFrame.xv);
	Vec2F frameDeltaY = delta.Project(mFrame.yv);

	transformedFrame.offs += frameDeltaY;
	transformedFrame.xv += frameDeltaX;
	transformedFrame.yv -= frameDeltaY;

	TransformActors(mFrame.Inverted()*transformedFrame);
}

void EditorFrameTool::OnPivotHandle(const Vec2F& position)
{
	auto selectedActors = o2EditorSceneScreen.GetSelectedActors();
	if (selectedActors.Count() == 1)
	{
		selectedActors[0]->transform.SetWorldPivot(position);
	}
}

void EditorFrameTool::OnLeftTopRotateHandle(const Vec2F& position)
{
	Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mFrame;
	Vec2F rotatePivot = mPivotHandle.position;
	float angle = (position - rotatePivot).SignedAngle(lastHandleCoords - rotatePivot);
	Basis transform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(-angle)*Basis::Translated(rotatePivot);
	TransformActors(transform);
}

void EditorFrameTool::OnLeftBottomRotateHandle(const Vec2F& position)
{
	Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mFrame;
	Vec2F rotatePivot = mPivotHandle.position;
	float angle = (position - rotatePivot).SignedAngle(lastHandleCoords - rotatePivot);
	Basis transform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(-angle)*Basis::Translated(rotatePivot);
	TransformActors(transform);
}

void EditorFrameTool::OnRightTopRotateHandle(const Vec2F& position)
{
	Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mFrame;
	Vec2F rotatePivot = mPivotHandle.position;
	float angle = (position - rotatePivot).SignedAngle(lastHandleCoords - rotatePivot);
	Basis transform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(-angle)*Basis::Translated(rotatePivot);
	TransformActors(transform);
}

void EditorFrameTool::OnRightBottomRotateHandle(const Vec2F& position)
{
	Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mFrame;
	Vec2F rotatePivot = mPivotHandle.position;
	float angle = (position - rotatePivot).SignedAngle(lastHandleCoords - rotatePivot);
	Basis transform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(-angle)*Basis::Translated(rotatePivot);
	TransformActors(transform);
}

void EditorFrameTool::SetHandlesEnable(bool enable)
{
	mPivotHandle.enabled = enable;
	mLeftTopRotateHandle.enabled = enable;
	mLeftBottomRotateHandle.enabled = enable;
	mRightTopRotateHandle.enabled = enable;
	mRightBottomRotateHandle.enabled = enable;
	mLeftTopHandle.enabled = enable;
	mLeftHandle.enabled = enable;
	mLeftBottomHandle.enabled = enable;
	mTopHandle.enabled = enable;
	mBottomHandle.enabled = enable;
	mRightTopHandle.enabled = enable;
	mRightHandle.enabled = enable;
	mRightBottomHandle.enabled = enable;
}

void EditorFrameTool::UdateHandlesTransform()
{
	float handlesAngle = mFrame.GetAngle();

	mLeftTopHandle.position = Vec2F(0.0f, 1.0f)*mFrame;
	mLeftHandle.position = Vec2F(0.0f, 0.5f)*mFrame;
	mLeftBottomHandle.position = Vec2F(0.0f, 0.0f)*mFrame;
	mTopHandle.position = Vec2F(0.5f, 1.0f)*mFrame;
	mBottomHandle.position = Vec2F(0.5f, 0.0f)*mFrame;
	mRightTopHandle.position = Vec2F(1.0f, 1.0f)*mFrame;
	mRightHandle.position = Vec2F(1.0f, 0.5f)*mFrame;
	mRightBottomHandle.position = Vec2F(1.0f, 0.0f)*mFrame;

	mLeftTopRotateHandle.position = Vec2F(0.0f, 1.0f)*mFrame;
	mLeftBottomRotateHandle.position = Vec2F(0.0f, 0.0f)*mFrame;
	mRightTopRotateHandle.position = Vec2F(1.0f, 1.0f)*mFrame;
	mRightBottomRotateHandle.position = Vec2F(1.0f, 0.0f)*mFrame;

	mLeftTopHandle.angle = handlesAngle;
	mLeftHandle.angle = handlesAngle;
	mLeftBottomHandle.angle = handlesAngle;
	mTopHandle.angle = handlesAngle;
	mBottomHandle.angle = handlesAngle;
	mRightTopHandle.angle = handlesAngle;
	mRightHandle.angle = handlesAngle;
	mRightBottomHandle.angle = handlesAngle;

	mLeftTopRotateHandle.angle = handlesAngle + Math::PI()*0.5f;
	mLeftBottomRotateHandle.angle = handlesAngle + Math::PI();
	mRightTopRotateHandle.angle = handlesAngle;
	mRightBottomRotateHandle.angle = handlesAngle + Math::PI()*1.5f;

	mNeedRedraw = true;
}

void EditorFrameTool::HandlePressed()
{
	mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
		[](Actor* x) { return x->transform; });
}

void EditorFrameTool::HandleReleased()
{
	auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
	o2EditorApplication.DoneAction(action);
}
