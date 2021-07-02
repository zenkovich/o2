#include "o2Editor/stdafx.h"
#include "Vec2KeyFramesTrackControl.h"

#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{

	Vec2KeyFramesTrackControl::Vec2KeyFramesTrackControl():
		Base()
	{
		InitializeControls();
	}

	Vec2KeyFramesTrackControl::Vec2KeyFramesTrackControl(const Vec2KeyFramesTrackControl& other):
		Base(other)
	{
		InitializeControls();
	}

	Vec2KeyFramesTrackControl::~Vec2KeyFramesTrackControl()
	{
		o2EditorSceneScreen.RemoveEditorLayer(&mSceneLayer);
	}

	void Vec2KeyFramesTrackControl::SetActive(bool active)
	{
	}

	Vec2KeyFramesTrackControl& Vec2KeyFramesTrackControl::operator=(const Vec2KeyFramesTrackControl& other)
	{
		Base::operator=(other);
		return *this;
	}

	void Vec2KeyFramesTrackControl::InitializeControls()
	{
		o2EditorSceneScreen.AddEditorLayer(&mSceneLayer);
		mSceneLayer.trackControl = this;
	}

	void Vec2KeyFramesTrackControl::OnSetTrack()
	{
		SplineWrapper* wrapper = mnew SplineWrapper();
		wrapper->trackControl = this;

		mSplineEditor.SetSpline(wrapper);
	}

	void Vec2KeyFramesTrackControl::OnKeysChanged()
	{
		mSplineEditor.OnSplineChanged();
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::WorldToLocal(const Vec2F& point) const
	{
		return o2EditorSceneScreen.ScreenToLocalPoint(point);
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::LocalToWorld(const Vec2F& point) const
	{
		return o2EditorSceneScreen.LocalToScreenPoint(point);
	}

	int Vec2KeyFramesTrackControl::SplineWrapper::GetPointsCount() const
	{
		return trackControl->mTrack->GetKeys().Count();
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::AddPoint(int idx, const Vec2F& position, const Vec2F& prevSupport,
															const Vec2F& nextSupport)
	{
		if (idx < 1 || idx > GetPointsCount() - 2)
			return;

		float idxKeyPos = trackControl->mTrack->GetKeyAt(idx).position;
		float idxNextKeyPos = trackControl->mTrack->GetKeyAt(idx + 1).position;
		trackControl->mTrack->AddKey((idxNextKeyPos + idxKeyPos)/2.0f, position, prevSupport, nextSupport, 0, 0, 0, 0);
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::RemovePoint(int idx)
	{
		trackControl->mTrack->RemoveKeyAt(idx);
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::GetPointPos(int idx) const
	{
		return trackControl->mTrack->GetKeyAt(idx).value;
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::SetPointPos(int idx, const Vec2F& pos)
	{
		auto key = trackControl->mTrack->GetKeyAt(idx);
		Vec2F delta = pos - key.value;
		key.value = pos;
		key.prevSupportValue += delta;
		key.nextSupportValue += delta;
		trackControl->mTrack->SetKey(idx, key);
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::GetPointPrevSupportPos(int idx) const
	{
		return trackControl->mTrack->GetKeyAt(idx).prevSupportValue;
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::SetPointPrevSupportPos(int idx, const Vec2F& pos)
	{
		auto key = trackControl->mTrack->GetKeyAt(idx);
		key.prevSupportValue = pos;
		trackControl->mTrack->SetKey(idx, key);
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::GetPointNextSupportPos(int idx) const
	{
		return trackControl->mTrack->GetKeyAt(idx).nextSupportValue;
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::SetPointNextSupportPos(int idx, const Vec2F& pos)
	{
		auto key = trackControl->mTrack->GetKeyAt(idx);
		key.nextSupportValue = pos;
		trackControl->mTrack->SetKey(idx, key);
	}

	void Vec2KeyFramesTrackControl::SplineSceneLayer::DrawOverScene()
	{
		trackControl->mSplineEditor.Draw();
	}

	void Vec2KeyFramesTrackControl::SplineSceneLayer::Update(float dt)
	{
		trackControl->mSplineEditor.Update(dt);
	}

	int Vec2KeyFramesTrackControl::SplineSceneLayer::GetOrder() const
	{
		return 0;
	}

	bool Vec2KeyFramesTrackControl::SplineSceneLayer::IsEnabled() const
	{
		return true;
	}

	const String& Vec2KeyFramesTrackControl::SplineSceneLayer::GetName() const
	{
		return "Animation spline";
	}

	const String& Vec2KeyFramesTrackControl::SplineSceneLayer::GetIconName() const
	{
		return "";
	}

}

DECLARE_CLASS(Editor::Vec2KeyFramesTrackControl);
