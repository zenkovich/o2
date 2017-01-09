#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Scene/Tags.h"

namespace o2
{
	class UIEditBox;
	class UIContextMenu;
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
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		const TagGroup& GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const TagGroup& value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(TagsProperty);

	protected:
		Function<void(void*, const TagGroup&)> mAssignFunc; // Value assign function
		Function<TagGroup(void*)>              mGetFunc;    // Get value function

		Vector<void*>  mValuesPointers;     // Fields' pointers
		TagGroup       mCommonValue;        // Common field value (if not different)
		bool           mValuesDifferent;    // Are values different

		UIEditBox*     mEditBox;            // Tags edit box
		UIContextMenu* mTagsContext;        // tags context
		bool           mPushingTag = false; // Is pushing tag and we don't need to check edit text

	protected:
		// Sets common value
		void SetCommonValue(const TagGroup& value);

		// Updates context menu data with filter
		void UpdateContextData(const WString& filter);

		// Calls when edit box changed
		void OnEditBoxChanged(const WString& text);

		// Calls when edit box changed
		void OnEditBoxChangeCompleted(const WString& text);

		// Sets tags from string
		void SetTags(const WString& text);

		// Push tag at the end
		void PushTag(String name);
	};
}
