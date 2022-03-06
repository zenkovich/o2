#pragma once

#include "o2/Scene/CameraActor.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Spoiler;
}

namespace Editor
{
	class EnumProperty;
	class Vec2FProperty;
	class SceneLayersListProperty;
	class ColorProperty;

	// -------------------
	// Camera actor viewer
	// -------------------
	class CameraActorViewer : public TObjectPropertiesViewer<CameraActor>
	{
	public:
		IOBJECT(CameraActorViewer);

	protected:
		ColorProperty*           mColorProperty = nullptr;
		SceneLayersListProperty* mLayersProperty = nullptr;

		EnumProperty*   mTypeProperty = nullptr;
		VerticalLayout* mHiddenTypeProperties = nullptr;

		Spoiler*       mSizePropertySpoiler = nullptr;
		Vec2FProperty* mSizeProperty = nullptr;

		Spoiler*      mUnitsPropertySpoiler = nullptr;
		EnumProperty* mUnitsProperty = nullptr;

	protected:
		// It is called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// It is called when type enum selected, shows required property spoiler
		void OnTypeSelected();
	};
}

CLASS_BASES_META(Editor::CameraActorViewer)
{
	BASE_CLASS(Editor::TObjectPropertiesViewer<CameraActor>);
}
END_META;
CLASS_FIELDS_META(Editor::CameraActorViewer)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mColorProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLayersProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTypeProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHiddenTypeProperties);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSizePropertySpoiler);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSizeProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mUnitsPropertySpoiler);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mUnitsProperty);
}
END_META;
CLASS_METHODS_META(Editor::CameraActorViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, _tmp1);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTypeSelected);
}
END_META;
