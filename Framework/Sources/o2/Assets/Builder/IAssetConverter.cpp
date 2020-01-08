#include "o2/stdafx.h"
#include "IAssetConverter.h"

#include "o2/Assets/Builder/AssetsBuilder.h"

namespace o2
{
	Vector<const Type*> IAssetConverter::GetProcessingAssetsTypes() const
	{
		return Vector<const Type*>();
	}

	void IAssetConverter::ConvertAsset(const AssetTree::AssetNode& node)
	{}

	void IAssetConverter::RemoveAsset(const AssetTree::AssetNode& node)
	{}

	void IAssetConverter::MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo)
	{}

	Vector<UID> IAssetConverter::AssetsPostProcess()
	{
		return Vector<UID>();
	}

	void IAssetConverter::Reset()
	{}

	void IAssetConverter::SetAssetsBuilder(AssetsBuilder* builder)
	{
		mAssetsBuilder = builder;
	}

}

DECLARE_CLASS(o2::IAssetConverter);
