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
		UIVerticalLayout* mDataView = nullptr;         // Data view widget
		UIVerticalLayout* mPropertiesLayout = nullptr; // Properties layout
		UIButton*         mExpandBtn = nullptr;        // Expand spoiler button
		UISpoiler*        mSpoiler = nullptr;          // Spoiler, containing properties layout
		UILabel*          mNameCaption = nullptr;      // Component name caption label
		UIButton*         mOptionsBtn = nullptr;       // Component option button
		UIButton*         mSaveBtn = nullptr;          // Save prototype button
		UIImage*          mIcon = nullptr;             // Component icon
	};
}

CLASS_BASES_META(Editor::IActorComponentViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;

CLASS_FIELDS_META(Editor::IActorComponentViewer)
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

CLASS_METHODS_META(Editor::IActorComponentViewer)
{
	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
	PUBLIC_FUNCTION(const Type*, GetComponentType);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
}
END_META;
