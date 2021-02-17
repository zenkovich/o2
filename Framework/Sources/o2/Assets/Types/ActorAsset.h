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
		// Default constructor
		ActorAsset();

		// Constructor
		ActorAsset(Actor* actor);

		// Copy-constructor
		ActorAsset(const ActorAsset& other);

		// Destructor
		~ActorAsset();

		// Check equals operator
		ActorAsset& operator=(const ActorAsset& asset);

		// Returns meta information
		Meta* GetMeta() const;

		// Returns actor
		Actor* GetActor() const;

		// Returns extensions string
		static const char* GetFileExtensions();

		// Returns editor icon
		static String GetEditorIcon() { return "ui/UI4_actor_icon.png"; }

		// Returns editor sorting weight
		static int GetEditorSorting() { return 99; }

		// Is this asset type is available to create from editor's assets window
		static bool IsAvailableToCreateFromEditor() { return true; }

		SERIALIZABLE(ActorAsset);

	protected:
		Actor* mActor; // Asset data 

	protected:
		// Beginning serialization callback
		void OnSerialize(DataValue& node) const override;

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

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
	FIELD().NAME(mActor).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::ActorAsset)
{

	PUBLIC_FUNCTION(Meta*, GetMeta);
	PUBLIC_FUNCTION(Actor*, GetActor);
	PUBLIC_STATIC_FUNCTION(const char*, GetFileExtensions);
	PUBLIC_STATIC_FUNCTION(String, GetEditorIcon);
	PUBLIC_STATIC_FUNCTION(int, GetEditorSorting);
	PUBLIC_STATIC_FUNCTION(bool, IsAvailableToCreateFromEditor);
	PROTECTED_FUNCTION(void, OnSerialize, DataValue&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
}
END_META;
