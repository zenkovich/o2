#include "stdafx.h"
#include "KeyHandlesSheet.h"

#include "Animation/AnimatedValue.h"
#include "Animation/Animation.h"
#include "AnimationWindow/AnimationWindow.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/ContextMenu.h"
#include "Timeline.h"
#include "TrackControls/ITrackControl.h"
#include "Tree.h"
#include "Utils/System/Clipboard.h"
#include "AnimationKeysActions.h"

namespace Editor
{

	KeyHandlesSheet::KeyHandlesSheet() :
		Widget()
	{
		mSelectionFrame = mnew Sprite("ui/UI4_keys_select.png");
		mSelectionFrame->enabled = false;

		InitializeHandles();
		InitializeContextMenu();
	}

	KeyHandlesSheet::KeyHandlesSheet(const KeyHandlesSheet& other) :
		Widget(other), mSelectionFrame(other.mSelectionFrame->CloneAs<Sprite>())
	{
		InitializeHandles();
		InitializeContextMenu();
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
			mSelectionFrame->SetRect(RectF(mAnimationWindow->mTimeline->LocalToWorld(mSelectionRect.left) + offsets.left,
										   mAnimationWindow->mTree->GetLineWorldPosition(mSelectionRect.top) + offsets.top,
										   mAnimationWindow->mTimeline->LocalToWorld(mSelectionRect.right) + offsets.right,
										   mAnimationWindow->mTree->GetLineWorldPosition(mSelectionRect.bottom) + offsets.bottom));
			mSelectionFrame->Draw();

			mCenterFrameDragHandle.Draw();
			mLeftFrameDragHandle.Draw();
			mRightFrameDragHandle.Draw();
		}

		o2Render.DisableScissorTest();

		DrawDebugFrame();

		if (mNeedUpdateSelectionFrame)
			UpdateSelectionFrame();
	}

	void KeyHandlesSheet::UpdateInputDrawOrder()
	{
		CursorAreaEventsListener::OnDrawn();

		for (auto handle : mHandles)
		{
			if (handle->IsEnabled())
				handle->CursorAreaEventsListener::OnDrawn();
		}

		if (mSelectionFrame->enabled) {
			mCenterFrameDragHandle.CursorAreaEventsListener::OnDrawn();

			if (!Math::Equals(mSelectionRect.left, mSelectionRect.right))
			{
				mLeftFrameDragHandle.CursorAreaEventsListener::OnDrawn();
				mRightFrameDragHandle.CursorAreaEventsListener::OnDrawn();
			}
		}
	}

	void KeyHandlesSheet::SetAnimation(Animation* animation)
	{
	}

	bool KeyHandlesSheet::IsUnderPoint(const Vec2F& point)
	{
		return Widget::IsUnderPoint(point);
	}

	void KeyHandlesSheet::RegTrackControl(ITrackControl* trackControl, const std::string& path)
	{
		mTrackControls.Add(trackControl);
		mTrackControlsMap.Add({ path, trackControl });
	}

	void KeyHandlesSheet::UnregTrackControl(ITrackControl* trackControl)
	{
		mTrackControls.Remove(trackControl);		
		mTrackControlsMap.RemoveAll([=](auto& p) { return p.second == trackControl; });
	}

	void KeyHandlesSheet::UnregAllTrackControls()
	{
		mTrackControls.Clear();
		mTrackControlsMap.Clear();
	}

	void KeyHandlesSheet::AddHandle(DragHandle* handle)
	{
		if (auto animHandle = dynamic_cast<AnimationKeyDragHandle*>(handle))
		{
			if (!mHandlesGroups.ContainsKey(animHandle->animatedValue))
				mHandlesGroups.Add(animHandle->animatedValue, {});

			mHandlesGroups[animHandle->animatedValue].Add(animHandle);
		}

		SelectableDragHandlesGroup::AddHandle(handle);
	}

	void KeyHandlesSheet::RemoveHandle(DragHandle* handle)
	{
		if (auto animHandle = dynamic_cast<AnimationKeyDragHandle*>(handle))
			mHandlesGroups[animHandle->animatedValue].Remove(animHandle);

		SelectableDragHandlesGroup::RemoveHandle(handle);
	}

	void KeyHandlesSheet::SetSelectedKeys(const Map<String, Vector<UInt64>>& keys)
	{
		DeselectAll();

		for (auto& handlesGroup : mHandlesGroups)
		{
			for (auto& handle : handlesGroup.second)
			{
				if (keys.ContainsKey(handle->animatedValuePath) && keys.Get(handle->animatedValuePath).Contains(handle->keyUid))
					SelectHandle(handle);
			}
		}
	}

	Map<String, Vector<UInt64>> KeyHandlesSheet::GetSelectedKeys() const
	{
		Map<String, Vector<UInt64>> res;
		for (auto& handlesGroup : mHandlesGroups)
		{
			for (auto& handle : handlesGroup.second)
			{
				if (handle->selected)
					res[handle->animatedValuePath].Add(handle->keyUid);
			}
		}

		return res;
	}

	ContextMenu* KeyHandlesSheet::GetContextMenu() const
	{
		return mContextMenu;
	}

	void KeyHandlesSheet::OnSelectionChanged()
	{
		mNeedUpdateSelectionFrame = true;
	}

	void KeyHandlesSheet::OnHandleCursorPressed(DragHandle* handle, const Input::Cursor& cursor)
	{
		for (auto trackControl : mTrackControls)
			trackControl->BeginKeysDrag();

		if (!GetSelectedHandles().Contains(handle) && handle != &mCenterFrameDragHandle && 
			handle != &mLeftFrameDragHandle && handle != &mRightFrameDragHandle)
		{
			if (!o2Input.IsKeyDown(VK_CONTROL))
				DeselectAll();

			SelectHandle(handle);
		}

		for (auto handle : GetSelectedHandles())
			handle->BeginDrag(cursor.position);

		mHandleHasMoved = false;
	}

	void KeyHandlesSheet::OnHandleCursorReleased(DragHandle* handle, const Input::Cursor& cursor)
	{
		SelectableDragHandlesGroup::OnHandleCursorReleased(handle, cursor);

		for (auto trackControl : mTrackControls)
			trackControl->EndKeysDrag();

		if (mSelectedHandles.Count() == 1 && !mHandleHasMoved)
			mAnimationWindow->mTimeline->SetTimeCursor(mSelectedHandles[0]->GetPosition().x);
	}

	void KeyHandlesSheet::OnHandleMoved(DragHandle* handle, const Vec2F& cursorPos)
	{
		for (auto animatedValueDef : mAnimationWindow->mAnimation->GetAnimationsValues())
			animatedValueDef.animatedValue->BeginKeysBatchChange();

		for (auto& kv : mHandlesGroups)
		{
			for (auto handle : kv.second)
			{
				if (!mSelectedHandles.Contains(handle))
					continue;

				if (handle->isMapping && kv.second.ContainsPred([=](auto x) { return !x->isMapping && x->id == handle->id; }))
					continue;

				handle->SetDragPosition(handle->ScreenToLocal(cursorPos) + handle->GetDraggingOffset());
				handle->onChangedPos(handle->GetPosition());
			}
		}

		for (auto animatedValueDef : mAnimationWindow->mAnimation->GetAnimationsValues())
			animatedValueDef.animatedValue->CompleteKeysBatchingChange();
		
		mHandleHasMoved = true;
		mNeedUpdateSelectionFrame = true;
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
			return Vec2F(mAnimationWindow->mTimeline->LocalToWorld(point.x), mAnimationWindow->mTree->GetLineWorldPosition(point.y));
		};

		mCenterFrameDragHandle.screenToLocalTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mAnimationWindow->mTimeline->WorldToLocal(point.x), mAnimationWindow->mTree->GetLineNumber(point.y));
		};

		mCenterFrameDragHandle.isPointInside = [&](const Vec2F& point) {
			auto local = Vec2F(mAnimationWindow->mTimeline->WorldToLocal(point.x), mAnimationWindow->mTree->GetLineNumber(point.y));
			return local.x > mSelectionRect.left && local.x < mSelectionRect.right && local.y > mSelectionRect.top && local.y < mSelectionRect.bottom;
		};

		mCenterFrameDragHandle.checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(point.x, mSelectionRect.Center().y);
		};

		mCenterFrameDragHandle.onPressed = [&]() {
			OnHandleCursorPressed(&mCenterFrameDragHandle, *o2Input.GetCursor());
		};

		mCenterFrameDragHandle.onReleased = [&]() {
			OnHandleCursorReleased(&mCenterFrameDragHandle, *o2Input.GetCursor());
		};

		mCenterFrameDragHandle.onChangedPos = [&](const Vec2F& point) {
			OnHandleMoved(&mCenterFrameDragHandle, o2Input.GetCursorPos());
		};

		mCenterFrameDragHandle.onRightButtonReleased = [&](auto& x) { mContextMenu->Show(); };
		mCenterFrameDragHandle.cursorType = CursorType::SizeWE;
	}

	void KeyHandlesSheet::InitializeLeftHandle()
	{
		mLeftFrameDragHandle.localToScreenTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mAnimationWindow->mTimeline->LocalToWorld(point.x), mAnimationWindow->mTree->GetLineWorldPosition(point.y));
		};

		mLeftFrameDragHandle.screenToLocalTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mAnimationWindow->mTimeline->WorldToLocal(point.x), mAnimationWindow->mTree->GetLineNumber(point.y));
		};

		mLeftFrameDragHandle.isPointInside = [&](const Vec2F& point) {
			auto r = mSelectionFrame->GetRect();
			return RectF(r.left - 5, r.bottom, r.left + 5, r.top).IsInside(point);
		};

		mLeftFrameDragHandle.checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(point.x, mSelectionRect.Center().y);
		};

		mLeftFrameDragHandle.onPressed = [&]() {
			OnHandleCursorPressed(&mCenterFrameDragHandle, *o2Input.GetCursor());
		};

		mLeftFrameDragHandle.onReleased = [&]() {
			OnHandleCursorReleased(&mCenterFrameDragHandle, *o2Input.GetCursor());
		};

		mLeftFrameDragHandle.onChangedPos = [&](const Vec2F& point) {
			if (Math::Equals(mSelectionRect.Width(), 0.0f))
				return;

			float scale = (point.x - mSelectionRect.right) / (mSelectionRect.left - mSelectionRect.right);

			for (auto animatedValueDef : mAnimationWindow->mAnimation->GetAnimationsValues())
				animatedValueDef.animatedValue->BeginKeysBatchChange();

			for (auto handle : GetSelectedHandles()) {
				handle->SetPosition(Vec2F((handle->GetPosition().x - mSelectionRect.right)*scale + mSelectionRect.right, handle->GetPosition().y));
				handle->onChangedPos(handle->GetPosition());
			}

			for (auto animatedValueDef : mAnimationWindow->mAnimation->GetAnimationsValues())
				animatedValueDef.animatedValue->CompleteKeysBatchingChange();

			mNeedUpdateSelectionFrame = true;
		};

		mLeftFrameDragHandle.onRightButtonReleased = [&](auto& x) { mContextMenu->Show(); };
		mLeftFrameDragHandle.cursorType = CursorType::SizeWE;
	}

	void KeyHandlesSheet::InitializeRightHandle()
	{
		mRightFrameDragHandle.localToScreenTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mAnimationWindow->mTimeline->LocalToWorld(point.x), mAnimationWindow->mTree->GetLineWorldPosition(point.y));
		};

		mRightFrameDragHandle.screenToLocalTransformFunc = [&](const Vec2F& point) {
			return Vec2F(mAnimationWindow->mTimeline->WorldToLocal(point.x), mAnimationWindow->mTree->GetLineNumber(point.y));
		};

		mRightFrameDragHandle.isPointInside = [&](const Vec2F& point) {
			auto r = mSelectionFrame->GetRect();
			return RectF(r.right - 5, r.bottom, r.right + 5, r.top).IsInside(point);
		};

		mRightFrameDragHandle.checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(point.x, mSelectionRect.Center().y);
		};

		mRightFrameDragHandle.onPressed = [&]() {
			OnHandleCursorPressed(&mCenterFrameDragHandle, *o2Input.GetCursor());
		};

		mRightFrameDragHandle.onReleased = [&]() {
			OnHandleCursorReleased(&mCenterFrameDragHandle, *o2Input.GetCursor());
		};

		mRightFrameDragHandle.onChangedPos = [&](const Vec2F& point) {
			if (Math::Equals(mSelectionRect.Width(), 0.0f))
				return;

			float scale = (point.x - mSelectionRect.left) / (mSelectionRect.right - mSelectionRect.left);

			for (auto animatedValueDef : mAnimationWindow->mAnimation->GetAnimationsValues())
				animatedValueDef.animatedValue->BeginKeysBatchChange();

			for (auto handle : GetSelectedHandles()) {
				handle->SetPosition(Vec2F((handle->GetPosition().x - mSelectionRect.left)*scale + mSelectionRect.left, handle->GetPosition().y));
				handle->onChangedPos(handle->GetPosition());
			}

			for (auto animatedValueDef : mAnimationWindow->mAnimation->GetAnimationsValues())
				animatedValueDef.animatedValue->CompleteKeysBatchingChange();

			mNeedUpdateSelectionFrame = true;
		};

		mRightFrameDragHandle.onRightButtonReleased = [&](auto& x) { mContextMenu->Show(); };
		mRightFrameDragHandle.cursorType = CursorType::SizeWE;
	}

	void KeyHandlesSheet::InitializeContextMenu()
	{
		mContextMenu = o2UI.CreateWidget<ContextMenu>();

		mContextMenu->AddItem("Copy", [&]() { CopyKeys(); }, ImageAssetRef(), ShortcutKeys('C', true));
		mContextMenu->AddItem("Cut", [&]() { CopyKeys(); DeleteKeys(GetSelectedKeys()); }, ImageAssetRef(), ShortcutKeys('X', true));
		mContextMenu->AddItem("Paste", [&]() { PasteKeys(); }, ImageAssetRef(), ShortcutKeys('V', true));
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Delete", [&]() { DeleteKeys(GetSelectedKeys()); }, ImageAssetRef(), ShortcutKeys(VK_DELETE));
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Select all", [&]() { SelectAll(); }, ImageAssetRef(), ShortcutKeys('A', true));
		mContextMenu->AddItem("Deselect all", [&]() { DeselectAll(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Undo", [&]() { mAnimationWindow->mActionsList.UndoAction(); }, ImageAssetRef(), ShortcutKeys('Z', true));
		mContextMenu->AddItem("Redo", [&]() { mAnimationWindow->mActionsList.RedoAction(); }, ImageAssetRef(), ShortcutKeys('Y', true));

		AddChild(mContextMenu);

		mIsFocusable = true;
	}

	void KeyHandlesSheet::UpdateSelectionFrame()
	{
		mNeedUpdateSelectionFrame = false;

		if (mIsFrameSelecting)
			return;

		if (mSelectedHandles.Count() > 0) {
			mSelectionFrame->enabled = true;

			mSelectionRect.left = mSelectedHandles.First()->GetPosition().x;
			mSelectionRect.bottom = mAnimationWindow->mTree->GetLineNumber(mSelectedHandles.First()->GetScreenPosition().y);
			mSelectionRect.right = mSelectionRect.left;
			mSelectionRect.top = mSelectionRect.bottom;

			for (auto handle : mSelectedHandles) {
				float localPos = handle->GetPosition().x;
				float lineNumber = mAnimationWindow->mTree->GetLineNumber(handle->GetScreenPosition().y);

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

	void KeyHandlesSheet::SerializeSelectedKeys(DataNode& data, Map<String, Vector<UInt64>>& keys, float relativeTime)
	{
		Map<String, Pair<DataNode*, Vector<UInt64>>> serializedKeysUids;

		for (auto handle : mSelectedHandles)
		{
			if (auto keyHandle = dynamic_cast<AnimationKeyDragHandle*>(handle))
			{
				if (!serializedKeysUids.ContainsKey(keyHandle->animatedValuePath))
				{
					auto pathNode = data.AddNode("KeysGroup");
					*pathNode->AddNode("Path") = keyHandle->animatedValuePath;
					serializedKeysUids.Add(keyHandle->animatedValuePath, { pathNode->AddNode("Keys"), {} });
				}

				if (serializedKeysUids[keyHandle->animatedValuePath].second.Contains(keyHandle->keyUid))
					continue;

				keys[keyHandle->animatedValuePath].Add(keyHandle->keyUid);

				auto& pair = serializedKeysUids[keyHandle->animatedValuePath];

				auto node = pair.first->AddNode("key");
				keyHandle->trackControl->SerializeKey(keyHandle->keyUid, *node, relativeTime);

				serializedKeysUids[keyHandle->animatedValuePath].second.Add(keyHandle->keyUid);
			}
		}
	}

	void KeyHandlesSheet::DeserializeKeys(const DataNode& data, Map<String, Vector<UInt64>>& keys, float relativeTime)
	{
		for (auto animatedValueDef : mAnimationWindow->mAnimation->GetAnimationsValues())
			animatedValueDef.animatedValue->BeginKeysBatchChange();

		if (data.GetChildNodes().Count() == 1 && mAnimationWindow->mTree->GetSelectedObjects().Count() == 1)
		{
			auto dataNode = (AnimationTree::AnimationValueNode*)mAnimationWindow->mTree->GetSelectedObjects()[0];
			for (auto keyNode : *data.GetChildNodes()[0]->GetNode("Keys"))
			{
				UInt64 uid = dataNode->trackControl->DeserializeKey(*keyNode, relativeTime);
				if (uid != 0)
					keys[dataNode->path].Add(uid);
			}
		}
		else
		{
			for (auto pathNode : data)
			{
				String path = *pathNode->GetNode("Path");
				for (auto& kv : mTrackControlsMap)
				{
					if (kv.first != path)
						continue;

					for (auto keyNode : *pathNode->GetNode("Keys"))
					{
						UInt64 uid = kv.second->DeserializeKey(*keyNode, relativeTime);
						if (uid != 0)
							keys[path].Add(uid);
					}
				}
			}
		}

		for (auto animatedValueDef : mAnimationWindow->mAnimation->GetAnimationsValues())
			animatedValueDef.animatedValue->CompleteKeysBatchingChange();
	}

	void KeyHandlesSheet::CopyKeys()
	{
		DataNode data;
		float relativeTime = mAnimationWindow->mTimeline->WorldToLocal(o2Input.GetCursorPos().x);
		Map<String, Vector<UInt64>> keys;

		SerializeSelectedKeys(data, keys, relativeTime);

		Clipboard::SetText(data.SaveAsWString());
	}

	void KeyHandlesSheet::PasteKeys()
	{
		DeselectAll();
		Map<String, Vector<UInt64>> newKeys;

		DataNode data;
		data.LoadFromData(Clipboard::GetText());

		DeserializeKeys(data, newKeys, mAnimationWindow->mTimeline->WorldToLocal(o2Input.GetCursorPos().x));
		SetSelectedKeys(newKeys);

		mAnimationWindow->mActionsList.DoneAction(mnew AnimationAddKeysAction(data, this));
	}

	void KeyHandlesSheet::DeleteKeys(const Map<String, Vector<UInt64>>& keys)
	{
		for (auto animatedValueDef : mAnimationWindow->mAnimation->GetAnimationsValues())
			animatedValueDef.animatedValue->BeginKeysBatchChange();
		
		for (auto& handlesGroup : mHandlesGroups)
		{
			for (auto& handle : handlesGroup.second)
			{
				if (keys.ContainsKey(handle->animatedValuePath) && keys.Get(handle->animatedValuePath).Contains(handle->keyUid))
				{
					handle->Deselect();
					handle->trackControl->DeleteKey(handle->keyUid);
				}
			}
		}

		for (auto animatedValueDef : mAnimationWindow->mAnimation->GetAnimationsValues())
			animatedValueDef.animatedValue->CompleteKeysBatchingChange();
	}

	void KeyHandlesSheet::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (!o2Input.IsKeyDown(VK_CONTROL)) 
			DeselectAll();

		mBeginSelectHandles = mSelectedHandles;

		mBeginSelectPoint.x = mAnimationWindow->mTimeline->WorldToLocal(cursor.position.x);
		mBeginSelectPoint.y = mAnimationWindow->mTree->GetLineNumber(cursor.position.y);

		Focus();
		mContextMenu->SetItemsMaxPriority();
	}

	void KeyHandlesSheet::OnCursorReleased(const Input::Cursor& cursor)
	{
		mIsFrameSelecting = false;
		mNeedUpdateSelectionFrame = true;
	}

	void KeyHandlesSheet::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		mIsFrameSelecting = false;
		mNeedUpdateSelectionFrame = true;
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

				Vec2F current(mAnimationWindow->mTimeline->WorldToLocal(cursor.position.x), mAnimationWindow->mTree->GetLineNumber(cursor.position.y));
				mSelectionRect.left = Math::Min(mBeginSelectPoint.x, current.x);
				mSelectionRect.right = Math::Max(mBeginSelectPoint.x, current.x);
				mSelectionRect.top = Math::Floor(Math::Min(mBeginSelectPoint.y, current.y));
				mSelectionRect.bottom = Math::Ceil(Math::Max(mBeginSelectPoint.y, current.y));

				DeselectAll();

				for (auto handle : mBeginSelectHandles)
					SelectHandle(handle);

				for (auto handle : mHandles) {
					Vec2F handlePos(handle->GetPosition().x, mAnimationWindow->mTree->GetLineNumber(handle->GetScreenPosition().y));
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
		auto treeNode = mAnimationWindow->mTree->GetTreeNodeUnderPoint(cursor.position);
		if (!treeNode)
			return;

		auto animTreeNode = dynamic_cast<AnimationTreeNode*>(treeNode);
		animTreeNode->OnDoubleClicked(cursor);
	}

	void KeyHandlesSheet::OnCursorRightMousePressed(const Input::Cursor& cursor)
	{
		Focus();
		mContextMenu->SetItemsMaxPriority();
		mContextMenuPressPoint = cursor.position;
	}

	void KeyHandlesSheet::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
	{
	}

	void KeyHandlesSheet::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		const float threshold = 5.0f;
		if ((cursor.position - mContextMenuPressPoint).Length() < threshold) 
			mContextMenu->Show();
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
