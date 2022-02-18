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

	// ----------------------------------------
	// Editor actor properties viewer interface
	// ----------------------------------------
	class IActorPropertiesViewer: public IObject
	{
	public:
		// Default constructor. Initializes data widget
		IActorPropertiesViewer();

		// Virtual destructor
		virtual ~IActorPropertiesViewer();

		// Sets target actors
		virtual void SetTargetActors(const Vector<Actor*>& actors) {}

		// Returns viewing actor type 
		virtual const Type* GetActorType() const { return nullptr; }

		// Returns data widget
		virtual Widget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates all actor values
		virtual void Refresh();

		// Returns is there no properties
		virtual bool IsEmpty() const;

		// Sets viewer enabled
		void SetEnabled(bool enabled);

		// Returns is viewer enabled
		bool IsEnabled() const;

		IOBJECT(IActorPropertiesViewer);

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

CLASS_BASES_META(Editor::IActorPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorPropertiesViewer)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSpoiler).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mEnabled).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::IActorPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(const Type*, GetActorType);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
}
END_META;
