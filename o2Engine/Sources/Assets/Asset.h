#pragma once

#include "Assets/AssetInfo.h"
#include "Utils/EditorPropertyAttribute.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/Property.h"
#include "Utils/Serializable.h"

namespace o2
{
	// ---------------------
	// Basic asset interface
	// ---------------------
	class Asset: public ISerializable
	{
	public:
		class IMetaInfo;

	public:
		PROPERTIES(Asset);
		PROPERTY(String, path, SetPath, GetPath); // Asset path property
		GETTER(String, fullPath, GetFullPath);    // Full asset path getter (from binary path)
		GETTER(UID, id, GetAssetId);              // Asset id getter
		GETTER(IMetaInfo*, meta, GetMeta);        // Asset meta information pointer getter

		// Virtual destructor
		virtual ~Asset();

		// Assign operator
		Asset& operator=(const Asset& asset);

		// Returns asset info
		AssetInfo GetAssetInfo() const;

		// Returns path
		String GetPath() const;

		// Sets path
		void SetPath(const String& path);

		// Returns full asset path (from binary path)
		String GetFullPath() const;

		// Returns full builded asset path (from binary)
		String GetDataFullPath() const;

		// Returns id of asset
		UID GetAssetId() const;

		// Returns meta information pointer
		IMetaInfo* GetMeta() const;

		// Loads asset
		void Load();

		// Loads asset from path
		void Load(const String& path);

		// Loads asset by id
		void Load(UID id);

		// Loads asset by info
		void Load(const AssetInfo& info);

		// Saves asset by path
		void Save(const String& path, bool rebuildAssetsImmediately = true);

		// Saves asset
		void Save(bool rebuildAssetsImmediately = true);

		// Saves asset by info
		void Save(const AssetInfo& info, bool rebuildAssetsImmediately = true);

		// Returns extensions string (something like "ext1 ext2 ent asf")
		virtual const char* GetFileExtensions() const;

		SERIALIZABLE(Asset);

	public:
		// --------------------------------------
		// Basic asset meta information interface
		// --------------------------------------
		class IMetaInfo: public ISerializable
		{
			friend class Asset;

			UID mId; // Id of asset @SERIALIZABLE

		public:
			// Default constructor
			IMetaInfo();

			// Virtual destructor
			virtual ~IMetaInfo();

			// Returns asset type id
			virtual const Type* GetAssetType() const;

			// Returns true if other meta is equal to this
			virtual bool IsEqual(IMetaInfo* other) const;

			// Returns asset id
			UID ID() const;

			void r() { mId.Randomize(); }

			SERIALIZABLE(IMetaInfo);
		};

	protected:
		String     mPath; // Asset path
		IMetaInfo* mMeta; // Asset meta information @EDITOR_PROPERTY

	protected:
		// Default constructor
		Asset();

		// Copy-constructor
		Asset(const Asset& asset);

		// Beginning serialization callback
		void OnSerialize(DataNode& node) const;

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Loads asset data
		virtual void LoadData(const String& path) {};

		// Saves asset data
		virtual void SaveData(const String& path) {};

		// Loads asset meta data
		virtual void LoadMeta(const String& path);

		// Saves asset meta data
		virtual void SaveMeta(const String& path);

		// Returns meta full path (from binary path)
		String GetMetaFullPath() const;

		// Returns reference to id in meta
		UID& IdRef();

		// Returns assets log stream pointer
		LogStream* GetAssetsLogStream() const;

		friend class AssetsBuilder;
		friend class Assets;
	};

	// ---------------
	// Asset reference
	// ---------------
	class AssetRef: public ISerializable
	{
	public:
		// Default constructor, references to null
		AssetRef();

		// Copy-constructor
		AssetRef(const AssetRef& other);

		// Constructor from asset path
		AssetRef(const String& path);

		// Constructor from asset id
		AssetRef(UID id);

		// Destructor
		virtual ~AssetRef();

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Assign operator
		AssetRef& operator=(const AssetRef& other);

		// Getter operator
		Asset& operator*();

		// Constant getter operator
		const Asset& operator*() const;

		// Asset members and field operator
		Asset* operator->();

		// Constant asset members and field operator
		const Asset* operator->() const;

		// Check equals operator
		bool operator==(const AssetRef& other) const;

		// Check not equals operator
		bool operator!=(const AssetRef& other) const;

		// Returns is reference is valid
		bool IsValid() const;

		// Returns asset type
		virtual const Type& GetAssetType() const;

		SERIALIZABLE(AssetRef);

	protected:
		int*   mRefCounter;
		Asset* mAssetPtr;

	protected:
		// Constructor for Assets manager
		AssetRef(Asset* assetPtr, int* refCounter);

		// Beginning serialization callback - writes path and id
		void OnSerialize(DataNode& node) const;

		// Completion deserialization callback -  reads path and id and searchs asset
		void OnDeserialized(const DataNode& node);

		// Check thats asset type is based on _asset_type. If not, resets reference
		template<typename _asset_type>
		void CheckType()
		{
			if (mAssetPtr && !mAssetPtr->GetType().IsBasedOn(TypeOf(_asset_type)))
			{
				(*mRefCounter)--;
				mAssetPtr = nullptr;
				mRefCounter = nullptr;
			}
		}

		friend class Assets;
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
	PROTECTED_FIELD(mPath);
	PROTECTED_FIELD(mMeta).EDITOR_PROPERTY_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::Asset)
{

	PUBLIC_FUNCTION(void, PROPERTIES, Asset);
	PUBLIC_FUNCTION(AssetInfo, GetAssetInfo);
	PUBLIC_FUNCTION(String, GetPath);
	PUBLIC_FUNCTION(void, SetPath, const String&);
	PUBLIC_FUNCTION(String, GetFullPath);
	PUBLIC_FUNCTION(String, GetDataFullPath);
	PUBLIC_FUNCTION(UID, GetAssetId);
	PUBLIC_FUNCTION(IMetaInfo*, GetMeta);
	PUBLIC_FUNCTION(void, Load);
	PUBLIC_FUNCTION(void, Load, const String&);
	PUBLIC_FUNCTION(void, Load, UID);
	PUBLIC_FUNCTION(void, Load, const AssetInfo&);
	PUBLIC_FUNCTION(void, Save, const String&, bool);
	PUBLIC_FUNCTION(void, Save, bool);
	PUBLIC_FUNCTION(void, Save, const AssetInfo&, bool);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
	PROTECTED_FUNCTION(void, LoadMeta, const String&);
	PROTECTED_FUNCTION(void, SaveMeta, const String&);
	PROTECTED_FUNCTION(String, GetMetaFullPath);
	PROTECTED_FUNCTION(UID&, IdRef);
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

CLASS_BASES_META(o2::Asset::IMetaInfo)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Asset::IMetaInfo)
{
	PUBLIC_FIELD(mId).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::Asset::IMetaInfo)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(bool, IsEqual, IMetaInfo*);
	PUBLIC_FUNCTION(UID, ID);
	PUBLIC_FUNCTION(void, r);
}
END_META;
