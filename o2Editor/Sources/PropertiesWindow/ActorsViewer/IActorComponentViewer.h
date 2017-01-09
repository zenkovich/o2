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
	// Editor actor component viewer interface
	// ---------------------------------------
	class IActorComponentViewer: public IObject
	{
	public:
		// Default constructor. Initializes data widget
		IActorComponentViewer();

		// Virtual destructor
		virtual ~IActorComponentViewer();

		// Sets target actors
		virtual void SetTargetComponents(const Vector<Component*>& components) {}

		// Returns viewing component type 
		virtual const Type* GetComponentType() const { return nullptr; }

		// Returns data widget
		virtual UIWidget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates all component values
		virtual void Refresh();

		IOBJECT(IActorComponentViewer);

	protected:
		UIVerticalLayout* mDataView;         // Data view widget
		UIVerticalLayout* mPropertiesLayout; // Properties layout
		UIButton*         mExpandBtn;        // Expand spoiler button
		UISpoiler*        mSpoiler;          // Spoiler, containing properties layout
		UILabel*          mNameCaption;      // Component name caption label
		UIButton*         mOptionsBtn;       // Component option button
		UIButton*         mSaveBtn;          // Save prototype button
		UIImage*          mIcon;             // Component icon
	};
}