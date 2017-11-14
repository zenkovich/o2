#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Scene/Tags.h"

namespace o2
{
	class UIButton;
	class UIContextMenu;
	class UIEditBox;
	class UIWidget;
}

namespace Editor
{
	// ------------------------
	// Editor tags property box
	// ------------------------
	class TagsProperty: public IPropertyField
	{
	public:
		// Default constructor
		TagsProperty(UIWidget* widget = nullptr);

		// Destructor
		~TagsProperty();

		// Sets fields
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns value
		const TagGroup& GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const TagGroup& value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(TagsProperty);

	protected:
		Function<void(void*, const TagGroup&)> mAssignFunc; // Value assign function
		Function<TagGroup(void*)>              mGetFunc;    // Get value function

		TargetsVec mValuesPointers;         // Fields' pointers
		TagGroup   mCommonValue;            // Common field value (if not different)
		bool       mValuesDifferent = true; // Are values different

		UIWidget*      mPropertyWidget = nullptr; // Property root widget, contains editbox and revert button
		UIEditBox*     mEditBox = nullptr;        // Edit box 
		UIButton*      mRevertBtn = nullptr;      // Revert to prototype button
		UIContextMenu* mTagsContext = nullptr;    // tags context
		bool           mPushingTag = false;       // Is pushing tag and we don't need to check edit text

	protected:
		// Sets common value
		void SetCommonValue(const TagGroup& value);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// Updates context menu data with filter
		void UpdateContextData(const WString& filter);

		// It is called when edit box changed
		void OnEditBoxChanged(const WString& text);

		// It is called when edit box changed
		void OnEditBoxChangeCompleted(const WString& text);

		// Sets tags from string
		void SetTags(const WString& text);

		// Push tag at the end
		void PushTag(String name);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetValueByUser(const TagGroup& value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}

CLASS_BASES_META(Editor::TagsProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::TagsProperty)
{
	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mTagsContext);
	PROTECTED_FIELD(mPushingTag);
}
END_META;
CLASS_METHODS_META(Editor::TagsProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(const TagGroup&, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, const TagGroup&);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const TagGroup&);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, UpdateContextData, const WString&);
	PROTECTED_FUNCTION(void, OnEditBoxChanged, const WString&);
	PROTECTED_FUNCTION(void, OnEditBoxChangeCompleted, const WString&);
	PROTECTED_FUNCTION(void, SetTags, const WString&);
	PROTECTED_FUNCTION(void, PushTag, String);
	PROTECTED_FUNCTION(void, SetValueByUser, const TagGroup&);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
