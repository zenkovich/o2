#pragma once

#include "Assets/Asset.h"
#include "Assets/Builder/StdAssetConverter.h"
#include "Utils/String.h"

namespace o2
{
	class FolderInfo;
	class IAssetConverter;

	class AssetsBuilder
	{
	public:
		AssetsBuilder();
		~AssetsBuilder();

		void BuildAssets(const String& assetsPath, const String& dataAssetsPath, bool forcible = false);

	protected:
		struct AssetInfo
		{
			typedef Vector<Ptr<AssetInfo>> AssetsInfosVec;

			Type::Id              mType;
			String                mPath;
			TimeStamp             mTime;
			Ptr<Asset::IMetaInfo> mMeta;
			AssetsInfosVec        mChilds;
			Ptr<AssetInfo>        mParent;

			bool operator==(const AssetInfo& other) const;
		};
		typedef Vector<Ptr<AssetInfo>> AssetsInfosVec;

	protected:
		typedef Dictionary<Type::Id, Ptr<IAssetConverter>> ConvertersDict;

		Ptr<LogStream>    mLog;

		String            mSourceAssetsPath;
		AssetInfo         mSourceAssetsFolder;
		AssetsInfosVec    mSourceAssetsInfos;

		String            mBuildedAssetsPath;
		AssetInfo         mBuildedAssetsFolder;
		AssetsInfosVec    mBuildedAssetsInfos;

		ConvertersDict    mAssetConverters;
		StdAssetConverter mStdAssetConverter;

	protected:
		void InitializeConverters();

		void RemoveBuildedAssets();

		void CreateMissingMetas();

		void LoadSourceAssetsInfos();

		void LoadBuildedAssetsInfos();

		void ProcessRemovedAssets();

		void ProcessModifiedAssets();

		void ProcessNewAssets();

		void ProcessMissingMetasCreation(FolderInfo& folder);

		void ProcessAssetsInfosLoading(FolderInfo& folder, Ptr<AssetInfo> parentFolderInfo, AssetsInfosVec& infosVec);

		void GenerateMeta(const Type* assetType, const String& metaFullPath);
		Ptr<AssetInfo> LoadAssetInfo(const String& path, const TimeStamp& time, Ptr<AssetInfo> parent);

		Ptr<IAssetConverter> GetAssetConverter(Type::Id assetTypeId);

		void Reset();
	};
}
