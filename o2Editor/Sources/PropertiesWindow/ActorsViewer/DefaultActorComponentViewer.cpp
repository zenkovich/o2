#include "stdafx.h"
#include "DefaultActorComponentViewer.h"

#include "Core/Actions/PropertyChange.h"
#include "Core/EditorApplication.h"
#include "Core/EditorScope.h"
#include "Core/Properties/Basic/ObjectProperty.h"
#include "Core/Properties/Basic/ObjectPtrProperty.h"
#include "Core/Properties/Properties.h"
#include "Core/UI/SpoilerWithHead.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Scene/Component.h"
#include "Scene/UI/Widget.h"
#include "Scene/UI/Widgets/Image.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/Spoiler.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	DefaultActorComponentViewer::DefaultActorComponentViewer()
	{}

	DefaultActorComponentViewer::~DefaultActorComponentViewer()
	{}

	void DefaultActorComponentViewer::SetTargetComponents(const Vector<Component*>& components)
	{
		IActorComponentViewer::SetTargetComponents(components);
		Refresh();
	}

	const Type* DefaultActorComponentViewer::GetComponentType() const
	{
		return mComponentType;
	}

	void DefaultActorComponentViewer::SpecializeComponentType(const Type* type)
	{
		if (mComponentType == type)
			return;

		mComponentType = type;
		Rebuild();
	}

	void DefaultActorComponentViewer::Refresh()
	{
		mFieldProperties.Set(mTargetComponents.Select<Pair<IObject*, IObject*>>([](Component* x) {
			return Pair<IObject*, IObject*>(dynamic_cast<IObject*>(x), dynamic_cast<IObject*>(x->GetPrototypeLink())); 
		}));
	}

	void DefaultActorComponentViewer::Rebuild()
	{
		mSpoiler->name = "component " + mComponentType->GetName();

		o2EditorProperties.FreeProperties(mFieldProperties);
		o2EditorProperties.BuildObjectProperties((VerticalLayout*)mSpoiler, mComponentType, mFieldProperties,
			(String)"component:" + mComponentType->GetName() + "/", THIS_FUNC(OnPropertyChanged));
	}

	void DefaultActorComponentViewer::OnPropertyChanged(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
	{
		for (auto component : mTargetComponents)
			component->GetOwnerActor()->OnChanged();

		o2EditorApplication.DoneActorPropertyChangeAction(path, before, after);
	}

}

DECLARE_CLASS(Editor::DefaultActorComponentViewer);
