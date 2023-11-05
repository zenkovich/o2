#pragma once
#include "o2/Assets/Meta.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/System/Time/TimeStamp.h"

namespace o2
{
    class AssetsTree;

    // -----------------
    // Asset information
    // -----------------
    struct AssetInfo : public ISerializable
    {
        const AssetsTree* tree = nullptr; // Owner asset tree
        
        String    path;     // Path of asset @SERIALIZABLE
        TimeStamp editTime; // Asset edited time @SERIALIZABLE        

        AssetMeta* meta = nullptr; // Asset meta data @SERIALIZABLE

        AssetInfo* parent = nullptr; // Parent asset info

    public:
        // Default constructor
        AssetInfo();

        // Default constructor with meta
        AssetInfo(AssetMeta* meta);

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

        // Removes all children
        void RemoveAllChildren();

        // Sets parent node
        void SetParent(AssetInfo* parent);

        // Sets asset tree, adds in tree's allAssets map
        void SetTree(AssetsTree* tree);

        // Returns children list. if it doesn't own children, then looks for the same in the tree and returns its children
        const Vector<AssetInfo*>& GetChildren() const;

        // Returns is asset valid - checks id for empty
        bool IsValid() const;

        static AssetInfo empty;

        SERIALIZABLE(AssetInfo);

    protected:
        Vector<AssetInfo*> mChildren;           // Children assets infos @SERIALIZABLE
        bool               mOwnChildren = true; // Is children assets is owned by this asset info

    protected:
        // Completion deserialization callback, reads asset type name
        void OnDeserialized(const DataValue& node) override;

        friend class Asset;
        friend class Assets;
        friend class AssetsBuilder;
        friend class AssetsTree;
    };
}
// --- META ---

CLASS_BASES_META(o2::AssetInfo)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetInfo)
{
    FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(tree);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(path);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(editTime);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(meta);
    FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(parent);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mChildren);
    FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mOwnChildren);
}
END_META;
CLASS_METHODS_META(o2::AssetInfo)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(AssetMeta*);
    FUNCTION().PUBLIC().CONSTRUCTOR(const AssetInfo&);
    FUNCTION().PUBLIC().SIGNATURE(AssetInfo*, AddChild, AssetInfo*);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveChild, AssetInfo*, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllChildren);
    FUNCTION().PUBLIC().SIGNATURE(void, SetParent, AssetInfo*);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTree, AssetsTree*);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<AssetInfo*>&, GetChildren);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsValid);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
