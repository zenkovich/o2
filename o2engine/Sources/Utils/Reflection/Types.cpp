#include "Types.h"

#include "Utils/IObject.h"

#include "Animation/Animatable.h"
#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedValue.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Animation/IAnimation.h"
#include "Assets/AtlasAsset.h"
#include "Assets/BinaryAsset.h"
#include "Assets/BitmapFontAsset.h"
#include "Assets/Builder/AtlasAssetConverter.h"
#include "Assets/Builder/FolderAssetConverter.h"
#include "Assets/Builder/IAssetConverter.h"
#include "Assets/Builder/ImageAssetConverter.h"
#include "Assets/Builder/StdAssetConverter.h"
#include "Assets/FolderAsset.h"
#include "Assets/ImageAsset.h"
#include "Assets/VectorFontAsset.h"
#include "Config/ProjectConfig.h"
#include "Render/RectDrawable.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Render/VectorFont.h"
#include "Render/VectorFontEffects.h"
#include "UI/Button.h"
#include "UI/CustomDropDown.h"
#include "UI/CustomList.h"
#include "UI/DropDown.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/HorizontalProgress.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/Label.h"
#include "UI/List.h"
#include "UI/ScrollArea.h"
#include "UI/Toggle.h"
#include "UI/VerticalLayout.h"
#include "UI/VerticalProgress.h"
#include "UI/VerticalScrollBar.h"
#include "UI/Widget.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "UI/Window.h"
#include "Utils/Math/Curve.h"
#include "Utils/Math/Layout.h"
#include "Utils/Math/Transform.h"

namespace o2
{
	const Vector<Ptr<Type>>& Types::GetTypes()
	{
		return instance->mTypes;
	}

	IObject* Types::CreateTypeSample(const String& typeName)
	{
		for (auto type : instance->mTypes)
		{
			if (type->Name() == typeName)
				return type->Sample()->Clone();
		}

		return nullptr;
	}

	Ptr<Type> Types::GetType(Type::Id id)
	{
		for (auto type : instance->mTypes)
		{
			if (type->ID() == id)
				return type;
		}

		return nullptr;
	}	

	void Types::InitializeTypes()
	{
		Type::Dummy::type = mnew Type();
		Type::Dummy::type.SetupAsRoot();

		INIT_TYPE(ProjectConfig);
		INIT_TYPE(AssetInfo);
		INIT_TYPE(Asset);
		INIT_TYPE(Asset::IMetaInfo);
		INIT_TYPE(ImageAsset);
		INIT_TYPE(ImageAsset::MetaInfo);
		INIT_TYPE(ImageAsset::PlatformMeta);
		INIT_TYPE(AtlasAsset);
		INIT_TYPE(AtlasAsset::MetaInfo);
		INIT_TYPE(AtlasAsset::PlatformMeta);
		INIT_TYPE(AtlasAsset::Page);
		INIT_TYPE(BinaryAsset);
		INIT_TYPE(BinaryAsset::MetaInfo);
		INIT_TYPE(TimeStamp);
		INIT_TYPE(FolderAsset);
		INIT_TYPE(FolderAsset::MetaInfo);
		INIT_TYPE(IAssetConverter);
		INIT_TYPE(StdAssetConverter);
		INIT_TYPE(FolderAssetConverter);
		INIT_TYPE(ImageAssetConverter);
		INIT_TYPE(AtlasAssetConverter);
		INIT_TYPE(AtlasAssetConverter::Image);
		INIT_TYPE(VectorFont::Effect);
		INIT_TYPE(FontColorEffect);
		INIT_TYPE(FontGradientEffect);
		INIT_TYPE(FontStrokeEffect);
		INIT_TYPE(FontShadowEffect);
		INIT_TYPE(Transform);
		INIT_TYPE(IAnimation);
		INIT_TYPE(IAnimatedValue);
		INIT_TYPE(AnimatedValue<float>);
		INIT_TYPE(AnimatedValue<Vec2F>);
		INIT_TYPE(AnimatedValue<Vec2F>::Key);
		INIT_TYPE(AnimatedValue<Color4>);
		INIT_TYPE(AnimatedValue<Color4>::Key);
		INIT_TYPE(AnimatedValue<RectF>);
		INIT_TYPE(AnimatedValue<RectF>::Key);
		INIT_TYPE(Curve);
		INIT_TYPE(Curve::Key);
		INIT_TYPE(Animation);
		INIT_TYPE(Animation::AnimatedValueDef);
		INIT_TYPE(IRectDrawable);
		INIT_TYPE(Sprite);
		INIT_TYPE(Layout);
		INIT_TYPE(BitmapFontAsset);
		INIT_TYPE(BitmapFontAsset::MetaInfo);
		INIT_TYPE(VectorFontAsset);
		INIT_TYPE(VectorFontAsset::MetaInfo);
		INIT_TYPE(Text);
		INIT_TYPE(UIWidgetLayer);
		INIT_TYPE(UIWidgetLayout);
		INIT_TYPE(UIWidgetState);
		INIT_TYPE(UIWidget);
		INIT_TYPE(UIButton);
		INIT_TYPE(UILabel);
		INIT_TYPE(UIHorizontalLayout);
		INIT_TYPE(UIVerticalLayout);
		INIT_TYPE(UIHorizontalProgress);
		INIT_TYPE(UIVerticalProgress);
		INIT_TYPE(UIHorizontalScrollBar);
		INIT_TYPE(UIVerticalScrollBar);
		INIT_TYPE(UIToggle);
		INIT_TYPE(UIScrollArea);
		INIT_TYPE(UIEditBox);
		INIT_TYPE(UICustomList);
		INIT_TYPE(UIList);
		INIT_TYPE(UICustomDropDown);
		INIT_TYPE(UIDropDown);
		INIT_TYPE(UIWindow);
		INIT_TYPE(Animatable);
	}
}