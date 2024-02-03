const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorEnter, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorExit, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMousePressed, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseStayDown, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseReleased, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMiddleMousePressed, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMiddleMouseStayDown, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMiddleMouseReleased, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnScrolled, float);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyPressed, const Input::Key&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyReleased, const Input::Key&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyStayDown, const Input::Key&);
}
END_META;

namespace Editor
{
	Toggle* IEditTool::GetPanelToggle() const
	{
		if (!mPanelToggle)
		{
			mPanelToggle = CreatePanelToggle();
		}
		return mPanelToggle;
	}

	Toggle* IEditTool::CreatePanelToggle() const
	{
		Toggle* toggle = new Toggle();
		toggle->SetIcon(GetPanelIcon());
		toggle->SetShortcutKeys(GetShortcut());
		toggle->SetEnableHandler([this](bool enable)
		{
			if (enable)
			{
				OnEnabled();
			}
			else
			{
				OnDisabled();
			}
		});
		toggle->SetDrawScreenHandler([this]()
		{
			DrawScreen();
		});
		toggle->SetCursorPressedHandler([this](const Input::Cursor& cursor)
		{
			OnCursorPressed(cursor);
		});
		toggle->SetCursorReleasedHandler([this](const Input::Cursor& cursor)
		{
			OnCursorReleased(cursor);
		});
		toggle->SetCursorPressBreakHandler([this](const Input::Cursor& cursor)
		{
			OnCursorPressBreak(cursor);
		});
		toggle->SetCursorStillDownHandler([this](const Input::Cursor& cursor)
		{
			OnCursorStillDown(cursor);
		});
		toggle->SetCursorMovedHandler([this](const Input::Cursor& cursor)
		{
			OnCursorMoved(cursor);
		});
		toggle->SetCursorEnterHandler([this](const Input::Cursor& cursor)
		{
			OnCursorEnter(cursor);
		});
		toggle->SetCursorExitHandler([this](const Input::Cursor& cursor)
		{
			OnCursorExit(cursor);
		});
		toggle->SetCursorRightMousePressedHandler([this](const Input::Cursor& cursor)
		{
			OnCursorRightMousePressed(cursor);
		});
		toggle->SetCursorRightMouseStayDownHandler([this](const Input::Cursor& cursor)
		{
			OnCursorRightMouseStayDown(cursor);
		});
		toggle->SetCursorRightMouseReleasedHandler([this](const Input::Cursor& cursor)
		{
			OnCursorRightMouseReleased(cursor);
		});
		toggle->SetCursorMiddleMousePressedHandler([this](const Input::Cursor& cursor)
		{
			OnCursorMiddleMousePressed(cursor);
		});
		toggle->SetCursorMiddleMouseStayDownHandler([this](const Input::Cursor& cursor)
		{
			OnCursorMiddleMouseStayDown(cursor);
		});
		toggle->SetCursorMiddleMouseReleasedHandler([this](const Input::Cursor& cursor)
		{
			OnCursorMiddleMouseReleased(cursor);
		});
		toggle->SetScrolledHandler([this](float scroll)
		{
			OnScrolled(scroll);
		});
		toggle->SetKeyPressedHandler([this](const Input::Key& key)
		{
			OnKeyPressed(key);
		});
		toggle->SetKeyReleasedHandler([this](const Input::Key& key)
		{
			OnKeyReleased(key);
		});
		toggle->SetKeyStayDownHandler([this](const Input::Key& key)
		{
			OnKeyStayDown(key);
		});
		return toggle;
	}
}replace raw pointers with Ref<>, except void* pointers. Ref<> is a smart pointer like shared_ptr. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Don't replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>. /n const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorEnter, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorExit, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMousePressed, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseStayDown, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseReleased, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMiddleMousePressed, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMiddleMouseStayDown, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMiddleMouseReleased, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnScrolled, float);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyPressed, const Input::Key&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyReleased, const Input::Key&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyStayDown, const Input::Key&);
}
END_META;
// --- END META ---