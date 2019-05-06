#include "stdafx.h"
#include "KeyHandlesSheet.h"

#include "Scene/UI/WidgetLayout.h"
#include "Timeline.h"

namespace Editor
{

	KeyHandlesSheet::KeyHandlesSheet() :
		Widget()
	{
		mSelectionFrame = mnew Sprite("ui/UI4_keys_select.png");
		mSelectionFrame->enabled = false;
	}

	KeyHandlesSheet::KeyHandlesSheet(const KeyHandlesSheet& other) :
		Widget(other), mSelectionFrame(other.mSelectionFrame->CloneAs<Sprite>())
	{}

	KeyHandlesSheet::~KeyHandlesSheet()
	{
		delete mSelectionFrame;
	}

	KeyHandlesSheet& KeyHandlesSheet::operator=(const KeyHandlesSheet& other)
	{
		delete mSelectionFrame;

		Widget::operator=(other);
		
		mSelectionFrame = other.mSelectionFrame->CloneAs<Sprite>();

		return *this;
	}

	void KeyHandlesSheet::Update(float dt)
	{
		Widget::Update(dt);
	}

	void KeyHandlesSheet::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		Widget::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

		o2Render.EnableScissorTest(layout->GetWorldRect());

		for (auto child : mHandles)
			child->Draw();

		o2Render.DisableScissorTest();

		DrawDebugFrame();
	}

	void KeyHandlesSheet::SetTimeline(AnimationTimeline* timeline)
	{
		mTimeline = timeline;
	}

	void KeyHandlesSheet::OnSelectionChanged()
	{
		if (!mSelectedHandles.IsEmpty())
		{
			mSelectionFrame->enabled = true;

			//RectF rect(mSelectedHandles.First().GetScreenPosition
		}
		else mSelectionFrame->enabled = false;
	}

	void KeyHandlesSheet::OnCursorPressed(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorReleased(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorPressBreak(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorPressedOutside(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorReleasedOutside(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorStillDown(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorMoved(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorEnter(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorExit(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorDblClicked(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorRightMousePressed(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorMiddleMousePressed(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorMiddleMouseStayDown(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorMiddleMouseReleased(const Input::Cursor& cursor)
	{

	}

}

DECLARE_CLASS(Editor::KeyHandlesSheet);
