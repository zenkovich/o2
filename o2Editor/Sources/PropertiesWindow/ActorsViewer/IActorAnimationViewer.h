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
		UIWidget* mDataView = nullptr; // Data view widget
	};
}

CLASS_BASES_META(Editor::IActorAnimationViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorAnimationViewer)
{
	PROTECTED_FIELD(mDataView);
}
END_META;
CLASS_METHODS_META(Editor::IActorAnimationViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
}
END_META;
