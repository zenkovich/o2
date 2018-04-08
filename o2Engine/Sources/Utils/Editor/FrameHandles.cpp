#include "stdafx.h"
#include "FrameHandles.h"

#include "Render/Render.h"
#include "Render/Sprite.h"

namespace o2
{
	FrameHandles::FrameHandles()
	{
		Vec2F rotateHandleSize = Vec2F(mHandlesRotateSize, mHandlesRotateSize);
		Vec2F frameHandleSize = Vec2F(mFrameHandlesSize, mFrameHandlesSize);

		mLeftTopRotateHandle.SetRegularSprite(mnew Sprite(Color4(0, 0, 0, 20)));
		mLeftTopRotateHandle.SetHoverSprite(mnew Sprite(Color4(0, 0, 0, 10)));
		mLeftTopRotateHandle.SetPressedSprite(mnew Sprite(Color4(0, 0, 0, 50)));
		mLeftTopRotateHandle.GetRegularSprite()->size = rotateHandleSize;
		mLeftTopRotateHandle.GetHoverSprite()->size = rotateHandleSize;
		mLeftTopRotateHandle.GetPressedSprite()->size = rotateHandleSize;
		mLeftTopRotateHandle.GetRegularSprite()->szPivot = frameHandleSize*0.5f;
		mLeftTopRotateHandle.GetHoverSprite()->szPivot = frameHandleSize*0.5f;
		mLeftTopRotateHandle.GetPressedSprite()->szPivot = frameHandleSize*0.5f;

		mLeftBottomRotateHandle = mLeftTopRotateHandle;
		mRightTopRotateHandle = mLeftTopRotateHandle;
		mRightBottomRotateHandle = mLeftTopRotateHandle;

		mLeftTopHandle.SetRegularSprite(mnew Sprite("ui/UI2_handle_regular.png"));
		mLeftTopHandle.SetHoverSprite(mnew Sprite("ui/UI2_handle_select.png"));
		mLeftTopHandle.SetPressedSprite(mnew Sprite("ui/UI2_handle_pressed.png"));

		mLeftBottomHandle = mLeftTopHandle;
		mRightTopHandle = mLeftTopHandle;
		mRightBottomHandle = mLeftTopHandle;

		mLeftHandle.SetRegularSprite(mnew Sprite("ui/UI2_handle_side_regular.png"));
		mLeftHandle.SetHoverSprite(mnew Sprite("ui/UI2_handle_side_select.png"));
		mLeftHandle.SetPressedSprite(mnew Sprite("ui/UI2_handle_side_pressed.png"));

		mTopHandle = mLeftHandle;
		mBottomHandle = mLeftHandle;
		mRightHandle = mLeftHandle;

		mPivotHandle.SetRegularSprite(mnew Sprite("ui/UI2_pivot.png"));
		mPivotHandle.SetHoverSprite(mnew Sprite("ui/UI2_pivot_select.png"));
		mPivotHandle.SetPressedSprite(mnew Sprite("ui/UI2_pivot_pressed.png"));

		mLeftTopHandle.onChangedPos = Func(this, &FrameHandles::OnLeftTopHandle);
		mLeftHandle.onChangedPos = Func(this, &FrameHandles::OnLeftHandle);
		mLeftBottomHandle.onChangedPos = Func(this, &FrameHandles::OnLeftBottomHandle);
		mTopHandle.onChangedPos = Func(this, &FrameHandles::OnTopHandle);
		mBottomHandle.onChangedPos = Func(this, &FrameHandles::OnBottomHandle);
		mRightTopHandle.onChangedPos = Func(this, &FrameHandles::OnRightTopHandle);
		mRightHandle.onChangedPos = Func(this, &FrameHandles::OnRightHandle);
		mRightBottomHandle.onChangedPos = Func(this, &FrameHandles::OnRightBottomHandle);
		mLeftTopRotateHandle.onChangedPos = Func(this, &FrameHandles::OnLeftTopRotateHandle);
		mLeftBottomRotateHandle.onChangedPos = Func(this, &FrameHandles::OnLeftBottomRotateHandle);
		mRightTopRotateHandle.onChangedPos = Func(this, &FrameHandles::OnRightTopRotateHandle);
		mRightBottomRotateHandle.onChangedPos = Func(this, &FrameHandles::OnRightBottomRotateHandle);

		mLeftTopHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mLeftHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mLeftBottomHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mTopHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mBottomHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mRightTopHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mRightHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mRightBottomHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mPivotHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mLeftTopRotateHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mLeftBottomRotateHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mRightTopRotateHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);
		mRightBottomRotateHandle.onPressed = Func(this, &FrameHandles::OnHandlePressed);

		mLeftTopHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mLeftHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mLeftBottomHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mTopHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mBottomHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mRightTopHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mRightHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mRightBottomHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mPivotHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mLeftTopRotateHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mLeftBottomRotateHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mRightTopRotateHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);
		mRightBottomRotateHandle.onReleased = Func(this, &FrameHandles::OnHandleReleased);

		mLeftTopHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mLeftHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mLeftBottomHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mTopHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mBottomHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mRightTopHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mRightHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mRightBottomHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mPivotHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mLeftTopRotateHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mLeftBottomRotateHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mRightTopRotateHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
		mRightBottomRotateHandle.onChangeCompleted = Func(this, &FrameHandles::OnChangeCompleted);
	}

	void FrameHandles::SetBasis(const Basis& basis)
	{
		mFrame = basis;
		mPivotHandle.position = mFrame*Vec2F(0.5f, 0.5f);
		UpdateHandlesTransform();
	}

	Basis FrameHandles::GetCurrentBasis() const
	{
		return mFrame;
	}

	void FrameHandles::SetPivot(const Vec2F& position)
	{
		mPivotHandle.position = position;
	}

	Vec2F FrameHandles::GetPivot() const
	{
		return mPivotHandle.GetPosition();
	}

	void FrameHandles::Draw()
	{
		o2Render.DrawBasis(mFrame, mFrameColor, mFrameColor, mFrameColor);
		CursorAreaEventsListener::OnDrawn();

		if (mIsRotationAvailable)
		{
			mLeftTopRotateHandle.Draw();
			mLeftBottomRotateHandle.Draw();
			mRightTopRotateHandle.Draw();
			mRightBottomRotateHandle.Draw();
		}

		mLeftTopHandle.Draw();
		mLeftHandle.Draw();
		mLeftBottomHandle.Draw();
		mTopHandle.Draw();
		mBottomHandle.Draw();
		mRightTopHandle.Draw();
		mRightHandle.Draw();
		mRightBottomHandle.Draw();

		if (mIsPivotAvailable)
			mPivotHandle.Draw();
	}

	bool FrameHandles::IsUnderPoint(const Vec2F& point)
	{
		return mFrame.IsPointInside(point);
	}

	void FrameHandles::SetPivotEnabled(bool enabled)
	{
		mIsPivotAvailable = enabled;
	}

	bool FrameHandles::IsPivotEnabled() const
	{
		return mIsPivotAvailable;
	}

	void FrameHandles::SetRotationEnabled(bool enabled)
	{
		mIsRotationAvailable = enabled;
	}

	bool FrameHandles::IsRotationEnabled() const
	{
		return mIsRotationAvailable;
	}

	const Color4& FrameHandles::GetFrameColor() const
	{
		return mFrameColor;
	}

	void FrameHandles::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (mFrame.IsPointInside(cursor.position))
		{
			mIsDragging = true;
			mBeginDraggingFrame = mFrame;
			mBeginDraggingOffset = cursor.position - mFrame.offs;

			SetHandlesEnable(false);

			onPressed();
		}
	}

	void FrameHandles::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			mIsDragging = false;
			SetHandlesEnable(true);

			onReleased();

			if (mBeginDraggingFrame != mFrame)
				onChangeCompleted();
		}
	}

	void FrameHandles::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			mIsDragging = false;
			SetHandlesEnable(true);

			onReleased();
		}
	}

	void FrameHandles::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			Basis transformed(cursor.position - mBeginDraggingOffset, mFrame.xv, mFrame.yv);

			if (o2Input.IsKeyDown(VK_SHIFT))
			{
				if (Math::Abs(transformed.offs.x - mBeginDraggingFrame.offs.x) > Math::Abs(transformed.offs.y - mBeginDraggingFrame.offs.y))
					transformed.offs.y = mBeginDraggingFrame.offs.y;
				else
					transformed.offs.x = mBeginDraggingFrame.offs.x;
			}

			OnTransformed(mFrame.Inverted()*transformed);
		}
	}

	void FrameHandles::OnLeftTopHandle(const Vec2F& position)
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

		OnTransformed(mFrame.Inverted()*transformedFrame);
	}

	void FrameHandles::OnLeftHandle(const Vec2F& position)
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

		OnTransformed(mFrame.Inverted()*transformedFrame);
	}

	void FrameHandles::OnLeftBottomHandle(const Vec2F& position)
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

		OnTransformed(mFrame.Inverted()*transformedFrame);
	}

	void FrameHandles::OnTopHandle(const Vec2F& position)
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

		OnTransformed(mFrame.Inverted()*transformedFrame);
	}

	void FrameHandles::OnBottomHandle(const Vec2F& position)
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

		OnTransformed(mFrame.Inverted()*transformedFrame);
	}

	void FrameHandles::OnRightTopHandle(const Vec2F& position)
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

		OnTransformed(mFrame.Inverted()*transformedFrame);
	}

	void FrameHandles::OnRightHandle(const Vec2F& position)
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

		OnTransformed(mFrame.Inverted()*transformedFrame);
	}

	void FrameHandles::OnRightBottomHandle(const Vec2F& position)
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

		OnTransformed(mFrame.Inverted()*transformedFrame);
	}

	void FrameHandles::OnLeftTopRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameHandles::OnLeftBottomRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameHandles::OnRightTopRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameHandles::OnRightBottomRotateHandle(const Vec2F& position)
	{
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mFrame;
		OnRotateHandle(position, lastHandleCoords);
	}

	void FrameHandles::OnRotateHandle(const Vec2F& position, Vec2F lastHandleCoords)
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

		OnTransformed(mFrame.Inverted()*transformed);
	}

	void FrameHandles::OnHandlePressed()
	{
		mBeginDraggingFrame = mFrame;
		onPressed();
	}

	void FrameHandles::OnHandleReleased()
	{
		onReleased();
	}

	void FrameHandles::SetHandlesEnable(bool enable)
	{
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

		mPivotHandle.enabled = enable;
	}

	void FrameHandles::UpdateHandlesTransform()
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
	}

	void FrameHandles::OnTransformed(const Basis& transform)
	{
		mFrame = mFrame*transform;
		mPivotHandle.position = mPivotHandle.position*transform;
		mChangedFromThis = true;
		UpdateHandlesTransform();

		onTransformed(mFrame);
	}

	void FrameHandles::OnChangeCompleted()
	{
		onChangeCompleted();
	}

}