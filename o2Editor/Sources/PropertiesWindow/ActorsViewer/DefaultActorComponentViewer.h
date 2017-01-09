#pragma once

#include "PropertiesWindow/ActorsViewer/IActorComponentViewer.h"
#include "PropertiesWindow/Properties/FieldPropertiesInfo.h"
#include "PropertiesWindow/Properties/IPropertyField.h"

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