#include "o2Editor/stdafx.h"
#include "MeshTopologyTool.h"

#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
    class MeshTopologyTool : public ITool
    {
    public:
        MeshTopologyTool():
            mSelectionSprite("ui/UI_Window_place.png")
        {
            sceneLayer.tool = mmake<MeshTopologyTool>(Ref<AppState>());
            
            mHandleSample = mmake<DragHandle>(Ref<Sprite>(mmake<Sprite>("ui/CurveHandle.png")),
                                              Ref<Sprite>(mmake<Sprite>("ui/CurveHandleHover.png")),
                                              Ref<Sprite>(mmake<Sprite>("ui/CurveHandlePressed.png")),
                                              Ref<Sprite>(mmake<Sprite>("ui/CurveHandleSelected.png")));

            typedef MeshTopologyTool thisclass;

            mTransformFrame.SetPivotEnabled(false);
            mTransformFrame.SetRotationEnabled(false);
            mTransformFrame.onTransformed = mlambda<void(const Basis&)>([=](const Basis& basis){ OnTransformFrameTransformed(basis); });
            // mTransformFrame.onPressed = mlambda<void()>([=](){ OnTransformBegin(); });
            // mTransformFrame.onChangeCompleted = mlambda<void()>([=](){ OnTransformCompleted(); });
            mTransformFrame.isInputTransparent = true;
        }

        ~MeshTopologyTool()
        {
            ClearHandles();
        }

        void Setup(const Function<Vector<Vec2F>> getPoints, const Function<void(int, Vec2F)> setPoint,
                   const Function<Basis> getTransform, const Function<void(Vec2F)> addPoint,
                   const Function<void(int)> removePoint)
        {
            mGetPoints = getPoints;
            mSetPoint = setPoint;
            mAddPoint = addPoint;
            mRemovePoint = removePoint;
            mGetTransform = getTransform;

            InitializeHandles();
        }

        void Reset()
        {
            mGetPoints.reset();
            mSetPoint.reset();
            mAddPoint.reset();
            mRemovePoint.reset();
            mGetTransform.reset();

            ClearHandles();
        }

        String GetPanelIcon() const
        {
            return "ui/TopologyTool.png";
        }

        void OnEnabled()
        {
            o2EditorSceneScreen.AddEditorLayer(&sceneLayer);
            isEnabled = true;
        }

        void OnDisabled()
        {
            o2EditorSceneScreen.RemoveEditorLayer(&sceneLayer);
            isEnabled = false;
        }

        bool IsUnderPoint(const Vec2F& point) const
        {
            return true;
        }

        bool IsInputTransparent() const
        {
            return true;
        }

        void InitializeHandles()
        {
            ClearHandles();

            for (int i = 0; i < mGetPoints().Count(); i++)
            {
                mHandles.Add(mHandleSample.CloneAs<DragHandle>());
                auto& newHandle = mHandles.Last();

                newHandle->SetPosition(mGetPoints()[i]);
                newHandle->SetSelectionGroup(this);
                newHandle->onChangedPos = [=](const Vec2F& pos) { OnHandleMoved(i, pos); };
                newHandle->localToScreenTransformFunc = [=](const Vec2F& p) { return LocalToWorld(p); };
                newHandle->screenToLocalTransformFunc = [=](const Vec2F& p) { return WorldToLocal(p); };
            }
        }

        void ClearHandles()
        {
            mHandles.Clear();
        }

        void OnHandleMoved(int i, const Vec2F& pos)
        {
            mSetPoint(i, pos);
            UpdateTransformFrame();
        }

        Vec2F WorldToLocal(const Vec2F& point) const
        {
            Basis transform = mGetTransform();
            return o2EditorSceneScreen.ScreenToLocalPoint(point)*transform.Inverted();
        }

        Vec2F LocalToWorld(const Vec2F& point) const
        {
            Basis transform = mGetTransform();
            return o2EditorSceneScreen.LocalToScreenPoint(point*transform);
        }

        void DrawSelection() const
        {
            if (mIsPressed)
            {
                mSelectionSprite.rect = RectF(LocalToWorld(mSelectingPressedPoint), o2Input.cursorPos);
                mSelectionSprite.Draw();
            }
        }

        void DrawTransformFrame() const
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

        void UpdateTransformFrame()
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

        bool IsTransformFrameVisible() const
        {
            return mSelectedHandles.Count() > 1;
        }

        void OnTransformFrameTransformed(const Basis& basis)
        {
            Vec2F localBasisOrig = WorldToLocal(basis.origin + mTransformBasisOffet);
            Vec2F localBasisXV = WorldToLocal(basis.xv - Vec2F(mTransformBasisOffet.x*2.0f, 0) + basis.origin + mTransformBasisOffet) - localBasisOrig;
            Vec2F localBasi
    };
}
#include <memory>
#include "Ref.h"

using namespace std;

Replace raw pointers with Ref<>:

    Ref<> localBasisOrig;
    Ref<> localBasisXV;
    Ref<> localBasisYV;
    Ref<> delta.origin;
    Ref<> delta.xv;
    Ref<> delta.yv;
    Ref<Vec2F> newPosHandlesPositions;
    Ref<Vec2F> newSupportnHandlesPositions;
    Ref<> handle->GetPosition();
    Ref<> selectionLocalRect().SetHandleSelectedState(handle, false);
    Ref<> selectionLocalRect().SetHandleSelectedState(handle, true);
    Ref<> local = WorldToLocal(cursor.position);
    Ref<> tool->OnDrawn();
    Ref<> tool->DrawTransformFrame();
    Ref<> handle->Draw();
    Ref<> tool->DrawSelection();
    Ref<> res("Topology");

Use const Ref<>& for function arguments with pointer types:

    void MeshTopologyTool::TransformFrame::SetBasis(const Ref<>& frameBasis);

    void MeshTopologyTool::InitializeHandles(const Ref<>& parent);

    void MeshTopologyTool::SetHandleSelectedState(const Ref<>& handle, bool selected);

    void MeshTopologyTool::AddPoint(const Ref<>& position);

    void MeshTopologyTool::WorldToLocal(const Ref<>& worldPosition) const;

    void MeshTopologyTool::OnChangedPos(const Ref<>& position);

    void MeshTopologyTool::OnCursorPressed(const Ref<>& cursor);

    void MeshTopologyTool::OnCursorReleased(const Ref<>& cursor);

    void MeshTopologyTool::OnCursorStillDown(const Ref<>& cursor);

    void MeshTopologyTool::OnCursorDblClicked(const Ref<>& cursor);

    void MeshTopologyTool::SceneLayer::Update(float dt);

Change class members with default nullptr value to Ref<>:

    Ref<> localBasisOrig;
    Ref<> localBasisXV;
    Ref<> localBasisYV;
    Ref<> delta.origin;
    Ref<> delta.xv;
    Ref<> delta.yv;
    Ref<Vec2F> newPosHandlesPositions;
    Ref<Vec2F> newSupportnHandlesPositions;
    Ref<> handle->GetPosition();
    Ref<> selectionLocalRect().SetHandleSelectedState(handle, false);
    Ref<> selectionLocalRect().SetHandleSelectedState(handle, true);
    Ref<> local = WorldToLocal(cursor.position);
    Ref<> tool->OnDrawn();
    Ref<> tool->DrawTransformFrame();
    Ref<> handle->Draw();
    Ref<> tool->DrawSelection();
    Ref<> res("Topology");

Replace dynamic_cast<type*> with DynamicCast<type>:

    DynamicCast<type> local = WorldToLocal(cursor.position);