#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Render/TextureRef.h"
#include "Events/CursorEventsArea.h"
#include "Utils/Math/Curve.h"
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
	class UICurvePreview;

	// ------------------------------
	// Editor curve property edit box
	// ------------------------------
	class CurveProperty: public IPropertyField
	{
	public:
		// Default constructor
		CurveProperty(UIWidget* widget = nullptr);

		// Destructor
		~CurveProperty();

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns value
		const Curve& GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const Curve& value);

		// Sets value as unknown
		void SetUnknownValue(const Curve& defaultValue = Curve());

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(CurveProperty);

	protected:
		TargetsVec       mValuesProxies;             // Fields' pointers
		Curve            mCommonValue;               // Common field value (if not different)
		bool             mValuesDifferent = true;    // Are values different

		UIWidget*        mPropertyWidget = nullptr;  // Root property widget
		UIWidget*        mEditBox = nullptr;         // Edit box 
		UICurvePreview*  mPreviewImage = nullptr;    // Curve preview image
		UIButton*        mRevertBtn = nullptr;       // Property revert button to prototype source
		CursorEventsArea mClickArea;                 // Box click area

	protected:
		// It is called when common curve changed in curve edit dialog, tells to drawing content
		void OnValueChanged();

		// Sets common value
		void SetCommonValue(const Curve& value);

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

CLASS_BASES_META(Editor::CurveProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::CurveProperty)
{
	PROTECTED_FIELD(mValuesProxies);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mPreviewImage);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mClickArea);
}
END_META;
CLASS_METHODS_META(Editor::CurveProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(const Curve&, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, const Curve&);
	PUBLIC_FUNCTION(void, SetUnknownValue, const Curve&);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, OnValueChanged);
	PROTECTED_FUNCTION(void, SetCommonValue, const Curve&);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnClicked);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
