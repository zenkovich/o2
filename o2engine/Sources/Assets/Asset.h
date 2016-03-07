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
		Getter<AssetId>    id;       // Asset id getter
		Getter<IMetaInfo*> meta;     // Asset meta information pointer getter

		// Default constructor
		Asset();

		// Copy-constructor
		Asset(const Asset& asset);

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
		AssetId GetAssetId() const;

		// Returns meta information pointer
		IMetaInfo* GetMeta() const;

		// Loads asset
		void Load();

		// Loads asset from path
		void Load(const String& path);

		// Loads asset by id
		void Load(AssetId id);

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

			AssetId mId; // Id of assets @SERIALIZABLE

		public:
			// Default constructor
			IMetaInfo();

			// Virtual destructor
			virtual ~IMetaInfo();

			// Returns asset type id
			virtual Type::Id GetAssetType() const;

			// Returns true if other meta is equal to this
			virtual bool IsEqual(IMetaInfo* other) const;

			// Returns asset id
			AssetId ID() const;

			SERIALIZABLE(IMetaInfo);
		};

	protected:
		String     mPath; // Asset path
		IMetaInfo* mMeta; // Asset meta information

	protected:
		// Beginning serialization callback
		void OnSerialize(DataNode& node);

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
		AssetId& IdRef();

		// Returns assets log stream pointer
		LogStream* GetAssetsLogStream() const;

		// Initializes properties
		void InitializeProperties();

		friend class AssetsBuilder;
	};
}