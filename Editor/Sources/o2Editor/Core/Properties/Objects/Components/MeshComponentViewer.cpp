#include "o2Editor/stdafx.h"
#include "MeshComponentViewer.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	MeshComponentViewer::MeshComponentViewer()
	{}

	void MeshComponentViewer::SetTargetComponents(const Vector<Component*>& components)
	{
		TActorComponentViewer<MeshComponent>::SetTargetComponents(components);

		if (!mTargetComponents.IsEmpty())
		{
			mSplineTool.SetSpline(&mTargetComponents[0]->spline,
							[&]() { return mTargetComponents[0]->GetOwnerActor()->transform->GetWorldPosition(); });

			mFrameTool.SetFrame(Basis(mTargetComponents[0]->GetMappingFrame()));
			mFrameTool.frameHandles.SetRotationEnabled(false);
			mFrameTool.onChanged = [&](const Basis& b) { mTargetComponents[0]->SetMappingFrame(b.AABB()); };
		}
	}

	MeshComponentViewer& MeshComponentViewer::operator=(const MeshComponentViewer& other)
	{
		TActorComponentViewer<MeshComponent>::operator=(other);
		return *this;
	}

	void MeshComponentViewer::OnEnabled()
	{
		o2EditorSceneScreen.AddTool(&mSplineTool);
		o2EditorSceneScreen.AddTool(&mFrameTool);
		mPrevSelectedTool = o2EditorSceneScreen.GetSelectedTool();
		o2EditorSceneScreen.SelectTool<SplineTool>();
	}

	void MeshComponentViewer::OnDisabled()
	{
		if (o2EditorSceneScreen.GetSelectedTool() == &mSplineTool || o2EditorSceneScreen.GetSelectedTool() == &mFrameTool)
			o2EditorSceneScreen.SelectTool(mPrevSelectedTool);

		o2EditorSceneScreen.RemoveTool(&mSplineTool);
		o2EditorSceneScreen.RemoveTool(&mFrameTool);
	}
}

DECLARE_TEMPLATE_CLASS(Editor::TActorComponentViewer<MeshComponent>);

DECLARE_CLASS(Editor::MeshComponentViewer);
