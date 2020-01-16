#pragma once

#include "o2/Scene/Components/AnimationComponent.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"

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
		void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

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

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PUBLIC_FUNCTION(void, Refresh, _tmp1);
}
END_META;
