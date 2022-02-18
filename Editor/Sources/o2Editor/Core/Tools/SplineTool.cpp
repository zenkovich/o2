#include "o2Editor/stdafx.h"
#include "SplineTool.h"

#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	SplineTool::SplineTool()
	{
		sceneLayer.tool = this;
	}

	Vec2F SplineTool::SplineWrapper::GetOrigin() const
	{
		return getOrigin();
	}

	Vec2F SplineTool::SplineWrapper::WorldToLocal(const Vec2F& point) const
	{
		return o2EditorSceneScreen.ScreenToLocalPoint(point) - GetOrigin();
	}

	Vec2F SplineTool::SplineWrapper::LocalToWorld(const Vec2F& point) const
	{
		return o2EditorSceneScreen.LocalToScreenPoint(point + GetOrigin());
	}

	int SplineTool::SplineWrapper::GetPointsCount() const
	{
		return spline->GetKeys().Count();
	}

	bool SplineTool::SplineWrapper::IsClosed() const
	{
		return spline->IsClosed();
	}

	void SplineTool::SplineWrapper::AddPoint(int idx, const Vec2F& position,
											 const Vec2F& prevSupport, const Vec2F& nextSupport)
	{
		spline->InsertKey(idx, position, prevSupport, nextSupport);
	}

	void SplineTool::SplineWrapper::RemovePoint(int idx)
	{
		spline->RemoveKey(idx);
	}

	Vec2F SplineTool::SplineWrapper::GetPointPos(int idx) const
	{
		return spline->GetKey(idx).value;
	}

	void SplineTool::SplineWrapper::SetPointPos(int idx, const Vec2F& pos)
	{
		auto key = spline->GetKey(idx);
		key.value = pos;
		spline->SetKey(key, idx);
	}

	Vec2F SplineTool::SplineWrapper::GetPointPrevSupportPos(int idx) const
	{
		auto key = spline->GetKey(idx);
		return key.prevSupport + key.value;
	}

	void SplineTool::SplineWrapper::SetPointPrevSupportPos(int idx, const Vec2F& pos)
	{
		auto key = spline->GetKey(idx);
		key.prevSupport = pos - key.value;
		spline->SetKey(key, idx);
	}

	Vec2F SplineTool::SplineWrapper::GetPointNextSupportPos(int idx) const
	{
		auto key = spline->GetKey(idx);
		return key.nextSupport + key.value;
	}

	void SplineTool::SplineWrapper::SetPointNextSupportPos(int idx, const Vec2F& pos)
	{
		auto key = spline->GetKey(idx);
		key.nextSupport = pos - key.value;
		spline->SetKey(key, idx);
	}

	Vector<Vec2F> SplineTool::SplineWrapper::GetDrawPoints() const
	{
		Vector<Vec2F> res;
		auto& keys = spline->GetKeys();
		for (int i = 1; i < keys.Count(); i++)
		{
			for (int j = 0; j < keys[i].GetApproximatedPointsCount() - 1; j++)
				res.Add(LocalToWorld(keys[i].GetApproximatedPoints()[j].value));

			if (i == keys.Count() - 1)
				res.Add(LocalToWorld(keys[i].value));
		}

		return res;
	}

	const ApproximationVec2F* SplineTool::SplineWrapper::GetPointApproximation(int idx) const
	{
		return spline->GetKeys()[idx].GetApproximatedPoints();
	}

	int SplineTool::SplineWrapper::GetPointApproximationCount(int idx) const
	{
		return spline->GetKeys()[idx].GetApproximatedPointsCount();
	}

	void SplineTool::SplineWrapper::OnChanged()
	{
		tool->onChanged();
	}

	void SplineTool::SplineSceneLayer::DrawOverScene()
	{
		tool->splineEditor.Draw();
	}

	void SplineTool::SplineSceneLayer::Update(float dt)
	{
		tool->splineEditor.Update(dt);
	}

	int SplineTool::SplineSceneLayer::GetOrder() const
	{
		return 0;
	}

	bool SplineTool::SplineSceneLayer::IsEnabled() const
	{
		return tool->isEnabled;
	}

	const String& SplineTool::SplineSceneLayer::GetName() const
	{
		static String res("Animation spline");
		return res;
	}

	const String& SplineTool::SplineSceneLayer::GetIconName() const
	{
		return String::empty;
	}

	void SplineTool::SplineTool::SetSpline(Spline* spline, const Function<Vec2F()>& getOrigin)
	{
		auto wrapper = mnew SplineWrapper();
		wrapper->spline = spline;
		wrapper->getOrigin = getOrigin;
		wrapper->tool = this;
		splineEditor.SetSpline(wrapper);
	}

	String SplineTool::SplineTool::GetPanelIcon() const
	{
		return "ui/UI4_path_tool.png";
	}

	void SplineTool::SplineTool::OnEnabled()
	{
		o2EditorSceneScreen.AddEditorLayer(&sceneLayer);
		isEnabled = true;
	}

	void SplineTool::SplineTool::OnDisabled()
	{
		o2EditorSceneScreen.RemoveEditorLayer(&sceneLayer);
		isEnabled = false;
	}
}