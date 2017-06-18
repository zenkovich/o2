#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/CursorEventsArea.h"
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
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Reverts value to prototype value
		void Revert();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		Color4 GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const Color4& value);

		// Sets value as unknown
		void SetUnknownValue(const Color4& defaultValue = Color4());

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(ColorProperty);

	protected:
		Function<void(void*, const Color4&)> mAssignFunc; // Value assign function
		Function<Color4(void*)>              mGetFunc;    // Get value function

		TargetsVec       mValuesPointers;         // Fields' pointers
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
		void CheckRevertableState();

		// Calls when box pressed and shows color picking dialog
		void OnClicked();

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();
	};
}
