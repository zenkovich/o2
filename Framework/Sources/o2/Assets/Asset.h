#pragma once

#include "o2/Assets/AssetInfo.h"
#include "o2/Assets/Meta.h"
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
		PROPERTY(String, path, SetPath, GetPath); // Asset path property
		GETTER(String, fullPath, GetFullPath);    // Full asset path getter (from binary path)
		GETTER(UID, id, GetUID);                  // Asset id getter
		GETTER(AssetMeta*, meta, GetMeta);        // Asset meta information pointer getter

	public:
		// Virtual destructor
		virtual ~Asset();

		// Assign operator
		Asset& operator=(const Asset& asset);

		// Returns path
		const String& GetPath() const;

		// Sets path
		void SetPath(const String& path);

		// Returns full asset path (from binary path)
		String GetFullPath() const;

		// Returns full built asset path (from binary)
		String GetDataFullPath() const;

		// Returns id of asset
		const UID& GetUID() const;

		// Returns meta information pointer
		AssetMeta* GetMeta() const;

		// Returns asset info
		const AssetInfo& GetInfo() const;

		// Loads asset from path
		void Load(const String& path);

		// Loads asset by id
		void Load(const UID& id);

		// Saves asset by path
		void Save(const String& path, bool rebuildAssetsImmediately = true);

		// Saves asset
		void Save(bool rebuildAssetsImmediately = true);

		// Returns extensions string (something like "ext1 ext2 ent asf")
		virtual const char* GetFileExtensions() const;

		SERIALIZABLE(Asset);

	protected:
		AssetInfo mInfo; // Asset info

	protected:
		// Default constructor
		Asset();

		// Copy-constructor
		Asset(const Asset& asset);

		// Returns meta full path (from binary path)
		String GetMetaFullPath() const;

		// Returns reference to id in meta
		UID& ID();

		// Returns assets log stream pointer
		LogStream* GetAssetsLogStream() const;

		// Loads asset data, using DataNode and serialization
		virtual void LoadData(const String& path);

		// Saves asset data, using DataNode and serialization
		virtual void SaveData(const String& path) const;

		// Returns new meta for specific asset type. Must be overridden in other asset types. Use ASSET_TYPE or DEFAULT_ASSET_TYPE macro for defining asset types
		virtual AssetMeta* CreateMeta() const;

		friend class AssetRef;
		friend class Assets;
		friend class AssetsBuilder;
	};

	// This macro defines asset type
#define ASSET_TYPE(THIS_TYPE, META_TYPE)                            \
SERIALIZABLE(THIS_TYPE);                                            \
typedef META_TYPE MetaType;                                         \
AssetMeta* CreateMeta() const override { return mnew META_TYPE(); }

	// ----------------------------
	// Base asset with default meta
	// ----------------------------
	template<typename T>
	class AssetWithDefaultMeta: public Asset
	{
	public:
		// ------------------
		// Default asset meta
		// ------------------
		struct Meta : public AssetMeta
		{
			// Returns asset type id
			const Type* GetAssetType() const override { return &TypeOf(T); }
		};

		typedef Meta MetaType;

	public:
		PROPERTIES(AssetWithDefaultMeta<T>);
		GETTER(Meta*, meta, GetMeta);  // Meta information getter

	public:
		AssetWithDefaultMeta() {}
		AssetWithDefaultMeta(const AssetWithDefaultMeta<T>& other) : Asset(other), meta(this) {}
		Meta* GetMeta() const { return dynamic_cast<Meta*>(mInfo.meta); }
		SERIALIZABLE(AssetWithDefaultMeta<T>);

	protected:
		AssetMeta* CreateMeta() const override { return mnew Meta(); }
	};
}

CLASS_BASES_META(o2::Asset)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Asset)
{
	PUBLIC_FIELD(path);
	PUBLIC_FIELD(fullPath);
	PUBLIC_FIELD(id);
	PUBLIC_FIELD(meta);
	PROTECTED_FIELD(mInfo);
}
END_META;
CLASS_METHODS_META(o2::Asset)
{

	PUBLIC_FUNCTION(const String&, GetPath);
	PUBLIC_FUNCTION(void, SetPath, const String&);
	PUBLIC_FUNCTION(String, GetFullPath);
	PUBLIC_FUNCTION(String, GetDataFullPath);
	PUBLIC_FUNCTION(const UID&, GetUID);
	PUBLIC_FUNCTION(AssetMeta*, GetMeta);
	PUBLIC_FUNCTION(const AssetInfo&, GetInfo);
	PUBLIC_FUNCTION(void, Load, const String&);
	PUBLIC_FUNCTION(void, Load, const UID&);
	PUBLIC_FUNCTION(void, Save, const String&, bool);
	PUBLIC_FUNCTION(void, Save, bool);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(String, GetMetaFullPath);
	PROTECTED_FUNCTION(UID&, ID);
	PROTECTED_FUNCTION(LogStream*, GetAssetsLogStream);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
	PROTECTED_FUNCTION(AssetMeta*, CreateMeta);
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
	PUBLIC_FIELD(meta);
}
END_META;
META_TEMPLATES(typename T)
CLASS_METHODS_META(o2::AssetWithDefaultMeta<T>)
{

	PUBLIC_FUNCTION(Meta*, GetMeta);
	PROTECTED_FUNCTION(AssetMeta*, CreateMeta);
}
END_META;
