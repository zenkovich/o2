#pragma once

#include "o2Editor/Core/Properties/Objects/DefaultObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Button;
	class WidgetDragHandle;
}

namespace Editor
{
	class AssetProperty;
	class BorderIProperty;
	class ImageSlicesEditorWidget;

	// ------------------
	// Image asset viewer
	// ------------------
	class ImageAssetViewer : public DefaultObjectPropertiesViewer
	{
	public:
		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(ImageAssetViewer);

	private:
		ImageSlicesEditorWidget* mSlicesEditor = nullptr;

		AssetProperty* mAtlasProperty = nullptr;

	private:
		// It is called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// It is called when viewer is refreshed
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;
	};
}

CLASS_BASES_META(Editor::ImageAssetViewer)
{
	BASE_CLASS(Editor::DefaultObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::ImageAssetViewer)
{
	PRIVATE_FIELD(mSlicesEditor).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mAtlasProperty).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(Editor::ImageAssetViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;
	typedef const Vector<Pair<IObject*, IObject*>>& _tmp2;

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
	PRIVATE_FUNCTION(void, RebuildProperties, _tmp1);
	PRIVATE_FUNCTION(void, OnRefreshed, _tmp2);
}
END_META;
