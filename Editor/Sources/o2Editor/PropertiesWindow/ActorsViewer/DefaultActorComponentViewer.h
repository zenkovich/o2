#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorComponentViewer.h"

namespace Editor
{
	class IObjectPropertiesViewer;

	// ------------------------------
	// Default actor component viewer
	// ------------------------------
	class DefaultActorComponentViewer: public IActorComponentViewer
	{
	public:
		// Default constructor. Initializes data widget
		DefaultActorComponentViewer();

		// Virtual destructor
		~DefaultActorComponentViewer();

		// Sets target actors
		void SetTargetComponents(const Vector<Component*>& components) override;

		// Returns viewing component type 
		const Type* GetComponentType() const override;

		// Updates all component values
		void Refresh() override;

		IOBJECT(DefaultActorComponentViewer);

	protected:
		const Type*              mComponentType = nullptr; // Target component type
		IObjectPropertiesViewer* mViewer = nullptr;        //Component properties viewer

	protected:
		// It is called when some property changed, marks Actor as changed and calls default Undo create callback
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& before, 
							   const Vector<DataDocument>& after);

		// Enable viewer event function
		void OnEnabled() override;

		// Disable viewer event function
		void OnDisabled() override;
	};

	template<typename _component_type>
	class TActorComponentViewer: public DefaultActorComponentViewer
	{
	public:
		// Sets target actors
		void SetTargetComponents(const Vector<Component*>& components) override;

		// Returns viewing component type 
		const Type* GetComponentType() const override;

		IOBJECT(TActorComponentViewer<_component_type>);

	protected:
		Vector<_component_type*> mTargetComponents;
	};

	template<typename _component_type>
	void TActorComponentViewer<_component_type>::SetTargetComponents(const Vector<Component*>& components)
	{
		DefaultActorComponentViewer::SetTargetComponents(components);
		mTargetComponents = components.DynamicCast<_component_type*>();
	}

	template<typename _component_type>
	const Type* TActorComponentViewer<_component_type>::GetComponentType() const
	{
		return &TypeOf(_component_type);
	}
}

CLASS_BASES_META(Editor::DefaultActorComponentViewer)
{
	BASE_CLASS(Editor::IActorComponentViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultActorComponentViewer)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mComponentType).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mViewer).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::DefaultActorComponentViewer)
{

	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
	PUBLIC_FUNCTION(const Type*, GetComponentType);
	PUBLIC_FUNCTION(void, Refresh);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
}
END_META;

META_TEMPLATES(typename _component_type)
CLASS_BASES_META(Editor::TActorComponentViewer<_component_type>)
{
	BASE_CLASS(Editor::DefaultActorComponentViewer);
}
END_META;
META_TEMPLATES(typename _component_type)
CLASS_FIELDS_META(Editor::TActorComponentViewer<_component_type>)
{
	FIELD().NAME(mTargetComponents).PROTECTED();
}
END_META;
META_TEMPLATES(typename _component_type)
CLASS_METHODS_META(Editor::TActorComponentViewer<_component_type>)
{

	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
	PUBLIC_FUNCTION(const Type*, GetComponentType);
}
END_META;
