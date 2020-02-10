#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetRef.h"

namespace o2
{
	class Actor;

	// -----------
	// Actor asset
	// -----------
	class ActorAsset: public AssetWithDefaultMeta<ActorAsset>
	{
	public:
		// Destructor
		~ActorAsset();

		// Check equals operator
		ActorAsset& operator=(const ActorAsset& asset);

		// Returns meta information
		Meta* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const override;

		// Returns actor
		Actor* GetActor() const;

		SERIALIZABLE(ActorAsset);

	protected:
		Actor* mActor; // Asset data @SERIALIZABLE

	protected:
		// Default constructor
		ActorAsset();

		// Copy-constructor
		ActorAsset(const ActorAsset& other);

		friend class Assets;
	};

	typedef Ref<ActorAsset> ActorAssetRef;
}

CLASS_BASES_META(o2::ActorAsset)
{
	BASE_CLASS(o2::AssetWithDefaultMeta<ActorAsset>);
}
END_META;
CLASS_FIELDS_META(o2::ActorAsset)
{
	PROTECTED_FIELD(mActor).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::ActorAsset)
{

	PUBLIC_FUNCTION(Meta*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PUBLIC_FUNCTION(Actor*, GetActor);
}
END_META;
