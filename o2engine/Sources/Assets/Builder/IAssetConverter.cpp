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

	void IAssetConverter::AssetsPostProcess()
	{}

	void IAssetConverter::Reset()
	{}

	void IAssetConverter::SetAssetsBuilder(Ptr<AssetsBuilder> builder)
	{
		mAssetsBuilder = builder;
	}

}