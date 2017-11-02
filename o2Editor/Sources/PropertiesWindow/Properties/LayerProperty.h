#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Scene/Scene.h"
#include "Scene/SceneLayer.h"

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
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns value
		SceneLayer* GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(SceneLayer* value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(LayerProperty);

	protected:
		Function<void(void*, SceneLayer*)> mAssignFunc; // Value assign function
		Function<SceneLayer*(void*)>       mGetFunc;    // Get value function

		TargetsVec     mValuesPointers;           // Fields' pointers
		SceneLayer*    mCommonValue = nullptr;    // Common field value (if not different)
		bool           mValuesDifferent = true;   // Are values different

		UIWidget*      mPropertyWidget = nullptr; // Property widget
		UIButton*      mRevertBtn = nullptr;      // Revert to source prototype button		       
		UIDropDown*    mDropDown = nullptr;       // Layer name dropdown
		bool           mUpdatingValue = false;    // Is dropdown value updating and we don't we don't check selection

	protected:
		// Sets common value
		void SetCommonValue(SceneLayer* value);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// Updates layer list to actual list of layers on scene
		void UpdateLayersList();

		// Selects layer
		void SelectLayer(const WString& name);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetValueByUser(SceneLayer* value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}

CLASS_BASES_META(Editor::LayerProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::LayerProperty)
{
	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mDropDown);
	PROTECTED_FIELD(mUpdatingValue);
}
END_META;
CLASS_METHODS_META(Editor::LayerProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(SceneLayer*, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, SceneLayer*);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, SceneLayer*);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, UpdateLayersList);
	PROTECTED_FUNCTION(void, SelectLayer, const WString&);
	PROTECTED_FUNCTION(void, SetValueByUser, SceneLayer*);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
