#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Reflection/Type.h"
#include "o2/Utils/Types/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Actor;
	class Component;
	class Widget;
}

namespace Editor
{
	class SpoilerWithHead;

	// ---------------------------------------
	// Editor actor transform viewer interface
	// ---------------------------------------
	class IActorTransformViewer: public IObject
	{
	public:
		// Default constructor. Initializes data widget
		IActorTransformViewer();

		// Virtual destructor
		virtual ~IActorTransformViewer();

		// Sets target actors
		virtual void SetTargetActors(const Vector<Actor*>& actors) {}

		// Returns data widget
		virtual Widget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates properties values
		virtual void Refresh();

		IOBJECT(IActorTransformViewer);

	protected:
		SpoilerWithHead* mSpoiler = nullptr;
	};
}

CLASS_BASES_META(Editor::IActorTransformViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorTransformViewer)
{
	PROTECTED_FIELD(mSpoiler);
}
END_META;
CLASS_METHODS_META(Editor::IActorTransformViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
}
END_META;
