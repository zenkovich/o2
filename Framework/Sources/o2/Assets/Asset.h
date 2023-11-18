#pragma once

#include "o2/Assets/AssetInfo.h"
#include "o2/Assets/Meta.h"
#include "o2/Utils/Editor/Attributes/DontDeleteAttribute.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"
#include "o2/Utils/Editor/Attributes/ExpandedByDefaultAttribute.h"
#include "o2/Utils/Editor/Attributes/NoHeaderAttribute.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
    // -------------------------------------------------------------------------------------------------
    // Basic asset interface. Contains copy of asset, without caching. For regular use assets references
    // -------------------------------------------------------------------------------------------------
    class Asset : public ISerializable
    {
    public:
        typedef AssetMeta MetaType;

    public:
        PROPERTIES(Asset);
        PROPERTY(String, path, SetPath, GetPath); // Asset path property @EDITOR_IGNORE
        GETTER(String, fullPath, GetFullPath);    // Full asset path getter (from binary path)
        GETTER(UID, id, GetUID);                  // Asset id getter
        GETTER(AssetMeta*, meta, GetMeta);        // Asset meta information pointer getter

    public:
        // Virtual destructor
        virtual ~Asset();

        // Assign operator
        Asset& operator=(const Asset& asset);

        // Returns path @SCRIPTABLE
        const String& GetPath() const;

        // Sets path @SCRIPTABLE
        void SetPath(const String& path);

        // Returns full asset path (from binary path) @SCRIPTABLE
        String GetFullPath() const;

        // Returns full built asset path (from binary) @SCRIPTABLE
        String GetBuiltFullPath() const;

        // Returns id of asset
        const UID& GetUID() const;

        // Returns meta information pointer
        AssetMeta* GetMeta() const;

        // Returns asset info
        const AssetInfo& GetInfo() const;

#if IS_EDITOR
        // Does the asset for the editor or not
        void SetEditorAsset(bool isEditor);

        // Returns is asset for editor or not
        bool IsEditorAsset() const;
#endif

        // Loads asset from path
        void Load(const String& path);

        // Loads asset by id
        void Load(const UID& id);

        // Reloads asset
        void Reload();

        // Saves asset by path
        void Save(const String& path);

        // Saves asset
        void Save();

        // Sets asset dirty
        void SetDirty(bool dirty = true);

        // Returns is asset dirty
        bool IsDirty() const;

        // Returns extensions string (something like "ext1 ext2 ent asf")
        static Vector<String> GetFileExtensions();

        // Returns editor icon
        static String GetEditorIcon() { return "ui/UI4_big_file_icon.png"; }

        // Returns editor sorting weight
        static int GetEditorSorting() { return 1; }

        // Is this asset type is available to create from editor's assets window
        static bool IsAvailableToCreateFromEditor() { return false; }

        // Is asset reference available to contain instance inside
        static bool IsReferenceCanOwnInstance() { return false; }

        SERIALIZABLE(Asset);

    protected:
        PROPERTY(AssetMeta*, mMeta, SetMeta, GetMeta); // @EDITOR_PROPERTY @DONT_DELETE @EXPANDED_BY_DEFAULT

        AssetInfo mInfo; // Asset info 

        bool mDirty = false; // Is asset was changed

    private:
        // Hidden default constructor
        Asset();

    protected:
        // Constructor with meta, use it as default constructor
        Asset(AssetMeta* meta);

        // Copy-constructor
        Asset(const Asset& asset);

        // Returns meta full path (from binary path)
        String GetMetaFullPath() const;

        // Returns reference to id in meta
        UID& ID();

        // Returns assets log stream pointer
        const Ref<LogStream>& GetAssetsLogStream() const;

        // Meta setter, used for property
        void SetMeta(AssetMeta* meta);

        // Loads asset from path
        void Load(const AssetInfo& info);

        // Loads asset data, using DataValue and serialization
        virtual void LoadData(const String& path);

        // Saves asset data, using DataValue and serialization
        virtual void SaveData(const String& path) const;

        // It is called when UID has changed
        virtual void OnUIDChanged(const UID& oldUID);

        friend class AssetRef;
        friend class Assets;
        friend class AssetsBuilder;
    };

    // This macro defines asset type
#define ASSET_TYPE(THIS_TYPE, META_TYPE) \
SERIALIZABLE(THIS_TYPE);                 \
typedef META_TYPE MetaType;                                         

    // ----------------------------
    // Base asset with default meta
    // ----------------------------
    template<typename T>
    class AssetWithDefaultMeta: public Asset
    {
    public:
        typedef DefaultAssetMeta<T> Meta;
        typedef Meta MetaType;

    public:
        PROPERTIES(AssetWithDefaultMeta<T>);
        GETTER(Meta*, meta, GetMeta);  // Meta information getter

    public:
        AssetWithDefaultMeta(): Asset(mnew Meta()) {}
        AssetWithDefaultMeta(const AssetWithDefaultMeta<T>& other) : Asset(other), meta(this) {}
        Meta* GetMeta() const { return dynamic_cast<Meta*>(mInfo.meta); }

        SERIALIZABLE(AssetWithDefaultMeta<T>);
    };
}
// --- META ---

CLASS_BASES_META(o2::Asset)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Asset)
{
    FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(path);
    FIELD().PUBLIC().NAME(fullPath);
    FIELD().PUBLIC().NAME(id);
    FIELD().PUBLIC().NAME(meta);
    FIELD().PUBLIC().DONT_DELETE_ATTRIBUTE().EDITOR_PROPERTY_ATTRIBUTE().EXPANDED_BY_DEFAULT_ATTRIBUTE().NAME(mMeta);
    FIELD().PROTECTED().NAME(mInfo);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mDirty);
}
END_META;
CLASS_METHODS_META(o2::Asset)
{

    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const String&, GetPath);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetPath, const String&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(String, GetFullPath);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(String, GetBuiltFullPath);
    FUNCTION().PUBLIC().SIGNATURE(const UID&, GetUID);
    FUNCTION().PUBLIC().SIGNATURE(AssetMeta*, GetMeta);
    FUNCTION().PUBLIC().SIGNATURE(const AssetInfo&, GetInfo);
#if  IS_EDITOR
    FUNCTION().PUBLIC().SIGNATURE(void, SetEditorAsset, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEditorAsset);
#endif
    FUNCTION().PUBLIC().SIGNATURE(void, Load, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, Load, const UID&);
    FUNCTION().PUBLIC().SIGNATURE(void, Reload);
    FUNCTION().PUBLIC().SIGNATURE(void, Save, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, Save);
    FUNCTION().PUBLIC().SIGNATURE(void, SetDirty, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsDirty);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Vector<String>, GetFileExtensions);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetEditorIcon);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(int, GetEditorSorting);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, IsAvailableToCreateFromEditor);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, IsReferenceCanOwnInstance);
    FUNCTION().PRIVATE().CONSTRUCTOR();
    FUNCTION().PROTECTED().CONSTRUCTOR(AssetMeta*);
    FUNCTION().PROTECTED().CONSTRUCTOR(const Asset&);
    FUNCTION().PROTECTED().SIGNATURE(String, GetMetaFullPath);
    FUNCTION().PROTECTED().SIGNATURE(UID&, ID);
    FUNCTION().PROTECTED().SIGNATURE(const Ref<LogStream>&, GetAssetsLogStream);
    FUNCTION().PROTECTED().SIGNATURE(void, SetMeta, AssetMeta*);
    FUNCTION().PROTECTED().SIGNATURE(void, Load, const AssetInfo&);
    FUNCTION().PROTECTED().SIGNATURE(void, LoadData, const String&);
    FUNCTION().PROTECTED().SIGNATURE(void, SaveData, const String&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnUIDChanged, const UID&);
}
END_META;

META_TEMPLATES(typename T)
CLASS_BASES_META(o2::AssetWithDefaultMeta<T>)
{
    BASE_CLASS(o2::Asset);
}
END_META;
META_TEMPLATES(typename T)
CLASS_FIELDS_META(o2::AssetWithDefaultMeta<T>)
{
    FIELD().PUBLIC().NAME(meta);
}
END_META;
META_TEMPLATES(typename T)
CLASS_METHODS_META(o2::AssetWithDefaultMeta<T>)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const AssetWithDefaultMeta<T>&);
    FUNCTION().PUBLIC().SIGNATURE(Meta*, GetMeta);
}
END_META;
// --- END META ---
