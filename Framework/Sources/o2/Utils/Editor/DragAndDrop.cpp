#include "o2/stdafx.h"
#include "DragAndDrop.h"

#include "o2/Application/Input.h"
#include "o2/Events/EventSystem.h"

namespace o2
{
    DragableObject::DragableObject() :CursorAreaEventsListener()
    {}

    DragableObject::~DragableObject()
    {
        EventSystem::UnregDragListener(this);
    }

    Ref<DragDropArea> DragableObject::GetDropAreaUnderCursor(CursorId cursorId) const
    {
        auto underCursorListeners = o2Events.GetAllCursorListenersUnderCursor(cursorId);
        Ref<DragDropArea> dragDropArea = nullptr;

        for (auto listener : underCursorListeners)
        {
            if (listener != this)
            {
                if (auto listenerDragDropArea = DynamicCast<DragDropArea>(listener))
                {
                    dragDropArea = listenerDragDropArea;
                    break;
                }

                if (!listener->IsInputTransparent())
                    break;
            }
        }

        return dragDropArea;
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

        auto dragArea = GetDropAreaUnderCursor(cursor.id);

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
                    mDragDropArea->OnDragExit(Ref(this));

                mDragDropArea = dragArea;

                if (mDragDropArea)
                    mDragDropArea->OnDragEnter(Ref(this));
            }

            if (dragArea)
                dragArea->OnDraggedAbove(Ref(this));
        }
    }

    void DragableObject::OnCursorReleased(const Input::Cursor& cursor)
    {
        if (mIsDragging)
        {
            OnDragEnd(cursor);

            if (auto dragArea = GetDropAreaUnderCursor(cursor.id))
            {
                OnDropped(dragArea);
                dragArea->OnDropped(Ref(this));
            }
        }

        mDragDropArea = nullptr;
        mIsDragging = false;
    }

    void DragableObject::OnDragStart(const Input::Cursor& cursor)
    {}

    void DragableObject::OnDragged(const Input::Cursor& cursor, const Ref<DragDropArea>& area)
    {}

    void DragableObject::OnDragEnd(const Input::Cursor& cursor)
    {}

    void DragableObject::OnDropped(const Ref<DragDropArea>& area)
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

    void DragDropArea::OnDropped(const Ref<DragableObject>& draggable)
    {}

    void DragDropArea::OnDropped(const Ref<ISelectableDragableObjectsGroup>& group)
    {}

    void DragDropArea::OnDragEnter(const Ref<DragableObject>& draggable)
    {}

    void DragDropArea::OnDragEnter(const Ref<ISelectableDragableObjectsGroup>& group)
    {}

    void DragDropArea::OnDraggedAbove(const Ref<DragableObject>& draggable)
    {}

    void DragDropArea::OnDraggedAbove(const Ref<ISelectableDragableObjectsGroup>& group)
    {}

    void DragDropArea::OnDragExit(const Ref<DragableObject>& draggable)
    {}

    void DragDropArea::OnDragExit(const Ref<ISelectableDragableObjectsGroup>& group)
    {}

    SelectableDragableObjectsGroup::~SelectableDragableObjectsGroup()
    {
        auto objects = mObjects;
        for (auto object : objects)
            RemoveSelectableObject(object.Get());
    }

    Vector<Ref<SelectableDragableObject>> SelectableDragableObjectsGroup::GetSelectedDragObjects() const
    {
        return mSelectedObjects;
    }

    Vector<Ref<SelectableDragableObject>> SelectableDragableObjectsGroup::GetAllObjects() const
    {
        return mObjects;
    }

    void SelectableDragableObjectsGroup::Select(const Ref<SelectableDragableObject>& object)
    {
        if (object->IsSelected())
            return;

        object->mIsSelected = true;
        mSelectedObjects.Add(object);
        object->OnSelected();
    }

    void SelectableDragableObjectsGroup::Deselect(const Ref<SelectableDragableObject>& object)
    {
        if (!object->IsSelected())
            return;

        object->mIsSelected = false;
        mSelectedObjects.Remove(object);
        object->OnDeselected();
    }

    void SelectableDragableObjectsGroup::AddSelectableObject(const Ref<SelectableDragableObject>& object)
    {
        if (mObjects.Contains(object))
            return;

        mObjects.Add(object);

        if (object->IsSelected())
            mSelectedObjects.Add(object);

        object->mSelectGroup = Ref(this);
    }

    void SelectableDragableObjectsGroup::RemoveSelectableObject(SelectableDragableObject* object)
    {
        mSelectedObjects.RemoveFirst([&](auto& x) { return x == object; });
        mObjects.RemoveFirst([&](auto& x) { return x == object; });

        object->mSelectGroup = nullptr;
    }

    bool SelectableDragableObjectsGroup::IsUnderPoint(const Vec2F& point)
    {
        return true;
    }

    void SelectableDragableObjectsGroup::OnSelectableObjectCursorPressed(const Ref<SelectableDragableObject>& object,
                                                                         const Input::Cursor& cursor)
    {
        if (!o2Input.IsKeyDown(VK_CONTROL))
            DeselectAll();
    }

    SelectableDragableObject::~SelectableDragableObject()
    {
        if (mSelectGroup)
            mSelectGroup.Lock()->RemoveSelectableObject(this);
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
                mSelectGroup.Lock()->Select(Ref(this));
            else
                mSelectGroup.Lock()->Deselect(Ref(this));
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

    void SelectableDragableObject::SetSelectionGroup(const Ref<ISelectableDragableObjectsGroup>& group)
    {
        if (mSelectGroup)
            mSelectGroup.Lock()->RemoveSelectableObject(this);

        mSelectGroup = group;

        if (mSelectGroup)
            mSelectGroup.Lock()->AddSelectableObject(Ref(this));
    }

    Ref<ISelectableDragableObjectsGroup> SelectableDragableObject::GetSelectionGroup() const
    {
        return mSelectGroup.Lock();
    }

    void SelectableDragableObject::SetDragOnlySelected(bool value)
    {
        mDragOnlySelected = value;
    }

    bool SelectableDragableObject::IsDragOnlySelected() const
    {
        return mDragOnlySelected;
    }

    bool SelectableDragableObject::IsInputTransparent() const
    {
        return mDragOnlySelected && !mIsSelected;
    }

    void SelectableDragableObject::OnCursorPressed(const Input::Cursor& cursor)
    {
        DragableObject::OnCursorPressed(cursor);

        if (mSelectGroup)
            mSelectGroup.Lock()->OnSelectableObjectCursorPressed(Ref(this), cursor);

        if (mDragOnlySelected && !mIsSelected)
            mIsDragAvailable = false;

        mPressedTime = o2Time.GetApplicationTime();

        CursorAreaEventsListener::OnCursorPressed(cursor);
    }

    void SelectableDragableObject::OnCursorStillDown(const Input::Cursor& cursor)
    {
        if (!mIsPressed || cursor.id != mPressedCursorId || cursor.delta.Length() < 0.5f)
        {
            CursorAreaEventsListener::OnCursorStillDown(cursor);
            return;
        }

        Ref<DragDropArea> dragDropArea = GetDropAreaUnderCursor(cursor.id);

        if (!mIsDragAvailable)
        {
            if (o2Time.GetApplicationTime() - mPressedTime < mDragDelay)
                mIsDragAvailable = true;
        }

        if (!mIsDragging && mIsDragAvailable)
        {
            float delta = (cursor.position - mPressedCursorPos).Length();
            if (delta > mDragDistanceThreshold)
            {
                mIsDragging = true;

                if (auto selectGroup = mSelectGroup.Lock())
                {
                    selectGroup->OnSelectableObjectBeganDragging(Ref(this));
                    selectGroup->mDraggingObject = Ref(this);
                }
                else
                    Select();

                OnDragStart(cursor);
            }
        }

        if (mIsDragging)
        {
            OnDragged(cursor, dragDropArea);

            if (mDragDropArea != dragDropArea)
            {
                if (mDragDropArea)
                {
                    if (mSelectGroup)
                        mDragDropArea->OnDragExit(mSelectGroup.Lock());
                    else
                        mDragDropArea->OnDragExit(Ref(this));
                }

                mDragDropArea = dragDropArea;

                if (mDragDropArea)
                {
                    if (mSelectGroup)
                        mDragDropArea->OnDragEnter(mSelectGroup.Lock());
                    else
                        mDragDropArea->OnDragEnter(Ref(this));
                }
            }

            if (dragDropArea)
            {
                if (mSelectGroup)
                    dragDropArea->OnDraggedAbove(mSelectGroup.Lock());
                else
                    dragDropArea->OnDraggedAbove(Ref(this));
            }
        }

        CursorAreaEventsListener::OnCursorStillDown(cursor);
    }

    void SelectableDragableObject::OnCursorReleased(const Input::Cursor& cursor)
    {
        if (mIsDragging)
        {
            OnDragEnd(cursor);

            if (Ref<DragDropArea> dragDropArea = GetDropAreaUnderCursor(cursor.id))
            {
                OnDropped(dragDropArea);
                if (mSelectGroup)
                {
                    dragDropArea->OnDropped(mSelectGroup.Lock());
                    mSelectGroup.Lock()->mDraggingObject = nullptr;
                }
                else
                    dragDropArea->OnDropped(Ref(this));
            }

            mDragDropArea = nullptr;
            mIsDragging = false;
        }
        else
        {
            if (mSelectGroup)
                mSelectGroup.Lock()->OnSelectableObjectCursorReleased(Ref(this), cursor);
            else
                SetSelected(!IsSelected());
        }

        mIsDragAvailable = true;
        CursorAreaEventsListener::OnCursorReleased(cursor);
    }

    void SelectableDragableObject::OnCursorReleasedOutside(const Input::Cursor& cursor)
    {
        if (!mSelectGroup)
            Deselect();

        mIsDragAvailable = true;
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

    const Ref<SelectableDragableObject> ISelectableDragableObjectsGroup::GetDraggingObject() const
    {
        return mDraggingObject;
    }

}
