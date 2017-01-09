#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Scene/Scene.h"

namespace o2
{
	class Text;
	class UIDropDown;
}

namespace Editor
{
	// -------------------------
	// Editor layer property box
	// -------------------------
	class LayerProperty: public IPropertyField
	{
	public:
		// Default constructor
		LayerProperty(UIWidget* widget = nullptr);

		// Destructor
		~LayerProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		Scene::Layer* GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(Scene::Layer* value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(LayerProperty);

	protected:
		Function<void(void*, Scene::Layer*)> mAssignFunc; // Value assign function
		Function<Scene::Layer*(void*)>       mGetFunc;    // Get value function

		Vector<void*>  mValuesPointers;        // Fields' pointers
		Scene::Layer*  mCommonValue;           // Common field value (if not different)
		bool           mValuesDifferent;       // Are values different
										       
		UIDropDown*    mDropDown;              // Layer name dropdown
		bool           mUpdatingValue = false; // Is dropdown value updating and we don't we don't check selection

	protected:
		// Sets common value
		void SetCommonValue(Scene::Layer* value);

		// Updates layer list to actual list of layers on scene
		void UpdateLayersList();

		// Selects layer
		void SelectLayer(const WString& name);
	};
}

