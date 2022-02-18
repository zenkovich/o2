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

		// Sets viewer enabled
		void SetEnabled(bool enabled);

		// Returns is viewer enabled
		bool IsEnabled() const;

		IOBJECT(IActorTransformViewer);

	protected:
		SpoilerWithHead* mSpoiler = nullptr;

		bool mEnabled = false; // Is viewer enabled 

	protected:
		// Enable viewer event function
		virtual void OnEnabled() {}

		// Disable viewer event function
		virtual void OnDisabled() {}

		friend class ActorViewer;
	};
}

CLASS_BASES_META(Editor::IActorTransformViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorTransformViewer)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSpoiler).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mEnabled).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::IActorTransformViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
}
END_META;
