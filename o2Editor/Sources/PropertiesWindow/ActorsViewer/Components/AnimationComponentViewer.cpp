#include "stdafx.h"
#include "AnimationComponentViewer.h"

#include "Core/EditorApplication.h"
#include "Core/EditorScope.h"
#include "Core/Properties/Basic/VectorProperty.h"
#include "Core/Properties/Properties.h"
#include "Core/UI/SpoilerWithHead.h"
#include "Scene/Components/AnimationComponent.h"


namespace Editor
{
	AnimationComponentViewer::AnimationComponentViewer()
	{

	}

	AnimationComponentViewer::~AnimationComponentViewer()
	{}

	void AnimationComponentViewer::SetTargetComponents(const Vector<Component*>& components)
	{
		TActorComponentViewer<AnimationComponent>::SetTargetComponents(components);

		Refresh();
	}

	const Type* AnimationComponentViewer::GetComponentType() const
	{
		return &TypeOf(AnimationComponent);
	}

	void AnimationComponentViewer::Rebuild()
	{
		o2EditorProperties.FreeProperties(mFieldProperties);

		const Type& type = TypeOf(AnimationComponent);

		mAnimations = dynamic_cast<VectorProperty*>(
			o2EditorProperties.BuildField(mSpoiler, type, "mStates", (String)"component:" + GetComponentType()->GetName() + "/",
										  mFieldProperties, THIS_FUNC(OnPropertyChanged)));
	}

	void AnimationComponentViewer::OnPropertyChanged(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after)
	{
		for (auto component : mTargetComponents)
			component->GetOwnerActor()->OnChanged();

		o2EditorApplication.DoneActorPropertyChangeAction(path, before, after);
	}

}

template<>
DECLARE_CLASS_MANUAL(Editor::TActorComponentViewer<o2::AnimationComponent>);

DECLARE_CLASS(Editor::AnimationComponentViewer);
