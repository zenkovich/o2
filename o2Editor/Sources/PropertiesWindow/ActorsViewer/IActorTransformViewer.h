#pragma once

#include "Utils/Types/Containers/Vector.h"
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"

using namespace o2;

namespace o2
{
	class Actor;
	class Component;
	class UIButton;
	class UIImage;
	class UILabel;
	class UISpoiler;
	class UIVerticalLayout;
	class UIWidget;
}

namespace Editor
{
	// ---------------------------------------
	// Editor actor transform viewer interface
	// ---------------------------------------
	class IActorTransformViewer: public IObject
	{
	public:
		// Default constructor. Initializes data widget
		IActorTransformViewer();

		// Virtual destructor
		virtual ~IActorTransformViewer();

		// Sets target actors
		virtual void SetTargetActors(const Vector<Actor*>& actors) {}

		// Returns data widget
		virtual UIWidget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates properties values
		virtual void Refresh();

		IOBJECT(IActorTransformViewer);

	protected:
		UIVerticalLayout* mDataView;         // Data view widget
		UIWidget*         mPropertiesLayout; // Properties layout
		UIButton*         mExpandBtn;        // Expand spoiler button
		UISpoiler*        mSpoiler;          // Spoiler, containing properties layout
		UILabel*          mNameCaption;      // Component name caption label
		UIButton*         mOptionsBtn;       // Component option button
		UIButton*         mSaveBtn;          // Save prototype button
		UIImage*          mIcon;             // Component icon
	};
}

CLASS_BASES_META(Editor::IActorTransformViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorTransformViewer)
{
	PROTECTED_FIELD(mDataView);
	PROTECTED_FIELD(mPropertiesLayout);
	PROTECTED_FIELD(mExpandBtn);
	PROTECTED_FIELD(mSpoiler);
	PROTECTED_FIELD(mNameCaption);
	PROTECTED_FIELD(mOptionsBtn);
	PROTECTED_FIELD(mSaveBtn);
	PROTECTED_FIELD(mIcon);
}
END_META;
CLASS_METHODS_META(Editor::IActorTransformViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
}
END_META;
