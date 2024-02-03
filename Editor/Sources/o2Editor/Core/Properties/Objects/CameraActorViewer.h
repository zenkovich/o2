#pragma once

#include "o2/Scene/CameraActor.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Ref.h"
#include "o2Editor/Core/WeakRef.h"

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
		Ref<ColorProperty>           mColorProperty;
		Ref<SceneLayersListProperty> mLayersProperty;

		Ref<EnumProperty>   mTypeProperty;
		Ref<VerticalLayout> mHiddenTypeProperties;

		Ref<Spoiler>       mSizePropertySpoiler;
		Ref<Vec2FProperty> mSizeProperty;

		Ref<Spoiler>      mUnitsPropertySpoiler;
		Ref<EnumProperty> mUnitsProperty;

	protected:
		// Called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Ref<const Vector<Pair<Ref<IObject>, Ref<IObject>>>>& targetObjets) override;

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
    FIELD().PROTECTED().NAME(mColorProperty);
    FIELD().PROTECTED().NAME(mLayersProperty);
    FIELD().PROTECTED().NAME(mTypeProperty);
    FIELD().PROTECTED().NAME(mHiddenTypeProperties);
    FIELD().PROTECTED().NAME(mSizePropertySpoiler);
    FIELD().PROTECTED().NAME(mSizeProperty);
    FIELD().PROTECTED().NAME(mUnitsPropertySpoiler);
    FIELD().PROTECTED().NAME(mUnitsProperty);
}
END_META;

CLASS_METHODS_META(Editor::CameraActorViewer)
{

    typedef const Ref<const Vector<Pair<Ref<IObject>, Ref<IObject>>>>& _tmp1;

    FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, _tmp1);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTypeSelected);
}
END_META;
// --- END META ---