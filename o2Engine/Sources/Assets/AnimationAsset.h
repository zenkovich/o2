#pragma once

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedValue.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Assets/Asset.h"

namespace o2
{
	// ---------------
	// Animation asset
	// ---------------
	class AnimationAsset: public Asset
	{
	public:
		class MetaInfo;

	public:
		Animation         animation; // Asset data
		Getter<MetaInfo*> meta;      // Meta information getter

		// Default constructor
		AnimationAsset();

		// Constructor by path - loads asset by path
		AnimationAsset(const String& path);

		// Constructor by id - loads asset by id
		AnimationAsset(UID id);

		// Copy-constructor
		AnimationAsset(const AnimationAsset& asset);

		// Destructor
		~AnimationAsset();

		// Check equals operator
		AnimationAsset& operator=(const AnimationAsset& asset);

		// Check equals operator
		bool operator==(const AnimationAsset& other) const;

		// Check not equals operator
		bool operator!=(const AnimationAsset& other) const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		SERIALIZABLE(AnimationAsset);

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