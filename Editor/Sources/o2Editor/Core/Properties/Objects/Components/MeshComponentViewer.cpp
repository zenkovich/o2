#include "o2Editor/stdafx.h"
#include "MeshComponentViewer.h"

#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	MeshComponentViewer::MeshComponentViewer() = default;

	MeshComponentViewer::~MeshComponentViewer()
	{
		o2EditorSceneScreen.RemoveTool(&mSplineTool);
		o2EditorSceneScreen.RemoveTool(&mFrameTool);
		o2EditorSceneScreen.RemoveTool(&mTopologyTool);
		o2EditorSceneScreen.RemoveEditorLayer(&mFrameTetxureLayer);
	}

	MeshComponentViewer& MeshComponentViewer::operator=(const MeshComponentViewer& other)
	{
		TObjectPropertiesViewer<MeshComponent>::operator=(other);
		return *this;
	}

	void MeshComponentViewer::RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		o2EditorProperties.BuildObjectProperties(mSpoiler, &TypeOf(MeshComponent), mPropertiesContext, "",
												 mOnChildFieldChangeCompleted, onChanged);

		mFitAndCenterButton = o2UI.CreateButton("Fit and centerize", THIS_FUNC(FitAndCenterize));

		mSpoiler->AddChild(mFitAndCenterButton);
	}

	void MeshComponentViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		auto prevTargetObjects = mTypeTargetObjects;

		TObjectPropertiesViewer<MeshComponent>::OnRefreshed(targetObjets);

		if (!mTypeTargetObjects.IsEmpty() && prevTargetObjects != mTypeTargetObjects)
		{
			Function<Vec2F()> getOrigin = [&]() {
				return mTypeTargetObjects[0].first->GetOwnerActor()->transform->GetWorldNonSizedBasis().origin;
			};

			mSplineTool.SetSpline(&mTypeTargetObjects[0].first->spline, getOrigin);
			mSplineTool.onChanged = [&]() { mTypeTargetObjects[0].first->GetOwnerActor()->OnChanged(); };

			mFrameTool.SetFrame(Basis(mTypeTargetObjects[0].first->GetMappingFrame()));
			mFrameTool.frameHandles.SetRotationEnabled(false);
			mFrameTool.getOrigin = getOrigin;
			mFrameTool.onChanged = [&](const Basis& b) {
				mTypeTargetObjects[0].first->SetMappingFrame(b.AABB());
				mTypeTargetObjects[0].first->GetOwnerActor()->OnChanged();
			};

			mFrameTetxureLayer.viewer = this;

			mTopologyTool.SetMeshComponent(mTypeTargetObjects[0].first);
		}
	}

	void MeshComponentViewer::OnEnabled()
	{
		o2EditorSceneScreen.AddTool(&mSplineTool);
		o2EditorSceneScreen.AddTool(&mFrameTool);
		o2EditorSceneScreen.AddTool(&mTopologyTool);

		mPrevSelectedTool = o2EditorSceneScreen.GetSelectedTool();
		o2EditorSceneScreen.SelectTool<SplineTool>();

		o2EditorSceneScreen.AddEditorLayer(&mFrameTetxureLayer);
	}

	void MeshComponentViewer::OnDisabled()
	{
		auto selectedTool = o2EditorSceneScreen.GetSelectedTool();
		if (selectedTool == &mSplineTool || selectedTool == &mFrameTool || selectedTool == &mTopologyTool)
			o2EditorSceneScreen.SelectTool(mPrevSelectedTool);

		o2EditorSceneScreen.RemoveTool(&mSplineTool);
		o2EditorSceneScreen.RemoveTool(&mFrameTool);
		o2EditorSceneScreen.RemoveTool(&mTopologyTool);

		o2EditorSceneScreen.RemoveEditorLayer(&mFrameTetxureLayer);
	}

	void MeshComponentViewer::FitAndCenterize()
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

	void MeshComponentViewer::SceneLayer::DrawOverScene()
	{
		if (!viewer->mTypeTargetObjects.IsEmpty())
		{
			if (viewer->mFrameTool.isEnabled || viewer->mSplineTool.isEnabled)
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

	void MeshComponentViewer::SceneLayer::Update(float dt)
	{}

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
		static String str("mesh texture overlay");
		return str;
	}

	const String& MeshComponentViewer::SceneLayer::GetIconName() const
	{
		return String::empty;
	}

	void MeshComponentViewer::SceneLayer::DrawMeshWire()
	{
		if (!viewer->mTypeTargetObjects.IsEmpty())
		{
			auto& mesh = viewer->mTypeTargetObjects[0].first->GetMesh();

			Color4 wireColor(0, 0, 0, 100);
			Vector<Vertex2> verticies;
			for (int i = 0; i < mesh.polyCount; i++)
			{
				auto v = o2EditorSceneScreen.LocalToScreenPoint(mesh.vertices[mesh.indexes[i*3]]);
				auto v1 = o2EditorSceneScreen.LocalToScreenPoint(mesh.vertices[mesh.indexes[i*3 + 1]]);
				auto v2 = o2EditorSceneScreen.LocalToScreenPoint(mesh.vertices[mesh.indexes[i*3 + 2]]);

				verticies.Clear();
				verticies.Add(Vertex2(v.x, v.y, 0.0f, wireColor.ARGB(), 0.0f, 0.0f));
				verticies.Add(Vertex2(v1.x, v1.y, 0.0f, wireColor.ARGB(), 0.0f, 0.0f));
				verticies.Add(Vertex2(v2.x, v2.y, 0.0f, wireColor.ARGB(), 0.0f, 0.0f));
				o2Render.DrawPolyLine(verticies.Data(), verticies.Count());
			}
		}
	}

	MeshTopologyTool::MeshTopologyTool():
		mSelectionSprite("ui/UI_Window_place.png")
	{
		sceneLayer.tool = this;

		mHandleSample = DragHandle(mnew Sprite("ui/CurveHandle.png"),
								   mnew Sprite("ui/CurveHandleHover.png"),
								   mnew Sprite("ui/CurveHandlePressed.png"),
								   mnew Sprite("ui/CurveHandleSelected.png"));

		typedef MeshTopologyTool thisclass;

		mTransformFrame.SetPivotEnabled(false);
		mTransformFrame.SetRotationEnabled(false);
		mTransformFrame.onTransformed = THIS_FUNC(OnTransformFrameTransformed);
// 		mTransformFrame.onPressed = THIS_FUNC(OnTransformBegin);
// 		mTransformFrame.onChangeCompleted = THIS_FUNC(OnTransformCompleted);
		mTransformFrame.isInputTransparent = true;
	}

	MeshTopologyTool::~MeshTopologyTool()
	{
		ClearHandles();
	}

	void MeshTopologyTool::SetMeshComponent(MeshComponent* mesh)
	{
		this->mesh = mesh;

		InitializeHandles();
	}

	String MeshTopologyTool::GetPanelIcon() const
	{
		return "ui/TopologyTool.png";
	}

	void MeshTopologyTool::OnEnabled()
	{
		o2EditorSceneScreen.AddEditorLayer(&sceneLayer);
		isEnabled = true;
	}

	void MeshTopologyTool::OnDisabled()
	{
		o2EditorSceneScreen.RemoveEditorLayer(&sceneLayer);
		isEnabled = false;
	}

	bool MeshTopologyTool::IsUnderPoint(const Vec2F& point)
	{
		return true;
	}

	bool MeshTopologyTool::IsInputTransparent() const
	{
		return true;
	}

	void MeshTopologyTool::InitializeHandles()
	{
		ClearHandles();

		if (!mesh)
			return;

		for (int i = 0; i < mesh->GetExtraPoints().Count(); i++)
		{
			mHandles.Add(mHandleSample.CloneAs<DragHandle>());
			auto& newHandle = mHandles.Last();

			newHandle->SetPosition(mesh->GetExtraPoints()[i]);
			newHandle->SetSelectionGroup(this);
			newHandle->onChangedPos = [=](const Vec2F& pos) { OnHandleMoved(i, pos); };
			newHandle->localToScreenTransformFunc = [&](const Vec2F& p) { return LocalToWorld(p); };
			newHandle->screenToLocalTransformFunc = [&](const Vec2F& p) { return WorldToLocal(p); };
		}
	}

	void MeshTopologyTool::ClearHandles()
	{
		for (auto handle : mHandles)
			delete handle;

		mHandles.Clear();
	}

	void MeshTopologyTool::OnHandleMoved(int i, const Vec2F& pos)
	{
		if (!mesh)
			return;

		mesh->SetExtraPoint(i, pos); 
		mesh->GetOwnerActor()->OnChanged();
		UpdateTransformFrame();
	}

	Vec2F MeshTopologyTool::WorldToLocal(const Vec2F& point) const
	{
		if (!mesh)
			return point;

		Basis transform = mesh->GetOwnerActor()->transform->GetWorldNonSizedBasis();
		return o2EditorSceneScreen.ScreenToLocalPoint(point)*transform.Inverted();
	}

	Vec2F MeshTopologyTool::LocalToWorld(const Vec2F& point) const
	{
		if (!mesh)
			return point;

		Basis transform = mesh->GetOwnerActor()->transform->GetWorldNonSizedBasis();
		return o2EditorSceneScreen.LocalToScreenPoint(point*transform);
	}

	void MeshTopologyTool::DrawSelection()
	{
		if (mIsPressed)
		{
			mSelectionSprite.rect = RectF(LocalToWorld(mSelectingPressedPoint), o2Input.cursorPos);
			mSelectionSprite.Draw();
		}
	}

	void MeshTopologyTool::DrawTransformFrame()
	{
		if (!mTransformFrameVisible)
			return;

		Vec2F worldOrig = LocalToWorld(mTransformFrameBasis.origin);
		Vec2F worldXV = LocalToWorld(mTransformFrameBasis.xv + mTransformFrameBasis.origin) - worldOrig;
		Vec2F worldYV = LocalToWorld(mTransformFrameBasis.yv + mTransformFrameBasis.origin) - worldOrig;
		mTransformFrame.SetBasis(Basis(worldOrig - mTransformBasisOffet,
								 worldXV + Vec2F(mTransformBasisOffet.x*2.0f, 0),
								 worldYV + Vec2F(0, mTransformBasisOffet.y*2.0f)));

		mTransformFrame.Draw();
	}

	void MeshTopologyTool::UpdateTransformFrame()
	{
		mTransformFrameVisible = IsTransformFrameVisible();

		if (!mTransformFrameVisible || mSelectedHandles.IsEmpty())
			return;

		RectF aabb((mSelectedHandles[0])->GetPosition(), (mSelectedHandles[0])->GetPosition());

		for (auto handle : mSelectedHandles)
		{
			aabb.left = Math::Min(handle->GetPosition().x, aabb.left);
			aabb.right = Math::Max(handle->GetPosition().x, aabb.right);
			aabb.top = Math::Max(handle->GetPosition().y, aabb.top);
			aabb.bottom = Math::Min(handle->GetPosition().y, aabb.bottom);
		}

		mTransformFrameBasis = Basis(aabb.LeftBottom(), Vec2F::Right()*aabb.Width(), Vec2F::Up()*aabb.Height());
	}

	bool MeshTopologyTool::IsTransformFrameVisible() const
	{
		return mSelectedHandles.Count() > 1;
	}

	void MeshTopologyTool::OnTransformFrameTransformed(const Basis& basis)
	{
		Vec2F localBasisOrig = WorldToLocal(basis.origin + mTransformBasisOffet);
		Vec2F localBasisXV = WorldToLocal(basis.xv - Vec2F(mTransformBasisOffet.x*2.0f, 0) + basis.origin + mTransformBasisOffet) - localBasisOrig;
		Vec2F localBasisYV = WorldToLocal(basis.yv - Vec2F(0, mTransformBasisOffet.y*2.0f) + basis.origin + mTransformBasisOffet) - localBasisOrig;
		Basis localBasis(localBasisOrig, localBasisXV, localBasisYV);

		Basis delta = mTransformFrameBasis.Inverted()*localBasis;

		if (delta.origin.Length() > 0.01f || delta.xv != Vec2F(1, 0) || delta.yv != Vec2F(0, 1))
		{
			Vector<Vec2F> newPosHandlesPositions, newSupportnHandlesPositions;

			for (int i = 0; i < mSelectedHandles.Count(); i++)
				newPosHandlesPositions.Add(mSelectedHandles[i]->GetPosition()*delta);

			for (int i = 0; i < mSelectedHandles.Count(); i++)
			{
				mSelectedHandles[i]->SetPosition(newPosHandlesPositions[i]);
				mSelectedHandles[i]->onChangedPos(mSelectedHandles[i]->GetPosition());
			}

			UpdateTransformFrame();
		}
	}

	void MeshTopologyTool::OnCursorPressed(const Input::Cursor& cursor)
	{
		mSelectingPressedPoint = WorldToLocal(cursor.position);

		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAll();
	}

	void MeshTopologyTool::OnCursorReleased(const Input::Cursor& cursor)
	{
		for (auto handle : mSelectingHandlesBuf)
		{
			SetHandleSelectedState(handle, false);
			handle->SetSelected(true);
		}

		mSelectingHandlesBuf.Clear();
		UpdateTransformFrame();
	}

	void MeshTopologyTool::OnCursorStillDown(const Input::Cursor& cursor)
	{
		for (auto handle : mSelectingHandlesBuf)
			SetHandleSelectedState(handle, false);

		mSelectingHandlesBuf.Clear();

		RectF selectionLocalRect(mSelectingPressedPoint, WorldToLocal(cursor.position));

		for (auto handle : mHandles)
		{
			if (handle->IsEnabled() && selectionLocalRect.IsInside(handle->GetPosition()) &&
				!mSelectedHandles.Contains(handle))
			{
				mSelectingHandlesBuf.Add(handle);
				SetHandleSelectedState(handle, true);
			}
		}
	}

	void MeshTopologyTool::OnCursorDblClicked(const Input::Cursor& cursor)
	{
		if (!mesh)
			return;

		auto local = WorldToLocal(cursor.position);
		mesh->AddExtraPoint(local);
		mesh->GetOwnerActor()->OnChanged();
		InitializeHandles();
	}

	void MeshTopologyTool::SceneLayer::DrawOverScene()
	{
		tool->OnDrawn();

		tool->DrawTransformFrame();

		for (auto handle : tool->mHandles)
			handle->Draw();

		tool->DrawSelection();
	}

	void MeshTopologyTool::SceneLayer::Update(float dt)
	{

	}

	int MeshTopologyTool::SceneLayer::GetOrder() const
	{
		return 0;
	}

	bool MeshTopologyTool::SceneLayer::IsEnabled() const
	{
		return tool->isEnabled;
	}

	const String& MeshTopologyTool::SceneLayer::GetName() const
	{
		static String res("Topology");
		return res;
	}

	const String& MeshTopologyTool::SceneLayer::GetIconName() const
	{
		return String::empty;
	}
}

DECLARE_TEMPLATE_CLASS(Editor::TObjectPropertiesViewer<MeshComponent>);

DECLARE_CLASS(Editor::MeshComponentViewer);
