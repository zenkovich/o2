#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/IObject.h"
#include "Utils/Reflection/Reflection.h"

using namespace o2;

namespace o2
{
	class Actor;
	class UIWidget;
}

namespace Editor
{
	// ------------------------------------
	// Editor actor header viewer interface
	// ------------------------------------
	class IActorHeaderViewer: public IObject
	{
	public:
		// Virtual destructor
		virtual ~IActorHeaderViewer() {}

		// Sets target actors
		virtual void SetTargetActors(const Vector<Actor*>& actors) {}

		// Returns data widget
		virtual UIWidget* GetWidget() const { return nullptr; }

		// Updates properties values
		virtual void Refresh() {}

		IOBJECT(IActorHeaderViewer);
	};
}