#include "o2Editor/stdafx.h"
#include "SplineEditor.h"

namespace Editor
{
	SplineEditor::SplineEditor()
	{
		mSelectionSprite = std::make_unique<Sprite>("ui/UI_Window_place.png");

		mHandlesSample.position = std::make_unique<DragHandle>(
			std::make_unique<Sprite>("ui/CurveHandle.png"),
			std::make_unique<Sprite>("ui/CurveHandleHover.png"),
			std::make_unique<Sprite>("ui/CurveHandlePressed.png"),
			std::make_unique<Sprite>("ui/CurveHandleSelected.png")
		);

		mHandlesSample.prevSupport = std::make_unique<DragHandle>(
			std::make_unique<Sprite>("ui/CurveSupportHandle.png"),
			std::make_unique<Sprite>("ui/CurveSupportHandleHover.png"),
			std::make_unique<Sprite>("ui/CurveSupportHandlePressed.png"),
			std::make_unique<Sprite>("ui/CurveSupportHandleSelected.png")
		);

		mHandlesSample.nextSupport = std::make_unique<DragHandle>(
			std::make_unique<Sprite>("ui/CurveSupportHandle.png"),
			std::make_unique<Sprite>("ui/CurveSupportHandleHover.png"),
			std::make_unique<Sprite>("ui/CurveSupportHandlePressed.png"),
			std::make_unique<Sprite>("ui/CurveSupportHandleSelected.png")
		);

		mSplineColor = Color4(44, 62, 80, 255);
		mSplineSupportColor = Color4(190, 190, 190, 255);

		mTransformFrame.SetPivotEnabled(false);
		mTransformFrame.SetRotationEnabled(false);
		mTransformFrame.onTransformed = THIS_FUNC(OnTransformFrameTransformed);
		mTransformFrame.onPressed = THIS_FUNC(OnTransformBegin);
		mTransformFrame.onChangeCompleted = THIS_FUNC(OnTransformCompleted);
		mTransformFrame.isInputTransparent = true;
	}

	SplineEditor::~SplineEditor()
	{
		mSelectionSprite = nullptr;

		if (mSplineWrapper)
			mSplineWrapper = nullptr;

		auto splineHandles = mSplineHandles;
		mSplineHandles.clear();

		for (auto handles : splineHandles)
			handles = nullptr;

		mSplineHandles.clear();
	}

	void SplineEditor::Draw()
	{
		if (!mSplineWrapper)
			return;

		OnDrawn();

		auto drawPoints = mSplineWrapper->GetDrawPoints();
		o2Render.DrawAALine(drawPoints, mSplineColor);

		DrawTransformFrame();
		DrawHandles();
		DrawSelection();
	}

	void SplineEditor::DrawHandles()
	{
		for (auto& handles : mSplineHandles)
		{
			if (handles->IsSupportsVisible())
			{
				handles->prevSupport->UpdateScreenPosition();
				handles->nextSupport->UpdateScreenPosition();

				bool broken = (o2Input.IsKeyDown(VK_MENU) || handles->startDragFromZero) && o2Input.IsCursorDown();

				if (!handles->isFirst || mSplineWrapper->IsClosed())
				{
					o2Render.DrawAALine(handles->position->GetScreenPosition(), handles->prevSupport->GetScreenPosition(),
					                     mSplineSupportColor, 1.0f, broken ? LineType::Dash : LineType::Solid);
				}

				if (!handles->isLast || mSplineWrapper->IsClosed())
				{
					o2Render.DrawAALine(handles->position->GetScreenPosition(), handles->nextSupport->GetScreenPosition(),
					                     mSplineSupportColor, 1.0f, broken ? LineType::Dash : LineType::Solid);
				}
			}
		}

		if (o2Input.IsKeyDown(VK_MENU))
		{
			DrawMainHandles();
			DrawSupportHandles();
		}
		else
		{
			DrawSupportHandles();
			DrawMainHandles();
		}
	}

	void SplineEditor::DrawSelection()
	{
		if (mIsPressed)
		{
			mSelectionSprite->rect = RectF(mSplineWrapper->LocalToWorld(mSelectingPressedPoint), o2Input.cursorPos);
			mSelectionSprite->Draw();
		}
	}

	void SplineEditor::DrawTransformFrame()
	{
		if (!mTransformFrameVisible)
			return;

		Vec2F worldOrig = mSplineWrapper->LocalToWorld(mTransformFrameBasis.origin);
		Vec2F worldXV = mSplineWrapper->LocalToWorld(mTransformFrameBasis.xv + mTransformFrameBasis.origin) - worldOrig;
		Vec2F worldYV = mSplineWrapper->LocalToWorld(mTransformFrameBasis.yv + mTransformFrameBasis.origin) - worldOrig;
		mTransformFrame.SetBasis(Basis(worldOrig - mTransformBasisOffet,
		                              worldXV + Vec2F(mTransformBasisOffet.x * 2.0f, 0),
		                              worldYV + Vec2F(0, mTransformBasisOffet.y * 2.0f)));

		mTransformFrame.Draw();
	}

	void SplineEditor::DrawMainHandles()
	{
		for (auto& handles : mSplineHandles)
			handles->position->Draw();
	}

	void SplineEditor::DrawSupportHandles()
	{
		for (auto& handles : mSplineHandles)
		{
			if (handles->IsSupportsVisible())
			{
				if (!handles->isFirst || mSplineWrapper->IsClosed())
					handles->prevSupport->Draw();

				if (!handles->isLast || mSplineWrapper->IsClosed())
					handles->nextSupport->Draw();
			}
		}
	}

	void SplineEditor::Update(float dt)
	{
	}

	void SplineEditor::SetSpline(const Ref<ISplineWrapper>& wrapper)
	{
		mSplineWrapper = wrapper;
		if (!mSplineWrapper)
			return;

		InitializeHandles();
	}

	void SplineEditor::Reset()
	{
		SetSpline(nullptr);
		onChanged.Clear();
	}

	bool SplineEditor::IsUnderPoint(const Vec2F& point)
	{
		return true;
	}

	bool SplineEditor::IsInputTransparent() const
	{
		return true;
	}

	void SplineEditor::ClearHandles()
	{
		mSplineHandles.clear();
	}

	void SplineEditor::OnSplineChanged()
	{
		ClearHandles();
		InitializeHandles();
	}

	void SplineEditor::InitializeHandles()
	{
		if (!mSplineWrapper)
			return;

		for (int i = 0; i < mSplineWrapper->GetPointsCount(); ++i)
		{
			auto& position = mSplineWrapper->GetPositionAt(i);
			auto& prevSupport = mSplineWrapper->GetPrevSupportAt(i);
			auto& nextSupport = mSplineWrapper->GetNextSupportAt(i);

			auto handles = std::make_unique<SplineHandles>(position.get(), prevSupport.get(), nextSupport.get());

			handles->SetPositionCallback([&](const Vec2F& position)
				{
					mSplineWrapper->SetPositionAt(i, position);
				});

			handles->SetPrevSupportCallback([&](const Vec2F& support)
				{
					mSplineWrapper->SetPrevSupportAt(i, support);
				});

			handles->SetNextSupportCallback([&](const Vec2F& support)
				{
					mSplineWrapper->SetNextSupportAt(i, support);
				});

			mSplineHandles.push_back(std::move(handles));
		}
	}
}replace raw pointers with Ref<>, except void* pointers. Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Dont replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). /nt(); i++)
		{
			Ref<PointHandles> handles = mmake<PointHandles>();

			handles->isFirst = i == 0;
			handles->isLast = i == mSplineWrapper->GetPointsCount() - 1;

			handles->position = mHandlesSample.position;
			handles->position.SetPosition(mSplineWrapper->GetPointPos(i));
			handles->position.SetSelectionGroup(this);
			handles->position.onPressed = [=]() { handles->positionDragged = false; };
			handles->position.onBeganDragging = [=]() { handles->positionDragged = true; };
			handles->position.onReleased = [=]() { if (!handles->positionDragged) OnMainHandleReleasedNoDrag(i, handles); };
			handles->position.onChangedPos = [=](const Vec2F& pos) { OnMainHandleMoved(i, pos, handles); };
			handles->position.localToScreenTransformFunc = [&](const Vec2F& p) { return mSplineWrapper->LocalToWorld(p); };
			handles->position.screenToLocalTransformFunc = [&](const Vec2F& p) { return mSplineWrapper->WorldToLocal(p); };

			handles->prevSupport = mHandlesSample.prevSupport;
			handles->prevSupport.SetPosition(mSplineWrapper->GetPointPrevSupportPos(i));
			handles->prevSupport.SetSelectionGroup(&mSupportHandlesGroup);
			handles->prevSupport.onChangedPos = [=](const Vec2F& pos) { OnPrevHandleMoved(i, pos, handles); };
			handles->prevSupport.onPressed = [=]() { handles->prevSupportDragged = false; CheckDragFromZero(i, handles); };
			handles->prevSupport.onBeganDragging = [=]() { handles->prevSupportDragged = true; };
			handles->prevSupport.onReleased = [=]() { if (!handles->prevSupportDragged) OnPrevHandleReleasedNoDrag(i, handles); };
			handles->prevSupport.localToScreenTransformFunc = [&](const Vec2F& p) { return mSplineWrapper->LocalToWorld(p); };
			handles->prevSupport.screenToLocalTransformFunc = [&](const Vec2F& p) { return mSplineWrapper->WorldToLocal(p); };

			handles->nextSupport = mHandlesSample.nextSupport;
			handles->nextSupport.SetPosition(mSplineWrapper->GetPointNextSupportPos(i));
			handles->nextSupport.SetSelectionGroup(&mSupportHandlesGroup);
			handles->nextSupport.onChangedPos = [=](const Vec2F& pos) { OnNextHandleMoved(i, pos, handles); };
			handles->nextSupport.onPressed = [=]() { handles->nextSupportDragged = false; CheckDragFromZero(i, handles); };
			handles->nextSupport.onBeganDragging = [=]() { handles->nextSupportDragged = true; };
			handles->nextSupport.onReleased = [=]() { if (!handles->nextSupportDragged) OnNextHandleReleasedNoDrag(i, handles); };
			handles->nextSupport.localToScreenTransformFunc = [&](const Vec2F& p) { return mSplineWrapper->LocalToWorld(p); };
			handles->nextSupport.screenToLocalTransformFunc = [&](const Vec2F& p) { return mSplineWrapper->WorldToLocal(p); };

			mSplineHandles.Add(handles);
		}
	}

	void SplineEditor::OnPrevHandleMoved(int i, const Vec2F& pos, Ref<PointHandles> handles)
	{
		Vec2F prev = mSplineWrapper->GetPointPrevSupportPos(i);
		Vec2F curr = mSplineWrapper->GetPointPos(i);
		Vec2F next = mSplineWrapper->GetPointNextSupportPos(i);

		if (!o2Input.IsKeyDown(VK_MENU) || handles->startDragFromZero)
		{
			float distanceDiff = (pos - curr).Length() - (prev - curr).Length();
			float nextDistance = (next - curr).Length();
			Vec2F symmetricNextPos = (curr - pos).Normalized()*(nextDistance + distanceDiff) + curr;
			mSplineWrapper->SetPointNextSupportPos(i, symmetricNextPos);

			handles->nextSupport.SetPosition(mSplineWrapper->GetPointNextSupportPos(i));
		}

		mSplineWrapper->SetPointPrevSupportPos(i, pos);
		mSplineWrapper->OnChanged();
		onChanged();
	}

	void SplineEditor::OnNextHandleMoved(int i, const Vec2F& pos, Ref<PointHandles> handles)
	{
		Vec2F prev = mSplineWrapper->GetPointPrevSupportPos(i);
		Vec2F curr = mSplineWrapper->GetPointPos(i);
		Vec2F next = mSplineWrapper->GetPointNextSupportPos(i);

		if (!o2Input.IsKeyDown(VK_MENU) || handles->startDragFromZero)
		{
			float distanceDiff = (pos - curr).Length() - (next - curr).Length();
			float prevDistance = (prev - curr).Length();
			Vec2F symmetricPrevPos = (curr - pos).Normalized()*(prevDistance + distanceDiff) + curr;
			mSplineWrapper->SetPointPrevSupportPos(i, symmetricPrevPos);

			handles->prevSupport.SetPosition(mSplineWrapper->GetPointPrevSupportPos(i));
		}

		mSplineWrapper->SetPointNextSupportPos(i, pos);
		mSplineWrapper->OnChanged();
		onChanged();
	}

	void SplineEditor::OnMainHandleMoved(int i, const Vec2F& pos, Ref<PointHandles> handles)
	{
		mSplineWrapper->SetPointPos(i, pos);
		handles->prevSupport.SetPosition(mSplineWrapper->GetPointPrevSupportPos(i));
		handles->nextSupport.SetPosition(mSplineWrapper->GetPointNextSupportPos(i));

		UpdateTransformFrame();
		mSplineWrapper->OnChanged();
		onChanged();
	}

	void SplineEditor::OnPrevHandleReleasedNoDrag(int i, Ref<PointHandles> handles)
	{
		if (o2Input.IsKeyDown(VK_MENU))
		{
			mSplineWrapper->SetPointPrevSupportPos(i, mSplineWrapper->GetPointPos(i));
			handles->prevSupport.SetPosition(mSplineWrapper->GetPointPrevSupportPos(i));

			UpdateTransformFrame();
			mSplineWrapper->OnChanged();
			onChanged();
		}
	}

	void SplineEdRef<>::OnNextHandleReleasedNoDrag(const int i, Ref<PointHandles>& handles)
{
	if (o2Input.IsKeyDown(VK_MENU))
	{
		mSplineWrapper->SetPointNextSupportPos(i, mSplineWrapper->GetPointPos(i));
		handles->nextSupport.SetPosition(mSplineWrapper->GetPointNextSupportPos(i));

		UpdateTransformFrame();
		mSplineWrapper->OnChanged();
		onChanged();
	}
}

void SplineEditor::OnMainHandleReleasedNoDrag(const int i, Ref<PointHandles>& handles)
{
	if (o2Input.IsKeyDown(VK_MENU))
	{
		mSplineWrapper->SetPointNextSupportPos(i, mSplineWrapper->GetPointPos(i));
		mSplineWrapper->SetPointPrevSupportPos(i, mSplineWrapper->GetPointPos(i));

		handles->nextSupport.SetPosition(mSplineWrapper->GetPointNextSupportPos(i));
		handles->prevSupport.SetPosition(mSplineWrapper->GetPointPrevSupportPos(i));

		UpdateTransformFrame();
		mSplineWrapper->OnChanged();
		onChanged();
	}
}

void SplineEditor::OnCursorDblClicked(const Input::Cursor& cursor)
{
	const float createPointDistanceThreshold = 7.0f;

	Vec2F localCursorPos = mSplineWrapper->WorldToLocal(cursor.position);
	Vec2F newPointPosition = localCursorPos;
	int newPointIdx = Math::Max(0, mSplineWrapper->GetPointsCount());

	for (int i = 1; i < mSplineWrapper->GetPointsCount(); i++)
	{
		bool found = false;

		const ApproximationVec2F* points = mSplineWrapper->GetPointApproximation(i);
		for (int j = 1; j < mSplineWrapper->GetPointApproximationCount(i); j++)
		{
			Vec2F a = mSplineWrapper->LocalToWorld(points[j - 1].value);
			Vec2F b = mSplineWrapper->LocalToWorld(points[j].value);

			Vec2F ab = b - a;
			float abl = ab.Length();
			Vec2F abn = ab/abl;
			Vec2F abnp = abn.Perpendicular();
			Vec2F ac = cursor.position - a;

			float pointDistance = (b - cursor.position).Length();
			float lineDistance = Math::Abs(abnp.Dot(ac));
			float proj = abn.Dot(ac);

			if (pointDistance < createPointDistanceThreshold ||
				(lineDistance < createPointDistanceThreshold && proj > 0.0f && proj < abl))
			{
				newPointPosition = mSplineWrapper->WorldToLocal(abnp*(abnp.Dot(ac)) + cursor.position);
				newPointIdx = i;

				found = true;
				break;
			}
		}

		if (found)
			break;
	}

	mSplineWrapper->AddPoint(newPointIdx, newPointPosition, Vec2F(), Vec2F());
	OnSplineChanged();
	mSplineWrapper->OnChanged();
	onChanged();
}

void SplineEditor::OnCursorPressed(const Input::Cursor& cursor)
{
	mSelectingPressedPoint = mSplineWrapper->WorldToLocal(cursor.position);

	if (!o2Input.IsKeyDown(VK_CONTROL))
	{
		DeselectAll();
		mSupportHandlesGroup.DeselectAll();
	}
}

void SplineEditor::OnCursorReleased(const Input::Cursor& cursor)
{
	for (auto handle : mSelectingHandlesBuf)
	{
		SetHandleSelectedState(handle, false);
		handle->SetSelected(true);
	}

	mSelectingHandlesBuf.Clear();
	UpdateTransformFrame();
}

void SplineEditor::OnCursorStillDown(const Input::Cursor& cursor)
{
	for (auto handle : mSelectingHandlesBuf)
		SetHandleSelectedState(handle, false);

	mSelectingHandlesBuf.Clear();

	RectF selectionLocalRect(mSelectingPressedPoint, mSplineWrapper->WorldToLocal(cursor.position));

	for (auto handle : mHandles)
	{
		if (handle->IsEnabled() && selectionLocalRect.IsInside(handle->GetPosition()) &&
			!mSelectedHandles.Contains(handle))
		{
			mSelectingHandlesBuf.Add(handle);
			SetHandleSelectedState(handle, true);
		}
	}

	for (auto handle : mSupportHandlesGroup.GetAllHandles())
	{
		if (handle->IsEnabled() && selectionLocalRect.IsInside(handle->GetPosition()) &&
			!mSupportHandlesGroup.GetSelectedHandles().Contains(handle))
		{
			mSelectingHandlesBuf.Add(handle);
			SetHandleSelectedState(handle, true);
		}
	}
}

void SplineEditor::OnSelectionChanged()
{
}

void SplineEditor::OnKeyReleased(const Input::Key& key)
{
	if (!mSplineWrapper)
		return;

	if (key.keyCode == VK_DELETE)
	{
		auto selectedHandles = mSelectedHandles;
		for (auto handle : selectedHandles)
		{
			int idx = mHandles.IndexOf(handle);
			mSplineWrapper->RemovePoint(idx);
		}

		OnSplineChanged();
		mSplineWrapper->OnChanged();
		onChanged();
	}
}

void SplineEditor::CheckDragFromZero(const int i, Ref<PointHandles>& handles)
{
	float screenThreshold = 3.0f;
	float threshold = mSplineWrapper->WorldToLocal(Vec2F(screenThreshold, screenThreshold)).x -
		mSplineWrapper->WorldToLocal(Vec2F()).x;

	Vec2F prev = mSplineWrapper->GetPointPrevSupportPos(i);
	Vec2F next = mSplineWrapper->GetPointNextSupportPos(i);

	handles->startDragFromZero = (next - prev).Length() < threshold;
}

void SplineEditor::UpdateTransformFrame()
{
	mTransformFrameVisible = IsTransformFrameVisible();

	if (!mTransformFrameVisible || mSelectedHandles.IsEmpty())
		return;

	RectF aabb((mSelectedHandles[0])->GetPosition(), (mSelectedHandles[0])->GetPosition());

	for (auto handle : mSelectedHandles)
	{
		aabb.left = Math::Min(handle->GetPosition().x, aabb.left);
		aabb.right = Math::Max(handle->GetPosition().x, aabb.right);
		aabb.top = Math::Max(handle->GetPosition().t#include <memory>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using WeakRef = std::weak_ptr<T>;

template <typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
class DynamicCast
{
public:
    explicit DynamicCast(const Ref<T>& ptr) : m_ptr(ptr) {}

    template <typename U>
    operator Ref<U>() const
    {
        return std::dynamic_pointer_cast<U>(m_ptr);
    }

private:
    Ref<T> m_ptr;
};

...

class SplineEditor
{
public:
    void UpdateTransformFrame();
    bool IsTransformFrameVisible() const;
    void OnTransformFrameTransformed(const Basis& basis);
    void OnTransformBegin();
    void OnTransformCompleted();
    
private:
    Vec2F ISplineWrapper::WorldToLocal(const Vec2F& point) const;
    Vec2F ISplineWrapper::LocalToWorld(const Vec2F& point) const;
    Vector<Vec2F> ISplineWrapper::GetDrawPoints() const;

private:
    class PointHandles
    {
    public:
        Ref<IHandle> position;
        Ref<IHandle> prevSupport;
        Ref<IHandle> nextSupport;
        Input o2Input;

        bool IsSupportsVisible() const;
    };

    Vector<Ref<PointHandles>> mSplineHandles;
    Ref<ISplineWrapper> mSplineWrapper;
    HandleGroup mSupportHandlesGroup;
    Ref<TransformFrame> mTransformFrame;
    Vec2F mTransformBasisOffet;
    Basis mTransformFrameBasis;
};