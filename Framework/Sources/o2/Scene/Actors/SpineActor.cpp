#include "o2/stdafx.h"
#include "SpineActor.h"

namespace o2
{
	SpineActor::SpineActor(RefCounter* refCounter) :
		Actor(refCounter)
	{}

	SpineActor::SpineActor(RefCounter* refCounter, const SpineActor& other) :
		Actor(refCounter, other)
	{}

	SpineActor::~SpineActor()
	{}

	void SpineActor::SetSpineAsset(const AssetRef<SpineAsset>& spineAsset)
	{
		mSpineAsset = spineAsset;
		LoadSpine();
	}

	const AssetRef<SpineAsset>& SpineActor::GetSpineAsset() const
	{
		return mSpineAsset;
	}

	SpineActor& SpineActor::operator=(const SpineActor& other)
	{
		Actor::operator=(other);

		mSpineAsset = other.mSpineAsset;
		LoadSpine();

		return *this;
	}

	void SpineActor::LoadSpine()
	{
		if (!mSpineAsset)
			return;

		mSpineRenderer = mmake<Spine>(mSpineAsset);
	}

	void SpineActor::OnInitialized()
	{
		Actor::OnInitialized();

		LoadSpine();
	}

	void SpineActor::OnTransformUpdated()
	{
		Actor::OnTransformUpdated();

		if (mSpineRenderer)
			mSpineRenderer->transform = transform->worldNonSizedBasis;
	}

	void SpineActor::OnUpdate(float dt)
	{
		Actor::OnUpdate(dt);

		if (mSpineRenderer)
			mSpineRenderer->Update(dt);
	}

	void SpineActor::OnDraw()
	{
		Actor::OnDraw();

		if (mSpineRenderer)
			mSpineRenderer->Draw();
	}

}

DECLARE_TEMPLATE_CLASS(o2::LinkRef<o2::SpineActor>);
// --- META ---

DECLARE_CLASS(o2::SpineActor, o2__SpineActor);
// --- END META ---
