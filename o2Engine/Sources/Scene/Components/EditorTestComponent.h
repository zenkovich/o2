#pragma once

#include "Assets/ActorAsset.h"
#include "Assets/Asset.h"
#include "Assets/DataAsset.h"
#include "Assets/ImageAsset.h"
#include "ImageComponent.h"
#include "Render/Sprite.h"
#include "Scene/Component.h"
#include "Scene/Tags.h"
#include "Utils/Math/Curve.h"

namespace o2
{
	class EditorTestComponent: public Component
	{
	public:
		enum class TestEnum { A, B, C, D, E, F, G, H, K, L, M, N };
		 
		class TestInside: public ISerializable
		{
		public:
			float mFloat;	  // @SERIALIZABLE
			String mString;	  // @SERIALIZABLE
			WString mWString; // @SERIALIZABLE
			bool mBool;		  // @SERIALIZABLE

			bool operator==(const TestInside& other) const { return false; }

			SERIALIZABLE(TestInside);
		};

		void SetSpritePtr(Sprite* sprite) { mSprite = sprite; }
		Sprite* GetSpritePtr() const { return mSprite; }

		void SetSprite(const Sprite& sprite) { *mSprite = sprite; }
		Sprite GetSprite() const { return *mSprite; }

		void SetArray(const Vector<Vec2I>& arr) { mVecs = arr; }
		const Vector<Vec2I>& GetArray() const { return mVecs; }

		PROPERTIES(EditorTestComponent);
		PROPERTY(Sprite*, spritePropPtr, SetSpritePtr, GetSpritePtr);
		PROPERTY(Sprite, spriteProp, SetSprite, GetSprite);
		PROPERTY(Vector<Vec2I>, arr, SetArray, GetArray);

		int mInteger;					           // @SERIALIZABLE
		float mFloat;					           // @SERIALIZABLE
		String mString;					           // @SERIALIZABLE
		WString mWString;				           // @SERIALIZABLE
		bool mBool;						           // @SERIALIZABLE
		ImageAssetRef mImageAsset;			       // @SERIALIZABLE
		ActorAssetRef mActorAsset;			       // @SERIALIZABLE
		DataAssetRef mDataAsset;                   // @SERIALIZABLE
		Sprite spritex;                            // @SERIALIZABLE
		Sprite* mSprite = mnew Sprite();           // @SERIALIZABLE
		Actor* mActor = nullptr;                   // @SERIALIZABLE
		TagGroup mTags;                            // @SERIALIZABLE
		SceneLayer* mLayer;                        // @SERIALIZABLE
		Component* mComponent = nullptr;           // @SERIALIZABLE
		ImageComponent* mImageComponent = nullptr; // @SERIALIZABLE
		Color4 mColor;					           // @SERIALIZABLE
		Vec2F mVec2F;					           // @SERIALIZABLE
		Vec2I mVec2I;					           // @SERIALIZABLE
		Vertex2 mVertex;				           // @SERIALIZABLE
		RectF mRectF;					           // @SERIALIZABLE
		RectI mRectI;					           // @SERIALIZABLE
		BorderF mBorderF;					       // @SERIALIZABLE
		BorderI mBorderI;					       // @SERIALIZABLE
		Curve mCurve = Curve::EaseInOut();         // @SERIALIZABLE
		TestInside mTestInside;                    // @SERIALIZABLE
		TestEnum mTestEnum;                        // @SERIALIZABLE
		TestInside* mTestInsidePtr = nullptr;      // @SERIALIZABLE

		Vector<Vec2I> mVecs = { Vec2I(0, 1), Vec2I(2, 3), Vec2I(4, 5) }; // @SERIALIZABLE
		Vector<int> mIntVector ={ 1, 2, 3, 4, 5 }; // @SERIALIZABLE
		Vector<TestInside> mTestInsideVector;      // @SERIALIZABLE
		Vector<Actor*> mActorVector;               // @SERIALIZABLE

		Dictionary<String, String> mDictionary;    // @SERIALIZABLE
		float mFloat2;					           // @SERIALIZABLE
		float mFloat3;					           // @SERIALIZABLE
		float mFloat4;					           // @SERIALIZABLE
		float mFloat5;					           // @SERIALIZABLE
		float mFloat6;					           // @SERIALIZABLE
		float mFloat7;					           // @SERIALIZABLE
		float mFloat8;					           // @SERIALIZABLE
		float mFloat9;					           // @SERIALIZABLE

		SERIALIZABLE(EditorTestComponent);
	};
}

CLASS_BASES_META(o2::EditorTestComponent)
{
	BASE_CLASS(o2::Component);
}
END_META;
CLASS_FIELDS_META(o2::EditorTestComponent)
{
	PUBLIC_FIELD(spritePropPtr);
	PUBLIC_FIELD(spriteProp);
	PUBLIC_FIELD(arr);
	PUBLIC_FIELD(mInteger).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFloat).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mString).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mWString).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mBool).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mImageAsset).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mActorAsset).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mDataAsset).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(spritex).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mSprite).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mActor).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mTags).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mLayer).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mComponent).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mImageComponent).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mColor).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mVec2F).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mVec2I).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mVertex).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mRectF).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mRectI).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mBorderF).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mBorderI).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mCurve).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mTestInside).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mTestEnum).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mTestInsidePtr).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mVecs).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mIntVector).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mTestInsideVector).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mActorVector).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mDictionary).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFloat2).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFloat3).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFloat4).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFloat5).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFloat6).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFloat7).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFloat8).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFloat9).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::EditorTestComponent)
{

	PUBLIC_FUNCTION(void, SetSpritePtr, Sprite*);
	PUBLIC_FUNCTION(Sprite*, GetSpritePtr);
	PUBLIC_FUNCTION(void, SetSprite, const Sprite&);
	PUBLIC_FUNCTION(Sprite, GetSprite);
	PUBLIC_FUNCTION(void, SetArray, const Vector<Vec2I>&);
	PUBLIC_FUNCTION(const Vector<Vec2I>&, GetArray);
}
END_META;

CLASS_BASES_META(o2::EditorTestComponent::TestInside)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::EditorTestComponent::TestInside)
{
	PUBLIC_FIELD(mFloat).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mString).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mWString).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mBool).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::EditorTestComponent::TestInside)
{
}
END_META;
