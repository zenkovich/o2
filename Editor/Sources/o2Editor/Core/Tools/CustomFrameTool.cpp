#include "o2Editor/stdafx.h"
#include "CustomFrameTool.h"

#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	CustomFrameTool::CustomFrameTool()
	{
		sceneLayer.tool = this;
		frameHandles.onTransformed = [&](const Basis& x) {
			mBasis = x*o2EditorSceneScreen.GetScreenToLocalTransform();
			onChanged(mBasis);
		};
	}

	void CustomFrameTool::SceneLayer::DrawOverScene()
	{
		tool->frameHandles.SetBasis(tool->mBasis*o2EditorSceneScreen.GetLocalToScreenTransform());
		tool->frameHandles.Draw();
	}

	void CustomFrameTool::SceneLayer::Update(float dt)
	{
	}

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

	String CustomFrameTool::CustomFrameTool::GetPanelIcon() const
	{
		return "ui/UI4_frame_tool.png";
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