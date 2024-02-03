#include "o2Editor/stdafx.h"
#include "SkinningMeshComponentViewer.h"

#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/Tools/SkeletonTool.h"
#include "o2Editor/SceneWindow/SceneDragHandle.h"
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
		o2EditorProperties.BuildObjectProperties(mSpoiler, MakeRef<SkinningMeshComponent>(), std::move(mPropertiesContext), "",
			std::move(mOnChildFieldChangeCompleted), onChanged);

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
			mSplineTool.SetSpline(MakeRef(mTypeTargetObjects[0].first->spline), getOrigin);
			mSplineTool.onChanged = [&]() { mTypeTargetObjects[0].first->GetOwnerActor()->OnChanged(); };

			// Frame tool
			mFrameTool.SetFrame(Basis(mTypeTargetObjects[0].first->GetMappingFrame()));
			mFrameTool.frameHandles.SetRotationEnabled(false);
			mFrameTool.getOrigin = getOrigin;
			mFrameTool.onChanged = [&](const Basis& b) {
				mTypeTargetObjects[0].first->SetMappingFrame(MakeRef(b.AABB()));
				mTypeTargetObjects[0].first->GetOwnerActor()->OnChanged();
			};

			mFrameTetxureLayer.mesh = MakeRef(mTypeTargetObjects[0].first);

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
			mEditingSkeleton = mSkeletonTool->IsEditingSkeleton(MakeRef(mTypeTargetObjects[0].first));

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

			component->SetMappingFrame(MakeRef(RectF(size * -0.5f, size * 0.5f)));

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
				mSkeletonTool = mmake<SkeletonTool>();
				o2EditorSceneScreen.AddTool(MakeRef(mSkeletonTool));
			}

			mSkeletonTool->AddSkeletonInstance(MakeRef(component));

			o2EditorSceneScreen.SelectTool<SkeletonTool>();
		}
		else
		{
			if (mSkeletonTool)
			{
				mSkeletonTool->RemoveSkeletonInstance(MakeRef(component));

				if (mSkeletonTool->GetE#include <memory>

// Ref class
template<typename T>
using Ref = std::shared_ptr<T>;

// WeakRef class
template<typename T>
using WeakRef = std::weak_ptr<T>;

class SkinningMeshComponentViewer : public TObjectPropertiesViewer<SkinningMeshComponent>
{
private:
	Ref<SkeletonTool> mSkeletonTool;
	Ref<Button> mEditSkeletonButton;
	bool mEditingSkeleton;

public:
	SkinningMeshComponentViewer()
		: mSkeletonTool(nullptr),
		mEditSkeletonButton(nullptr),
		mEditingSkeleton(false)
	{
	}

	virtual ~SkinningMeshComponentViewer() {}

	virtual void OnInitialize() override
	{
		// Initialize mEditSkeletonButton
		mEditSkeletonButton = mmake<Button>();
		mEditSkeletonButton->caption = "Edit skeleton";
		mEditSkeletonButton->onPress = std::bind(&SkinningMeshComponentViewer::OnEditSkeletonPressed, this);
	}

	virtual void OnFinalize() override
	{
		// If mSkeletonTool exists, remove it from the scene screen
		if (mSkeletonTool)
		{
			o2EditorSceneScreen.RemoveTool(mSkeletonTool);
			mSkeletonTool = nullptr;
		}

		UpdateEditSkeletonButtonCaption();
	}

	void OnEditSkeletonPressed()
	{
		// If mEditingSkeleton is true, stop editing skeleton
		if (mEditingSkeleton)
		{
			mEditingSkeleton = false;

			// If mSkeletonTool exists, remove it from the scene screen
			if (mSkeletonTool)
			{
				o2EditorSceneScreen.RemoveTool(mSkeletonTool);
				mSkeletonTool = nullptr;
			}
		}
		else // Start editing skeleton
		{
			// Create a new SkeletonTool using mmake<>
			mSkeletonTool = mmake<SkeletonTool>();

			// Add mSkeletonTool to the scene screen
			o2EditorSceneScreen.AddTool(mSkeletonTool);

			mEditingSkeleton = true;
		}

		UpdateEditSkeletonButtonCaption();
	}

	void OnReskinPressed()
	{
		// Update bones for each target object and call OnUpdate()
		for (auto targetPair : mTypeTargetObjects)
		{
			targetPair.first->NeedUpdateBones();
			targetPair.first->OnUpdate(0.0f);
		}
	}

	void UpdateEditSkeletonButtonCaption()
	{
		// Set the caption for mEditSkeletonButton based on mEditingSkeleton
		mEditSkeletonButton->caption = mEditingSkeleton ? "Stop editing skeleton" : "Edit skeleton";
	}
};

DECLARE_TEMPLATE_CLASS(Editor::TObjectPropertiesViewer<SkinningMeshComponent>);
// --- META ---

DECLARE_CLASS(Editor::SkinningMeshComponentViewer, Editor__SkinningMeshComponentViewer);
// --- END META ---