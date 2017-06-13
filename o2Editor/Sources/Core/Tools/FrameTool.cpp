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
		mLeftTopRotateHandle.SetRegularSprite(mnew Sprite("ui/UI3_rotate_regular.png"));
		mLeftTopRotateHandle.SetHoverSprite(mnew Sprite("ui/UI3_rotate_hover.png"));
		mLeftTopRotateHandle.SetPressedSprite(mnew Sprite("ui/UI3_rotate_pressed.png"));
		mLeftTopRotateHandle.GetRegularSprite()->pivot = Vec2F(0, 0);
		mLeftTopRotateHandle.GetHoverSprite()->pivot = Vec2F(0, 0);
		mLeftTopRotateHandle.GetPressedSprite()->pivot = Vec2F(0, 0);
		mLeftTopRotateHandle.pixelPerfect = false;

		mLeftBottomRotateHandle = mLeftTopRotateHandle;
		mRightTopRotateHandle = mLeftTopRotateHandle;
		mRightBottomRotateHandle = mLeftTopRotateHandle;

		mLeftTopHandle.SetRegularSprite(mnew Sprite("ui/UI2_handle_regular.png"));
		mLeftTopHandle.SetHoverSprite(mnew Sprite("ui/UI2_handle_select.png"));
		mLeftTopHandle.SetPressedSprite(mnew Sprite("ui/UI2_handle_pressed.png"));
		mLeftTopHandle.pixelPerfect = false;

		mLeftBottomHandle = mLeftTopHandle;
		mRightTopHandle = mLeftTopHandle;
		mRightBottomHandle = mLeftTopHandle;

		mLeftHandle.SetRegularSprite(mnew Sprite("ui/UI2_handle_side_regular.png"));
		mLeftHandle.SetHoverSprite(mnew Sprite("ui/UI2_handle_side_select.png"));
		mLeftHandle.SetPressedSprite(mnew Sprite("ui/UI2_handle_side_pressed.png"));
		mLeftHandle.pixelPerfect = false;

		mTopHandle = mLeftHandle;
		mBottomHandle = mLeftHandle;
		mRightHandle = mLeftHandle;

		mPivotHandle.SetRegularSprite(mnew Sprite("ui/UI2_pivot.png"));
		mPivotHandle.SetHoverSprite(mnew Sprite("ui/UI2_pivot_select.png"));
		mPivotHandle.SetPressedSprite(mnew Sprite("ui/UI2_pivot_pressed.png"));
		mPivotHandle.checkPositionFunc = Function<Vec2F(const Vec2F&)>(this, &FrameTool::CheckPivotSnapping);

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

		mTopHandle.checkPositionFunc = Function<Vec2F(const Vec2F&)>(this, &FrameTool::CheckTopSnapping);
		mLeftHandle.checkPositionFunc = Function<Vec2F(const Vec2F&)>(this, &FrameTool::CheckLeftSnapping);
		mBottomHandle.checkPositionFunc = Function<Vec2F(const Vec2F&)>(this, &FrameTool::CheckBottomSnapping);
		mRightHandle.checkPositionFunc = Function<Vec2F(const Vec2F&)>(this, &FrameTool::CheckRightSnapping);
		mLeftTopHandle.checkPositionFunc = Function<Vec2F(const Vec2F&)>(this, &FrameTool::CheckLeftTopSnapping);
		mLeftBottomHandle.checkPositionFunc = Function<Vec2F(const Vec2F&)>(this, &FrameTool::CheckLeftBottomSnapping);
		mRightTopHandle.checkPositionFunc = Function<Vec2F(const Vec2F&)>(this, &FrameTool::CheckRightTopSnapping);
		mRightBottomHandle.checkPositionFunc = Function<Vec2F(const Vec2F&)>(this, &FrameTool::CheckRightBottomSnapping);

		SetHandlesEnable(false);
	}

	FrameTool::~FrameTool()
	{}

	void FrameTool::DrawScene()
	{
		SelectionTool::DrawScene();

		if (o2EditorSceneScreen.GetSelectedActors().Count() > 0)
			o2Render.DrawBasis(mFrame, mFrameColor, mFrameColor, mFrameColor);

		DrawSnapLines();
	}

	void FrameTool::DrawSnapLines()
	{
		for (auto& line : mSnapLines)
			o2Render.DrawLine(line.begin, line.end, line.color);
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
		UpdateHandlesTransform();
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

			const Vec2F cp[4] ={ Vec2F(0, 0), Vec2F(0, 1), Vec2F(1, 0), Vec2F(1, 1) };
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

		UpdateHandlesTransform();
	}

	void FrameTool::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (o2EditorSceneScreen.GetSelectedActors().Count() > 0)
		{
			if (mFrame.IsPointInside(o2EditorSceneScreen.ScreenToScenePoint(cursor.position)))
			{
				mIsDragging = true;
				mBeginDraggingFrame = mFrame;
				mBeginDraggingOffset = o2EditorSceneScreen.ScreenToLocalPoint(cursor.position) - mFrame.offs;

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
			mSnapLines.Clear();

			Vec2F cursorPos = o2EditorSceneScreen.ScreenToLocalPoint(cursor.position);

			if (o2Input.IsKeyDown(VK_CONTROL))
			{
				Basis preTransformed(cursorPos - mBeginDraggingOffset, mBeginDraggingFrame.xv, mBeginDraggingFrame.yv);

				cursorPos = CalculateSnapOffset(cursorPos, preTransformed, 
				{ Vec2F(0, 0), Vec2F(0, 1), Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(1, 0), Vec2F(1, 1) }, preTransformed.xv.Normalized(),
				{ Vec2F(0, 0), Vec2F(1, 0), Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, 1), Vec2F(1, 1) }, preTransformed.yv.Normalized());
			}

			Vec2F delta = cursorPos - mBeginDraggingOffset;
			Basis transformed(delta, mBeginDraggingFrame.xv, mBeginDraggingFrame.yv);

			if (o2Input.IsKeyDown(VK_SHIFT))
			{
				if (Math::Abs(transformed.offs.x - mBeginDraggingFrame.offs.x) > Math::Abs(transformed.offs.y - mBeginDraggingFrame.offs.y))
					transformed.offs.y = mBeginDraggingFrame.offs.y;
				else
					transformed.offs.x = mBeginDraggingFrame.offs.x;
			}

			TransformActors(mFrame.Inverted()*transformed);
		}
		else SelectionTool::OnCursorStillDown(cursor);
	}

	void FrameTool::OnLeftTopHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetLeftTopHandleTransformed(position));
	}

	void FrameTool::OnLeftHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetLeftHandleTransformed(position));
	}

	void FrameTool::OnLeftBottomHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetLeftBottomHandleTransformed(position));
	}

	void FrameTool::OnTopHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetTopHandleTransformed(position));
	}

	void FrameTool::OnBottomHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetBottomHandleTransformed(position));
	}

	void FrameTool::OnRightTopHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetRightTopHandleTransformed(position));
	}

	void FrameTool::OnRightHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetRightHandleTransformed(position));
	}

	void FrameTool::OnRightBottomHandle(const Vec2F& position)
	{
		TransformActors(mFrame.Inverted()*GetRightBottomHandleTransformed(position));
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
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameTool::OnLeftBottomRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameTool::OnRightTopRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameTool::OnRightBottomRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameTool::OnRotateHandle(const Vec2F& position, Vec2F lastHandleCoords)
	{
		Vec2F rotatePivot = mPivotHandle.position;
		float angle = (position - rotatePivot).SignedAngle(lastHandleCoords - rotatePivot);
		Basis transform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(-angle)*Basis::Translated(rotatePivot);
		Basis transformed = mFrame*transform;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float angle = Math::Rad2deg(transformed.GetAngle());
			float step = 15.0f;
			float targetAngle = Math::Round(angle/step)*step;
			float delta = targetAngle - angle;

			Basis deltaTransform = Basis::Translated(rotatePivot*-1.0f)*Basis::Rotated(Math::Deg2rad(delta))
				*Basis::Translated(rotatePivot);

			transformed = transformed*deltaTransform;
		}

		TransformActors(mFrame.Inverted()*transformed);
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

	void FrameTool::UpdateHandlesTransform()
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

		//mTopHandle.regularSprite->size = Vec2F(mTopHandle.regularSprite->size->x, frameSize.x);

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

		mLeftTopHandle.angle = handlesAngle + Math::PI()*0.5f;
		mLeftHandle.angle = handlesAngle + Math::PI();
		mLeftBottomHandle.angle = handlesAngle + Math::PI();
		mTopHandle.angle = handlesAngle + Math::PI()*0.5f;
		mBottomHandle.angle = handlesAngle - Math::PI()*0.5f;
		mRightTopHandle.angle = handlesAngle;
		mRightHandle.angle = handlesAngle;
		mRightBottomHandle.angle = handlesAngle - Math::PI()*0.5f;

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

		mBeginDraggingFrame = mFrame;
	}

	void FrameTool::HandleReleased()
	{
		auto action = mnew EditorActorsTransformAction(o2EditorSceneScreen.GetTopSelectedActors(), mBeforeTransforms);
		o2EditorApplication.DoneAction(action);

		mSnapLines.Clear();
	}

	Basis FrameTool::GetLeftTopHandleTransformed(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.offs += frameDeltaX;
		transformedFrame.xv -= frameDeltaX;
		transformedFrame.yv += frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();

			if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
			{
				Vec2F xd = transformedFrame.xv.Normalized()*
					(transformedFrame.yv.Length()*aspect - transformedFrame.xv.Length());

				transformedFrame.offs -= xd;
				transformedFrame.xv += xd;
			}
			else transformedFrame.yv = transformedFrame.yv.Normalized()*transformedFrame.xv.Length()/aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetLeftHandleTransformed(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.5f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);

		transformedFrame.offs += frameDeltaX;
		transformedFrame.xv -= frameDeltaX;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();
			transformedFrame.yv = transformedFrame.yv.Normalized()*transformedFrame.xv.Length()/aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetLeftBottomHandleTransformed(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.offs += frameDeltaX + frameDeltaY;
		transformedFrame.xv -= frameDeltaX;
		transformedFrame.yv -= frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();

			if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
			{
				Vec2F xd = transformedFrame.xv.Normalized()*
					(transformedFrame.yv.Length()*aspect - transformedFrame.xv.Length());

				transformedFrame.offs -= xd;
				transformedFrame.xv += xd;
			}
			else
			{
				Vec2F yd = transformedFrame.yv.Normalized()*
					(transformedFrame.xv.Length()/aspect - transformedFrame.yv.Length());

				transformedFrame.offs -= yd;
				transformedFrame.yv += yd;
			}
		}

		return transformedFrame;
	}

	Basis FrameTool::GetTopHandleTransformed(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.5f, 1.0f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.yv += frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();
			transformedFrame.xv = transformedFrame.xv.Normalized()*transformedFrame.yv.Length()*aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetBottomHandleTransformed(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.5f, 0.0f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.offs += frameDeltaY;
		transformedFrame.yv -= frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();
			transformedFrame.xv = transformedFrame.xv.Normalized()*transformedFrame.yv.Length()*aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetRightTopHandleTransformed(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.xv += frameDeltaX;
		transformedFrame.yv += frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();

			if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
				transformedFrame.xv = transformedFrame.xv.Normalized()*transformedFrame.yv.Length()*aspect;
			else
				transformedFrame.yv = transformedFrame.yv.Normalized()*transformedFrame.xv.Length()/aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetRightHandleTransformed(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.5f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);

		transformedFrame.xv += frameDeltaX;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();
			transformedFrame.yv = transformedFrame.yv.Normalized()*transformedFrame.xv.Length()/aspect;
		}

		return transformedFrame;
	}

	Basis FrameTool::GetRightBottomHandleTransformed(const Vec2F& position)
	{
		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mFrame;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.offs += frameDeltaY;
		transformedFrame.xv += frameDeltaX;
		transformedFrame.yv -= frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();

			if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
				transformedFrame.xv = transformedFrame.xv.Normalized()*transformedFrame.yv.Length()*aspect;
			else
			{
				Vec2F yd = transformedFrame.yv.Normalized()*
					(transformedFrame.xv.Length()/aspect - transformedFrame.yv.Length());

				transformedFrame.offs -= yd;
				transformedFrame.yv += yd;
			}
		}

		return transformedFrame;
	}

	Vec2F FrameTool::CheckPivotSnapping(const Vec2F& point)
	{
		if (mPivotHandle.IsPressed())
			mSnapLines.Clear();
		else
			return point;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			return point;

		Vector<Vec2F> snapPoints =
		{
			Vec2F(0.0f, 0.0f), Vec2F(0.0f, 0.5f), Vec2F(0.0f, 1.0f),
			Vec2F(0.5f, 0.0f), Vec2F(0.5f, 0.5f), Vec2F(0.5f, 1.0f),
			Vec2F(1.0f, 0.0f), Vec2F(1.0f, 0.5f), Vec2F(1.0f, 1.0f)
		};

		Vector<Vector<Vector<Vec2F>>> snapPointsLines =
		{
			{ { Vec2F(0.0f, 0.0f), Vec2F(0.5f, 0.0f) },{ Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.0f) } },
			{ { Vec2F(0.0f, 0.25f), Vec2F(0.0f, 0.75f) },{ Vec2F(0.0f, 0.5f), Vec2F(0.25f, 0.5f) } },
			{ { Vec2F(0.0f, 1.0f), Vec2F(0.5f, 1.0f) },{ Vec2F(0.0f, 1.0f), Vec2F(0.0f, 0.5f) } },

			{ { Vec2F(0.25f, 0.0f), Vec2F(0.75f, 0.0f) },{ Vec2F(0.5f, 0.0), Vec2F(0.5f, 0.25f) } },
			{ { Vec2F(0.25f, 0.5f), Vec2F(0.75f, 0.5f) },{ Vec2F(0.5f, 0.25f), Vec2F(0.5f, 0.75f) } },
			{ { Vec2F(0.25f, 1.0f), Vec2F(0.75f, 1.0f) },{ Vec2F(0.5f, 1.0f), Vec2F(0.5f, 0.75f) } },

			{ { Vec2F(0.5f, 0.0f), Vec2F(1.0f, 0.0f) },{ Vec2F(1.0f, 0.0f), Vec2F(1.0f, 0.5f) } },
			{ { Vec2F(1.0f, 0.25f), Vec2F(1.0f, 0.75f) },{ Vec2F(1.0f, 0.5f), Vec2F(0.75f, 0.5f) } },
			{ { Vec2F(1.0f, 1.0f), Vec2F(0.5f, 1.0f) },{ Vec2F(1.0f, 1.0f), Vec2F(1.0f, 0.5f) } }
		};

		const float snapThresholdPx = 5.0f;
		Vec2F screenpoint = o2EditorSceneScreen.SceneToScreenPoint(point);

		int i = 0;
		for (auto snapPoint : snapPoints)
		{
			Vec2F framePoint = o2EditorSceneScreen.LocalToScreenPoint(snapPoint*mFrame);

			if ((screenpoint - framePoint).Length() < snapThresholdPx)
			{
				mSnapLines.Add(SnapLine(snapPointsLines[i][0][0]*mFrame, snapPointsLines[i][0][1]*mFrame, Color4::Red()));
				mSnapLines.Add(SnapLine(snapPointsLines[i][1][0]*mFrame, snapPointsLines[i][1][1]*mFrame, Color4::Red()));

				return o2EditorSceneScreen.ScreenToLocalPoint(framePoint);
			}

			i++;
		}

		return point;
	}

	Vec2F FrameTool::CheckTopSnapping(const Vec2F& point)
	{
		if (mTopHandle.IsPressed())
			mSnapLines.Clear();
		else
			return point;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			return point;

		Basis transformedFrame = GetTopHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame, 
		{}, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 1), Vec2F(1, 1) }, transformedFrame.yv.Normalized());

		return snapped;
	}

	Vec2F FrameTool::CheckBottomSnapping(const Vec2F& point)
	{
		if (mBottomHandle.IsPressed())
			mSnapLines.Clear();
		else
			return point;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			return point;

		Basis transformedFrame = GetBottomHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{}, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 0), Vec2F(1, 0) }, transformedFrame.yv.Normalized());

		return snapped;
	}

	Vec2F FrameTool::CheckLeftSnapping(const Vec2F& point)
	{
		if (mLeftHandle.IsPressed())
			mSnapLines.Clear();
		else
			return point;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			return point;

		Basis transformedFrame = GetLeftHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(0, 0), Vec2F(0, 1) }, transformedFrame.xv.Normalized(),
		{}, transformedFrame.yv.Normalized());

		return snapped;
	}

	Vec2F FrameTool::CheckRightSnapping(const Vec2F& point)
	{
		if (mRightHandle.IsPressed())
			mSnapLines.Clear();
		else
			return point;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			return point;

		Basis transformedFrame = GetRightHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(1, 0), Vec2F(1, 1) }, transformedFrame.xv.Normalized(),
		{}, transformedFrame.yv.Normalized());

		return snapped;
	}

	Vec2F FrameTool::CheckLeftTopSnapping(const Vec2F& point)
	{
		if (mLeftTopHandle.IsPressed())
			mSnapLines.Clear();
		else
			return point;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			return point;

		Basis transformedFrame = GetLeftTopHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(0, 0), Vec2F(0, 1) }, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 1), Vec2F(1, 1) }, transformedFrame.yv.Normalized());

		return snapped;
	}

	Vec2F FrameTool::CheckLeftBottomSnapping(const Vec2F& point)
	{
		if (mLeftBottomHandle.IsPressed())
			mSnapLines.Clear();
		else
			return point;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			return point;

		Basis transformedFrame = GetLeftBottomHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(0, 0), Vec2F(0, 1) }, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 0), Vec2F(1, 0) }, transformedFrame.yv.Normalized());

		return snapped;
	}

	Vec2F FrameTool::CheckRightTopSnapping(const Vec2F& point)
	{
		if (mRightTopHandle.IsPressed())
			mSnapLines.Clear();
		else
			return point;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			return point;

		Basis transformedFrame = GetRightTopHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(1, 0), Vec2F(1, 1) }, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 1), Vec2F(1, 1) }, transformedFrame.yv.Normalized());

		return snapped;
	}

	Vec2F FrameTool::CheckRightBottomSnapping(const Vec2F& point)
	{
		if (mRightBottomHandle.IsPressed())
			mSnapLines.Clear();
		else
			return point;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			return point;

		Basis transformedFrame = GetRightBottomHandleTransformed(point);

		Vec2F snapped = CalculateSnapOffset(point, transformedFrame,
		{ Vec2F(1, 0), Vec2F(1, 1) }, transformedFrame.xv.Normalized(),
		{ Vec2F(0, 0), Vec2F(1, 0) }, transformedFrame.yv.Normalized());

		return snapped;
	}

	Vec2F FrameTool::CalculateSnapOffset(const Vec2F& point, const Basis& frame, 
										 const Vector<Vec2F>& xLines, const Vec2F& xNormal,
										 const Vector<Vec2F>& yLines, const Vec2F& yNormal)
	{
		const float pxThreshold = 5.0f;
		const float sameSnapDistanceThreshold = 0.01f;
		const Camera& camera = o2EditorSceneScreen.GetCamera();

		Vector<Vector<Vec2F>> snapLines =
		{
			{ Vec2F(0.0f, 0.0f), Vec2F(1.0f, 0.0f) },
			{ Vec2F(1.0f, 0.0f), Vec2F(1.0f, 1.0f) },
			{ Vec2F(1.0f, 1.0f), Vec2F(0.0f, 1.0f) },
			{ Vec2F(0.0f, 1.0f), Vec2F(0.0f, 0.0f) },
			{ Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f) },
			{ Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f) }
		};

		Vector<Vec2F> worldSnapLines;
		for (auto actor : o2Scene.GetAllActors())
		{
			if (o2EditorSceneScreen.GetSelectedActors().Contains(actor))
				continue;

			Basis actorBasis = actor->transform.GetWorldBasis();

			for (auto snapLine : snapLines)
			{
				Vec2F actorLineBegin = snapLine[0]*actorBasis;
				Vec2F actorLineEnd = snapLine[1]*actorBasis;
				Vec2F actorLineVec = actorLineEnd - actorLineBegin;

				if (actorLineVec.SqrLength() < 0.1f)
					continue;

				worldSnapLines.Add(actorLineBegin);
				worldSnapLines.Add(actorLineEnd);
			}
		}

		Vec2F offset;
		Vector<Vec2F> localSnapLines;

		for (int i = 0; i < 2; i++)
		{
			const Vector<Vec2F>& currentAxisLines = i == 0 ? xLines : yLines;
			Vec2F currentAxisNormal = i == 0 ? xNormal : yNormal;

			bool axisSnapLineFound = false;
			Vec2F axisSnapOffset;
			float axisSnapDistance = FLT_MAX;
			Vec2F axisSnapBegin, axisSnapEnd;

			for (int j = 0; j < currentAxisLines.Count(); j += 2)
			{
				Vec2F lineBeg = currentAxisLines[j]*frame;
				Vec2F lineEnd = currentAxisLines[j + 1]*frame;
				Vec2F lineVec = lineEnd - lineBeg;

				for (int k = 0; k < worldSnapLines.Count(); k += 2)
				{
					Vec2F actorLineBegin = worldSnapLines[k];
					Vec2F actorLineEnd = worldSnapLines[k + 1];
					Vec2F actorLineVec = actorLineEnd - actorLineBegin;

					if (!lineVec.IsParallel(actorLineVec))
						continue;

					float projDistance = (actorLineBegin - lineBeg).Dot(currentAxisNormal);
					float screenProjDistance = Math::Abs(projDistance/camera.GetScale().x);

					if (screenProjDistance < pxThreshold && screenProjDistance < axisSnapDistance)
					{
						axisSnapOffset = currentAxisNormal*projDistance;
						axisSnapDistance = screenProjDistance;
						axisSnapBegin = currentAxisLines[j];
						axisSnapEnd = currentAxisLines[j + 1];

						axisSnapLineFound = true;
					}
				}
			}

			if (!axisSnapLineFound)
				continue;

			for (int j = 0; j < currentAxisLines.Count(); j += 2)
			{
				Vec2F lineBeg = currentAxisLines[j]*frame + axisSnapOffset;
				Vec2F lineEnd = currentAxisLines[j + 1]*frame + axisSnapOffset;
				Vec2F lineVec = lineEnd - lineBeg;

				bool found = false;

				for (int k = 0; k < worldSnapLines.Count(); k += 2)
				{
					Vec2F actorLineBegin = worldSnapLines[k];
					Vec2F actorLineEnd = worldSnapLines[k + 1];
					Vec2F actorLineVec = actorLineEnd - actorLineBegin;

					if (!lineVec.IsParallel(actorLineVec))
						continue;

					float projDistance = Math::Abs((actorLineBegin - lineBeg).Dot(currentAxisNormal));

					if (projDistance < sameSnapDistanceThreshold)
					{
						mSnapLines.Add(SnapLine(actorLineBegin, actorLineEnd, Color4::Red()));

						found = true;
					}
				}

				if (found)
				{
					localSnapLines.Add(currentAxisLines[j]);
					localSnapLines.Add(currentAxisLines[j + 1]);
				}
			}

			offset += axisSnapOffset;
		}

		Basis frameWithOffset = frame;
		frameWithOffset.Translate(offset);

		for (int i = 0; i < localSnapLines.Count(); i += 2)
			mSnapLines.Add(SnapLine(localSnapLines[i]*frameWithOffset, localSnapLines[i + 1]*frameWithOffset, Color4::Red()));

		return point + offset;
	}

}

CLASS_META(Editor::FrameTool)
{
	BASE_CLASS(Editor::SelectionTool);

	PROTECTED_FIELD(mFrameColor);
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
	PROTECTED_FIELD(mBeginDraggingFrame);
	PROTECTED_FIELD(mBeginDraggingOffset);
	PROTECTED_FIELD(mIsDragging);
	PROTECTED_FIELD(mChangedFromThis);
	PROTECTED_FIELD(mBeforeTransforms);
	PROTECTED_FIELD(mSnapLines);

	PROTECTED_FUNCTION(void, DrawScene);
	PROTECTED_FUNCTION(void, DrawSnapLines);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnSceneChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, OnActorsSelectionChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyStayDown, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, TransformActors, const Basis&);
	PROTECTED_FUNCTION(void, TransformActorsWithAction, const Basis&);
	PROTECTED_FUNCTION(void, UpdateSelectionFrame);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnLeftTopHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnLeftHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnLeftBottomHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnTopHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnBottomHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRightTopHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRightHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRightBottomHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnPivotHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnLeftTopRotateHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnLeftBottomRotateHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRightTopRotateHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRightBottomRotateHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRotateHandle, const Vec2F&, Vec2F);
	PROTECTED_FUNCTION(void, SetHandlesEnable, bool);
	PROTECTED_FUNCTION(void, UpdateHandlesTransform);
	PROTECTED_FUNCTION(void, HandlePressed);
	PROTECTED_FUNCTION(void, HandleReleased);
	PROTECTED_FUNCTION(Basis, GetLeftTopHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetLeftHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetLeftBottomHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetTopHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetBottomHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetRightTopHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetRightHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetRightBottomHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckPivotSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CalculateSnapOffset, const Vec2F&, const Basis&, const Vector<Vec2F>&, const Vec2F&, const Vector<Vec2F>&, const Vec2F&);
}
END_META;
