#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetRef.h"

namespace o2
{
	class Actor;

	// -----------
	// Actor asset
	// -----------
	class ActorAsset: public TAsset<ActorAsset>
	{
	public:
		class Meta;

		PROPERTIES(ActorAsset);
		GETTER(Meta*, meta, GetMeta);  // Meta information getter

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

	public:
		// ----------------
		// Meta information
		// ----------------
		class Meta: public TAssetMeta<ActorAsset>
		{
		public:
			SERIALIZABLE(Meta);
		};

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
	BASE_CLASS(o2::TAsset<ActorAsset>);
}
END_META;
CLASS_FIELDS_META(o2::ActorAsset)
{
	PUBLIC_FIELD(meta);
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

CLASS_BASES_META(o2::ActorAsset::Meta)
{
	BASE_CLASS(o2::TAssetMeta<ActorAsset>);
}
END_META;
CLASS_FIELDS_META(o2::ActorAsset::Meta)
{
}
END_META;
CLASS_METHODS_META(o2::ActorAsset::Meta)
{
}
END_META;
