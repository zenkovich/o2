#pragma once
#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetRef.h"
#include "o2/Scene/ActorRef.h"

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

		// Instantiates actor toscene
		ActorRef Instantiate();

		// Returns meta information
		Meta* GetMeta() const;

		// Returns actor
		Actor* GetActor() const;

		// Sets actor
		void SetActor(Actor* actor, bool own = true);

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
		Actor* mActor = nullptr;  // Asset data 
		bool   mOwnActor = false; // Is asset owns this actor

	protected:
		// Itis called when UID has changed; updates actor asset id
		void OnUIDChanged(const UID& oldUID) override;

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
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mActor);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mOwnActor);
}
END_META;
CLASS_METHODS_META(o2::ActorAsset)
{

	FUNCTION().PUBLIC().SIGNATURE(ActorRef, Instantiate);
	FUNCTION().PUBLIC().SIGNATURE(Meta*, GetMeta);
	FUNCTION().PUBLIC().SIGNATURE(Actor*, GetActor);
	FUNCTION().PUBLIC().SIGNATURE(void, SetActor, Actor*, bool);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const char*, GetFileExtensions);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetEditorIcon);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(int, GetEditorSorting);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, IsAvailableToCreateFromEditor);
	FUNCTION().PROTECTED().SIGNATURE(void, OnUIDChanged, const UID&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
