#include "o2/stdafx.h"
#include "AssetRef.h"

#include "o2/Assets/Assets.h"

namespace o2
{
    AssetRef::AssetRef()
    {}

    AssetRef::AssetRef(const AssetRef& other) :
        mAssetPtr(other.mAssetPtr), mRefCounter(other.mRefCounter), mIsInstance(other.mIsInstance)
    {
        if (mAssetPtr)
            (*mRefCounter)++;
    }

    AssetRef::AssetRef(const String& path):
        AssetRef(o2Assets.GetAssetRef(path))
    {}

    AssetRef::AssetRef(const UID& id):
        AssetRef(o2Assets.GetAssetRef(id))
    {}

    AssetRef::AssetRef(Asset* assetPtr, int* refCounter):
        mAssetPtr(assetPtr), mRefCounter(refCounter)
    {
        if (mAssetPtr)
            (*mRefCounter)++;
    }

    AssetRef::AssetRef(Asset* instance):
        AssetRef(instance ? o2Assets.GetAssetRef(instance->GetUID()) : AssetRef())
    {
        mIsInstance = true;
    }

    void AssetRef::OnSerialize(DataValue& node) const
    {
        if (mIsInstance)
        {
            if (mAssetPtr)
            {
                AssetRef* nonConstThis = const_cast<AssetRef*>(this);
                *nonConstThis = AssetRef(mAssetPtr->CloneAs<Asset>());

                node["instance"] = mAssetPtr;
                node["meta"] = mAssetPtr->GetMeta();
            }
        }
        else if (mAssetPtr)
        {
            node["id"] = mAssetPtr->GetUID().ToString();
            node["path"] = mAssetPtr->GetPath();
        }
    }

    void AssetRef::OnDeserialized(const DataValue& node)
    {
        if (mAssetPtr)
            (*mRefCounter)--;

        mAssetPtr = nullptr;
        mRefCounter = nullptr;
        mIsInstance = false;

        if (auto instanceNode = node.FindMember("instance"))
        {
            mIsInstance = true;
            mAssetPtr = (Asset*)(*instanceNode);

            UID oldUid = mAssetPtr->GetUID();
            mAssetPtr->mInfo.meta = node.GetMember("meta");
            auto assetCache = o2Assets.UpdateAssetCache(mAssetPtr, "", oldUid);
            Assert(assetCache, "Can't find asset cache");
            if (assetCache)
            {
                mRefCounter = &assetCache->referencesCount;
                (*mRefCounter)++;
            }
            else
                mAssetPtr = nullptr;

            UpdateSpecAsset();
        }
        else if (auto idNode = node.FindMember("id"))
        {
            *this = o2Assets.GetAssetRef((UID)(*idNode));
            UpdateSpecAsset();
        }
        else if (auto pathNode = node.FindMember("path"))
        {
            *this = o2Assets.GetAssetRef((String)pathNode);
            UpdateSpecAsset();
        }
    }

    void AssetRef::OnSerializeDelta(DataValue& node, const IObject& origin) const
    {
        OnSerialize(node);
    }

    void AssetRef::OnDeserializedDelta(const DataValue& node, const IObject& origin)
    {
        OnDeserialized(node);
    }

    bool AssetRef::IsDeltaAsSingleObject()
    {
        return true;
    }

    AssetRef::~AssetRef()
    {
        if (mAssetPtr)
            (*mRefCounter)--;
    }

    AssetRef& AssetRef::operator=(const AssetRef& other)
    {
        if (mAssetPtr)
            (*mRefCounter)--;

        mAssetPtr = other.mAssetPtr;
        mRefCounter = other.mRefCounter;
        mIsInstance = other.mIsInstance;
        UpdateSpecAsset();

        if (mAssetPtr)
            (*mRefCounter)++;

        return *this;
    }

    AssetRef::operator bool() const
    {
        return mAssetPtr != nullptr;
    }

    Asset& AssetRef::operator*()
    {
        return *mAssetPtr;
    }

    const Asset& AssetRef::operator*() const
    {
        return *mAssetPtr;
    }

    Asset* AssetRef::operator->()
    {
        return mAssetPtr;
    }

    const Asset* AssetRef::operator->() const
    {
        return mAssetPtr;
    }

    bool AssetRef::operator<(const AssetRef& other) const
    {
        return mAssetPtr < other.mAssetPtr;
    }

    bool AssetRef::IsValid() const
    {
        return mAssetPtr != nullptr;
    }

    Asset* AssetRef::Get()
    {
        return mAssetPtr;
    }

    const Asset* AssetRef::Get() const
    {
        return mAssetPtr;
    }

    const Type& AssetRef::GetAssetType() const
    {
        if (mAssetPtr)
            return mAssetPtr->GetType();

        return TypeOf(Asset);
    }

    void AssetRef::SetInstance(Asset* asset)
    {
        if (mIsInstance)
            return;

        if (mAssetPtr)
            *mRefCounter--;

        mAssetPtr = asset;

        auto assetCache = o2Assets.FindAssetCache(mAssetPtr->GetUID());
        Assert(assetCache, "Can't find asset cache");
        if (assetCache)
        {
            mRefCounter = &assetCache->referencesCount;
            (*mRefCounter)++;
        }
        else
            mAssetPtr = nullptr;

        UpdateSpecAsset();

        mIsInstance = true;
    }

    void AssetRef::CreateInstance()
    {
        if (mIsInstance)
            return;

        if (mAssetPtr)
            *mRefCounter--;

        if (mAssetPtr)
            mAssetPtr = mAssetPtr->CloneAs<Asset>();
        else
        {
            auto objectType = dynamic_cast<const ObjectType*>(&GetAssetType());
            mAssetPtr = dynamic_cast<Asset*>(objectType->DynamicCastToIObject(objectType->CreateSample()));
        }

        auto assetCache = o2Assets.FindAssetCache(mAssetPtr->GetUID());
        Assert(assetCache, "Can't find asset cache");
        if (assetCache)
        {
            mRefCounter = &assetCache->referencesCount;
            (*mRefCounter)++;
        }
        else
            mAssetPtr = nullptr;

        UpdateSpecAsset();

        mIsInstance = true;
    }

    void AssetRef::RemoveInstance()
    {
        if (!mIsInstance)
            return;

        *mRefCounter--;

        mIsInstance = false;
        mAssetPtr = nullptr;

        UpdateSpecAsset();
    }

    void AssetRef::SaveInstance(const String& path)
    {
        if (!mIsInstance)
            return;

        mAssetPtr->SetPath(path);
        mAssetPtr->Save();

        *this = AssetRef(path);
    }

    bool AssetRef::IsInstance() const
    {
        return mIsInstance;
    }

    bool AssetRef::operator!=(const AssetRef& other) const
    {
        return mAssetPtr != other.mAssetPtr;
    }

    bool AssetRef::operator==(const AssetRef& other) const
    {
        return mAssetPtr == other.mAssetPtr;
    }

}
// --- META ---

DECLARE_CLASS(o2::AssetRef, o2__AssetRef);
// --- END META ---
