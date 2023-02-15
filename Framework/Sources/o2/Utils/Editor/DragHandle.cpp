#include "o2/stdafx.h"
#include "DragHandle.h"

#include "o2/Application/Application.h"
#include "o2/Events/EventSystem.h"
#include "o2/Render/RectDrawable.h"
#include "o2/Scene/UI/WidgetLayout.h"

namespace o2
{
	DragHandle::DragHandle()
	{
		screenToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToScreenTransformFunc = [](const Vec2F& point) { return point; };
		checkPositionFunc = [](const Vec2F& point) { return point; };
		checkSnappingFunc = [](const Vec2F& point) { return point; };
	}

	DragHandle::DragHandle(IRectDrawable* regular, IRectDrawable* hover /*= nullptr*/, IRectDrawable* pressed /*= nullptr*/,
						   IRectDrawable* selected /*= nullptr*/, IRectDrawable* selectedHovered /*= nullptr*/, 
						   IRectDrawable* selectedPressed /*= nullptr*/) :
		mRegularDrawable(regular), mHoverDrawable(hover), mPressedDrawable(pressed), mSelectedDrawable(selected),
		mSelectedHoverDrawable(selectedHovered), mSelectedPressedDrawable(selectedPressed)
	{
		screenToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToScreenTransformFunc = [](const Vec2F& point) { return point; };
		checkPositionFunc = [](const Vec2F& point) { return point; };
		checkSnappingFunc = [](const Vec2F& point) { return point; };
	}

	DragHandle::DragHandle(const DragHandle& other) :
		angle(this), position(this), enabled(this)
	{
		if (other.mRegularDrawable)
			mRegularDrawable = other.mRegularDrawable->CloneAs<IRectDrawable>();

		if (other.mHoverDrawable)
			mHoverDrawable = other.mHoverDrawable->CloneAs<IRectDrawable>();

		if (other.mPressedDrawable)
			mPressedDrawable = other.mPressedDrawable->CloneAs<IRectDrawable>();

		if (other.mSelectedDrawable)
			mSelectedDrawable = other.mSelectedDrawable->CloneAs<IRectDrawable>();

		if (other.mSelectedHoverDrawable)
			mSelectedHoverDrawable = other.mSelectedHoverDrawable->CloneAs<IRectDrawable>();

		if (other.mSelectedPressedDrawable)
			mSelectedPressedDrawable = other.mSelectedPressedDrawable->CloneAs<IRectDrawable>();

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
		if (mRegularDrawable)
			delete mRegularDrawable;

		if (mHoverDrawable)
			delete mHoverDrawable;

		if (mPressedDrawable)
			delete mPressedDrawable;

		if (mSelectedDrawable)
			delete mSelectedDrawable;

		if (mSelectedHoverDrawable)
			delete mSelectedHoverDrawable;

		if (mSelectedPressedDrawable)
			delete mSelectedPressedDrawable;

		if (mSelectGroup)
			mSelectGroup->RemoveHandle(this);
	}

	DragHandle& DragHandle::operator=(const DragHandle& other)
	{
		if (mRegularDrawable)
			delete mRegularDrawable;

		if (mHoverDrawable)
			delete mHoverDrawable;

		if (mPressedDrawable)
			delete mPressedDrawable;

		if (mSelectedDrawable)
			delete mSelectedDrawable;

		if (mSelectedHoverDrawable)
			delete mSelectedHoverDrawable;

		if (mSelectedPressedDrawable)
			delete mSelectedPressedDrawable;

		if (other.mRegularDrawable)
			mRegularDrawable = other.mRegularDrawable->CloneAs<IRectDrawable>();
		else
			mRegularDrawable = nullptr;

		if (other.mHoverDrawable)
			mHoverDrawable = other.mHoverDrawable->CloneAs<IRectDrawable>();
		else
			mHoverDrawable = nullptr;

		if (other.mPressedDrawable)
			mPressedDrawable = other.mPressedDrawable->CloneAs<IRectDrawable>();
		else
			mPressedDrawable = nullptr;

		if (other.mSelectedDrawable)
			mSelectedDrawable = other.mSelectedDrawable->CloneAs<IRectDrawable>();
		else
			mSelectedDrawable = nullptr;

		if (other.mSelectedHoverDrawable)
			mSelectedHoverDrawable = other.mSelectedHoverDrawable->CloneAs<IRectDrawable>();
		else
			mSelectedHoverDrawable = nullptr;

		if (other.mSelectedPressedDrawable)
			mSelectedPressedDrawable = other.mSelectedPressedDrawable->CloneAs<IRectDrawable>();
		else
			mSelectedPressedDrawable = nullptr;

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

		if (mLastScreenPosUpdateFrame != o2Time.GetCurrentFrame())
		{
			UpdateScreenPosition();
			mLastScreenPosUpdateFrame = o2Time.GetCurrentFrame();
		}

		UpdateDrawablesPositions();
		DrawInternal();
	}

	void DragHandle::Draw(const RectF& screenClipRect)
	{
		if (!mEnabled)
			return;

		if (mLastScreenPosUpdateFrame != o2Time.GetCurrentFrame())
		{
			UpdateScreenPosition();
			mLastScreenPosUpdateFrame = o2Time.GetCurrentFrame();
		}

		if (screenClipRect.IsInside(mScreenPosition))
		{
			UpdateDrawablesPositions();
			DrawInternal();
		}
	}

	bool DragHandle::IsUnderPoint(const Vec2F& point)
	{
		if (!isPointInside.IsEmpty())
			return mDrawingScissorRect.IsInside(point) && isPointInside(point);

		if (mRegularDrawable)
			return mDrawingScissorRect.IsInside(point) && mRegularDrawable->IsPointInside(point);

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

	void DragHandle::UpdateDrawablesPositions()
	{
		if (mLastDrawnScreenPosition == mScreenPosition)
			return;

		mLastDrawnScreenPosition = mScreenPosition;

		if (mRegularDrawable)
			mRegularDrawable->SetPosition(mScreenPosition);

		if (mHoverDrawable)
			mHoverDrawable->SetPosition(mScreenPosition);

		if (mPressedDrawable)
			mPressedDrawable->SetPosition(mScreenPosition);

		if (mSelectedDrawable)
			mSelectedDrawable->SetPosition(mScreenPosition);

		if (mSelectedHoverDrawable)
			mSelectedHoverDrawable->SetPosition(mScreenPosition);

		if (mSelectedPressedDrawable)
			mSelectedPressedDrawable->SetPosition(mScreenPosition);
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

		if (mRegularDrawable)
			mRegularDrawable->angle = rad;

		if (mHoverDrawable)
			mHoverDrawable->angle = rad;

		if (mPressedDrawable)
			mPressedDrawable->angle = rad;

		if (mSelectedDrawable)
			mSelectedDrawable->angle = rad;

		if (mSelectedHoverDrawable)
			mSelectedHoverDrawable->angle = rad;

		if (mSelectedPressedDrawable)
			mSelectedPressedDrawable->angle = rad;
	}

	float DragHandle::GetAngle() const
	{
		return mAngle;
	}

	const Vec2F& DragHandle::GetPressedCursorPos() const
	{
		return mPressedCursorPos;
	}

	void DragHandle::SetRegularDrawable(IRectDrawable* IRectDrawable)
	{
		if (mRegularDrawable)
			delete mRegularDrawable;

		mRegularDrawable = IRectDrawable;
	}

	IRectDrawable* DragHandle::GetRegularDrawable() const
	{
		return mRegularDrawable;
	}

	void DragHandle::SetHoverDrawable(IRectDrawable* IRectDrawable)
	{
		if (mHoverDrawable)
			delete mHoverDrawable;

		mHoverDrawable = IRectDrawable;
	}

	IRectDrawable* DragHandle::GetHoverDrawable() const
	{
		return mHoverDrawable;
	}

	void DragHandle::SetPressedDrawable(IRectDrawable* IRectDrawable)
	{
		if (mPressedDrawable)
			delete mPressedDrawable;

		mPressedDrawable = IRectDrawable;
	}

	IRectDrawable* DragHandle::GetPressedDrawable() const
	{
		return mPressedDrawable;
	}

	void DragHandle::SetSelectedDrawable(IRectDrawable* IRectDrawable)
	{
		if (mSelectedDrawable)
			delete mSelectedDrawable;

		mSelectedDrawable = IRectDrawable;
	}

	IRectDrawable* DragHandle::GetSelectedDrawable() const
	{
		return mSelectedDrawable;
	}

	void DragHandle::SetSelectedHoveredDrawable(IRectDrawable* IRectDrawable)
	{
		if (mSelectedHoverDrawable)
			delete mSelectedHoverDrawable;

		mSelectedHoverDrawable = IRectDrawable;
	}

	IRectDrawable* DragHandle::GetSelectedHoveredDrawable() const
	{
		return mSelectedHoverDrawable;
	}

	void DragHandle::SetSelectedPressedDrawable(IRectDrawable* IRectDrawable)
	{
		if (mSelectedPressedDrawable)
			delete mSelectedPressedDrawable;

		mSelectedPressedDrawable = IRectDrawable;
	}

	IRectDrawable* DragHandle::GetSelectedPressedDrawable() const
	{
		return mSelectedPressedDrawable;
	}

	void DragHandle::SetDrawablesSize(const Vec2F& size)
	{
		if (mRegularDrawable)
			mRegularDrawable->SetSize(size);

		if (mHoverDrawable)
			mHoverDrawable->SetSize(size);

		if (mPressedDrawable)
			mPressedDrawable->SetSize(size);

		if (mSelectedDrawable)
			mSelectedDrawable->SetSize(size);

		if (mSelectedHoverDrawable)
			mSelectedHoverDrawable->SetSize(size);

		if (mSelectedPressedDrawable)
			mSelectedPressedDrawable->SetSize(size);
	}

	void DragHandle::SetDrawablesColor(const Color4& color)
	{
		if (mRegularDrawable)
			mRegularDrawable->SetColor(color);

		if (mHoverDrawable)
			mHoverDrawable->SetColor(color);

		if (mPressedDrawable)
			mPressedDrawable->SetColor(color);
	}

	void DragHandle::SetDrawablesSizePivot(const Vec2F& pivot)
	{
		if (mRegularDrawable)
			mRegularDrawable->SetSizePivot(pivot);

		if (mHoverDrawable)
			mHoverDrawable->SetSizePivot(pivot);

		if (mPressedDrawable)
			mPressedDrawable->SetSizePivot(pivot);

		if (mSelectedDrawable)
			mSelectedDrawable->SetSizePivot(pivot);

		if (mSelectedHoverDrawable)
			mSelectedHoverDrawable->SetSizePivot(pivot);

		if (mSelectedPressedDrawable)
			mSelectedPressedDrawable->SetSizePivot(pivot);
	}

	bool DragHandle::IsScrollable() const
	{
		return true;
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

	void DragHandle::DrawInternal()
	{
		float alphaChangeCoef = 15.0f;

		if (mRegularDrawable)
			mRegularDrawable->Draw();

		if (mHoverDrawable && !(mIsSelected && mSelectedHoverDrawable))
		{
			float transparency = Math::Lerp(mHoverDrawable->GetTransparency(), mIsHovered ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef);

			if (!Math::Equals(transparency, mHoverDrawable->GetTransparency()))
				mHoverDrawable->SetTransparency(transparency);

			mHoverDrawable->Draw();
		}

		if (mPressedDrawable && !(mIsSelected && mSelectedPressedDrawable))
		{
			float transparency = Math::Lerp(mPressedDrawable->GetTransparency(), mIsPressed ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef);

			if (!Math::Equals(transparency, mPressedDrawable->GetTransparency()))
				mPressedDrawable->SetTransparency(transparency);

			mPressedDrawable->Draw();
		}

		if (mSelectedDrawable)
		{
			float transparency = Math::Lerp(mSelectedDrawable->GetTransparency(), mIsSelected ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef);

			if (!Math::Equals(transparency, mSelectedDrawable->GetTransparency()))
				mSelectedDrawable->SetTransparency(transparency);

			mSelectedDrawable->Draw();
		}

		if (mIsSelected && mSelectedHoverDrawable)
		{
			float transparency = Math::Lerp(mSelectedHoverDrawable->GetTransparency(), mIsHovered ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef);

			if (!Math::Equals(transparency, mSelectedHoverDrawable->GetTransparency()))
				mSelectedHoverDrawable->SetTransparency(transparency);

			mSelectedHoverDrawable->Draw();
		}

		if (mIsSelected && mSelectedPressedDrawable)
		{

			float transparency = Math::Lerp(mSelectedPressedDrawable->GetTransparency(), mIsPressed ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef);

			if (!Math::Equals(transparency, mSelectedPressedDrawable->GetTransparency()))
				mSelectedPressedDrawable->SetTransparency(transparency);

			mSelectedPressedDrawable->Draw();
		}

		CursorAreaEventsListener::OnDrawn();
		IDrawable::OnDrawn();
	}

	WidgetDragHandle::WidgetDragHandle() :
		DragHandle(), Widget()
	{
		widgetOffsetToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToWidgetOffsetTransformFunc = [](const Vec2F& point) { return point; };
	}

	WidgetDragHandle::WidgetDragHandle(IRectDrawable* regular, IRectDrawable* hover /*= nullptr*/, IRectDrawable* pressed /*= nullptr*/, 
									   IRectDrawable* selected /*= nullptr*/, IRectDrawable* selectedHovered /*= nullptr*/, 
									   IRectDrawable* selectedPressed /*= nullptr*/) :
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

	String WidgetDragHandle::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void WidgetDragHandle::UpdateLayersLayouts()
	{
		onLayoutUpdated();
		UpdateScreenPosition();
	}

	void WidgetDragHandle::OnSerialize(DataValue& node) const
	{
		Widget::OnSerialize(node);
	}

	void WidgetDragHandle::OnDeserialized(const DataValue& node)
	{
		Widget::OnDeserialized(node);
	}

	void WidgetDragHandle::OnDeserializedDelta(const DataValue& node, const IObject& origin)
	{
		Widget::OnDeserializedDelta(node, origin);
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

	const Vector<DragHandle*>& SelectableDragHandlesGroup::GetSelectedHandles() const
	{
		return mSelectedHandles;
	}

	Vector<DragHandle*> SelectableDragHandlesGroup::GetAllHandles() const
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

		int idx = mSelectedHandles.IndexOf(handle);
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
