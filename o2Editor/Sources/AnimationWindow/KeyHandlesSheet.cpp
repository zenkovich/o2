#include "stdafx.h"
#include "KeyHandlesSheet.h"

#include "Scene/UI/WidgetLayout.h"
#include "Timeline.h"
#include "Tree.h"
#include "Animation/AnimatedValue.h"
#include "Animation/Animation.h"

namespace Editor
{

	KeyHandlesSheet::KeyHandlesSheet() :
		Widget()
	{
		mSelectionFrame = mnew Sprite("ui/UI4_keys_select.png");
		mSelectionFrame->enabled = false;

		InitializeHandles();
	}

	KeyHandlesSheet::KeyHandlesSheet(const KeyHandlesSheet& other) :
		Widget(other), mSelectionFrame(other.mSelectionFrame->CloneAs<Sprite>())
	{
		InitializeHandles();
	}

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

#undef DrawText
	void KeyHandlesSheet::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		Widget::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

		o2Render.EnableScissorTest(layout->GetWorldRect());

		if (mSelectionFrame->enabled) {
			auto offsets = mIsFrameSelecting ? mSelectionFrameCursorOffsets : mSelectionFrameOffsets;
			mSelectionFrame->SetRect(RectF(mTimeline->LocalToWorld(mSelectionRect.left) + offsets.left,
										   mTree->GetLineWorldPosition(mSelectionRect.top) + offsets.top,
										   mTimeline->LocalToWorld(mSelectionRect.right) + offsets.right,
										   mTree->GetLineWorldPosition(mSelectionRect.bottom) + offsets.bottom));
			mSelectionFrame->Draw();

			mCenterFrameDragHandle.Draw();
			mLeftFrameDragHandle.Draw();
			mRightFrameDragHandle.Draw();
		}

		o2Render.DisableScissorTest();

		DrawDebugFrame();
	}

	void KeyHandlesSheet::UpdateInputDrawOrder()
	{
		CursorAreaEventsListener::OnDrawn();

		for (auto handle : mHandles)
		{
			if (handle->IsEnabled())
				handle->CursorAreaEventsListener::OnDrawn();
		}

		mCenterFrameDragHandle.CursorAreaEventsListener::OnDrawn();
		mLeftFrameDragHandle.CursorAreaEventsListener::OnDrawn();
		mRightFrameDragHandle.CursorAreaEventsListener::OnDrawn();
	}

	void KeyHandlesSheet::Initialize(AnimationTimeline* timeline, AnimationTree* tree)
	{
		mTimeline = timeline;
		mTree = tree;
	}

	void KeyHandlesSheet::SetAnimation(Animation* animation)
	{
		mAnimation = animation;
	}

	bool KeyHandlesSheet::IsUnderPoint(const Vec2F& point)
	{
		return Widget::IsUnderPoint(point);
	}

	void KeyHandlesSheet::OnSelectionChanged()
	{
		UpdateSelectionFrame();
	}

	void KeyHandlesSheet::OnHandleMoved(DragHandle* handle, const Input::Cursor& cursor)
	{
		for (auto animatedValueDef : mAnimation->GetAnimationsValues())
			animatedValueDef.animatedValue->BeginKeysBatchChange();

		SelectableDragHandlesGroup::OnHandleMoved(handle, cursor);

		for (auto animatedValueDef : mAnimation->GetAnimationsValues())
			animatedValueDef.animatedValue->CompleteKeysBatchingChange();

		UpdateSelectionFrame();
	}

	void KeyHandlesSheet::InitializeHandles()
	{
		InitializeCenterHandle();
		InitializeLeftHandle();
		InitializeRightHandle();
	}

	void KeyHandlesSheet::InitializeCenterHandle()
	{
		mCenterFrameDragHandle.localToScreenTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mTimeline->LocalToWorld(point.x), mTree->GetLineWorldPosition(point.y));
		};

		mCenterFrameDragHandle.screenToLocalTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mTimeline->WorldToLocal(point.x), mTree->GetLineNumber(point.y));
		};

		mCenterFrameDragHandle.isPointInside = [&](const Vec2F& point) {
			auto local = Vec2F(mTimeline->WorldToLocal(point.x), mTree->GetLineNumber(point.y));
			return local.x > mSelectionRect.left && local.x < mSelectionRect.right && local.y > mSelectionRect.top && local.y < mSelectionRect.bottom;
		};

		mCenterFrameDragHandle.checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(point.x, mSelectionRect.Center().y);
		};

		mCenterFrameDragHandle.onChangedPos = [&](const Vec2F& point) {
			Vec2F delta(point.x - mSelectionRect.Center().x, mSelectionRect.Center().y);

			for (auto animatedValueDef : mAnimation->GetAnimationsValues())
				animatedValueDef.animatedValue->BeginKeysBatchChange();

			for (auto handle : GetSelectedHandles()) {
				handle->SetPosition(handle->GetPosition() + delta);
				handle->onChangedPos(handle->GetPosition());
			}

			for (auto animatedValueDef : mAnimation->GetAnimationsValues())
				animatedValueDef.animatedValue->CompleteKeysBatchingChange();

			UpdateSelectionFrame();
		};

		mCenterFrameDragHandle.cursorType = CursorType::SizeWE;
	}

	void KeyHandlesSheet::InitializeLeftHandle()
	{
		mLeftFrameDragHandle.localToScreenTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mTimeline->LocalToWorld(point.x), mTree->GetLineWorldPosition(point.y));
		};

		mLeftFrameDragHandle.screenToLocalTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mTimeline->WorldToLocal(point.x), mTree->GetLineNumber(point.y));
		};

		mLeftFrameDragHandle.isPointInside = [&](const Vec2F& point) {
			auto r = mSelectionFrame->GetRect();
			return RectF(r.left - 5, r.bottom, r.left + 5, r.top).IsInside(point);
		};

		mLeftFrameDragHandle.checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(point.x, mSelectionRect.Center().y);
		};

		mLeftFrameDragHandle.onChangedPos = [&](const Vec2F& point) {
			float scale = (point.x - mSelectionRect.right) / (mSelectionRect.left - mSelectionRect.right);

			for (auto animatedValueDef : mAnimation->GetAnimationsValues())
				animatedValueDef.animatedValue->BeginKeysBatchChange();

			for (auto handle : GetSelectedHandles()) {
				handle->SetPosition(Vec2F((handle->GetPosition().x - mSelectionRect.right)*scale + mSelectionRect.right, handle->GetPosition().y));
				handle->onChangedPos(handle->GetPosition());
			}

			for (auto animatedValueDef : mAnimation->GetAnimationsValues())
				animatedValueDef.animatedValue->CompleteKeysBatchingChange();

			UpdateSelectionFrame();
		};

		mLeftFrameDragHandle.cursorType = CursorType::SizeWE;
	}

	void KeyHandlesSheet::InitializeRightHandle()
	{
		mRightFrameDragHandle.localToScreenTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mTimeline->LocalToWorld(point.x), mTree->GetLineWorldPosition(point.y));
		};

		mRightFrameDragHandle.screenToLocalTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mTimeline->WorldToLocal(point.x), mTree->GetLineNumber(point.y));
		};

		mRightFrameDragHandle.isPointInside = [&](const Vec2F& point) {
			auto r = mSelectionFrame->GetRect();
			return RectF(r.right - 5, r.bottom, r.right + 5, r.top).IsInside(point);
		};

		mRightFrameDragHandle.checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(point.x, mSelectionRect.Center().y);
		};

		mRightFrameDragHandle.onChangedPos = [&](const Vec2F& point) {
			float scale = (point.x - mSelectionRect.left) / (mSelectionRect.right - mSelectionRect.left);

			for (auto animatedValueDef : mAnimation->GetAnimationsValues())
				animatedValueDef.animatedValue->BeginKeysBatchChange();

			for (auto handle : GetSelectedHandles()) {
				handle->SetPosition(Vec2F((handle->GetPosition().x - mSelectionRect.left)*scale + mSelectionRect.left, handle->GetPosition().y));
				handle->onChangedPos(handle->GetPosition());
			}

			for (auto animatedValueDef : mAnimation->GetAnimationsValues())
				animatedValueDef.animatedValue->CompleteKeysBatchingChange();

			UpdateSelectionFrame();
		};

		mRightFrameDragHandle.cursorType = CursorType::SizeWE;
	}

	void KeyHandlesSheet::UpdateSelectionFrame()
	{
		if (mIsFrameSelecting)
			return;

		if (mSelectedHandles.Count() > 1) {
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

			mCenterFrameDragHandle.position = mSelectionRect.Center();
			mLeftFrameDragHandle.position = Vec2F(mSelectionRect.left, mSelectionRect.Center().y);
			mRightFrameDragHandle.position = Vec2F(mSelectionRect.right, mSelectionRect.Center().y);
		}
		else mSelectionFrame->enabled = false;
	}

	void KeyHandlesSheet::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (!o2Input.IsKeyDown(VK_CONTROL)) 
			DeselectAll();

		mBeginSelectHandles = mSelectedHandles;

		mBeginSelectPoint.x = mTimeline->WorldToLocal(cursor.position.x);
		mBeginSelectPoint.y = mTree->GetLineNumber(cursor.position.y);
	}

	void KeyHandlesSheet::OnCursorReleased(const Input::Cursor& cursor)
	{
		mIsFrameSelecting = false;
		UpdateSelectionFrame();
	}

	void KeyHandlesSheet::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		mIsFrameSelecting = false;
		UpdateSelectionFrame();
	}

	void KeyHandlesSheet::OnCursorPressedOutside(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorReleasedOutside(const Input::Cursor& cursor)
	{

	}

	void KeyHandlesSheet::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (cursor.isPressed) {
			if (!mIsFrameSelecting) 
			{
				if (cursor.delta != Vec2F())
					mIsFrameSelecting = true;
			}
			

			if (mIsFrameSelecting)
			{
				mSelectionFrame->enabled = true;

				Vec2F current(mTimeline->WorldToLocal(cursor.position.x), mTree->GetLineNumber(cursor.position.y));
				mSelectionRect.left = Math::Min(mBeginSelectPoint.x, current.x);
				mSelectionRect.right = Math::Max(mBeginSelectPoint.x, current.x);
				mSelectionRect.top = Math::Floor(Math::Min(mBeginSelectPoint.y, current.y));
				mSelectionRect.bottom = Math::Ceil(Math::Max(mBeginSelectPoint.y, current.y));

				DeselectAll();

				for (auto handle : mBeginSelectHandles)
					SelectHandle(handle);

				for (auto handle : mHandles) {
					Vec2F handlePos(handle->GetPosition().x, mTree->GetLineNumber(handle->GetScreenPosition().y));
					if (handlePos.x > mSelectionRect.left && handlePos.x < mSelectionRect.right && handlePos.y > mSelectionRect.top && handlePos.y < mSelectionRect.bottom + 0.5f) {
						SelectHandle(handle);
					}
				}
			}
		}
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
