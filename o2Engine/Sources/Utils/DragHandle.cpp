#include "DragHandle.h"

#include "Events/EventSystem.h"
#include "Render/Sprite.h"
#include "Application/Application.h"

namespace o2
{
	DragHandle::DragHandle()
	{
		cursorType = CursorType::Arrow;

		screenToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToScreenTransformFunc = [](const Vec2F& point) { return point; };
		checkPositionFunc = [](const Vec2F& point) { return point; };

		InitializeProperties();
	}

	DragHandle::DragHandle(Sprite* regular, Sprite* hover /*= nullptr*/, Sprite* pressed /*= nullptr*/):
		mRegularSprite(regular), mHoverSprite(hover), mPressedSprite(pressed)
	{
		cursorType = CursorType::Arrow;

		screenToLocalTransformFunc = [](const Vec2F& point) { return point; };
		localToScreenTransformFunc = [](const Vec2F& point) { return point; };
		checkPositionFunc = [](const Vec2F& point) { return point; };

		InitializeProperties();
	}

	DragHandle::DragHandle(const DragHandle& other)
	{
		if (other.mRegularSprite)
			mRegularSprite = other.mRegularSprite->Clone();

		if (other.mHoverSprite)
			mHoverSprite = other.mHoverSprite->Clone();

		if (other.mPressedSprite)
			mPressedSprite = other.mPressedSprite->Clone();

		onChangedPos = other.onChangedPos;
		screenToLocalTransformFunc = other.screenToLocalTransformFunc;
		localToScreenTransformFunc = other.localToScreenTransformFunc;
		checkPositionFunc = other.checkPositionFunc;
		cursorType = other.cursorType;

		SetPosition(other.mPosition);
		InitializeProperties();
	}

	DragHandle::~DragHandle()
	{
		delete mRegularSprite;
		delete mHoverSprite;
		delete mPressedSprite;
	}

	DragHandle& DragHandle::operator=(const DragHandle& other)
	{
		delete mRegularSprite;
		delete mHoverSprite;
		delete mPressedSprite;

		if (other.mRegularSprite)
			mRegularSprite = other.mRegularSprite->Clone();

		if (other.mHoverSprite)
			mHoverSprite = other.mHoverSprite->Clone();

		if (other.mPressedSprite)
			mPressedSprite = other.mPressedSprite->Clone();

		onChangedPos = other.onChangedPos;
		screenToLocalTransformFunc = other.screenToLocalTransformFunc;
		localToScreenTransformFunc = other.localToScreenTransformFunc;
		checkPositionFunc = other.checkPositionFunc;
		cursorType = other.cursorType;

		SetPosition(other.mPosition);

		return *this;
	}

	void DragHandle::Draw()
	{
		if (!mEnabled)
			return;

		Vec2F screenPosition = localToScreenTransformFunc(mPosition);
		float alphaChangeCoef = 15.0f;

		if (mRegularSprite)
		{
			mRegularSprite->SetPosition(screenPosition);
			mRegularSprite->Draw();
		}

		if (mHoverSprite)
		{
			mHoverSprite->SetTransparency(Math::Lerp(mHoverSprite->GetTransparency(), mIsHovered ? 1.0f : 0.0f,
										  o2Time.GetDeltaTime()*alphaChangeCoef));

			mHoverSprite->SetPosition(screenPosition);
			mHoverSprite->Draw();
		}

		if (mPressedSprite)
		{
			mPressedSprite->SetTransparency(Math::Lerp(mPressedSprite->GetTransparency(), mIsPressed ? 1.0f : 0.0f,
											o2Time.GetDeltaTime()*alphaChangeCoef));

			mPressedSprite->SetPosition(screenPosition);
			mPressedSprite->Draw();
		}

		CursorAreaEventsListener::OnDrawn();
		IDrawable::OnDrawn();
	}

	bool DragHandle::IsUnderPoint(const Vec2F& point)
	{
		if (mRegularSprite)
			return mDrawingScissorRect.IsInside(point) && mRegularSprite->IsPointInside(point);

		return false;
	}

	void DragHandle::OnCursorPressed(const Input::Cursor& cursor)
	{
		mIsPressed = true;
		mDragOffset = mPosition - screenToLocalTransformFunc(cursor.position);
		mDragPosition = mPosition;
		mPressedCursorId = cursor.id;
	}

	void DragHandle::OnCursorReleased(const Input::Cursor& cursor)
	{
		mIsPressed = false;
		mIsDragging = false;

		if (!IsUnderPoint(cursor.position))
			o2Application.SetCursor(CursorType::Arrow);
	}

	void DragHandle::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		mIsPressed = false;
		mIsDragging = false;
	}

	void DragHandle::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (mIsPressed && cursor.delta != Vec2F())
		{
			mIsDragging = true;
			mDragPosition = screenToLocalTransformFunc(cursor.position) + mDragOffset;
			mDragBeginPosition = mPosition;

			SetPosition(mDragPosition);
			onChangedPos(mPosition);
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

	void DragHandle::InitializeProperties()
	{
		INITIALIZE_PROPERTY(DragHandle, position, SetPosition, GetPosition);
		INITIALIZE_PROPERTY(DragHandle, enabled, SetEnabled, IsEnabled);
		INITIALIZE_PROPERTY(DragHandle, angle, SetAngle, GetAngle);
	}

	SelectableDragHandle::SelectableDragHandle():
		DragHandle()
	{
		mSelectedSprite = mnew Sprite();
	}

	SelectableDragHandle::SelectableDragHandle(Sprite* regular, Sprite* hover /*= nullptr*/,
											   Sprite* pressed /*= nullptr*/, Sprite* selected /*= nullptr*/):
		DragHandle(regular, hover, pressed), mSelectedSprite(selected)
	{}

	SelectableDragHandle::SelectableDragHandle(const SelectableDragHandle& other) :
		DragHandle(other)
	{
		if (other.mSelectedSprite)
			mSelectedSprite = other.mSelectedSprite->Clone();
	}

	SelectableDragHandle::~SelectableDragHandle()
	{
		if (mSelectedSprite)
			delete mSelectedSprite;

		if (mSelectGroup)
			mSelectGroup->RemoveHandle(this);
	}

	SelectableDragHandle& SelectableDragHandle::operator=(const SelectableDragHandle& other)
	{
		DragHandle::operator=(other);

		if (mSelectedSprite)
		{
			delete mSelectedSprite;
			mSelectedSprite = nullptr;
		}

		if (other.mSelectedSprite)
			mSelectedSprite = other.mSelectedSprite->Clone();

		return *this;
	}

	bool SelectableDragHandle::IsSelected() const
	{
		return mIsSelected;
	}

	void SelectableDragHandle::SetSelected(bool selected)
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

	void SelectableDragHandle::Draw()
	{
		DragHandle::Draw();

		Vec2F screenPosition = localToScreenTransformFunc(mPosition);
		float alphaChangeCoef = 15.0f;

		if (mSelectedSprite)
		{
			mSelectedSprite->SetTransparency(Math::Lerp(mSelectedSprite->GetTransparency(), mIsSelected ? 1.0f : 0.0f,
											 o2Time.GetDeltaTime()*alphaChangeCoef));

			mSelectedSprite->SetPosition(screenPosition);
			mSelectedSprite->Draw();
		}
	}

	void SelectableDragHandle::Select()
	{
		SetSelected(true);
	}

	void SelectableDragHandle::Deselect()
	{
		SetSelected(false);
	}

	void SelectableDragHandle::SetSelectionGroup(ISelectableDragHandlesGroup* group)
	{
		if (mSelectGroup)
			mSelectGroup->RemoveHandle(this);

		mSelectGroup = group;

		if (mSelectGroup)
			mSelectGroup->AddHandle(this);
	}

	ISelectableDragHandlesGroup* SelectableDragHandle::GetSelectionGroup() const
	{
		return mSelectGroup;
	}

	void SelectableDragHandle::SetAngle(float rad)
	{
		DragHandle::SetAngle(rad);

		if (mSelectedSprite)
			mSelectedSprite->angle = rad;
	}

	void SelectableDragHandle::OnCursorPressed(const Input::Cursor& cursor)
	{
		DragHandle::OnCursorPressed(cursor);

		if (mSelectGroup)
			mSelectGroup->OnHandleCursorPressed(this, cursor);

		mPressedCursorPos = cursor.position;
	}

	void SelectableDragHandle::OnCursorStillDown(const Input::Cursor& cursor)
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
			mDragPosition = screenToLocalTransformFunc(cursor.position) + mDragOffset;

			SetPosition(mDragPosition);
			onChangedPos(mPosition);

			if (mSelectGroup)
				mSelectGroup->OnHandleMoved(this, cursor);
		}
	}

	void SelectableDragHandle::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			mIsDragging = false;
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

		mIsPressed = false;
	}

	void SelectableDragHandle::OnCursorReleasedOutside(const Input::Cursor& cursor)
	{
		if (!mSelectGroup)
			Deselect();
	}

	void SelectableDragHandle::OnSelected()
	{}

	void SelectableDragHandle::OnDeselected()
	{}



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

	void ISelectableDragHandlesGroup::SetHandleSelectedState(SelectableDragHandle* handle, bool selected)
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

	void SelectableDragHandlesGroup::SelectHandle(SelectableDragHandle* handle)
	{
		if (mSelectedHandles.Contains(handle))
			return;

		handle->mIsSelected = true;
		mSelectedHandles.Add(handle);
		handle->OnSelected();
	}

	void SelectableDragHandlesGroup::DeselectHandle(SelectableDragHandle* handle)
	{
		if (!mSelectedHandles.Contains(handle))
			return;

		handle->mIsSelected = false;
		mSelectedHandles.Remove(handle);
		handle->OnDeselected();
	}

	void SelectableDragHandlesGroup::AddHandle(SelectableDragHandle* handle)
	{
		if (mHandles.Contains(handle))
			return;

		mHandles.Add(handle);
	}

	void SelectableDragHandlesGroup::RemoveHandle(SelectableDragHandle* handle)
	{
		mHandles.Remove(handle);
		mSelectedHandles.Remove(handle);
	}

	void SelectableDragHandlesGroup::OnHandleCursorPressed(SelectableDragHandle* handle, const Input::Cursor& cursor)
	{
		for (auto handle : GetSelectedHandles())
		{
			handle->mDragOffset = handle->mPosition - handle->screenToLocalTransformFunc(cursor.position);
			handle->mDragPosition = handle->mPosition;
		}
	}

	void SelectableDragHandlesGroup::OnHandleCursorReleased(SelectableDragHandle* handle, const Input::Cursor& cursor)
	{
		if (!o2Input.IsKeyDown(VK_SHIFT))
			DeselectAll();

		SelectHandle(handle);
	}

	void SelectableDragHandlesGroup::OnHandleBeganDragging(SelectableDragHandle* handle)
	{
		if (handle->IsSelected())
			return;

		if (!o2Input.IsKeyDown(VK_SHIFT))
			DeselectAll();

		SelectHandle(handle);
	}

	void SelectableDragHandlesGroup::OnHandleMoved(SelectableDragHandle* handle, const Input::Cursor& cursor)
	{
		for (auto handle : GetSelectedHandles())
		{
			handle->mDragPosition = handle->screenToLocalTransformFunc(cursor.position) + handle->mDragOffset;
			handle->SetPosition(handle->mDragPosition);
			handle->onChangedPos(handle->GetPosition());
		}
	}

}

CLASS_META(o2::DragHandle)
{
	BASE_CLASS(o2::IDrawable);
	BASE_CLASS(o2::CursorAreaEventsListener);
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(angle);
	PUBLIC_FIELD(cursorType);
	PUBLIC_FIELD(onChangedPos);
	PUBLIC_FIELD(position);
	PUBLIC_FIELD(enabled);
	PUBLIC_FIELD(screenToLocalTransformFunc);
	PUBLIC_FIELD(localToScreenTransformFunc);
	PUBLIC_FIELD(checkPositionFunc);
	PUBLIC_FIELD(onRightButtonPressed);
	PUBLIC_FIELD(onRightButtonReleased);
	PROTECTED_FIELD(mEnabled);
	PROTECTED_FIELD(mRegularSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHoverSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPressedSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPosition);
	PROTECTED_FIELD(mAngle);
	PROTECTED_FIELD(mDragOffset);
	PROTECTED_FIELD(mDragPosition);
	PROTECTED_FIELD(mDragBeginPosition);
	PROTECTED_FIELD(mIsHovered);
	PROTECTED_FIELD(mIsPressed);
	PROTECTED_FIELD(mPressedCursorId);
	PROTECTED_FIELD(mIsDragging);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, SetPosition, const Vec2F&);
	PUBLIC_FUNCTION(void, SetDragPosition, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPosition);
	PUBLIC_FUNCTION(Vec2F, GetDraggingOffset);
	PUBLIC_FUNCTION(Vec2F, GetDraggingBeginPosition);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(void, SetAngle, float);
	PUBLIC_FUNCTION(float, GetAngle);
	PUBLIC_FUNCTION(void, SetRegularSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetRegularSprite);
	PUBLIC_FUNCTION(void, SetHoverSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetHoverSprite);
	PUBLIC_FUNCTION(void, SetPressedSprite, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetPressedSprite);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(o2::SelectableDragHandle)
{
	BASE_CLASS(o2::DragHandle);

	PROTECTED_FIELD(mSelectedSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIsSelected);
	PROTECTED_FIELD(mSelectGroup);
	PROTECTED_FIELD(mDragDistanceThreshold);
	PROTECTED_FIELD(mPressedCursorPos);

	PUBLIC_FUNCTION(bool, IsSelected);
	PUBLIC_FUNCTION(void, SetSelected, bool);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Deselect);
	PUBLIC_FUNCTION(void, SetSelectionGroup, ISelectableDragHandlesGroup*);
	PUBLIC_FUNCTION(ISelectableDragHandlesGroup*, GetSelectionGroup);
	PUBLIC_FUNCTION(void, SetAngle, float);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleasedOutside, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnSelected);
	PROTECTED_FUNCTION(void, OnDeselected);
}
END_META;
 