#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Events/CursorEventsArea.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIImage;
	class UIWidget;
	class UIWindow;
}

namespace Editor
{
	// ------------------------------
	// Editor color property edit box
	// ------------------------------
	class ColorProperty: public IPropertyField
	{
	public:
		// Default constructor
		ColorProperty(UIWidget* widget = nullptr);

		// Destructor
		~ColorProperty();

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns value
		Color4 GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const Color4& value);

		// Sets value as unknown
		void SetUnknownValue(const Color4& defaultValue = Color4());

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(ColorProperty);

	protected:
		TargetsVec       mValuesProxies;         // Fields' pointers
		Color4           mCommonValue;            // Common field value (if not different)
		bool             mValuesDifferent = true; // Are values different

		UIWidget*        mPropertyWidget = nullptr; // Root property widget
		UIWidget*        mEditBox = nullptr;        // Edit box 
		UIButton*        mRevertBtn = nullptr;      // Property revert button to prototype source
		UIImage*         mColorSprite = nullptr;    // Color preview sprite
		CursorEventsArea mClickArea;                // Box click area

	protected:
		// Sets common value
		void SetCommonValue(const Color4& value);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// It is called when box pressed and shows color picking dialog
		void OnClicked();

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}

CLASS_BASES_META(Editor::ColorProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::ColorProperty)
{
	PROTECTED_FIELD(mValuesProxies);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mColorSprite);
	PROTECTED_FIELD(mClickArea);
}
END_META;
CLASS_METHODS_META(Editor::ColorProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(Color4, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, const Color4&);
	PUBLIC_FUNCTION(void, SetUnknownValue, const Color4&);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const Color4&);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnClicked);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
