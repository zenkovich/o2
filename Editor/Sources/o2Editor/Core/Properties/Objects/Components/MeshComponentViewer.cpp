#include "o2Editor/stdafx.h"
#include "MeshComponentViewer.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{

	MeshComponentViewer::MeshComponentViewer()
	{
		mSceneLayer.viewer = this;
		mTool.viewer = this;
	}

	void MeshComponentViewer::SetTargetComponents(const Vector<Component*>& components)
	{
		TActorComponentViewer<MeshComponent>::SetTargetComponents(components);

		SplineWrapper* wrapper = mnew SplineWrapper();
		wrapper->viewer = this;

		mSplineEditor.SetSpline(wrapper);
	}

	MeshComponentViewer& MeshComponentViewer::operator=(const MeshComponentViewer& other)
	{
		TActorComponentViewer<MeshComponent>::operator=(other);
		return *this;
	}

	void MeshComponentViewer::OnEnabled()
	{
		o2EditorSceneScreen.AddEditorLayer(&mSceneLayer);
		mIsEnabled = true;

		o2EditorSceneScreen.AddTool(&mTool);
		mPrevSelectedTool = o2EditorSceneScreen.GetSelectedTool();
		o2EditorSceneScreen.SelectTool<SplineTool>();
	}

	void MeshComponentViewer::OnDisabled()
	{
		if (o2EditorSceneScreen.GetSelectedTool() == &mTool)
			o2EditorSceneScreen.SelectTool(mPrevSelectedTool);

		o2EditorSceneScreen.RemoveTool(&mTool);

		o2EditorSceneScreen.RemoveEditorLayer(&mSceneLayer);
		mIsEnabled = false;
	}

	Vec2F MeshComponentViewer::SplineWrapper::GetOrigin() const
	{
		if (!viewer->mTargetComponents.IsEmpty())
			return viewer->mTargetComponents[0]->GetOwnerActor()->transform->GetWorldPosition();

		return Vec2F();
	}

	Vec2F MeshComponentViewer::SplineWrapper::WorldToLocal(const Vec2F& point) const
	{
		return o2EditorSceneScreen.ScreenToLocalPoint(point) - GetOrigin();
	}

	Vec2F MeshComponentViewer::SplineWrapper::LocalToWorld(const Vec2F& point) const
	{
		return o2EditorSceneScreen.LocalToScreenPoint(point + GetOrigin());
	}

	int MeshComponentViewer::SplineWrapper::GetPointsCount() const
	{
		if (!viewer->mTargetComponents.IsEmpty())
			return viewer->mTargetComponents[0]->spline.GetKeys().Count();

		return 0;
	}

	void MeshComponentViewer::SplineWrapper::AddPoint(int idx, const Vec2F& position, 
													  const Vec2F& prevSupport, const Vec2F& nextSupport)
	{
		if (!viewer->mTargetComponents.IsEmpty())
			viewer->mTargetComponents[0]->spline.InsertKey(idx, position, prevSupport, nextSupport);
	}

	void MeshComponentViewer::SplineWrapper::RemovePoint(int idx)
	{
		if (!viewer->mTargetComponents.IsEmpty())
			viewer->mTargetComponents[0]->spline.RemoveKey(idx);
	}

	Vec2F MeshComponentViewer::SplineWrapper::GetPointPos(int idx) const
	{
		if (!viewer->mTargetComponents.IsEmpty())
			return viewer->mTargetComponents[0]->spline.GetKey(idx).value;

		return Vec2F();
	}

	void MeshComponentViewer::SplineWrapper::SetPointPos(int idx, const Vec2F& pos)
	{
		if (viewer->mTargetComponents.IsEmpty())
			return;

		auto key = viewer->mTargetComponents[0]->spline.GetKey(idx);
		key.value = pos;
		viewer->mTargetComponents[0]->spline.SetKey(key, idx);
	}

	Vec2F MeshComponentViewer::SplineWrapper::GetPointPrevSupportPos(int idx) const
	{
		if (viewer->mTargetComponents.IsEmpty())
			return Vec2F();

		auto key = viewer->mTargetComponents[0]->spline.GetKey(idx);
		return key.prevSupport + key.value;
	}

	void MeshComponentViewer::SplineWrapper::SetPointPrevSupportPos(int idx, const Vec2F& pos)
	{
		if (viewer->mTargetComponents.IsEmpty())
			return;

		auto key = viewer->mTargetComponents[0]->spline.GetKey(idx);
		key.prevSupport = pos - key.value;
		viewer->mTargetComponents[0]->spline.SetKey(key, idx);
	}

	Vec2F MeshComponentViewer::SplineWrapper::GetPointNextSupportPos(int idx) const
	{
		if (viewer->mTargetComponents.IsEmpty())
			return Vec2F();

		auto key = viewer->mTargetComponents[0]->spline.GetKey(idx);
		return key.nextSupport + key.value;
	}

	void MeshComponentViewer::SplineWrapper::SetPointNextSupportPos(int idx, const Vec2F& pos)
	{
		if (viewer->mTargetComponents.IsEmpty())
			return;

		auto key = viewer->mTargetComponents[0]->spline.GetKey(idx);
		key.nextSupport = pos - key.value;
		viewer->mTargetComponents[0]->spline.SetKey(key, idx);
	}

	Vector<Vec2F> MeshComponentViewer::SplineWrapper::GetDrawPoints() const
	{
		if (viewer->mTargetComponents.IsEmpty())
			return Vector<Vec2F>();

		Vector<Vec2F> res;
		auto& keys = viewer->mTargetComponents[0]->spline.GetKeys();
		for (int i = 1; i < keys.Count(); i++)
		{
			for (int j = 0; j < keys[i].GetApproximatedPointsCount() - 1; j++)
				res.Add(LocalToWorld(keys[i].GetApproximatedPoints()[j].value));

			if (i == keys.Count() - 1)
				res.Add(LocalToWorld(keys[i].value));
		}

		return res;
	}

	const ApproximationVec2F* MeshComponentViewer::SplineWrapper::GetPointApproximation(int idx) const
	{
		if (viewer->mTargetComponents.IsEmpty())
			return nullptr;

		return viewer->mTargetComponents[0]->spline.GetKeys()[idx].GetApproximatedPoints();
	}

	int MeshComponentViewer::SplineWrapper::GetPointApproximationCount(int idx) const
	{
		if (viewer->mTargetComponents.IsEmpty())
			return 0;

		return viewer->mTargetComponents[0]->spline.GetKeys()[idx].GetApproximatedPointsCount();
	}

	void MeshComponentViewer::SplineWrapper::OnChanged()
	{
	}

	void MeshComponentViewer::SplineSceneLayer::DrawOverScene()
	{
		viewer->mSplineEditor.Draw();
	}

	void MeshComponentViewer::SplineSceneLayer::Update(float dt)
	{
		viewer->mSplineEditor.Update(dt);
	}

	int MeshComponentViewer::SplineSceneLayer::GetOrder() const
	{
		return 0;
	}

	bool MeshComponentViewer::SplineSceneLayer::IsEnabled() const
	{
		return viewer->mIsEnabled;
	}

	const String& MeshComponentViewer::SplineSceneLayer::GetName() const
	{
		static String res("Animation spline");
		return res;
	}

	const String& MeshComponentViewer::SplineSceneLayer::GetIconName() const
	{
		return String::empty;
	}

	String MeshComponentViewer::SplineTool::GetPanelIcon() const
	{
		return "ui/UI4_path_tool.png";
	}

	void MeshComponentViewer::SplineTool::OnEnabled()
	{
		viewer->mIsEnabled = true;
	}

	void MeshComponentViewer::SplineTool::OnDisabled()
	{
		viewer->mIsEnabled = false;
	}
}

DECLARE_TEMPLATE_CLASS(Editor::TActorComponentViewer<MeshComponent>);

DECLARE_CLASS(Editor::MeshComponentViewer);
