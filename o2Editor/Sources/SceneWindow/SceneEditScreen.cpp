#include "SceneEditScreen.h"

#include "AssetsWindow/AssetsIconsScroll.h"
#include "Core/Actions/SelectActors.h"
#include "Core/EditorApplication.h"
#include "Core/Tools/IEditorTool.h"
#include "Core/Tools/MoveTool.h"
#include "Core/Tools/SelectionTool.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "Scene/DrawableComponent.h"
#include "Scene/Scene.h"
#include "SceneWindow/SceneDragHandle.h"
#include "TreeWindow/ActorsTree.h"
#include "TreeWindow/TreeWindow.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
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

		if (mNeedRedraw || (mEnabledTool && mEnabledTool->mNeedRedraw) || true)
			RedrawRenderTarget();

		mRenderTargetSprite->Draw();

		if (mEnabledTool)
			mEnabledTool->DrawScreen();

		CursorAreaEventsListener::OnDrawn();

		for (auto handle : mDragHandles)
			handle->Draw();
	}

#undef DrawText

	void SceneEditScreen::Update(float dt)
	{
		UIWidget::Update(dt);

		UpdateCamera(dt);
		o2Scene.CheckChangedActors();

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

	void SceneEditScreen::OnActorsSelectedFromThis()
	{
		mSelectedFromThis = true;
		mActorsTree->SetSelectedActors(mSelectedActors);

		if (mEnabledTool)
			mEnabledTool->OnActorsSelectionChanged(mSelectedActors);

		onSelectionChanged(mSelectedActors);
		o2EditorProperties.SetTargets(mSelectedActors.Select<IObject*>([](auto x) { return (IObject*)x; }));
	}

	void SceneEditScreen::RedrawContent()
	{
		DrawGrid();
		DrawActors();
		DrawSelection();

		if (mEnabledTool)
		{
			mEnabledTool->DrawScene();
			mEnabledTool->mNeedRedraw = false;
		}
	}

	void SceneEditScreen::DrawActors()
	{
		for (auto layer : o2Scene.GetLayers())
			for (auto drw : layer->GetEnabledDrawableComponents())
				drw->Draw();
	}

	void SceneEditScreen::DrawSelection()
	{
		if (mSelectedActors.Count() == 1)
		{
			DrawActorSelection(mSelectedActors[0], mSelectedActorColor);
		}
		else
		{
			for (auto actor : mSelectedActors)
				DrawActorSelection(actor, mMultiSelectedActorColor);
		}
	}

	void SceneEditScreen::DrawActorSelection(Actor* actor, const Color4& color)
	{
		float camScale = mViewCamera.GetScale().x;
		Vec2F screenSize = actor->transform->GetWorldAxisAlignedRect().Size() / camScale;
		if (screenSize.SqrLength() < mActorMinimalSelectionSize*mActorMinimalSelectionSize)
		{
			Vec2F wpos = actor->transform->GetWorldPosition();
			float sz = mActorMinimalSelectionSize*0.5f*camScale;
			o2Render.DrawCircle(wpos, sz, color);
			o2Render.DrawLine(wpos, wpos + actor->transform->GetUp()*sz, color);
		}
		else o2Render.DrawBasis(actor->transform->GetWorldBasis(), color, color, color);
		// 
		// 	auto bs = actor->transform->GetWorldNonSizedBasis();
		// 	o2Render.DrawLine(bs.offs, bs.offs + bs.xv*100.0f);
		// 	o2Render.DrawLine(bs.offs, bs.offs + bs.yv*100.0f);
	}

	void SceneEditScreen::SelectActors(ActorsVec actors, bool additive /*= true*/)
	{
		auto prevSelectedActors = mSelectedActors;

		SelectActorsWithoutAction(actors, additive);

		if (mSelectedActors != prevSelectedActors)
		{
			auto selectionAction = mnew SelectActorsAction(mSelectedActors, prevSelectedActors);
			o2EditorApplication.DoneAction(selectionAction);
		}
	}

	void SceneEditScreen::SelectActor(Actor* actor, bool additive /*= true*/)
	{
		auto prevSelectedActors = mSelectedActors;

		SelectActorWithoutAction(actor, additive);

		if (mSelectedActors != prevSelectedActors)
		{
			auto selectionAction = mnew SelectActorsAction(mSelectedActors, prevSelectedActors);
			o2EditorApplication.DoneAction(selectionAction);
		}
	}

	void SceneEditScreen::SelectAllActors()
	{
		auto prevSelectedActors = mSelectedActors;

		mSelectedActors.Clear();
		for (auto layer : o2Scene.GetLayers())
			mSelectedActors.Add(layer->GetEnabledActors().FindAll([](auto x) { return !x->IsLockedInHierarchy(); }));

		mNeedRedraw = true;
		OnActorsSelectedFromThis();

		if (mSelectedActors != prevSelectedActors)
		{
			auto selectionAction = mnew SelectActorsAction(mSelectedActors, prevSelectedActors);
			o2EditorApplication.DoneAction(selectionAction);
		}
	}

	void SceneEditScreen::ClearSelection()
	{
		auto prevSelectedActors = mSelectedActors;

		ClearSelectionWithoutAction();

		if (mSelectedActors != prevSelectedActors)
		{
			auto selectionAction = mnew SelectActorsAction(mSelectedActors, prevSelectedActors);
			o2EditorApplication.DoneAction(selectionAction);
		}
	}

	const SceneEditScreen::ActorsVec& SceneEditScreen::GetSelectedActors() const
	{
		return mSelectedActors;
	}

	const SceneEditScreen::ActorsVec& SceneEditScreen::GetTopSelectedActors() const
	{
		return mTopSelectedActors;
	}

	const Color4& SceneEditScreen::GetSingleActorSelectionColor() const
	{
		return mSelectedActorColor;
	}

	const Color4& SceneEditScreen::GetManyActorsSelectionColor() const
	{
		return mMultiSelectedActorColor;
	}

	bool SceneEditScreen::IsUnderPoint(const Vec2F& point)
	{
		return UIWidget::IsUnderPoint(point);
	}

	void SceneEditScreen::BindActorsTree()
	{
		mActorsTree = o2EditorWindows.GetWindow<TreeWindow>()->GetActorsTree();

		mActorsTree->onObjectsSelectionChanged += THIS_FUNC(OnTreeSelectionChanged);

		o2Scene.onChanged += Function<void(ActorsVec)>(this, &SceneEditScreen::OnSceneChanged);
	}

	void SceneEditScreen::OnTreeSelectionChanged(Vector<Actor*> selectedActors)
	{
		if (mSelectedFromThis)
		{
			mSelectedFromThis = false;
			return;
		}

		auto prevSelectedActors = mSelectedActors;

		mSelectedActors = selectedActors;
		mNeedRedraw = true;

		UpdateTopSelectedActors();

		if (mEnabledTool)
			mEnabledTool->OnActorsSelectionChanged(mSelectedActors);

		if (mSelectedActors != prevSelectedActors || 
			mSelectedActors.Cast<IObject*>() != o2EditorProperties.GetTargets())
		{
			auto selectionAction = mnew SelectActorsAction(mSelectedActors, prevSelectedActors);
			o2EditorApplication.DoneAction(selectionAction);

			onSelectionChanged(mSelectedActors);
			o2EditorProperties.SetTargets(mSelectedActors.Select<IObject*>([](auto x) { return (IObject*)x; }));
		}
	}

	int SceneEditScreen::GetActorIdx(Actor* actor)
	{
		if (actor->GetParent())
		{
			return actor->GetParent()->GetChilds().Find(actor) + GetActorIdx(actor->GetParent());
		}

		return o2Scene.GetRootActors().Find(actor);
	}

	void SceneEditScreen::UpdateTopSelectedActors()
	{
		mTopSelectedActors.Clear();
		for (auto actor : mSelectedActors)
		{
			bool processing = true;

			Actor* parent = actor->GetParent();
			while (parent)
			{
				if (mSelectedActors.ContainsPred([&](auto x) { return parent == x; }))
				{
					processing = false;
					break;
				}

				parent = parent->GetParent();
			}

			if (processing)
				mTopSelectedActors.Add(actor);
		}
	}

	void SceneEditScreen::OnSceneChanged(ActorsVec actors)
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
		mSelectedActors.Clear();
		mTopSelectedActors.Clear();
		mNeedRedraw = true;

		if (sendSelectedMessage)
			OnActorsSelectedFromThis();
	}

	void SceneEditScreen::SelectActorsWithoutAction(ActorsVec actors, bool additive /*= true*/)
	{
		if (!additive)
			mSelectedActors.Clear();

		mSelectedActors.Add(actors);
		mNeedRedraw = true;

		UpdateTopSelectedActors();
		OnActorsSelectedFromThis();
	}

	void SceneEditScreen::SelectActorWithoutAction(Actor* actor, bool additive /*= true*/)
	{
		if (!additive)
			mSelectedActors.Clear();

		mSelectedActors.Add(actor);
		mNeedRedraw = true;

		UpdateTopSelectedActors();
		OnActorsSelectedFromThis();
	}

	void SceneEditScreen::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		assetsScroll->RegActorsCreationAction();
		
		o2UI.FocusWidget(o2EditorTree.GetActorsTree());
		o2EditorTree.GetActorsTree()->SetSelectedActors(assetsScroll->mInstSceneDragActors);
		
		assetsScroll->mInstSceneDragActors.Clear();

		o2Application.SetCursor(CursorType::Arrow);
	}

	void SceneEditScreen::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		assetsScroll->InstantiateDraggingAssets();
		if (assetsScroll->mInstSceneDragActors.Count() > 0)
			o2Application.SetCursor(CursorType::Hand);
	}

	void SceneEditScreen::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{
		auto assetsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (!assetsScroll)
			return;

		for (auto actor : assetsScroll->mInstSceneDragActors)
			actor->transform->position = ScreenToScenePoint(o2Input.cursorPos);
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
		o2UI.FocusWidget(mActorsTree);

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
		o2UI.FocusWidget(mActorsTree);

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

CLASS_META(Editor::SceneEditScreen)
{
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::Singleton<SceneEditScreen>);
	BASE_CLASS(Editor::UIScrollView);

	PUBLIC_FIELD(onSelectionChanged);
	PROTECTED_FIELD(mSelectedActorColor);
	PROTECTED_FIELD(mMultiSelectedActorColor);
	PROTECTED_FIELD(mActorMinimalSelectionSize);
	PROTECTED_FIELD(mActorsTree);
	PROTECTED_FIELD(mSelectedActors);
	PROTECTED_FIELD(mTopSelectedActors);
	PROTECTED_FIELD(mSelectedFromThis);
	PROTECTED_FIELD(mTools);
	PROTECTED_FIELD(mEnabledTool);
	PROTECTED_FIELD(mDragHandles);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(Vec2F, ScreenToScenePoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, SceneToScreenPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, ScreenToSceneVector, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, SceneToScreenVector, const Vec2F&);
	PUBLIC_FUNCTION(void, DrawActorSelection, Actor*, const Color4&);
	PUBLIC_FUNCTION(void, SelectActors, ActorsVec, bool);
	PUBLIC_FUNCTION(void, SelectActor, Actor*, bool);
	PUBLIC_FUNCTION(void, SelectAllActors);
	PUBLIC_FUNCTION(void, ClearSelection);
	PUBLIC_FUNCTION(const ActorsVec&, GetSelectedActors);
	PUBLIC_FUNCTION(const ActorsVec&, GetTopSelectedActors);
	PUBLIC_FUNCTION(const Color4&, GetSingleActorSelectionColor);
	PUBLIC_FUNCTION(const Color4&, GetManyActorsSelectionColor);
	PUBLIC_FUNCTION(void, OnSceneChanged);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, InitializeTools, const Type*);
	PROTECTED_FUNCTION(bool, IsHandleWorking, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMiddleMousePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMiddleMouseStayDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMiddleMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyStayDown, const Input::Key&);
	PROTECTED_FUNCTION(void, OnActorsSelectedFromThis);
	PROTECTED_FUNCTION(void, RedrawContent);
	PROTECTED_FUNCTION(void, DrawActors);
	PROTECTED_FUNCTION(void, DrawSelection);
	PROTECTED_FUNCTION(void, BindActorsTree);
	PROTECTED_FUNCTION(void, OnTreeSelectionChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, UpdateTopSelectedActors);
	PROTECTED_FUNCTION(int, GetActorIdx, Actor*);
	PROTECTED_FUNCTION(void, OnSceneChanged, ActorsVec);
	PROTECTED_FUNCTION(void, ClearSelectionWithoutAction, bool);
	PROTECTED_FUNCTION(void, SelectActorsWithoutAction, ActorsVec, bool);
	PROTECTED_FUNCTION(void, SelectActorWithoutAction, Actor*, bool);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
}
END_META;
