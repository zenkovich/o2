#include "o2Editor/stdafx.h"
#include "Vec2KeyFramesTrackControl.h"
#include "o2Editor/Core/ToolsPanel.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
    Vec2KeyFramesTrackControl* Vec2KeyFramesTrackControl::mLastActive = nullptr;

    Vec2KeyFramesTrackControl::Vec2KeyFramesTrackControl(): Base()
    {
        InitializeControls();
    }

    Vec2KeyFramesTrackControl::Vec2KeyFramesTrackControl(const Vec2KeyFramesTrackControl& other): Base(other)
    {
        InitializeControls();
    }

    Vec2KeyFramesTrackControl::~Vec2KeyFramesTrackControl()
    {
        o2EditorSceneScreen.RemoveTool(&mTool);
    }

    void Vec2KeyFramesTrackControl::SetActive(bool active)
    {
        if (active)
        {
            if (mLastActive)
                mLastActive->SetActive(false);

            o2EditorSceneScreen.AddTool(&mTool);
            mPrevSelectedTool = o2EditorSceneScreen.GetSelectedTool();
            o2EditorSceneScreen.SelectTool<SplineTool>();

            mLastActive = this;
        }
        else
        {
            if (o2EditorSceneScreen.GetSelectedTool() == &mTool)
                o2EditorSceneScreen.SelectTool(mPrevSelectedTool);

            o2EditorSceneScreen.RemoveTool(&mTool);

            if (mLastActive == this)
                mLastActive = nullptr;
        }
    }

    Vec2KeyFramesTrackControl& Vec2KeyFramesTrackControl::operator=(const Vec2KeyFramesTrackControl& other)
    {
        Base::operator=(other);
        return *this;
    }

    void Vec2KeyFramesTrackControl::InitializeControls()
    {}

    void Vec2KeyFramesTrackControl::TryFindOwnerTrack()
    {
        mTrackOwner = nullptr;

        if (!mPlayer)
            return;

        Ref<String> path = mTrack->path;
        Ref<Actor> root = Ref<Actor>::DynamicCast(mPlayer->GetOwnerPlayer().Lock()->GetTarget());
        while (root && path->StartsWith("child"))
        {
            int nextSlash = path->Find("/");
            if (nextSlash < 0)
                return;

            int nextChildSlash = path->Find("/", nextSlash + 1);
            Ref<String> childName = path->SubStr(nextSlash + 1, nextChildSlash);
            root = root->GetChild(childName);
            path->Erase(0, nextChildSlash + 1);
        }

        mTrackOwner = root;
    }

    void Vec2KeyFramesTrackControl::OnSetTrack()
    {
        TryFindOwnerTrack();

        mTool.SetSpline(&mTrack->spline, [&]() {
            if (mTrackOwner && mTrackOwner->GetParent())
                return mTrackOwner->GetParent()->transform->worldPosition.Get(); 

            return Vec2F();
        });
    }

    void Vec2KeyFramesTrackControl::OnKeysChanged()
    {
        mTool.splineEditor.OnSplineChanged();
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
            Ref<KeyFrame>& key = mTrack->timeCurve.GetKeys()[i];
            Ref<KeyFrame>& prevKey = mTrack->timeCurve.GetKeys()[i - 1];

            Ref<Basis> drawCoords = Ref<Basis>::Mmake(RectF(mTimeline->LocalToWorld(prevKey->position) - 3,
                                layout->GetWorldTop() - 5,
                                mTimeline->LocalToWorld(key->position) - 3,
                                layout->GetWorldBottom() + 5));

            DrawCurveInCoords(key->GetApproximatedPoints(), key->GetApproximatedPointsCount(),
                              key->GetGetApproximatedPointsBounds(), drawCoords, Color4(44, 62, 80));
        }

        for (Ref<Handle> handle : mHandles)
            handle->handle->Draw();

        o2Render.DisableScissorTest();

        DrawDebugFrame();
    }
}
// --- META ---

DECLARE_CLASS(Editor::Vec2KeyFramesTrackControl, Editor__Vec2KeyFramesTrackControl);
// --- END META ---