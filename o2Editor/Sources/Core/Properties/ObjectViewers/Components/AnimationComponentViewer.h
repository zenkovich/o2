#pragma once

#include "Core/Properties/IObjectPropertiesViewer.h"
#include "Scene/Components/AnimationComponent.h"

using namespace o2;

namespace Editor
{
	// ------------------------------------
	// AnimationComponent properties viewer
	// ------------------------------------
	class AnimationComponentViewer : public TObjectPropertiesViewer<AnimationComponent>
	{
	public:
		// Default constructor. Initializes fields controls
		AnimationComponentViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const TargetsVec& targetObjets) override;

		IOBJECT(AnimationComponentViewer);
	};
}

CLASS_BASES_META(Editor::AnimationComponentViewer)
{
	BASE_CLASS(Editor::TObjectPropertiesViewer<AnimationComponent>);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationComponentViewer)
{
}
END_META;
CLASS_METHODS_META(Editor::AnimationComponentViewer)
{

	PUBLIC_FUNCTION(void, Refresh, const TargetsVec&);
}
END_META;
