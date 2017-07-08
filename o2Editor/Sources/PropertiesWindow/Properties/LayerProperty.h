#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Scene/Scene.h"

namespace o2
{
	class Text;
	class UIButton;
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
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Reverts value to prototype value
		void Revert();

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

		TargetsVec     mValuesPointers;           // Fields' pointers
		Scene::Layer*  mCommonValue = nullptr;    // Common field value (if not different)
		bool           mValuesDifferent = true;   // Are values different

		UIWidget*      mPropertyWidget = nullptr; // Property widget
		UIButton*      mRevertBtn = nullptr;      // Revert to source prototype button		       
		UIDropDown*    mDropDown = nullptr;       // Layer name dropdown
		bool           mUpdatingValue = false;    // Is dropdown value updating and we don't we don't check selection

	protected:
		// Sets common value
		void SetCommonValue(Scene::Layer* value);

		// Checks value for reverting to prototype
		void CheckRevertableState();

		// Updates layer list to actual list of layers on scene
		void UpdateLayersList();

		// Selects layer
		void SelectLayer(const WString& name);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetValueByUser(Scene::Layer* value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}
