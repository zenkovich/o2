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

		IOBJECT(SpoilerWithHead);

	private:
		Button* mOptionsBtn = nullptr; // Component option button
		Button* mSaveBtn = nullptr;    // Save prototype button
		Image*  mIcon = nullptr;       // Component icon

	private:
		// Initializes controls: options, save button and icon
		void InitializeControls();

		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;
	};
}

CLASS_BASES_META(Editor::SpoilerWithHead)
{
	BASE_CLASS(o2::Spoiler);
}
END_META;
CLASS_FIELDS_META(Editor::SpoilerWithHead)
{
	PUBLIC_FIELD(onOptionsPressed);
	PUBLIC_FIELD(onSavePressed);
	PRIVATE_FIELD(mOptionsBtn).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mSaveBtn).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mIcon).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(Editor::SpoilerWithHead)
{

	PUBLIC_FUNCTION(Image*, GetIcon);
	PRIVATE_FUNCTION(void, InitializeControls);
	PRIVATE_FUNCTION(void, CopyData, const Actor&);
}
END_META;
