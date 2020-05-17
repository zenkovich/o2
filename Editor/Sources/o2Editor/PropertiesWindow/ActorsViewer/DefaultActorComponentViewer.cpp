#include "o2Editor/stdafx.h"
#include "DefaultActorComponentViewer.h"

#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"

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
		mSpoiler->name = "component " + mComponentType->GetName();
	}

	void DefaultActorComponentViewer::Refresh()
	{
		bool requiredNewViewer = mViewer ? !mComponentType->IsBasedOn(*mViewer->GetViewingObjectType()) : mComponentType != nullptr;
		if (requiredNewViewer) 
		{
			if (mViewer)
				o2EditorProperties.FreeObjectViewer(mViewer);

			mViewer = o2EditorProperties.CreateObjectViewer(mComponentType, (String)"component:" + mComponentType->GetName() + "/",
															THIS_FUNC(OnPropertyChanged));

			mViewer->SetHeaderEnabled(false);
		}

		if (mViewer)
		{
			mViewer->Refresh(mTargetComponents.Select<Pair<IObject*, IObject*>>([](Component* x) {
				return Pair<IObject*, IObject*>(dynamic_cast<IObject*>(x), dynamic_cast<IObject*>(x->GetPrototypeLink()));
			}));

			mSpoiler->AddChild(mViewer->GetSpoiler());
		}
	}

	void DefaultActorComponentViewer::OnPropertyChanged(const String& path, const Vector<DataValue>& before, const Vector<DataValue>& after)
	{
		for (auto component : mTargetComponents)
			component->GetOwnerActor()->OnChanged();

		o2EditorApplication.DoneActorPropertyChangeAction(path, before, after);
	}

}

DECLARE_CLASS(Editor::DefaultActorComponentViewer);
