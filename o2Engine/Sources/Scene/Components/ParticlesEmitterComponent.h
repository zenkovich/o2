#pragma once

#include "Scene/DrawableComponent.h"
#include "Render/ParticlesEmitter.h"

namespace o2
{
	class ParticlesEmitterComponent: public DrawableComponent
	{
		SERIALIZABLE(ParticlesEmitterComponent);

	public:
		ParticlesEmitterComponent();
		ParticlesEmitterComponent(const ParticlesEmitterComponent& other);
		~ParticlesEmitterComponent();

		ParticlesEmitterComponent& operator=(const ParticlesEmitterComponent& other);

		// Draw particle system
		void Draw();

		// Updates component
		void Update(float dt);

		// Returns name of component
		String GetName() const;

	protected:
		ParticlesEmitter mEmitter; // @SERIALIZABLE @EDITOR_PROPERTY

	protected:
		// It is called when actor's transform was changed
		void OnTransformUpdated();
	};
}
