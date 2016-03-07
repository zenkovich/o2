#pragma once

#include "PropertiesWindow/ActorsViewer/IActorComponentViewer.h"
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

		IOBJECT(DefaultActorComponentViewer);

	protected:
		Vector<Component*>            mTargetComponents;   // Target components
		Vector<IPropertyField*> mUsedPropertyFields; // Used property fields
	};
}