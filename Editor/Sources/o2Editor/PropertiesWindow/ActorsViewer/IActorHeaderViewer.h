#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Types/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Actor;
	class Widget;
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
		virtual Widget* GetWidget() const { return nullptr; }

		// Updates properties values
		virtual void Refresh() {}

		IOBJECT(IActorHeaderViewer);

	protected:
		// Enable viewer event function
		virtual void OnEnabled() {}

		// Disable viewer event function
		virtual void OnDisabled() {}

		friend class ActorViewer;
	};
}

CLASS_BASES_META(Editor::IActorHeaderViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorHeaderViewer)
{
}
END_META;
CLASS_METHODS_META(Editor::IActorHeaderViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
}
END_META;
