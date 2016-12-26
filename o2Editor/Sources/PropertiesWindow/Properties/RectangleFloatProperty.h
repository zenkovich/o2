#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIEditBox;
	class UIHorizontalLayout;
	class UIWidget;
}

namespace Editor
{
	// -------------------------------
	// Editor float rectangle property
	// -------------------------------
	class RectFProperty: public IPropertyField
	{
	public:
		// Default constructor
		RectFProperty(UIWidget* widget = nullptr);

		// Destructor
		~RectFProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Update();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Sets value
		void SetValue(const RectF& value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns value
		RectF GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(RectFProperty);

	protected:
		Function<void(void*, const RectF&)> mAssignFunc; // Value assign function
		Function<RectF(void*)>              mGetFunc;    // Get value function

		Vector<void*> mValuesPointers;          // Fields' pointers
		RectF         mCommonValue;             // Common field value (if not different)
		bool          mLeftValuesDifferent;     // Are left values different
		bool          mBottomValuesDifferent;   // Are bottom values different
		bool          mRightValuesDifferent;    // Are right values different
		bool          mTopValuesDifferent;      // Are top values different

		UIWidget*     mWidget = nullptr;        // horizontal edit boxes layout
		UIEditBox*    mLeftEditBox = nullptr;   // Left Edit box 
		UIEditBox*    mBottomEditBox = nullptr; // Bottom Edit box 
		UIEditBox*    mRightEditBox = nullptr;  // Right Edit box 
		UIEditBox*    mTopEditBox = nullptr;    // Top Edit box 

	protected:
		// Left Edit box change event
		void OnLeftEdited(const WString& data);

		// Bottom Edit box change event
		void OnBottomEdited(const WString& data);

		// Right Edit box change event
		void OnRightEdited(const WString& data);

		// Top Edit box change event
		void OnTopEdited(const WString& data);
	};
}
