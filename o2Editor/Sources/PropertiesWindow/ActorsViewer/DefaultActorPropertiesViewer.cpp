#include "stdafx.h"
#include "DefaultActorPropertiesViewer.h"

#include "Core/Actions/PropertyChange.h"
#include "Core/EditorApplication.h"
#include "Core/EditorScope.h"
#include "Core/Properties/Basic/ObjectProperty.h"
#include "Core/Properties/Basic/ObjectPtrProperty.h"
#include "Core/Properties/Properties.h"
#include "Core/UI/SpoilerWithHead.h"
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
			mSpoiler->SetCaption(o2EditorProperties.MakeSmartFieldName(mActorType->GetName()));

		mTargetActors = actors;
		Refresh();
	}

	const Type* DefaultActorPropertiesViewer::GetActorType() const
	{
		return mActorType;
	}

	void DefaultActorPropertiesViewer::SpecializeActorType(const Type* type)
	{
		mSpoiler->name = "actor " + type->GetName();
		mActorType = type;

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
		PushScopeEnterOnStack scope;

		static Vector<String> hiddenFields ={
			"prototype", "parent", "name", "layer", "layerName", "enabled", "locked", "tags", "transform",
			"parentWidget", "layout", "drawDepth"
		};

		static Vector<String> additionalFields ={
			"mStates"
		};

		o2EditorProperties.FreeProperties(mFieldProperties);

		auto fields = mActorType->GetFieldsWithBaseClasses();
		if (!o2EditorProperties.IsPrivateFieldsVisible())
		{
			fields.RemoveAll([&](FieldInfo* x) { return hiddenFields.Contains(x->GetName()); });

			for (auto fieldName : additionalFields)
			{
				if (auto fieldInfo = fields.FindMatch([&](FieldInfo* x) { return x->GetName() == fieldName; }))
				{
					o2EditorProperties.BuildField(mSpoiler,
												  fieldInfo, mFieldProperties, "");
				}
			}
		}

		o2EditorProperties.BuildObjectProperties(mSpoiler,
												 fields, mFieldProperties, "");

		mBuiltWithHidden = o2EditorProperties.IsPrivateFieldsVisible();
	}

	bool DefaultActorPropertiesViewer::IsEmpty() const
	{
		return mSpoiler->GetChildren().Count() == 0;
	}

	bool DefaultActorPropertiesViewer::IsBuiltWithHiddenFields() const
	{
		return mBuiltWithHidden;
	}

}

DECLARE_CLASS(Editor::DefaultActorPropertiesViewer);
