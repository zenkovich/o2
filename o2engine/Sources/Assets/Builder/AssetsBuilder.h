#pragma once

#include "Assets/Asset.h"
#include "Utils/String.h"

namespace o2
{
	class FolderInfo;

	class AssetsBuilder
	{
	public:
		AssetsBuilder();
		~AssetsBuilder();

		void BuildAssets(const String& assetsPath, const String& dataAssetsPath);

	protected:
		struct AssetInfo
		{
			TypeId                mType;
			String                mPath;
			UInt                  mId;
			TimeStamp             mTime;
			Ptr<Asset::IMetaInfo> mMeta;

			bool operator==(const AssetInfo& other) const;
		};
		typedef Vector<AssetInfo> AssetsInfosVec;

	protected:
		Ptr<LogStream> mLog;
		String         mSourceAssetsPath;
		String         mBuildedAssetsPath;
		AssetsInfosVec mSourceAssetsInfos;
		AssetsInfosVec mBuildedAssetsInfos;

	protected:
		void CreateMissingMetas();

		void LoadSourceAssetsInfos();

		void LoadBuildedAssetsInfos();

		void ProcessMovedAssets();

		void ProcessRemovedAssets();

		void ProcessModifiedAssets();

		void ProcessNewAssets();

		void ProcessMissingMetasCreation(FolderInfo& folder);
		void ProcessSourceAssetsInfodLoading(FolderInfo& folder);
	};
}
