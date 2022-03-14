#pragma once

#include "o2/Scene/SceneLayer.h"
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
	class LayerProperty: public TPropertyField<SceneLayer*>
	{
	public:
		// Default constructor
		LayerProperty();

		// Copy constructor
		LayerProperty(const LayerProperty& other);

		// Copy operator
		LayerProperty& operator=(const LayerProperty& other);

		IOBJECT(LayerProperty);

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

CLASS_BASES_META(Editor::LayerProperty)
{
	BASE_CLASS(Editor::TPropertyField<SceneLayer*>);
}
END_META;
CLASS_FIELDS_META(Editor::LayerProperty)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDropDown);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mUpdatingValue);
}
END_META;
CLASS_METHODS_META(Editor::LayerProperty)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const LayerProperty&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateLayersList);
	FUNCTION().PROTECTED().SIGNATURE(void, SelectLayer, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(bool, IsAlwaysRefresh);
}
END_META;
