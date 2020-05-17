#pragma once

#include "o2/Render/ParticlesEmitter.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"

namespace o2
{
	class ParticlesEmitterComponent: public DrawableComponent, public ParticlesEmitter
	{
		SERIALIZABLE(ParticlesEmitterComponent);

	public:
		ParticlesEmitterComponent();
		ParticlesEmitterComponent(const ParticlesEmitterComponent& other);
		~ParticlesEmitterComponent();

		ParticlesEmitterComponent& operator=(const ParticlesEmitterComponent& other);

		// Draw particle system
		void Draw() override;

		// Updates component
		void Update(float dt) override;

		// Returns name of component
		String GetName() const override;

		// Returns category of component
		String GetCategory() const override;

		// Returns name of component icon
		String GetIcon() const override;

	protected:
		// It is called when actor's transform was changed
		void OnTransformUpdated();

		// It is called when object was deserialized
		void OnDeserialized(const DataValue& node) override;
	};
}

CLASS_BASES_META(o2::ParticlesEmitterComponent)
{
	BASE_CLASS(o2::DrawableComponent);
	BASE_CLASS(o2::ParticlesEmitter);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesEmitterComponent)
{
}
END_META;
CLASS_METHODS_META(o2::ParticlesEmitterComponent)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(String, GetCategory);
	PUBLIC_FUNCTION(String, GetIcon);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
}
END_META;
