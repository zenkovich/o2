#include "Ref.h"
#include "WeakRef.h"

#include "o2Editor/stdafx.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
    class CustomFrameTool
    {
    public:
        CustomFrameTool();

        void DrawOverScene();
        void Update(float dt);
        int GetOrder() const;
        bool IsEnabled() const;
        const String& GetName() const;
        const String& GetIconName() const;

        void SetFrame(const Basis& frame);
        const Basis& GetFrame() const;
        void Reset();

        String GetPanelIcon() const;
        void OnEnabled();
        void OnDisabled();

    private:
        Ref<SceneLayer> sceneLayer;
        Ref<FrameHandles> frameHandles;
        Ref<onTransformed> onTransformed;
        Ref<Basis> mBasis;
        Ref<getOrigin> getOrigin;
        bool isEnabled;

        // ...
    };

    class SceneLayer
    {
    public:
        SceneLayer(CustomFrameTool* t) : tool(t) {}

        void DrawOverScene();
        void Update(float dt);
        int GetOrder() const;
        bool IsEnabled() const;
        const String& GetName() const;
        const String& GetIconName() const;

    private:
        WeakRef<CustomFrameTool> tool;

        // ...
    };

    CustomFrameTool::CustomFrameTool()
    {
        sceneLayer = mmake<SceneLayer>(this);
        frameHandles = mmake<FrameHandles>();
        onTransformed = [&](const Basis& x) {
            auto b = x * o2EditorSceneScreen.GetScreenToLocalTransform();
            b.origin -= getOrigin();
            mBasis = b;
            onChanged(mBasis);
        };
    }

    void SceneLayer::DrawOverScene()
    {
        Ref<Basis> b = tool->mBasis;
        b->origin += tool->getOrigin();

        tool->frameHandles->SetBasis(*b * o2EditorSceneScreen.GetLocalToScreenTransform());
        tool->frameHandles->Draw();
    }

    void SceneLayer::Update(float dt)
    {}

    int SceneLayer::GetOrder() const
    {
        return 0;
    }

    bool SceneLayer::IsEnabled() const
    {
        return tool->isEnabled;
    }

    const String& SceneLayer::GetName() const
    {
        static const Ref<String> res = mmake<String>("Custom frame");
        return *res;
    }

    const String& SceneLayer::GetIconName() const
    {
        static const Ref<String> res = mmake<String>();
        return *res;
    }

    void CustomFrameTool::SetFrame(const Basis& frame)
    {
        mBasis = frame;
    }

    const Basis& CustomFrameTool::GetFrame() const
    {
        return *mBasis;
    }

    void CustomFrameTool::Reset()
    {
        onChanged.Clear();
        getOrigin.Clear();
    }

    String CustomFrameTool::GetPanelIcon() const
    {
        return "ui/UI4_image_frame_tool.png";
    }

    void CustomFrameTool::OnEnabled()
    {
        o2EditorSceneScreen.AddEditorLayer(sceneLayer);
        isEnabled = true;
    }

    void CustomFrameTool::OnDisabled()
    {
        o2EditorSceneScreen.RemoveEditorLayer(sceneLayer);
        isEnabled = false;
    }
}

// --- META ---

DECLARE_CLASS(Editor::CustomFrameTool, Editor__CustomFrameTool);

// --- END META ---