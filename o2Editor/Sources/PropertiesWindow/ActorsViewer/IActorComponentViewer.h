#pragma once

#include "Core/Properties/PropertiesContext.h"
#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Reflection/Type.h"
#include "Utils/Types/Containers/Vector.h"

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

		IOBJECT(IActorComponentViewer);

	protected:
		Vector<Component*> mTargetComponents; // Target components

		SpoilerWithHead* mSpoiler = nullptr;      // Component's spoiler
		Button*          mRemoveButton = nullptr; // Remove component button

	protected:
		// Removes target components
		void RemoveTargetComponents();
	};

	template<typename _component_type>
	class TActorComponentViewer : public IActorComponentViewer
	{
	public:
		// Sets target actors
		void SetTargetComponents(const Vector<Component*>& components) override;

		IOBJECT(TActorComponentViewer<_component_type>);

	protected:
		Vector<_component_type*> mTargetComponents;
	};

	template<typename _component_type>
	void TActorComponentViewer<_component_type>::SetTargetComponents(const Vector<Component*>& components)
	{
		IActorComponentViewer::SetTargetComponents(components);
		mTargetComponents = components.DynamicCast<_component_type*>();
	}

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
	PROTECTED_FUNCTION(void, RemoveTargetComponents);
}
END_META;

META_TEMPLATES(typename _component_type)
CLASS_BASES_META(Editor::TActorComponentViewer<_component_type>)
{
	BASE_CLASS(Editor::IActorComponentViewer);
}
END_META;
META_TEMPLATES(typename _component_type)
CLASS_FIELDS_META(Editor::TActorComponentViewer<_component_type>)
{
	PROTECTED_FIELD(mTargetComponents);
}
END_META;
META_TEMPLATES(typename _component_type)
CLASS_METHODS_META(Editor::TActorComponentViewer<_component_type>)
{

	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
}
END_META;
