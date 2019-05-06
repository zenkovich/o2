#include "stdafx.h"
#include "KeyHandlesSheet.h"

#include "Scene/UI/WidgetLayout.h"
#include "Timeline.h"
#include "Tree.h"

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

		if (mSelectionFrame->enabled) {
			mSelectionFrame->SetRect(RectF(mTimeline->LocalToWorld(mSelectionRect.left) + mSelectionFrameOffsets.left,
										   mTree->GetLineWorldPosition(mSelectionRect.top) + mSelectionFrameOffsets.top,
										   mTimeline->LocalToWorld(mSelectionRect.right) + mSelectionFrameOffsets.right,
										   mTree->GetLineWorldPosition(mSelectionRect.bottom) + mSelectionFrameOffsets.bottom));
			mSelectionFrame->Draw();
		}

		for (auto child : mHandles)
			child->Draw();

		o2Render.DisableScissorTest();

		DrawDebugFrame();
	}

	void KeyHandlesSheet::SetTimeline(AnimationTimeline* timeline)
	{
		mTimeline = timeline;
	}

	void KeyHandlesSheet::SetTree(AnimationTree* tree)
	{
		mTree = tree;
	}

	void KeyHandlesSheet::OnSelectionChanged()
	{
		if (mSelectedHandles.Count() > 1)
		{
			mSelectionFrame->enabled = true;

			mSelectionRect.left = mSelectedHandles.First()->GetPosition().x;
			mSelectionRect.bottom = mTree->GetLineNumber(mSelectedHandles.First()->GetScreenPosition().y);
			mSelectionRect.right = mSelectionRect.left;
			mSelectionRect.top = mSelectionRect.bottom;

			for (auto handle : mSelectedHandles) {
				float localPos = handle->GetPosition().x;
				float lineNumber = mTree->GetLineNumber(handle->GetScreenPosition().y);

				mSelectionRect.left = Math::Min(mSelectionRect.left, localPos);
				mSelectionRect.right = Math::Max(mSelectionRect.right, localPos);

				mSelectionRect.bottom = Math::Max(mSelectionRect.bottom, Math::Ceil(lineNumber));
				mSelectionRect.top = Math::Min(mSelectionRect.top, Math::Floor(lineNumber - 0.5f));
			}
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
