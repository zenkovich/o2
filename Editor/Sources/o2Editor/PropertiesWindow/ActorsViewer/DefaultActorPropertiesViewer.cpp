#include "o2Editor/stdafx.h"
#include "DefaultActorPropertiesViewer.h"

#include "o2/Utils/StringUtils.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"

namespace Editor
{
	DefaultActorPropertiesViewer::DefaultActorPropertiesViewer()
	{}

	DefaultActorPropertiesViewer::~DefaultActorPropertiesViewer()
	{}

	void DefaultActorPropertiesViewer::SetTargetActors(const Vector<Actor*>& actors)
	{
		if (!actors.IsEmpty())
			mSpoiler->SetCaption(GetSmartName(mActorType->GetName()));

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
	}

	void DefaultActorPropertiesViewer::Refresh()
	{
		bool requiredNewViewer = mViewer ? mViewer->GetViewingObjectType() != mActorType : mActorType != nullptr;
		if (requiredNewViewer) 
		{
			if (mViewer)
				o2EditorProperties.FreeObjectViewer(mViewer);

			mViewer = o2EditorProperties.CreateObjectViewer(mActorType, "", THIS_FUNC(OnPropertyChanged));
			mViewer->SetHeaderEnabled(false);
			mSpoiler->AddChild(mViewer->GetSpoiler());
		}

		if (mViewer)
		{
			mViewer->Refresh(mTargetActors.Convert<Pair<IObject*, IObject*>>([](Actor* x) {
				return Pair<IObject*, IObject*>(dynamic_cast<IObject*>(x), dynamic_cast<IObject*>(x->GetPrototypeLink().Get()));
			}));
		}
	}

// 	void DefaultActorPropertiesViewer::Rebuild()
// 	{
// 		PushEditorScopeOnStack scope;
// 
// 		static Vector<String> hiddenFields ={
// 			"prototype", "parent", "name", "layer", "layerName", "enabled", "locked", "tags", "transform",
// 			"parentWidget", "layout", "drawDepth"
// 		};
// 
// 		static Vector<String> additionalFields ={
// 			"mStates"
// 		};
// 
// 		o2EditorProperties.FreeProperties(mPropertiesContext);
// 
// 		auto fields = mActorType->GetFieldsWithBaseClasses();
// 		if (!o2EditorProperties.IsPrivateFieldsVisible())
// 		{
// 			fields.RemoveAll([&](FieldInfo* x) { return hiddenFields.Contains(x->GetName()); });
// 
// 			for (auto fieldName : additionalFields)
// 			{
// 				if (auto fieldInfo = fields.FindMatch([&](FieldInfo* x) { return x->GetName() == fieldName; }))
// 					o2EditorProperties.BuildField(mSpoiler, fieldInfo, mPropertiesContext, "");
// 			}
// 		}
// 
// 		o2EditorProperties.BuildObjectProperties(mSpoiler, fields, mPropertiesContext, "");
// 	}

	bool DefaultActorPropertiesViewer::IsEmpty() const
	{
		return mViewer && mViewer->IsEmpty();
	}

	void DefaultActorPropertiesViewer::OnPropertyChanged(const String& path, const Vector<DataDocument>& before, 
														 const Vector<DataDocument>& after)
	{
		for (auto actors : mTargetActors)
			actors->OnChanged();

		o2EditorApplication.DoneActorPropertyChangeAction(path, before, after);
	}

	void DefaultActorPropertiesViewer::OnEnabled()
	{
		if (mViewer)
			mViewer->OnEnabled();
	}

	void DefaultActorPropertiesViewer::OnDisabled()
	{
		if (mViewer)
			mViewer->OnDisabled();
	}

}
// --- META ---

DECLARE_CLASS(Editor::DefaultActorPropertiesViewer, Editor__DefaultActorPropertiesViewer);
// --- END META ---
