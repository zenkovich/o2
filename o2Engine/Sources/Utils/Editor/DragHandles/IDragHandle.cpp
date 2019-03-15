#include "stdafx.h"
#include "IDragHandle.h"

namespace o2
{
	IDragHandle::IDragHandle()
	{
		checkPositionFunc = [](const Vec2F& point) { return point; };
		checkSnappingFunc = [](const Vec2F& point) { return point; };
	}

	IDragHandle::IDragHandle(const IDragHandle& other):
		position(this)
	{
		onChangedPos = other.onChangedPos;
		checkPositionFunc = other.checkPositionFunc;
		checkSnappingFunc = other.checkSnappingFunc;
		cursorType = other.cursorType;
		snappingKey = other.snappingKey;

		SetPosition(other.mPosition);
	}

	IDragHandle::~IDragHandle()
	{}

	IDragHandle& IDragHandle::operator=(const IDragHandle& other)
	{

		onChangedPos = other.onChangedPos;
		checkPositionFunc = other.checkPositionFunc;
		checkSnappingFunc = other.checkSnappingFunc;
		cursorType = other.cursorType;
		snappingKey = other.snappingKey;

		SetPosition(other.mPosition);

		return *this;
	}

	void IDragHandle::SetPosition(const Vec2F& position)
	{
		mPosition = checkPositionFunc(position);

		if (mIsDragging && o2Input.IsKeyDown(snappingKey))
			mPosition = checkSnappingFunc(mPosition);
	}

	void IDragHandle::SetDragPosition(const Vec2F& position)
	{
		mDragPosition = position;
		SetPosition(position);
	}

	Vec2F IDragHandle::GetPosition() const
	{
		return mPosition;
	}

	Vec2F IDragHandle::GetDraggingOffset() const
	{
		return mDragOffset;
	}

	Vec2F IDragHandle::GetDraggingBeginPosition() const
	{
		return mDragBeginPosition;
	}
}

DECLARE_CLASS(o2::DragHandle);

DECLARE_CLASS(o2::SelectableDragHandle);
