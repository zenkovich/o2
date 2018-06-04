#include "stdafx.h"
#include "DefaultActorPropertiesViewer.h"

#include "Core/Actions/ActorsPropertyChange.h"
#include "Core/EditorApplication.h"
#include "Core/Properties/Properties.h"
#include "Core/Properties/Widgets/ObjectProperty.h"
#include "Core/Properties/Widgets/ObjectPtrProperty.h"
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

		mSpoiler->name = "spoiler " + type->GetName();
		mPropertiesLayout->name = "properties " + type->GetName();

		Rebuild();
	}

	void DefaultActorPropertiesViewer::Refresh()
	{
		mFieldProperties.Set(mTargetActors.Select<Pair<IObject*, IObject*>>([](Actor* x) {
			return Pair<IObject*, IObject*>(dynamic_cast<IObject*>(x), dynamic_cast<IObject*>(x->GetPrototypeLink().Get()));
		}));
	}

	void DefaultActorPropertiesViewer::Rebuild()
	{
		static Vector<String> hiddenFields ={
			"prototype", "parent", "name", "layer", "layerName", "enabled", "locked", "tags", "transform",
			"parentWidget", "layout", "drawDepth"
		};

		static Vector<String> additionalFields ={
			"mLayers", "mStates"
		};

		auto fields = mActorType->GetFieldsWithBaseClasses();
		if (!o2EditorProperties.IsPrivateFieldsVisible())
		{
			fields.RemoveAll([&](FieldInfo* x) { return hiddenFields.Contains(x->GetName()); });

			for (auto fieldName : additionalFields)
			{
				if (auto fieldInfo = fields.FindMatch([&](FieldInfo* x) { return x->GetName() == fieldName; }))
				{
					o2EditorProperties.BuildField((UIVerticalLayout*)mPropertiesLayout,
												  fieldInfo, mFieldProperties, "");
				}
			}
		}

		o2EditorProperties.BuildObjectProperties((UIVerticalLayout*)mPropertiesLayout,
												 fields, mFieldProperties, "");
	}

	bool DefaultActorPropertiesViewer::IsEmpty() const
	{
		return mPropertiesLayout->GetChildren().Count() == 0;
	}
}

DECLARE_CLASS(Editor::DefaultActorPropertiesViewer);
