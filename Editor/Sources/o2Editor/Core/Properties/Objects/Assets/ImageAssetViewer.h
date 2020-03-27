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
	class BorderIProperty;
	class ImageSlicesEditorWidget;

	// ------------------
	// Image asset viewer
	// ------------------
	class ImageAssetViewer : public DefaultObjectPropertiesViewer
	{
	public:
		// Default constructor. Initializes fields controls
		ImageAssetViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(ImageAssetViewer);

	private:
		ImageSlicesEditorWidget* mSlicesEditor = nullptr;
	};
}

CLASS_BASES_META(Editor::ImageAssetViewer)
{
	BASE_CLASS(Editor::DefaultObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::ImageAssetViewer)
{
	PRIVATE_FIELD(mSlicesEditor);
}
END_META;
CLASS_METHODS_META(Editor::ImageAssetViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PUBLIC_FUNCTION(void, Refresh, _tmp1);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
}
END_META;
