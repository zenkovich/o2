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
		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(AnimationViewer);

	private:
		Button*        mEditButton = nullptr;
		AnimationClip* mCurrentAnimation = nullptr;

	private:
		// It is called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// It is called when viewer is refreshed
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// It is called when edit button has pressed, opens animation editor window
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
	FIELD().DEFAULT_VALUE(nullptr).NAME(mEditButton).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mCurrentAnimation).PRIVATE();
}
END_META;
CLASS_METHODS_META(Editor::AnimationViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;
	typedef const Vector<Pair<IObject*, IObject*>>& _tmp2;

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
	PRIVATE_FUNCTION(void, RebuildProperties, _tmp1);
	PRIVATE_FUNCTION(void, OnRefreshed, _tmp2);
	PRIVATE_FUNCTION(void, OnEditPressed);
}
END_META;
