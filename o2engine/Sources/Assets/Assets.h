#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/CustomRTTI.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Property.h"
#include "Utils/Serialization.h"
#include "Utils/Singleton.h"

namespace o2
{

#define  o2Assets Assets::Instance()

	class Asset;
	class LogStream;
	class AssetsBuilder;

	class Assets: public Singleton<Assets>
	{
		friend class Asset;

	public:
		struct AssetInfo: ISerializable
		{
			typedef Vector<AssetInfo> AssetInfosVec;

			TypeId        mType;
			String        mPath;
			UInt          mId;
			AssetInfosVec mChilds;

			AssetInfo();
			AssetInfo(const String& path, UInt id, TypeId type);

			bool operator==(const AssetInfo& other) const;

			AssetInfo FindAssetInfo(const String& path) const;
			AssetInfo FindAssetInfo(UInt id) const;

			SERIALIZABLE_IMPL(AssetInfo);

			IOBJECT(AssetInfo)
			{
				SRLZ_FIELD(mType);
				SRLZ_FIELD(mPath);
				SRLZ_FIELD(mId);
				SRLZ_FIELD(mChilds);
			}
		};
		typedef Vector<AssetInfo> AssetInfosVec;

	protected:
		AssetInfosVec      mAssetsInfos;
		Ptr<LogStream>     mLog;
		Ptr<AssetsBuilder> mAssetsBuilder;

	public:
		Getter<String> AssetsPath;

		Assets();
		~Assets();

		String GetAssetsPath() const;
		String GetDataPath() const;

		String GetAssetPath(UInt id) const;
		UInt GetAssetId(const String& path) const;

		template<typename _asset_type>
		Ptr<_asset_type> CreateAsset();

		template<typename _asset_type>
		Ptr<_asset_type> LoadAsset(const String& path);

		template<typename _asset_type>
		Ptr<_asset_type> LoadAsset(UInt id);

		bool IsAssetExist(const String& path) const;
		bool IsAssetExist(UInt id) const;

		bool RemoveAsset(Ptr<Asset> asset, bool rebuildAssets = true);
		bool RemoveAsset(const String& path, bool rebuildAssets = true);
		bool RemoveAsset(UInt id, bool rebuildAssets = true);

		bool MoveAsset(Ptr<Asset> asset, const String& newPath, bool rebuildAssets = true);
		bool MoveAsset(const String& path, const String& newPath, bool rebuildAssets = true);
		bool MoveAsset(UInt id, const String& newPath, bool rebuildAssets = true);

		void RebuildAssets();

	protected:
		void LoadAssetsInfos();
		void LoadAssetTypes();

		AssetInfo FindAssetInfo(const String& path) const;
		AssetInfo FindAssetInfo(UInt id) const;

		void InitializeProperties();
	};

	template<typename _asset_type>
	Ptr<_asset_type> Assets::CreateAsset()
	{
		return mnew _asset_type();
	}

	template<typename _asset_type>
	Ptr<_asset_type> Assets::LoadAsset(const String& path)
	{
		return mnew _asset_type(path);
	}

	template<typename _asset_type>
	Ptr<_asset_type> Assets::LoadAsset(UInt id)
	{
		return mnew _asset_type(id);
	}
}