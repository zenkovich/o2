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

REG_TYPE(Editor::AssetProperty<o2::ActorAsset>);
REG_TYPE(Editor::AssetProperty<o2::AnimationAsset>);
REG_TYPE(Editor::AssetProperty<o2::AtlasAsset>);
REG_TYPE(Editor::AssetProperty<o2::BinaryAsset>);
REG_TYPE(Editor::AssetProperty<o2::BitmapFontAsset>);
REG_TYPE(Editor::AssetProperty<o2::DataAsset>);
REG_TYPE(Editor::AssetProperty<o2::FolderAsset>);
REG_TYPE(Editor::AssetProperty<o2::ImageAsset>);
REG_TYPE(Editor::AssetProperty<o2::VectorFontAsset>);

CLASS_META(Editor::IAssetProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::DrawableCursorEventsListener);
	BASE_CLASS(o2::KeyboardEventsListener);


	PUBLIC_FUNCTION(void, SetAssetId, UID);
}
END_META;
