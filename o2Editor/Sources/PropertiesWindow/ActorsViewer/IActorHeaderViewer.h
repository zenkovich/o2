#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Types/Containers/Vector.h"

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
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Refresh);
}
END_META;
