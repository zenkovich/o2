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
	class IActorComponentViewer: public IObject, virtual public RefCounterable
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
		virtual Ref<Widget> GetWidget() const;

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

		Ref<SpoilerWithHead> mSpoiler;      // Component's spoiler
		Ref<Button>          mRemoveButton; // Remove component button

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
// --- META ---

CLASS_BASES_META(Editor::IActorComponentViewer)
{
    BASE_CLASS(o2::IObject);
    BASE_CLASS(o2::RefCounterable);
}
END_META;
CLASS_FIELDS_META(Editor::IActorComponentViewer)
{
    FIELD().PROTECTED().NAME(mTargetComponents);
    FIELD().PROTECTED().NAME(mSpoiler);
    FIELD().PROTECTED().NAME(mRemoveButton);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mEnabled);
}
END_META;
CLASS_METHODS_META(Editor::IActorComponentViewer)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargetComponents, const Vector<Component*>&);
    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetComponentType);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Widget>, GetWidget);
    FUNCTION().PUBLIC().SIGNATURE(void, Expand);
    FUNCTION().PUBLIC().SIGNATURE(void, Collapse);
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
    FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, RemoveTargetComponents);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
}
END_META;
// --- END META ---
