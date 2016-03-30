#pragma once

#include "Assets/ActorAsset.h"
#include "Assets/Asset.h"
#include "Assets/DataAsset.h"
#include "Assets/ImageAsset.h"
#include "ImageComponent.h"
#include "Scene/Component.h"
#include "Scene/Tags.h"

namespace o2
{
	class EditorTestComponent: public Component
	{
	public:
		int mInteger;					           // @SERIALIZABLE
		float mFloat;					           // @SERIALIZABLE
		String mString;					           // @SERIALIZABLE
		WString mWString;				           // @SERIALIZABLE
		bool mBool;						           // @SERIALIZABLE
		ImageAsset mImageAsset;			           // @SERIALIZABLE
		ActorAsset mActorAsset;			           // @SERIALIZABLE
		DataAsset mDataAsset;                      // @SERIALIZABLE
		Actor* mActor = nullptr;                   // @SERIALIZABLE
		TagGroup mTags;                            // @SERIALIZABLE
		Scene::Layer* mLayer;                      // @SERIALIZABLE
		Component* mComponent = nullptr;           // @SERIALIZABLE
		ImageComponent* mImageComponent = nullptr; // @SERIALIZABLE
		Color4 mColor;					           // @SERIALIZABLE
		Vec2F mVec2F;					           // @SERIALIZABLE
		Vec2I mVec2I;					           // @SERIALIZABLE
		Vertex2 mVertex;				           // @SERIALIZABLE
		RectF mRectF;					           // @SERIALIZABLE
		RectI mRectI;					           // @SERIALIZABLE

		SERIALIZABLE(EditorTestComponent);
	};
}
