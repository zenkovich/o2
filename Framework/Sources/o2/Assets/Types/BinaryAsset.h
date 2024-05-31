#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetRef.h"

namespace o2
{
    // -------------------
    // Binary format asset
    // -------------------
    class BinaryAsset: public AssetWithDefaultMeta<BinaryAsset>
    {
    public:
        PROPERTIES(BinaryAsset);
        GETTER(char*, data, GetData);        // Data getter
        GETTER(UInt, dataSize, GetDataSize); // Data size getter

    public:
        // Default constructor
        BinaryAsset();

        // Copy-constructor
        BinaryAsset(const BinaryAsset& asset);

        // Destructor
        ~BinaryAsset();

        // Check equals operator
        BinaryAsset& operator=(const BinaryAsset& asset);

        // Returns data pointer
        char* GetData() const;

        // Returns data size
        UInt GetDataSize() const;

        // Sets data and size
        void SetData(char* data, UInt size);

        // Returns extensions string
        static Vector<String> GetFileExtensions();

        // Returns editor sorting weight
        static int GetEditorSorting() { return 96; }

        SERIALIZABLE(BinaryAsset);
        CLONEABLE_REF(BinaryAsset);

    protected:
        char* mData = nullptr; // Asset data
        UInt  mDataSize = 0;   // Asset data size

    protected:
        // Loads asset data, using DataValue and serialization
        void LoadData(const String& path) override;

        // Saves asset data, using DataValue and serialization
        void SaveData(const String& path) const override;

        friend class Assets;
    };
}
// --- META ---

CLASS_BASES_META(o2::BinaryAsset)
{
    BASE_CLASS(o2::AssetWithDefaultMeta<BinaryAsset>);
}
END_META;
CLASS_FIELDS_META(o2::BinaryAsset)
{
    FIELD().PUBLIC().NAME(data);
    FIELD().PUBLIC().NAME(dataSize);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mData);
    FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mDataSize);
}
END_META;
CLASS_METHODS_META(o2::BinaryAsset)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const BinaryAsset&);
    FUNCTION().PUBLIC().SIGNATURE(char*, GetData);
    FUNCTION().PUBLIC().SIGNATURE(UInt, GetDataSize);
    FUNCTION().PUBLIC().SIGNATURE(void, SetData, char*, UInt);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Vector<String>, GetFileExtensions);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(int, GetEditorSorting);
    FUNCTION().PROTECTED().SIGNATURE(void, LoadData, const String&);
    FUNCTION().PROTECTED().SIGNATURE(void, SaveData, const String&);
}
END_META;
// --- END META ---
