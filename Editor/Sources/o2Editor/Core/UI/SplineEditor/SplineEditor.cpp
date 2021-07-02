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
	}

	SplineEditor::~SplineEditor()
	{
		delete mSelectionSprite;

		if (mSplineWrapper)
			delete mSplineWrapper;

		for (auto handles : mSplineHandles)
			delete handles;

		mSplineHandles.Clear();
	}

	void SplineEditor::Draw()
	{
		if (!mSplineWrapper)
			return;

		auto drawPoints = mSplineWrapper->GetDrawPoints();
		o2Render.DrawAALine(drawPoints, mSplineColor);

		for (auto handles : mSplineHandles)
		{
			o2Render.DrawAALine(handles->position.GetScreenPosition(), handles->prevSupport.GetScreenPosition(), mSplineSupportColor);
			o2Render.DrawAALine(handles->position.GetScreenPosition(), handles->nextSupport.GetScreenPosition(), mSplineSupportColor);
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

	void SplineEditor::DrawMainHandles()
	{
		for (auto handles : mSplineHandles)
			handles->position.Draw();
	}

	void SplineEditor::DrawSupportHandles()
	{
		for (auto handles : mSplineHandles)
		{
			handles->prevSupport.Draw();
			handles->nextSupport.Draw();
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

			handles->position = mHandlesSample.position;
			handles->position.SetPosition(mSplineWrapper->GetPointPos(i));
			handles->position.onChangedPos = [=](const Vec2F& pos) { OnMainHandleMoved(i, pos, handles); };
			handles->position.localToScreenTransformFunc = [&](const Vec2F& p) { return mSplineWrapper->LocalToWorld(p); };
			handles->position.screenToLocalTransformFunc = [&](const Vec2F& p) { return mSplineWrapper->WorldToLocal(p); };

			handles->prevSupport = mHandlesSample.prevSupport;
			handles->prevSupport.SetPosition(mSplineWrapper->GetPointPrevSupportPos(i));
			handles->prevSupport.onChangedPos = [=](const Vec2F& pos) { OnPrevHandleMoved(i, pos, handles); };
			handles->prevSupport.onPressed = [=]() { CheckDragSymmetric(i, handles); };
			handles->prevSupport.localToScreenTransformFunc = [&](const Vec2F& p) { return mSplineWrapper->LocalToWorld(p); };
			handles->prevSupport.screenToLocalTransformFunc = [&](const Vec2F& p) { return mSplineWrapper->WorldToLocal(p); };

			handles->nextSupport = mHandlesSample.nextSupport;
			handles->nextSupport.SetPosition(mSplineWrapper->GetPointNextSupportPos(i));
			handles->nextSupport.onChangedPos = [=](const Vec2F& pos) { OnNextHandleMoved(i, pos, handles); };
			handles->nextSupport.onPressed = [=]() { CheckDragSymmetric(i, handles); };
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

		if (handles->dragSymmetric && !o2Input.IsKeyDown(VK_MENU))
		{
			float distanceDiff = (pos - curr).Length() - (prev - curr).Length();
			float nextDistance = (next - curr).Length();
			Vec2F symmetricNextPos = (curr - pos).Normalized()*(nextDistance + distanceDiff) + curr;
			mSplineWrapper->SetPointNextSupportPos(i, symmetricNextPos);

			handles->nextSupport.SetPosition(mSplineWrapper->GetPointNextSupportPos(i));
		}

		mSplineWrapper->SetPointPrevSupportPos(i, pos);
	}

	void SplineEditor::OnNextHandleMoved(int i, const Vec2F& pos, PointHandles* handles)
	{
		Vec2F prev = mSplineWrapper->GetPointPrevSupportPos(i);
		Vec2F curr = mSplineWrapper->GetPointPos(i);
		Vec2F next = mSplineWrapper->GetPointNextSupportPos(i);

		if (handles->dragSymmetric && !o2Input.IsKeyDown(VK_MENU))
		{
			float distanceDiff = (pos - curr).Length() - (next - curr).Length();
			float prevDistance = (prev - curr).Length();
			Vec2F symmetricPrevPos = (curr - pos).Normalized()*(prevDistance + distanceDiff) + curr;
			mSplineWrapper->SetPointPrevSupportPos(i, symmetricPrevPos);

			handles->prevSupport.SetPosition(mSplineWrapper->GetPointPrevSupportPos(i));
		}

		mSplineWrapper->SetPointNextSupportPos(i, pos);
	}

	void SplineEditor::OnMainHandleMoved(int i, const Vec2F& pos, PointHandles* handles)
	{
		mSplineWrapper->SetPointPos(i, pos);
		handles->prevSupport.SetPosition(mSplineWrapper->GetPointPrevSupportPos(i));
		handles->nextSupport.SetPosition(mSplineWrapper->GetPointNextSupportPos(i));
	}

	void SplineEditor::CheckDragSymmetric(int i, PointHandles* handles)
	{
		Vec2F prev = mSplineWrapper->GetPointPrevSupportPos(i);
		Vec2F curr = mSplineWrapper->GetPointPos(i);
		Vec2F next = mSplineWrapper->GetPointNextSupportPos(i);
		handles->dragSymmetric = (next - curr).Normalized().Perpendicular().Dot(prev - curr) < 0.01f;
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

			Vec2F prevNextSupportPos = GetPointNextSupportPos(i - 1);
			Vec2F currPrevSupportPos = GetPointPrevSupportPos(i);

			for (int j = 0; j < segmentPoints; j++)
				res.Add(LocalToWorld(Bezier(prevPos, prevNextSupportPos, currPrevSupportPos, currPos, (float)j/(float)segmentPoints)));

			if (i == GetPointsCount() - 1)
				res.Add(LocalToWorld(currPos));
		}

		return res;
	}

}