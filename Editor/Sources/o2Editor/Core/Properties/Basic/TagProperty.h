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

		// Called when edit box changed
		void OnEditBoxChanged(const WString& text);

		// Called when edit box changed
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
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEditBox);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTagsContext);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mPushingTag);
}
END_META;
CLASS_METHODS_META(Editor::TagsProperty)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const TagsProperty&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
	FUNCTION().PROTECTED().SIGNATURE(void, SetCommonValue, const TagGroup&);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateContextData, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEditBoxChanged, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEditBoxChangeCompleted, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, SetTags, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, PushTag, String);
}
END_META;
