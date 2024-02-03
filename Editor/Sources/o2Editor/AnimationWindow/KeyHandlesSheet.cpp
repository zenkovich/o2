#include "o2Editor/stdafx.h"
#include "KeyHandlesSheet.h"

#include "o2/Animation/Tracks/AnimationTrack.h"
#include "o2/Animation/AnimationClip.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Utils/System/Clipboard.h"
#include "o2Editor/AnimationWindow/AnimationKeysActions.h"
#include "o2Editor/AnimationWindow/AnimationWindow.h"
#include "o2Editor/AnimationWindow/Timeline.h"
#include "o2Editor/AnimationWindow/TrackControls/ITrackControl.h"
#include "o2Editor/AnimationWindow/Tree.h"

namespace Editor
{

	KeyHandlesSheet::KeyHandlesSheet() :
		Widget()
	{
		mSelectionFrame = mmake<Sprite>("ui/UI4_keys_select.png");
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
		// delete mSelectionFrame; // Don't delete, using smart pointer
	}

	KeyHandlesSheet& KeyHandlesSheet::operator=(const KeyHandlesSheet& other)
	{
		// delete mSelectionFrame; // Don't delete, using smart pointer

		Widget::operator=(other);

		mSelectionFrame = other.mSelectionFrame->CloneAs<Sprite>();

		return *this;
	}

	void KeyHandlesSheet::Update(float dt)
	{
		Widget::Update(dt);
	}

	// #undef DrawText // Commenting this out, as it is not being used

	void KeyHandlesSheet::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		Widget::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

		o2Render.EnableScissorTest(layout->GetWorldRect());

		if (mSelectionFrame->enabled)
		{
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

		if (mSelectionFrame->enabled)
		{
			mCenterFrameDragHandle.CursorAreaEventsListener::OnDrawn();

			if (!Math::Equals(mSelectionRect.left, mSelectionRect.right))
			{
				mLeftFrameDragHandle.CursorAreaEventsListener::OnDrawn();
				mRightFrameDragHandle.CursorAreaEventsListener::OnDrawn();
			}
		}
	}

	void KeyHandlesSheet::SetAnimation(Ref<AnimationClip> animation)
	{
	}

	bool KeyHandlesSheet::IsUnderPoint(const Vec2F& point)
	{
		return Widget::IsUnderPoint(point);
	}

	void KeyHandlesSheet::RegTrackControl(const Ref<ITrackControl>& trackControl, const std::string& path)
	{
		mTrackControls.Add(trackControl);
		mTrackControlsMap.Add({ path, trackControl });
	}

	void KeyHandlesSheet::UnregTrackControl(const Ref<ITrackControl>& trackControl)
	{
		mTrackControls.Remove(trackControl);
		mTrackControlsMap.RemoveAll([=](auto& p) { return p.second == trackControl; });
	}

	void KeyHandlesSheet::UnregAllTrackControls()
	{
		mTrackControls.Clear();
		mTrackControlsMap.Clear();
		mHandles.Clear();
	}

	void KeyHandlesSheet::AddHandle(const Ref<DragHandle>& handle)
	{
		if (auto animHandle = dynamic_cast<AnimationKeyDragHandle*>(handle.get()))
		{
			if (!mHandlesGroups.ContainsKey(animHandle->track))
				mHandlesGroups.Add(animHandle->track, {});

			mHandlesGroups[animHandle->track].Add(animHandle);
		}

		SelectableDragHandlesGroup::AddHandle(handle);
	}

	void KeyHandlesSheet::RemoveHandle(DragHandle* handle)
	{
		if (auto animHandle = dynamic_cast<AnimationKeyDragHandle*>(handle))
			mHandlesGroups[animHandle->track].Remove(animHandle);

		SelectableDragHandlesGroup::RemoveHandle(handle);
	}

	void KeyHandlesSheet::SetSelectedKeys(const Map<String, Vector<UInt64>>& keys)
	{
		DeselectAll();

		for (auto& handlesGroup : mHandlesGroups)
		{
			for (auto& handle : handlesGroup.second)
			{
				if (keys.ContainsKey(handle->trackPath) && keys.Get(handle->trackPath).Contains(handle->keyUid))
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
					res[handle->trackPath].Add(handle->keyUid);
			}
		}

		return res;
	}

	Ref<ContextMenu> KeyHandlesSheet::GetContextMenu() const
	{
		return mContextMenu;
	}

	String KeyHandlesSheet::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void KeyHandlesSheet::OnSelectionChanged()
	{
		mNevoid KeyHandlesSheet::OnHandleCursorPressed(const Ref<DragHandle>& handle, const Input::Cursor& cursor)
{
    mBeforeChangeKeysData.Clear();
    SerializeKeys(mBeforeChangeKeysData, GetSelectedKeys(), 0);

    for (const auto& trackControl : mTrackControls)
        trackControl->BeginKeysDrag();

    if (!GetSelectedHandles().Contains(handle) && handle != &mCenterFrameDragHandle &&
        handle != &mLeftFrameDragHandle && handle != &mRightFrameDragHandle)
    {
        if (!o2Input.IsKeyDown(VK_CONTROL))
            DeselectAll();

        SelectHandle(handle);
    }

    for (const auto& handle : GetSelectedHandles())
        handle->BeginDrag(cursor.position);

    mHandleHasMoved = false;
}

void KeyHandlesSheet::OnHandleCursorReleased(const Ref<DragHandle>& handle, const Input::Cursor& cursor)
{
    SelectableDragHandlesGroup::OnHandleCursorReleased(handle, cursor);

    for (const auto& trackControl : mTrackControls)
        trackControl->EndKeysDrag();

    if (mSelectedHandles.Count() == 1 && !mHandleHasMoved)
        mAnimationWindow->mTimeline->SetTimeCursor(mSelectedHandles[0]->GetPosition().x);

    DataDocument data;
    const auto selectedKeys = GetSelectedKeys();
    SerializeKeys(data, selectedKeys, 0);
    mAnimationWindow->mActionsList.DoneAction(Make<AnimationKeysChangeAction>(selectedKeys, mBeforeChangeKeysData, data, this));
}

void KeyHandlesSheet::OnHandleMoved(const Ref<DragHandle>& handle, const Vec2F& cursorPos)
{
    for (const auto& track : mAnimationWindow->mAnimation->GetTracks())
        track->BeginKeysBatchChange();

    for (auto& kv : mHandlesGroups)
    {
        for (const auto& handle : kv.second)
        {
            if (!mSelectedHandles.Contains(handle))
                continue;

            if (handle->isMapping && kv.second.Contains([&](const auto& x) { return !x->isMapping && x->id == handle->id; }))
                continue;

            handle->SetDragPosition(handle->ScreenToLocal(cursorPos) + handle->GetDraggingOffset());
            handle->onChangedPos(handle->GetPosition());
        }
    }

    for (const auto& track : mAnimationWindow->mAnimation->GetTracks())
        track->CompleteKeysBatchingChange();

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
        const auto local = Vec2F(mAnimationWindow->mTimeline->WorldToLocal(point.x), mAnimationWindow->mTree->GetLineNumber(point.y));
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

    mCenterFrameDragHandle.onRightButtonReleased = [&](const auto& x) { mContextMenu->Show(); };
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
        const auto r = mSelectionFrame->GetRect();
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

        const float scale = (point.x - mSelectionRect.right) / (mSelectionRect.left - mSelectionRect.right);

        for (const auto& track : mAnimationWindow->mAnimation->GetT#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename BaseType, typename DerivedType>
Ref<BaseType> DynamicCast(const Ref<DerivedType>& derived)
{
    return std::dynamic_pointer_cast<BaseType>(derived);
}

template<typename T>
class KeyHandlesSheet
{
public:
    void InitializeLeftHandle()
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
            OnHandleCursorPressed(mCenterFrameDragHandle, *o2Input.GetCursor());
        };

        mLeftFrameDragHandle.onReleased = [&]() {
            OnHandleCursorReleased(mCenterFrameDragHandle, *o2Input.GetCursor());
        };

        mLeftFrameDragHandle.onChangedPos = [&](const Vec2F& point) {
            if (Math::Equals(mSelectionRect.Width(), 0.0f))
                return;

            float scale = (point.x - mSelectionRect.right) / (mSelectionRect.left - mSelectionRect.right);

            for (auto track : mAnimationWindow->mAnimation->GetTracks())
                track->BeginKeysBatchChange();

            for (auto handle : GetSelectedHandles()) {
                handle->SetPosition(Vec2F((handle->GetPosition().x - mSelectionRect.right)*scale + mSelectionRect.right, handle->GetPosition().y));
                handle->onChangedPos(handle->GetPosition());
            }

            for (auto track : mAnimationWindow->mAnimation->GetTracks())
                track->CompleteKeysBatchingChange();

            mNeedUpdateSelectionFrame = true;
        };

        mLeftFrameDragHandle.onRightButtonReleased = [&](auto& x) { mContextMenu->Show(); };
        mLeftFrameDragHandle.cursorType = CursorType::SizeWE;
    }

    void InitializeRightHandle()
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
            OnHandleCursorPressed(mCenterFrameDragHandle, *o2Input.GetCursor());
        };

        mRightFrameDragHandle.onReleased = [&]() {
            OnHandleCursorReleased(mCenterFrameDragHandle, *o2Input.GetCursor());
        };

        mRightFrameDragHandle.onChangedPos = [&](const Vec2F& point) {
            if (Math::Equals(mSelectionRect.Width(), 0.0f))
                return;

            float scale = (point.x - mSelectionRect.left) / (mSelectionRect.right - mSelectionRect.left);

            for (auto track : mAnimationWindow->mAnimation->GetTracks())
                track->BeginKeysBatchChange();

            for (auto handle : GetSelectedHandles()) {
                handle->SetPosition(Vec2F((handle->GetPosition().x - mSelectionRect.left)*scale + mSelectionRect.left, handle->GetPosition().y));
                handle->onChangedPos(handle->GetPosition());
            }

            for (auto track : mAnimationWindow->mAnimation->GetTracks())
                track->CompleteKeysBatchingChange();

            mNeedUpdateSelectionFrame = true;
        };

        mRightFrameDragHandle.onRightButtonReleased = [&](auto& x) { mContextMenu->Show(); };
        mRightFrameDragHandle.cursorType = CursorType::SizeWE;
    }

    void InitializeContextMenu()
    {
        mContextMenu = mmake<ContextMenu>();

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

    void UpdateSelectionFrame()
    {
        mNeedUpdateSelectionFrame = false;

        if (mIsFrameSelecting)
            return;

        if (mSelectedHandles.size() > 0) {
            mSelectionFrame->enabled = true;

            mSelectionRect.left = mSelectedHandles[0]->GetPosition().x;
            mSelectionRect.bottom = mAnimationWindow->mTree->GetLineNumber(mSelectedHandles[0]->GetScreenPosition().y);
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
        else
            mSelectionFrame->enabled = false;
    }

    void SerializeKeys(DataValue& data, const Map<String, Vector<UInt64>>& keys, float relativeTime)
    {
        Map<String, Vector<Ref<UInt64>>> serializedKeys;

        for (auto& pair : keys) {
            Vector<Ref<UInt64>> serializedPair;
            for (auto& key : pair.second) {
                serializedPair.push_back(mmake<UInt64>(*key));
            }
            serializedKeys[pair.first] = std::move(serializedPair);
        }

        data["keys"] = std::move(serializedKeys);
        data["relativeTime"] = relativeTime;
    }

private:
    Ref<ContextMenu> mContextMenu;
    Vec2F mSelectionRect;
    std::vector<Ref<Int64>> mSelectedHandles;
    bool mNeedUpdateSelectionFrame;
    bool mIsFrameSelecting;
    // ...
};```
class RefBase
{
public:
    RefBase() : m_refCount(0) {}

    void AddRef()
    {
        ++m_refCount;
    }

    void Release()
    {
        if (--m_refCount == 0)
            delete this;
    }

private:
    int m_refCount;
};

template<typename T>
class Ref
{
public:
    Ref() : m_ptr(nullptr) {}

    Ref(T* ptr) : m_ptr(ptr)
    {
        if (m_ptr)
            m_ptr->AddRef();
    }

    Ref(const Ref& other) : m_ptr(other.m_ptr)
    {
        if (m_ptr)
            m_ptr->AddRef();
    }

    Ref(Ref&& other) : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }

    ~Ref()
    {
        if (m_ptr)
            m_ptr->Release();
    }

    Ref& operator=(T* ptr)
    {
        if (ptr != m_ptr)
        {
            if (m_ptr)
                m_ptr->Release();

            m_ptr = ptr;

            if (m_ptr)
                m_ptr->AddRef();
        }

        return *this;
    }

    Ref& operator=(const Ref& other)
    {
        return *this = other.m_ptr;
    }

    Ref& operator=(Ref&& other)
    {
        if (this != &other)
        {
            if (m_ptr)
                m_ptr->Release();

            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }

        return *this;
    }

    T* operator->() const
    {
        return m_ptr;
    }

    T* Get() const
    {
        return m_ptr;
    }

    operator bool() const
    {
        return m_ptr != nullptr;
    }

private:
    T* m_ptr;
};

template<typename T>
Ref<T> mmake()
{
    return Ref<T>(new T());
}

template<typename T>
class WeakRef
{
public:
    WeakRef() : m_ptr(nullptr), m_refCount(nullptr) {}

    WeakRef(const Ref<T>& ref) : m_ptr(ref.Get()), m_refCount(ref.m_refCount)
    {
        if (m_ptr)
            ++(*m_refCount);
    }

    WeakRef(const WeakRef& other) : m_ptr(other.m_ptr), m_refCount(other.m_refCount)
    {
        if (m_ptr)
            ++(*m_refCount);
    }

    WeakRef(WeakRef&& other) noexcept : m_ptr(other.m_ptr), m_refCount(other.m_refCount)
    {
        other.m_ptr = nullptr;
        other.m_refCount = nullptr;
    }

    ~WeakRef()
    {
        if (m_ptr && --(*m_refCount) == 0)
            delete m_refCount;
    }

    WeakRef& operator=(const Ref<T>& ref)
    {
        return *this = ref.Get();
    }

    WeakRef& operator=(const WeakRef& other)
    {
        return *this = other.m_ptr;
    }

    WeakRef& operator=(WeakRef&& other) noexcept
    {
        if (this != &other)
        {
            if (m_ptr && --(*m_refCount) == 0)
                delete m_refCount;

            m_ptr = other.m_ptr;
            m_refCount = other.m_refCount;

            other.m_ptr = nullptr;
            other.m_refCount = nullptr;
        }

        return *this;
    }

    Ref<T> Lock() const
    {
        return Ref<T>(m_ptr);
    }

private:
    T* m_ptr;
    int* m_refCount;
};
```
```
Ref<ITrackControl> fnd = mTrackControlsMap.FindOrDefault([=](const Pair<String, Ref<ITrackControl>>& p) { return p.first == kv.first; });
```
```
Ref<DataValue> trackData = mmake<DataValue>("KeysGroup");
trackData->AddMember("Path") = kv.first;
Ref<DataValue> keysData = trackData->AddMember("Keys");
```
```
Ref<AnimationTree::TrackNode> DataValue = (AnimationTree::TrackNode*)mAnimationWindow->mTree->GetSelectedObjects()[0];
```
```
class KeyHandlesSheet
{
    // ...

private:
    void SerializeKeys(DataValue& data, const Map<String, Vector<UInt64>>& keys, float relativeTime);
    void DeserializeKeys(const DataValue& data, Map<String, Vector<UInt64>>& keys, float relativeTime, bool generateNewUid = true);

    Map<String, Ref<ITrackControl>> mTrackControlsMap;
    Vector<Ref<KeyHandle>> mHandlesGroups;
    Set<Ref<KeyHandle>> mSelectedHandles;
    Set<Ref<KeyHandle>> mBeginSelectHandles;

    // ...
};

void KeyHandlesSheet::SerializeKeys(DataValue& data, const Map<String, Vector<UInt64>>& keys, float relativeTime)
{
    for (const auto& kv : keys)
    {
        const Ref<ITrackControl>& fnd = mTrackControlsMap.FindOrDefault([=](const Pair<String, Ref<ITrackControl>>& p) { return p.first == kv.first; });
        if (!fnd)
            continue;

        Ref<DataValue>& trackData = data.AddMember("KeysGroup");
        trackData->AddMember("Path") = kv.first;
        Ref<DataValue>& keysData = trackData->AddMember("Keys");

        for (const auto& handle : fnd->GetKeyHandles())
        {
            if (kv.second.Contains(handle->keyUid))
            {
                Ref<DataValue> node = keysData->AddMember("key");
                fnd->SerializeKey(handle->keyUid, node, relativeTime);
            }
        }
    }
}

void KeyHandlesSheet::DeserializeKeys(const DataValue& data, Map<String, Vector<UInt64>>& keys, float relativeTime, bool generateNewUid)
{
    for (const auto& track : mAnimationWindow->mAnimation->GetTracks())
        track->BeginKeysBatchChange();

    if (data.GetMembersCount() == 1 && mAnimationWindow->mTree->GetSelectedObjects().Count() == 1)
    {
        Ref<AnimationTree::TrackNode> DataValue = (AnimationTree::TrackNode*)mAnimationWindow->mTree->GetSelectedObjects()[0];
        for (const auto& keyNode : data[0].GetMember("Keys"))
        {
            UInt64 uid = DataValue->trackControl->DeserializeKey(keyNode, relativeTime);
            if (uid != 0)
                keys[DataValue->path].Add(uid);
        }
    }
    else
    {
        for (const auto& pathNode : data)
        {
            const String& path = pathNode.GetMember("Path");
            for (const auto& kv : mTrackControlsMap)
            {
                if (kv.first != path)
                    continue;

                for (const auto& keyNode : pathNode.GetMember("Keys"))
                {
                    UInt64 uid = kv.second->DeserializeKey(keyNode, relativeTime, generateNewUid);
                    if (uid != 0)
                        keys[path].Add(uid);
                }
            }
        }
    }

    for (const auto& track : mAnimationWindow->mAnimation->GetTracks())
        track->CompleteKeysBatchingChange();
}

void KeyHandlesSheet::CopyKeys()
{
    float relativeTime = mAnimationWindow->mTimeline->WorldToLocal(o2Input.GetCursorPos().x);
    Map<String, Vector<UInt64>> keys = GetSelectedKeys();

    Ref<DataValue> data = mmake<DataValue>();
    SerializeKeys(*data, keys, relativeTime);

    Clipboard::SetText(data->SaveAsString());
}

void KeyHandlesSheet::PasteKeys()
{
    DeselectAll();
    Map<String, Vector<UInt64>> keys;

    Ref<DataValue> data = mmake<DataValue>();
    data->LoadFromData(Clipboard::GetText());

    DeserializeKeys(*data, keys, mAnimationWindow->mTimeline->WorldToLocal(o2Input.GetCursorPos().x));
    SetSelectedKeys(keys);

    data->Clear();
    SerializeKeys(*data, keys, 0);
    mAnimationWindow->mActionsList.DoneAction(mmake<AnimationAddKeysAction>(keys, *data, this));
}

void KeyHandlesSheet::DeleteKeys(const Map<String, Vector<UInt64>>& keys, bool createAction)
{
    if (createAction)
    {
        Ref<DataValue> data = mmake<DataValue>();
        SerializeKeys(*data, keys, 0);
        mAnimationWindow->mActionsList.DoneAction(mmake<AnimationDeleteKeysAction>(keys, *data, this));
    }

    for (const auto& track : mAnimationWindow->mAnimation->GetTracks())
        track->BeginKeysBatchChange();

    for (const auto& handlesGroup : mHandlesGroups)
    {
        for (const auto& handle : handlesGroup.second)
        {
            if (keys.ContainsKey(handle->trackPath) && keys.Get(handle->trackPath).Contains(handle->keyUid))
            {
                handle->Deselect();
                handle->trackControl->DeleteKey(handle->keyUid);
            }
        }
    }

    for (const auto& track : mAnimationWindow->mAnimation->GetTracks())
        track->CompleteKeysBatchingChange();
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
```#include <memory>
#include "Ref.h"

using namespace std;

namespace Editor {
	class KeyHandlesSheet {
	public:
		using Ref = Editor::Ref;
		using WeakRef = Editor::WeakRef;

		KeyHandlesSheet() = default;
		~KeyHandlesSheet() = default;

		void Init(Ref	animationWindow) {
			mAnimationWindow = animationWindow;
		}

		void SetSelectionRect(const RectF& rect) {
			mSelectionRect = rect;
		}

		void Update() {
			if (!mAnimationWindow || !mAnimationWindow->mTree)
				return;

			for (const auto& handle : mHandles) {
				Vec2F handlePos(handle->GetPosition().x, mAnimationWindow->mTree->GetLineNumber(handle->GetScreenPosition().y));
				if (handlePos.x > mSelectionRect.left && handlePos.x < mSelectionRect.right && handlePos.y > mSelectionRect.top && handlePos.y < mSelectionRect.bottom + 0.5f) {
					SelectHandle(handle);
				}
			}
		}

		void KeyHandlesSheet::OnCursorMoved(const Input::Cursor& cursor) {
		}

		void OnCursorEnter(const Input::Cursor& cursor) {

		}

		void OnCursorExit(const Input::Cursor& cursor) {

		}

		void OnCursorDblClicked(const Input::Cursor& cursor) {
			auto treeNode = mAnimationWindow->mTree->GetTreeNodeUnderPoint(cursor.position);
			if (!treeNode)
				return;

			auto animTreeNode = DynamicCast<AnimationTreeNode>(treeNode);
			animTreeNode->OnDoubleClicked(cursor);
		}

		void OnCursorRightMousePressed(const Input::Cursor& cursor) {
			Focus();
			mContextMenu->SetItemsMaxPriority();
			mContextMenuPressPoint = cursor.position;
		}

		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) {

		}

		void OnCursorRightMouseReleased(const Input::Cursor& cursor) {
			const float threshold = 5.0f;
			if ((cursor.position - mContextMenuPressPoint).Length() < threshold)
				mContextMenu->Show();
		}

		void OnCursorMiddleMousePressed(const Input::Cursor& cursor) {

		}

		void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor) {

		}

		void OnCursorMiddleMouseReleased(const Input::Cursor& cursor) {

		}
	private:
		void SelectHandle(Ref handle) {
			// select handle
		}

		void Focus() {
			// set focus
		}

	private:
		Ref mAnimationWindow;
		RectF mSelectionRect;
		Vec2F mContextMenuPressPoint;
		Ref mContextMenu;
		vector<Ref> mHandles;
	};

	// --- META ---
	DECLARE_CLASS(Editor::KeyHandlesSheet, Editor__KeyHandlesSheet);
	// --- END META ---
}