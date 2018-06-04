#pragma once

#include "Core/Properties/FieldPropertiesInfo.h"
#include "Core/Properties/IPropertyField.h"
#include "PropertiesWindow/ActorsViewer/IActorComponentViewer.h"

namespace Editor
{
	// ------------------------------
	// Default actor component viewer
	// ------------------------------
	class DefaultActorComponentViewer: public IActorComponentViewer
	{
	public:
		// Default constructor. Initializes data widget
		DefaultActorComponentViewer();

		// Virtual destructor
		~DefaultActorComponentViewer();

		// Sets target actors
		void SetTargetComponents(const Vector<Component*>& components);

		// Returns viewing component type 
		const Type* GetComponentType() const;

		// Specialize viewing component type. Creates all using properties
		void SepcializeComponentType(const Type* type);

		// Updates all component values
		void Refresh();

		IOBJECT(DefaultActorComponentViewer);

	protected:
		Vector<Component*>  mTargetComponents;        // Target components
		FieldPropertiesInfo mFieldProperties;         // Field properties information
		const Type*         mComponentType = nullptr; // Target component type
	};
}

CLASS_BASES_META(Editor::DefaultActorComponentViewer)
{
	BASE_CLASS(Editor::IActorComponentViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultActorComponentViewer)
{
	PROTECTED_FIELD(mTargetComponents);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mComponentType);
}
END_META;
CLASS_METHODS_META(Editor::DefaultActorComponentViewer)
{

	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
	PUBLIC_FUNCTION(const Type*, GetComponentType);
	PUBLIC_FUNCTION(void, SepcializeComponentType, const Type*);
	PUBLIC_FUNCTION(void, Refresh);
}
END_META;
