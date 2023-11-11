#include "o2Editor/stdafx.h"
#include "CustomFrameTool.h"

#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	CustomFrameTool::CustomFrameTool()
	{
		sceneLayer.tool = this;
		frameHandles.isInputTransparent = true;
		frameHandles.onTransformed = [&](const Basis& x) {
			auto b = x*o2EditorSceneScreen.GetScreenToLocalTransform();
			b.origin -= getOrigin();
			mBasis = b;
			onChanged(mBasis);
		};
	}

	void CustomFrameTool::SceneLayer::DrawOverScene()
	{
		Basis b = tool->mBasis;
		b.origin += tool->getOrigin();

		tool->frameHandles.SetBasis(b*o2EditorSceneScreen.GetLocalToScreenTransform());
		tool->frameHandles.Draw();
	}

	void CustomFrameTool::SceneLayer::Update(float dt)
	{}

	int CustomFrameTool::SceneLayer::GetOrder() const
	{
		return 0;
	}

	bool CustomFrameTool::SceneLayer::IsEnabled() const
	{
		return tool->isEnabled;
	}

	const String& CustomFrameTool::SceneLayer::GetName() const
	{
		static String res("Custom frame");
		return res;
	}

	const String& CustomFrameTool::SceneLayer::GetIconName() const
	{
		return String::empty;
	}

	void CustomFrameTool::SetFrame(const Basis& frame)
	{
		mBasis = frame;
	}

	const Basis& CustomFrameTool::GetFrame() const
	{
		return mBasis;
	}

	void CustomFrameTool::Reset()
	{
		onChanged.Clear();
		getOrigin.Clear();
	}

	String CustomFrameTool::CustomFrameTool::GetPanelIcon() const
	{
		return "ui/UI4_image_frame_tool.png";
	}

	void CustomFrameTool::CustomFrameTool::OnEnabled()
	{
		o2EditorSceneScreen.AddEditorLayer(&sceneLayer);
		isEnabled = true;
	}

	void CustomFrameTool::CustomFrameTool::OnDisabled()
	{
		o2EditorSceneScreen.RemoveEditorLayer(&sceneLayer);
		isEnabled = false;
	}
}
// --- META ---

DECLARE_CLASS(Editor::CustomFrameTool, Editor__CustomFrameTool);
// --- END META ---
