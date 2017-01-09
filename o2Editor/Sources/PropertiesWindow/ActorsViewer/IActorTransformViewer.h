#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/IObject.h"
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