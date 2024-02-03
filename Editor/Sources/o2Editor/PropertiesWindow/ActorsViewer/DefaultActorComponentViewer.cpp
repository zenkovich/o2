#include "o2Editor/stdafx.h"
#include "DefaultActorComponentViewer.h"

#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"

namespace Editor
{
	DefaultActorComponentViewer::DefaultActorComponentViewer()
		: mComponentType(nullptr), mViewer(nullptr)
	{}

	DefaultActorComponentViewer::~DefaultActorComponentViewer()
	{
		mViewer = nullptr;
	}

	void DefaultActorComponentViewer::SetTargetComponents(const Vector<Ref<Component>>& components)
	{
		IActorComponentViewer::SetTargetComponents(components);
		Refresh();
	}

	const Type* DefaultActorComponentViewer::GetComponentType() const
	{
		return mComponentType.Get();
	}

	void DefaultActorComponentViewer::Refresh()
	{
		const Type* objectsType = &mTargetComponents[0]->GetType();

		if (mComponentType != objectsType)
		{
			mComponentType = objectsType;
			mSpoiler->name = "component " + mComponentType->GetName();

			if (mViewer)
				o2EditorProperties.FreeObjectViewer(mViewer.Get());

			mViewer = o2EditorProperties.CreateObjectViewer(mComponentType, (String)"component:" + mComponentType->GetName() + "/",
															THIS_FUNC(OnPropertyChanged));

			mViewer->SetHeaderEnabled(false);
		}

		if (mViewer)
		{
			mViewer->Refresh(mTargetComponents.Convert<Pair<IObject*, IObject*>>([](Component* x) {
				return Pair<IObject*, IObject*>(dynamic_cast<IObject*>(x), dynamic_cast<IObject*>(x->GetPrototypeLink()));
			}));

			mSpoiler->AddChild(mViewer->GetSpoiler());
		}
	}

	void DefaultActorComponentViewer::OnPropertyChanged(const String& path, const Vector<DataDocument>& before,
														const Vector<DataDocument>& after)
	{
		for (const auto& component : mTargetComponents)
			component->GetOwnerActor()->OnChanged();

		o2EditorApplication.DoneActorPropertyChangeAction(path, before, after);
	}

	void DefaultActorComponentViewer::OnEnabled()
	{
		if (mViewer)
			mViewer->OnEnabled();
	}

	void DefaultActorComponentViewer::OnDisabled()
	{
		if (mViewer)
			mViewer->OnDisabled();
	}

}
// --- META ---

DECLARE_CLASS(Editor::DefaultActorComponentViewer, Editor__DefaultActorComponentViewer);
// --- END META ---