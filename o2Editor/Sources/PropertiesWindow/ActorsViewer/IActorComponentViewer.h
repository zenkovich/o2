#pragma once

#include "Utils/Types/Containers/Vector.h"
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"

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

		// Rebuilds properties layout
		virtual void Rebuild();

		// Returns true when properties was built with hidden fields
		virtual bool IsBuiltWithEmpty() const;

		IOBJECT(IActorComponentViewer);

	protected:
		Vector<Component*> mTargetComponents;

		SpoilerWithHead* mSpoiler = nullptr;
		Button*          mRemoveButton = nullptr;

	protected:
		// Removes target components
		void RemoveTargetComponents();
	};
}

CLASS_BASES_META(Editor::IActorComponentViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorComponentViewer)
{
	PROTECTED_FIELD(mTargetComponents);
	PROTECTED_FIELD(mSpoiler);
	PROTECTED_FIELD(mRemoveButton);
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
	PUBLIC_FUNCTION(void, Rebuild);
	PUBLIC_FUNCTION(bool, IsBuiltWithEmpty);
	PROTECTED_FUNCTION(void, RemoveTargetComponents);
}
END_META;
