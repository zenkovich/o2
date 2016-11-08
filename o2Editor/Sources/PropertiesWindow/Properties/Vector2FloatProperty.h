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
	// -----------------------------
	// Editor float vector2 property
	// -----------------------------
	class Vec2FProperty: public IPropertyField
	{
	public:
		// Default constructor
		Vec2FProperty(UIWidget* widget = nullptr);

		// Destructor
		~Vec2FProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Update();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Sets value
		void SetValue(const Vec2F& value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns value
		Vec2F GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(Vec2FProperty);

	protected:
		Function<void(void*, const Vec2F&)> mAssignFunc; // Value assign function
		Function<Vec2F(void*)>              mGetFunc;    // Get value function

		Vector<void*> mValuesPointers;   // Fields' pointers
		Vec2F         mCommonValue;      // Common field value (if not different)
		bool          mXValuesDifferent; // Are x values different
		bool          mYValuesDifferent; // Are y values different
					  
		UIWidget*     mWidget;           // horizontal edit boxes layout
		UIEditBox*    mXEditBox;         // X Edit box 
		UIEditBox*    mYEditBox;         // Y Edit box 

	protected:
		// X Edit box change event
		void OnXEdited(const WString& data);

		// Y Edit box change event
		void OnYEdited(const WString& data);
	};
}
