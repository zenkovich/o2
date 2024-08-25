#pragma once

#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2/Scene/Components/ParticlesEmitterComponent.h"

using namespace o2;

namespace o2
{
	class Spoiler;
}

namespace Editor
{
	// ----------------------------------
	// Particles emitter component viewer
	// ----------------------------------
	class ParticlesEmitterComponentViewer: public TObjectPropertiesViewer<ParticlesEmitterComponent>
	{
	public:
		// Default constructor
		ParticlesEmitterComponentViewer();

		// Destructor
		~ParticlesEmitterComponentViewer();

		// Copy operator
		ParticlesEmitterComponentViewer& operator=(const ParticlesEmitterComponentViewer& other);

		IOBJECT(ParticlesEmitterComponentViewer);

	protected:
		Ref<VerticalLayout> mControls; // Play/Pause/Reset buttons, timeline, etc.

	protected:
		// Called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Called when viewer is refreshed
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Enable viewer event function
		void OnEnabled() override;

		// Disable viewer event function
		void OnDisabled() override;
	};
}
// --- META ---

CLASS_BASES_META(Editor::ParticlesEmitterComponentViewer)
{
    BASE_CLASS(Editor::TObjectPropertiesViewer<ParticlesEmitterComponent>);
}
END_META;
CLASS_FIELDS_META(Editor::ParticlesEmitterComponentViewer)
{
    FIELD().PROTECTED().NAME(mControls);
}
END_META;
CLASS_METHODS_META(Editor::ParticlesEmitterComponentViewer)
{

    typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;
    typedef const Vector<Pair<IObject*, IObject*>>& _tmp2;

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, _tmp1);
    FUNCTION().PROTECTED().SIGNATURE(void, OnRefreshed, _tmp2);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
}
END_META;
// --- END META ---
