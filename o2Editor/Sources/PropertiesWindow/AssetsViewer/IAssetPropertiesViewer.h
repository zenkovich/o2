#pragma once

#include "Assets/Asset.h"

using namespace o2;

namespace o2
{
	class Widget;
}

namespace Editor
{
	class IAssetPropertiesViewer: public IObject
	{
	public:
		// Default constructor
		IAssetPropertiesViewer() {}

		// Virtual destructor
		virtual ~IAssetPropertiesViewer() {}

		// Sets target actors
		virtual void SetTargetAssets(const Vector<AssetRef*>& assets) {}

		// Returns viewing asset type 
		virtual const Type* GetAssetType() const { return nullptr; }

		// Returns data widget
		virtual Widget* GetWidget() const { return nullptr; }

		IOBJECT(IAssetPropertiesViewer);
	};
}

CLASS_BASES_META(Editor::IAssetPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IAssetPropertiesViewer)
{
}
END_META;
CLASS_METHODS_META(Editor::IAssetPropertiesViewer)
{

	PUBLIC_FUNCTION(void, SetTargetAssets, const Vector<AssetRef*>&);
	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(Widget*, GetWidget);
}
END_META;
