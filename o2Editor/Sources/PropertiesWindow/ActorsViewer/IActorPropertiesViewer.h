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
	// ----------------------------------------
	// Editor actor properties viewer interface
	// ----------------------------------------
	class IActorPropertiesViewer: public IObject
	{
	public:
		// Default constructor. Initializes data widget
		IActorPropertiesViewer();

		// Virtual destructor
		virtual ~IActorPropertiesViewer();

		// Sets target actors
		virtual void SetTargetActors(const Vector<Actor*>& actors) {}

		// Returns viewing actor type 
		virtual const Type* GetActorType() const { return nullptr; }

		// Returns data widget
		virtual UIWidget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates all actor values
		virtual void Refresh();

		IOBJECT(IActorPropertiesViewer);

	protected:
		UIVerticalLayout* mDataView = nullptr;         // Data view widget
		UIVerticalLayout* mPropertiesLayout = nullptr; // Properties layout
		UIButton*         mExpandBtn = nullptr;        // Expand spoiler button
		UISpoiler*        mSpoiler = nullptr;          // Spoiler, containing properties layout
		UILabel*          mNameCaption = nullptr;      // Actor type name caption label
		UIButton*         mOptionsBtn = nullptr;       // Option button
		UIButton*         mSaveBtn = nullptr;          // Save prototype button
		UIImage*          mIcon = nullptr;             // Actor icon
	};
}

CLASS_BASES_META(Editor::IActorPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorPropertiesViewer)
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
CLASS_METHODS_META(Editor::IActorPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(const Type*, GetActorType);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
}
END_META;
