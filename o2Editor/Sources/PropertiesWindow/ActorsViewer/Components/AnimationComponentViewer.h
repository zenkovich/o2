#pragma once

#include "PropertiesWindow/ActorsViewer/IActorComponentViewer.h"
#include "Scene/Components/AnimationComponent.h"
#include "Core/Properties/FieldPropertiesInfo.h"

namespace Editor
{
	class VectorProperty;

	// --------------------------
	// Animation component viewer
	// --------------------------
	class AnimationComponentViewer : public TActorComponentViewer<AnimationComponent>
	{
	public:
		// Default constructor. Initializes data widget
		AnimationComponentViewer();

		// Virtual destructor
		~AnimationComponentViewer();

		// Sets target actors
		void SetTargetComponents(const Vector<Component*>& components) override;

		// Returns viewing component type 
		const Type* GetComponentType() const override;

		// Rebuilds properties layout
		void Rebuild() override;

		IOBJECT(AnimationComponentViewer);

	protected:
		VectorProperty* mAnimations = nullptr;

	protected:
		// It is called when some property changed, marks Actor as changed and calls default Undo create callback
		void OnPropertyChanged(const String& path, const Vector<DataNode>& before, const Vector<DataNode>& after);
	};
}

CLASS_BASES_META(Editor::AnimationComponentViewer)
{
	BASE_CLASS(Editor::TActorComponentViewer<AnimationComponent>);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationComponentViewer)
{
	PROTECTED_FIELD(mAnimations);
}
END_META;
CLASS_METHODS_META(Editor::AnimationComponentViewer)
{

	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
	PUBLIC_FUNCTION(const Type*, GetComponentType);
	PUBLIC_FUNCTION(void, Rebuild);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataNode>&, const Vector<DataNode>&);
}
END_META;
