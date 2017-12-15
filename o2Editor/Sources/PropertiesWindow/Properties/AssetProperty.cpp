#include "AssetProperty.h"

#include "Assets/ActorAsset.h"
#include "Assets/AnimationAsset.h"
#include "Assets/AtlasAsset.h"
#include "Assets/BinaryAsset.h"
#include "Assets/BitmapFontAsset.h"
#include "Assets/DataAsset.h"
#include "Assets/FolderAsset.h"
#include "Assets/ImageAsset.h"
#include "Assets/VectorFontAsset.h"

REG_TYPE(Editor::AssetProperty<o2::ActorAssetRef>);
REG_TYPE(Editor::AssetProperty<o2::AnimationAssetRef>);
REG_TYPE(Editor::AssetProperty<o2::AtlasAssetRef>);
REG_TYPE(Editor::AssetProperty<o2::BinaryAssetRef>);
REG_TYPE(Editor::AssetProperty<o2::BitmapFontAssetRef>);
REG_TYPE(Editor::AssetProperty<o2::DataAssetRef>);
REG_TYPE(Editor::AssetProperty<o2::FolderAssetRef>);
REG_TYPE(Editor::AssetProperty<o2::ImageAssetRef>);
REG_TYPE(Editor::AssetProperty<o2::VectorFontAssetRef>);

CLASS_META(Editor::IAssetProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);


	PUBLIC_FUNCTION(void, SetAssetId, UID);
}
END_META;
