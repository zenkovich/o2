#include "IAssetPropertiesViewer.h"

#include "Utils/Reflection/Reflection.h"
#include "UI/Widget.h"

CLASS_META(Editor::IAssetPropertiesViewer)
{
	BASE_CLASS(o2::IObject);


	PUBLIC_FUNCTION(void, SetTargetAssets, const Vector<AssetRef*>&);
	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
}
END_META;
