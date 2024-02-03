#include "o2Editor/stdafx.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/Scene.h"
#include "o2/Utils/Editor/SceneEditableObject.h"
#include "o2Editor/Core/Actions/Select.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/TreeWindow.h"
#include "o2Editor/Utils/Ref.h"
#include "o2Editor/Utils/WeakRef.h"

namespace Editor
{
	SelectionTool::SelectionTool()
	{
		mSelectionSprite = mmake<Ref<Sprite>>("ui/UI_Window_place.png");
	}

	SelectionTool::~SelectionTool()
	{
		if (mSelectionSprite)
			delete mSelectionSprite;
	}

	String SelectionTool::GetPanelIcon() const
	{
		return "ui/UI4_select_tool.png";
	}

	ShortcutKeys SelectionTool::GetShortcut() const
	{
		return ShortcutKeys('Q');
	}

	void SelectionTool::DrawScene()
	{
		for (const Ref<SceneEditableObject>& object : mCurrentSelectingObjects)
			o2EditorSceneScreen.DrawObjectSelection(object, o2EditorSceneScreen.GetManyObjectsSelectionColor());
	}

	void SelectionTool::DrawScreen()
	{
		if (mSelectingObjects)
			mSelectionSprite->Draw();
	}

	void SelectionTool::Update(float dt)
	{}

	void SelectionTool::OnEnabled()
	{}

	void SelectionTool::OnDisabled()
	{
		mSelectingObjects = false;
	}

	void SelectionTool::OnObjectsSelectionChanged(const Vector<Ref<SceneEditableObject>>& objects)
	{}

	void SelectionTool::OnCursorPressed(const Input::Cursor& cursor)
	{
		mPressPoint = cursor.position;
	}

	void SelectionTool::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mSelectingObjects)
		{
			o2EditorSceneScreen.SelectObjectsWithoutAction(mCurrentSelectingObjects, true);
			mCurrentSelectingObjects.clear();
			mSelectingObjects = false;

			const Ref<SelectAction>& selectionAction = mmake<Ref<SelectAction>>(o2EditorSceneScreen.GetSelectedObjects(), mBeforeSelectingObjects);
			o2EditorApplication.DoneAction(selectionAction);
		}
		else
		{
			bool selected = false;
			const Vec2F& sceneSpaceCursor = o2EditorSceneScreen.ScreenToScenePoint(cursor.position);
			const Vector<WeakRef<SceneEditableObject>> drawnObjects = o2Scene.GetDrawnEditableObjects();

			int startIdx = drawnObjects.size() - 1;
			if (!o2EditorSceneScreen.GetSelectedObjects().empty())
				startIdx = drawnObjects.IndexOf(o2EditorSceneScreen.GetSelectedObjects().back()) - 1;

			for (int i = startIdx; i >= 0; i--)
			{
				const Ref<SceneEditableObject>& object = drawnObjects[i].Lock();
				if (!object->IsLockedInHierarchy() && object->GetTransform().IsPointInside(sceneSpaceCursor))
				{
					mBeforeSelectingObjects = o2EditorSceneScreen.GetSelectedObjects();

					if (!o2Input.IsKeyDown(VK_CONTROL))
						o2EditorSceneScreen.ClearSelectionWithoutAction(false);

					o2EditorSceneScreen.SelectObjectWithoutAction(object);
					o2EditorTree.HighlightObjectTreeNode(object);
					selected = true;

					const Ref<SelectAction>& selectionAction = mmake<Ref<SelectAction>>(o2EditorSceneScreen.GetSelectedObjects(), mBeforeSelectingObjects);
					o2EditorApplication.DoneAction(selectionAction);
					break;
				}
			}

			if (!o2Input.IsKeyDown(VK_CONTROL) && !selected)
				o2EditorSceneScreen.ClearSelection();
		}
	}

	void SelectionTool::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		if (mSelectingObjects)
		{
			mSelectingObjects = false;
			mCurrentSelectingObjects.clear();
		}
	}

	void SelectionTool::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (!mSelectingObjects && (mPressPoint - cursor.position).Length() > 5.0f)
		{
			mSelectingObjects = true;

			mBeforeSelectingObjects = o2EditorSceneScreen.GetSelectedObjects();

			if (!o2Input.IsKeyDown(VK_CONTROL))
				o2EditorSceneScreen.ClearSelectionWithoutAction();
		}

		if (mSelectingObjects && cursor.delta.Length() > 0.1f)
		{
			mSelectionSprite->SetRect(RectF(mPressPoint, cursor.position));
			RectF selectionRect(o2EditorSceneScreen.ScreenToScenePoint(cursor.position),
				o2EditorSceneScreen.ScreenToScenePoint(mPressPoint));

			Vector<Ref<SceneEditableObject>> currentSelectedObjects = mCurrentSelectingObjects;
			mCurrentSelectingObjects.clear();
			for (const Ref<SceneEditableObject>& object : currentSelectedObjects)
			{
				if (object->GetTransform().AABB().IsIntersects(selectionRect))
					mCurrentSelectingObjects.push_back(object);
			}

			const Vector<WeakRef<SceneEditableObject>> drawnObjects = o2Scene.GetDrawnEditableObjects();
			for (const WeakRef<SceneEditableObject>& obj : drawnObjects)
			{
				if (mCurrentSelectingObjects.Contains(obj.Lock()))
					continue;

				const Ref<SceneEditableObject>& object = obj.Lock();
				if (!object->IsLockedInHierarchy() && object->GetTransform().AABB().IsIntersects(selectionRect))
					mCurrentSelectingObjects.push_back(object);
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
			o2EditorSceneScreen.SelectAllObjects();
	}

}
// --- META ---

DECLARE_CLASS(Editor::SelectionTool, Editor__SelectionTool);
// --- END META ---