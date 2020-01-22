#pragma once
#include "o2/Assets/Asset.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/System/Time/TimeStamp.h"

namespace o2
{
	class AssetsTree;

	// -----------------
	// Asset information
	// -----------------
	class AssetInfo : public ISerializable
	{
	public:
		// Copy-constructor
		AssetInfo(const AssetInfo& other);

		// Destructor
		~AssetInfo();

		// Copy-operator
		AssetInfo& operator=(const AssetInfo& other);

		// Check equal operator
		bool operator==(const AssetInfo& other) const;

		// Returns is asset info valid - checks id for empty
		operator bool() const;

		// Adds new child node and returns him
		AssetInfo* AddChild(AssetInfo* node);

		// Remove child node and releases him if needs
		void RemoveChild(AssetInfo* node, bool release = true);

		// Sets parent node
		void SetParent(AssetInfo* parent);

		// Sets asset tree, adds in tree's allAssets map
		void SetTree(AssetsTree* tree);

		// Returns is asset valid - checks id for empty
		bool IsValid() const;

		static AssetInfo empty;

		SERIALIZABLE(AssetInfo);

	private:
		AssetsTree* mTree = nullptr; // Owner asset tree

		const Type* mAssetType = nullptr; // Type of asset

		String    mPath;     // Path of asset @SERIALIZABLE
		TimeStamp mEditTime; // Asset edited time			

		Asset::IMetaInfo* mMeta = nullptr; // Asset meta data @SERIALIZABLE

		AssetInfo*         mParent = nullptr; // Parent asset info
		Vector<AssetInfo*> mChildren;         // Children assets infos @SERIALIZABLE

	private:
		// Default constructor
		AssetInfo();

		// Beginning serialization callback, writes asset type name
		void OnSerialize(DataNode& node) const override;

		// Completion deserialization callback, reads asset type name
		void OnDeserialized(const DataNode& node) override;

		friend class AssetsBuilder;
		friend class AssetsTree;
	};
}

CLASS_BASES_META(o2::AssetInfo)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetInfo)
{
	PRIVATE_FIELD(mTree);
	PRIVATE_FIELD(mAssetType);
	PRIVATE_FIELD(mPath).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mEditTime);
	PRIVATE_FIELD(mMeta).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mParent);
	PRIVATE_FIELD(mChildren).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AssetInfo)
{

	PUBLIC_FUNCTION(AssetInfo*, AddChild, AssetInfo*);
	PUBLIC_FUNCTION(void, RemoveChild, AssetInfo*, bool);
	PUBLIC_FUNCTION(void, SetParent, AssetInfo*);
	PUBLIC_FUNCTION(void, SetTree, AssetsTree*);
	PUBLIC_FUNCTION(bool, IsValid);
	PRIVATE_FUNCTION(void, OnSerialize, DataNode&);
	PRIVATE_FUNCTION(void, OnDeserialized, const DataNode&);
}
END_META;
