#include "DragHandle.h"

#include "Render/Sprite.h"

namespace o2
{
	DragHandle::DragHandle()
	{
		InitializeProperties();
	}

	DragHandle::DragHandle(Ptr<Sprite> regular, Ptr<Sprite> hover /*= nullptr*/, Ptr<Sprite> pressed /*= nullptr*/):
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
		regularSprite.Release();
		hoverSprite.Release();
		pressedSprite.Release();
	}

	void DragHandle::Draw()
	{
		if (regularSprite)
			regularSprite->Draw();

		if (hoverSprite)
			hoverSprite->Draw();

		if (pressedSprite)
			pressedSprite->Draw();
	}

	DragHandle& DragHandle::operator=(const DragHandle& other)
	{
		regularSprite.Release();
		hoverSprite.Release();
		pressedSprite.Release();

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
			return regularSprite->IsPointInside(point);

		return false;
	}

	float DragHandle::Depth()
	{
		return regularSprite ? regularSprite->GetDrawingDepth():0.0f;
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

	void DragHandle::OnCursorStayDown(const Input::Cursor& cursor)
	{
		if (mIsPressed && cursor.mDelta != Vec2F())
		{
			SetPosition(cursor.mPosition);
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