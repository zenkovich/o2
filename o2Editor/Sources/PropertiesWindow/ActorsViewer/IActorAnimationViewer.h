#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"

using namespace o2;

namespace o2
{
	class Actor;
	class UIWidget;
}

namespace Editor
{
	// ---------------------------------------
	// Editor actor animation viewer interface
	// ---------------------------------------
	class IActorAnimationViewer: public IObject
	{
	public:
		// Default constructor. Initializes data widget
		IActorAnimationViewer();

		// Virtual destructor
		virtual ~IActorAnimationViewer();

		// Sets target actors
		virtual void SetTargetActors(const Vector<Actor*>& actors) {}

		// Returns data widget
		virtual UIWidget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		IOBJECT(IActorAnimationViewer);

	protected:
		UIWidget* mDataView; // Data view widget
	};
}
