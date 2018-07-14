#pragma once

#include "UI/Spoiler.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIImage;
}

namespace Editor
{
	// -----------------------------------------------
	// Editor properties spoiler with head and caption
	// -----------------------------------------------
	class UISpoilerWithHead : public UISpoiler
	{
	public:
		Function<void()> onOptionsPressed; // Options button pressed event
		Function<void()> onSavePressed;    // Save button pressed event

		// Default constructor
		UISpoilerWithHead();

		// Copy constructor
		UISpoilerWithHead(const UISpoilerWithHead& other);

		// Copy operator
		UISpoilerWithHead& operator=(const UISpoilerWithHead& other);

		// Returns icon
		UIImage* GetIcon() const;

		IOBJECT(UISpoilerWithHead);

	private:
		UIButton* mOptionsBtn = nullptr; // Component option button
		UIButton* mSaveBtn = nullptr;    // Save prototype button
		UIImage*  mIcon = nullptr;       // Component icon

	private:
		// Initializes controls: options, save button and icon
		void InitializeControls();

		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;
	};
}

CLASS_BASES_META(Editor::UISpoilerWithHead)
{
	BASE_CLASS(o2::UISpoiler);
}
END_META;
CLASS_FIELDS_META(Editor::UISpoilerWithHead)
{
	PUBLIC_FIELD(onOptionsPressed);
	PUBLIC_FIELD(onSavePressed);
	PRIVATE_FIELD(mOptionsBtn);
	PRIVATE_FIELD(mSaveBtn);
	PRIVATE_FIELD(mIcon);
}
END_META;
CLASS_METHODS_META(Editor::UISpoilerWithHead)
{

	PUBLIC_FUNCTION(UIImage*, GetIcon);
	PRIVATE_FUNCTION(void, InitializeControls);
	PRIVATE_FUNCTION(void, CopyData, const Actor&);
}
END_META;
