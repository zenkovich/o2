#include "SelectionTool.h"

#include "Core/Actions/Selection.h"
#include "Core/EditorApplication.h"
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
		o2EditorSceneScreen.SelectActorsWithoutAction(mCurrentSelectingActors, true);
		mCurrentSelectingActors.Clear();
		mSelectingActors = false;

		auto selectionAction = mnew EditorSelectionAction(o2EditorSceneScreen.GetSelectedActors(), mBeforeSelectingActors);
		o2EditorApplication.DoneAction(selectionAction);
	}
	else
	{
		bool selected = false;
		Vec2F sceneSpaceCursor = o2EditorSceneScreen.ScreenToScenePoint(cursor.mPosition);
		for (auto layer : o2Scene.GetLayers())
		{
			for (auto actor : layer->enabledActors)
			{
				if (!actor->IsLockedInHierarchy() && actor->transform.IsPointInside(sceneSpaceCursor))
				{
					mBeforeSelectingActors = o2EditorSceneScreen.GetSelectedActors();

					if (!o2Input.IsKeyDown(VK_SHIFT))
						o2EditorSceneScreen.ClearSelectionWithoutAction();

					o2EditorSceneScreen.SelectActorWithoutAction(actor);
					o2EditorTree.ExpandActorsTreeNode(actor);
					selected = true;

					auto selectionAction = mnew EditorSelectionAction(o2EditorSceneScreen.GetSelectedActors(), 
																	  mBeforeSelectingActors);
					o2EditorApplication.DoneAction(selectionAction);
					break;
				}
			}
		}

		if (!o2Input.IsKeyDown(VK_SHIFT) && !selected)
			o2EditorSceneScreen.ClearSelection();
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

		mBeforeSelectingActors = o2EditorSceneScreen.GetSelectedActors();

		if (!o2Input.IsKeyDown(VK_SHIFT))
			o2EditorSceneScreen.ClearSelectionWithoutAction();
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

void EditorSelectionTool::OnKeyPressed(const Input::Key& key)
{
	if (key == VK_ESCAPE)
		o2EditorSceneScreen.ClearSelection();

	if (key == 'A' && o2Input.IsKeyDown(VK_CONTROL))
		o2EditorSceneScreen.SelectAllActors();
}
