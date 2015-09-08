#pragma once

#include "Assets/AssetInfo.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/Property.h"
#include "Utils/Serialization.h"

namespace o2
{
	// ---------------------
	// Basic asset interface
	// ---------------------
	class Asset: public ISerializable
	{
		friend class AssetsBuilder;

	public:
		// --------------------------------------
		// Basic asset meta information interface
		// --------------------------------------
		class IMetaInfo: public ISerializable
		{
			friend class Asset;

			UInt mId; // Id of assets

		public:
			// Default constructor
			IMetaInfo();

			// Virtual destructor
			virtual ~IMetaInfo();

			// Returns asset type id
			virtual Type::Id GetAssetType() const;

			// Returns true if other meta is equal to this
			virtual bool IsEqual(Ptr<IMetaInfo> other) const;

			// Returns asset id
			UInt ID() const;

			SERIALIZABLE_IMPL(IMetaInfo);

			IOBJECT(IMetaInfo)
			{
				SRLZ_FIELD(mId);
			}
		};

	public:
		Property<String>       Path;     // Asset path property
		Getter<String>         FullPath; // Full asset path getter (from binary path)
		Getter<UInt>           Id;       // Asset id getter
		Getter<Ptr<IMetaInfo>> Meta;     // Asset meta information pointer getter

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
		UInt GetAssetId() const;

		// Returns meta information pointer
		Ptr<IMetaInfo> GetMeta() const;

		// Loads asset
		void Load();

		// Loads asset from path
		void Load(const String& path);

		// Loads asset by id
		void Load(UInt id);

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

		SERIALIZABLE_IMPL(Asset);

		IOBJECT(Asset) 
		{
			FIELD(mPath);
			FIELD(mMeta);
		}

	protected:
		String         mPath; // Asset path
		Ptr<IMetaInfo> mMeta; // Asset meta information

	protected:
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
		UInt& IdRef();

		// Returns assets log stream pointer
		Ptr<LogStream> GetAssetsLogStream() const;

		// Initializes properties
		void InitializeProperties();
	};
}