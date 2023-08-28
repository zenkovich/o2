#pragma once

#include "o2/Scene/SceneLayerRef.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

namespace o2
{
	class SceneLayer;
	class DropDown;
}

namespace Editor
{
	// -------------------------
	// Editor layer property box
	// -------------------------
	class SceneLayerRefProperty: public TPropertyField<Ref<SceneLayer>>
	{
	public:
		// Default constructor
		SceneLayerRefProperty();

		// Copy constructor
		SceneLayerRefProperty(const SceneLayerRefProperty& other);

		// Copy operator
		SceneLayerRefProperty& operator=(const SceneLayerRefProperty& other);

		IOBJECT(SceneLayerRefProperty);

	protected:		       
		DropDown* mDropDown = nullptr;       // Layer name dropdown
		bool      mUpdatingValue = false;    // Is dropdown value updating and we don't we don't check selection

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Updates layer list to actual list of layers on scene
		void UpdateLayersList();

		// Selects layer
		void SelectLayer(const WString& name);

		// Is required refresh view every time
		bool IsAlwaysRefresh() const override;
	};
}
// --- META ---

CLASS_BASES_META(Editor::SceneLayerRefProperty)
{
	BASE_CLASS(Editor::TPropertyField<Ref<SceneLayer>>);
}
END_META;
CLASS_FIELDS_META(Editor::SceneLayerRefProperty)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDropDown);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mUpdatingValue);
}
END_META;
CLASS_METHODS_META(Editor::SceneLayerRefProperty)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const SceneLayerRefProperty&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateLayersList);
	FUNCTION().PROTECTED().SIGNATURE(void, SelectLayer, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(bool, IsAlwaysRefresh);
}
END_META;
// --- END META ---
