#pragma once

#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"

namespace Editor
{
	// ------------------------------------------------------------
	// Default object properties viewer. Builds field by reflection
	// ------------------------------------------------------------
	class DefaultObjectPropertiesViewer: public IObjectPropertiesViewer
	{
	public:
		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		IOBJECT(DefaultObjectPropertiesViewer);

	protected:
		const Type* mRealObjectType = &TypeOf(IObject);  // Specialized viewing object type
		const Type* mBuiltObjectType = &TypeOf(IObject); // Type of built object properties
		bool        mBuiltWithHiddenProperties = false;  // Is properties was built with hidden properties section

	protected:
		// Checks if properties need to be rebuilt, rebuilds if necessary
		bool CheckBuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// It is called when viewer is refreshed, stores real object type
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;
	};
}

CLASS_BASES_META(Editor::DefaultObjectPropertiesViewer)
{
	BASE_CLASS(Editor::IObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::DefaultObjectPropertiesViewer)
{
	FIELD().PROTECTED().DEFAULT_VALUE(&TypeOf(IObject)).NAME(mRealObjectType);
	FIELD().PROTECTED().DEFAULT_VALUE(&TypeOf(IObject)).NAME(mBuiltObjectType);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mBuiltWithHiddenProperties);
}
END_META;
CLASS_METHODS_META(Editor::DefaultObjectPropertiesViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;
	typedef const Vector<Pair<IObject*, IObject*>>& _tmp2;

	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
	FUNCTION().PROTECTED().SIGNATURE(bool, CheckBuildProperties, _tmp1);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRefreshed, _tmp2);
}
END_META;
