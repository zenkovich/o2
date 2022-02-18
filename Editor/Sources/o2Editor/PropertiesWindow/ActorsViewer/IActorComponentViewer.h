#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Reflection/Type.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"

using namespace o2;

namespace o2
{
	class Actor;
	class Button;
	class Component;
	class Widget;
}

namespace Editor
{
	class SpoilerWithHead;

	// ---------------------------------------
	// Editor actor component viewer interface
	// ---------------------------------------
	class IActorComponentViewer: public IObject
	{
	public:
		// Default constructor. Initializes data widget
		IActorComponentViewer();

		// Virtual destructor
		virtual ~IActorComponentViewer();

		// Sets target actors
		virtual void SetTargetComponents(const Vector<Component*>& components);

		// Returns viewing component type 
		virtual const Type* GetComponentType() const { return nullptr; }

		// Returns data widget
		virtual Widget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates all component values
		virtual void Refresh();

		// Sets viewer enabled
		void SetEnabled(bool enabled);

		// Returns is viewer enabled
		bool IsEnabled() const;

		IOBJECT(IActorComponentViewer);

	protected:
		Vector<Component*> mTargetComponents; // Target components

		SpoilerWithHead* mSpoiler = nullptr;      // Component's spoiler
		Button*          mRemoveButton = nullptr; // Remove component button

		bool mEnabled = false; // Is viewer enabled 

	protected:
		// Removes target components
		void RemoveTargetComponents();

		// Enable viewer event function
		virtual void OnEnabled() {}

		// Disable viewer event function
		virtual void OnDisabled() {}

		friend class ActorViewer;
	};
}

CLASS_BASES_META(Editor::IActorComponentViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorComponentViewer)
{
	FIELD().NAME(mTargetComponents).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSpoiler).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mRemoveButton).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mEnabled).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::IActorComponentViewer)
{

	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
	PUBLIC_FUNCTION(const Type*, GetComponentType);
	PUBLIC_FUNCTION(Widget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PROTECTED_FUNCTION(void, RemoveTargetComponents);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
}
END_META;
