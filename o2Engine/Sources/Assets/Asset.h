#pragma once

#include "Assets/AssetInfo.h"
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
		Property<String>   path;     // Asset path property
		Getter<String>     fullPath; // Full asset path getter (from binary path)
		Getter<UID>        id;       // Asset id getter
		Getter<IMetaInfo*> meta;     // Asset meta information pointer getter

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

		// Initializes properties
		void InitializeProperties();

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