#include "FrameTool.h"

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
	FrameTool::FrameTool()
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
		mLeftTopRotateHandle.pixelPerfect = false;

		mLeftBottomRotateHandle = mLeftTopRotateHandle;
		mRightTopRotateHandle = mLeftTopRotateHandle;
		mRightBottomRotateHandle = mLeftTopRotateHandle;

		mLeftTopHandle.regularSprite = mnew Sprite("ui/UI2_handle_regular.png");
		mLeftTopHandle.hoverSprite = mnew Sprite("ui/UI2_handle_select.png");
		mLeftTopHandle.pressedSprite = mnew Sprite("ui/UI2_handle_pressed.png");
		mLeftTopHandle.pixelPerfect = false;

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

		mLeftTopHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnLeftTopHandle);
		mLeftHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnLeftHandle);
		mLeftBottomHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnLeftBottomHandle);
		mTopHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnTopHandle);
		mBottomHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnBottomHandle);
		mRightTopHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnRightTopHandle);
		mRightHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnRightHandle);
		mRightBottomHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnRightBottomHandle);
		mPivotHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnPivotHandle);
		mLeftTopRotateHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnLeftTopRotateHandle);
		mLeftBottomRotateHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnLeftBottomRotateHandle);
		mRightTopRotateHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnRightTopRotateHandle);
		mRightBottomRotateHandle.onChangedPos = Function<void(const Vec2F&)>(this, &FrameTool::OnRightBottomRotateHandle);

		mLeftTopHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mLeftHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mLeftBottomHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mTopHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mBottomHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mRightTopHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mRightHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mRightBottomHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mPivotHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mLeftTopRotateHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mLeftBottomRotateHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mRightTopRotateHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);
		mRightBottomRotateHandle.onPressed = Function<void()>(this, &FrameTool::HandlePressed);

		mLeftTopHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mLeftHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mLeftBottomHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mTopHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mBottomHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mRightTopHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mRightHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mRightBottomHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mPivotHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mLeftTopRotateHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mLeftBottomRotateHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mRightTopRotateHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);
		mRightBottomRotateHandle.onReleased = Function<void()>(this, &FrameTool::HandleReleased);

		SetHandlesEnable(false);
	}

	FrameTool::~FrameTool()
	{}

	void FrameTool::DrawScene()
	{
		SelectionTool::DrawScene();

		if (o2EditorSceneScreen.GetSelectedActors().Count() > 0)
		{
			o2Render.DrawBasis(mFrame, mHandleRegularColor, mHandleRegularColor, mHandleRegularColor);
		}
	}

	void FrameTool::OnEnabled()
	{
		SetHandlesEnable(true);
		UpdateSelectionFrame();
	}

	void FrameTool::OnDisabled()
	{
		SetHandlesEnable(false);
	}

	void FrameTool::OnSceneChanged(Vector<Actor*> changedActors)
	{
		mNeedRedraw = true;
		if (mChangedFromThis)
		{
			mChangedFromThis = false;
		}
		else UpdateSelectionFrame();
	}

	void FrameTool::OnActorsSelectionChanged(Vector<Actor*> actors)
	{
		UpdateSelectionFrame();
	}

	void FrameTool::OnKeyPressed(const Input::Key& key)
	{
		if (!o2EditorTree.GetActorsTree()->IsFocused())
			return;

		if (key == VK_LEFT)
			TransformActorsWithAction(Basis::Translated(Vec2F::Left()));

		if (key == VK_RIGHT)
			TransformActorsWithAction(Basis::Translated(Vec2F::Right()));

		if (key == VK_UP)
			TransformActorsWithAction(Basis::Translated(Vec2F::Up()));

		if (key == VK_DOWN)
			TransformActorsWithAction(Basis::Translated(Vec2F::Down()));

		SelectionTool::OnKeyPressed(key);
	}

	void FrameTool::OnKeyStayDown(const Input::Key& key)
	{
		if (!o2EditorTree.GetActorsTree()->IsFocused())
			return;

		if (key.pressedTime < 0.3f)
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

	void FrameTool::OnKeyReleased(const Input::Key& key)
	{}

	void FrameTool::TransformActors(const Basis& transform)
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

	void FrameTool::TransformActorsWithAction(const Basis& transform)
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
			[](Actor* x) { return x->transform; });

		TransformActors(transform);

		auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
		o2EditorApplication.DoneAction(action);
	}

	void FrameTool::UpdateSelectionFrame()
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

	void FrameTool::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (o2EditorSceneScreen.GetSelectedActors().Count() > 0)
		{
			if (mFrame.IsPointInside(o2EditorSceneScreen.ScreenToScenePoint(cursor.position)))
			{
				mIsDragging = true;
				SetHandlesEnable(false);
				HandlePressed();
				return;
			}
		}

		SelectionTool::OnCursorPressed(cursor);
	}

	void FrameTool::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			mIsDragging = false;
			SetHandlesEnable(true);
			HandleReleased();
		}
		else SelectionTool::OnCursorReleased(cursor);
	}

	void FrameTool::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			mIsDragging = false;
			SetHandlesEnable(true);
		}
		else SelectionTool::OnCursorPressBreak(cursor);
	}

	void FrameTool::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			TransformActors(Basis::Translated(o2EditorSceneScreen.ScreenToSceneVector(cursor.delta)));
		}
		else SelectionTool::OnCursorStillDown(cursor);
	}

	void FrameTool::OnLeftTopHandle(const Vec2F& position)
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

	void FrameTool::OnLeftHandle(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.5f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);

		transformedFrame.offs += frameDeltaX;
		transformedFrame.xv -= frameDeltaX;

		TransformActors(mFrame.Inverted()*transformedFrame);
	}

	void FrameTool::OnLeftBottomHandle(const Vec2F& position)
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

	void FrameTool::OnTopHandle(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.5f, 1.0f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.yv += frameDeltaY;

		TransformActors(mFrame.Inverted()*transformedFrame);
	}

	void FrameTool::OnBottomHandle(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.5f, 0.0f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.offs += frameDeltaY;
		transformedFrame.yv -= frameDeltaY;

		TransformActors(mFrame.Inverted()*transformedFrame);
	}

	void FrameTool::OnRightTopHandle(const Vec2F& position)
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

	void FrameTool::OnRightHandle(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.5f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);

		transformedFrame.xv += frameDeltaX;

		TransformActors(mFrame.Inverted()*transformedFrame);
	}

	void FrameTool::OnRightBottomHandle(const Vec2F& position)
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

	void FrameTool::OnPivotHandle(const Vec2F& position)
	{
		auto selectedActors = o2EditorSceneScreen.GetSelectedActors();
		if (selectedActors.Count() == 1)
		{
			selectedActors[0]->transform.SetWorldPivot(position);
		}
	}

	void FrameTool::OnLeftTopRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mFrame;
		Vec2F rotatePivot = mPivotHandle.position;
		float angle = (position - rotatePivot).SignedAngle(lastHandleCoords - rotatePivot);
		Basis transform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(-angle)*Basis::Translated(rotatePivot);
		TransformActors(transform);
	}

	void FrameTool::OnLeftBottomRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mFrame;
		Vec2F rotatePivot = mPivotHandle.position;
		float angle = (position - rotatePivot).SignedAngle(lastHandleCoords - rotatePivot);
		Basis transform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(-angle)*Basis::Translated(rotatePivot);
		TransformActors(transform);
	}

	void FrameTool::OnRightTopRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mFrame;
		Vec2F rotatePivot = mPivotHandle.position;
		float angle = (position - rotatePivot).SignedAngle(lastHandleCoords - rotatePivot);
		Basis transform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(-angle)*Basis::Translated(rotatePivot);
		TransformActors(transform);
	}

	void FrameTool::OnRightBottomRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mFrame;
		Vec2F rotatePivot = mPivotHandle.position;
		float angle = (position - rotatePivot).SignedAngle(lastHandleCoords - rotatePivot);
		Basis transform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(-angle)*Basis::Translated(rotatePivot);
		TransformActors(transform);
	}

	void FrameTool::SetHandlesEnable(bool enable)
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

	void FrameTool::UdateHandlesTransform()
	{
		float handlesAngle = mFrame.GetAngle();

		auto getHandleType = [&](const Vec2F& pos) {
			Vec2F v = pos - Vec2F(0.5f, 0.5f)*mFrame;
			float angle = Math::Rad2deg(v.Angle(Vec2F::Right()));

			if (angle < 22.5f || angle > 337.5f)
				return CursorType::SizeWE;
			else if (angle > 22.5f && angle < 67.5f)
				return CursorType::SizeNwSe;
			else if (angle > 67.5f && angle < 112.5f)
				return CursorType::SizeNS;
			else if (angle > 112.5f && angle < 157.5f)
				return CursorType::SizeNeSw;
			else if (angle > 157.5f && angle < 202.5f)
				return CursorType::SizeWE;
			else if (angle > 202.5f && angle < 247.5f)
				return CursorType::SizeNwSe;
			else if (angle > 247.5f && angle < 292.5f)
				return CursorType::SizeNS;
			else if (angle > 292.5f && angle < 337.5f)
				return CursorType::SizeNeSw;

			return CursorType::SizeNeSw;
		};

		mLeftTopHandle.position = Vec2F(0.0f, 1.0f)*mFrame;
		mLeftHandle.position = Vec2F(0.0f, 0.5f)*mFrame;
		mLeftBottomHandle.position = Vec2F(0.0f, 0.0f)*mFrame;
		mTopHandle.position = Vec2F(0.5f, 1.0f)*mFrame;
		mBottomHandle.position = Vec2F(0.5f, 0.0f)*mFrame;
		mRightTopHandle.position = Vec2F(1.0f, 1.0f)*mFrame;
		mRightHandle.position = Vec2F(1.0f, 0.5f)*mFrame;
		mRightBottomHandle.position = Vec2F(1.0f, 0.0f)*mFrame;

		mLeftTopHandle.cursorType = getHandleType(mLeftTopHandle.GetPosition());
		mLeftHandle.cursorType = getHandleType(mLeftHandle.GetPosition());
		mLeftBottomHandle.cursorType = getHandleType(mLeftBottomHandle.GetPosition());
		mTopHandle.cursorType = getHandleType(mTopHandle.GetPosition());
		mBottomHandle.cursorType = getHandleType(mBottomHandle.GetPosition());
		mRightTopHandle.cursorType = getHandleType(mRightTopHandle.GetPosition());
		mRightHandle.cursorType = getHandleType(mRightHandle.GetPosition());
		mRightBottomHandle.cursorType = getHandleType(mRightBottomHandle.GetPosition());

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

	void FrameTool::HandlePressed()
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedActors().Select<ActorTransform>(
			[](Actor* x) { return x->transform; });
	}

	void FrameTool::HandleReleased()
	{
		auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
		o2EditorApplication.DoneAction(action);
	}

}
 
CLASS_META(Editor::FrameTool)
{
	BASE_CLASS(Editor::SelectionTool);

	PROTECTED_FIELD(mHandleRegularColor);
	PROTECTED_FIELD(mHandleSelectColor);
	PROTECTED_FIELD(mHandlePressedColor);
	PROTECTED_FIELD(mFrameHandlesSize);
	PROTECTED_FIELD(mHandlesRotateSize);
	PROTECTED_FIELD(mLeftTopRotateHandle);
	PROTECTED_FIELD(mLeftBottomRotateHandle);
	PROTECTED_FIELD(mRightTopRotateHandle);
	PROTECTED_FIELD(mRightBottomRotateHandle);
	PROTECTED_FIELD(mLeftTopHandle);
	PROTECTED_FIELD(mLeftHandle);
	PROTECTED_FIELD(mLeftBottomHandle);
	PROTECTED_FIELD(mTopHandle);
	PROTECTED_FIELD(mBottomHandle);
	PROTECTED_FIELD(mRightTopHandle);
	PROTECTED_FIELD(mRightHandle);
	PROTECTED_FIELD(mRightBottomHandle);
	PROTECTED_FIELD(mPivotHandle);
	PROTECTED_FIELD(mFrame);
	PROTECTED_FIELD(mIsDragging);
	PROTECTED_FIELD(mChangedFromThis);
	PROTECTED_FIELD(mBeforeTransforms);

	PUBLIC_FUNCTION(void, DrawScene);
	PUBLIC_FUNCTION(void, OnEnabled);
	PUBLIC_FUNCTION(void, OnDisabled);
	PUBLIC_FUNCTION(void, OnSceneChanged, Vector<Actor*>);
	PUBLIC_FUNCTION(void, OnActorsSelectionChanged, Vector<Actor*>);
	PUBLIC_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PUBLIC_FUNCTION(void, OnKeyStayDown, const Input::Key&);
	PUBLIC_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PUBLIC_FUNCTION(void, TransformActors, const Basis&);
	PUBLIC_FUNCTION(void, TransformActorsWithAction, const Basis&);
	PUBLIC_FUNCTION(void, UpdateSelectionFrame);
	PUBLIC_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnLeftTopHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnLeftHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnLeftBottomHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnTopHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnBottomHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnRightTopHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnRightHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnRightBottomHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnPivotHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnLeftTopRotateHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnLeftBottomRotateHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnRightTopRotateHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, OnRightBottomRotateHandle, const Vec2F&);
	PUBLIC_FUNCTION(void, SetHandlesEnable, bool);
	PUBLIC_FUNCTION(void, UdateHandlesTransform);
	PUBLIC_FUNCTION(void, HandlePressed);
	PUBLIC_FUNCTION(void, HandleReleased);
}
END_META;
 