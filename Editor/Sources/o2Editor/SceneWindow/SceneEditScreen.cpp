#include "o2Editor/stdafx.h"
#include "SceneEditScreen.h"

#include "o2/Physics/PhysicsWorld.h"
#include "o2/Render/Render.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/SceneLayer.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Utils/Math/Math.h"
#include "o2Editor/AssetsWindow/AssetsIconsScroll.h"
#include "o2Editor/Core/Actions/Select.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/Tools/IEditorTool.h"
#include "o2Editor/Core/Tools/MoveTool.h"
#include "o2Editor/Core/Tools/SelectionTool.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "o2Editor/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/SceneWindow/SceneDragHandle.h"
#include "o2Editor/TreeWindow/SceneTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

DECLARE_SINGLETON(Editor::SceneEditScreen);

namespace Editor
{

	SceneEditScreen::SceneEditScreen()
	{
		InitializeTools();
		SelectTool<MoveTool>();
	}

	SceneEditScreen::SceneEditScreen(const SceneEditScreen& other):
		SceneEditScreen()
	{}

	SceneEditScreen::~SceneEditScreen()
	{
		for (auto tool : mTools)
			delete tool;
	}

	void SceneEditScreen::Draw()
	{
		Widget::Draw();

		if (!mReady)
			return;

		if (mNeedRedraw || (mEnabledTool && mEnabledTool->mNeedRedraw))
			RedrawRenderTarget();

		mRenderTargetSprite->Draw();

		if (mEnabledTool)
			mEnabledTool->DrawScreen();

		CursorAreaEventsListener::OnDrawn();

		for (auto handle : mDragHandles)
			handle->Draw();
	}

	void SceneEditScreen::NeedRedraw()
	{
		mNeedRedraw = true;
	}

#undef DrawText

	void SceneEditScreen::Update(float dt)
	{
		Widget::Update(dt);

		UpdateCamera(dt);
		o2Scene.CheckChangedObjects();

		if (mEnabledTool)
			mEnabledTool->Update(dt);
	}

	bool SceneEditScreen::IsScrollable() const
	{
		return true;
	}

	Vec2F SceneEditScreen::ScreenToScenePoint(const Vec2F& point)
	{
		return ScreenToLocalPoint(point);
	}

	Vec2F SceneEditScreen::SceneToScreenPoint(const Vec2F& point)
	{
		return LocalToScreenPoint(point);
	}

	Vec2F SceneEditScreen::ScreenToSceneVector(const Vec2F& point)
	{
		return point*mViewCamera.GetScale();
	}

	Vec2F SceneEditScreen::SceneToScreenVector(const Vec2F& point)
	{
		return point / mViewCamera.GetScale();
	}

	void SceneEditScreen::InitializeTools(const Type* toolType /*= nullptr*/)
	{
		auto toolsTypes = toolType ? toolType->GetDerivedTypes() : TypeOf(IEditTool).GetDerivedTypes();
		for (auto toolType : toolsTypes)
		{
			mTools.Add((IEditTool*)toolType->CreateSample());
			InitializeTools(toolType);
		}
	}

	bool SceneEditScreen::IsHandleWorking(const Input::Cursor& cursor) const
	{
		return false;
	}

	void SceneEditScreen::OnObjectsSelectedFromThis()
	{
		mSelectedFromThis = true;
		mSceneTree->SetSelectedObjects(mSelectedObjects);

		if (mEnabledTool)
			mEnabledTool->OnObjectsSelectionChanged(mSelectedObjects);

		onSelectionChanged(mSelectedObjects);
		o2EditorPropertiesWindow.SetTargets(mSelectedObjects.Convert<IObject*>([](auto x) { return (IObject*)x; }));
	}

	void SceneEditScreen::RedrawContent()
	{
		DrawGrid();
 		DrawObjects();
		DrawSelection();

		if (mEnabledTool)
		{
			mEnabledTool->DrawScene();
			mEnabledTool->mNeedRedraw = false;
		}
	}

	void SceneEditScreen::DrawObjects()
	{
		if (o2EditorTree.GetSceneTree()->IsEditorWatching())
		{
			static bool drawing = false;
			if (drawing)
				return;
			
			drawing = true;
			EditorUIRoot.GetRootWidget()->Draw();
			drawing = false;

			mNeedRedraw = true;
		}
		else
		{
			o2Scene.BeginDrawingScene();

			for (auto layer : o2Scene.GetLayers())
			{
				if (!layer->visible)
					continue;

				for (auto drw : layer->GetEnabledDrawables())
					drw->Draw();
			}

			o2Scene.EndDrawingScene();

			o2Physics.DrawDebug();
		}
	}

	void SceneEditScreen::DrawSelection()
	{
		if (mSelectedObjects.Count() == 1)
		{
			DrawObjectSelection(mSelectedObjects[0], mSelectedObjectColor);
		}
		else
		{
			for (auto object : mSelectedObjects)
				DrawObjectSelection(object, mMultiSelectedObjectColor);
		}
	}

	void SceneEditScreen::DrawObjectSelection(SceneEditableObject* object, const Color4& color)
	{
		o2Render.DrawAABasis(object->GetTransform(), color, color, color);
		// 
		// 	auto bs = actor->transform->GetWorldNonSizedBasis();
		// 	o2Render.DrawLine(bs.offs, bs.offs + bs.xv*100.0f);
		// 	o2Render.DrawLine(bs.offs, bs.offs + bs.yv*100.0f);
	}

	void SceneEditScreen::SelectObjects(Vector<SceneEditableObject*> objects, bool additive /*= true*/)
	{
		auto prevSelectedObjects = mSelectedObjects;

		SelectObjectsWithoutAction(objects, additive);

		if (mSelectedObjects != prevSelectedObjects)
		{
			auto selectionAction = mnew SelectAction(mSelectedObjects, prevSelectedObjects);
			o2EditorApplication.DoneAction(selectionAction);
		}
	}

	void SceneEditScreen::SelectObject(SceneEditableObject* actor, bool additive /*= true*/)
	{
		auto prevSelectedObjects = mSelectedObjects;

		SelectObjectWithoutAction(actor, additive);

		if (mSelectedObjects != prevSelectedObjects)
		{
			auto selectionAction = mnew SelectAction(mSelectedObjects, prevSelectedObjects);
			o2EditorApplication.DoneAction(selectionAction);
		}
	}

	void SceneEditScreen::SelectAllObjects()
	{
		auto prevSelectedObjects = mSelectedObjects;

		mSelectedObjects.Clear();
		for (auto layer : o2Scene.GetLayers())
		{
			mSelectedObjects.Add(layer->GetEnabledActors().
								 FindAll([](auto x) { return !x->IsLockedInHierarchy(); }).
								 Convert<SceneEditableObject*>([](auto x) { return dynamic_cast<SceneEditableObject*>(x); }));
		}

		mNeedRedraw = true;
		OnObjectsSelectedFromThis();

		if (mSelectedObjects != prevSelectedObjects)
		{
			auto selectionAction = mnew SelectAction(mSelectedObjects, prevSelectedObjects);
			o2EditorApplication.DoneAction(selectionAction);
		}
	}

	void SceneEditScreen::ClearSelection()
	{
		auto prevSelectedObjects = mSelectedObjects;

		ClearSelectionWithoutAction();

		if (mSelectedObjects != prevSelectedObjects)
		{
			auto selectionAction = mnew SelectAction(mSelectedObjects, prevSelectedObjects);
			o2EditorApplication.DoneAction(selectionAction);
		}
	}

	const Vector<SceneEditableObject*>& SceneEditScreen::GetSelectedObjects() const
	{
		return mSelectedObjects;
	}

	const Vector<SceneEditableObject*>& SceneEditScreen::GetTopSelectedObjects() const
	{
		return mTopSelectedObjects;
	}

	const Color4& SceneEditScreen::GetSingleObjectSelectionColor() const
	{
		return mSelectedObjectColor;
	}

	const Color4& SceneEditScreen::GetManyObjectsSelectionColor() const
	{
		return mMultiSelectedObjectColor;
	}

	bool SceneEditScreen::IsUnderPoint(const Vec2F& point)
	{
		return Widget::IsUnderPoint(point);
	}

	void SceneEditScreen::BindSceneTree()
	{
		mSceneTree = o2EditorWindows.GetWindow<TreeWindow>()->GetSceneTree();

		mSceneTree->onObjectsSelectionChanged += THIS_FUNC(OnTreeSelectionChanged);

		o2Scene.onObjectsChanged += Function<void(Vector<SceneEditableObject*>)>(this, &SceneEditScreen::OnSceneChanged);
	}

	void SceneEditScreen::OnTreeSelectionChanged(Vector<SceneEditableObject*> selectedObjects)
	{
		if (mSelectedFromThis)
		{
			mSelectedFromThis = false;
			return;
		}

		auto prevSelectedObjects = mSelectedObjects;

		mSelectedObjects = selectedObjects;
		mNeedRedraw = true;

		UpdateTopSelectedObjects();

		if (mEnabledTool)
			mEnabledTool->OnObjectsSelectionChanged(mSelectedObjects);

		auto selectedIObjects = mSelectedObjects.Convert<IObject*>([](auto x) { return dynamic_cast<IObject*>(x); });

		if (mSelectedObjects != prevSelectedObjects || 
			selectedIObjects != o2EditorPropertiesWindow.GetTargets())
		{
			auto selectionAction = mnew SelectAction(mSelectedObjects, prevSelectedObjects);
			o2EditorApplication.DoneAction(selectionAction);

			onSelectionChanged(mSelectedObjects);
			o2EditorPropertiesWindow.SetTargets(selectedIObjects);
		}
	}

	void SceneEditScreen::UpdateTopSelectedObjects()
	{
		mTopSelectedObjects.Clear();
		for (auto object : mSelectedObjects)
		{
			bool processing = true;

			SceneEditableObject* parent = object->GetEditableParent();
			while (parent)
			{
				if (mSelectedObjects.Contains([&](auto x) { return parent == x; }))
				{
					processing = false;
					break;
				}

				parent = parent->GetEditableParent();
			}

			if (processing)
				mTopSelectedObjects.Add(object);
		}
	}

	void SceneEditScreen::OnSceneChanged(Vector<SceneEditableObject*> actors)
	{
		mNeedRedraw = true;

		if (mEnabledTool)
			mEnabledTool->OnSceneChanged(actors);
	}

	void SceneEditScreen::OnSceneChanged()
	{
		mNeedRedraw = true;
	}

	void SceneEditScreen::ClearSelectionWithoutAction(bool sendSelectedMessage /*= true*/)
	{
		mSelectedObjects.Clear();
		mTopSelectedObjects.Clear();
		mNeedRedraw = true;

		if (sendSelectedMessage)
			OnObjectsSelectedFromThis();
	}

	void SceneEditScreen::SelectObjectsWithoutAction(Vector<SceneEditableObject*> objects, bool additive /*= true*/)
	{
		if (!additive)
			mSelectedObjects.Clear();

		mSelectedObjects.Add(objects);
		mNeedRedraw = true;

		UpdateTopSelectedObjects();
		OnObjectsSelectedFromThis();
	}

	void SceneEditScreen::SelectObjectWithoutAction(SceneEditableObject* object, bool additive /*= true*/)
	{
		if (!additive)
			mSelectedObjects.Clear();

		mSelectedObjects.Add(object);
		mNeedRedraw = true;

		UpdateTopSelectedObjects();
		OnObjectsSelectedFromThis();
	}

	void SceneEditScreen::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		assetsScroll->RegObjectsCreationAction();
		
		o2UI.FocusWidget(o2EditorTree.GetSceneTree());
		o2EditorTree.GetSceneTree()->SetSelectedObjects(assetsScroll->mInstantiatedSceneDragObjects);
		
		assetsScroll->mInstantiatedSceneDragObjects.Clear();

		o2Application.SetCursor(CursorType::Arrow);
	}

	void SceneEditScreen::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		assetsScroll->InstantiateDraggingAssets();
		if (assetsScroll->mInstantiatedSceneDragObjects.Count() > 0)
			o2Application.SetCursor(CursorType::Hand);
	}

	void SceneEditScreen::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		for (auto object : assetsScroll->mInstantiatedSceneDragObjects)
		{
			object->UpdateTransform();
			Basis transform = object->GetTransform();
			transform.origin = ScreenToScenePoint(o2Input.cursorPos) - (transform.xv + transform.yv)*0.5f;
			object->SetTransform(transform);
		}
	}

	void SceneEditScreen::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		assetsScroll->ClearInstantiatedDraggingAssets();
		o2Application.SetCursor(CursorType::Arrow);

		mNeedRedraw = true;
	}

	void SceneEditScreen::OnScrolled(float scroll)
	{
		ScrollView::OnScrolled(scroll);

		if (mEnabledTool)
			mEnabledTool->OnScrolled(scroll);
	}

	void SceneEditScreen::OnKeyPressed(const Input::Key& key)
	{
		if (mEnabledTool)
			mEnabledTool->OnKeyPressed(key);
	}

	void SceneEditScreen::OnKeyReleased(const Input::Key& key)
	{
		if (mEnabledTool)
			mEnabledTool->OnKeyReleased(key);
	}

	void SceneEditScreen::OnKeyStayDown(const Input::Key& key)
	{
		if (mEnabledTool)
			mEnabledTool->OnKeyStayDown(key);
	}

	void SceneEditScreen::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(mSceneTree);

		if (mEnabledTool && !IsHandleWorking(cursor))
			mEnabledTool->OnCursorPressed(cursor);
	}

	void SceneEditScreen::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mEnabledTool && !IsHandleWorking(cursor))
			mEnabledTool->OnCursorReleased(cursor);
	}

	void SceneEditScreen::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		if (mEnabledTool && !IsHandleWorking(cursor))
			mEnabledTool->OnCursorPressBreak(cursor);
	}

	void SceneEditScreen::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (mEnabledTool && !IsHandleWorking(cursor))
			mEnabledTool->OnCursorStillDown(cursor);
	}

	void SceneEditScreen::OnCursorMoved(const Input::Cursor& cursor)
	{
		if (mEnabledTool && !IsHandleWorking(cursor))
			mEnabledTool->OnCursorMoved(cursor);
	}

	void SceneEditScreen::OnCursorEnter(const Input::Cursor& cursor)
	{
		if (mEnabledTool && !IsHandleWorking(cursor))
			mEnabledTool->OnCursorEnter(cursor);
	}

	void SceneEditScreen::OnCursorExit(const Input::Cursor& cursor)
	{
		if (mEnabledTool && !IsHandleWorking(cursor))
			mEnabledTool->OnCursorExit(cursor);
	}

	void SceneEditScreen::OnCursorRightMousePressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(mSceneTree);

		if (mEnabledTool && !IsHandleWorking(cursor))
			mEnabledTool->OnCursorRightMousePressed(cursor);

		ScrollView::OnCursorRightMousePressed(cursor);
	}

	void SceneEditScreen::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
	{
		ScrollView::OnCursorRightMouseStayDown(cursor);

		if (mEnabledTool)
			mEnabledTool->OnCursorRightMouseStayDown(cursor);
	}

	void SceneEditScreen::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		if (mEnabledTool)
			mEnabledTool->OnCursorRightMouseReleased(cursor);

		ScrollView::OnCursorRightMouseReleased(cursor);
	}

	void SceneEditScreen::OnCursorMiddleMousePressed(const Input::Cursor& cursor)
	{
		if (mEnabledTool)
			mEnabledTool->OnCursorMiddleMousePressed(cursor);
	}

	void SceneEditScreen::OnCursorMiddleMouseStayDown(const Input::Cursor& cursor)
	{
		if (mEnabledTool)
			mEnabledTool->OnCursorMiddleMouseStayDown(cursor);
	}

	void SceneEditScreen::OnCursorMiddleMouseReleased(const Input::Cursor& cursor)
	{
		if (mEnabledTool)
			mEnabledTool->OnCursorMiddleMouseReleased(cursor);
	}
}

DECLARE_CLASS(Editor::SceneEditScreen);
