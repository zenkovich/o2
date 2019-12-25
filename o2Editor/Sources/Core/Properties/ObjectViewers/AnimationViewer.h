#pragma once

#include "Core/Properties/ObjectViewers/DefaultObjectViewer.h"

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
	class AnimationViewer : public DefaultObjectViewer
	{
	public:
		// Default constructor. Initializes fields controls
		AnimationViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const TargetsVec& targetObjets) override;

		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(AnimationViewer);

	private:
		Button*    mEditButton = nullptr;
		Animation* mCurrentAnimation = nullptr;

	private:
		void OnEditPressed();
	};
}

CLASS_BASES_META(Editor::AnimationViewer)
{
	BASE_CLASS(Editor::DefaultObjectViewer);
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

	PUBLIC_FUNCTION(void, Refresh, const TargetsVec&);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
	PRIVATE_FUNCTION(void, OnEditPressed);
}
END_META;
