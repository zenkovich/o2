#include "o2Editor/stdafx.h"
#include "ActorViewer.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"
#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/AddComponentPanel.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/DefaultActorComponentViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/DefaultActorHeaderViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/DefaultActorPropertiesViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/DefaultActorTransformViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorComponentViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorHeaderViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorPropertiesViewer.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorTransformViewer.h"

namespace Editor
{
	ActorViewer::ActorViewer()
	{
		PushEditorScopeOnStack scope;

		// Create viewers
		mHeaderViewer = mmake<DefaultActorHeaderViewer>();
		mTransformViewer = mmake<DefaultActorTransformViewer>();
		mDefaultComponentViewer = mmake<DefaultActorComponentViewer>();
		mDefaultActorPropertiesViewer = mmake<DefaultActorPropertiesViewer>();

		// Create available component and actors viewers
		auto componentsViewersTypes = TypeOf<IActorComponentViewer>::GetDerivedTypes();
		for (auto type : componentsViewersTypes)
		{
			if (type->GetName().Contains("TActorComponentViewer"))
				continue;

			mAvailableComponentsViewers.Add(DynamicCast<IActorComponentViewer>(type->CreateSample()));
		}

		auto actorPropertiesViewersTypes = TypeOf<IActorPropertiesViewer>::GetDerivedTypes();
		for (auto type : actorPropertiesViewersTypes)
			mAvailableActorPropertiesViewers.Add(DynamicCast<IActorPropertiesViewer>(type->CreateSample()));

		// Initialize content widget and viewers layout
		mContentWidget = mmake<Widget>();
		mContentWidget->name = "content";
		*mContentWidget->layout = WidgetLayout::BothStretch();

		auto scrollArea = o2UI.CreateScrollArea("backless");
		scrollArea->SetViewLayout(Layout::BothStretch(0, 0, 15, 0));
		scrollArea->SetClippingLayout(Layout::BothStretch());
		scrollArea->SetEnableScrollsHiding(false);
		scrollArea->name = "actors scroll area";
		*scrollArea->layout = WidgetLayout::BothStretch(0, 40, 0, 0);
		mContentWidget->AddChild(scrollArea);

		mViewersLayout = o2UI.CreateVerLayout();
		mViewersLayout->name = "viewers layout";
		mViewersLayout->spacing = 0.0f;
		mViewersLayout->border = BorderF();
		mViewersLayout->expandHeight = false;
		mViewersLayout->expandWidth = true;
		mViewersLayout->fitByChildren = true;
		mViewersLayout->baseCorner = BaseCorner::Top;
		*mViewersLayout->layout = WidgetLayout::BothStretch();
		scrollArea->AddChild(mViewersLayout);

		// Initialize add component panel
		mAddComponentPanel = mmake<AddComponentPanel>(this);
		mAddComponentPanel->name = "add component";
		*mAddComponentPanel->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 40, 0);
		mContentWidget->AddChild(mAddComponentPanel);

		auto addComponentAnim = AnimationClip::EaseInOut<float>("child/add component/layout/offsetTop",
																40, 400, 0.4f);

		*addComponentAnim->AddTrack<float>("child/actors scroll area/layout/offsetBottom") =
			AnimationTrack<float>::EaseInOut(40, 400, 0.4f);

		mContentWidget->AddState("add component", addComponentAnim);

		mAddComponentPanel->onCursorReleased = [&](const Ref<Cursor>& curs) { mContentWidget->SetState("add component", true); };
		mAddComponentPanel->onCursorPressedOutside = [&](const Ref<Cursor>& curs) { mContentWidget->SetState("add component", false); };

		o2Scene.onObjectsChanged += THIS_FUNC(OnSceneObjectsChanged);
	}

	ActorViewer::~ActorViewer()
	{
		o2Scene.onObjectsChanged -= THIS_FUNC(OnSceneObjectsChanged);

		for (auto& kv : mComponentViewersPool)
		{
			for (auto x : kv.second)
				delete x;
		}

		for (auto x : mAvailableComponentsViewers)
			delete x;

		if (mDefaultComponentViewer)
			delete mDefaultComponentViewer;

		for (auto& kv : mActorPropertiesViewersPool)
			delete kv.second;

		for (auto x : mAvailableActorPropertiesViewers)
			delete x;

		if (mDefaultActorPropertiesViewer)
			delete mDefaultActorPropertiesViewer;

		if (mHeaderViewer)
			delete mHeaderViewer;

		if (mTransformViewer)
			delete mTransformViewer;
	}

	const Type* ActorViewer::GetViewingObjectType() const
	{
		return &TypeOf<Actor>;
	}

	void ActorViewer::SetActorHeaderViewer(const Ref<IActorHeaderViewer>& viewer)
	{
		mHeaderViewer = viewer;
	}

	void ActorViewer::SetActorTransformViewer(const Ref<IActorTransformViewer>& viewer)
	{
		mTransformViewer = viewer;
	}

	void ActorViewer::AddComponentViewerType(const Ref<IActorComponentViewer>& type)
	{
		mAvailableComponentsViewers.Add(type);
	}

	void ActorViewer::AddActorPropertiesViewerType(const Ref<IActorPropertiesViewer>& type)
	{
		mAvailableActorPropertiesViewers.Add(type);
	}

	void ActorViewer::OnSceneObjectsChanged(const Scene::Scene::ObjectsChangedData& data)
	{
		if (data.action == Scene::ObjectsChangedAction::Add &&
			TypeOf<Actor>::IsA(data.type))
		{
			AddActor(data.actor);
		}
		else if (data.action == Scene::ObjectsChangedAction::Remove &&
				 TypeOf<Actor>::IsA(data.type))
		{
			RemoveActor(data.actor);
		}
	}

	void ActorViewer::AddActor(const Ref<Actor>& actor)
	{
		auto actorWidget = mViewersLayout->CreateWidget<ActorWidget>(actor);

		UpdateViewingWidget(actorWidget, actor);
	}

	void ActorViewer::RemoveActor(const Ref<Actor>& actor)
	{
		// Find the actor widget in the layout
		auto actorWidgets = mViewersLayout->FindWidgets<ActorWidget>(
			[actor](const Ref<ActorWidget>& actorWidget) { return actorWidget->GetActor() == actor; });

		// Remove the actor widget from the layout
		for (auto& actorWidget : actorWidgets)
		{
			RemoveViewingWidget(actorWidget);
			mViewersLayout->RemoveChild(actorWidget);
		}
	}

	void ActorViewer::UpdateViewingWidget(const Ref<ActorWidget>& actorWidget, const Ref<Actor>& actor)
	{
		if (!actor)
		{
			actorWidget->SetHeaderViewer(nullptr);
			actorWidget->SetTransformViewer(nullptr);
			actorWidget->SetPropertiesViewer(nullptr);
			actorWidget->SetComponentViewer(nullptr);
			return;
		}

		if (mHeaderViewer)
		{
			auto widgetHeaderViewer = mHeaderViewer->CreateViewerWidget(actor);
			actorWidget->SetHeaderViewer(widgetHeaderViewer);
		}

		if (mTransformViewer)
		{
			auto widgetTransformViewer = mTransformViewer->CreateViewerWidget(actor);
			actorWidget->SetTransformViewer(widgetTransformViewer);
		}

		auto componentViewer = GetComponentViewer(actor);
		actorWidget->SetComponentViewer(componentViewer);

		auto propertiesViewer = GetPropertiesViewer(actor);
		actorWidget->SetPropertiesViewer(propertiesViewer);
	}

	const Ref<IActorComponentViewer>& ActorViewer::GetComponentViewer(const Ref<Actor>& actor)
	{
		// Find the existing component viewer for the actor
		auto it = mComponentViewersPool.find(actor);
		if (it != mComponentViewersPool.end())
		{
			// Reuse the existing component viewer if it exists
			return it->second;
		}

		// Create a new component viewer for the actor
		auto component = actor->GetComponent();
		if (component)
		{
			// Find the component viewer type for the component
			auto type = TypeOf<IActorComponentViewer>::GetType(component);
			if (type)
			{
				// Create an instance of the component viewer type
				auto componentViewer = DynamicCast<IActorComponentViewer>(type->CreateSample());

				// Cache the component viewer for the actor
				mComponentViewersPool[actor] = componentViewer;

				return componentViewer;
			}
		}

		// Use the default component viewer if no other component viewer is found
		return mDefaultComponentViewer;
	}

	const Ref<IActorPropertiesViewer>& ActorViewer::GetPropertiesViewer(const Ref<Actor>& actor)
	{
		// Find the existing properties viewer for the actor
		auto it = mActorPropertiesViewersPool.find(actor);
		if (it != mActorPropertiesViewersPool.end())
		{
			// Reuse the existing properties viewer if it exists
			return it->second;
		}

		// Create a new properties viewer for the actor
		auto properties = actor->GetProperties();
		if (properties && properties->HasViewers())
		{
			// Find the properties viewer type for the properties
			auto type = TypeOf<IActorPropertiesViewer>::GetType(properties);
			if (type)
			{
				// Create an instance of the properties viewer type
				auto propertiesViewer = DynamicCast<IActorPropertiesViewer>(type->CreateSample());

				// Cache the properties viewer for the actor
				mActorPropertiesViewersPool[actor] = propertiesViewer;

				return propertiesViewer;
			}
		}

		// Use the default actor properties viewer if no other properties viewer is found
		return mDefaultActorPropertiesViewer;
	}
}#include <memory>
#include <vector>
#include <map>
#include "Widget.h"
#include "Type.h"

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename ...Args>
Ref<T> mmake(Args&& ...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using DynamicCast = std::dynamic_pointer_cast<T>;

class IObject {};

class SceneEditableObject : public IObject {};

class Component {
public:
    virtual const Type& GetType() const = 0;
};

class DefaultActorPropertiesViewer {};

class Widget {};

template<typename K, typename V>
using Map = std::map<K, V>;

template<typename T>
using Vector = std::vector<T>;

class PushEditorScopeOnStack {};

class Actor : public IObject {
public:
    Vector<Component*>& GetComponents()
    {
        return mComponents;
    }

private:
    Vector<Component*> mComponents;
};

class IActorPropertiesViewer {
public:
    virtual const Type& GetActorType() const = 0;
};

class IActorComponentViewer {
public:
    virtual const Type& GetComponentType() const = 0;
    virtual void SetEnabled(bool enabled) = 0;
    virtual Widget* GetWidget() = 0;
    virtual void SetTargetComponents(const Vector<Component*>& components) = 0;
};

class ActorViewer {
public:
    void AddComponentsViewerType(IActorComponentViewer* viewer)
    {
        mAvailableComponentsViewers.push_back(viewer);
    }

    void AddActorPropertiesViewerType(IActorPropertiesViewer* viewer)
    {
        mAvailableActorPropertiesViewers.push_back(viewer);
    }

    void Refresh()
    {
        PushEditorScopeOnStack scope;

        if (mTargetActors.empty())
            return;

        auto currentComponentGroups = GetGroupedComponents();
        if (mComponentGroupsTypes != currentComponentGroups)
        {
            SetTargets(DynamicCast<IObject>(mTargetActors));
            return;
        }

        for (const auto& viewer : mComponentsViewers)
            viewer->Refresh();

        if (mActorPropertiesViewer)
            mActorPropertiesViewer->Refresh();

        mTransformViewer->Refresh();
        mHeaderViewer->Refresh();
    }

    void OnSceneObjectsChanged(const Vector<SceneEditableObject*>& objects)
    {
        Refresh();
    }

    void SetTargets(const Vector<IObject*>& targets)
    {
        PushEditorScopeOnStack scope;

        mTargetActors.reserve(targets.size());
        for (auto target : targets)
            mTargetActors.push_back(DynamicCast<Actor>(target));

        // clear 
        mViewersLayout->RemoveAllChildren(false);

        // rebuild
        Vector<Widget*> viewersWidgets;
        viewersWidgets.push_back(mHeaderViewer->GetWidget());
        mHeaderViewer->SetTargetActors(mTargetActors);

        viewersWidgets.push_back(mTransformViewer->GetWidget());
        mTransformViewer->SetTargetActors(mTargetActors);

        SetTargetsActorProperties(targets, viewersWidgets);
        SetTargetsComponents(targets, viewersWidgets);

        for (auto viewerWidget : viewersWidgets)
            mViewersLayout->AddChild(DynamicCast<Actor>(viewerWidget));
    }

    void SetTargetsActorProperties(const Vector<IObject*>& targets, Vector<Widget*>& viewersWidgets)
    {
        PushEditorScopeOnStack scope;

        const Type* type = &mTargetActors[0]->GetType();
        bool isAllSameType = std::all_of(mTargetActors.begin(), mTargetActors.end(), [&](Actor* x) { return &x->GetType() == type; });

        if (!isAllSameType)
            return;

        bool usingDefaultViewer = false;

        auto viewerSample = findActorPropertiesViewerType(type);
        if (!viewerSample)
        {
            viewerSample = mDefaultActorPropertiesViewer;
            usingDefaultViewer = true;
        }

        if (!mActorPropertiesViewersPool.count(type))
        {
            PushEditorScopeOnStack scope;

            auto newViewer = dynamic_cast<IActorPropertiesViewer*>(viewerSample->GetType().CreateSample());
            if (usingDefaultViewer)
                dynamic_cast<DefaultActorPropertiesViewer*>(newViewer)->SpecializeActorType(type);

            mActorPropertiesViewersPool[type] = newViewer;
        }

        auto propertiesViewer = mActorPropertiesViewersPool[type];

        propertiesViewer->SetTargetActors(mTargetActors);

        if (!propertiesViewer->IsEmpty())
            viewersWidgets.push_back(propertiesViewer->GetWidget());

        mActorPropertiesViewer = propertiesViewer;
    }

    Vector<Pair<const Type*, Vector<Component*>>> GetGroupedComponents() const
    {
        Vector<Pair<const Type*, Vector<Component*>>> res;

        for (const auto& actor : mTargetActors)
        {
            Map<const Type*, int> offsets;
            for (auto component : actor->GetComponents())
            {
                auto type = &component->GetType();
                auto offset = offsets[type];
                offsets[type]++;

                Vector<Component*>* list = nullptr;
                for (int i = 0; i < res.size(); i++)
                {
                    if (res[i].first == type)
                    {
                        if (offset == 0)
                        {
                            list = &res[i].second;
                            break;
                        }
                        else
                            offset--;
                    }
                }

                if (!list)
                {
                    res.push_back({ type, {} });
                    list = &res.back().second;
                }

                list->push_back(component);
            }
        }

        return res;
    }

    void SetTargetsComponents(const Vector<IObject*>& targets, Vector<Widget*>& viewersWidgets)
    {
        PushEditorScopeOnStack scope;

        auto lastComponentViewers = mComponentsViewers;

        Vector<IActorComponentViewer*> enableComponentViewers;

        for (auto viewer : mComponentsViewers)
            mComponentViewersPool[viewer->GetComponentType()].push_back(viewer);

        mComponentsViewers.clear();

        mComponentGroupsTypes = GetGroupedComponents();
        for (const auto& pair : mComponentGroupsTypes)
        {
            auto viewerSample = findComponentsViewerType(pair.first);
            if (!viewerSample)
                viewerSample = mDefaultComponentViewer;

            if (!mComponentViewersPool.count(type) || mComponentViewersPool[type].empty())
            {
                if (!mComponentViewersPool.count(type))
                    mComponentViewersPool[type] = {};

                auto newViewer = dynamic_cast<IActorComponentViewer*>(viewerSample->GetType().CreateSample());
                mComponentViewersPool[type].push_back(newViewer);
            }

            auto componentViewer = mComponentViewersPool[type].back();
            mComponentViewersPool[type].pop_back();

            viewersWidgets.push_back(componentViewer->GetWidget());
            mComponentsViewers.push_back(componentViewer);

            componentViewer->SetTargetComponents(pair.second);

            if (std::find(lastComponentViewers.begin(), lastComponentViewers.end(), componentViewer) != lastComponentViewers.end())
                lastComponentViewers.erase(std::find(lastComponentViewers.begin(), lastComponentViewers.end(), componentViewer));
            else
                enableComponentViewers.push_back(componentViewer);
        }

        for (auto viewer : lastComponentViewers)
            viewer->SetEnabled(false);

        for (auto viewer : enableComponentViewers)
            viewer->SetEnabled(true);
    }

private:
    Vector<Ref<IObject>> mTargetActors;
    Vector<IActorComponentViewer*> mComponentsViewers;
    Vector<IActorComponentViewer*> lastComponentViewers;
    Ref<IActorPropertiesViewer> mActorPropertiesViewer;
    const Type* mAvailableActorPropertiesViewers;
    const Type* mDefaultActorPropertiesViewer;
    Ref<Widget> mViewersLayout;
    Ref<Widget> mHeaderViewer;
    Ref<Widget> mTransformViewer;
    Map<const Type*, IActorPropertiesViewer*> mActorPropertiesViewersPool;
    Map<const Type*, Vector<IActorComponentViewer*>> mComponentViewersPool;
    Vector<Pair<const Type*, Vector<Component*>>> mComponentGroupsTypes;

    IActorPropertiesViewer* findActorPropertiesViewerType(const Type* type)
    {
        for (auto x : mAvailableActorPropertiesViewers)
        {
            if (x->GetActorType() == type)
                return x;
        }

        return nullptr;
    }

    IActorComponentViewer* findComponentsViewerType(const Type* type)
    {
        for (auto x : mAvailableComponentsViewers)
        {
            if (x->GetComponentType() == type)
                return x;
        }

        return nullptr;
    }
};#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
class DynamicCast
{
public:
    template<typename U>
    static Ref<U> cast(const Ref<T>& ptr)
    {
        return std::dynamic_pointer_cast<U>(ptr);
    }
};

void ActorViewer::OnEnabled()
{
    mHeaderViewer->SetEnabled(true);
    mTransformViewer->SetEnabled(true);

    if (mActorPropertiesViewer)
        mActorPropertiesViewer->SetEnabled(true);

    mComponentsViewers.ForEach([](const Ref<Viewer>& x) { x->SetEnabled(true); });
}

void ActorViewer::OnDisabled()
{
    mHeaderViewer->SetEnabled(false);
    mTransformViewer->SetEnabled(false);

    if (mActorPropertiesViewer)
        mActorPropertiesViewer->SetEnabled(false);

    mComponentsViewers.ForEach([](const Ref<Viewer>& x) { x->SetEnabled(false); });

    mTargetActors.Clear();
}

// --- META ---

DECLARE_CLASS(Editor::ActorViewer, Editor__ActorViewer);
// --- END META ---