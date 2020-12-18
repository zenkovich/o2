#pragma once

#include "o2/Scene/Tags.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

namespace o2
{
	class Button;
	class ContextMenu;
	class EditBox;
	class Widget;
}

namespace Editor
{
	// ------------------------
	// Editor tags property box
	// ------------------------
	class TagsProperty: public TPropertyField<TagGroup>
	{
	public:
		// Default constructor
		TagsProperty();

		// Copy constructor
		TagsProperty(const TagsProperty& other);

		// Copy operator
		TagsProperty& operator=(const TagsProperty& other);

		IOBJECT(TagsProperty);

	protected:
		EditBox*     mEditBox = nullptr;     // Edit box 
		ContextMenu* mTagsContext = nullptr; // tags context
		bool         mPushingTag = false;    // Is pushing tag and we don't need to check edit text

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Sets common value
		void SetCommonValue(const TagGroup& value) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

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
	};
}

CLASS_BASES_META(Editor::TagsProperty)
{
	BASE_CLASS(Editor::TPropertyField<TagGroup>);
}
END_META;
CLASS_FIELDS_META(Editor::TagsProperty)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mEditBox).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTagsContext).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mPushingTag).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::TagsProperty)
{

	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, SetCommonValue, const TagGroup&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, UpdateContextData, const WString&);
	PROTECTED_FUNCTION(void, OnEditBoxChanged, const WString&);
	PROTECTED_FUNCTION(void, OnEditBoxChangeCompleted, const WString&);
	PROTECTED_FUNCTION(void, SetTags, const WString&);
	PROTECTED_FUNCTION(void, PushTag, String);
}
END_META;
