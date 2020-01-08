#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2/Scene/SceneLayer.h"

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
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Updates layer list to actual list of layers on scene
		void UpdateLayersList();

		// Selects layer
		void SelectLayer(const WString& name);
	};
}

CLASS_BASES_META(Editor::LayerProperty)
{
	BASE_CLASS(Editor::TPropertyField<SceneLayer*>);
}
END_META;
CLASS_FIELDS_META(Editor::LayerProperty)
{
	PROTECTED_FIELD(mDropDown);
	PROTECTED_FIELD(mUpdatingValue);
}
END_META;
CLASS_METHODS_META(Editor::LayerProperty)
{

	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, UpdateLayersList);
	PROTECTED_FUNCTION(void, SelectLayer, const WString&);
}
END_META;
