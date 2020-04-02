#pragma once

#include "o2Editor/Core/Properties/Objects/DefaultObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Button;
}

namespace Editor
{
	// -----------------------
	// Editor animation viewer
	// -----------------------
	class AnimationViewer : public DefaultObjectPropertiesViewer
	{
	public:
		// Default constructor. Initializes fields controls
		AnimationViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(AnimationViewer);

	private:
		Button*    mEditButton = nullptr;
		AnimationClip* mCurrentAnimation = nullptr;

	private:
		void OnEditPressed();
	};
}

CLASS_BASES_META(Editor::AnimationViewer)
{
	BASE_CLASS(Editor::DefaultObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationViewer)
{
	PRIVATE_FIELD(mEditButton);
	PRIVATE_FIELD(mCurrentAnimation);
}
END_META;
CLASS_METHODS_META(Editor::AnimationViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PUBLIC_FUNCTION(void, Refresh, _tmp1);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
	PRIVATE_FUNCTION(void, OnEditPressed);
}
END_META;
