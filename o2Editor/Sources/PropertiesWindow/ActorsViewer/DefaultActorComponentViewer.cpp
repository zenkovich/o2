#include "stdafx.h"
#include "DefaultActorComponentViewer.h"

#include "Core/Actions/ActorsPropertyChange.h"
#include "Core/EditorApplication.h"
#include "Core/Properties/Properties.h"
#include "Core/Properties/Widgets/ObjectProperty.h"
#include "Core/Properties/Widgets/ObjectPtrProperty.h"
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
		mDataView->name = "component " + mComponentType->GetName();

		o2EditorProperties.FreeProperties(mFieldProperties);
		o2EditorProperties.BuildObjectProperties((UIVerticalLayout*)mPropertiesLayout, mComponentType, mFieldProperties,
			(String)"component:" + mComponentType->GetName() + "/");

		mSpoiler->name = "spoiler " + mComponentType->GetName();
		mPropertiesLayout->name = "properties " + mComponentType->GetName();
	}

}

DECLARE_CLASS(Editor::DefaultActorComponentViewer);
