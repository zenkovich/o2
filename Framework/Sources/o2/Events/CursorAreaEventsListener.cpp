#include "o2/stdafx.h"
#include "CursorAreaEventsListener.h"

#include "o2/Events/EventSystem.h"
#include "o2/Render/IDrawable.h"
#include "o2/Render/Render.h"

namespace o2
{
    CursorAreaEventsListener::CursorAreaEventsListener()
    {}

    CursorAreaEventsListener::CursorAreaEventsListener(const CursorAreaEventsListener& other):
        interactable(this)
    {}

    CursorAreaEventsListener::~CursorAreaEventsListener()
    {
        if (EventSystem::IsSingletonInitialzed())
            o2Events.UnregCursorAreaListener(this);
    }

    bool CursorAreaEventsListener::IsUnderPoint(const Vec2F& point)
    {
        return false;
    }

    bool CursorAreaEventsListener::IsScrollable() const
    {
        return false;
    }

    void CursorAreaEventsListener::SetInteractable(bool interactable)
    {
        if (mInteractable == interactable)
            return;

        mInteractable = interactable;

        if (mInteractable)
            OnBecomeInteractable();
        else
            OnBecomeNotInteractable();
    }

    bool CursorAreaEventsListener::IsInteractable() const
    {
        return mInteractable;
    }

    bool CursorAreaEventsListener::IsInputTransparent() const
    {
        return false;
    }

    void CursorAreaEventsListener::OnDrawn()
    {
        if (!mInteractable)
            return;

        mScissorRect = o2Render.GetResScissorRect();
        o2Events.DrawnCursorAreaListener(Ref(this));
    }

    void CursorAreaEventsListener::OnCursorPressed(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorPressed(cursor);
    }

    void CursorAreaEventsListener::OnCursorReleased(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorReleased(cursor);
    }

    void CursorAreaEventsListener::OnCursorPressBreak(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorPressBreak(cursor);
    }

    void CursorAreaEventsListener::OnCursorPressedOutside(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorPressedOutside(cursor);
    }

    void CursorAreaEventsListener::OnCursorReleasedOutside(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorReleasedOutside(cursor);
    }

    void CursorAreaEventsListener::OnCursorStillDown(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorStillDown(cursor);
    }

    void CursorAreaEventsListener::OnCursorMoved(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorMoved(cursor);
    }

    void CursorAreaEventsListener::OnCursorEnter(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorEnter(cursor);
    }

    void CursorAreaEventsListener::OnCursorExit(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorExit(cursor);
    }

    void CursorAreaEventsListener::OnCursorDblClicked(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorDblClicked(cursor);
    }

    void CursorAreaEventsListener::OnCursorRightMousePressed(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorRightMousePressed(cursor);
    }

    void CursorAreaEventsListener::OnCursorRightMouseReleased(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorRightMouseReleased(cursor);
    }

    void CursorAreaEventsListener::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorRightMouseStayDown(cursor);
    }

    void CursorAreaEventsListener::OnCursorMiddleMousePressed(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorMiddleMousePressed(cursor);
    }

    void CursorAreaEventsListener::OnCursorMiddleMouseStayDown(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorMiddleMouseStayDown(cursor);
    }

    void CursorAreaEventsListener::OnCursorMiddleMouseReleased(const Input::Cursor& cursor)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnCursorMiddleMouseReleased(cursor);
    }

    void CursorAreaEventsListener::OnScrolled(float scroll)
    {
        if (messageFallDownListener)
            messageFallDownListener->OnScrolled(scroll);
    }

    bool CursorAreaEventsListener::IsPressed() const
    {
        return mIsPressed;
    }

    CursorEventsListener::CursorEventsListener()
    {
        if (EventSystem::IsSingletonInitialzed())
            EventSystem::RegCursorListener(this);
    }

    CursorEventsListener::CursorEventsListener(const CursorAreaEventsListener& other)
    {
        if (EventSystem::IsSingletonInitialzed())
            EventSystem::RegCursorListener(this);
    }

    CursorEventsListener::~CursorEventsListener()
    {
        EventSystem::UnregCursorListener(this);
    }

    CursorAreaEventsListenerDelegate::CursorAreaEventsListenerDelegate(RefCounter* refCounter,
                                                                       CursorAreaEventsListenerInterface& cursorListenerInterface):
        mCursorListenerInterface(cursorListenerInterface)
    {
        mCursorListenerInterface.mCursorEventsDelegate = Ref(this);
    }

    bool CursorAreaEventsListenerDelegate::IsUnderPoint(const Vec2F& point)
    {
        return mCursorListenerInterface.IsUnderPoint(point);
    }

    bool CursorAreaEventsListenerDelegate::IsScrollable() const
    {
        return mCursorListenerInterface.IsScrollable();
    }

    bool CursorAreaEventsListenerDelegate::IsInputTransparent() const
    {
        return mCursorListenerInterface.IsInputTransparent();
    }

    void CursorAreaEventsListenerDelegate::OnCursorPressed(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorPressed(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorReleased(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorReleased(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorPressBreak(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorPressBreak(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorPressedOutside(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorPressedOutside(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorReleasedOutside(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorReleasedOutside(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorStillDown(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorStillDown(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorMoved(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorMoved(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorEnter(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorEnter(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorExit(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorExit(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorDblClicked(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorDblClicked(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorRightMousePressed(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorRightMousePressed(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorRightMouseStayDown(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorRightMouseReleased(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorRightMouseReleased(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorMiddleMousePressed(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorMiddleMousePressed(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorMiddleMouseStayDown(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorMiddleMouseStayDown(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnCursorMiddleMouseReleased(const Input::Cursor& cursor)
    {
        mCursorListenerInterface.OnCursorMiddleMouseReleased(cursor);
    }

    void CursorAreaEventsListenerDelegate::OnScrolled(float scroll)
    {
        mCursorListenerInterface.OnScrolled(scroll);
    }

    bool CursorAreaEventsListenerInterface::IsUnderPoint(const Vec2F& point)
    {
        return true;
    }

    bool CursorAreaEventsListenerInterface::IsScrollable() const
    {
        return false;
    }

    void CursorAreaEventsListenerInterface::SetInteractable(bool interactable)
    {
        if (mCursorEventsDelegate)
            mCursorEventsDelegate.Lock()->SetInteractable(interactable);
    }

    bool CursorAreaEventsListenerInterface::IsInteractable() const
    {
        if (mCursorEventsDelegate)
            return mCursorEventsDelegate.Lock()->IsInteractable();

        return false;
    }

    bool CursorAreaEventsListenerInterface::IsInputTransparent() const
    {
        return false;
    }

    bool CursorAreaEventsListenerInterface::IsPressed() const
    {
        if (mCursorEventsDelegate)
            return mCursorEventsDelegate.Lock()->IsPressed();

        return false;
    }

    void CursorAreaEventsListenerInterface::OnDrawn()
    {
        if (mCursorEventsDelegate)
            mCursorEventsDelegate.Lock()->OnDrawn();
    }

}
