#pragma once

#include "o2Editor/PropertiesWindow/DefaultPropertiesViewer.h"

using namespace o2;

namespace Editor
{
	class DefaultObjectPropertiesViewer;

	// -------------------------
	// Default properties viewer
	// -------------------------
	class AssetPropertiesViewer : public DefaultPropertiesViewer
	{
	public:
		// Returns viewing object type
		const Type* GetViewingObjectType() const override;

		IOBJECT(AssetPropertiesViewer);

	protected:
		// Sets target objects
		void SetTargets(const Vector<IObject*>& targets) override;
	};

}

CLASS_BASES_META(Editor::AssetPropertiesViewer)
{
	BASE_CLASS(Editor::DefaultPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::AssetPropertiesViewer)
{
}
END_META;
CLASS_METHODS_META(Editor::AssetPropertiesViewer)
{

	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
	FUNCTION().PROTECTED().SIGNATURE(void, SetTargets, const Vector<IObject*>&);
}
END_META;
