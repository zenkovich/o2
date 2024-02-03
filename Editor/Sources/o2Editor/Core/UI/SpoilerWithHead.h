#pragma once

#include "o2/Scene/UI/Widgets/Spoiler.h"
#include <memory>

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
		const Ref<Image>& GetIcon() const;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		IOBJECT(SpoilerWithHead);

	private:
		Ref<Button> mOptionsBtn; // Component option button
		Ref<Button> mSaveBtn;    // Save prototype button
		Ref<Image>  mIcon;       // Component icon

	private:
		// Initializes controls: options, save button and icon
		void InitializeControls();
	};
}

// --- META ---
CLASS_BASES_META(Editor::SpoilerWithHead)
{
    BASE_CLASS(o2::Spoiler);
}
END_META;
CLASS_FIELDS_META(Editor::SpoilerWithHead)
{
    FIELD().PUBLIC().NAME(onOptionsPressed);
    FIELD().PUBLIC().NAME(onSavePressed);
    FIELD().PRIVATE().DEFAULT_VALUE(Ref<Button>()).NAME(mOptionsBtn);
    FIELD().PRIVATE().DEFAULT_VALUE(Ref<Button>()).NAME(mSaveBtn);
    FIELD().PRIVATE().DEFAULT_VALUE(Ref<Image>()).NAME(mIcon);
}
END_META;
CLASS_METHODS_META(Editor::SpoilerWithHead)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const SpoilerWithHead&);
    FUNCTION().PUBLIC().SIGNATURE(const Ref<Image>&, GetIcon);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeControls);
}
END_META;
// --- END META ---