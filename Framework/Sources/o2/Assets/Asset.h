#pragma once

#include "o2/Assets/AssetInfo.h"
#include "o2/Assets/Meta.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class AssetInfo;

	// ---------------------
	// Basic asset interface
	// ---------------------
	class Asset: public ISerializable
	{
	public:
		typedef AssetMeta MetaType;

	public:
		PROPERTIES(Asset);
		PROPERTY(String, path, SetPath, GetPath); // Asset path property
		GETTER(String, fullPath, GetFullPath);    // Full asset path getter (from binary path)
		GETTER(UID, id, GetAssetId);              // Asset id getter
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
		const UID& GetAssetId() const;

		// Returns meta information pointer
		AssetMeta* GetMeta() const;

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
		template<typename Type = Asset>
		Asset();

		// Copy-constructor
		Asset(const Asset& asset);

		// Returns meta full path (from binary path)
		String GetMetaFullPath() const;

		// Returns reference to id in meta
		UID& ID();

		// Returns assets log stream pointer
		LogStream* GetAssetsLogStream() const;

		friend class AssetsBuilder;
		friend class Assets;
	};

	template<typename Type>
	Asset::Asset()
	{
		mInfo.SetType<Type>();
	}

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
	PUBLIC_FUNCTION(const UID&, GetAssetId);
	PUBLIC_FUNCTION(AssetMeta*, GetMeta);
	PUBLIC_FUNCTION(void, Load);
	PUBLIC_FUNCTION(void, Load, const String&);
	PUBLIC_FUNCTION(void, Load, const UID&);
	PUBLIC_FUNCTION(void, Load, const AssetInfo&);
	PUBLIC_FUNCTION(void, Save, const String&, bool);
	PUBLIC_FUNCTION(void, Save, bool);
	PUBLIC_FUNCTION(void, Save, const AssetInfo&, bool);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(String, GetMetaFullPath);
	PROTECTED_FUNCTION(UID&, ID);
	PROTECTED_FUNCTION(LogStream*, GetAssetsLogStream);
}
END_META;

CLASS_BASES_META(o2::AssetRef)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetRef)
{
	PROTECTED_FIELD(mAssetOwner);
	PROTECTED_FIELD(mRefCounter);
	PROTECTED_FIELD(mAssetPtr);
}
END_META;
CLASS_METHODS_META(o2::AssetRef)
{

	PUBLIC_FUNCTION(bool, IsValid);
	PUBLIC_FUNCTION(const Type&, GetAssetType);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
}
END_META;
