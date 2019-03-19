#include "stdafx.h"
#include "DragHandle.h"

#include "Application/Application.h"
#include "Events/EventSystem.h"
#include "Render/Sprite.h"
#include "Scene/UI/WidgetLayout.h"

namespace o2
{
	DragHandle::DragHandle()
	{
		screenToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToScreenTransformFunc = [](const Vec2F& point) { return point; };
		checkPositionFunc = [](const Vec2F& point) { return point; };
		checkSnappingFunc = [](const Vec2F& point) { return point; };
	}

	DragHandle::DragHandle(Sprite* regular, Sprite* hover /*= nullptr*/, Sprite* pressed /*= nullptr*/, 
						   Sprite* selected /*= nullptr*/):
		mRegularSprite(regular), mHoverSprite(hover), mPressedSprite(pressed), mSelectedSprite(selected)
	{
		screenToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToScreenTransformFunc = [](const Vec2F& point) { return point; };
		checkPositionFunc = [](const Vec2F& point) { return point; };
		checkSnappingFunc = [](const Vec2F& point) { return point; };
	}

	DragHandle::DragHandle(const DragHandle& other):
		angle(this), position(this), enabled(this)
	{
		if (other.mRegularSprite)
			mRegularSprite = other.mRegularSprite->CloneAs<Sprite>();

		if (other.mHoverSprite)
			mHoverSprite = other.mHoverSprite->CloneAs<Sprite>();

		if (other.mPressedSprite)
			mPressedSprite = other.mPressedSprite->CloneAs<Sprite>();

		if (other.mSelectedSprite)
			mSelectedSprite = other.mSelectedSprite->CloneAs<Sprite>();

		onChangedPos = other.onChangedPos;
		screenToLocalTransformFunc = other.screenToLocalTransformFunc;
		localToScreenTransformFunc = other.localToScreenTransformFunc;
		checkPositionFunc = other.checkPositionFunc;
		checkSnappingFunc = other.checkSnappingFunc;
		cursorType = other.cursorType;
		snappingKey = other.snappingKey;
		pixelPerfect = other.pixelPerfect;

		SetPosition(other.mPosition);
	}

	DragHandle::~DragHandle()
	{
		if (mRegularSprite)
			delete mRegularSprite;

		if (mHoverSprite)
			delete mHoverSprite;

		if (mPressedSprite)
			delete mPressedSprite;

		if (mSelectedSprite)
			delete mSelectedSprite;

		if (mSelectGroup)
			mSelectGroup->RemoveHandle(this);
	}

	DragHandle& DragHandle::operator=(const DragHandle& other)
	{
		delete mRegularSprite;
		delete mHoverSprite;
		delete mPressedSprite;
		delete mSelectedSprite;

		if (other.mRegularSprite)
			mRegularSprite = other.mRegularSprite->CloneAs<Sprite>();
		else
			mRegularSprite = nullptr;

		if (other.mHoverSprite)
			mHoverSprite = other.mHoverSprite->CloneAs<Sprite>();
		else
			mHoverSprite = nullptr;

		if (other.mPressedSprite)
			mPressedSprite = other.mPressedSprite->CloneAs<Sprite>();
		else
			mPressedSprite = nullptr;

		if (other.mSelectedSprite)
			mSelectedSprite = other.mSelectedSprite->CloneAs<Sprite>();
		else
			mSelectedSprite = nullptr;

		onChangedPos = other.onChangedPos;
		screenToLocalTransformFunc = other.screenToLocalTransformFunc;
		localToScreenTransformFunc = other.localToScreenTransformFunc;
		checkPositionFunc = other.checkPositionFunc;
		checkSnappingFunc = other.checkSnappingFunc;
		cursorType = other.cursorType;
		snappingKey = other.snappingKey;
		pixelPerfect = other.pixelPerfect;

		SetPosition(other.mPosition);

		return *this;
	}

	bool DragHandle::IsSelected() const
	{
		return mIsSelected;
	}

	void DragHandle::SetSelected(bool selected)
	{
		if (mIsSelected == selected)
			return;

		if (mSelectGroup) {
			if (selected)
				mSelectGroup->SelectHandle(this);
			else
				mSelectGroup->DeselectHandle(this);
		}
		else {
			mIsSelected = selected;

			if (mIsSelected)
				OnSelected();
			else
				OnDeselected();
		}
	}

	void DragHandle::Select()
	{
		SetSelected(true);
	}

	void DragHandle::Deselect()
	{
		SetSelected(false);
	}

	void DragHandle::Draw()
	{
		if (!mEnabled)
			return;

		UpdateScreenPosition();

		float alphaChangeCoef = 15.0f;

		if (mRegularSprite)
			mRegularSprite->Draw();

		if (mHoverSprite)
		{
			mHoverSprite->SetTransparency(Math::Lerp(mHoverSprite->GetTransparency(), mIsHovered ? 1.0f : 0.0f,
										  o2Time.GetDeltaTime()*alphaChangeCoef));

			mHoverSprite->Draw();
		}

		if (mPressedSprite)
		{
			mPressedSprite->SetTransparency(Math::Lerp(mPressedSprite->GetTransparency(), mIsPressed ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef));

			mPressedSprite->Draw();
		}

		if (mSelectedSprite) {
			mSelectedSprite->SetTransparency(Math::Lerp(mSelectedSprite->GetTransparency(), mIsSelected ? 1.0f : 0.0f,
														o2Time.GetDeltaTime()*alphaChangeCoef));

			mSelectedSprite->Draw();
		}

		CursorAreaEventsListener::OnDrawn();
		IDrawable::OnDrawn();
	}

	bool DragHandle::IsUnderPoint(const Vec2F& point)
	{
		if (!isPointInside.IsEmpty())
			return mDrawingScissorRect.IsInside(point) && isPointInside(point);

		if (mRegularSprite)
			return mDrawingScissorRect.IsInside(point) && mRegularSprite->IsPointInside(point);

		return false;
	}

	Vec2F DragHandle::ScreenToLocal(const Vec2F& point)
	{
		return screenToLocalTransformFunc(point);
	}

	Vec2F DragHandle::LocalToScreen(const Vec2F& point)
	{
		return localToScreenTransformFunc(point);
	}

	void DragHandle::OnCursorPressed(const Input::Cursor& cursor)
	{
		mDragOffset = mPosition - ScreenToLocal(cursor.position);
		mDragPosition = mPosition;
		mPressedCursorId = cursor.id;

		if (mSelectGroup)
			mSelectGroup->OnHandleCursorPressed(this, cursor);

		mPressedCursorPos = cursor.position;

		onPressed();
	}

	void DragHandle::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mIsDragging) {
			mIsDragging = false;

			if (mDragBeginPosition != mPosition) {
				if (mSelectGroup)
					mSelectGroup->OnHandleCompletedChange(this);
				else
					onChangeCompleted();
			}
		}
		else {
			if (mSelectGroup)
				mSelectGroup->OnHandleCursorReleased(this, cursor);
			else
				SetSelected(!IsSelected());
		}

		if (!IsUnderPoint(cursor.position))
			o2Application.SetCursor(CursorType::Arrow);
	}

	void DragHandle::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		mIsDragging = false;

		onReleased();

		if (mDragBeginPosition != mPosition)
			onChangeCompleted();
	}

	void DragHandle::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (!mIsPressed)
			return;

		if (cursor.id != mPressedCursorId)
			return;

		if (cursor.delta.Length() < 0.5f)
			return;

		if (!mIsDragging) {
			float delta = (cursor.position - mPressedCursorPos).Length();
			if (delta > mDragDistanceThreshold) {
				mIsDragging = true;
				mDragBeginPosition = mPosition;

				if (mSelectGroup)
					mSelectGroup->OnHandleBeganDragging(this);
				else
					Select();
			}
		}

		if (mIsDragging) {
			mDragPosition = ScreenToLocal(cursor.position) + mDragOffset;

			SetPosition(mDragPosition);
			onChangedPos(mPosition);

			if (mSelectGroup)
				mSelectGroup->OnHandleMoved(this, cursor);
		}
	}

	void DragHandle::OnCursorEnter(const Input::Cursor& cursor)
	{
		mIsHovered = true;
		if (!cursor.isPressed)
			o2Application.SetCursor(cursorType);
	}

	void DragHandle::OnCursorExit(const Input::Cursor& cursor)
	{
		mIsHovered = false;
		if (!IsPressed() && !cursor.isPressed)
			o2Application.SetCursor(CursorType::Arrow);
	}

	void DragHandle::OnCursorRightMousePressed(const Input::Cursor& cursor)
	{
		onRightButtonPressed(cursor);
	}

	void DragHandle::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		onRightButtonReleased(cursor);
	}

	void DragHandle::SetPosition(const Vec2F& position)
	{
		mPosition = checkPositionFunc(position);

		if (mIsDragging && o2Input.IsKeyDown(snappingKey))
			mPosition = checkSnappingFunc(mPosition);

		UpdateScreenPosition();
	}

	const Vec2F& DragHandle::GetScreenPosition() const
	{
		return mScreenPosition;
	}

	void DragHandle::UpdateScreenPosition()
	{
		mScreenPosition = LocalToScreen(mPosition);

		if (pixelPerfect)
		{
			mScreenPosition.x = Math::Round(mScreenPosition.x);
			mScreenPosition.y = Math::Round(mScreenPosition.y);
		}

		if (mRegularSprite)
			mRegularSprite->SetPosition(mScreenPosition);

		if (mHoverSprite)
			mHoverSprite->SetPosition(mScreenPosition);

		if (mPressedSprite)
			mPressedSprite->SetPosition(mScreenPosition);

		if (mSelectedSprite)
			mSelectedSprite->SetPosition(mScreenPosition);
	}

	void DragHandle::SetDragPosition(const Vec2F& position)
	{
		mDragPosition = position;
		SetPosition(position);
	}

	Vec2F DragHandle::GetPosition() const
	{
		return mPosition;
	}

	Vec2F DragHandle::GetDraggingOffset() const
	{
		return mDragOffset;
	}

	Vec2F DragHandle::GetDraggingBeginPosition() const
	{
		return mDragBeginPosition;
	}

	void DragHandle::SetEnabled(bool enabled)
	{
		mEnabled = enabled;
	}

	bool DragHandle::IsEnabled() const
	{
		return mEnabled;
	}

	void DragHandle::SetAngle(float rad)
	{
		mAngle = rad;

		if (mRegularSprite)
			mRegularSprite->angle = rad;

		if (mHoverSprite)
			mHoverSprite->angle = rad;

		if (mPressedSprite)
			mPressedSprite->angle = rad;

		if (mSelectedSprite)
			mSelectedSprite->angle = rad;
	}

	float DragHandle::GetAngle() const
	{
		return mAngle;
	}

	void DragHandle::SetRegularSprite(Sprite* sprite)
	{
		if (mRegularSprite)
			delete mRegularSprite;

		mRegularSprite = sprite;
	}

	Sprite* DragHandle::GetRegularSprite() const
	{
		return mRegularSprite;
	}

	void DragHandle::SetHoverSprite(Sprite* sprite)
	{
		if (mHoverSprite)
			delete mHoverSprite;

		mHoverSprite = sprite;
	}

	Sprite* DragHandle::GetHoverSprite() const
	{
		return mHoverSprite;
	}

	void DragHandle::SetPressedSprite(Sprite* sprite)
	{
		if (mPressedSprite)
			delete mPressedSprite;

		mPressedSprite = sprite;
	}

	Sprite* DragHandle::GetPressedSprite() const
	{
		return mPressedSprite;
	}

	void DragHandle::SetSelectionGroup(ISelectableDragHandlesGroup* group)
	{
		if (mSelectGroup)
			mSelectGroup->RemoveHandle(this);

		mSelectGroup = group;

		if (mSelectGroup)
			mSelectGroup->AddHandle(this);
	}

	ISelectableDragHandlesGroup* DragHandle::GetSelectionGroup() const
	{
		return mSelectGroup;
	}
	
	void DragHandle::OnCursorReleasedOutside(const Input::Cursor& cursor)
	{
		if (!mSelectGroup)
			Deselect();
	}

	void DragHandle::OnSelected()
	{}

	void DragHandle::OnDeselected()
	{}

	WidgetDragHandle::WidgetDragHandle():
		DragHandle(), Widget()
	{
		widgetOffsetToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToWidgetOffsetTransformFunc = [](const Vec2F& point) { return point; };
	}

	WidgetDragHandle::WidgetDragHandle(Sprite* regular, Sprite* hover /*= nullptr*/, Sprite* pressed /*= nullptr*/, Sprite* selected /*= nullptr*/):
		DragHandle(regular, hover, pressed, selected), Widget()
	{
		widgetOffsetToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToWidgetOffsetTransformFunc = [](const Vec2F& point) { return point; };
	}

	WidgetDragHandle::WidgetDragHandle(const WidgetDragHandle& other):
		DragHandle(other), Widget(other)
	{
		widgetOffsetToLocalTransformFunc = other.widgetOffsetToLocalTransformFunc;
		localToWidgetOffsetTransformFunc = other.localToWidgetOffsetTransformFunc;

		SetPosition(mPosition);
	}

	WidgetDragHandle::~WidgetDragHandle()
	{}

	WidgetDragHandle& WidgetDragHandle::operator=(const WidgetDragHandle& other)
	{
		Widget::operator=(other);
		DragHandle::operator=(other);

		widgetOffsetToLocalTransformFunc = other.widgetOffsetToLocalTransformFunc;
		localToWidgetOffsetTransformFunc = other.localToWidgetOffsetTransformFunc;

		SetPosition(mPosition);

		return *this;
	}

	void WidgetDragHandle::Draw()
	{
		Widget::Draw();
		DragHandle::Draw();
	}

	Vec2F WidgetDragHandle::ScreenToLocal(const Vec2F& point)
	{
		if (mParentWidget)
			return widgetOffsetToLocalTransformFunc(point - mParentWidget->layout->GetWorldLeftBottom());

		return DragHandle::ScreenToLocal(point);
	}

	Vec2F WidgetDragHandle::LocalToScreen(const Vec2F& point)
	{
		if (mParentWidget)
			return localToWidgetOffsetTransformFunc(point) + mParentWidget->layout->GetWorldLeftBottom();

		return DragHandle::ScreenToLocal(point);
	}

	void WidgetDragHandle::UpdateLayersLayouts()
	{
		UpdateScreenPosition();
	}

	void WidgetDragHandle::OnSerialize(DataNode& node) const
	{
		Widget::OnSerialize(node);
	}

	void WidgetDragHandle::OnDeserialized(const DataNode& node)
	{
		Widget::OnDeserialized(node);
	}

	void ISelectableDragHandlesGroup::DeselectAll()
	{
		auto handles = GetAllHandles();
		for (auto handle : handles)
			DeselectHandle(handle);
	}

	void ISelectableDragHandlesGroup::SelectAll()
	{
		auto handles = GetAllHandles();
		for (auto handle : handles)
			SelectHandle(handle);
	}

	void ISelectableDragHandlesGroup::SetHandleSelectedState(DragHandle* handle, bool selected)
	{
		handle->mIsSelected = selected;
	}

	SelectableDragHandlesGroup::~SelectableDragHandlesGroup()
	{
// 		for (auto handle : mHandles)
// 		{
// 			handle->mSelectGroup = nullptr;
// 			delete handle;
// 		}
	}

	SelectableDragHandlesGroup::SelectableDragHandlesVec SelectableDragHandlesGroup::GetSelectedHandles() const
	{
		return mSelectedHandles;
	}

	SelectableDragHandlesGroup::SelectableDragHandlesVec SelectableDragHandlesGroup::GetAllHandles() const
	{
		return mHandles;
	}

	void SelectableDragHandlesGroup::SelectHandle(DragHandle* handle)
	{
		if (mSelectedHandles.Contains(handle))
			return;

		handle->mIsSelected = true;
		mSelectedHandles.Add(handle);
		handle->OnSelected();
	}

	void SelectableDragHandlesGroup::DeselectHandle(DragHandle* handle)
	{
		if (!mSelectedHandles.Contains(handle))
			return;

		handle->mIsSelected = false;
		mSelectedHandles.Remove(handle);
		handle->OnDeselected();
	}

	void SelectableDragHandlesGroup::AddHandle(DragHandle* handle)
	{
		if (mHandles.Contains(handle))
			return;

		mHandles.Add(handle);
	}

	void SelectableDragHandlesGroup::RemoveHandle(DragHandle* handle)
	{
		mHandles.Remove(handle);
		mSelectedHandles.Remove(handle);
	}

	void SelectableDragHandlesGroup::OnHandleCursorPressed(DragHandle* handle, const Input::Cursor& cursor)
	{
		for (auto handle : GetSelectedHandles())
		{
			handle->mDragOffset = handle->mPosition - handle->ScreenToLocal(cursor.position);
			handle->mDragPosition = handle->mPosition;
		}
	}

	void SelectableDragHandlesGroup::OnHandleCursorReleased(DragHandle* handle, const Input::Cursor& cursor)
	{
		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAll();

		SelectHandle(handle);
	}

	void SelectableDragHandlesGroup::OnHandleBeganDragging(DragHandle* handle)
	{
		if (handle->IsSelected())
			return;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAll();

		SelectHandle(handle);
	}

	void SelectableDragHandlesGroup::OnHandleMoved(DragHandle* handle, const Input::Cursor& cursor)
	{
		for (auto handle : GetSelectedHandles())
		{
			handle->mDragPosition = handle->ScreenToLocal(cursor.position) + handle->mDragOffset;
			handle->SetPosition(handle->mDragPosition);
			handle->onChangedPos(handle->GetPosition());
		}
	}

}

DECLARE_CLASS(o2::DragHandle);

DECLARE_CLASS(o2::WidgetDragHandle);
