#include "o2Editor/stdafx.h"
#include "SplineEditor.h"

namespace Editor
{
	SplineEditor::SplineEditor()
	{
		mSelectionSprite = mnew Sprite("ui/UI_Window_place.png");

		mHandlesSample.position = DragHandle(mnew Sprite("ui/CurveHandle.png"),
											 mnew Sprite("ui/CurveHandleHover.png"),
											 mnew Sprite("ui/CurveHandlePressed.png"),
											 mnew Sprite("ui/CurveHandleSelected.png"));

		mHandlesSample.prevSupport = DragHandle(mnew Sprite("ui/CurveSupportHandle.png"),
												mnew Sprite("ui/CurveSupportHandleHover.png"),
												mnew Sprite("ui/CurveSupportHandlePressed.png"),
												mnew Sprite("ui/CurveSupportHandleSelected.png"));

		mHandlesSample.nextSupport = DragHandle(mnew Sprite("ui/CurveSupportHandle.png"),
												mnew Sprite("ui/CurveSupportHandleHover.png"),
												mnew Sprite("ui/CurveSupportHandlePressed.png"),
												mnew Sprite("ui/CurveSupportHandleSelected.png"));

		mSplineColor = Color4(44, 62, 80, 255);
		mSplineSupportColor = Color4(190, 190, 190, 255);

		typedef SplineEditor thisclass;

		mTransformFrame.SetPivotEnabled(false);
		mTransformFrame.SetRotationEnabled(false);
		mTransformFrame.onTransformed = THIS_FUNC(OnTransformFrameTransformed);
		mTransformFrame.onPressed = THIS_FUNC(OnTransformBegin);
		mTransformFrame.onChangeCompleted = THIS_FUNC(OnTransformCompleted);
		mTransformFrame.isInputTransparent = true;
	}

	SplineEditor::~SplineEditor()
	{
		delete mSelectionSprite;

		if (mSplineWrapper)
			delete mSplineWrapper;

		auto splineHandles = mSplineHandles;
		mSplineHandles.Clear();

		for (auto handles : splineHandles)
			delete handles;

		mSplineHandles.Clear();
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
		for (auto handles : mSplineHandles)
		{
			if (handles->IsSupportsVisible())
			{
				handles->prevSupport.UpdateScreenPosition();
				handles->nextSupport.UpdateScreenPosition();

				if (!handles->isFirst)
				{
					o2Render.DrawAALine(handles->position.GetScreenPosition(), handles->prevSupport.GetScreenPosition(),
										mSplineSupportColor);
				}

				if (!handles->isLast)
				{
					o2Render.DrawAALine(handles->position.GetScreenPosition(), handles->nextSupport.GetScreenPosition(),
										mSplineSupportColor);
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
								 worldXV + Vec2F(mTransformBasisOffet.x*2.0f, 0),
								 worldYV + Vec2F(0, mTransformBasisOffet.y*2.0f)));

		mTransformFrame.Draw();
	}

	void SplineEditor::DrawMainHandles()
	{
		for (auto handles : mSplineHandles)
			handles->position.Draw();
	}

	void SplineEditor::DrawSupportHandles()
	{
		for (auto handles : mSplineHandles)
		{
			if (handles->IsSupportsVisible())
			{
				if (!handles->isFirst)
					handles->prevSupport.Draw();

				if (!handles->isLast)
					handles->nextSupport.Draw();
			}
		}
	}

	void SplineEditor::Update(float dt)
	{

	}

	void SplineEditor::SetSpline(ISplineWrapper* wrapper)
	{
		if (mSplineWrapper)
			delete mSplineWrapper;

		ClearHandles();


		mSplineWrapper = wrapper;
		if (!mSplineWrapper)
			return;

		InitializeHandles();

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
		for (auto handles : mSplineHandles)
			delete handles;

		mSplineHandles.Clear();
	}

	void SplineEditor::OnSplineChanged()
	{
		ClearHandles();
		InitializeHandles();
	}

	void SplineEditor::InitializeHandles()
	{
		for (int i = 0; i < mSplineWrapper->GetPointsCount(); i++)
		{
			PointHandles* handles = mnew PointHandles();

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

	void SplineEditor::OnPrevHandleMoved(int i, const Vec2F& pos, PointHandles* handles)
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

	void SplineEditor::OnNextHandleMoved(int i, const Vec2F& pos, PointHandles* handles)
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

	void SplineEditor::OnMainHandleMoved(int i, const Vec2F& pos, PointHandles* handles)
	{
		mSplineWrapper->SetPointPos(i, pos);
		handles->prevSupport.SetPosition(mSplineWrapper->GetPointPrevSupportPos(i));
		handles->nextSupport.SetPosition(mSplineWrapper->GetPointNextSupportPos(i));

		UpdateTransformFrame();
		mSplineWrapper->OnChanged();
		onChanged();
	}

	void SplineEditor::OnPrevHandleReleasedNoDrag(int i, PointHandles* handles)
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

	void SplineEditor::OnNextHandleReleasedNoDrag(int i, PointHandles* handles)
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

	void SplineEditor::OnMainHandleReleasedNoDrag(int i, PointHandles* handles)
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
			for (int j = 1; j < mSplineWrapper->GetPointApproximationCount(j); j++)
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

	void SplineEditor::CheckDragFromZero(int i, PointHandles* handles)
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

		if (!mTransformFrameVisible)
			return;

		RectF aabb((mSelectedHandles[0])->GetPosition(), (mSelectedHandles[0])->GetPosition());

		for (auto handle : mSelectedHandles)
		{
			aabb.left = Math::Min(handle->GetPosition().x, aabb.left);
			aabb.right = Math::Max(handle->GetPosition().x, aabb.right);
			aabb.top = Math::Max(handle->GetPosition().y, aabb.top);
			aabb.bottom = Math::Min(handle->GetPosition().y, aabb.bottom);
		}

		for (auto handle : mSupportHandlesGroup.GetSelectedHandles())
		{
			aabb.left = Math::Min(handle->GetPosition().x, aabb.left);
			aabb.right = Math::Max(handle->GetPosition().x, aabb.right);
			aabb.top = Math::Max(handle->GetPosition().y, aabb.top);
			aabb.bottom = Math::Min(handle->GetPosition().y, aabb.bottom);
		}

		mTransformFrameBasis = Basis(aabb.LeftBottom(), Vec2F::Right()*aabb.Width(), Vec2F::Up()*aabb.Height());
	}

	bool SplineEditor::IsTransformFrameVisible() const
	{
		int selectedMainHandles = 0;

		for (auto handles : mSplineHandles)
		{
			if (handles->position.IsSelected())
				selectedMainHandles++;
		}

		return selectedMainHandles > 1;
	}

	void SplineEditor::OnTransformFrameTransformed(const Basis& basis)
	{
		Vec2F localBasisOrig = mSplineWrapper->WorldToLocal(basis.origin + mTransformBasisOffet);
		Vec2F localBasisXV = mSplineWrapper->WorldToLocal(basis.xv - Vec2F(mTransformBasisOffet.x*2.0f, 0) + basis.origin + mTransformBasisOffet) - localBasisOrig;
		Vec2F localBasisYV = mSplineWrapper->WorldToLocal(basis.yv - Vec2F(0, mTransformBasisOffet.y*2.0f) + basis.origin + mTransformBasisOffet) - localBasisOrig;
		Basis localBasis(localBasisOrig, localBasisXV, localBasisYV);

		Basis delta = mTransformFrameBasis.Inverted()*localBasis;

		if (delta.origin.Length() > 0.01f || delta.xv != Vec2F(1, 0) || delta.yv != Vec2F(0, 1))
		{
			Vector<Vec2F> newPosHandlesPositions, newSupportnHandlesPositions;

			for (int i = 0; i < mSelectedHandles.Count(); i++)
				newPosHandlesPositions.Add(mSelectedHandles[i]->GetPosition()*delta);

			auto& supportHandles = mSupportHandlesGroup.GetSelectedHandles();
			for (int i = 0; i < supportHandles.Count(); i++)
				newSupportnHandlesPositions.Add(supportHandles[i]->GetPosition()*delta);

			for (int i = 0; i < mSelectedHandles.Count(); i++)
			{
				mSelectedHandles[i]->SetPosition(newPosHandlesPositions[i]);
				mSelectedHandles[i]->onChangedPos(mSelectedHandles[i]->GetPosition());
			}

			for (int i = 0; i < supportHandles.Count(); i++)
			{
				supportHandles[i]->SetPosition(newSupportnHandlesPositions[i]);
				supportHandles[i]->onChangedPos(supportHandles[i]->GetPosition());
			}

			UpdateTransformFrame();
		}

		mSplineWrapper->OnChanged();
		onChanged();
	}

	void SplineEditor::OnTransformBegin()
	{
	}

	void SplineEditor::OnTransformCompleted()
	{
	}

	Vec2F SplineEditor::ISplineWrapper::WorldToLocal(const Vec2F& point) const
	{
		return point;
	}

	Vec2F SplineEditor::ISplineWrapper::LocalToWorld(const Vec2F& point) const
	{
		return point;
	}

	Vector<Vec2F> SplineEditor::ISplineWrapper::GetDrawPoints() const
	{
		Vector<Vec2F> res;
		int segmentPoints = 20;

		for (int i = 1; i < GetPointsCount(); i++)
		{
			Vec2F prevPos = GetPointPos(i - 1);
			Vec2F currPos = GetPointPos(i);

			Vec2F prevNextSupportPos = GetPointNextSupportPos(i - 1) + currPos;
			Vec2F currPrevSupportPos = GetPointPrevSupportPos(i) + currPos;

			for (int j = 0; j < segmentPoints - 1; j++)
				res.Add(LocalToWorld(Bezier(prevPos, prevNextSupportPos, currPrevSupportPos, currPos, (float)j/(float)segmentPoints)));
		}

		return res;
	}

	bool SplineEditor::PointHandles::IsSupportsVisible() const
	{
		return position.IsSelected() || prevSupport.IsSelected() ||nextSupport.IsSelected() || o2Input.IsKeyDown(VK_MENU);
	}

}
