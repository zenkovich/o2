#include "o2Editor/stdafx.h"
#include "SkinningMeshComponentViewer.h"

#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/Tools/SkeletonTool.h"
#include "o2Editor/SceneWindow/SceneDraghandle.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	SkinningMeshComponentViewer::SkinningMeshComponentViewer() = default;

	SkinningMeshComponentViewer::~SkinningMeshComponentViewer()
	{
		o2EditorSceneScreen.RemoveTool(&mSplineTool);
		o2EditorSceneScreen.RemoveTool(&mFrameTool);
		o2EditorSceneScreen.RemoveTool(&mTopologyTool);
		o2EditorSceneScreen.RemoveEditorLayer(&mFrameTetxureLayer);
	}

	SkinningMeshComponentViewer& SkinningMeshComponentViewer::operator=(const SkinningMeshComponentViewer& other)
	{
		TObjectPropertiesViewer<SkinningMeshComponent>::operator=(other);
		return *this;
	}

	void SkinningMeshComponentViewer::RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		o2EditorProperties.BuildObjectProperties(mSpoiler, &TypeOf(SkinningMeshComponent), mPropertiesContext, "",
												 mOnChildFieldChangeCompleted, onChanged);

		mFitAndCenterButton = o2UI.CreateButton("Fit and centerize image", THIS_FUNC(FitAndCenterize));
		mEditSkeletonButton = o2UI.CreateButton("Edit skeleton", THIS_FUNC(OnEditSkeletonPressed));
		mReskinButton = o2UI.CreateButton("Reskin", THIS_FUNC(OnReskinPressed));

		mSpoiler->AddChild(mFitAndCenterButton);
		mSpoiler->AddChild(mEditSkeletonButton);
		mSpoiler->AddChild(mReskinButton);
	}

	void SkinningMeshComponentViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		auto prevTargetObjects = mTypeTargetObjects;

		TObjectPropertiesViewer<SkinningMeshComponent>::OnRefreshed(targetObjets);

		if (!mTypeTargetObjects.IsEmpty() && prevTargetObjects != mTypeTargetObjects)
		{
			Function<Vec2F()> getOrigin = [&]() {
				return mTypeTargetObjects[0].first->GetOwnerActor()->transform->GetWorldNonSizedBasis().origin;
			};

			// Spline tool
			mSplineTool.SetSpline(&mTypeTargetObjects[0].first->spline, getOrigin);
			mSplineTool.onChanged = [&]() { mTypeTargetObjects[0].first->GetOwnerActor()->OnChanged(); };

			// Frame tool
			mFrameTool.SetFrame(Basis(mTypeTargetObjects[0].first->GetMappingFrame()));
			mFrameTool.frameHandles.SetRotationEnabled(false);
			mFrameTool.getOrigin = getOrigin;
			mFrameTool.onChanged = [&](const Basis& b) {
				mTypeTargetObjects[0].first->SetMappingFrame(b.AABB());
				mTypeTargetObjects[0].first->GetOwnerActor()->OnChanged();
			};

			mFrameTetxureLayer.viewer = this;

			// Topology tool
			auto mesh = mTypeTargetObjects[0].first;
			mTopologyTool.Setup([=]() { return mesh->GetExtraPoints(); },
								[=](int idx, Vec2F p) { mesh->SetExtraPoint(idx, p); mesh->GetOwnerActor()->OnChanged(); },
								[=]() { return mesh->GetOwnerActor()->transform->GetWorldNonSizedBasis(); },
								[=](Vec2F p) { mesh->AddExtraPoint(p); mesh->GetOwnerActor()->OnChanged(); },
								[=](int idx) { mesh->RemoveExtraPoint(idx); mesh->GetOwnerActor()->OnChanged(); });
		}
	}

	void SkinningMeshComponentViewer::OnEnabled()
	{
		o2EditorSceneScreen.AddTool(&mSplineTool);
		o2EditorSceneScreen.AddTool(&mFrameTool);
		o2EditorSceneScreen.AddTool(&mTopologyTool);

		o2EditorSceneScreen.SelectTool<SplineTool>();

		o2EditorSceneScreen.AddEditorLayer(&mFrameTetxureLayer);

		mSkeletonTool = o2EditorSceneScreen.GetTool<SkeletonTool>();
		if (mSkeletonTool && !mTypeTargetObjects.IsEmpty() && mTypeTargetObjects[0].first)
			mEditingSkeleton = mSkeletonTool->IsEditingSkeleton(mTypeTargetObjects[0].first);

		UpdateEditSkeletonButtonCaption();
	}

	void SkinningMeshComponentViewer::OnDisabled()
	{
		o2EditorSceneScreen.RemoveTool(&mSplineTool);
		o2EditorSceneScreen.RemoveTool(&mFrameTool);
		o2EditorSceneScreen.RemoveTool(&mTopologyTool);

		o2EditorSceneScreen.RemoveEditorLayer(&mFrameTetxureLayer);

		mSplineTool.Reset();
		mFrameTool.Reset();
		mTopologyTool.Reset();
	}

	void SkinningMeshComponentViewer::FitAndCenterize()
	{
		for (auto& target : mTypeTargetObjects)
		{
			auto component = target.first;
			auto texture = component->GetImage();
			auto size = texture->GetSize();

			component->SetMappingFrame(RectF(size*-0.5f, size*0.5f));

			mFrameTool.SetFrame(Basis(mTypeTargetObjects[0].first->GetMappingFrame()));
		}
	}

	void SkinningMeshComponentViewer::OnEditSkeletonPressed()
	{
		if (mTypeTargetObjects.IsEmpty() || !mTypeTargetObjects[0].first)
			return;

		mEditingSkeleton = !mEditingSkeleton;

		auto component = mTypeTargetObjects[0].first;

		if (mEditingSkeleton) 
		{
			mSkeletonTool = o2EditorSceneScreen.GetTool<SkeletonTool>();
			if (!mSkeletonTool)
			{
				mSkeletonTool = mnew SkeletonTool();
				o2EditorSceneScreen.AddTool(mSkeletonTool);
			}

			mSkeletonTool->AddSkeletonInstance(component);

			o2EditorSceneScreen.SelectTool<SkeletonTool>();
		}
		else 
		{
			if (mSkeletonTool)
			{
				mSkeletonTool->RemoveSkeletonInstance(component);

				if (mSkeletonTool->GetEditingSkeletons().Count() == 0)
				{
					o2EditorSceneScreen.RemoveTool(mSkeletonTool);
					delete mSkeletonTool;
					mSkeletonTool = nullptr;
				}
			}
		}

		UpdateEditSkeletonButtonCaption();
	}

	void SkinningMeshComponentViewer::OnReskinPressed()
	{
		for (auto targetPair : mTypeTargetObjects)
		{
			targetPair.first->NeedUpdateBones();
			targetPair.first->Update(0.0f);
		}
	}

	void SkinningMeshComponentViewer::UpdateEditSkeletonButtonCaption()
	{
		mEditSkeletonButton->caption = mEditingSkeleton ? "Stop editing skeleton" : "Edit skeleton";
	}

	void SkinningMeshComponentViewer::SceneLayer::DrawOverScene()
	{
		if (!viewer->mTypeTargetObjects.IsEmpty())
		{
			if (viewer->mFrameTool.isEnabled || viewer->mSplineTool.isEnabled || true)
			{
				auto obj = viewer->mTypeTargetObjects[0].first;

				textureSprite.SetImageAsset(obj->GetImage());
				textureSprite.SetBasis(Basis(obj->GetMappingFrame())
									   *Basis::Translated(obj->GetOwnerActor()->transform->GetWorldPosition())
									   *o2EditorSceneScreen.GetLocalToScreenTransform());
				textureSprite.SetTransparency(0.5f);
				textureSprite.Draw();
			}

			if (viewer->mTopologyTool.isEnabled || viewer->mSplineTool.isEnabled)
				DrawMeshWire();
		}
	}

	void SkinningMeshComponentViewer::SceneLayer::Update(float dt)
	{}

	int SkinningMeshComponentViewer::SceneLayer::GetOrder() const
	{
		return 0;
	}

	bool SkinningMeshComponentViewer::SceneLayer::IsEnabled() const
	{
		return viewer->mFrameTool.isEnabled || viewer->mSplineTool.isEnabled || viewer->mTopologyTool.isEnabled;
	}

	const String& SkinningMeshComponentViewer::SceneLayer::GetName() const
	{
		static String str("mesh texture overlay");
		return str;
	}

	const String& SkinningMeshComponentViewer::SceneLayer::GetIconName() const
	{
		return String::empty;
	}

	void SkinningMeshComponentViewer::SceneLayer::DrawMeshWire()
	{
		if (!viewer->mTypeTargetObjects.IsEmpty())
		{
			auto& mesh = viewer->mTypeTargetObjects[0].first->GetMesh();

			Color4 wireColor(0, 0, 0, 100);
			Vector<Vertex> verticies;
			for (int i = 0; i < mesh.polyCount; i++)
			{
				auto v = o2EditorSceneScreen.LocalToScreenPoint(mesh.vertices[mesh.indexes[i*3]]);
				auto v1 = o2EditorSceneScreen.LocalToScreenPoint(mesh.vertices[mesh.indexes[i*3 + 1]]);
				auto v2 = o2EditorSceneScreen.LocalToScreenPoint(mesh.vertices[mesh.indexes[i*3 + 2]]);

				verticies.Clear();
				verticies.Add(Vertex(v.x, v.y, 0.0f, wireColor.ABGR(), 0.0f, 0.0f));
				verticies.Add(Vertex(v1.x, v1.y, 0.0f, wireColor.ABGR(), 0.0f, 0.0f));
				verticies.Add(Vertex(v2.x, v2.y, 0.0f, wireColor.ABGR(), 0.0f, 0.0f));
				verticies.Add(Vertex(v.x, v.y, 0.0f, wireColor.ABGR(), 0.0f, 0.0f));
				o2Render.DrawPolyLine(verticies.Data(), verticies.Count());
			}
		}
	}

}

DECLARE_TEMPLATE_CLASS(Editor::TObjectPropertiesViewer<SkinningMeshComponent>);

DECLARE_CLASS(Editor::SkinningMeshComponentViewer);
