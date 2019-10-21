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
						   Sprite* selected /*= nullptr*/, Sprite* selectedHovered /*= nullptr*/, 
						   Sprite* selectedPressed /*= nullptr*/) :
		mRegularSprite(regular), mHoverSprite(hover), mPressedSprite(pressed), mSelectedSprite(selected),
		mSelectedHoverSprite(selectedHovered), mSelectedPressedSprite(selectedPressed)
	{
		screenToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToScreenTransformFunc = [](const Vec2F& point) { return point; };
		checkPositionFunc = [](const Vec2F& point) { return point; };
		checkSnappingFunc = [](const Vec2F& point) { return point; };
	}

	DragHandle::DragHandle(const DragHandle& other) :
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

		if (other.mSelectedHoverSprite)
			mSelectedHoverSprite = other.mSelectedHoverSprite->CloneAs<Sprite>();

		if (other.mSelectedPressedSprite)
			mSelectedPressedSprite = other.mSelectedPressedSprite->CloneAs<Sprite>();

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

		if (mSelectedHoverSprite)
			delete mSelectedHoverSprite;

		if (mSelectedPressedSprite)
			delete mSelectedPressedSprite;

		if (mSelectGroup)
			mSelectGroup->RemoveHandle(this);
	}

	DragHandle& DragHandle::operator=(const DragHandle& other)
	{
		if (mRegularSprite)
			delete mRegularSprite;

		if (mHoverSprite)
			delete mHoverSprite;

		if (mPressedSprite)
			delete mPressedSprite;

		if (mSelectedSprite)
			delete mSelectedSprite;

		if (mSelectedHoverSprite)
			delete mSelectedHoverSprite;

		if (mSelectedPressedSprite)
			delete mSelectedPressedSprite;

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

		if (other.mSelectedHoverSprite)
			mSelectedHoverSprite = other.mSelectedHoverSprite->CloneAs<Sprite>();
		else
			mSelectedHoverSprite = nullptr;

		if (other.mSelectedPressedSprite)
			mSelectedPressedSprite = other.mSelectedPressedSprite->CloneAs<Sprite>();
		else
			mSelectedPressedSprite = nullptr;

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

		if (mSelectGroup)
		{
			if (selected)
				mSelectGroup->SelectHandle(this);
			else
				mSelectGroup->DeselectHandle(this);
		}
		else
		{
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
		UpdateSpritesPositions();

		float alphaChangeCoef = 15.0f;

		if (mRegularSprite)
			mRegularSprite->Draw();
		
		if (mHoverSprite && !(mIsSelected && mSelectedHoverSprite))
		{
			float transparency = Math::Lerp(mHoverSprite->GetTransparency(), mIsHovered ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef);

			if (!Math::Equals(transparency, mHoverSprite->GetTransparency()))
				mHoverSprite->SetTransparency(transparency);

			mHoverSprite->Draw();
		}
		
		if (mPressedSprite && !(mIsSelected && mSelectedPressedSprite))
		{
			float transparency = Math::Lerp(mPressedSprite->GetTransparency(), mIsPressed ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef);

			if (!Math::Equals(transparency, mPressedSprite->GetTransparency()))
				mPressedSprite->SetTransparency(transparency);

			mPressedSprite->Draw();
		}

		if (mSelectedSprite)
		{
			float transparency = Math::Lerp(mSelectedSprite->GetTransparency(), mIsSelected ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef);

			if (!Math::Equals(transparency, mSelectedSprite->GetTransparency()))
				mSelectedSprite->SetTransparency(transparency);

			mSelectedSprite->Draw();
		}
		
		if (mIsSelected && mSelectedHoverSprite)
		{
			float transparency = Math::Lerp(mSelectedHoverSprite->GetTransparency(), mIsHovered ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef);

			if (!Math::Equals(transparency, mSelectedHoverSprite->GetTransparency()))
				mSelectedHoverSprite->SetTransparency(transparency);

			mSelectedHoverSprite->Draw();
		}
		
		if (mIsSelected && mSelectedPressedSprite) 
		{

			float transparency = Math::Lerp(mSelectedPressedSprite->GetTransparency(), mIsPressed ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef);

			if (!Math::Equals(transparency, mSelectedPressedSprite->GetTransparency()))
				mSelectedPressedSprite->SetTransparency(transparency);

			mSelectedPressedSprite->Draw();
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
		onPressed();

		mDragOffset = mPosition - ScreenToLocal(cursor.position);
		mDragPosition = mPosition;
		mPressedCursorId = cursor.id;

		if (mSelectGroup)
			mSelectGroup->OnHandleCursorPressed(this, cursor);

		mPressedCursorPos = cursor.position;
	}

	void DragHandle::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			mIsDragging = false;

			if (mDragBeginPosition != mPosition)
			{
				if (mSelectGroup)
					mSelectGroup->OnHandleCompletedChange(this);
				else
					onChangeCompleted();
			}
		}
		else
		{
			if (mSelectGroup)
				mSelectGroup->OnHandleCursorReleased(this, cursor);
			else
				SetSelected(!IsSelected());
		}

		if (!IsUnderPoint(cursor.position))
			o2Application.SetCursor(CursorType::Arrow);

		onReleased();
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

		if (!mIsDragging)
		{
			float delta = (cursor.position - mPressedCursorPos).Length();
			if (delta > mDragDistanceThreshold)
			{
				onBeganDragging();

				mIsDragging = true;
				mDragBeginPosition = mPosition;

				if (mSelectGroup)
					mSelectGroup->OnHandleBeganDragging(this);
				else
					Select();
			}
		}

		if (mIsDragging)
		{
			mDragPosition = ScreenToLocal(cursor.position) + mDragOffset;

			SetPosition(mDragPosition);
			onChangedPos(mPosition);

			if (mSelectGroup)
				mSelectGroup->OnHandleMoved(this, cursor.position);
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
	}

	void DragHandle::UpdateSpritesPositions()
	{
		if (mLastDrawnScreenPosition == mScreenPosition)
			return;

		mLastDrawnScreenPosition = mScreenPosition;

		if (mRegularSprite)
			mRegularSprite->SetPosition(mScreenPosition);

		if (mHoverSprite)
			mHoverSprite->SetPosition(mScreenPosition);

		if (mPressedSprite)
			mPressedSprite->SetPosition(mScreenPosition);

		if (mSelectedSprite)
			mSelectedSprite->SetPosition(mScreenPosition);

		if (mSelectedHoverSprite)
			mSelectedHoverSprite->SetPosition(mScreenPosition);

		if (mSelectedPressedSprite)
			mSelectedPressedSprite->SetPosition(mScreenPosition);
	}

	void DragHandle::SetDragPosition(const Vec2F& position)
	{
		mDragPosition = position;
		SetPosition(position);
	}

	Vec2F DragHandle::GetDragPosition() const
	{
		return mDragPosition;
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

	void DragHandle::BeginDrag(const Vec2F& cursor)
	{
		mDragOffset = mPosition - ScreenToLocal(cursor);
		mDragPosition = mPosition;
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

		if (mSelectedHoverSprite)
			mSelectedHoverSprite->angle = rad;

		if (mSelectedPressedSprite)
			mSelectedPressedSprite->angle = rad;
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

	void DragHandle::SetSelectedSprite(Sprite* sprite)
	{
		if (mSelectedSprite)
			delete mSelectedSprite;

		mSelectedSprite = sprite;
	}

	Sprite* DragHandle::GetSelectedSprite() const
	{
		return mSelectedSprite;
	}

	void DragHandle::SetSelectedHoveredSprite(Sprite* sprite)
	{
		if (mSelectedHoverSprite)
			delete mSelectedHoverSprite;

		mSelectedHoverSprite = sprite;
	}

	Sprite* DragHandle::GetSelectedHoveredSprite() const
	{
		return mSelectedHoverSprite;
	}

	void DragHandle::SetSelectedPressedSprite(Sprite* sprite)
	{
		if (mSelectedPressedSprite)
			delete mSelectedPressedSprite;

		mSelectedPressedSprite = sprite;
	}

	Sprite* DragHandle::GetSelectedPressedSprite() const
	{
		return mSelectedPressedSprite;
	}

	void DragHandle::SetSpritesSize(const Vec2F& size)
	{
		if (mRegularSprite)
			mRegularSprite->SetSize(size);

		if (mHoverSprite)
			mHoverSprite->SetSize(size);

		if (mPressedSprite)
			mPressedSprite->SetSize(size);

		if (mSelectedSprite)
			mSelectedSprite->SetSize(size);

		if (mSelectedHoverSprite)
			mSelectedHoverSprite->SetSize(size);

		if (mSelectedPressedSprite)
			mSelectedPressedSprite->SetSize(size);
	}

	void DragHandle::SetSpritesColor(const Color4& color)
	{
		if (mRegularSprite)
			mRegularSprite->SetColor(color);

		if (mHoverSprite)
			mHoverSprite->SetColor(color);

		if (mPressedSprite)
			mPressedSprite->SetColor(color);

		if (mSelectedSprite)
			mSelectedSprite->SetColor(color);

		if (mSelectedHoverSprite)
			mSelectedHoverSprite->SetColor(color);

		if (mSelectedPressedSprite)
			mSelectedPressedSprite->SetColor(color);
	}

	void DragHandle::SetSpritesSizePivot(const Vec2F& pivot)
	{
		if (mRegularSprite)
			mRegularSprite->SetSizePivot(pivot);

		if (mHoverSprite)
			mHoverSprite->SetSizePivot(pivot);

		if (mPressedSprite)
			mPressedSprite->SetSizePivot(pivot);

		if (mSelectedSprite)
			mSelectedSprite->SetSizePivot(pivot);

		if (mSelectedHoverSprite)
			mSelectedHoverSprite->SetSizePivot(pivot);

		if (mSelectedPressedSprite)
			mSelectedPressedSprite->SetSizePivot(pivot);
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

	WidgetDragHandle::WidgetDragHandle() :
		DragHandle(), Widget()
	{
		widgetOffsetToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToWidgetOffsetTransformFunc = [](const Vec2F& point) { return point; };
	}

	WidgetDragHandle::WidgetDragHandle(Sprite* regular, Sprite* hover /*= nullptr*/, Sprite* pressed /*= nullptr*/, 
									   Sprite* selected /*= nullptr*/, Sprite* selectedHovered /*= nullptr*/, 
									   Sprite* selectedPressed /*= nullptr*/) :
		DragHandle(regular, hover, pressed, selected, selectedHovered, selectedPressed), Widget()
	{
		widgetOffsetToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToWidgetOffsetTransformFunc = [](const Vec2F& point) { return point; };
	}

	WidgetDragHandle::WidgetDragHandle(const WidgetDragHandle& other) :
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
		if (!mResEnabledInHierarchy)
			return;

		DragHandle::Draw();
	}

	void WidgetDragHandle::SetEnabled(bool enabled)
	{
		DragHandle::SetEnabled(enabled);
	}

	bool WidgetDragHandle::IsEnabled() const
	{
		return DragHandle::IsEnabled();
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
		onLayoutUpdated();
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
		for (auto handle : mHandles)
			handle->mSelectGroup = nullptr;
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

		OnSelectionChanged();
	}

	void SelectableDragHandlesGroup::DeselectHandle(DragHandle* handle)
	{
		if (!mSelectedHandles.Contains(handle))
			return;

		handle->mIsSelected = false;
		mSelectedHandles.Remove(handle);
		handle->OnDeselected();

		OnSelectionChanged();
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

		int idx = mSelectedHandles.Find(handle);
		if (idx >= 0)
		{
			mSelectedHandles.Remove(handle);
			OnSelectionChanged();
		}
	}

	void SelectableDragHandlesGroup::DeselectAll()
	{
		for (auto handle : mSelectedHandles)
		{
			handle->mIsSelected = false;
			handle->OnDeselected();
		}

		mSelectedHandles.Clear();
		OnSelectionChanged();
	}

	void SelectableDragHandlesGroup::SelectAll()
	{
		for (auto handle : mHandles) {
			handle->mIsSelected = true;
			handle->OnSelected();
		}

		mSelectedHandles = mHandles;
		OnSelectionChanged();
	}

	void SelectableDragHandlesGroup::OnSelectionChanged()
	{}

	void SelectableDragHandlesGroup::OnHandleCursorPressed(DragHandle* handle, const Input::Cursor& cursor)
	{
		if (!GetSelectedHandles().Contains(handle))
		{
			if (!o2Input.IsKeyDown(VK_CONTROL))
				DeselectAll();

			SelectHandle(handle);
		}

		for (auto handle : GetSelectedHandles())
			handle->BeginDrag(cursor.position);
	}

	void SelectableDragHandlesGroup::OnHandleCursorReleased(DragHandle* handle, const Input::Cursor& cursor)
	{}

	void SelectableDragHandlesGroup::OnHandleBeganDragging(DragHandle* handle)
	{
		if (handle->IsSelected())
			return;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAll();

		SelectHandle(handle);
	}

	void SelectableDragHandlesGroup::OnHandleMoved(DragHandle* handle, const Vec2F& cursorPos)
	{
		for (auto handle : GetSelectedHandles())
		{
			handle->mDragPosition = handle->ScreenToLocal(cursorPos) + handle->mDragOffset;
			handle->SetPosition(handle->mDragPosition);
			handle->onChangedPos(handle->GetPosition());
		}
	}

}

DECLARE_CLASS(o2::DragHandle);

DECLARE_CLASS(o2::WidgetDragHandle);
