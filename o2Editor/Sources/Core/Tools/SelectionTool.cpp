#include "SelectionTool.h"

#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/TreeWindow.h"

EditorSelectionTool::EditorSelectionTool()
{
	mSelectionSprite = mnew Sprite("ui/UI_Window_place.png");
}

EditorSelectionTool::~EditorSelectionTool()
{
	delete mSelectionSprite;
}

void EditorSelectionTool::DrawScene()
{
	for (auto actor : mCurrentSelectingActors)
		o2EditorSceneScreen.DrawActorSelection(actor, o2EditorSceneScreen.GetManyActorsSelectionColor());
}

void EditorSelectionTool::DrawScreen()
{
	if (mSelectingActors)
		mSelectionSprite->Draw();
}

void EditorSelectionTool::Update(float dt)
{}

void EditorSelectionTool::OnEnabled()
{}

void EditorSelectionTool::OnDisabled()
{
	mSelectingActors = false;
}

void EditorSelectionTool::OnActorsSelectionChanged(Vector<Actor*> actors)
{}

void EditorSelectionTool::OnCursorPressed(const Input::Cursor& cursor)
{
	mPressPoint = cursor.mPosition;
}

void EditorSelectionTool::OnCursorReleased(const Input::Cursor& cursor)
{
	if (mSelectingActors)
	{
		o2EditorSceneScreen.SelectActors(mCurrentSelectingActors, true);
		mCurrentSelectingActors.Clear();
		mSelectingActors = false;
	}
	else
	{
		if (!o2Input.IsKeyDown(VK_SHIFT))
			o2EditorSceneScreen.ClearSelection();

		Vec2F sceneSpaceCursor = o2EditorSceneScreen.ScreenToScenePoint(cursor.mPosition);
		for (auto layer : o2Scene.GetLayers())
		{
			for (auto actor : layer->enabledActors)
			{
				if (!actor->IsLockedInHierarchy() && actor->transform.IsPointInside(sceneSpaceCursor))
				{
					o2EditorSceneScreen.SelectActor(actor);
					o2EditorTree.ExpandActorsTreeNode(actor);
					break;
				}
			}
		}
	}
}

void EditorSelectionTool::OnCursorPressBreak(const Input::Cursor& cursor)
{
	if (mSelectingActors)
	{
		mSelectingActors = false;
		mCurrentSelectingActors.Clear();
	}
}

void EditorSelectionTool::OnCursorStillDown(const Input::Cursor& cursor)
{
	if (!mSelectingActors && (mPressPoint - cursor.mPosition).Length() > 5.0f)
	{
		mSelectingActors = true;

		if (!o2Input.IsKeyDown(VK_SHIFT))
			o2EditorSceneScreen.ClearSelection();
	}

	if (mSelectingActors && cursor.mDelta.Length() > 0.1f)
	{
		mSelectionSprite->SetRect(RectF(mPressPoint, cursor.mPosition));
		RectF selectionRect(o2EditorSceneScreen.ScreenToScenePoint(cursor.mPosition),
							o2EditorSceneScreen.ScreenToScenePoint(mPressPoint));

		auto currentSelectedActors = mCurrentSelectingActors;
		mCurrentSelectingActors.Clear();
		for (auto actor : currentSelectedActors)
		{
			if (actor->transform.GetWorldRect().IsIntersects(selectionRect))
				mCurrentSelectingActors.Add(actor);
		}

		for (auto layer : o2Scene.GetLayers())
		{
			for (auto actor : layer->enabledActors)
			{
				if (mCurrentSelectingActors.Contains(actor))
					continue;

				if (!actor->IsLockedInHierarchy() && actor->transform.GetWorldRect().IsIntersects(selectionRect))
				{
					mCurrentSelectingActors.Add(actor);
				}
			}
		}

		mNeedRedraw = true;
	}
}

void EditorSelectionTool::OnCursorMoved(const Input::Cursor& cursor)
{}
