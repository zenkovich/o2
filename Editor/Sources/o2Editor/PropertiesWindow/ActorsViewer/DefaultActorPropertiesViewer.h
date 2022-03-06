#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"
#include "o2Editor/PropertiesWindow/ActorsViewer/IActorPropertiesViewer.h"

namespace Editor
{
	class IObjectPropertiesViewer;

	// -------------------------------
	// Default actor properties viewer
	// -------------------------------
	class DefaultActorPropertiesViewer: public IActorPropertiesViewer
	{
	public:
		// Default constructor. Initializes data widget
		DefaultActorPropertiesViewer();

		// Virtual destructor
		~DefaultActorPropertiesViewer();

		// Sets target actors
		void SetTargetActors(const Vector<Actor*>& actors) override;

		// Returns viewing actor type 
		const Type* GetActorType() const override;

		// Specialize viewing actor type. Creates all using properties
		void SpecializeActorType(const Type* type);

		// Updates all actor values
		void Refresh() override;

		// Returns is there no properties
		bool IsEmpty() const override;

		IOBJECT(DefaultActorPropertiesViewer);

	protected:
		Vector<Actor*> mTargetActors;        // Target actors
		const Type*    mActorType = nullptr; // Target actor type

		IObjectPropertiesViewer* mViewer = nullptr; // Actor properties viewer

	protected:
		// It is called when some property changed, marks Actor as changed and calls default Undo create callback
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& before,
							   const Vector<DataDocument>& after);

		// Enable viewer event function
		void OnEnabled() override;

		// Disable viewer event function
		void OnDisabled() override;
	};
}

CLASS_BASES_META(Editor::DefaultActorPropertiesViewer)
{
	BASE_CLASS(Editor::IActorPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultActorPropertiesViewer)
{
	FIELD().PROTECTED().NAME(mTargetActors);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mActorType);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mViewer);
}
END_META;
CLASS_METHODS_META(Editor::DefaultActorPropertiesViewer)
{

	FUNCTION().PUBLIC().SIGNATURE(void, SetTargetActors, const Vector<Actor*>&);
	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetActorType);
	FUNCTION().PUBLIC().SIGNATURE(void, SpecializeActorType, const Type*);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEmpty);
	FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChanged, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
}
END_META;
