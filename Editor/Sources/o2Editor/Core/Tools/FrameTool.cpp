Replace raw pointers with Ref<> and const Ref<>& for function arguments:

```cpp
#include "o2Editor/stdafx.h"
#include "FrameTool.h"

#include "o2/Render/Render.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"
#include "o2Editor/Core/Actions/Transform.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/SceneHierarchyTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

namespace Editor
{
	FrameTool::FrameTool()
	{
		mLeftTopRotateHandle.SetRegularDrawable(mmake<Ref<Sprite>>("ui/UI3_rotate_regular.png"));
		mLeftTopRotateHandle.SetHoverDrawable(mmake<Ref<Sprite>>("ui/UI3_rotate_hover.png"));
		mLeftTopRotateHandle.SetPressedDrawable(mmake<Ref<Sprite>>("ui/UI3_rotate_pressed.png"));
		mLeftTopRotateHandle.GetRegularDrawable()->pivot = Vec2F(0, 0);
		mLeftTopRotateHandle.GetHoverDrawable()->pivot = Vec2F(0, 0);
		mLeftTopRotateHandle.GetPressedDrawable()->pivot = Vec2F(0, 0);
		mLeftTopRotateHandle.pixelPerfect = false;

		mLeftBottomRotateHandle = mLeftTopRotateHandle;
		mRightTopRotateHandle = mLeftTopRotateHandle;
		mRightBottomRotateHandle = mLeftTopRotateHandle;

		mLeftTopHandle.SetRegularDrawable(mmake<Ref<Sprite>>("ui/UI2_handle_regular.png"));
		mLeftTopHandle.SetHoverDrawable(mmake<Ref<Sprite>>("ui/UI2_handle_select.png"));
		mLeftTopHandle.SetPressedDrawable(mmake<Ref<Sprite>>("ui/UI2_handle_pressed.png"));
		mLeftTopHandle.pixelPerfect = false;

		mLeftBottomHandle = mLeftTopHandle;
		mRightTopHandle = mLeftTopHandle;
		mRightBottomHandle = mLeftTopHandle;

		mLeftHandle.SetRegularDrawable(mmake<Ref<Sprite>>("ui/UI2_handle_side_regular.png"));
		mLeftHandle.SetHoverDrawable(mmake<Ref<Sprite>>("ui/UI2_handle_side_select.png"));
		mLeftHandle.SetPressedDrawable(mmake<Ref<Sprite>>("ui/UI2_handle_side_pressed.png"));
		mLeftHandle.pixelPerfect = false;

		mTopHandle = mLeftHandle;
		mBottomHandle = mLeftHandle;
		mRightHandle = mLeftHandle;

		mPivotHandle.SetRegularDrawable(mmake<Ref<Sprite>>("ui/UI2_pivot.png"));
		mPivotHandle.SetHoverDrawable(mmake<Ref<Sprite>>("ui/UI2_pivot_select.png"));
		mPivotHandle.SetPressedDrawable(mmake<Ref<Sprite>>("ui/UI2_pivot_pressed.png"));
		mPivotHandle.checkSnappingFunc = THIS_FUNC(CheckPivotSnapping);

		mAnchorsLeftTopHandle.SetRegularDrawable(mmake<Ref<Sprite>>("ui/UI3_anchor_pressed.png"));
		mAnchorsLeftTopHandle.SetHoverDrawable(mmake<Ref<Sprite>>("ui/UI3_anchor_hover.png"));
		mAnchorsLeftTopHandle.SetPressedDrawable(mmake<Ref<Sprite>>("ui/UI3_anchor_regular.png"));
		mAnchorsLeftTopHandle.GetRegularDrawable()->pivot = Vec2F(1, 0);
		mAnchorsLeftTopHandle.GetHoverDrawable()->pivot = Vec2F(1, 0);
		mAnchorsLeftTopHandle.GetPressedDrawable()->pivot = Vec2F(1, 0);

		mAnchorsRightBottomHandle = mAnchorsLeftTopHandle;
		mAnchorsLeftBottomHandle = mAnchorsLeftTopHandle;
		mAnchorsRightTopHandle = mAnchorsLeftTopHandle;

		auto centerAnchorsRegularSprite = mmake<Ref<Sprite>>();
		centerAnchorsRegularSprite->SetSize(Vec2F(0, 0));
		mAnchorsCenter.SetRegularDrawable(centerAnchorsRegularSprite);
		mAnchorsCenter.SetHoverDrawable(mmake<Ref<Sprite>>("ui/UI3_anchors_hover.png"));
		mAnchorsCenter.SetPressedDrawable(mmake<Ref<Sprite>>("ui/UI3_anchors_pressed.png"));

		mLeftTopHandle.onChangedPos = THIS_FUNC(OnLeftTopHandle);
		mLeftHandle.onChangedPos = THIS_FUNC(OnLeftHandle);
		mLeftBottomHandle.onChangedPos = THIS_FUNC(OnLeftBottomHandle);
		mTopHandle.onChangedPos = THIS_FUNC(OnTopHandle);
		mBottomHandle.onChangedPos = THIS_FUNC(OnBottomHandle);
		mRightTopHandle.onChangedPos = THIS_FUNC(OnRightTopHandle);
		mRightHandle.onChangedPos = THIS_FUNC(OnRightHandle);
		mRightBottomHandle.onChangedPos = THIS_FUNC(OnRightBottomHandle);
		mPivotHandle.onChangedPos = THIS_FUNC(OnPivotHandle);
		mLeftTopRotateHandle.onChangedPos = THIS_FUNC(OnLeftTopRotateHandle);
		mLeftBottomRotateHandle.onChangedPos = THIS_FUNC(OnLeftBottomRotateHandle);
		mRightTopRotateHandle.onChangedPos = THIS_FUNC(OnRightTopRotateHandle);
		mRightBottomRotateHandle.onChangedPos = THIS_FUNC(OnRightBottomRotateHandle);
		mAnchorsLeftTopHandle.onChangedPos = THIS_FUNC(OnAnchorLeftTopHandle);
		mAnchorsRightBottomHandle.onChangedPos = THIS_FUNC(OnAnchorRightBottomHandle);
		mAnchorsLeftBottomHandle.onChangedPos = THIS_FUNC(OnAnchorLeftBottomHandle);
		mAnchorsRightTopHandle.onChangedPos = THIS_FUNC(OnAnchorRightTopHandle);
		mAnchorsCenter.onChangedPos = THIS_FUNC(OnCenterAnchorHandle);

		mLeftTopHandle.onPressed = THIS_FUNC(HandlePressed);
		mLeftHandle.onPressed = THIS_FUNC(HandlePressed);
		mLeftBottomHandle.onPressed = THIS_FUNC(HandlePressed);
		mTopHandle.onPressed = THIS_FUNC(HandlePressed);
		mBottomHandle.onPressed = THIS_FUNC(HandlePressed);
		mRightTopHandle.onPressed = THIS_FUNC(HandlePressed);
		mRightHandle.onPressed = THIS_FUNC(HandlePressed);
		mRightBottomHandle.onPressed = THIS_FUNC(HandlePressed);
		mPivotHandle.onPressed = THIS_FUNC(HandlePressed);
		mLeftTopRotateHandle.onPressed = THIS_FUNC(HandlePressed);
		mLeftBottomRotateHandle.onPressed = THIS_FUNC(HandlePressed);
		mRightTopRotateHandle.onPressed = THIS_FUNC(HandlePressed);
		mRightBottomRotateHandle.onPressed = THIS_FUNC(HandlePressed);
	}
}
```

Note: I haven't replaced the `void*` pointers and removed the default nullptr values for class members.Ref<>::Cast<TransformObjectsWithAction(Basis::Translated(Vec2F::Left())));

		if (key == VK_RIGHT)
			TransformObjectsWithAction(Basis::Translated(Vec2F::Right()));

		if (key == VK_UP)
			TransformObjectsWithAction(Basis::Translated(Vec2F::Up()));

		if (key == VK_DOWN)
			TransformObjectsWithAction(Basis::Translated(Vec2F::Down()));

		SelectionTool::OnKeyPressed(key);
	}

	void FrameTool::OnKeyReleased(const Input::Key& key)
	{
		SelectionTool::OnKeyReleased(key);
	}

	void FrameTool::OnMousePressed(const Input::MouseEvent& event)
	{
		if (o2UI.GetEditorPopup()->GetSelection() && o2UI.GetEditorPopup()->GetSelection()->IsValid ())
			return;

		SelectionTool::OnMousePressed(event);

		if (event.button != Input::MouseButton::Left)
			return;

		for (auto& handle : mHandles)
		{
			if (!handle->IsPointInside(event.position))
				continue;

			if (auto lockedChild = handle->GetLockedChild())
			{
				for (auto object : o2EditorSceneScreen.GetSelectedObjects())
					object->Lock(lockedChild);
			}
			else if (handle == mPivotHandle)
			{
				mRotationCenter = mAnchorsCenter->GetCurrentCenter();

				auto rotationCenterMatrix =
					Basis::Translated(-mRotationCenter) * Basis::Rotated(mFrameRotation) * Basis::Translated(mRotationCenter);

				for (auto object : o2EditorSceneScreen.GetSelectedObjects())
				{
					object->SetStartingRotation(rotationCenterMatrix *
											  (((Basis)object->GetTransform().rotation).Angle2D()).RotationMatrix());
				}
			}

			break;
		}
	}

	void FrameTool::OnMouseMoved(const Input::MouseEvent& event)
	{
		SelectionTool::OnMouseMoved(event);

		if (!mCanMoveSelection || event.button != Input::MouseButton::Left)
			return;

		Vec2F delta = event.position - mPrevMousePos;

		TransformObjects(delta);
	}

	void FrameTool::OnMouseReleased(const Input::MouseEvent& event)
	{
		SelectionTool::OnMouseReleased(event);

		if (event.button != Input::MouseButton::Left)
			return;

		for (auto& handle : mHandles)
		{
			if (!handle->IsPointInside(event.position))
				continue;

			if (auto lockedChild = handle->GetLockedChild())
			{
				SetHandlesEnable(false);

				for (auto object : o2EditorSceneScreen.GetSelectedObjects())
				{
					array<SceneEditableObject*, PoolAllocator<SceneEditableObject*>>::reverse_iterator
						it = std::find(o2EditorSceneScreen.GetSelectedObjects().rbegin(),
									   o2EditorSceneScreen.GetSelectedObjects().rend(), object);

					bool isUnlocked = true;

					for (++it; it != o2EditorSceneScreen.GetSelectedObjects().rend(); ++it)
					{
						SceneEditableObject* nextObject = *it;

						if (nextObject->GetEditableParent() == object)
						{
							nextObject->Unlock();
							nextObject->SetParent(lockedChild);
							object->SetParent(lockedChild);

							it = --(o2EditorSceneScreen.GetSelectedObjects().rend());
							isUnlocked = false;
							break;
						}
					}

					if (isUnlocked)
					{
						object->Unlock();
						object->SetParent(lockedChild);
					}
				}

				if (false == handle->isPermanent)
					handle->UnlockChild();
			}
			else if (handle == mPivotHandle)
			{
				for (auto object : o2EditorSceneScreen.GetSelectedObjects())
				{
					object->SetStartingRotation(object->GetTransform().rotation);
					object->ClearAction();
				}
			}

			break;
		}
	}

	void FrameTool::TransformObjects(const Vec2F& delta)
	{
		Vec2F pixelOffset = delta / o2Renderer.GetZoom();

		int snappedPixelAccuracy = GetSnappedPixelAccuracy();

		if (mCurrentHandle)
		{
			if (false == mCurrentHandle->isPermanent)
				SetHandlesEnable(true);

			Vec2F v0 = mCurrentHandle->GetCurrentCenter();
			Vec2F v1 = mCurrentHandle->GetCurrentCenter() + pixelOffset;

			if (mAnchorsFrameEnabled == false)
			{
				v1 = GetSnappedPoint(v1, snappedPixelAccuracy);

				mTransformStep = v1 - v0;

				SetSelectionTranslation(Basis::Translated(mTransformStep));
			}
			else
			{
				Vec2F v2 = mCurrentHandle->GetCurrentCenter();

				v2.x += 0.5f;
				v2.y += 0.5f;

				Line l(v1 - v2, v0 - v2);

				bool v1IsAround = l.side(v1) < 0;
				bool v0IsAround = l.side(v0) < 0;

				if (v1IsAround != v0IsAround)
				{
					float side = l.side(v1);
					Vec2F intersectionPoint = mCurrentHandle->GetCurrentCenter();

					if (v1IsAround)
						intersectionPoint = intersectionPoint - (v2 - v1) * abs(side);
					else
						intersectionPoint = intersectionPoint - (v2 - v0) * abs(side);

					v1 = intersectionPoint + (v1 - intersectionPoint).Normalized() * snappedPixelAccuracy;

					mTransformStep = v1 - v0;

					SetSelectionTranslation(Basis::Translated(mTransformStep));
				}
				else
				{
					Vec2F newV1 = v1;

					newV1 = GetSnappedPoint(v1, snappedPixelAccuracy);

					mTransformStep = newV1 - v0;

					SetSelectionTranslation(Basis::Translated(mTransformStep));
					return;
				}
			}
		}
		else
		{
			SetHandlesEnable(true);

			Vec2F v0 = mFrame.position;
			Vec2F v1 = v0 + pixelOffset;

			if (o2UI.IsMouseInUI())
			{
				mTransformStep = v1 - v0;

				SetSelectionTranslation(Basis::Translated(mTransformStep));
			}
			else
			{
				mTransformStep += pixelOffset;

				Vec2F newV1 = v1;

				Vec2F snapped = GetSnappedPoint(v1, snappedPixelAccuracy);
				newV1.x = snapped.x - (snapped.x - v0.x > 0 ? 1 : -1) * snappedPixelAccuracy;
				newV1.y = snapped.y - (snapped.y - v0.y > 0 ? 1 : -1) * snappedPixelAccuracy;

				mTransformStep = newV1 - v0;

				SetSelectionTranslation(Basis::Translated(mTransformStep));
				return;
			}
		}

		for (auto object : o2EditorSceneScreen.GetSelectedObjects ())
		{
			object->TransformSelectedObjects();
		}

		UpdateSelectionFrame();

		UpdateAnchorsFrame();
	}

	void FrameTool::HandlePressed(Handle* handle)
	{
		SetHandlesEnable(true);

		mCurrentHandle = handle;

		mTransformStep = Vec2F::Zero();
	}

	void FrameTool::HandleReleased()
	{
		mCurrentHandle = nullptr;

		ResetTransformStep();
	}

	void FrameTool::SetHandlesEnable(bool enable)
	{
		mHandlesEnabled = enable;

		for (auto& handle : mHandles)
			handle->SetEnabled(enable);
	}

	void FrameTool::TransformObjectsWithAction(const Basis& action)
	{
		TransformActionObjects(action);
	}

	void FrameTool::TransformActionObjects(const Basis& action)
	{
		Vec2F snappedOffset = GetSnappedOffset(Vec2F(mTransformStep.x(), mTransformStep.y()), GetSnappedPixelAccuracy());

		Basis currAction = Basis::Translated(snappedOffset.x, snappedOffset.y).Translated(mRotationCenter) * action;

		for (auto object : o2EditorSceneScreen.GetSelectedObjects ())
		{
			TransformTransformAction(object, currAction);
			mNeedRedraw = true;
		}

		mTransformStep = snappedOffset;
	}

	void FrameTool::TransformTransformAction(SceneEditableObject* obj, const Basis& action)
	{
		if (obj->GetEditingObject())
			obj->PerformAction(obj->GetTransform(), action);
	}

	void FrameTool::SetSelectionTranslation(const Basis& transform)
	{
		int lockedMask = 0;
		bool allLocked = true;
		bool allNotLocked = true;
		Vec2F minCorner = Vec2F(FLT_MAX, FLT_MAX);
		Vec2F maxCorner = Vec2F(FLT_MIN, FLT_MIN);
		Vec2F rotationCenter = Vec2F::Zero();

		int count = 0;

		for (auto object : o2EditorSceneScreen.GetSelectedObjects ())
		{
			if (object->GetEditableParent())
			{
				maxCorner = std::max(maxCorner, object->GetEditableParent()->GetTransformedNode()->GetMinBorder());
				minCorner = std::min(minCorner, object->GetEditableParent()->GetTransformedNode()->GetMaxBorder());
			}

			lockedMask |= object->GetNode()->GetLockMask();

			if (object->IsLocked())
			{
				allNotLocked = false;
				count++;
			}
			else
			{
				allLocked = false;
			}
		}

		if (count == 1)
		{
			if (allLocked)
			{
				// Lock : 1. Base On Pivot.
			}
			if (allNotLocked)
			{
				// Unlock : .
			}
		}
		else if (count == o2EditorSceneScreen.GetSelectedObjects().Count())
		{
			if (allLocked)
			{
				// Lock : 2. Base On Rect.
			}
			if (allNotLocked && !lockedMask)
			{
				// Unlock : 2. Base On Rect.
			}
			if (allNotLocked && lockedMask)
			{
				// Partial lock.
			}
		}
		else if (count < o2EditorSceneScreen.GetSelectedObjects().Count())
		{
			if (allNotLocked && !lockedMask)
			{
				// Unlock : 3. Base On Rect.
			}
			if (allNotLocked && lockedMask)
			{
				// Partial lock.
			}
		}

		Basis currentTransform = CalculateTranslate(transform, true);

		for (auto object : o2EditorSceneScreen.GetSelectedObjects ())
		{
			if (object->IsLocked())
			{
				mChangedFromThis = true;
				continue;
			}

			Basis t = object->GetTransform();

			t.origin.x += mFrame.position.x - mCurrentStatus.frame.position.x;
			t.origin.y += mFrame.position.y - mCurrentStatus.frame.position.y;

			Vec2F pos = currentTransform * object->GetPosition ();

			object->SetTransform(t.Translated(pos));
		}

		if (lockedMask == 0)
			mCurrentStatus.frame.position += mTransformStep;

		SetHandlesEnable(!(lockedMask == 0 && count == 1));
	}

	void FrameTool::ResetTransformStep()
	{
		if (!mHandlesEnabled)
			return;

		mTransformStep = Basis::Identity ().origin;
	}

	void FrameTool::UpdateSelectionFrame()
	{
		const Vector<SceneEditableObject*>& selectedObjects = o2EditorSceneScreen.GetSelectedObjects ();

		mFrame.position = mObjectColor = Vec2F(FLT_MAX, FLT_MAX);
		mFrame.extents = mObjectColor = Vec2F(FLT_MIN, FLT_MIN);

		mFrameRotation = 0.0f;

		bool anyEditableObjects = false;

		for (auto object : selectedObjects)
		{
			if (!object->IsEditable ())
				continue;

			anyEditableObjects = true;

			AABB2 frame = object->GetTransform ().TransformedAABB2(object->GetBounds ());

			mFrame.position = std::min(mFrame.position,#include <memory>

class RefBase {
protected:
	RefBase() : refCount(0) {}
	virtual ~RefBase() {}

public:
	void AddRef() { ++refCount; }
	void Release() { if (--refCount == 0) delete this; }

private:
	int refCount;
};

template<typename T>
class Ref {
public:
	Ref() : p(nullptr) {}
	Ref(T* ptr) : p(ptr) { if (p) p->AddRef(); }
	Ref(const Ref& other) : p(other.p) { if (p) p->AddRef(); }
	~Ref() { if (p) p->Release(); }

	Ref& operator=(T* ptr) {
		if (p != ptr) {
			if (p) p->Release();
			p = ptr;
			if (p) p->AddRef();
		}
		return *this;
	}

	Ref& operator=(const Ref& other) {
		if (this != &other) {
			if (p) p->Release();
			p = other.p;
			if (p) p->AddRef();
		}
		return *this;
	}

	T* operator->() { return p; }
	const T* operator->() const { return p; }

	T& operator*() { return *p; }
	const T& operator*() const { return *p; }

	operator bool() const { return p != nullptr; }

private:
	T* p;
};

template<typename T>
class WeakRef {
public:
	WeakRef() : p(nullptr) {}
	WeakRef(T* ptr) : p(ptr) { if (p) p->AddWeakRef(); }
	WeakRef(const WeakRef& other) : p(other.p) { if (p) p->AddWeakRef(); }
	~WeakRef() { if (p) p->ReleaseWeak(); }

	WeakRef& operator=(T* ptr) {
		if (p != ptr) {
			if (p) p->ReleaseWeak();
			p = ptr;
			if (p) p->AddWeakRef();
		}
		return *this;
	}

	WeakRef& operator=(const WeakRef& other) {
		if (this != &other) {
			if (p) p->ReleaseWeak();
			p = other.p;
			if (p) p->AddWeakRef();
		}
		return *this;
	}

	bool expired() const {
		return p == nullptr || p->expired();
	}

	Ref<T> lock() const {
		if (expired()) {
			return nullptr;
		}
		else {
			return Ref<T>(p);
		}
	}

	operator bool() const { return p != nullptr; }

private:
	T* p;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
	return Ref<T>(new T(std::forward<Args>(args)...));
}

class RefBase {
protected:
	RefBase() : refCount(0) {}
	virtual ~RefBase() {}

public:
	void AddRef() { ++refCount; }
	void Release() { if (--refCount == 0) delete this; }

private:
	int refCount;
};

template<typename T>
class Ref {
public:
	Ref() : p(nullptr) {}
	Ref(T* ptr) : p(ptr) { if (p) p->AddRef(); }
	Ref(const Ref& other) : p(other.p) { if (p) p->AddRef(); }
	template<typename U, typename std::enable_if<std::is_convertible<U*, T*>::value, int>::type = 0>
	Ref(const Ref<U>& other) : p(other.get()) { if (p) p->AddRef(); }
	~Ref() { if (p) p->Release(); }

	Ref& operator=(T* ptr) {
		if (p != ptr) {
			if (p) p->Release();
			p = ptr;
			if (p) p->AddRef();
		}
		return *this;
	}

	Ref& operator=(const Ref& other) {
		if (this != &other) {
			if (p) p->Release();
			p = other.p;
			if (p) p->AddRef();
		}
		return *this;
	}

	template<typename U, typename std::enable_if<std::is_base_of<T, U>::value, int>::type = 0>
	Ref& operator=(const Ref<U>& other) {
		if (p != other.get()) {
			if (p) p->Release();
			p = other.get();
			if (p) p->AddRef();
		}
		return *this;
	}

	T* operator->() { return p; }
	const T* operator->() const { return p; }

	T& operator*() { return *p; }
	const T& operator*() const { return *p; }

	operator bool() const { return p != nullptr; }

private:
	template<typename U>
	friend class Ref;

	T* p;
};

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr) {
	if (ptr) {
		T* dynamicPtr = dynamic_cast<T*>(ptr.get());
		if (dynamicPtr) {
			return Ref<T>(dynamicPtr);
		}
	}
	return nullptr;
}

constexpr Vec2F Vec2F::Left() const { return Vec2F(-1, 0); }
constexpr Vec2F Vec2F::Right() const { return Vec2F(1, 0); }
constexpr Vec2F Vec2F::Up() const { return Vec2F(0, 1); }
constexpr Vec2F Vec2F::Down() const { return Vec2F(0, -1); }

void FrameTool::TransformObjects(const Basis& transform)
{
	for (const auto& object : o2EditorSceneScreen.GetTopSelectedObjects())
	{
		if (object->GetTransform().GetScale() != Vec2F())
			object->SetTransform(object->GetTransform()*transform);
		// 			else
		// 				actor->transform->SetWorldNonSizedBasis(actor->transform->GetWorldNonSizedBasis()*transform);

		object->UpdateTransform();
	}

	mChangedFromThis = true;

	UpdateSelectionFrame();
	UpdateHandlesTransform();
}

void FrameTool::TransformObjectsWithAction(const Basis& transform)
{
	mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedObjects().Convert<Basis>(
		[](SceneEditableObject* x) { return x->GetTransform(); });

	auto action = mmake<TransformAction>(o2EditorSceneScreen.GetTopSelectedObjects());

	TransformObjects(transform);

	action->Completed();
	o2EditorApplication.DoneAction(action);
}

void FrameTool::TransformAnchorsObjects(const Basis& transform)
{
	RectF anchorsFrame(transform.origin, transform.origin + Vec2F(transform.xv.Length(), transform.yv.Length()));

	for (const auto& object : o2EditorSceneScreen.GetTopSelectedObjects())
	{
		if (object->IsSupportsLayout())
		{
			const auto& parent = object->GetEditableParent();
			auto parentWidget = DynamicCast<Widget>(parent);

			if (parent)
			{
				RectF parentWorldRect;

				if (parentWidget)
					parentWorldRect = parentWidget->GetChildrenWorldRect();
				else
					parentWorldRect = parent->GetTransform().AABB();

				auto prevTransform = object->GetTransform();
				auto layout = object->GetLayout();
				layout.anchorMin = (anchorsFrame.LeftBottom() - parentWorldRect.LeftBottom()) / parentWorldRect.Size();
				layout.anchorMax = (anchorsFrame.RightTop() - parentWorldRect.LeftBottom()) / parentWorldRect.Size();
				object->SetLayout(layout);
				object->UpdateTransform();

				object->SetTransform(prevTransform);
				object->UpdateTransform();
			}
		}
	}

	mChangedFromThis = true;

	UpdateSelectionFrame();
	UpdateHandlesTransform();
}

void FrameTool::UpdateSelectionFrame()
{
	const auto& selectedObjects = o2EditorSceneScreen.GetSelectedObjects();

	mAnchorsFrameEnabled = false;
	mPivotHandleEnabled = true;

	if (selectedObjects.Count() == 1)
	{
		const auto& object = selectedObjects[0];

		mFrame = object->GetTransform();
		mPivotHandle.position = object->GetPivot();

		mAnchorsFrameEnabled = object->IsSupportsLayout();
		mPivotHandleEnabled = object->IsSupportsPivot();

		if (mAnchorsFrameEnabled)
		{
			const auto& parent = object->GetEditableParent();
			auto parentWidget = DynamicCast<Widget>(parent);

			RectF parentWorldRect;

			if (parentWidget)
				parentWorldRect = parentWidget->GetChildrenWorldRect();
			else if (parent)
				parentWorldRect = parent->GetTransform().AABB();
			else
				mAnchorsFrameEnabled = false;

			RectF worldRectangle(parentWorldRect.LeftBottom() + object->GetLayout().anchorMin*parentWorldRect.Size(),
				parentWorldRect.LeftBottom() + object->GetLayout().anchorMax*parentWorldRect.Size());

			mAnchorsFrame.origin = worldRectangle.LeftBottom();
			mAnchorsFrame.xv = Vec2F(Math::Max(worldRectangle.Width(), 0.001f), 0);
			mAnchorsFrame.yv = Vec2F(0, Math::Max(worldRectangle.Height(), 0.001f));
		}
	}
	else if (selectedObjects.Count() > 0)
	{
		Basis frameBasis = selectedObjects.Last()->GetTransform();
		Vec2F frameOrigin = frameBasis.origin;
		Vec2F xAxis = frameBasis.xv.Normalized();
		Vec2F yAxis = frameBasis.yv.Normalized();
		Vec2F sx, sy;

		const Vec2F cp[4] = { Vec2F(0, 0), Vec2F(0, 1), Vec2F(1, 0), Vec2F(1, 1) };
		for (const auto& object : selectedObjects)
		{
			const Basis objectTransform = object->GetTransform();
			for (int i = 0; i < 4; i++)
			{
				Vec2F wp = cp[i] * objectTransform;
				float px = (wp - frameOrigin).Dot(xAxis);
				float py = (wp - frameOrigin).Dot(yAxis);

				sx.x = Math::Min(sx.x, px);
				sx.y = Math::Max(sx.y, px);
				sy.x = Math::Min(sy.x, py);
				sy.y = Math::Max(sy.y, py);
			}
		}

		mFrame.Set(frameOrigin + xAxis*sx.x + yAxis*sy.x, xAxis*(sx.y - sx.x), yAxis*(sy.y - sy.x));
		mPivotHandle.position = mFrame.origin + mFrame.xv*0.5f + mFrame.yv*0.5f;
	}
	else
	{
		SetHandlesEnable(false);
		return;
	}

	SetHandlesEnable(true);
	UpdateHandlesTransform();
}

void FrameTool::OnCursorPressed(const Input::Cursor& cursor)
{
	if (o2EditorSceneScreen.GetSelectedObjects().Count() > 0)
	{
		if (mFrame.IsPointInside(o2EditorSceneScreen.ScreenToScenePoint(cursor.position)))
		{
			mIsDragging = true;
			mBeginDraggingFrame = mFrame;
			mBeginDraggingOffset = o2EditorSceneScreen.ScreenToSceneVector(cursor.position) - mFrame.origin;
		}
	}
}

void FrameTool::OnCursorReleased(const Input::Cursor& cursor)
{
	if (mIsDragging)
	{
		mIsDragging = false;

		for (const auto& object : o2EditorSceneScreen.GetSelectedObjects())
		{
			if (object->IsSupportsLayout() && object->IsResizable())
			{
				const auto layout = object->GetLayout();

				Vec2F deltaScale;
				deltaScale.x = object->GetWidth() / mBeginDraggingFrame.xv.Length();
				deltaScale.y#include <memory>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using WeakRef = std::weak_ptr<T>;

template <typename T, typename... Args>
inline Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
struct DynamicCast
{
    template <typename U>
    static Ref<T> cast(const Ref<U>& ptr)
    {
        return std::dynamic_pointer_cast<T>(ptr);
    }
};

void FrameTool::OnCursorPressed(const Input::Cursor& cursor)
{
    if (o2EditorSceneScreen.GetSelectedObjects().Count() > 0)
    {
        Vec2F localPoint = o2EditorSceneScreen.ScreenToLocalPoint(cursor.position);
        if (mFrame.Contains(localPoint))
        {
            mIsDragging = true;
            mBeginDraggingOffset = localPoint;
            mBeginDraggingFrame = mFrame;

            if (!o2Input.IsKeyDown(VK_CONTROL))
                SelectionTool::OnCursorPressed(cursor);
            else
            {
                // Check handle pressed based on the position of cursor
                if (mLeftTopHandleBox.Contains(localPoint))
                    OnLeftTopHandle(localPoint);
                else if (mLeftHandleBox.Contains(localPoint))
                    OnLeftHandle(localPoint);
                else if (mLeftBottomHandleBox.Contains(localPoint))
                    OnLeftBottomHandle(localPoint);
                else if (mTopHandleBox.Contains(localPoint))
                    OnTopHandle(localPoint);
                else if (mBottomHandleBox.Contains(localPoint))
                    OnBottomHandle(localPoint);
                else if (mRightTopHandleBox.Contains(localPoint))
                    OnRightTopHandle(localPoint);
                else if (mRightHandleBox.Contains(localPoint))
                    OnRightHandle(localPoint);
                else if (mRightBottomHandleBox.Contains(localPoint))
                    OnRightBottomHandle(localPoint);
                else if (mCenterHandleBox.Contains(localPoint))
                    OnCenterHandle(localPoint);
                else if (mAnchorLeftTopHandleBox.Contains(localPoint))
                    OnAnchorLeftTopHandle(localPoint);
                else if (mAnchorLeftBottomHandleBox.Contains(localPoint))
                    OnAnchorLeftBottomHandle(localPoint);
                else if (mAnchorRightTopHandleBox.Contains(localPoint))
                    OnAnchorRightTopHandle(localPoint);
                else if (mAnchorRightBottomHandleBox.Contains(localPoint))
                    OnAnchorRightBottomHandle(localPoint);
                else if (mPivotHandleBox.Contains(localPoint))
                    OnPivotHandle(localPoint);
                else if (mLeftTopRotateHandleBox.Contains(localPoint))
                    OnLeftTopRotateHandle(localPoint);
                else if (mLeftBottomRotateHandleBox.Contains(localPoint))
                    OnLeftBottomRotateHandle(localPoint);
                else if (mRightTopRotateHandleBox.Contains(localPoint))
                    OnRightTopRotateHandle(localPoint);
                else if (mRightBottomRotateHandleBox.Contains(localPoint))
                    OnRightBottomRotateHandle(localPoint);
                else
                    SelectionTool::OnCursorPressed(cursor);

                SetHandlesEnable(false);
                HandlePressed();
                return;
            }
        }
    }

    SelectionTool::OnCursorPressed(cursor);
}

void FrameTool::OnCursorReleased(const Input::Cursor& cursor)
{
    if (mIsDragging)
    {
        mIsDragging = false;
        SetHandlesEnable(true);
        HandleReleased();

        const float hasntMovedThreshold = 2.0f;
        if (o2EditorSceneScreen.SceneToScreenVector(mFrame.origin - mBeginDraggingFrame.origin).Length() < hasntMovedThreshold)
            SelectionTool::OnCursorReleased(cursor);
    }
    else
        SelectionTool::OnCursorReleased(cursor);
}

void FrameTool::OnCursorPressBreak(const Input::Cursor& cursor)
{
    if (mIsDragging)
    {
        mIsDragging = false;
        SetHandlesEnable(true);
    }
    else
        SelectionTool::OnCursorPressBreak(cursor);
}

void FrameTool::OnCursorStillDown(const Input::Cursor& cursor)
{
    if (mIsDragging)
    {
        mSnapLines.Clear();

        Vec2F cursorPos = o2EditorSceneScreen.ScreenToLocalPoint(cursor.position);

        if (o2Input.IsKeyDown(VK_CONTROL))
        {
            Basis preTransformed(cursorPos - mBeginDraggingOffset, mBeginDraggingFrame.xv, mBeginDraggingFrame.yv);

            cursorPos = CalculateSnapOffset(cursorPos, preTransformed,
                                            { Vec2F(0, 0), Vec2F(0, 1), Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(1, 0), Vec2F(1, 1) }, preTransformed.xv.Normalized(),
                                            { Vec2F(0, 0), Vec2F(1, 0), Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, 1), Vec2F(1, 1) }, preTransformed.yv.Normalized(),
                                            GetObjectsTransforms(o2Scene.GetAllEditableObjects()));
        }

        Vec2F delta = cursorPos - mBeginDraggingOffset;
        Basis transformed(delta, mBeginDraggingFrame.xv, mBeginDraggingFrame.yv);

        if (o2Input.IsKeyDown(VK_SHIFT))
        {
            if (Math::Abs(transformed.origin.x - mBeginDraggingFrame.origin.x) > Math::Abs(transformed.origin.y - mBeginDraggingFrame.origin.y))
                transformed.origin.y = mBeginDraggingFrame.origin.y;
            else
                transformed.origin.x = mBeginDraggingFrame.origin.x;
        }

        TransformObjects(mFrame.Inverted() * transformed);
    }
    else
        SelectionTool::OnCursorStillDown(cursor);
}

void FrameTool::OnLeftTopHandle(const Vec2F& position)
{
    TransformObjects(mFrame.Inverted() * GetLeftTopHandleTransformed(position));
}

void FrameTool::OnLeftHandle(const Vec2F& position)
{
    TransformObjects(mFrame.Inverted() * GetLeftHandleTransformed(position));
}

void FrameTool::OnLeftBottomHandle(const Vec2F& position)
{
    TransformObjects(mFrame.Inverted() * GetLeftBottomHandleTransformed(position));
}

void FrameTool::OnTopHandle(const Vec2F& position)
{
    TransformObjects(mFrame.Inverted() * GetTopHandleTransformed(position));
}

void FrameTool::OnBottomHandle(const Vec2F& position)
{
    TransformObjects(mFrame.Inverted() * GetBottomHandleTransformed(position));
}

void FrameTool::OnRightTopHandle(const Vec2F& position)
{
    TransformObjects(mFrame.Inverted() * GetRightTopHandleTransformed(position));
}

void FrameTool::OnRightHandle(const Vec2F& position)
{
    TransformObjects(mFrame.Inverted() * GetRightHandleTransformed(position));
}

void FrameTool::OnRightBottomHandle(const Vec2F& position)
{
    TransformObjects(mFrame.Inverted() * GetRightBottomHandleTransformed(position));
}

void FrameTool::OnAnchorLeftTopHandle(const Vec2F& position)
{
    TransformAnchorsObjects(GetLeftTopAnchorHandleTransformed(position));
}

void FrameTool::OnAnchorLeftBottomHandle(const Vec2F& position)
{
    TransformAnchorsObjects(GetLeftBottomAnchorHandleTransformed(position));
}

void FrameTool::OnAnchorRightTopHandle(const Vec2F& position)
{
    TransformAnchorsObjects(GetRightTopAnchorHandleTransformed(position));
}

void FrameTool::OnAnchorRightBottomHandle(const Vec2F& position)
{
    TransformAnchorsObjects(GetRightBottomAnchorHandleTransformed(position));
}

void FrameTool::OnCenterAnchorHandle(const Vec2F& position)
{
    TransformAnchorsObjects(GetAnchorsCenterHandleTransformed(position));
}

void FrameTool::OnPivotHandle(const Vec2F& position)
{
    auto selectedObjects = o2EditorSceneScreen.GetSelectedObjects();
    if (selectedObjects.Count() == 1)
        selectedObjects[0]->SetPivot(position);
}

void FrameTool::OnLeftTopRotateHandle(const Vec2F& position)
{
    Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f) * mFrame;
    OnRotateHandle(position, lastHandleCoords);
}

void FrameTool::OnLeftBottomRotateHandle(const Vec2F& position)
{
    Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f) * mFrame;
    OnRotateHandle(position, lastHandleCoords);
}

void FrameTool::OnRightTopRotateHandle(const Vec2F& position)
{
    Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f) * mFrame;
    OnRotateHandle(position, lastHandleCoords);
}

void FrameTool::OnRightBottomRotateHandle(const Vec2F& position)
{
    Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f) * mFrame;
    OnRotateHandle(position, lastHandleCoords);
}

void FrameTool::OnRotateHandle(const Vec2F& position, Vec2F lastHandleCoords)
{
    Vec2F rotatePivot = mPivotHandle.position;
    float angle = (position - lastHandleCoords).GetAngle() - (rotatePivot - lastHandleCoords).GetAngle();
    if (Math::IsNaN(angle))
        return;

    if (o2Input.IsKeyDown(VK_SHIFT))
        angle = Math::Floor(angle / (Math::PI / 4.0f)) * (Math::PI / 4.0f);

    RotateObjects(rotatePivot, angle);
}#include <memory>

template <typename T>
class Ref
{
public:
	Ref() : ptr(nullptr) {}
	Ref(T* p) : ptr(p) {}
	Ref(const Ref<T>& other) : ptr(other.ptr) {}
	Ref(Ref<T>&& other) noexcept : ptr(std::move(other.ptr)) {}
	
	~Ref() { delete ptr; }
	
	Ref<T>& operator=(const Ref<T>& other)
	{
		if (this != &other)
		{
			delete ptr;
			ptr = other.ptr;
		}
		return *this;
	}
	
	Ref<T>& operator=(Ref<T>&& other) noexcept
	{
		if (this != &other)
		{
			delete ptr;
			ptr = std::move(other.ptr);
		}
		return *this;
	}
	
	T& operator*() const { return *ptr; }
	T* operator->() const { return ptr; }
	
private:
	T* ptr;
};

template <typename T>
Ref<T> mmake()
{
	return Ref<T>(new T());
}

template <typename Derived, typename Base>
Ref<Derived> DynamicCast(const Ref<Base>& ptr)
{
	return Ref<Derived>(dynamic_cast<Derived*>(ptr.operator->()));
}

Ref<const Vec2F>& mFrame;
Ref<FrameHandle> mLeftTopHandle;
Ref<FrameHandle> mLeftHandle;
Ref<FrameHandle> mLeftBottomHandle;
Ref<FrameHandle> mTopHandle;
Ref<FrameHandle> mBottomHandle;
Ref<FrameHandle> mRightTopHandle;
Ref<FrameHandle> mRightHandle;
Ref<FrameHandle> mRightBottomHandle;
Ref<FrameHandle> mLeftTopRotateHandle;
Ref<FrameHandle> mLeftBottomRotateHandle;
Ref<FrameHandle> mRightTopRotateHandle;
Ref<FrameHandle> mRightBottomRotateHandle;
Ref<FrameHandle> mAnchorsLeftTopHandle;
Ref<FrameHandle> mAnchorsLeftBottomHandle;
Ref<FrameHandle> mAnchorsRightTopHandle;
Ref<FrameHandle> mAnchorsRightBottomHandle;
Ref<FrameHandle> mAnchorsCenter;
Ref<Basis> mAnchorsFrame;

void FrameTool::RotateHandles()
{
	Vec2F rotatePivot = (mLeftTopHandle->GetPosition() + mRightBottomHandle->GetPosition()) * 0.5f;
	float lastHandleAngle = Math::Rad2deg(mRightTopHandle->GetPosition().Angle(rotatePivot - mRightHandle->GetPosition()));
	float angle = mSignAngle * (cursorPosition.Angle(rotatePivot - mRightHandle->GetPosition()) - lastHandleAngle);
	Basis transform = Basis::Translated(rotatePivot * -1.0f) * Basis::Rotated(-angle) * Basis::Translated(rotatePivot);
	Basis transformed = *mFrame * transform;
	if (o2Input.IsKeyDown(VK_SHIFT))
	{
		float angle = Math::Rad2deg(transformed.GetAngle());
		float step = 15.0f;
		float targetAngle = Math::Round(angle / step) * step;
		float delta = targetAngle - angle;
		Basis deltaTransform = Basis::Translated(rotatePivot * -1.0f) * Basis::Rotated(Math::Deg2rad(delta)) * Basis::Translated(rotatePivot);
		transformed = transformed * deltaTransform;
	}
	TransformObjects((*mFrame).Inverted() * transformed);
}

void FrameTool::SetHandlesEnable(bool enable)
{
	mPivotHandle.enabled = enable;
	mLeftTopRotateHandle->enabled = enable;
	mLeftBottomRotateHandle->enabled = enable;
	mRightTopRotateHandle->enabled = enable;
	mRightBottomRotateHandle->enabled = enable;
	mLeftTopHandle->enabled = enable;
	mLeftHandle->enabled = enable;
	mLeftBottomHandle->enabled = enable;
	mTopHandle->enabled = enable;
	mBottomHandle->enabled = enable;
	mRightTopHandle->enabled = enable;
	mRightHandle->enabled = enable;
	mRightBottomHandle->enabled = enable;
	mAnchorsLeftBottomHandle->enabled = enable && mAnchorsFrameEnabled;
	mAnchorsLeftTopHandle->enabled = enable && mAnchorsFrameEnabled;
	mAnchorsRightTopHandle->enabled = enable && mAnchorsFrameEnabled;
	mAnchorsRightBottomHandle->enabled = enable && mAnchorsFrameEnabled;
	CheckAnchorsCenterEnabled();
}

void FrameTool::UpdateHandlesTransform()
{
	float handlesAngle = (*mFrame).GetAngle();
	auto getHandleType = [&](const Vec2F& pos) -> CursorType {
		Vec2F v = pos - Vec2F(0.5f, 0.5f) * (*mFrame);
		float angle = Math::Rad2deg(v.Angle(Vec2F::Right()));
		if (angle < 22.5f || angle > 337.5f)
			return CursorType::SizeWE;
		else if (angle > 22.5f && angle < 67.5f)
			return CursorType::SizeNwSe;
		else if (angle > 67.5f && angle < 112.5f)
			return CursorType::SizeNS;
		else if (angle > 112.5f && angle < 157.5f)
			return CursorType::SizeNeSw;
		else if (angle > 157.5f && angle < 202.5f)
			return CursorType::SizeWE;
		else if (angle > 202.5f && angle < 247.5f)
			return CursorType::SizeNwSe;
		else if (angle > 247.5f && angle < 292.5f)
			return CursorType::SizeNS;
		else if (angle > 292.5f && angle < 337.5f)
			return CursorType::SizeNeSw;
		return CursorType::SizeNeSw;
	};
	mLeftTopHandle->position = Vec2F(0.0f, 1.0f) * (*mFrame);
	mLeftHandle->position = Vec2F(0.0f, 0.5f) * (*mFrame);
	mLeftBottomHandle->position = Vec2F(0.0f, 0.0f) * (*mFrame);
	mTopHandle->position = Vec2F(0.5f, 1.0f) * (*mFrame);
	mBottomHandle->position = Vec2F(0.5f, 0.0f) * (*mFrame);
	mRightTopHandle->position = Vec2F(1.0f, 1.0f) * (*mFrame);
	mRightHandle->position = Vec2F(1.0f, 0.5f) * (*mFrame);
	mRightBottomHandle->position = Vec2F(1.0f, 0.0f) * (*mFrame);
	mLeftTopHandle->cursorType = getHandleType(mLeftTopHandle->GetPosition());
	mLeftHandle->cursorType = getHandleType(mLeftHandle->GetPosition());
	mLeftBottomHandle->cursorType = getHandleType(mLeftBottomHandle->GetPosition());
	mTopHandle->cursorType = getHandleType(mTopHandle->GetPosition());
	mBottomHandle->cursorType = getHandleType(mBottomHandle->GetPosition());
	mRightTopHandle->cursorType = getHandleType(mRightTopHandle->GetPosition());
	mRightHandle->cursorType = getHandleType(mRightHandle->GetPosition());
	mRightBottomHandle->cursorType = getHandleType(mRightBottomHandle->GetPosition());
	mLeftTopRotateHandle->position = Vec2F(0.0f, 1.0f) * (*mFrame);
	mLeftBottomRotateHandle->position = Vec2F(0.0f, 0.0f) * (*mFrame);
	mRightTopRotateHandle->position = Vec2F(1.0f, 1.0f) * (*mFrame);
	mRightBottomRotateHandle->position = Vec2F(1.0f, 0.0f) * (*mFrame);
	mLeftTopHandle->angle = handlesAngle + Math::PI() * 0.5f;
	mLeftHandle->angle = handlesAngle + Math::PI();
	mLeftBottomHandle->angle = handlesAngle + Math::PI();
	mTopHandle->angle = handlesAngle + Math::PI() * 0.5f;
	mBottomHandle->angle = handlesAngle - Math::PI() * 0.5f;
	mRightTopHandle->angle = handlesAngle;
	mRightHandle->angle = handlesAngle;
	mRightBottomHandle->angle = handlesAngle - Math::PI() * 0.5f;
	mLeftTopRotateHandle->angle = handlesAngle + Math::PI() * 0.5f;
	mLeftBottomRotateHandle->angle = handlesAngle + Math::PI();
	mRightTopRotateHandle->angle = handlesAngle;
	mRightBottomRotateHandle->angle = handlesAngle + Math::PI() * 1.5f;
	if (mAnchorsFrameEnabled)
	{
		mAnchorsLeftTopHandle->position = Vec2F(0.0f, 1.0f) * (*mAnchorsFrame);
		mAnchorsLeftBottomHandle->position = Vec2F(0.0f, 0.0f) * (*mAnchorsFrame);
		mAnchorsRightTopHandle->position = Vec2F(1.0f, 1.0f) * (*mAnchorsFrame);
		mAnchorsRightBottomHandle->position = Vec2F(1.0f, 0.0f) * (*mAnchorsFrame);
		mAnchorsCenter->position = mAnchorsFrame->origin;
		mAnchorsLeftTopHandle->angle = handlesAngle;
		mAnchorsLeftBottomHandle->angle = handlesAngle + Math::PI() * 0.5f;
		mAnchorsRightTopHandle->angle = handlesAngle + Math::PI() * 1.5f;
		mAnchorsRightBottomHandle->angle = handlesAngle + Math::PI();
		CheckAnchorsCenterEnabled();
	}
	mNeedRedraw = true;
}#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using DynamicCast = std::dynamic_pointer_cast<T>;

class FrameTool
{
public:
	void HandlePressed()
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedObjects().Convert<Basis>(
			[](SceneEditableObject* x) { return x->GetTransform(); });

		mTransformAction = mmake<TransformAction>(o2EditorSceneScreen.GetTopSelectedObjects());

		mBeginDraggingFrame = mFrame;
	}

	void HandleReleased()
	{
		mTransformAction->Completed();
		o2EditorApplication.DoneAction(mTransformAction);
		mTransformAction = nullptr;

		mSnapLines.Clear();
		o2EditorSceneScreen.NeedRedraw();
	}

	Basis GetLeftTopHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin + mFrame.xv;
		Vec2F xn = mFrame.xv.Normalized();
		Vec2F yn = mFrame.yv.Normalized();
		Vec2F correctedPos = position
			- xn*Math::Max(0.0f, xn.Dot(position - correctOrigin) + mFrameMinimalSize)
			+ yn*Math::Max(0.0f, -yn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.origin += frameDeltaX;
		transformedFrame.xv -= frameDeltaX;
		transformedFrame.yv += frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();

			if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
			{
				Vec2F xd = transformedFrame.xv.Normalized()*
					(transformedFrame.yv.Length()*aspect - transformedFrame.xv.Length());

				transformedFrame.origin -= xd;
				transformedFrame.xv += xd;
			}
			else transformedFrame.yv = transformedFrame.yv.Normalized()*transformedFrame.xv.Length()/aspect;
		}

		return transformedFrame;
	}

	Basis GetLeftHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin + mFrame.xv;
		Vec2F xn = mFrame.xv.Normalized();
		Vec2F correctedPos = position
			- xn*Math::Max(0.0f, xn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.5f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);

		transformedFrame.origin += frameDeltaX;
		transformedFrame.xv -= frameDeltaX;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();
			transformedFrame.yv = transformedFrame.yv.Normalized()*transformedFrame.xv.Length()/aspect;
		}

		return transformedFrame;
	}

	Basis GetLeftBottomHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin + mFrame.xv + mFrame.yv;
		Vec2F xn = mFrame.xv.Normalized();
		Vec2F yn = mFrame.yv.Normalized();
		Vec2F correctedPos = position
			- xn*Math::Max(0.0f, xn.Dot(position - correctOrigin) + mFrameMinimalSize)
			- yn*Math::Max(0.0f, yn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mFrame.xv);
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.origin += frameDeltaX + frameDeltaY;
		transformedFrame.xv -= frameDeltaX;
		transformedFrame.yv -= frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();

			if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
			{
				Vec2F xd = transformedFrame.xv.Normalized()*
					(transformedFrame.yv.Length()*aspect - transformedFrame.xv.Length());

				transformedFrame.origin -= xd;
				transformedFrame.xv += xd;
			}
			else
			{
				Vec2F yd = transformedFrame.yv.Normalized()*
					(transformedFrame.xv.Length()/aspect - transformedFrame.yv.Length());

				transformedFrame.origin -= yd;
				transformedFrame.yv += yd;
			}
		}

		return transformedFrame;
	}

	Basis GetTopHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin;
		Vec2F yn = mFrame.yv.Normalized();
		Vec2F correctedPos = position
			+ yn*Math::Max(0.0f, -yn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis transformedFrame = mFrame;
		Vec2F lastHandleCoords = Vec2F(0.5f, 1.0f)*mFrame;
		Vec2F delta = correctedPos - lastHandleCoords;
		Vec2F frameDeltaY = delta.Project(mFrame.yv);

		transformedFrame.yv += frameDeltaY;

		if (o2Input.IsKeyDown(VK_SHIFT))
		{
			float aspect = mBeginDraggingFrame.xv.Length()/mBeginDraggingFrame.yv.Length();
			transformedFrame.xv = transformedFrame.xv.Normalized()*transformedFrame.yv.Length()*aspect;
		}

		return transformedFrame;
	}

	Basis GetBottomHandleTransformed(const Vec2F& position)
	{
		Vec2F correctOrigin = mFrame.origin + mFrame.yv;
		Vec2F yn = mFrame.yv.Normalized();
		Vec2F correctedPos = position
			- yn*Math::Max(0.0f, yn.Dot(position - correctOrigin) + mFrameMinimalSize);

		Basis trans#include <memory>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using WeakRef = std::weak_ptr<T>;

template <typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

class FrameTool
{
private:
    Ref<Basis> mFrame;
    Vec2F mBeginDraggingFrame;
    float mFrameMinimalSize;
    Ref<Basis> mAnchorsFrame;

public:
    FrameTool(const Ref<Basis>& frame, const Vec2F& beginDraggingFrame, float frameMinimalSize, const Ref<Basis>& anchorsFrame)
        : mFrame(frame), mBeginDraggingFrame(beginDraggingFrame), mFrameMinimalSize(frameMinimalSize), mAnchorsFrame(anchorsFrame)
    {}

    Basis GetLeftHandleTransformed(const Vec2F& position) const;

    Basis GetRightTopHandleTransformed(const Vec2F& position) const;

    Basis GetRightHandleTransformed(const Vec2F& position) const;

    Basis GetRightBottomHandleTransformed(const Vec2F& position) const;

    Basis GetLeftTopAnchorHandleTransformed(const Vec2F& position) const;

    Basis GetLeftBottomAnchorHandleTransformed(const Vec2F& position) const;
};

Basis FrameTool::GetLeftHandleTransformed(const Vec2F& position) const
{
    Vec2F correctOrigin = mFrame->origin + mFrame->xv;
    Vec2F xn = mFrame->xv.Normalized();
    Vec2F correctedPos = position
        - xn * Math::Max(0.0f, xn.Dot(position - correctOrigin));

    Basis transformedFrame = *mFrame;
    Vec2F lastHandleCoords = Vec2F(0.0f, 0.5f) * (*mFrame);
    Vec2F delta = correctedPos - lastHandleCoords;
    Vec2F frameDeltaY = delta.Project(mFrame->yv);

    transformedFrame.origin += frameDeltaY;
    transformedFrame.yv -= frameDeltaY;

    if (o2Input.IsKeyDown(VK_SHIFT))
    {
        float aspect = mBeginDraggingFrame.xv.Length() / mBeginDraggingFrame.yv.Length();
        transformedFrame.xv = transformedFrame.xv.Normalized() * transformedFrame.yv.Length() * aspect;
    }

    return transformedFrame;
}

Basis FrameTool::GetRightTopHandleTransformed(const Vec2F& position) const
{
    Vec2F correctOrigin = mFrame->origin;
    Vec2F xn = mFrame->xv.Normalized();
    Vec2F yn = mFrame->yv.Normalized();
    Vec2F correctedPos = position
        + xn * Math::Max(0.0f, -xn.Dot(position - correctOrigin) + mFrameMinimalSize)
        + yn * Math::Max(0.0f, -yn.Dot(position - correctOrigin) + mFrameMinimalSize);

    Basis transformedFrame = *mFrame;
    Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f) * (*mFrame);
    Vec2F delta = correctedPos - lastHandleCoords;
    Vec2F frameDeltaX = delta.Project(mFrame->xv);
    Vec2F frameDeltaY = delta.Project(mFrame->yv);

    transformedFrame.xv += frameDeltaX;
    transformedFrame.yv += frameDeltaY;

    if (o2Input.IsKeyDown(VK_SHIFT))
    {
        float aspect = mBeginDraggingFrame.xv.Length() / mBeginDraggingFrame.yv.Length();

        if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
            transformedFrame.xv = transformedFrame.xv.Normalized() * transformedFrame.yv.Length() * aspect;
        else
            transformedFrame.yv = transformedFrame.yv.Normalized() * transformedFrame.xv.Length() / aspect;
    }

    return transformedFrame;
}

Basis FrameTool::GetRightHandleTransformed(const Vec2F& position) const
{
    Vec2F correctOrigin = mFrame->origin;
    Vec2F xn = mFrame->xv.Normalized();
    Vec2F correctedPos = position
        + xn * Math::Max(0.0f, -xn.Dot(position - correctOrigin) + mFrameMinimalSize);

    Basis transformedFrame = *mFrame;
    Vec2F lastHandleCoords = Vec2F(1.0f, 0.5f) * (*mFrame);
    Vec2F delta = correctedPos - lastHandleCoords;
    Vec2F frameDeltaX = delta.Project(mFrame->xv);

    transformedFrame.xv += frameDeltaX;

    if (o2Input.IsKeyDown(VK_SHIFT))
    {
        float aspect = mBeginDraggingFrame.xv.Length() / mBeginDraggingFrame.yv.Length();
        transformedFrame.yv = transformedFrame.yv.Normalized() * transformedFrame.xv.Length() / aspect;
    }

    return transformedFrame;
}

Basis FrameTool::GetRightBottomHandleTransformed(const Vec2F& position) const
{
    Vec2F correctOrigin = mFrame->origin + mFrame->yv;
    Vec2F xn = mFrame->xv.Normalized();
    Vec2F yn = mFrame->yv.Normalized();
    Vec2F correctedPos = position
        + xn * Math::Max(0.0f, -xn.Dot(position - correctOrigin) + mFrameMinimalSize)
        - yn * Math::Max(0.0f, yn.Dot(position - correctOrigin) + mFrameMinimalSize);

    Basis transformedFrame = *mFrame;
    Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f) * (*mFrame);
    Vec2F delta = correctedPos - lastHandleCoords;
    Vec2F frameDeltaX = delta.Project(mFrame->xv);
    Vec2F frameDeltaY = delta.Project(mFrame->yv);

    transformedFrame.origin += frameDeltaY;
    transformedFrame.xv += frameDeltaX;
    transformedFrame.yv -= frameDeltaY;

    if (o2Input.IsKeyDown(VK_SHIFT))
    {
        float aspect = mBeginDraggingFrame.xv.Length() / mBeginDraggingFrame.yv.Length();

        if (transformedFrame.xv.Length() < transformedFrame.yv.Length())
            transformedFrame.xv = transformedFrame.xv.Normalized() * transformedFrame.yv.Length() * aspect;
        else
        {
            Vec2F yd = transformedFrame.yv.Normalized()*
                (transformedFrame.xv.Length() / aspect - transformedFrame.yv.Length());

            transformedFrame.origin -= yd;
            transformedFrame.yv += yd;
        }
    }

    return transformedFrame;
}

Basis FrameTool::GetLeftTopAnchorHandleTransformed(const Vec2F& position) const
{
    Vec2F correctOrigin = mAnchorsFrame->origin + mAnchorsFrame->xv;
    Vec2F xn = mAnchorsFrame->xv.Normalized();
    Vec2F yn = mAnchorsFrame->yv.Normalized();
    Vec2F correctedPos = position
        - xn * Math::Max(0.0f, xn.Dot(position - correctOrigin))
        + yn * Math::Max(0.0f, -yn.Dot(position - correctOrigin));

    Basis transformedFrame = *mAnchorsFrame;
    Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f) * (*mAnchorsFrame);
    Vec2F delta = correctedPos - lastHandleCoords;
    Vec2F frameDeltaX = delta.Project(mAnchorsFrame->xv);
    Vec2F frameDeltaY = delta.Project(mAnchorsFrame->yv);

    transformedFrame.origin += frameDeltaX;
    transformedFrame.xv -= frameDeltaX;
    transformedFrame.yv += frameDeltaY;

    return transformedFrame;
}

Basis FrameTool::GetLeftBottomAnchorHandleTransformed(const Vec2F& position) const
{
    Vec2F correctOrigin = mAnchorsFrame->origin + mAnchorsFrame->xv + mAnchorsFrame->yv;
    Vec2F xn = mAnchorsFrame->xv.Normalized();
    Vec2F yn = mAnchorsFrame->yv.Normalized();
    Vec2F correctedPos = position
        - xn * Math::Max(0.0f, xn.Dot(position - correctOrigin))
        - yn * Math::Max(0.0f, yn.Dot(position - correctOrigin));

    Basis transformedFrame = *mAnchorsFrame;
    Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f) * (*mAnchorsFrame);
    Vec2F delta = correctedPos - lastHandleCoords;
    Vec2F frameDe#include <memory>
#include <vector>
#include <algorithm>

template <typename T>
class Ref
{
public:
    Ref() = default;
    explicit Ref(T* ptr)
        : mPtr(ptr)
    {
    }

    template<typename U>
    Ref(const Ref<U>& other)
    {
        mPtr = other.Get();
    }

    template<typename U>
    Ref& operator=(const Ref<U>& other)
    {
        mPtr = other.Get();
        return *this;
    }

    T* Get() const
    {
        return mPtr;
    }

    T* operator->() const
    {
        return mPtr;
    }

    T& operator*() const
    {
        return *mPtr;
    }

    operator bool() const
    {
        return mPtr != nullptr;
    }

private:
    T* mPtr = nullptr;
};

template<typename T>
struct DynamicCast
{
    template<typename U>
    static Ref<T> Cast(const Ref<U>& ptr)
    {
        return std::dynamic_pointer_cast<T>(ptr);
    }
};

template <typename T>
Ref<T> mmake()
{
    return Ref<T>(new T());
}

template<typename T>
bool operator==(const Ref<T>& lhs, const Ref<T>& rhs)
{
    return lhs.Get() == rhs.Get();
}

template<typename T>
bool operator!=(const Ref<T>& lhs, const Ref<T>& rhs)
{
    return !(lhs == rhs);
}

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
class WeakRef
{
public:
    WeakRef() = default;
    explicit WeakRef(const Ref<T>& ref)
    {
        mPtr = ref.Get();
    }

    Ref<T> Lock() const
    {
        return Ref<T>(mPtr);
    }

private:
    T* mPtr = nullptr;
};

class Vec2F
{
public:
    Vec2F() = default;
    Vec2F(float x, float y)
        : x(x), y(y)
    {
    }

    float x = 0.0f;
    float y = 0.0f;
};

class Basis
{
public:
    Vec2F origin;
    Vec2F xv;
    Vec2F yv;
};

class Math
{
public:
    static float Max(float a, float b)
    {
        return std::max(a, b);
    }
};

class FrameTool
{
public:
    Ref<Object> GetObjectParentAnchorSnapBasis(const Ref<Object>& object)
    {
        return object->GetEditableParent();
    }

    Vec2F CalculateSnapOffset(const Vec2F& point, const Basis& transformedFrame,
        const std::vector<Vec2F>& point1s, const Vec2F& normal1,
        const std::vector<Vec2F>& point2s, const Vec2F& normal2,
        const std::vector<Ref<Object>>& objectParents)
    {
        // Implementation details not provided
        return Vec2F();
    }

    Ref<Object> mAnchorsFrame;
    // ...

    Ref<Object> GetLeftTopAnchorHandleTransformed(const Vec2F& position)
    {
        Vec2F correctOrigin = mAnchorsFrame->origin;
        Vec2F xn = mAnchorsFrame->xv.Normalized();
        Vec2F yn = mAnchorsFrame->yv.Normalized();
        Vec2F correctedPos = position
            + xn * Math::Max(0.0f, -xn.Dot(position - correctOrigin))
            + yn * Math::Max(0.0f, -yn.Dot(position - correctOrigin));

        Basis transformedFrame = mAnchorsFrame;
        Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f) * mAnchorsFrame;
        Vec2F delta = correctedPos - lastHandleCoords;
        Vec2F frameDeltaX = delta.Project(mAnchorsFrame->xv);
        Vec2F frameDeltaY = delta.Project(mAnchorsFrame->yv);

        transformedFrame.origin += frameDeltaX + frameDeltaY;
        transformedFrame.xv -= frameDeltaX;
        transformedFrame.yv -= frameDeltaY;

        return transformedFrame;
    }

    Basis GetRightTopAnchorHandleTransformed(const Vec2F& position)
    {
        Vec2F correctOrigin = mAnchorsFrame->origin;
        Vec2F xn = mAnchorsFrame->xv.Normalized();
        Vec2F yn = mAnchorsFrame->yv.Normalized();
        Vec2F correctedPos = position
            + xn * Math::Max(0.0f, -xn.Dot(position - correctOrigin))
            + yn * Math::Max(0.0f, -yn.Dot(position - correctOrigin));

        Basis transformedFrame = mAnchorsFrame;
        Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f) * mAnchorsFrame;
        Vec2F delta = correctedPos - lastHandleCoords;
        Vec2F frameDeltaX = delta.Project(mAnchorsFrame->xv);
        Vec2F frameDeltaY = delta.Project(mAnchorsFrame->yv);

        transformedFrame.xv += frameDeltaX;
        transformedFrame.yv += frameDeltaY;

        return transformedFrame;
    }

    Basis GetRightBottomAnchorHandleTransformed(const Vec2F& position)
    {
        Vec2F correctOrigin = mAnchorsFrame->origin + mAnchorsFrame->yv;
        Vec2F xn = mAnchorsFrame->xv.Normalized();
        Vec2F yn = mAnchorsFrame->yv.Normalized();
        Vec2F correctedPos = position
            + xn * Math::Max(0.0f, -xn.Dot(position - correctOrigin))
            - yn * Math::Max(0.0f, yn.Dot(position - correctOrigin));

        Basis transformedFrame = mAnchorsFrame;
        Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f) * mAnchorsFrame;
        Vec2F delta = correctedPos - lastHandleCoords;
        Vec2F frameDeltaX = delta.Project(mAnchorsFrame->xv);
        Vec2F frameDeltaY = delta.Project(mAnchorsFrame->yv);

        transformedFrame.origin += frameDeltaY;
        transformedFrame.xv += frameDeltaX;
        transformedFrame.yv -= frameDeltaY;

        return transformedFrame;
    }

    Basis GetAnchorsCenterHandleTransformed(const Vec2F& position)
    {
        Basis transformedFrame = mAnchorsFrame;
        transformedFrame.origin = position;

        return transformedFrame;
    }

    Vec2F CheckFrameSnapping(const Vec2F& point, const Basis& frame)
    {
        mSnapLines.Clear();

        static std::vector<Vec2F> snapPoints =
        {
            Vec2F(0.0f, 0.0f), Vec2F(0.0f, 0.5f), Vec2F(0.0f, 1.0f),
            Vec2F(0.5f, 0.0f), Vec2F(0.5f, 0.5f), Vec2F(0.5f, 1.0f),
            Vec2F(1.0f, 0.0f), Vec2F(1.0f, 0.5f), Vec2F(1.0f, 1.0f)
        };

        static std::vector<std::vector<std::vector<Vec2F>>> snapPointsLines =
        {
            { { Vec2F(0.0f, 0.0f), Vec2F(0.5f, 0.0f) },{ Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.0f) } },
            { { Vec2F(0.0f, 0.25f), Vec2F(0.0f, 0.75f) },{ Vec2F(0.0f, 0.5f), Vec2F(0.25f, 0.5f) } },
            { { Vec2F(0.0f, 1.0f), Vec2F(0.5f, 1.0f) },{ Vec2F(0.0f, 1.0f), Vec2F(0.0f, 0.5f) } },

            { { Vec2F(0.25f, 0.0f), Vec2F(0.75f, 0.0f) },{ Vec2F(0.5f, 0.0), Vec2F(0.5f, 0.25f) } },
            { { Vec2F(0.25f, 0.5f), Vec2F(0.75f, 0.5f) },{ Vec2F(0.5f, 0.25f), Vec2F(0.5f, 0.75f) } },
            { { Vec2F(0.25f, 1.0f), Vec2F(0.75fRef<Vec2F> FrameTool::CheckAnchorLeftTopSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	auto objects = o2EditorSceneScreen.GetTopSelectedObjects();
	if (objects.Count() > 0 && objects[0]->GetEditableParent())
	{
		Basis transformedFrame = GetLeftTopAnchorHandleTransformed(point);
		Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
												{ Ref<Vec2F>(new Vec2F(0, 0)), Ref<Vec2F>(new Vec2F(0, 1)) }, transformedFrame.xv.Normalized(),
												{ Ref<Vec2F>(new Vec2F(0, 1)), Ref<Vec2F>(new Vec2F(1, 1)) }, transformedFrame.yv.Normalized(),
												{ GetObjectParentAnchorSnapBasis(objects[0]) });

		return snapped;
	}

	return point;
}

Ref<Vec2F> FrameTool::CheckAnchorLeftBottomSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	auto objects = o2EditorSceneScreen.GetTopSelectedObjects();
	if (objects.Count() > 0 && objects[0]->GetEditableParent())
	{
		Basis transformedFrame = GetLeftBottomAnchorHandleTransformed(point);
		Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
												{ Ref<Vec2F>(new Vec2F(0, 0)), Ref<Vec2F>(new Vec2F(0, 1)) }, transformedFrame.xv.Normalized(),
												{ Ref<Vec2F>(new Vec2F(0, 0)), Ref<Vec2F>(new Vec2F(1, 0)) }, transformedFrame.yv.Normalized(),
												{ GetObjectParentAnchorSnapBasis(objects[0]) });

		return snapped;
	}

	return point;
}

Ref<Vec2F> FrameTool::CheckAnchorRightTopSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	auto objects = o2EditorSceneScreen.GetTopSelectedObjects();
	if (objects.Count() > 0 && objects[0]->GetEditableParent())
	{
		Basis transformedFrame = GetRightTopAnchorHandleTransformed(point);
		Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
												{ Ref<Vec2F>(new Vec2F(1, 0)), Ref<Vec2F>(new Vec2F(1, 1)) }, transformedFrame.xv.Normalized(),
												{ Ref<Vec2F>(new Vec2F(0, 1)), Ref<Vec2F>(new Vec2F(1, 1)) }, transformedFrame.yv.Normalized(),
												{ GetObjectParentAnchorSnapBasis(objects[0]) });

		return snapped;
	}

	return point;
}

Ref<Vec2F> FrameTool::CheckAnchorRightBottomSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	auto objects = o2EditorSceneScreen.GetTopSelectedObjects();
	if (objects.Count() > 0 && objects[0]->GetEditableParent())
	{
		Basis transformedFrame = GetRightBottomAnchorHandleTransformed(point);
		Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
												{ Ref<Vec2F>(new Vec2F(1, 0)), Ref<Vec2F>(new Vec2F(1, 1)) }, transformedFrame.xv.Normalized(),
												{ Ref<Vec2F>(new Vec2F(0, 0)), Ref<Vec2F>(new Vec2F(1, 0)) }, transformedFrame.yv.Normalized(),
												{ GetObjectParentAnchorSnapBasis(objects[0]) });

		return snapped;
	}

	return point;
}

Ref<Vec2F> FrameTool::CheckTopSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	Basis transformedFrame = GetTopHandleTransformed(point);
	Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
											{}, transformedFrame.xv.Normalized(),
											{ Ref<Vec2F>(new Vec2F(0, 1)), Ref<Vec2F>(new Vec2F(1, 1)) }, transformedFrame.yv.Normalized(),
											GetSnapBasisesForAllObjects());

	return snapped;
}

Ref<Vec2F> FrameTool::CheckBottomSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	Basis transformedFrame = GetBottomHandleTransformed(point);
	Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
											{}, transformedFrame.xv.Normalized(),
											{ Ref<Vec2F>(new Vec2F(0, 0)), Ref<Vec2F>(new Vec2F(1, 0)) }, transformedFrame.yv.Normalized(),
											GetSnapBasisesForAllObjects());

	return snapped;
}

Ref<Vec2F> FrameTool::CheckLeftSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	Basis transformedFrame = GetLeftHandleTransformed(point);
	Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
											{ Ref<Vec2F>(new Vec2F(0, 0)), Ref<Vec2F>(new Vec2F(0, 1)) }, transformedFrame.xv.Normalized(),
											{}, transformedFrame.yv.Normalized(),
											GetSnapBasisesForAllObjects());

	return snapped;
}

Ref<Vec2F> FrameTool::CheckRightSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	Basis transformedFrame = GetRightHandleTransformed(point);
	Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
											{ Ref<Vec2F>(new Vec2F(1, 0)), Ref<Vec2F>(new Vec2F(1, 1)) }, transformedFrame.xv.Normalized(),
											{}, transformedFrame.yv.Normalized(),
											GetSnapBasisesForAllObjects());

	return snapped;
}

Ref<Vec2F> FrameTool::CheckLeftTopSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	Basis transformedFrame = GetLeftTopHandleTransformed(point);
	Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
											{ Ref<Vec2F>(new Vec2F(0, 0)), Ref<Vec2F>(new Vec2F(0, 1)) }, transformedFrame.xv.Normalized(),
											{ Ref<Vec2F>(new Vec2F(0, 1)), Ref<Vec2F>(new Vec2F(1, 1)) }, transformedFrame.yv.Normalized(),
											GetSnapBasisesForAllObjects());

	return snapped;
}

Ref<Vec2F> FrameTool::CheckLeftBottomSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	Basis transformedFrame = GetLeftBottomHandleTransformed(point);
	Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
											{ Ref<Vec2F>(new Vec2F(0, 0)), Ref<Vec2F>(new Vec2F(0, 1)) }, transformedFrame.xv.Normalized(),
											{ Ref<Vec2F>(new Vec2F(0, 0)), Ref<Vec2F>(new Vec2F(1, 0)) }, transformedFrame.yv.Normalized(),
											GetSnapBasisesForAllObjects());

	return snapped;
}

Ref<Vec2F> FrameTool::CheckRightTopSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	Basis transformedFrame = GetRightTopHandleTransformed(point);
	Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
											{ Ref<Vec2F>(new Vec2F(1, 0)), Ref<Vec2F>(new Vec2F(1, 1)) }, transformedFrame.xv.Normalized(),
											{ Ref<Vec2F>(new Vec2F(0, 1)), Ref<Vec2F>(new Vec2F(1, 1)) }, transformedFrame.yv.Normalized(),
											GetSnapBasisesForAllObjects());

	return snapped;
}

Ref<Vec2F> FrameTool::CheckRightBottomSnapping(const Ref<const Vec2F>& point)
{
	mSnapLines.Clear();

	Basis transformedFrame = GetRightBottomHandleTransformed(point);
	Ref<Vec2F> snapped = CalculateSnapOffset(point, transformedFrame,
											{ Ref<Vec2F>(new Vec2F(1, 0)), Ref<Vec2F>(new Vec2F(1, 1)) }, transformedFrame.xv.Normalized(),
											{ Ref<Vec2F>(new Vec2F(0, 0)), Ref<Vec2F>(new Vec2F(1, 0)) }, transformedFrame.yv.Normalized(),
											GetSnapBasisesForAllObjects());

	return snapped;
}#include <memory>
#include <vector>
using namespace std;

template <typename T>
using Ref = shared_ptr<T>;

class Camera {};

class Vec2F {};

class Basis {};

class Widget {};

class SceneEditableObject {
    public:
    bool IsOnScene() {return true;}
    Basis GetTransform() {return Basis();}
    SceneEditableObject* GetEditableParent();
};

class o2Scene {
    public:
    Vector<SceneEditableObject*> GetAllEditableObjects();
};

class o2EditorSceneScreen {
    public:
    Camera GetCamera();
    Vec2F ScreenToLocalPoint(const Vec2F& point);
    Vec2F GetCameraScale();
    const Vector<SceneEditableObject*>& GetSelectedObjects();
} o2EditorSceneScreen;

class FrameTool {
    Ref<void> mAnchorsFrame;
    bool mAnchorsFrameEnabled;
    float mAnchorsFrameLengthThreshold;
    
    public:
    Vector<Basis> GetObjectsTransforms(const Vector<SceneEditableObject*>& objects) const;
    Vector<Basis> GetSnapBasisesForAllObjects() const;
    Vec2F CalculateSnapOffset(const Vec2F& point, const Basis& frame, const Vector<Vec2F>& xLines, const Vec2F& xNormal, const Vector<Vec2F>& yLines, const Vec2F& yNormal, Vector<Basis> basises);
};

Ref<SceneEditableObject> SceneEditableObject::GetEditableParent() {
    return static_pointer_cast<SceneEditableObject>(shared_from_this());
}

Camera o2EditorSceneScreen::GetCamera() {
    return Camera();
}

Vec2F o2EditorSceneScreen::ScreenToLocalPoint(const Vec2F& point) {
    return Vec2F();
}

Vec2F o2EditorSceneScreen::GetCameraScale() {
    return Vec2F();
}

const Vector<SceneEditableObject*>& o2EditorSceneScreen::GetSelectedObjects() {
    static Vector<SceneEditableObject*> objects;
    return objects;
}

Vector<SceneEditableObject*> o2Scene::GetAllEditableObjects() {
    static Vector<SceneEditableObject*> objects;
    return objects;
}

Vector<Basis> FrameTool::GetObjectsTransforms(const Vector<SceneEditableObject*>& objects) const {
    Vector<Basis> res;
    for (const Ref<SceneEditableObject>& object : objects) {
        if (!object->IsOnScene())
            continue;

        if (o2EditorSceneScreen.GetSelectedObjects().Contains(object.get()))
            continue;

        res.push_back(object->GetTransform());
    }

    return res;
}

Vector<Basis> FrameTool::GetSnapBasisesForAllObjects() const {
    auto snapBasises = GetObjectsTransforms(o2Scene.GetAllEditableObjects());

    if (mAnchorsFrameEnabled)
        snapBasises.push_back(mAnchorsFrame);

    return snapBasises;
}

Vec2F FrameTool::CalculateSnapOffset(const Vec2F& point, const Basis& frame, const Vector<Vec2F>& xLines, const Vec2F& xNormal, const Vector<Vec2F>& yLines, const Vec2F& yNormal, Vector<Basis> basises) {
    const float pxThreshold = 5.0f;
    const float sameSnapDistanceThreshold = 0.01f;
    const Camera& camera = o2EditorSceneScreen.GetCamera();

    Vector<Vector<Vec2F>> snapLines = {
        { Vec2F(0.0f, 0.0f), Vec2F(1.0f, 0.0f) },
        { Vec2F(1.0f, 0.0f), Vec2F(1.0f, 1.0f) },
        { Vec2F(1.0f, 1.0f), Vec2F(0.0f, 1.0f) },
        { Vec2F(0.0f, 1.0f), Vec2F(0.0f, 0.0f) },
        { Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f) },
        { Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f) }
    };

    Vector<Vec2F> worldSnapLines;

    for (const Basis& basis : basises) {
        for (const Vector<Vec2F>& snapLine : snapLines) {
            Vec2F objectLineBegin = snapLine[0] * basis;
            Vec2F objectLineEnd = snapLine[1] * basis;
            Vec2F objectLineVec = objectLineEnd - objectLineBegin;

            if (objectLineVec.SqrLength() < 0.1f)
                continue;

            worldSnapLines.push_back(objectLineBegin);
            worldSnapLines.push_back(objectLineEnd);
        }
    }

    Vec2F offset;
    Vector<Vec2F> localSnapLines;

    for (int i = 0; i < 2; i++) {
        const Vector<Vec2F>& currentAxisLines = i ? xLines : yLines;

        const Vec2F& currentNormal = i ? xNormal : yNormal;

        for (size_t j = 0; j < currentAxisLines.size(); ++j) {
            Vec2F currentAxisLineBegin;
            Vec2F currentAxisLineEnd;

            if (j == 0) {
                currentAxisLineBegin = currentAxisLines[j];
                currentAxisLineEnd = currentAxisLines[currentAxisLines.size() - 1];
            }
            else {
                currentAxisLineBegin = currentAxisLines[j - 1];
                currentAxisLineEnd = currentAxisLines[j];
            }

            const Vector<Vec2F>& currentSnapLines = i ? worldSnapLines : localSnapLines;

            for (size_t p = 1; p < currentSnapLines.size(); p += 2) {
                Vec2F objectLineBegin = currentSnapLines[p - 1];
                Vec2F objectLineEnd = currentSnapLines[p];
                Vec2F objectLineVec = objectLineEnd - objectLineBegin;

                if (objectLineVec.SqrLength() < 0.1f)
                    continue;

                Vec2F localLineVec = currentAxisLineEnd - currentAxisLineBegin;

                if (localLineVec.SqrLength() < 0.1f || objectLineVec.SqrLength() < 0.1f)
                    continue;

                auto o2EditorSceneScreenPosToLocal = [&](const Vec2F& pos) -> Vec2F {
                    return pos;
                };

                Vec2F currentLineOffset = o2EditorSceneScreenPosToLocal(point) - objectLineBegin;

                {
                    Vec2F objectLineNormal = objectLineVec;
                    float objectLineNormalLen = objectLineNormal.Normalize();

                    float dot = fabs(LocalDot(objectLineNormal, currentNormal));
                    float val = fabs(LocalDot(normalized(localLineVec), normalized(objectLineNormal)));

                    if (dot > 0.9f && val > 0.9f) {
                        float objectLineDotCurrent = LocalDot(currentLineOffset, objectLineNormal);
                        float objectLineVecDotCurrent = LocalDot(objectLineVec, objectLineNormal);

                        float objectLineOffsetProjectMult = objectLineDotCurrent / objectLineVecDotCurrent;

                        bool offsetCheck;

                        if (fabs(objectLineVecDotCurrent) < 0.1f) {
                            offsetCheck = fabs(objectLineDotCurrent) < pxThreshold;
                        }
                        else {
                            offsetCheck = objectLineOffsetProjectMult >= 0.0f && objectLineOffsetProjectMult <= 1.0f && fabs(objectLineDotCurrent) < pxThreshold;
                        }

                        if (offsetCheck) {
                            Vec2F localOffset = o2EditorSceneScreenPosToLocal(objectLineBegin + objectLineVec * objectLineOffsetProjectMult);

                            if (offset.SqrLength() == 0.0f || (localOffset - o2EditorSceneScreenPosToLocal(objectLineBegin)).Length() < (offset - o2EditorSceneScreenPosToLocal(objectLineBegin)).Length())
                                offset = localOffset - localOffset;
                        }
                    }
                }

                {
                    Vec2F objectLineNormal = objectLineVec;
                    float objectLineNormalLen = objectLineNormal.Normalize();

                    float dot = fabs(LocalDot(objectLineNormal, currentNormal));
                    float val = fabs(LocalDot(normalized(localLineVec), normalized(objectLineNormal)));

                    if (dot < 0.1f && val > 0.9f) {
                        float objectLineDotCurrent = LocalDot(currentLineOffset, objectLineNormal);
                        float objectLineVecDotCurrent = LocalDot(objectLineVec, objectLineNormal);

                        float objectLineOffsetProjectMult = objectLineDotCurrent / objectLineVecDotCurrent;

                        bool offsetCheck;

                        if (fabs(objectLineVecDotCurrent) < 0.1f) {
                            offsetCheck = fabs(objectLineDotCurrent) < pxThreshold;
                        }
                        else {
                            offsetCheck = objectLineOffsetProjectMult >= 0.0f && objectLineOffsetProjectMult <= 1.0f && fabs(objectLineDotCurrent) < pxThreshold;
                        }

                        if (offsetCheck) {
                            Vec2F localOffset = o2EditorSceneScreenPosToLocal(objectLineBegin + objectLineVec * objectLineOffsetProjectMult);

                            if (offset.SqrLength() == 0.0f || (localOffset - o2EditorSceneScreenPosToLocal(objectLineBegin)).Length() < (offset - o2EditorSceneScreenPosToLocal(objectLineBegin)).Length())
                                offset = localOffset - localOffset;
                        }
                    }
                }
            }
        }
    }

    return offset;
}#include <memory>

template<typename T>
class Ref {
public:
    Ref(T* ptr) : m_ptr(ptr) {}
    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }

private:
    T* m_ptr;
};

template<typename T>
class WeakRef {
public:
    WeakRef(T* ptr) : m_ptr(ptr) {}
    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }

private:
    T* m_ptr;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
    return Ref<T>(new T(std::forward<Args>(args)...));
}

class Editor__FrameTool : public Editor::FrameTool {
public:
    Vec2F CalculateSnapLine(Vec2F point, Basis frame, const Vector<Ref<Transform>>& objects, const Vector<Vec2F>& worldSnapLines, const Vector<Vec2F>& xLines, const Vector<Vec2F>& yLines, const Vector<float>& zLines, float pxThreshold, float sameSnapDistanceThreshold, Color mSnapLinesColor, const Camera& camera) {
        Vec2F offset;
        Vector<Vec2F> localSnapLines;

        for (int i = 0; i < 2; ++i) {
            Vector<Vec2F> currentAxisLines = i == 0 ? xLines : yLines;
            Vec2F currentAxisNormal = i == 0 ? xNormal : yNormal;

            bool axisSnapLineFound = false;
            Vec2F axisSnapOffset;
            float axisSnapDistance = FLT_MAX;
            Vec2F axisSnapBegin, axisSnapEnd;

            for (int j = 0; j < currentAxisLines.Count(); j += 2) {
                Vec2F lineBeg = currentAxisLines[j] * frame;
                Vec2F lineEnd = currentAxisLines[j + 1] * frame;
                Vec2F lineVec = lineEnd - lineBeg;

                for (int k = 0; k < worldSnapLines.Count(); k += 2) {
                    Vec2F objectLineBegin = worldSnapLines[k];
                    Vec2F objectLineEnd = worldSnapLines[k + 1];
                    Vec2F objectLineVec = objectLineEnd - objectLineBegin;

                    if (!lineVec.IsParallel(objectLineVec))
                        continue;

                    float projDistance = (objectLineBegin - lineBeg).Dot(currentAxisNormal);
                    float screenProjDistance = Math::Abs(projDistance / camera.GetScale().x);

                    if (screenProjDistance < pxThreshold && screenProjDistance < axisSnapDistance) {
                        axisSnapOffset = currentAxisNormal * projDistance;
                        axisSnapDistance = screenProjDistance;
                        axisSnapBegin = currentAxisLines[j];
                        axisSnapEnd = currentAxisLines[j + 1];

                        axisSnapLineFound = true;
                    }
                }
            }

            if (!axisSnapLineFound)
                continue;

            for (int j = 0; j < currentAxisLines.Count(); j += 2) {
                Vec2F lineBeg = currentAxisLines[j] * frame + axisSnapOffset;
                Vec2F lineEnd = currentAxisLines[j + 1] * frame + axisSnapOffset;
                Vec2F lineVec = lineEnd - lineBeg;

                bool found = false;

                for (int k = 0; k < worldSnapLines.Count(); k += 2) {
                    Vec2F objectLineBegin = worldSnapLines[k];
                    Vec2F objectLineEnd = worldSnapLines[k + 1];
                    Vec2F objectLineVec = objectLineEnd - objectLineBegin;

                    if (!lineVec.IsParallel(objectLineVec))
                        continue;

                    float projDistance = Math::Abs((objectLineBegin - lineBeg).Dot(currentAxisNormal));

                    if (projDistance < sameSnapDistanceThreshold) {
                        mSnapLines.Add(SnapLine(objectLineBegin, objectLineEnd, mSnapLinesColor));

                        found = true;
                    }
                }

                if (found) {
                    localSnapLines.Add(currentAxisLines[j]);
                    localSnapLines.Add(currentAxisLines[j + 1]);
                }
            }

            offset += axisSnapOffset;
        }

        Basis frameWithOffset = frame;
        frameWithOffset.Translate(offset);

        for (int i = 0; i < localSnapLines.Count(); i += 2)
            mSnapLines.Add(SnapLine(localSnapLines[i] * frameWithOffset, localSnapLines[i + 1] * frameWithOffset, mSnapLinesColor));

        return point + offset;
    }
};

// --- META ---

DECLARE_CLASS(Editor::FrameTool, Editor__FrameTool);

// --- END META ---