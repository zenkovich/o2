#pragma once

#include "o2/Scene/Actor.h"
#include "o2/Render/Spine/Spine.h"

namespace o2
{
	// ----------------------------
	// EsotericSoftware spine actor
	// ----------------------------
	class SpineActor : public Actor
	{
	public:
		PROPERTIES(SpineActor);
		PROPERTY(AssetRef<SpineAsset>, spineAsset, SetSpineAsset, GetSpineAsset); // Spine asset @EDITOR_PROPERTY

	public:
		// Default constructor
		explicit SpineActor(RefCounter* refCounter);

		// Copy-constructor
		SpineActor(RefCounter* refCounter, const SpineActor& other);

		// Copy-operator
		SpineActor& operator=(const SpineActor& other);

		// Destructor
		~SpineActor();

		// Sets spine asset and loads spine
		void SetSpineAsset(const AssetRef<SpineAsset>& spineAsset);

		// Returns spine asset
		const AssetRef<SpineAsset>& GetSpineAsset() const;

		SERIALIZABLE(SpineActor);
		CLONEABLE_REF(SpineActor);

	protected:
		AssetRef<SpineAsset> mSpineAsset; // Spine asset @SERIALIZABLE

		Ref<Spine> mSpineRenderer; // Spine renderer

	protected:
		// Loads spine from asset
		void LoadSpine();

		// Called when actor and scene is initialized
		void OnInitialized() override;

		// Called when transformation was updated. Updates spine root transform
		void OnTransformUpdated() override;

		// Called on update with frame dt, updates spine animation
		void OnUpdate(float dt) override;

		// Called when actor is drawing, draws spine animation
		void OnDraw() override;
	};
}
// --- META ---

CLASS_BASES_META(o2::SpineActor)
{
    BASE_CLASS(o2::Actor);
}
END_META;
CLASS_FIELDS_META(o2::SpineActor)
{
    FIELD().PUBLIC().EDITOR_PROPERTY_ATTRIBUTE().NAME(spineAsset);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mSpineAsset);
    FIELD().PROTECTED().NAME(mSpineRenderer);
}
END_META;
CLASS_METHODS_META(o2::SpineActor)
{

    FUNCTION().PUBLIC().CONSTRUCTOR(RefCounter*);
    FUNCTION().PUBLIC().CONSTRUCTOR(RefCounter*, const SpineActor&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetSpineAsset, const AssetRef<SpineAsset>&);
    FUNCTION().PUBLIC().SIGNATURE(const AssetRef<SpineAsset>&, GetSpineAsset);
    FUNCTION().PROTECTED().SIGNATURE(void, LoadSpine);
    FUNCTION().PROTECTED().SIGNATURE(void, OnInitialized);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
    FUNCTION().PROTECTED().SIGNATURE(void, OnUpdate, float);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDraw);
}
END_META;
// --- END META ---
