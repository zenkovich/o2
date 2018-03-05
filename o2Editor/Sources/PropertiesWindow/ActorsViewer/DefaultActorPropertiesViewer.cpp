#include "stdafx.h"
#include "DefaultActorPropertiesViewer.h"

#include "Core/Actions/ActorsPropertyChange.h"
#include "Core/EditorApplication.h"
#include "PropertiesWindow/Properties/ObjectProperty.h"
#include "PropertiesWindow/Properties/ObjectPtrProperty.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"

namespace Editor
{
	DefaultActorPropertiesViewer::DefaultActorPropertiesViewer()
	{}

	DefaultActorPropertiesViewer::~DefaultActorPropertiesViewer()
	{}

	void DefaultActorPropertiesViewer::SetTargetActors(const Vector<Actor*>& actors)
	{
		if (!actors.IsEmpty())
			mNameCaption->text = o2EditorProperties.MakeSmartFieldName(mActorType->GetName());

		mTargetActors = actors;
		Refresh();
	}

	const Type* DefaultActorPropertiesViewer::GetActorType() const
	{
		return mActorType;
	}

	void DefaultActorPropertiesViewer::SpecializeActorType(const Type* type)
	{
		struct helper
		{
			static void GetFields(Vector<FieldInfo*>& fields, const Type* type)
			{
				if (!o2EditorProperties.IsPrivateFieldsVisible() &&
					(type == &TypeOf(Actor) || type == &TypeOf(UIWidget)))
				{
					return;
				}

				fields.Add(type->GetFields());

				for (auto baseType : type->GetBaseTypes())
					GetFields(fields, baseType.type);
			}
		};

		mDataView->name = "actor " + type->GetName();
		mActorType = type;

		o2EditorProperties.BuildObjectProperties((UIVerticalLayout*)mPropertiesLayout, 
												 mActorType->GetFieldsWithBaseClasses(),
												 mFieldProperties, "");

		for (auto prop : mFieldProperties.properties)
			prop.Value()->onChangeCompleted = THIS_FUNC(OnPropertyChanged);

		mSpoiler->name = "spoiler " + type->GetName();
		mPropertiesLayout->name = "properties " + type->GetName();
	}

	void DefaultActorPropertiesViewer::Refresh()
	{
		mFieldProperties.Set(mTargetActors.Select<Pair<IObject*, IObject*>>([](Actor* x) {
			return Pair<IObject*, IObject*>(dynamic_cast<IObject*>(x), dynamic_cast<IObject*>(x->GetPrototypeLink().Get()));
		}));
	}

	void DefaultActorPropertiesViewer::OnPropertyChanged(const String& path,
														const Vector<DataNode>& prevValue,
														const Vector<DataNode>& newValue)
	{
		ActorsPropertyChangeAction* action = mnew ActorsPropertyChangeAction(
			o2EditorSceneScreen.GetSelectedActors(), mActorType, path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::DefaultActorPropertiesViewer);
