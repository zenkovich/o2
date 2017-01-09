#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/CursorEventsArea.h"
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
	// ----------------------------
	// Editor float border property
	// ----------------------------
	class BorderFProperty: public IPropertyField
	{
	public:
		// Default constructor
		BorderFProperty(UIWidget* widget = nullptr);

		// Destructor
		~BorderFProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Sets value
		void SetValue(const BorderF& value);

		// Sets value left
		void SetValueLeft(float value);

		// Sets value right
		void SetValueRight(float value);

		// Sets value top
		void SetValueTop(float value);

		// Sets value bottom
		void SetValueBottom(float value);

		// Sets value as unknown
		void SetUnknownValue(const BorderF& defaultValue = BorderF());

		// Sets value left as unknown
		void SetLeftUnknownValue(float defaultValue = 0.0f);

		// Sets value right as unknown
		void SetRightUnknownValue(float defaultValue = 0.0f);

		// Sets value top as unknown
		void SetTopUnknownValue(float defaultValue = 0.0f);

		// Sets value bottom as unknown
		void SetBottomUnknownValue(float defaultValue = 0.0f);

		// Returns value
		BorderF GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(BorderFProperty);

	protected:
		Function<void(void*, const BorderF&)> mAssignFunc;      // Value assign function
		Function<BorderF(void*)>              mGetFunc;         // Get value function

		Function<void(void*, float)>          mLeftAssignFunc;  // Left Value assign function
		Function<float(void*)>                mLeftGetFunc;     // Get left value function

		Function<void(void*, float)>          mRightAssignFunc; // Right Value assign function
		Function<float(void*)>                mRightGetFunc;    // Get right value function

		Function<void(void*, float)>          mTopAssignFunc;   // Top Value assign function
		Function<float(void*)>                mTopGetFunc;      // Get top value function

		Function<void(void*, float)>          mBottomAssignFunc; // Bottom Value assign function
		Function<float(void*)>                mBottomGetFunc;    // Get bottom value function

		Vector<void*> mValuesPointers;                 // Fields' pointers
		BorderF       mCommonValue;                    // Common field value (if not different)
		bool          mLeftValuesDifferent = true;     // Are left values different
		bool          mBottomValuesDifferent = true;   // Are bottom values different
		bool          mRightValuesDifferent = true;    // Are right values different
		bool          mTopValuesDifferent = true;      // Are top values different

		UIWidget*     mWidget = nullptr;               // horizontal edit boxes layout
		UIEditBox*    mLeftEditBox = nullptr;          // Left Edit box 
		UIEditBox*    mBottomEditBox = nullptr;        // Bottom Edit box 
		UIEditBox*    mRightEditBox = nullptr;         // Right Edit box 
		UIEditBox*    mTopEditBox = nullptr;           // Top Edit box 

		CursorEventsArea mLeftDragHangle;              // Left Value changing drag handle
		CursorEventsArea mRightDragHangle;             // Right Value changing drag handle
		CursorEventsArea mTopDragHangle;               // Top Value changing drag handle
		CursorEventsArea mBottomDragHangle;            // Bottom Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(const BorderF& value);

		// Sets common value left
		void SetCommonValueLeft(float value);

		// Sets common value right
		void SetCommonValueRight(float value);

		// Sets common value top
		void SetCommonValueTop(float value);

		// Sets common value bottom
		void SetCommonValueBottom(float value);

		// Left Edit box change event
		void OnLeftEdited(const WString& data);

		// Bottom Edit box change event
		void OnBottomEdited(const WString& data);

		// Right Edit box change event
		void OnRightEdited(const WString& data);

		// Top Edit box change event
		void OnTopEdited(const WString& data);

		// Returns value increasing multiplier by cursor delta
		float GetValueMultiplier(float delta) const;

		// Calls when drag handle was moved and changes the property value
		void OnLeftDragHandleMoved(const Input::Cursor& cursor);

		// Calls when drag handle was moved and changes the property value
		void OnRightDragHandleMoved(const Input::Cursor& cursor);

		// Calls when drag handle was moved and changes the property value
		void OnTopDragHandleMoved(const Input::Cursor& cursor);

		// Calls when drag handle was moved and changes the property value
		void OnBottomDragHandleMoved(const Input::Cursor& cursor);
	};
}
