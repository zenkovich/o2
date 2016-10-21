#pragma once

#include "Assets/Asset.h"
#include "Scene/Actor.h"

namespace o2
{
	// -----------
	// Actor asset
	// -----------
	class ActorAsset: public Asset
	{
	public:
		class MetaInfo;

	public:
		Actor             actor; // Asset data
		Getter<MetaInfo*> meta;  // Meta information getter

		// Default constructor
		ActorAsset();

		// Constructor by path - loads asset by path
		ActorAsset(const String& path);

		// Constructor by id - loads asset by id
		ActorAsset(UID id);

		// Copy-constructor
		ActorAsset(const ActorAsset& asset);

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

		SERIALIZABLE(ActorAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			// Returns asset type id
			Type::Id GetAssetType() const;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Initializes properties
		void InitializeProperties();
	};
}