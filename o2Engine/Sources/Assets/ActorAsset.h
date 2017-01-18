#pragma once

#include "Assets/Asset.h"

namespace o2
{
	class Actor;

	// -----------
	// Actor asset
	// -----------
	class ActorAsset: public Asset
	{
	public:
		class MetaInfo;

	public:
		Getter<MetaInfo*> meta;  // Meta information getter

		// Destructor
		~ActorAsset();

		// Check equals operator
		ActorAsset& operator=(const ActorAsset& asset);

		// Check equals operator
		bool operator==(const ActorAsset& other) const;

		// Check not equals operator
		bool operator!=(const ActorAsset& other) const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		// Returns actor
		Actor* GetActor() const;

		SERIALIZABLE(ActorAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			// Returns asset type id
			const Type* GetAssetType() const;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		Actor* mActor; // Asset data

	protected:
		// Default constructor
		ActorAsset();

		// Copy-constructor
		ActorAsset(const ActorAsset& other);

		// Constructor by path - loads asset by path
		ActorAsset(const String& path);

		// Constructor by id - loads asset by id
		ActorAsset(UID id);

		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Initializes properties
		void InitializeProperties();

		friend class Assets;
	};

	// ---------------------
	// Actor Asset reference
	// ---------------------
	class ActorAssetRef: public AssetRef
	{
	public:
		// Creates ActorAsset and returns reference to it
		static ActorAssetRef CreateAsset();

		// Default constructor, references to null
		ActorAssetRef(): AssetRef() {}

		// Copy-constructor
		ActorAssetRef(const AssetRef& other): AssetRef(other) { CheckType<ActorAsset>(); }

		// Copy-constructor
		ActorAssetRef(const ActorAssetRef& other): AssetRef(other) {}

		// Constructor from asset path
		ActorAssetRef(const String& path): AssetRef(path) {}

		// Constructor from asset id
		ActorAssetRef(UID id): AssetRef(id) {}

		// Destructor
		~ActorAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		ActorAssetRef& operator=(const ActorAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		ActorAsset& operator*() { return *((ActorAsset*)mAssetPtr); }

		// Constant getter operator
		const ActorAsset& operator*() const { return *((ActorAsset*)mAssetPtr); }

		// Asset members and field operator
		ActorAsset* operator->() { return ((ActorAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const ActorAsset* operator->() const { return ((ActorAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const ActorAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const ActorAssetRef& other) const { return AssetRef::operator!=(other); }

		SERIALIZABLE(ActorAssetRef);

	protected:
		// Constructor for Assets manager
		ActorAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}