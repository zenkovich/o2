#pragma once

#include "Assets/ActorAsset.h"
#include "Assets/Asset.h"
#include "Assets/ImageAsset.h"
#include "ImageComponent.h"
#include "Scene/Component.h"

namespace o2
{
	class EditorTestComponent: public Component
	{
	public:
		int mInteger;
		float mFloat;
		String mString;
		WString mWString;
		bool mBool;
		ImageAsset mImageAsset;
		ActorAsset mActorAsset;
		Actor* mActor;
		Component* mComponent;
		ImageComponent* mImageComponent;
		Asset* mAsset;
		Color4 mColor;
		Vec2F mVec2F;
		Vec2I mVec2I;
		Vertex2 mVertex;
		RectF mRectF;
		RectI mRectI;

		SERIALIZABLE(EditorTestComponent);
	};
}
