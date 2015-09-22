#include "Types.h"

#include "Utils/IObject.h"

#include "Assets/AtlasAsset.h"
#include "Assets/BinaryAsset.h"
#include "Assets/Builder/AtlasAssetConverter.h"
#include "Assets/Builder/FolderAssetConverter.h"
#include "Assets/Builder/IAssetConverter.h"
#include "Assets/Builder/ImageAssetConverter.h"
#include "Assets/Builder/StdAssetConverter.h"
#include "Assets/FolderAsset.h"
#include "Assets/ImageAsset.h"
#include "Config/ProjectConfig.h"
#include "Render/VectorFont.h"
#include "Render/VectorFontEffects.h"

namespace o2
{
	UInt Types::mLastGivenTypeId = 0;

	const Vector<Type*>& Types::GetTypes()
	{
		return mTypes;
	}

	IObject* Types::CreateTypeSample(const String& typeName)
	{
		for (auto type : mTypes)
		{
			if (type->Name() == typeName)
				return type->Sample()->Clone();
		}

		return nullptr;
	}

	Type* Types::GetType(Type::Id id)
	{
		for (auto type : mTypes)
		{
			if (type->ID() == id)
				return type;
		}

		return nullptr;
	}	
	
	Types::Registrator::Registrator()
	{
		Types::InitializeTypes();
	}

	void Types::InitializeTypes()
	{
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
	}

}