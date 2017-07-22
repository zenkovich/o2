#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Render/TextureRef.h"
#include "Utils/CursorEventsArea.h"
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
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Reverts value to prototype value
		void Revert();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		const Curve& GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const Curve& value);

		// Sets value as unknown
		void SetUnknownValue(const Curve& defaultValue = Curve());

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(CurveProperty);

	protected:
		Function<void(void*, const Curve&)> mAssignFunc; // Value assign function
		Function<Curve(void*)>              mGetFunc;    // Get value function

		TargetsVec       mValuesPointers;            // Fields' pointers
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
		void CheckRevertableState();

		// It is called when box pressed and shows color picking dialog
		void OnClicked();

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}
