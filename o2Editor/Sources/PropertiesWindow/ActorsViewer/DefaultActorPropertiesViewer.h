#pragma once

#include "Core/Properties/FieldPropertiesInfo.h"
#include "Core/Properties/IPropertyField.h"
#include "PropertiesWindow/ActorsViewer/IActorPropertiesViewer.h"

namespace Editor
{
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

		// Rebuilds properties layout
		void Rebuild() override;

		// Returns is there no properties
		bool IsEmpty() const override;

		// Returns true when properties was built with hidden fields
		bool IsBuiltWithEmpty() const override;

		IOBJECT(DefaultActorPropertiesViewer);

	protected:
		Vector<Actor*>      mTargetActors;        // Target actors
		FieldPropertiesInfo mFieldProperties;     // Field properties information
		bool                mBuiltWithHidden;     // True when properties was built with hidden fields
		const Type*         mActorType = nullptr; // Target actor type
	};
}

CLASS_BASES_META(Editor::DefaultActorPropertiesViewer)
{
	BASE_CLASS(Editor::IActorPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultActorPropertiesViewer)
{
	PROTECTED_FIELD(mTargetActors);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mBuiltWithHidden);
	PROTECTED_FIELD(mActorType);
}
END_META;
CLASS_METHODS_META(Editor::DefaultActorPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(const Type*, GetActorType);
	PUBLIC_FUNCTION(void, SpecializeActorType, const Type*);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Rebuild);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PUBLIC_FUNCTION(bool, IsBuiltWithEmpty);
}
END_META;
