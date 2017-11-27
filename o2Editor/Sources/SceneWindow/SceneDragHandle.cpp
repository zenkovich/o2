#include "stdafx.h"
#include "SceneDragHandle.h"

#include "Application/Application.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "Render/Sprite.h"
#include "SceneWindow/SceneEditScreen.h"
#include "SceneWindow/SceneWindow.h"

namespace Editor
{
	SceneDragHandle::SceneDragHandle():
		DragHandle()
	{
		if (WindowsManager::IsSingletonInitialzed())
		{
			o2EditorSceneScreen.mDragHandles.Add(this);
			messageFallDownListener = (UIScrollView*)SceneEditScreen::InstancePtr();
		}
	}

	SceneDragHandle::SceneDragHandle(Sprite* regular, Sprite* hover /*= nullptr*/, Sprite* pressed /*= nullptr*/):
		DragHandle(regular, hover, pressed)
	{
		if (WindowsManager::IsSingletonInitialzed())
		{
			o2EditorSceneScreen.mDragHandles.Add(this);
			messageFallDownListener = (UIScrollView*)SceneEditScreen::InstancePtr();
		}
	}

	SceneDragHandle::SceneDragHandle(const SceneDragHandle& other):
		DragHandle(other)
	{
		if (WindowsManager::IsSingletonInitialzed())
		{
			o2EditorSceneScreen.mDragHandles.Add(this);
			messageFallDownListener = (UIScrollView*)SceneEditScreen::InstancePtr();
		}
	}

	SceneDragHandle::~SceneDragHandle()
	{
		if (WindowsManager::IsSingletonInitialzed())
			o2EditorSceneScreen.mDragHandles.Remove(this);
	}

	SceneDragHandle& SceneDragHandle::operator=(const SceneDragHandle& other)
	{
		DragHandle::operator=(other);
		return *this;
	}

	Vec2F SceneDragHandle::ScreenToLocal(const Vec2F& point)
	{
		return o2EditorSceneScreen.ScreenToScenePoint(DragHandle::ScreenToLocal(point));
	}

	Vec2F SceneDragHandle::LocalToScreen(const Vec2F& point)
	{
		return o2EditorSceneScreen.SceneToScreenPoint(DragHandle::LocalToScreen(point));
	}

	void SceneDragHandle::SetEnabled(bool enable)
	{
		if (mEnabled == enable)
			return;

		DragHandle::SetEnabled(enable);

		if (mEnabled)
			o2EditorSceneScreen.mDragHandles.Add(this);
		else
			o2EditorSceneScreen.mDragHandles.Remove(this);
	}
}

DECLARE_CLASS(Editor::SceneDragHandle);
