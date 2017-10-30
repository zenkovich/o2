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

DECLARE_CLASS(Editor::AssetProperty<o2::ActorAssetRef>);
DECLARE_CLASS(Editor::AssetProperty<o2::AnimationAssetRef>);
DECLARE_CLASS(Editor::AssetProperty<o2::AtlasAssetRef>);
DECLARE_CLASS(Editor::AssetProperty<o2::BinaryAssetRef>);
DECLARE_CLASS(Editor::AssetProperty<o2::BitmapFontAssetRef>);
DECLARE_CLASS(Editor::AssetProperty<o2::DataAssetRef>);
DECLARE_CLASS(Editor::AssetProperty<o2::FolderAssetRef>);
DECLARE_CLASS(Editor::AssetProperty<o2::ImageAssetRef>);
DECLARE_CLASS(Editor::AssetProperty<o2::VectorFontAssetRef>);

CLASS_META(Editor::IAssetProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);


	PUBLIC_FUNCTION(void, SetAssetId, UID);
}
END_META;
