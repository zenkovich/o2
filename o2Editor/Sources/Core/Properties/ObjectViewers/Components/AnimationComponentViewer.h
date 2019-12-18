#pragma once

#include "Core/Properties/IObjectPropertiesViewer.h"

using namespace o2;

namespace Editor
{
	// ------------------------------------
	// AnimationComponent properties viewer
	// ------------------------------------
	class AnimationComponentViewer : public IObjectPropertiesViewer
	{
	public:
		// Default constructor. Initializes fields controls
		AnimationComponentViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const TargetsVec& targetObjets) override;

		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		IOBJECT(AnimationComponentViewer);
	};
}

CLASS_BASES_META(Editor::AnimationComponentViewer)
{
	BASE_CLASS(Editor::IObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationComponentViewer)
{
}
END_META;
CLASS_METHODS_META(Editor::AnimationComponentViewer)
{

	PUBLIC_FUNCTION(void, Refresh, const TargetsVec&);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
}
END_META;
