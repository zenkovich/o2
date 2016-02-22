#include "IAssetConverter.h"

namespace o2
{
	Vector<Type::Id> IAssetConverter::GetProcessingAssetsTypes() const
	{
		return Vector<Type::Id>();
	}

	void IAssetConverter::ConvertAsset(const AssetTree::AssetNode& node)
	{}

	void IAssetConverter::RemoveAsset(const AssetTree::AssetNode& node)
	{}

	void IAssetConverter::MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo)
	{}

	Vector<AssetId> IAssetConverter::AssetsPostProcess()
	{
		return Vector<AssetId>();
	}

	void IAssetConverter::Reset()
	{}

	void IAssetConverter::SetAssetsBuilder(AssetsBuilder* builder)
	{
		mAssetsBuilder = builder;
	}

}