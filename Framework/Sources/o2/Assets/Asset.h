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

		// Returns path
		const String& GetPath() const;

		// Sets path
		void SetPath(const String& path);

		// Returns full asset path (from binary path)
		String GetFullPath() const;

		// Returns full built asset path (from binary)
		String GetBuiltFullPath() const;

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
		static const char* GetFileExtensions();

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
		PROPERTY(AssetMeta*, mMeta, SetMeta, GetMeta); // @EDITOR_PROPERTY @DONT_DELETE @EXPANDED_BY_DEFAULT @NO_HEADER

		AssetInfo mInfo; // Asset info 

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
		LogStream* GetAssetsLogStream() const;

		// Meta setter, used for property
		void SetMeta(AssetMeta* meta);

		// Loads asset from path
		void Load(const AssetInfo& info);

		// Loads asset data, using DataValue and serialization
		virtual void LoadData(const String& path);

		// Saves asset data, using DataValue and serialization
		virtual void SaveData(const String& path) const;

		friend class AssetRef;
		friend class Assets;
		friend class AssetsBuilder;
	};

	// This macro defines asset type
#define ASSET_TYPE(THIS_TYPE, META_TYPE)                            \
SERIALIZABLE(THIS_TYPE);                                            \
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

CLASS_BASES_META(o2::Asset)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Asset)
{
	FIELD().EDITOR_IGNORE_ATTRIBUTE().NAME(path).PUBLIC();
	FIELD().NAME(fullPath).PUBLIC();
	FIELD().NAME(id).PUBLIC();
	FIELD().NAME(meta).PUBLIC();
	FIELD().DONT_DELETE_ATTRIBUTE().EDITOR_PROPERTY_ATTRIBUTE().EXPANDED_BY_DEFAULT_ATTRIBUTE().NO_HEADER_ATTRIBUTE().NAME(mMeta).PUBLIC();
	FIELD().NAME(mInfo).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::Asset)
{

	PUBLIC_FUNCTION(const String&, GetPath);
	PUBLIC_FUNCTION(void, SetPath, const String&);
	PUBLIC_FUNCTION(String, GetFullPath);
	PUBLIC_FUNCTION(String, GetBuiltFullPath);
	PUBLIC_FUNCTION(const UID&, GetUID);
	PUBLIC_FUNCTION(AssetMeta*, GetMeta);
	PUBLIC_FUNCTION(const AssetInfo&, GetInfo);
	PUBLIC_FUNCTION(void, Load, const String&);
	PUBLIC_FUNCTION(void, Load, const UID&);
	PUBLIC_FUNCTION(void, Save, const String&, bool);
	PUBLIC_FUNCTION(void, Save, bool);
	PUBLIC_STATIC_FUNCTION(const char*, GetFileExtensions);
	PUBLIC_STATIC_FUNCTION(String, GetEditorIcon);
	PUBLIC_STATIC_FUNCTION(int, GetEditorSorting);
	PUBLIC_STATIC_FUNCTION(bool, IsAvailableToCreateFromEditor);
	PUBLIC_STATIC_FUNCTION(bool, IsReferenceCanOwnInstance);
	PROTECTED_FUNCTION(String, GetMetaFullPath);
	PROTECTED_FUNCTION(UID&, ID);
	PROTECTED_FUNCTION(LogStream*, GetAssetsLogStream);
	PROTECTED_FUNCTION(void, SetMeta, AssetMeta*);
	PROTECTED_FUNCTION(void, Load, const AssetInfo&);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
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
	FIELD().NAME(meta).PUBLIC();
}
END_META;
META_TEMPLATES(typename T)
CLASS_METHODS_META(o2::AssetWithDefaultMeta<T>)
{

	PUBLIC_FUNCTION(Meta*, GetMeta);
}
END_META;
