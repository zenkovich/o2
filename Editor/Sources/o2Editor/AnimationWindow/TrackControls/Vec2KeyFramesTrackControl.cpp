#include "o2Editor/stdafx.h"
#include "Vec2KeyFramesTrackControl.h"

#include "o2Editor/Core/ToolsPanel.h"
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
		o2EditorSceneScreen.RemoveTool(&mTool);
	}

	void Vec2KeyFramesTrackControl::SetActive(bool active)
	{
		mIsEnabled = active;

		if (active)
		{
			o2EditorSceneScreen.AddTool(&mTool);
			mPrevSelectedTool = o2EditorSceneScreen.GetSelectedTool();
			o2EditorSceneScreen.SelectTool<SplineTool>();
		}
		else
		{
			if (o2EditorSceneScreen.GetSelectedTool() == &mTool)
				o2EditorSceneScreen.SelectTool(mPrevSelectedTool);

			o2EditorSceneScreen.RemoveTool(&mTool);
		}
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
		mTool.trackControl = this;
	}

	void Vec2KeyFramesTrackControl::TryFindOwnerTrack()
	{
		mTrackOwner = nullptr;

		if (!mPlayer)
			return;

		auto path = mTrack->path;
		Actor* root = dynamic_cast<Actor*>(mPlayer->GetOwnerPlayer()->GetTarget());
		while (root && path.StartsWith("child"))
		{
			int nextSlash = path.Find("/");
			if (nextSlash < 0)
				return;

			int nextChildSlash = path.Find("/", nextSlash + 1);
			auto childName = path.SubStr(nextSlash + 1, nextChildSlash);
			root = root->GetChild(childName);
			path.Erase(0, nextChildSlash + 1);
		}

		mTrackOwner = root;
	}

	void Vec2KeyFramesTrackControl::OnSetTrack()
	{
		SplineWrapper* wrapper = mnew SplineWrapper();
		wrapper->trackControl = this;

		mSplineEditor.SetSpline(wrapper);

		TryFindOwnerTrack();
	}

	void Vec2KeyFramesTrackControl::OnKeysChanged()
	{
		mSplineEditor.OnSplineChanged();
	}

	void Vec2KeyFramesTrackControl::SetCurveViewEnabled(bool enabled)
	{
		mAddKeyDotButton->enabled = enabled;
		mAddKeyButton->enabled = !enabled;
	}

	void Vec2KeyFramesTrackControl::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		if (!mHandlesSheet->enabled)
			return;

		OnDrawn();

		o2Render.EnableScissorTest(mTimeline->layout->GetWorldRect());

		for (int i = 1; i < mTrack->timeCurve.GetKeys().Count(); i++)
		{
			auto& key = mTrack->timeCurve.GetKeys()[i];
			auto& prevKey = mTrack->timeCurve.GetKeys()[i - 1];

			Basis drawCoords(RectF(mTimeline->LocalToWorld(prevKey.position) - 3,
							 layout->GetWorldTop() - 5,
							 mTimeline->LocalToWorld(key.position) - 3,
							 layout->GetWorldBottom() + 5));

			DrawCurveInCoords(key.GetApproximatedPoints(), key.GetApproximatedPointsCount(),
							  key.GetGetApproximatedPointsBounds(), drawCoords, Color4(44, 62, 80));
		}

		for (auto handle : mHandles)
			handle->handle->Draw();

		o2Render.DisableScissorTest();

		DrawDebugFrame();
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::GetOrigin() const
	{
		if (trackControl->mTrackOwner && trackControl->mTrackOwner->GetParent())
			return trackControl->mTrackOwner->GetParent()->transform->worldPosition;

		return Vec2F();
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::WorldToLocal(const Vec2F& point) const
	{
		return o2EditorSceneScreen.ScreenToLocalPoint(point) - GetOrigin();
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::LocalToWorld(const Vec2F& point) const
	{
		return o2EditorSceneScreen.LocalToScreenPoint(point + GetOrigin());
	}

	int Vec2KeyFramesTrackControl::SplineWrapper::GetPointsCount() const
	{
		return trackControl->mTrack->spline.GetKeys().Count();
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::AddPoint(int idx, const Vec2F& position, const Vec2F& prevSupport,
															const Vec2F& nextSupport)
	{
		trackControl->mTrack->spline.InsertKey(idx, position, prevSupport, nextSupport);
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::RemovePoint(int idx)
	{
		trackControl->mTrack->spline.RemoveKey(idx);
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::GetPointPos(int idx) const
	{
		return trackControl->mTrack->spline.GetKey(idx).value;
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::SetPointPos(int idx, const Vec2F& pos)
	{
		auto key = trackControl->mTrack->spline.GetKey(idx);
		Vec2F delta = pos - key.value;
		key.value = pos;
		trackControl->mTrack->spline.SetKey(key, idx);
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::GetPointPrevSupportPos(int idx) const
	{
		auto key = trackControl->mTrack->spline.GetKey(idx);
		return key.prevSupport + key.value;
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::SetPointPrevSupportPos(int idx, const Vec2F& pos)
	{
		auto key = trackControl->mTrack->spline.GetKey(idx);
		key.prevSupport = pos - key.value;
		trackControl->mTrack->spline.SetKey(key, idx);
	}

	Vec2F Vec2KeyFramesTrackControl::SplineWrapper::GetPointNextSupportPos(int idx) const
	{
		auto key = trackControl->mTrack->spline.GetKey(idx);
		return key.nextSupport + key.value;
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::SetPointNextSupportPos(int idx, const Vec2F& pos)
	{
		auto key = trackControl->mTrack->spline.GetKey(idx);
		key.nextSupport = pos - key.value;
		trackControl->mTrack->spline.SetKey(key, idx);
	}

	Vector<Vec2F> Vec2KeyFramesTrackControl::SplineWrapper::GetDrawPoints() const
	{
		Vector<Vec2F> res;
		auto& keys = trackControl->mTrack->spline.GetKeys();
		for (int i = 1; i < keys.Count(); i++)
		{
			for (int j = 0; j < keys[i].GetApproximatedPointsCount() - 1; j++)
				res.Add(LocalToWorld(keys[i].GetApproximatedPoints()[j].value));

			if (i == keys.Count() - 1)
				res.Add(LocalToWorld(keys[i].value));
		}

		return res;
	}

	const ApproximationVec2F* Vec2KeyFramesTrackControl::SplineWrapper::GetPointApproximation(int idx) const
	{
		return trackControl->mTrack->spline.GetKeys()[idx].GetApproximatedPoints();
	}

	int Vec2KeyFramesTrackControl::SplineWrapper::GetPointApproximationCount(int idx) const
	{
		return trackControl->mTrack->spline.GetKeys()[idx].GetApproximatedPointsCount();
	}

	void Vec2KeyFramesTrackControl::SplineWrapper::OnChanged()
	{
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
		return trackControl->mIsEnabled;
	}

	const String& Vec2KeyFramesTrackControl::SplineSceneLayer::GetName() const
	{
		static String res("Animation spline");
		return res;
	}

	const String& Vec2KeyFramesTrackControl::SplineSceneLayer::GetIconName() const
	{
		return String::empty;
	}

	String Vec2KeyFramesTrackControl::SplineTool::GetPanelIcon() const
	{
		return "ui/UI4_path_tool.png";
	}

	void Vec2KeyFramesTrackControl::SplineTool::OnEnabled()
	{
		trackControl->mIsEnabled = true;
	}

	void Vec2KeyFramesTrackControl::SplineTool::OnDisabled()
	{
		trackControl->mIsEnabled = false;
	}

}

DECLARE_CLASS(Editor::Vec2KeyFramesTrackControl);
