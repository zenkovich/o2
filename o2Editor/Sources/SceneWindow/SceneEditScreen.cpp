#include "stdafx.h"
#include "SceneEditScreen.h"

#include "AssetsWindow/AssetsIconsScroll.h"
#include "Core/Actions/Select.h"
#include "Core/EditorApplication.h"
#include "Core/Tools/IEditorTool.h"
#include "Core/Tools/MoveTool.h"
#include "Core/Tools/SelectionTool.h"
#include "Core/UIRoot.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "Scene/DrawableComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneLayer.h"
#include "SceneWindow/SceneDragHandle.h"
#include "TreeWindow/SceneTree.h"
#include "TreeWindow/TreeWindow.h"
#include "Scene/UI/Tree.h"
#include "Scene/UI/UIManager.h"
#include "Utils/Math/Math.h"

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
		UIWidget::Draw();

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
		UIWidget::Update(dt);

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
		o2EditorPropertiesWindow.SetTargets(mSelectedObjects.Select<IObject*>([](auto x) { return (IObject*)x; }));
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
		o2Scene.BeginDrawingScene();

		for (auto layer : o2Scene.GetLayers())
		{
			for (auto drw : layer->GetEnabledDrawables())
				drw->Draw();
		}

		o2Scene.EndDrawingScene();

// 		static bool drawing = false;
// 		if (drawing)
// 			return;
// 
// 		drawing = true;
// 		EditorUIRoot.GetRootWidget()->Draw();
// 		drawing = false;
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

	void SceneEditScreen::SelectObjects(SceneEditableObjectsVec objects, bool additive /*= true*/)
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
								 Select<SceneEditableObject*>([](auto x) { return dynamic_cast<SceneEditableObject*>(x); }));
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

	const SceneEditScreen::SceneEditableObjectsVec& SceneEditScreen::GetSelectedObjects() const
	{
		return mSelectedObjects;
	}

	const SceneEditScreen::SceneEditableObjectsVec& SceneEditScreen::GetTopSelectedObjects() const
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
		return UIWidget::IsUnderPoint(point);
	}

	void SceneEditScreen::BindSceneTree()
	{
		mSceneTree = o2EditorWindows.GetWindow<TreeWindow>()->GetSceneTree();

		mSceneTree->onObjectsSelectionChanged += THIS_FUNC(OnTreeSelectionChanged);

		o2Scene.onObjectsChanged += Function<void(SceneEditableObjectsVec)>(this, &SceneEditScreen::OnSceneChanged);
	}

	void SceneEditScreen::OnTreeSelectionChanged(SceneEditableObjectsVec selectedObjects)
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

		auto selectedIObjects = mSelectedObjects.Select<IObject*>([](auto x) { return dynamic_cast<IObject*>(x); });

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
				if (mSelectedObjects.ContainsPred([&](auto x) { return parent == x; }))
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

	void SceneEditScreen::OnSceneChanged(SceneEditableObjectsVec actors)
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

	void SceneEditScreen::SelectObjectsWithoutAction(SceneEditableObjectsVec objects, bool additive /*= true*/)
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
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		assetsScroll->RegObjectsCreationAction();
		
		o2UI.FocusWidget(o2EditorTree.GetSceneTree());
		o2EditorTree.GetSceneTree()->SetSelectedObjects(assetsScroll->mInstSceneDragObjects);
		
		assetsScroll->mInstSceneDragObjects.Clear();

		o2Application.SetCursor(CursorType::Arrow);
	}

	void SceneEditScreen::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		assetsScroll->InstantiateDraggingAssets();
		if (assetsScroll->mInstSceneDragObjects.Count() > 0)
			o2Application.SetCursor(CursorType::Hand);
	}

	void SceneEditScreen::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		for (auto object : assetsScroll->mInstSceneDragObjects)
		{
			Basis transform = object->GetTransform();
			transform.origin += ScreenToScenePoint(o2Input.cursorPos) - (transform.xv + transform.yv)*0.5f;
			object->SetTransform(transform);
		}
	}

	void SceneEditScreen::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		assetsScroll->ClearInstantiatedDraggingAssets();
		o2Application.SetCursor(CursorType::Arrow);

		mNeedRedraw = true;
	}

	void SceneEditScreen::OnScrolled(float scroll)
	{
		UIScrollView::OnScrolled(scroll);

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

		UIScrollView::OnCursorRightMousePressed(cursor);
	}

	void SceneEditScreen::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
	{
		UIScrollView::OnCursorRightMouseStayDown(cursor);

		if (mEnabledTool)
			mEnabledTool->OnCursorRightMouseStayDown(cursor);
	}

	void SceneEditScreen::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		if (mEnabledTool)
			mEnabledTool->OnCursorRightMouseReleased(cursor);

		UIScrollView::OnCursorRightMouseReleased(cursor);
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
