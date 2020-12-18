#pragma once

#include "o2/Scene/UI/Widgets/Spoiler.h"

using namespace o2;

namespace o2
{
	class Button;
	class Image;
}

namespace Editor
{
	// -----------------------------------------------
	// Editor properties spoiler with head and caption
	// -----------------------------------------------
	class SpoilerWithHead : public Spoiler
	{
	public:
		Function<void()> onOptionsPressed; // Options button pressed event
		Function<void()> onSavePressed;    // Save button pressed event

	public:
		// Default constructor
		SpoilerWithHead();

		// Copy constructor
		SpoilerWithHead(const SpoilerWithHead& other);

		// Copy operator
		SpoilerWithHead& operator=(const SpoilerWithHead& other);

		// Returns icon
		Image* GetIcon() const;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		IOBJECT(SpoilerWithHead);

	private:
		Button* mOptionsBtn = nullptr; // Component option button
		Button* mSaveBtn = nullptr;    // Save prototype button
		Image*  mIcon = nullptr;       // Component icon

	private:
		// Initializes controls: options, save button and icon
		void InitializeControls();
	};
}

CLASS_BASES_META(Editor::SpoilerWithHead)
{
	BASE_CLASS(o2::Spoiler);
}
END_META;
CLASS_FIELDS_META(Editor::SpoilerWithHead)
{
	FIELD().NAME(onOptionsPressed).PUBLIC();
	FIELD().NAME(onSavePressed).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mOptionsBtn).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSaveBtn).PRIVATE();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mIcon).PRIVATE();
}
END_META;
CLASS_METHODS_META(Editor::SpoilerWithHead)
{

	PUBLIC_FUNCTION(Image*, GetIcon);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PRIVATE_FUNCTION(void, InitializeControls);
}
END_META;
