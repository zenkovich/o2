#pragma once

#include "o2Editor/PropertiesWindow/DefaultPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Label;
	class Button;
}

namespace Editor
{
	class DefaultObjectPropertiesViewer;

	// -------------------------
	// Default properties viewer
	// -------------------------
	class AssetPropertiesViewer : public DefaultPropertiesViewer
	{
	public:
		// Default constructor, creates asset header controls
		AssetPropertiesViewer();

		// Returns viewing object type
		const Type* GetViewingObjectType() const override;

		IOBJECT(AssetPropertiesViewer);

	protected:
		Widget* mAssetHeader = nullptr;    // Asset header
		Label* mAssetNameLabel = nullptr; // Asset name label
		Button* mSaveButton = nullptr;     // Save asset button

	protected:
		// Sets target objects
		void SetTargets(const Vector<IObject*>& targets) override;

		// Called when save button is pressed
		void OnSavePressed();
	};

}
// --- META ---

CLASS_BASES_META(Editor::AssetPropertiesViewer)
{
    BASE_CLASS(Editor::DefaultPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::AssetPropertiesViewer)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAssetHeader);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAssetNameLabel);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSaveButton);
}
END_META;
CLASS_METHODS_META(Editor::AssetPropertiesViewer)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
    FUNCTION().PROTECTED().SIGNATURE(void, SetTargets, const Vector<IObject*>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSavePressed);
}
END_META;
// --- END META ---
