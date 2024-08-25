#pragma once
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/System/Time/TimeStamp.h"
#include "o2/Utils/Basic/ICloneable.h"

namespace o2
{
    FORWARD_CLASS_REF(AssetsTree);
    FORWARD_CLASS_REF(AssetMeta);

    // -----------------
    // Asset information
    // -----------------
    struct AssetInfo : public ISerializable, public RefCounterable, public ICloneableRef
    {
    public:
        static AssetInfo empty; // Empty asset info

    public:
        WeakRef<AssetsTree> tree; // Owner asset tree
        
        String    path;     // Path of asset @SERIALIZABLE
        TimeStamp editTime; // Asset edited time @SERIALIZABLE        

        Ref<AssetMeta> meta; // Asset meta data @SERIALIZABLE

        WeakRef<AssetInfo> parent; // Parent asset info

    public:
        // Default constructor
        AssetInfo();

        // Default constructor with meta
        AssetInfo(const Ref<AssetMeta>& meta);

        // Copy-constructor
        AssetInfo(const AssetInfo& other);

        // Destructor
        ~AssetInfo();

        // Copy-operator
        AssetInfo& operator=(const AssetInfo& other);

        // Check equal operator
        bool operator==(const AssetInfo& other) const;

        // Returns is asset info valid - checks id for empty
        explicit operator bool() const;

        // Adds new child node and returns him
        Ref<AssetInfo> AddChild(const Ref<AssetInfo>& node);

        // Remove child node and releases him if needs
        void RemoveChild(const Ref<AssetInfo>& node);

        // Removes all children
        void RemoveAllChildren();

        // Sets parent node
        void SetParent(const Ref<AssetInfo>& parent);

        // Sets asset tree, adds in tree's allAssets map
        void SetTree(const Ref<AssetsTree>& tree);

        // Returns children list. if it doesn't own children, then looks for the same in the tree and returns its children
        const Vector<Ref<AssetInfo>>& GetChildren() const;

        // Returns is asset valid - checks id for empty
        bool IsValid() const;

        SERIALIZABLE(AssetInfo);
        CLONEABLE_REF(AssetInfo);

    protected:
        Vector<Ref<AssetInfo>> mChildren; // Children assets infos @SERIALIZABLE

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
    BASE_CLASS(o2::RefCounterable);
    BASE_CLASS(o2::ICloneableRef);
}
END_META;
CLASS_FIELDS_META(o2::AssetInfo)
{
    FIELD().PUBLIC().NAME(tree);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(path);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(editTime);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(meta);
    FIELD().PUBLIC().NAME(parent);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mChildren);
}
END_META;
CLASS_METHODS_META(o2::AssetInfo)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<AssetMeta>&);
    FUNCTION().PUBLIC().CONSTRUCTOR(const AssetInfo&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<AssetInfo>, AddChild, const Ref<AssetInfo>&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveChild, const Ref<AssetInfo>&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllChildren);
    FUNCTION().PUBLIC().SIGNATURE(void, SetParent, const Ref<AssetInfo>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTree, const Ref<AssetsTree>&);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Ref<AssetInfo>>&, GetChildren);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsValid);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
