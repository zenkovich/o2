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
#include "o2Editor/Core/Tools/FrameTool.h"
#include "o2Editor/Core/Tools/IEditorTool.h"
#include "o2Editor/Core/Tools/MoveTool.h"
#include "o2Editor/Core/Tools/RotateTool.h"
#include "o2Editor/Core/Tools/ScaleTool.h"
#include "o2Editor/Core/Tools/SelectionTool.h"
#include "o2Editor/Core/ToolsPanel.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "o2Editor/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/SceneWindow/SceneDragHandle.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"
#include "o2Editor/TreeWindow/DrawOrderTree.h"
#include "o2Editor/TreeWindow/SceneHierarchyTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

DECLARE_SINGLETON(Editor::SceneEditScreen);

namespace Editor
{
	SceneEditScreen::SceneEditScreen()
	{
		InitializeTools();
		SelectTool(mmake<MoveTool>());

		mLeftTopWidgetsContainer = InitializeWidgetsContainer(BaseCorner::LeftTop);
		mRightTopWidgetsContainer = InitializeWidgetsContainer(BaseCorner::RightTop);
		mLeftBottomWidgetsContainer = InitializeWidgetsContainer(BaseCorner::LeftBottom);
		mRightBottomWidgetsContainer = InitializeWidgetsContainer(BaseCorner::RightBottom);
	}

	SceneEditScreen::SceneEditScreen(const SceneEditScreen& other) :
		SceneEditScreen()
	{}

	SceneEditScreen::~SceneEditScreen()
	{
		for (const auto& tool : mTools)
			delete tool;
	}

	void SceneEditScreen::Draw()
	{
		if (!mReady)
			return;

		if (mNeedRedraw || (mEnabledTool && mEnabledTool->mNeedRedraw) || true)
			RedrawRenderTarget();

		mRenderTargetSprite->Draw();

		CursorAreaEventsListener::OnDrawn();

		if (mEnabledTool)
			mEnabledTool->DrawScreen();

		for (const auto& handle : mDragHandles)
			handle->Draw();

		mEditorLayers.SortBy<int>([](const Ref<SceneEditorLayer>& l) { return l->GetOrder(); });

		for (const auto& layer : mEditorLayers)
		{
			if (layer->IsEnabled() && IsLayerEnabled(layer->GetName()))
				layer->DrawOverScene();
		}

		Widget::Draw();
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

		for (const auto& layer : mEditorLayers)
		{
			if (layer->IsEnabled() && IsLayerEnabled(layer->GetName()))
				layer->Update(dt);
		}

		for (const auto& tool : mTools)
			tool->Update(dt);
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
		return point * mViewCamera.GetScale();
	}

	Vec2F SceneEditScreen::SceneToScreenVector(const Vec2F& point)
	{
		return point / mViewCamera.GetScale();
	}

	void SceneEditScreen::InitializeTools()
	{
		mTools.PushBack(mmake<SelectionTool>());
		mTools.PushBack(mmake<MoveTool>());
		mTools.PushBack(mmake<RotateTool>());
		mTools.PushBack(mmake<ScaleTool>());
		mTools.PushBack(mmake<FrameTool>());
	}

	HorizontalLayout* SceneEditScreen::InitializeWidgetsContainer(BaseCorner baseCorner)
	{
		auto controlsWidget = mmake<HorizontalLayout>();
		*controlsWidget->layout = WidgetLayout::BothStretch();
		controlsWidget->baseCorner = baseCorner;
		controlsWidget->spacing = 5;
		controlsWidget->expandHeight = false;
		controlsWidget->expandWidth = false;
		controlsWidget->border = BorderF(5, 5, 5, 5);
		controlsWidget->layout->pivot = Vec2F(1, 1);
		AddInternalWidget(controlsWidget);

		return controlsWidget;
	}

	bool SceneEditScreen::IsHandleWorking(const Input::Cursor& cursor) const
	{
		return false;
	}

	void SceneEditScreen::OnObjectsSelectedFromThis()
	{
		mSelectedFromThis = true;

		o2EditorTree.SetSelectedObjects(mSelectedObjects);

		if (mEnabledTool)
			mEnabledTool->OnObjectsSelectionChanged(mSelectedObjects);

		onSelectionChanged(mSelectedObjects);
		o2EditorPropertiesWindow.SetTargets(mSelectedObjects.Convert<IObject*>([](auto x) { return (IObject*)x; }));
	}

	void SceneEditScreen::RedrawContent()
	{
		DrawGrid();
		DrawObjects();
		o2Debug.Draw(false);
		DrawSelection();

		mEditorLayers.SortBy<int>([](const Ref<SceneEditorLayer>& l) { return l->GetOrder(); });

		for (const auto& layer : mEditorLayers)
		{
			if (layer->IsEnabled() && IsLayerEnabled(layer->GetName()))
				layer->DrawScene();
		}

		if (mEnabledTool)
		{
			mEnabledTool->DrawScene();
			mEnabledTool->mNeedRedraw = false;
		}
	}

	void SceneEditScreen::DrawObje#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
inline Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

class SceneEditableObject
{
public:
    virtual ~SceneEditableObject() {}

    // virtual void Draw() = 0;
    virtual void OnSelected() {}
    virtual void OnDeselected() {}
};

class Color4
{
public:
    // implementation of Color4 class
};

class HorizontalLayout
{
public:
    // implementation of HorizontalLayout class
};

class IEditTool
{
public:
    virtual ~IEditTool() {}

    // implementation of IEditTool interface
};

class SceneEditorLayer
{
public:
    // implementation of SceneEditorLayer class
};

class SelectAction
{
public:
    // implementation of SelectAction class
};

class SceneEditScreen
{
private:
    Ref<HorizontalLayout> mLeftTopWidgetsContainer;
    Ref<HorizontalLayout> mRightTopWidgetsContainer;
    Ref<HorizontalLayout> mLeftBottomWidgetsContainer;
    Ref<HorizontalLayout> mRightBottomWidgetsContainer;

    Vector<SceneEditorLayer*> mEditorLayers;
    std::unordered_map<String, bool> mEditorLayersEnabled;

    Ref<IEditTool> mEnabledTool;
    Vector<IEditTool*> mTools;

    Vector<Ref<SceneEditableObject>> mSelectedObjects;
    Vector<Ref<SceneEditableObject>> mTopSelectedObjects;

    Color4 mSelectedObjectColor;
    Color4 mMultiSelectedObjectColor;

public:
    void DrawSelection()
    {
        if (mSelectedObjects.size() == 1)
        {
            DrawObjectSelection(mSelectedObjects[0], mSelectedObjectColor);
        }
        else
        {
            for (const auto& object : mSelectedObjects)
                DrawObjectSelection(object, mMultiSelectedObjectColor);
        }
    }

    void DrawObjectSelection(const Ref<SceneEditableObject>& object, const Color4& color)
    {
        // implementation of DrawObjectSelection method
    }

    void SelectObjects(const Vector<Ref<SceneEditableObject>>& objects, bool additive = true)
    {
        Vector<Ref<SceneEditableObject>> prevSelectedObjects = mSelectedObjects;

        SelectObjectsWithoutAction(objects, additive);

        if (mSelectedObjects != prevSelectedObjects)
        {
            Ref<SelectAction> selectionAction = mmake<SelectAction>(mSelectedObjects, prevSelectedObjects);
            // implementation of DoneAction method
        }
    }

    void SelectObject(const Ref<SceneEditableObject>& actor, bool additive = true)
    {
        Vector<Ref<SceneEditableObject>> prevSelectedObjects = mSelectedObjects;

        SelectObjectWithoutAction(actor, additive);

        if (mSelectedObjects != prevSelectedObjects)
        {
            Ref<SelectAction> selectionAction = mmake<SelectAction>(mSelectedObjects, prevSelectedObjects);
            // implementation of DoneAction method
        }
    }

    void SelectAllObjects()
    {
        Vector<Ref<SceneEditableObject>> prevSelectedObjects = mSelectedObjects;

        mSelectedObjects = o2Scene.GetAllActors().
            FindAll([](const auto& x) { return !x->IsLockedInHierarchy(); }).
            Convert<Ref<SceneEditableObject>>([](const auto& x) { return DynamicCast<SceneEditableObject>(x); });

        // implementation of OnObjectsSelectedFromThis method

        if (mSelectedObjects != prevSelectedObjects)
        {
            Ref<SelectAction> selectionAction = mmake<SelectAction>(mSelectedObjects, prevSelectedObjects);
            // implementation of DoneAction method
        }
    }

    void ClearSelection()
    {
        Vector<Ref<SceneEditableObject>> prevSelectedObjects = mSelectedObjects;

        ClearSelectionWithoutAction();

        if (mSelectedObjects != prevSelectedObjects)
        {
            Ref<SelectAction> selectionAction = mmake<SelectAction>(mSelectedObjects, prevSelectedObjects);
            // implementation of DoneAction method
        }
    }

    Ref<HorizontalLayout> GetLeftTopWidgetsContainer()
    {
        return mLeftTopWidgetsContainer;
    }

    Ref<HorizontalLayout> GetRightTopWidgetsContainer()
    {
        return mRightTopWidgetsContainer;
    }

    Ref<HorizontalLayout> GetLeftBottomWidgetsContainer()
    {
        return mLeftBottomWidgetsContainer;
    }

    Ref<HorizontalLayout> GetRightBottomWidgetsContainer()
    {
        return mRightBottomWidgetsContainer;
    }

    void AddEditorLayer(SceneEditorLayer* layer)
    {
        mEditorLayers.push_back(layer);
    }

    void RemoveEditorLayer(SceneEditorLayer* layer)
    {
        mEditorLayers.erase(std::remove(mEditorLayers.begin(), mEditorLayers.end(), layer), mEditorLayers.end());
    }

    void SetLayerEnabled(const String& name, bool enabled)
    {
        mEditorLayersEnabled[name] = enabled;
    }

    bool IsLayerEnabled(const String& name) const
    {
        bool enabled = true;
        auto it = mEditorLayersEnabled.find(name);
        if (it != mEditorLayersEnabled.end())
            enabled = it->second;
        return enabled;
    }

    void SelectTool(const IEditTool* tool)
    {
        if (tool == mEnabledTool)
            return;

        if (mEnabledTool)
            mEnabledTool->OnDisabled();

        mEnabledTool = const_cast<IEditTool*>(tool);
        if (auto toggle = mEnabledTool->GetPanelToggle())
            toggle->SetValue(true);

        if (mEnabledTool)
            mEnabledTool->OnEnabled();
    }

    IEditTool* GetSelectedTool() const
    {
        return mEnabledTool.get();
    }

    void AddTool(IEditTool* tool)
    {
        mTools.push_back(tool);
        o2EditorTools.AddToolToggle(tool->GetPanelToggle());
    }

    void RemoveTool(IEditTool* tool)
    {
        if (tool == mEnabledTool.get())
        {
            if (!mTools.empty())
                SelectTool(mTools[0]);
            else
                SelectTool(nullptr);
        }

        mTools.erase(std::remove(mTools.begin(), mTools.end(), tool), mTools.end());
        o2EditorTools.RemoveToolToggle(tool->GetPanelToggle());
    }

    const Vector<IEditTool*>& GetTools() const
    {
        return mTools;
    }

    const Vector<Ref<SceneEditableObject>>& GetSelectedObjects() const
    {
        return mSelectedObjects;
    }

    const Vector<Ref<SceneEditableObject>>& GetTopSelectedObjects() const
    {
        return mTopSelectedObjects;
    }

    const Color4& GetSingleObjectSelectionColor() const
    {
        return mSelectedObjectColor;
    }

    const Color4& GetManyObjectsSelectionColor() const
    {
        return mMultiSelectedObjectColor;
    }
};

void SceneEditScreen::Draw()
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

        for (const auto& layer : o2Scene.GetLayers())
        {
            if (!layer->visible)
                continue;

            for (const auto& drw : layer->GetDrawables())
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
        for (const auto& object : mSelectedObjects)
            DrawObjectSelection(object, mMultiSelectedObjectColor);
    }
}

void SceneEditScreen::DrawObjectSelection(const Ref<SceneEditableObject>& object, const Color4& color)
{
    o2Render.DrawAABasis(object->GetTransform(), color, color, color);
    // 
    // 	auto bs = actor->transform->GetWorldNonSizedBasis();
    // 	o2Render.DrawLine(bs.offs, bs.offs + bs.xv*100.0f);
    // 	o2Render.DrawLine(bs.offs, bs.offs + bs.yv*100.0f);
}

void SceneEditScreen::SelectObjects(const Vector<Ref<SceneEditableObject>>& objects, bool additive /*= true*/)
{
    auto prevSelectedObjects = mSelectedObjects;

    SelectObjectsWithoutAction(objects, additive);

    if (mSelectedObjects != prevSelectedObjects)
    {
        auto selectionAction = mmake<SelectAction>(mSelectedObjects, prevSelectedObjects);
        o2EditorApplication.DoneAction(selectionAction);
    }
}

void SceneEditScreen::SelectObject(const Ref<SceneEditableObject>& actor, bool additive /*= true*/)
{
    auto prevSelectedObjects = mSelectedObjects;

    SelectObjectWithoutAction(actor, additive);

    if (mSelectedObjects != prevSelectedObjects)
    {
        auto selectionAction = mmake<SelectAction>(mSelectedObjects, prevSelectedObjects);
        o2EditorApplication.DoneAction(selectionAction);
    }
}

void SceneEditScreen::SelectAllObjects()
{
    auto prevSelectedObjects = mSelectedObjects;

    mSelectedObjects = o2Scene.GetAllActors().
        FindAll([](auto x) { return !x->IsLockedInHierarchy(); }).
        Convert<Ref<SceneEditableObject>>([](auto x) { return DynamicCast<SceneEditableObject>(x); });

    mNeedRedraw = true;
    OnObjectsSelectedFromThis();

    if (mSelectedObjects != prevSelectedObjects)
    {
        auto selectionAction = mmake<SelectAction>(mSelectedObjects, prevSelectedObjects);
        o2EditorApplication.DoneAction(selectionAction);
    }
}

void SceneEditScreen::ClearSelection()
{
    auto prevSelectedObjects = mSelectedObjects;

    ClearSelectionWithoutAction();

    if (mSelectedObjects != prevSelectedObjects)
    {
        auto selectionAction = mmake<SelectAction>(mSelectedObjects, prevSelectedObjects);
        o2EditorApplication.DoneAction(selectionAction);
    }
}

Ref<HorizontalLayout> SceneEditScreen::GetLeftTopWidgetsContainer()
{
    return mLeftTopWidgetsContainer;
}

Ref<HorizontalLayout> SceneEditScreen::GetRightTopWidgetsContainer()
{
    return mRightTopWidgetsContainer;
}

Ref<HorizontalLayout> SceneEditScreen::GetLeftBottomWidgetsContainer()
{
    return mLeftBottomWidgetsContainer;
}

Ref<HorizontalLayout> SceneEditScreen::GetRightBottomWidgetsContainer()
{
    return mRightBottomWidgetsContainer;
}

void SceneEditScreen::AddEditorLayer(SceneEditorLayer* layer)
{
    mEditorLayers.Add(layer);
}

void SceneEditScreen::RemoveEditorLayer(SceneEditorLayer* layer)
{
    mEditorLayers.Remove(layer);
}

void SceneEditScreen::SetLayerEnabled(const String& name, bool enabled)
{
    mEditorLayersEnabled[name] = enabled;
}

bool SceneEditScreen::IsLayerEnabled(const String& name) const
{
    bool enabled = true;
    mEditorLayersEnabled.TryGetValue(name, enabled);
    return enabled;
}

void SceneEditScreen::SelectTool(const IEditTool* tool)
{
    if (tool == mEnabledTool)
        return;

    if (mEnabledTool)
        mEnabledTool->OnDisabled();

    mEnabledTool = const_cast<IEditTool*>(tool);
    if (auto toggle = mEnabledTool->GetPanelToggle())
        toggle->SetValue(true);

    if (mEnabledTool)
        mEnabledTool->OnEnabled();
}

IEditTool* SceneEditScreen::GetSelectedTool() const
{
    return mEnabledTool;
}

void SceneEditScreen::AddTool(IEditTool* tool)
{
    mTools.Add(tool);
    o2EditorTools.AddToolToggle(tool->GetPanelToggle());
}

void SceneEditScreen::RemoveTool(IEditTool* tool)
{
    if (tool == mEnabledTool)
    {
        if (!mTools.IsEmpty())
            SelectTool(mTools[0]);
        else
            SelectTool(nullptr);
    }

    mTools.Remove(tool);
    o2EditorTools.RemoveToolToggle(tool->GetPanelToggle());
}

const Vector<IEditTool*>& SceneEditScreen::GetTools() const
{
    return mTools;
}

const Vector<SceneEditableObject*>& SceneEditScreen::GetSelectedObjects#include <memory>

template<class T>
using Ref = std::shared_ptr<T>;
using WeakRef = std::weak_ptr<T>;

class SceneEditableObject {};

class Vec2F {};

template<class T>
using Vector = std::vector<T>;

template<class T>
using convert_function = T (*)(const SceneEditableObject*);

// define mmake function
template<typename T, typename... Args>
static Ref<T> mmake(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
struct DynamicCast {
    template<typename U>
    operator Ref<U>() const {
        return std::dynamic_pointer_cast<U>(ptr);
    }
    
    Ref<T> ptr;
};

#define THIS_FUNC(x) &x

enum class CursorType {
    Arrow,
    Hand
};

namespace Input {
    class Key {};
}

class o2UI {
public:
    static void FocusWidget(Ref<SceneEditableObject> obj) {}
};

class o2EditorTree {
public:
    // replace raw pointers with Ref<>
    Ref<SceneEditableObject> GetSceneTree() {return mSceneTree;}
    Ref<SceneEditableObject> GetDrawOrderTree() {return mDrawOrderTree;}
private:
    // replace raw pointers with Ref<>
    Ref<SceneEditableObject> mSceneTree;
    Ref<SceneEditableObject> mDrawOrderTree;
};

class o2EditorPropertiesWindow {
public:
    void SetTargets(std::vector<Ref<SceneEditableObject>> targets) {}
    std::vector<Ref<SceneEditableObject>> GetTargets() {return mTargets;}
private:
    std::vector<Ref<SceneEditableObject>> mTargets;
};

class o2EditorApplication {
public:
    template<typename T>
    static void DoneAction(Ref<T> action) {}
};

template<typename... Args>
class Function {
public:
    Function(void *func, Args&&... args) {}
};

class SceneEditScreen {
public:
    const Ref<SceneEditableObject>& GetSelectedObject() { return mSelectedObject; }
    bool IsUnderPoint(const Vec2F& point) {
        return Widget::IsUnderPoint(point);
    }
    
    void BindSceneTree() {
        mSceneTree.GetSceneTree()->onObjectsSelectionChanged += THIS_FUNC(OnTreeSelectionChanged);
        mSceneTree.GetDrawOrderTree()->onObjectsSelectionChanged += THIS_FUNC(OnTreeSelectionChanged);

        mScene.onObjectsChanged += Function<void(Vector<Ref<SceneEditableObject>>&)>(this, &SceneEditScreen::OnSceneChanged);
    }
    
    void OnTreeSelectionChanged(Vector<Ref<SceneEditableObject>>& selectedObjects) {
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

        auto selectedIObjects = mSelectedObjects.Convert<IObject*>([](auto x) { return DynamicCast<SceneEditableObject>(x); });

        if (mSelectedObjects != prevSelectedObjects ||
            selectedIObjects != o2EditorPropertiesWindow.GetTargets())
        {
            auto selectionAction = mmake<SelectAction>(mSelectedObjects, prevSelectedObjects);
            o2EditorApplication.DoneAction(selectionAction);

            onSelectionChanged(mSelectedObjects);
            o2EditorPropertiesWindow.SetTargets(selectedIObjects);
        }
    }
    
    void UpdateTopSelectedObjects() {
        mTopSelectedObjects.Clear();
        for (auto object : mSelectedObjects)
        {
            bool processing = true;

            Ref<SceneEditableObject> parent = object->GetEditableParent();
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

    void OnSceneChanged(Vector<Ref<SceneEditableObject>>& actors) {
        mNeedRedraw = true;

        if (mEnabledTool)
            mEnabledTool->OnSceneChanged(actors);
    }

    void OnSceneChanged() {
        mNeedRedraw = true;
    }

    void ClearSelectionWithoutAction(bool sendSelectedMessage) {
        mSelectedObjects.Clear();
        mTopSelectedObjects.Clear();
        mNeedRedraw = true;

        if (sendSelectedMessage)
            OnObjectsSelectedFromThis();
    }

    void SelectObjectsWithoutAction(Vector<Ref<SceneEditableObject>>& objects, bool additive) {
        if (!additive)
            mSelectedObjects.Clear();

        mSelectedObjects.Add(objects);
        mNeedRedraw = true;

        UpdateTopSelectedObjects();
        OnObjectsSelectedFromThis();
    }

    void SelectObjectWithoutAction(Ref<SceneEditableObject> object, bool additive) {
        if (!additive)
            mSelectedObjects.Clear();

        mSelectedObjects.Add(object);
        mNeedRedraw = true;

        UpdateTopSelectedObjects();
        OnObjectsSelectedFromThis();
    }

    void OnDropped(ISelectableDragableObjectsGroup* group) {
        auto assetsScroll = dynamic_cast<RefsaRA<SlotsAW3A>AA<SceneEditableObject>AA*>(group);
        if (!assetsScroll)
            return;

        assetsScroll->RegObjectsCreationAction();

        o2UI.FocusWidget(mSceneTree.GetSceneTree());
        mSceneTree.GetSceneTree()->SetSelectedObjects(assetsScroll->mInstantiatedSceneDragObjects);

        assetsScroll->mInstantiatedSceneDragObjects.Clear();

        o2Application.SetCursor(CursorType::Arrow);
    }

    void OnDragEnter(ISelectableDragableObjectsGroup* group) {
        auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
        if (!assetsScroll)
            return;

        assetsScroll->InstantiateDraggingAssets();
        if (assetsScroll->mInstantiatedSceneDragObjects.Count() > 0)
            o2Application.SetCursor(CursorType::Hand);
    }

    void OnDraggedAbove(ISelectableDragableObjectsGroup* group) {
        auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
        if (!assetsScroll)
            return;

        for (auto object : assetsScroll->mInstantiatedSceneDragObjects)
        {
            object->UpdateTransform();
            Basis transform = object->GetTransform();
            transform.origin = ScreenToScenePoint(o2Input.cursorPos) - (transform.xv + transform.yv) * 0.5f;
            object->SetTransform(transform);
        }
    }

    void OnDragExit(ISelectableDragableObjectsGroup* group) {
        auto assetsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
        if (!assetsScroll)
            return;

        assetsScroll->ClearInstantiatedDraggingAssets();
        o2Application.SetCursor(CursorType::Arrow);

        mNeedRedraw = true;
    }

    bool IsInputTransparent() const {
        return true;
    }

    void OnScrolled(float scroll) {
        ScrollView::OnScrolled(scroll);

        if (mEnabledTool)
            mEnabledTool->OnScrolled(scroll);
    }

    void OnKeyPressed(const Input::Key& key) {
        if (mEnabledTool)
            mEnabledTool->OnKeyPressed(key);
    }

    void OnKeyReleased(const Input::Key& key) {
        if (mEnabledTool)
            mEnabledTool->OnKeyReleased(key);
    }

    void OnKeyStayDown(const Input::Key& key) {
        if (mEnabledToo#include "Ref.h"

namespace Editor {
    class SceneEditScreen {
        Ref<Tool> mEnabledTool;
        Ref<EditorTree> o2EditorTree;

        void OnKeyStayDown(int key);
        void OnCursorPressed(const Input::Cursor& cursor);
        void OnCursorReleased(const Input::Cursor& cursor);
        void OnCursorPressBreak(const Input::Cursor& cursor);
        void OnCursorStillDown(const Input::Cursor& cursor);
        void OnCursorMoved(const Input::Cursor& cursor);
        void OnCursorEnter(const Input::Cursor& cursor);
        void OnCursorExit(const Input::Cursor& cursor);
        void OnCursorRightMousePressed(const Input::Cursor& cursor);
        void OnCursorRightMouseStayDown(const Input::Cursor& cursor);
        void OnCursorRightMouseReleased(const Input::Cursor& cursor);
        void OnCursorMiddleMousePressed(const Input::Cursor& cursor);
        void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor);
        void OnCursorMiddleMouseReleased(const Input::Cursor& cursor);

        DECLARE_CLASS(Editor::SceneEditScreen, Editor__SceneEditScreen);
    };

    void SceneEditScreen::OnKeyStayDown(int key) {
        if (mEnabledTool)
            mEnabledTool->OnKeyStayDown(key);
    }

    void SceneEditScreen::OnCursorPressed(const Input::Cursor& cursor) {
        o2EditorTree->FocusTree();

        if (mEnabledTool && !IsHandleWorking(cursor))
            mEnabledTool->OnCursorPressed(cursor);
    }

    void SceneEditScreen::OnCursorReleased(const Input::Cursor& cursor) {
        if (mEnabledTool && !IsHandleWorking(cursor))
            mEnabledTool->OnCursorReleased(cursor);
    }

    void SceneEditScreen::OnCursorPressBreak(const Input::Cursor& cursor) {
        if (mEnabledTool && !IsHandleWorking(cursor))
            mEnabledTool->OnCursorPressBreak(cursor);
    }

    void SceneEditScreen::OnCursorStillDown(const Input::Cursor& cursor) {
        if (mEnabledTool && !IsHandleWorking(cursor))
            mEnabledTool->OnCursorStillDown(cursor);
    }

    void SceneEditScreen::OnCursorMoved(const Input::Cursor& cursor) {
        if (mEnabledTool && !IsHandleWorking(cursor))
            mEnabledTool->OnCursorMoved(cursor);
    }

    void SceneEditScreen::OnCursorEnter(const Input::Cursor& cursor) {
        if (mEnabledTool && !IsHandleWorking(cursor))
            mEnabledTool->OnCursorEnter(cursor);
    }

    void SceneEditScreen::OnCursorExit(const Input::Cursor& cursor) {
        if (mEnabledTool && !IsHandleWorking(cursor))
            mEnabledTool->OnCursorExit(cursor);
    }

    void SceneEditScreen::OnCursorRightMousePressed(const Input::Cursor& cursor) {
        o2EditorTree->FocusTree();

        if (mEnabledTool && !IsHandleWorking(cursor))
            mEnabledTool->OnCursorRightMousePressed(cursor);

        ScrollView::OnCursorRightMousePressed(cursor);
    }

    void SceneEditScreen::OnCursorRightMouseStayDown(const Input::Cursor& cursor) {
        ScrollView::OnCursorRightMouseStayDown(cursor);

        if (mEnabledTool)
            mEnabledTool->OnCursorRightMouseStayDown(cursor);
    }

    void SceneEditScreen::OnCursorRightMouseReleased(const Input::Cursor& cursor) {
        if (mEnabledTool)
            mEnabledTool->OnCursorRightMouseReleased(cursor);

        ScrollView::OnCursorRightMouseReleased(cursor);
    }

    void SceneEditScreen::OnCursorMiddleMousePressed(const Input::Cursor& cursor) {
        if (mEnabledTool)
            mEnabledTool->OnCursorMiddleMousePressed(cursor);
    }

    void SceneEditScreen::OnCursorMiddleMouseStayDown(const Input::Cursor& cursor) {
        if (mEnabledTool)
            mEnabledTool->OnCursorMiddleMouseStayDown(cursor);
    }

    void SceneEditScreen::OnCursorMiddleMouseReleased(const Input::Cursor& cursor) {
        if (mEnabledTool)
            mEnabledTool->OnCursorMiddleMouseReleased(cursor);
    }
}
// --- META ---

DECLARE_CLASS(Editor::SceneEditScreen, Editor__SceneEditScreen);
// --- END META ---