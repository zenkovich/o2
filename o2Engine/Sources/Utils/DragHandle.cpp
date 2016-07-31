#include "DragHandle.h"

#include "Render/Sprite.h"

namespace o2
{
	DragHandle::DragHandle():
		regularSprite(nullptr), hoverSprite(nullptr), pressedSprite(nullptr)
	{
		InitializeProperties();
	}

	DragHandle::DragHandle(Sprite* regular, Sprite* hover /*= nullptr*/, Sprite* pressed /*= nullptr*/):
		regularSprite(regular), hoverSprite(hover), pressedSprite(pressed)
	{
		InitializeProperties();
	}

	DragHandle::DragHandle(const DragHandle& other)
	{
		if (other.regularSprite)
			regularSprite = other.regularSprite->Clone();

		if (other.hoverSprite)
			hoverSprite = other.hoverSprite->Clone();

		if (other.pressedSprite)
			pressedSprite = other.pressedSprite->Clone();

		onChangedPos = other.onChangedPos;

		SetPosition(other.mPosition);

		InitializeProperties();
	}

	DragHandle::~DragHandle()
	{
		delete regularSprite;
		delete hoverSprite;
		delete pressedSprite;
	}

	void DragHandle::Draw()
	{
		if (regularSprite)
			regularSprite->Draw();

		if (hoverSprite)
			hoverSprite->Draw();

		if (pressedSprite)
			pressedSprite->Draw();

		CursorAreaEventsListener::OnDrawn();
	}

	DragHandle& DragHandle::operator=(const DragHandle& other)
	{
		delete regularSprite;
		delete hoverSprite;
		delete pressedSprite;

		if (other.regularSprite)
			regularSprite = other.regularSprite->Clone();

		if (other.hoverSprite)
			hoverSprite = other.hoverSprite->Clone();

		if (other.pressedSprite)
			pressedSprite = other.pressedSprite->Clone();

		onChangedPos = other.onChangedPos;

		SetPosition(other.mPosition);

		return *this;
	}

	bool DragHandle::IsUnderPoint(const Vec2F& point)
	{
		if (regularSprite)
			return mDrawingScissorRect.IsInside(point) && regularSprite->IsPointInside(point);

		return false;
	}

	void DragHandle::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (pressedSprite)
			pressedSprite->enabled = true;
	}

	void DragHandle::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (pressedSprite)
			pressedSprite->enabled = false;
	}

	void DragHandle::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		if (pressedSprite)
			pressedSprite->enabled = false;
	}

	void DragHandle::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (mIsPressed && cursor.delta != Vec2F())
		{
			SetPosition(cursor.position);
			onChangedPos(mPosition);
		}
	}

	void DragHandle::OnCursorEnter(const Input::Cursor& cursor)
	{
		if (hoverSprite)
			hoverSprite->enabled = true;
	}

	void DragHandle::OnCursorExit(const Input::Cursor& cursor)
	{
		if (hoverSprite)
			hoverSprite->enabled = false;
	}

	void DragHandle::SetPosition(const Vec2F& position)
	{
		mPosition = position;

		if (regularSprite)
			regularSprite->SetPosition(position);

		if (hoverSprite)
			hoverSprite->SetPosition(position);

		if (pressedSprite)
			pressedSprite->SetPosition(position);
	}

	Vec2F DragHandle::GetPosition() const
	{
		return mPosition;
	}

	void DragHandle::InitializeProperties()
	{
		INITIALIZE_PROPERTY(DragHandle, position, SetPosition, GetPosition);
	}
}