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
        Ref<ColorProperty>           mColorProperty;  // Background fill color property
        Ref<SceneLayersListProperty> mLayersProperty; // Layers property

        Ref<EnumProperty>   mTypeProperty;         // Camera type property
        Ref<VerticalLayout> mHiddenTypeProperties; // Hidden type properties

        Ref<Spoiler>       mSizePropertySpoiler; // Size property spoiler
        Ref<Vec2FProperty> mSizeProperty;        // Size property

        Ref<Spoiler>      mUnitsPropertySpoiler; // Units property spoiler
        Ref<EnumProperty> mUnitsProperty;        // Units property

	protected:
		// Called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Called when type enum selected, shows required property spoiler
		void OnTypeSelected();
	};
}
// --- META ---

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
// --- END META ---
