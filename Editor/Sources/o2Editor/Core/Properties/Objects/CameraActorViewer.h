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

	// -------------------
	// Camera actor viewer
	// -------------------
	class CameraActorViewer : public TObjectPropertiesViewer<CameraActor>
	{
	public:
		IOBJECT(CameraActorViewer);

	protected:
		VerticalLayout* mHiddenProperties = nullptr; // Hidden properties

		EnumProperty*  mTypeProperty = nullptr;

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
	PROTECTED_FIELD(mHiddenProperties).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mTypeProperty).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mSizePropertySpoiler).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mSizeProperty).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mUnitsPropertySpoiler).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mUnitsProperty).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(Editor::CameraActorViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PROTECTED_FUNCTION(void, RebuildProperties, _tmp1);
	PROTECTED_FUNCTION(void, OnTypeSelected);
}
END_META;
