#pragma once

#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"

namespace Editor
{
	// ------------------------------------------------------------
	// Default object properties viewer. Builds field by reflection
	// ------------------------------------------------------------
	class DefaultObjectPropertiesViewer : public IObjectPropertiesViewer
	{
	public:
		// Default constructor, creates view widget as vertical layout
		DefaultObjectPropertiesViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		IOBJECT(DefaultObjectPropertiesViewer);

	private:
		const Type* mRealObjectType = &TypeOf(IObject); // Specialized viewing object type
		bool        mBuiltWithHiddenProperties = false; // Is properties was built with hidden properties section
	};
}

CLASS_BASES_META(Editor::DefaultObjectPropertiesViewer)
{
	BASE_CLASS(Editor::IObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultObjectPropertiesViewer)
{
	PRIVATE_FIELD(mRealObjectType);
	PRIVATE_FIELD(mBuiltWithHiddenProperties);
}
END_META;
CLASS_METHODS_META(Editor::DefaultObjectPropertiesViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PUBLIC_FUNCTION(void, Refresh, _tmp1);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
}
END_META;
