#include "DragAndDrop.h"

#include "Application/Input.h"
#include "Events/EventSystem.h"

namespace o2
{
	DragableObject::DragableObject():CursorAreaEventsListener()
	{}

	DragableObject::~DragableObject()
	{
		EventSystem::UnregDragListener(this);
	}

	void DragableObject::OnCursorPressed(const Input::Cursor& cursor)
	{
		mPressedCursorPos = cursor.position;
		mPressedCursorId = cursor.id;
	}

	void DragableObject::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		mIsDragging = false;
	}

	void DragableObject::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (!mIsPressed)
			return;

		if (cursor.id != mPressedCursorId)
			return;

		if (cursor.delta.Length() < 0.5f)
			return;

		auto underCursorListener = o2Events.GetCursorListenerUnderCursor(cursor.id);
		auto dragArea = dynamic_cast<DragDropArea*>(underCursorListener);

		if (!mIsDragging)
		{
			float delta = (cursor.position - mPressedCursorPos).Length();
			if (delta > mDragDistanceThreshold)
			{
				mIsDragging = true;
				OnDragStart(cursor);
			}
		}

		if (mIsDragging)
		{
			OnDragged(cursor, dragArea);

			if (mDragDropArea != dragArea)
			{
				if (mDragDropArea)
					mDragDropArea->OnDragExit(this);

				mDragDropArea = dragArea;

				if (mDragDropArea)
					mDragDropArea->OnDragEnter(this);
			}

			if (dragArea)
				dragArea->OnDraggedAbove(this);
		}
	}

	void DragableObject::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			OnDragEnd(cursor);

			auto underCursorListener = o2Events.GetCursorListenerUnderCursor(cursor.id);
			auto dragArea = dynamic_cast<DragDropArea*>(underCursorListener);

			if (dragArea)
			{
				OnDropped(dragArea);
				dragArea->OnDropped(this);
			}
		}

		mDragDropArea = nullptr;
		mIsDragging = false;
	}

	void DragableObject::OnDragStart(const Input::Cursor& cursor)
	{}

	void DragableObject::OnDragged(const Input::Cursor& cursor, DragDropArea* area)
	{}

	void DragableObject::OnDragEnd(const Input::Cursor& cursor)
	{}

	void DragableObject::OnDropped(DragDropArea* area)
	{}

	bool DragableObject::IsDragging() const
	{
		return mIsDragging;
	}

	void DragableObject::OnDrawn()
	{
		CursorAreaEventsListener::OnDrawn();
		EventSystem::RegDragListener(this);
	}

	Vec2F DragableObject::GetCursorPressedPoint() const
	{
		return mPressedCursorPos;
	}

	void DragDropArea::OnDropped(DragableObject* draggable)
	{}

	void DragDropArea::OnDropped(ISelectableDragableObjectsGroup* group)
	{}

	void DragDropArea::OnDragEnter(DragableObject* draggable)
	{}

	void DragDropArea::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{}

	void DragDropArea::OnDraggedAbove(DragableObject* draggable)
	{}

	void DragDropArea::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{}

	void DragDropArea::OnDragExit(DragableObject* draggable)
	{}

	void DragDropArea::OnDragExit(ISelectableDragableObjectsGroup* group)
	{}

	SelectableDragableObjectsGroup::~SelectableDragableObjectsGroup()
	{
		auto objects = mObjects;
		for (auto object : objects)
			RemoveSelectableObject(object);
	}

	SelectableDragableObjectsGroup::SelectDragObjectsVec SelectableDragableObjectsGroup::GetSelectedDragObjects() const
	{
		return mSelectedObjects;
	}

	SelectableDragableObjectsGroup::SelectDragObjectsVec SelectableDragableObjectsGroup::GetAllObjects() const
	{
		return mObjects;
	}

	void SelectableDragableObjectsGroup::Select(SelectableDragableObject* object)
	{
		if (object->IsSelected())
			return;

		object->mIsSelected = true;
		mSelectedObjects.Add(object);
		object->OnSelected();
	}

	void SelectableDragableObjectsGroup::Deselect(SelectableDragableObject* object)
	{
		if (!object->IsSelected())
			return;

		object->mIsSelected = false;
		mSelectedObjects.Remove(object);
		object->OnDeselected();
	}

	void SelectableDragableObjectsGroup::AddSelectableObject(SelectableDragableObject* object)
	{
		if (mObjects.Contains(object))
			return;

		mObjects.Add(object);

		if (object->IsSelected())
			mSelectedObjects.Add(object);

		object->mSelectGroup = this;
	}

	void SelectableDragableObjectsGroup::RemoveSelectableObject(SelectableDragableObject* object)
	{
		mSelectedObjects.Remove(object);
		mObjects.Remove(object);

		object->mSelectGroup = nullptr;
	}

	bool SelectableDragableObjectsGroup::IsUnderPoint(const Vec2F& point)
	{
		return true;
	}

	void SelectableDragableObjectsGroup::OnSelectableObjectCursorPressed(SelectableDragableObject* object,
																		 const Input::Cursor& cursor)
	{
		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAll();
	}

	SelectableDragableObject::~SelectableDragableObject()
	{
		if (mSelectGroup)
			mSelectGroup->RemoveSelectableObject(this);
	}

	bool SelectableDragableObject::IsSelected() const
	{
		return mIsSelected;
	}

	void SelectableDragableObject::SetSelected(bool selected)
	{
		if (mIsSelected == selected)
			return;

		if (mSelectGroup)
		{
			if (selected)
				mSelectGroup->Select(this);
			else
				mSelectGroup->Deselect(this);
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

	void SelectableDragableObject::Select()
	{
		SetSelected(true);
	}

	void SelectableDragableObject::Deselect()
	{
		SetSelected(false);
	}

	void SelectableDragableObject::SetSelectionGroup(ISelectableDragableObjectsGroup* group)
	{
		if (mSelectGroup)
			mSelectGroup->RemoveSelectableObject(this);

		mSelectGroup = group;

		if (mSelectGroup)
			mSelectGroup->AddSelectableObject(this);
	}

	ISelectableDragableObjectsGroup* SelectableDragableObject::GetSelectionGroup() const
	{
		return mSelectGroup;
	}

	void SelectableDragableObject::OnCursorPressed(const Input::Cursor& cursor)
	{
		DragableObject::OnCursorPressed(cursor);

		if (mSelectGroup)
			mSelectGroup->OnSelectableObjectCursorPressed(this, cursor);

		CursorAreaEventsListener::OnCursorPressed(cursor);
	}

	void SelectableDragableObject::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (!mIsPressed)
			return;

		if (cursor.id != mPressedCursorId)
			return;

		if (cursor.delta.Length() < 0.5f)
			return;


		auto underCursorListeners = o2Events.GetAllCursorListenersUnderCursor(cursor.id);
		CursorAreaEventsListener* underCursorListener = nullptr;

		for (auto listener : underCursorListeners)
		{
			if (listener != this)
			{
				underCursorListener = listener;
				break;
			}
		}

		auto dragArea = dynamic_cast<DragDropArea*>(underCursorListener);

		if (!mIsDragging)
		{
			float delta = (cursor.position - mPressedCursorPos).Length();
			if (delta > mDragDistanceThreshold)
			{
				mIsDragging = true;

				if (mSelectGroup)
					mSelectGroup->OnSelectableObjectBeganDragging(this);
				else 
					Select();

				OnDragStart(cursor);
			}
		}

		if (mIsDragging)
		{
			OnDragged(cursor, dragArea);

			if (mDragDropArea != dragArea)
			{
				if (mDragDropArea)
				{
					if (mSelectGroup)
						mDragDropArea->OnDragExit(mSelectGroup);
					else
						mDragDropArea->OnDragExit(this);
				}

				mDragDropArea = dragArea;

				if (mDragDropArea)
				{
					if (mSelectGroup)
						mDragDropArea->OnDragEnter(mSelectGroup);
					else
						mDragDropArea->OnDragEnter(this);
				}
			}

			if (dragArea)
			{
				if (mSelectGroup)
					dragArea->OnDraggedAbove(mSelectGroup);
				else
					dragArea->OnDraggedAbove(this);
			}
		}

		CursorAreaEventsListener::OnCursorStillDown(cursor);
	}

	void SelectableDragableObject::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mIsDragging)
		{
			OnDragEnd(cursor);

			auto underCursorListeners = o2Events.GetAllCursorListenersUnderCursor(cursor.id);
			CursorAreaEventsListener* underCursorListener = nullptr;

			for (auto listener : underCursorListeners)
			{
				if (listener != this)
				{
					underCursorListener = listener;
					break;
				}
			}

			auto dragArea = dynamic_cast<DragDropArea*>(underCursorListener);

			if (dragArea)
			{
				OnDropped(dragArea);
				if (mSelectGroup)
					dragArea->OnDropped(mSelectGroup);
				else
					dragArea->OnDropped(this);
			}

			mDragDropArea = nullptr;
			mIsDragging = false;
		}
		else
		{
			if (mSelectGroup)
				mSelectGroup->OnSelectableObjectCursorReleased(this, cursor);
			else
				SetSelected(!IsSelected());
		}

		CursorAreaEventsListener::OnCursorReleased(cursor);
	}

	void SelectableDragableObject::OnCursorReleasedOutside(const Input::Cursor& cursor)
	{
		if (!mSelectGroup)
			Deselect();
	}

	void SelectableDragableObject::OnSelected()
	{}

	void SelectableDragableObject::OnDeselected()
	{}

	void ISelectableDragableObjectsGroup::DeselectAll()
	{
		auto objects = GetAllObjects();
		for (auto object : objects)
			Deselect(object);
	}

	void ISelectableDragableObjectsGroup::SelectAll()
	{
		auto objects = GetAllObjects();
		for (auto object : objects)
			Select(object);
	}
}