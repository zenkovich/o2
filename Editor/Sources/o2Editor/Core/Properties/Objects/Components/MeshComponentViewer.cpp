#include "o2Editor/stdafx.h"
#include "MeshComponentViewer.h"

#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	MeshComponentViewer::MeshComponentViewer() = default;

	MeshComponentViewer::~MeshComponentViewer()
	{
		o2EditorSceneScreen.RemoveTool(mSplineTool);
		o2EditorSceneScreen.RemoveTool(mFrameTool);
		o2EditorSceneScreen.RemoveTool(mTopologyTool);
		o2EditorSceneScreen.RemoveEditorLayer(mFrameTetxureLayer);
	}

	MeshComponentViewer& MeshComponentViewer::operator=(const MeshComponentViewer& other)
	{
		TObjectPropertiesViewer<MeshComponent>::operator=(other);
		return *this;
	}

	void MeshComponentViewer::RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjects)
	{
		o2EditorProperties.BuildObjectProperties(mSpoiler, Ref<TypeOf<MeshComponent>>{}, mPropertiesContext, "",
												 mOnChildFieldChangeCompleted, onChanged);

		mFitAndCenterButton = o2UI.CreateButton("Fit and centerize", THIS_FUNC(FitAndCenterize));

		mSpoiler->AddChild(mFitAndCenterButton);
	}

	void MeshComponentViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjects)
	{
		auto prevTargetObjects = mTypeTargetObjects;

		TObjectPropertiesViewer<MeshComponent>::OnRefreshed(targetObjects);

		if (!mTypeTargetObjects.IsEmpty() && prevTargetObjects != mTypeTargetObjects)
		{
			Function<Vec2F()> getOrigin = [&]() {
				return mTypeTargetObjects[0].first->GetOwnerActor()->transform->GetWorldNonSizedBasis().origin;
			};

			// Spline tool
			mSplineTool.SetSpline(mTypeTargetObjects[0].first->spline, getOrigin);
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
			mTopologyTool.Setup([&]() { return mesh->GetExtraPoints(); },
								[&](int idx, Vec2F p) { mesh->SetExtraPoint(idx, p); mesh->GetOwnerActor()->OnChanged(); },
								[&]() { return mesh->GetOwnerActor()->transform->GetWorldNonSizedBasis(); },
								[&](Vec2F p) { mesh->AddExtraPoint(p); mesh->GetOwnerActor()->OnChanged(); },
								[&](int idx) { mesh->RemoveExtraPoint(idx); mesh->GetOwnerActor()->OnChanged(); });
		}
	}

	void MeshComponentViewer::OnEnabled()
	{
		o2EditorSceneScreen.AddTool(mSplineTool);
		o2EditorSceneScreen.AddTool(mFrameTool);
		o2EditorSceneScreen.AddTool(mTopologyTool);

		mPrevSelectedTool = o2EditorSceneScreen.GetSelectedTool();
		o2EditorSceneScreen.SelectTool<SplineTool>();

		o2EditorSceneScreen.AddEditorLayer(mFrameTetxureLayer);
	}

	void MeshComponentViewer::OnDisabled()
	{
		auto selectedTool = o2EditorSceneScreen.GetSelectedTool();
		if (selectedTool == mSplineTool || selectedTool == mFrameTool || selectedTool == mTopologyTool)
			o2EditorSceneScreen.SelectTool(mPrevSelectedTool);

		o2EditorSceneScreen.RemoveTool(mSplineTool);
		o2EditorSceneScreen.RemoveTool(mFrameTool);
		o2EditorSceneScreen.RemoveTool(mTopologyTool);

		o2EditorSceneScreen.RemoveEditorLayer(mFrameTetxureLayer);
	}

	void MeshComponentViewer::FitAndCenterize()
	{
		for (auto& target : mTypeTargetObjects)
		{
			auto component = target.first;
			auto texture = component->GetImage();
			auto size = texture->GetSize();

			component->SetMappingFrame(RectF(size * -0.5f, size * 0.5f));

			mFrameTool.SetFrame(Basis(mTypeTargetObjects[0].first->GetMappingFrame()));
		}
	}

	void MeshComponentViewer::SceneLayer::DrawOverScene()
	{
		if (!viewer->mTypeTargetObjects.IsEmpty())
		{
			if (viewer->mFrameTool.isEnabled || viewer->mSplineTool.isEnabled)
			{
				auto obj = viewer->mTypeTargetObjects[0].first;

				textureSprite.SetImageAsset(obj->GetImage());
				textureSprite.SetBasis(Basis(obj->GetMappingFrame())
									   * Basis::Translated(obj->GetOwnerActor()->transform->GetWorldPosition())
									   * o2EditorSceneScreen.GetLocalToScreenTransform());
				textureSprite.SetTransparency(0.5f);
				textureSprite.Draw();
			}

			if (viewer->mTopologyTool.isEnabled || viewer->mSplineTool.isEnabled)
				DrawMeshWire();
		}
	}

	void MeshComponentViewer::SceneLayer::Update(float dt)
	{
	}

	int MeshComponentViewer::SceneLayer::GetOrder() const
	{
		return 0;
	}

	bool MeshComponentViewer::SceneLayer::IsEnabled() const
	{
		return viewer->mFrameTool.isEnabled || viewer->mSplineTool.isEnabled || viewer->mTopologyTool.isEnabled;
	}

	const String& MeshComponentViewer::SceneLayer::GetName() const
	{
		static const String str("mesh texture overlay");
		return str;
	}

	const String& MeshComponentViewer::SceneLayer::GetIconName() const
	{
		return String::empty;
	}

	void MeshComponen#include <memory>
#include <vector>

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename Derived, typename Base>
Ref<Derived> DynamicCast(const Ref<Base>& base)
{
    return std::dynamic_pointer_cast<Derived>(base);
}

// ...

class ntViewer
{
    class SceneLayer
    {
        void DrawMeshWire()
        {
            if (!viewer->mTypeTargetObjects.IsEmpty())
            {
                auto& mesh = viewer->mTypeTargetObjects[0].first->GetMesh();

                Color4 wireColor(0, 0, 0, 100);
                std::vector<Vertex> verticies;
                for (UInt i = 0; i < mesh.polyCount; i++)
                {
                    auto v = o2EditorSceneScreen.LocalToScreenPoint(mesh.vertices[mesh.indexes[i*3]]);
                    auto v1 = o2EditorSceneScreen.LocalToScreenPoint(mesh.vertices[mesh.indexes[i*3 + 1]]);
                    auto v2 = o2EditorSceneScreen.LocalToScreenPoint(mesh.vertices[mesh.indexes[i*3 + 2]]);

                    verticies.clear();
                    verticies.push_back(Vertex(v.x, v.y, 0.0f, wireColor.ARGB(), 0.0f, 0.0f));
                    verticies.push_back(Vertex(v1.x, v1.y, 0.0f, wireColor.ARGB(), 0.0f, 0.0f));
                    verticies.push_back(Vertex(v2.x, v2.y, 0.0f, wireColor.ARGB(), 0.0f, 0.0f));
                    o2Render.DrawPolyLine(verticies.data(), verticies.size());
                }
            }
        }
    };
};

DECLARE_TEMPLATE_CLASS(Editor::TObjectPropertiesViewer<Ref<MeshComponent>>);
// --- META ---

DECLARE_CLASS(Editor::MeshComponentViewer, Editor__MeshComponentViewer);
// --- END META ---