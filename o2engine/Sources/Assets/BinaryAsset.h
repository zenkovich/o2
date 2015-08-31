#pragma once

#include "Assets/Asset.h"

namespace o2
{
	class BinaryAsset: public Asset
	{
	public:
		class MetaInfo: public IMetaInfo
		{
		public:
			String mSomeMetaParam;

			TypeId GetAssetType() const;
			bool IsEqual(Ptr<IMetaInfo> other) const;

			SERIALIZABLE_IMPL(MetaInfo);

			static Type type;
			const Type& GetType() const { return type; };
			Type::Id GetTypeId() const { return type.ID(); };
			static void InitializeType(MetaInfo* sample)
			{
				SRLZ_FIELD(mSomeMetaParam);
			}
		};

	protected:
		char* mData;
		UInt  mDataSize;

	public:
		DEFINE_TYPE(BinaryAsset);

		Getter<char*>     Data;
		Getter<UInt>      DataSize;
		Getter<MetaInfo>  Meta;

		BinaryAsset();
		BinaryAsset(const String& path);
		BinaryAsset(UInt id);
		BinaryAsset(const BinaryAsset& asset);
		~BinaryAsset();

		BinaryAsset& operator=(const BinaryAsset& asset);

		char* GetData() const;
		UInt GetDataSize() const;
		void SetData(char* data, UInt size);

		MetaInfo GetMeta() const;

		Vector<String> GetExtensions() const;

	protected:
		void LoadData(const String& path);
		void SaveData(const String& path);

		void InitializeProperties();
	};
}