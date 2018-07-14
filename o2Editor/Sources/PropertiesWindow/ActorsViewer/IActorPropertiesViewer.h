#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"
#include "Utils/Types/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Actor;
	class Component;
	class UIWidget;
}

namespace Editor
{
	class UISpoilerWithHead;

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
		virtual UIWidget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates all actor values
		virtual void Refresh();

		// Rebuilds properties layout
		virtual void Rebuild();

		// Returns is there no properties
		virtual bool IsEmpty() const;

		// Returns true when properties was built with hidden fields
		virtual bool IsBuiltWithHiddenFields() const;

		IOBJECT(IActorPropertiesViewer);

	protected:
		UISpoilerWithHead* mSpoiler = nullptr;
	};
}

CLASS_BASES_META(Editor::IActorPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorPropertiesViewer)
{
	PROTECTED_FIELD(mSpoiler);
}
END_META;
CLASS_METHODS_META(Editor::IActorPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(const Type*, GetActorType);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Rebuild);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PUBLIC_FUNCTION(bool, IsBuiltWithHiddenFields);
}
END_META;
