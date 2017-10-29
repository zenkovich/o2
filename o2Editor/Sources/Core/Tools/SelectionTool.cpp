#include "SelectionTool.h"

#include "Core/Actions/SelectActors.h"
#include "Core/EditorApplication.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "Scene/SceneLayer.h"
#include "SceneWindow/SceneEditScreen.h"
#include "TreeWindow/TreeWindow.h"

namespace Editor
{
	SelectionTool::SelectionTool()
	{
		BREAK_ON_REFLECTION_STAGE;

		mSelectionSprite = mnew Sprite("ui/UI_Window_place.png");
	}

	SelectionTool::~SelectionTool()
	{
		if (mSelectionSprite)
			delete mSelectionSprite;
	}

	void SelectionTool::DrawScene()
	{
		for (auto actor : mCurrentSelectingActors)
			o2EditorSceneScreen.DrawActorSelection(actor, o2EditorSceneScreen.GetManyActorsSelectionColor());
	}

	void SelectionTool::DrawScreen()
	{
		if (mSelectingActors)
			mSelectionSprite->Draw();
	}

	void SelectionTool::Update(float dt)
	{}

	void SelectionTool::OnEnabled()
	{}

	void SelectionTool::OnDisabled()
	{
		mSelectingActors = false;
	}

	void SelectionTool::OnActorsSelectionChanged(Vector<Actor*> actors)
	{}

	void SelectionTool::OnCursorPressed(const Input::Cursor& cursor)
	{
		mPressPoint = cursor.position;
	}

	void SelectionTool::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mSelectingActors)
		{
			o2EditorSceneScreen.SelectActorsWithoutAction(mCurrentSelectingActors, true);
			mCurrentSelectingActors.Clear();
			mSelectingActors = false;

			auto selectionAction = mnew SelectActorsAction(o2EditorSceneScreen.GetSelectedActors(), mBeforeSelectingActors);
			o2EditorApplication.DoneAction(selectionAction);
		}
		else
		{
			bool selected = false;
			Vec2F sceneSpaceCursor = o2EditorSceneScreen.ScreenToScenePoint(cursor.position);
			for (auto layer : o2Scene.GetLayers())
			{
				for (auto actor : layer->GetEnabledActors())
				{
					if (!actor->IsLockedInHierarchy() && actor->IsOnScene() && 
						actor->transform->IsPointInside(sceneSpaceCursor))
					{
						mBeforeSelectingActors = o2EditorSceneScreen.GetSelectedActors();

						if (!o2Input.IsKeyDown(VK_CONTROL))
							o2EditorSceneScreen.ClearSelectionWithoutAction(false);

						o2EditorSceneScreen.SelectActorWithoutAction(actor);
						o2EditorTree.HightlightActorsTreeNode(actor);
						selected = true;

						auto selectionAction = mnew SelectActorsAction(o2EditorSceneScreen.GetSelectedActors(),
																		  mBeforeSelectingActors);
						o2EditorApplication.DoneAction(selectionAction);
						break;
					}
				}
			}

			if (!o2Input.IsKeyDown(VK_CONTROL) && !selected)
				o2EditorSceneScreen.ClearSelection();
		}
	}

	void SelectionTool::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		if (mSelectingActors)
		{
			mSelectingActors = false;
			mCurrentSelectingActors.Clear();
		}
	}

	void SelectionTool::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (!mSelectingActors && (mPressPoint - cursor.position).Length() > 5.0f)
		{
			mSelectingActors = true;

			mBeforeSelectingActors = o2EditorSceneScreen.GetSelectedActors();

			if (!o2Input.IsKeyDown(VK_CONTROL))
				o2EditorSceneScreen.ClearSelectionWithoutAction();
		}

		if (mSelectingActors && cursor.delta.Length() > 0.1f)
		{
			mSelectionSprite->SetRect(RectF(mPressPoint, cursor.position));
			RectF selectionRect(o2EditorSceneScreen.ScreenToScenePoint(cursor.position),
								o2EditorSceneScreen.ScreenToScenePoint(mPressPoint));

			auto currentSelectedActors = mCurrentSelectingActors;
			mCurrentSelectingActors.Clear();
			for (auto actor : currentSelectedActors)
			{
				if (actor->transform->GetWorldRect().IsIntersects(selectionRect))
					mCurrentSelectingActors.Add(actor);
			}

			for (auto layer : o2Scene.GetLayers())
			{
				for (auto actor : layer->GetEnabledActors())
				{
					if (mCurrentSelectingActors.Contains(actor))
						continue;

					if (!actor->IsLockedInHierarchy() && actor->IsOnScene()
						&& actor->transform->GetWorldRect().IsIntersects(selectionRect))
					{
						mCurrentSelectingActors.Add(actor);
					}
				}
			}

			mNeedRedraw = true;
		}
	}

	void SelectionTool::OnCursorMoved(const Input::Cursor& cursor)
	{}

	void SelectionTool::OnKeyPressed(const Input::Key& key)
	{
		if (key == VK_ESCAPE)
			o2EditorSceneScreen.ClearSelection();

		if (key == 'A' && o2Input.IsKeyDown(VK_CONTROL))
			o2EditorSceneScreen.SelectAllActors();
	}

}

CLASS_META(Editor::SelectionTool)
{
	BASE_CLASS(Editor::IEditTool);

	PROTECTED_FIELD(mSelectionSprite);
	PROTECTED_FIELD(mCurrentSelectingActors);
	PROTECTED_FIELD(mBeforeSelectingActors);
	PROTECTED_FIELD(mPressPoint);
	PROTECTED_FIELD(mSelectingActors);

	PROTECTED_FUNCTION(void, DrawScene);
	PROTECTED_FUNCTION(void, DrawScreen);
	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnActorsSelectionChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
}
END_META;
