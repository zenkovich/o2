#include "o2Editor/stdafx.h"
#include "MenuPanel.h"

#include "o2/Application/Application.h"
#include "o2/Assets/Assets.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/MenuPanel.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/Utils/Math/Curve.h"
#include "o2/Utils/Tasks/TaskManager.h"
#include "o2Editor/AnimationWindow/AnimationWindow.h"
#include "o2Editor/AssetsWindow/AssetsWindow.h"
#include "o2Editor/Core/Dialogs/CurveEditorDlg.h"
#include "o2Editor/Core/Dialogs/System/OpenSaveDialog.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/EditorConfig.h"
#include "o2Editor/Core/Properties/Basic/FloatProperty.h"
#include "o2Editor/Core/Properties/Basic/IntegerProperty.h"
#include "o2Editor/Core/Properties/Basic/ObjectPtrProperty.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/Core/UIStyle/EditorUIStyle.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "o2Editor/GameWindow/GameWindow.h"
#include "o2Editor/LogWindow/LogWindow.h"
#include "o2Editor/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/SceneWindow/SceneWindow.h"
#include "o2Editor/TreeWindow/SceneHierarchyTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

DECLARE_SINGLETON(Editor::MenuPanel);

namespace Editor
{
	MenuPanel::MenuPanel()
	{
		mMenuPanel = mmake<o2::MenuPanel>();
		mMenuPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		EditorUIRoot.AddWidget(mMenuPanel);

		// FILE
		mMenuPanel->AddItem("File/New scene", [&]() { OnNewScenePressed(); }, ImageAssetRef(), ShortcutKeys('N', true, true));
		mMenuPanel->AddItem("File/Open scene", [&]() { OnOpenScenePressed(); }, ImageAssetRef(), ShortcutKeys('O', true));
		mMenuPanel->AddItem("File/Save scene", [&]() { OnSaveScenePressed(); }, ImageAssetRef(), ShortcutKeys('S', true));
		mMenuPanel->AddItem("File/Save scene as ...", [&]() { OnSaveSceneAsPressed(); }, ImageAssetRef(), ShortcutKeys('S', true, false, true));

		mMenuPanel->AddItem("File/---");

		mMenuPanel->AddItem("File/Exit", [&]() { OnExitPressed(); });

		// EDIT
		mMenuPanel->AddItem("Edit/Undo", [&]() { OnUndoPressed(); }, ImageAssetRef(), ShortcutKeys('Z', true));
		mMenuPanel->AddItem("Edit/Redo", [&]() { OnRedoPressed(); }, ImageAssetRef(), ShortcutKeys('Z', true, true));

		mMenuPanel->AddItem("Edit/---");

		mMenuPanel->AddItem("Edit/Copy", [&]() {}, ImageAssetRef(), ShortcutKeys('C', true));
		mMenuPanel->AddItem("Edit/Cut", [&]() {}, ImageAssetRef(), ShortcutKeys('X', true));
		mMenuPanel->AddItem("Edit/Paste", [&]() {}, ImageAssetRef(), ShortcutKeys('V', true));
		mMenuPanel->AddItem("Edit/Delete", [&]() {}, ImageAssetRef(), ShortcutKeys(VK_DELETE));

		mMenuPanel->AddItem("Edit/---");

		mMenuPanel->AddItem("Edit/Project/Physics", [&]() {
			o2EditorPropertiesWindow.SetTarget(&o2Config.physics);
		});

		// VIEW
		mMenuPanel->AddItem("View/Show Tree", [&]() { OnShowTreePressed(); });
		mMenuPanel->AddItem("View/Show Scene", [&]() { OnShowScenePressed(); });
		mMenuPanel->AddItem("View/Show Assets", [&]() { OnShowAssetsPressed(); });
		mMenuPanel->AddItem("View/Show Properties", [&]() { OnShowPropertiesPressed(); });
		mMenuPanel->AddItem("View/Show Animation", [&]() { OnShowAnimationPressed(); });
		mMenuPanel->AddItem("View/Show Log", [&]() { OnShowLogPressed(); });
		mMenuPanel->AddItem("View/Show Game", [&]() { OnShowGamePressed(); });
		mMenuPanel->AddItem("View/---");
		mMenuPanel->AddItem("View/Reset layout", [&]() { OnResetLayoutPressed(); });

		// BUILD
		mMenuPanel->AddItem("Run/Connect scripts debugger", [&]() { o2Scripts.ConnectDebugger(); }, ImageAssetRef(), ShortcutKeys(VK_F5));
		mMenuPanel->AddItem("Run/---");
		mMenuPanel->AddItem("Run/Build & Run", [&]() { OnBuildAndRunPressed(); }, ImageAssetRef(), ShortcutKeys('R', true));
		mMenuPanel->AddItem("Run/Build", [&]() { OnBuildPressed(); }, ImageAssetRef(), ShortcutKeys('R', true, true));

		// HELP
		mMenuPanel->AddItem("Help/About", [&]() { OnAboutPressed(); });
		mMenuPanel->AddItem("Help/Documentation", [&]() { OnDocumentationPressed(); });

		// DEBUG
		mMenuPanel->AddItem("Debug/Curve editor test", [&]() { OnCurveEditorTestPressed(); });
		mMenuPanel->AddItem("Debug/Save layout as default", [&]() { OnSaveDefaultLayoutPressed(); });
		mMenuPanel->AddItem("Debug/Add property", [&]() {
			static float xx = 0, yy = 1;
			ForcePopEditorScopeOnStack scope;
			auto prop = mmake<FloatProperty>("with caption");
			prop->SetValueAndPrototypeProxy({ { mnew PointerValueProxy(&xx), mnew PointerValueProxy(&yy) } });
		});

		mMenuPanel->AddItem("Debug/Randomize IDs", [&]() {
			Function<void(Ref<Actor>)> fixActor = [&fixActor](Ref<Actor> actor) {
				actor->GenerateNewID();
				actor->GetComponents().For ( ( Ref<Component> component ){ component->GenerateNewID(); } );
				fixActor(actor->GetChild());
				fixActor(actor->GetNext());
			};
			fixActor(GetScene().GetRoot());
		});
	}
}#include <memory>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using WeakRef = std::weak_ptr<T>;

template <typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
class DynamicCast
{
public:
    template <typename U>
    static Ref<T> cast(const Ref<U>& ptr)
    {
        return std::dynamic_pointer_cast<T>(ptr);
    }
};

class MenuPanel
{
public:
    using Item = o2::MenuPanel::Item;

    MenuPanel()
    {
        mMenuPanel = mmake<o2::MenuPanel>();

        o2Scene.OnDestroyed([&](o2::Scene* scene) {
            auto fixActor = [&](o2::Actor* actor) {
                actor->ForEach([](auto comp) { comp->GenerateNewID(); });
                actor->GetChildren().ForEach([&](o2::Actor* x) { fixActor(x); });
            };

            for (const auto& actor : o2Scene.GetRootActors())
                fixActor(actor);
        });

        mMenuPanel->AddToggleItem("Debug/View editor UI tree", false, [&](bool x) { o2EditorTree.GetSceneTree()->SetEditorWatching(x); });

        mMenuPanel->AddItem("Debug/Dump memory", [&]() { o2Memory.DumpInfo(); });

        mMenuPanel->AddItem("Debug/JS collect garbage", [&]() { o2Scripts.CollectGarbage(); });

        mMenuPanel->AddItem("Debug/Rebuild assets", [&]() { o2Assets.RebuildAssets(); });
        mMenuPanel->AddItem("Debug/Rebuild assets forcible", [&]() { o2Assets.RebuildAssets(true); });
    }

    ~MenuPanel() {}

    Widget* AddItem(const Item& item)
    {
        return mMenuPanel->AddItem(item);
    }

    void AddItem(const WString& path, const Function<void()>& clickFunc = Function<void()>(),
                 const ImageAssetRef& icon = ImageAssetRef(), const ShortcutKeys& shortcut = ShortcutKeys())
    {
        mMenuPanel->AddItem(path, clickFunc, icon, shortcut);
    }

    void InsertItem(const Item& item, int position)
    {
        mMenuPanel->InsertItem(item, position);
    }

    void AddItems(Vector<Item> items)
    {
        mMenuPanel->AddItems(items);
    }

    void InsertItems(Vector<Item> items, int position)
    {
        mMenuPanel->InsertItems(items, position);
    }

    Item GetItem(int position)
    {
        return mMenuPanel->GetItem(position);
    }

    Vector<Item> GetItems() const
    {
        return mMenuPanel->GetItems();
    }

    void RemoveItem(int position)
    {
        return mMenuPanel->RemoveItem(position);
    }

    void RemoveItem(const WString& path)
    {
        return mMenuPanel->RemoveItem(path);
    }

    void CheckSceneSaving(const Function<void()>& onCompleted)
    {
        if (o2EditorApplication.IsSceneChanged())
        {
            auto wnd = EditorUIRoot.AddWidget(o2UI.CreateWindow("Save scene?"));
            *wnd->layout = WidgetLayout::Based(BaseCorner::Center, Vec2F(400, 150));

            auto verLayout = o2UI.CreateVerLayout();
            wnd->AddChild(verLayout);
            *verLayout->layout = WidgetLayout::BothStretch();
            verLayout->baseCorner = BaseCorner::Top;

            auto text = o2UI.CreateLabel("Current scene was modified but wasn't saved.\nDo you want to save it?");
            text->horOverflow = Label::HorOverflow::Wrap;
            verLayout->AddChild(text);

            auto horLayout = o2UI.CreateHorLayout();
            verLayout->AddChild(horLayout);

            *horLayout->layout = WidgetLayout::BothStretch();
            horLayout->border = BorderF(10, 10, 10, 10);
            horLayout->spacing = 10;
            horLayout->AddChild(o2UI.CreateButton("Save", [=]() {
                o2EditorApplication.SaveScene();
                onCompleted();
                wnd->Hide();
            }));

            horLayout->AddChild(o2UI.CreateButton("Don't save", [=]() {
                onCompleted();
                wnd->Hide();
            }));

            horLayout->AddChild(o2UI.CreateButton("Cancel", [=]() { wnd->Hide(); }));

            return;
        }

        onCompleted();
    }

    void OnNewScenePressed()
    {
        o2Tasks.Invoke([&] { CheckSceneSaving([]() { o2EditorApplication.MakeNewScene(); }); });
    }

    void OnOpenScenePressed()
    {
        o2Tasks.Invoke([&] {
            auto openDialog = []() {
                String fileName = GetOpenFileNameDialog("Load scene", { { "o2 Scene", "*.scn" } });

                if (fileName.IsEmpty())
                    return;

                ForcePopEditorScopeOnStack scope;
                String assetsPath = o2FileSystem.GetPathRelativeToPath(fileName, ::GetAssetsPath());
                o2EditorApplication.LoadScene(SceneAssetRef(assetsPath));
            };

            CheckSceneSaving(openDialog);
        });
    }

    void OnSaveScenePressed()
    {
        o2Tasks.Invoke([&] {
            if (o2EditorApplication.GetLoadedSceneName().IsEmpty())
                OnSaveSceneAsPressed();
            else
                o2EditorApplication.SaveScene();
        });
    }

    void OnSaveSceneAsPressed()
    {
        String fileName = GetSaveFileNameDialog("Save scene", { { "o2 Scene", "*.scn" } });

        if (fileName.IsEmpty())
            return;

        if (!fileName.EndsWith(".scn"))
            fileName += ".scn";

        o2Tasks.Invoke([=] { o2EditorApplication.SaveSceneAs(fileName); });
    }

    void OnExitPressed()
    {
        o2Application.Shutdown();
    }

    void OnUndoPressed()
    {
        o2EditorApplication.UndoAction();
    }

    void OnRedoPressed()
    {
        o2EditorApplication.RedoAction();
    }

    void OnShowTreePressed()
    {
        auto window = o2EditorWindows.GetWindow<TreeWindow>();
        if (window)
            window->Show();
    }

    void OnShowScenePressed()
    {
        auto window = o2EditorWindows.GetWindow<SceneWindow>();
        if (window)
            window->Show();
    }

    void OnShowAssetsPressed()
    {
        auto window = o2EditorWindows.GetWindow<AssetsWindow>();
        if (window)
            window->Show();
    }

    void OnShowPropertiesPressed()
    {
        auto window = o2EditorWindows.GetWindow<PropertiesWindow>();
        if (window)
            window->Show();
    }

    void OnShowAnimationPressed()
    {
        auto window = o2EditorWindows.GetWindow#include <Ref.h>

class AnimationWindow
{
public:
    void Show()
    {
        // Show animation window
    }
};

class LogWindow
{
public:
    void Show()
    {
        // Show log window
    }
};

class GameWindow
{
public:
    void Show()
    {
        // Show game window
    }
};

class MenuPanel
{
public:
    void OnShowAnimationPressed()
    {
        auto window = o2EditorWindows.GetWindow<Ref<AnimationWindow>>();
        if (window)
            window->Show();
    }

    void OnShowLogPressed()
    {
        auto window = o2EditorWindows.GetWindow<Ref<LogWindow>>();
        if (window)
            window->Show();
    }

    void OnShowGamePressed()
    {
        auto window = o2EditorWindows.GetWindow<Ref<GameWindow>>();
        if (window)
            window->Show();
    }

    void OnResetLayoutPressed()
    {
        o2EditorWindows.SetDefaultWindowsLayout();
    }

    void OnRunPressed()
    {

    }

    void OnBuildAndRunPressed()
    {

    }

    void OnBuildPressed()
    {

    }

    void OnAboutPressed()
    {

    }

    void OnDocumentationPressed()
    {

    }

    void OnSaveDefaultLayoutPressed()
    {
        o2EditorConfig.globalConfig.mDefaultLayout = o2EditorWindows.GetWindowsLayout();
        o2Debug.Log("Default windows layout saved!");
    }

    void OnCurveEditorTestPressed()
    {
        CurveEditorDlg::Show([=]() {
            // Callback function
        });

        int testCurves = 50;
        int testKeys = 50;
        for (int i = 0; i < testCurves; i++)
        {
            Ref<Curve> curve = mmake<Curve>();

            for (int j = 0; j < testKeys; j++)
            {
                curve->AppendKey(Math::Random(0.1f, 2.0f), Math::Random(-1.0f, 1.0f), 1.0f);
            }

            CurveEditorDlg::AddEditingCurve("test" + std::to_string(i), curve);
        }
    }
};