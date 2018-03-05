#include "stdafx.h"
#include "DefaultActorComponentViewer.h"

#include "Core/Actions/ActorsPropertyChange.h"
#include "Core/EditorApplication.h"
#include "PropertiesWindow/Properties/ObjectProperty.h"
#include "PropertiesWindow/Properties/ObjectPtrProperty.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Scene/Component.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"

namespace Editor
{
	DefaultActorComponentViewer::DefaultActorComponentViewer()
	{}

	DefaultActorComponentViewer::~DefaultActorComponentViewer()
	{}

	void DefaultActorComponentViewer::SetTargetComponents(const Vector<Component*>& components)
	{
		if (!components.IsEmpty())
		{
			String caption = components[0]->GetName();
			if (caption.IsEmpty())
				caption = o2EditorProperties.MakeSmartFieldName(mComponentType->GetName());
				
			mNameCaption->text = caption;
		}

		mTargetComponents = components;
		Refresh();
	}

	const Type* DefaultActorComponentViewer::GetComponentType() const
	{
		return mComponentType;
	}

	void DefaultActorComponentViewer::SepcializeComponentType(const Type* type)
	{
		mDataView->name = "component " + type->GetName();
		mComponentType = type;
		o2EditorProperties.BuildObjectProperties((UIVerticalLayout*)mPropertiesLayout, type, mFieldProperties, "");
		
		for (auto prop : mFieldProperties.properties)
			prop.Value()->onChangeCompleted = THIS_FUNC(OnPropertyChanged);

		mSpoiler->name = "spoiler " + type->GetName();
		mPropertiesLayout->name = "properties " + type->GetName();
	}

	void DefaultActorComponentViewer::Refresh()
	{
		mFieldProperties.Set(mTargetComponents.Select<Pair<IObject*, IObject*>>([](Component* x) {
			return Pair<IObject*, IObject*>(dynamic_cast<IObject*>(x), dynamic_cast<IObject*>(x->GetPrototypeLink())); 
		}));
	}

	void DefaultActorComponentViewer::OnPropertyChanged(const String& path, 
														const Vector<DataNode>& prevValue, 
														const Vector<DataNode>& newValue)
	{
		ActorsPropertyChangeAction* action = mnew ActorsPropertyChangeAction(
			o2EditorSceneScreen.GetSelectedActors(), mComponentType, path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::DefaultActorComponentViewer);
