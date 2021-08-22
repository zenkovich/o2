#pragma once

#include "ImageComponent.h"
#include "o2/Assets/Asset.h"
#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Assets/Types/DataAsset.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Components/ParticlesEmitterComponent.h"
#include "o2/Scene/Physics/RigidBody.h"
#include "o2/Scene/Tags.h"
#include "o2/Utils/Editor/Attributes/DontDeleteAttribute.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"
#include "o2/Utils/Math/Curve.h"

namespace o2
{
	class EditorTestComponent: public Component
	{
	public:
		Vector<Vec2I> mVecs; // @SERIALIZABLE @INVOKE_ON_CHANGE(Test)

		enum class TestEnum { A, B, C, D, E, F, G, H, K, L, M, N };
		 
		class TestInside: public ISerializable
		{
		public:
			float mFloat;	  // @SERIALIZABLE
			String mString;	  // @SERIALIZABLE
			WString mWString; // @SERIALIZABLE
			bool mBool;		  // @SERIALIZABLE

			ComponentRef mComponent;   // @SERIALIZABLE
			Ref<RigidBody> mRigidBody; // @SERIALIZABLE

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

		int mInteger;					                    // @SERIALIZABLE @INVOKE_ON_CHANGE(Test)
		float mFloat;					                    // @SERIALIZABLE
		String mString;					                    // @SERIALIZABLE
		WString mWString;				                    // @SERIALIZABLE
		bool mBool;						                    // @SERIALIZABLE
		ImageAssetRef mImageAsset;			                // @SERIALIZABLE
		ActorAssetRef mActorAsset;			                // @SERIALIZABLE
		DataAssetRef mDataAsset;                            // @SERIALIZABLE
		AnimationAssetRef mAnimationAsset;                  // @SERIALIZABLE
		Sprite* mSprite = mnew Sprite();                    // @SERIALIZABLE @DONT_DELETE
		ActorRef mActor;                                    // @SERIALIZABLE
		TagGroup mTags;                                     // @SERIALIZABLE
		SceneLayer* mLayer;                               
		ComponentRef mComponent;                            // @SERIALIZABLE
		Ref<RigidBody> mRigidBody;                          // @SERIALIZABLE
		Ref<ImageComponent> mImageComponent;                // @SERIALIZABLE
		Ref<ParticlesEmitterComponent> mParticlesComponent; // @SERIALIZABLE
		Color4 mColor;					                    // @SERIALIZABLE
		Vec2F mVec2F;					                    // @SERIALIZABLE
		Vec2I mVec2I;					                    // @SERIALIZABLE
		Vertex2 mVertex;				                    // @SERIALIZABLE
		RectF mRectF;					                    // @SERIALIZABLE
		RectI mRectI;					                    // @SERIALIZABLE
		BorderF mBorderF;					                // @SERIALIZABLE
		BorderI mBorderI;					                // @SERIALIZABLE
		Curve mCurve = Curve::EaseInOut();                  // @SERIALIZABLE
		TestInside mTestInside;                             // @SERIALIZABLE
		TestEnum mTestEnum;                                 // @SERIALIZABLE
		TestInside* mTestInsidePtr = nullptr;               // @SERIALIZABLE

		Vector<int> mIntVector;                    // @SERIALIZABLE
		Vector<TestInside> mTestInsideVector;      // @SERIALIZABLE @INVOKE_ON_CHANGE(Test)
		Vector<TestInside*> mTestInsideptrsVector; // @SERIALIZABLE @INVOKE_ON_CHANGE(Test)
		Vector<ActorRef> mActorVector;             // @SERIALIZABLE
		Vector<AnimationAssetRef> mAssetsVector;   // @SERIALIZABLE

		Vector<Vector<TestInside*>> mVectorOfVector; // @SERIALIZABLE

		Map<String, String> mDictionary;    // @SERIALIZABLE
		float mFloat2;					    // @SERIALIZABLE
		float mFloat3;					    // @SERIALIZABLE
		float mFloat4;					    // @SERIALIZABLE
		float mFloat5;					    // @SERIALIZABLE
		float mFloat6;					    // @SERIALIZABLE
		float mFloat7;					    // @SERIALIZABLE
		float mFloat8;					    // @SERIALIZABLE
		float mFloat9;					    // @SERIALIZABLE

		static String GetCategory();

		void Test();

		SERIALIZABLE(EditorTestComponent);
	};
}

PRE_ENUM_META(o2::EditorTestComponent::TestEnum);

CLASS_BASES_META(o2::EditorTestComponent)
{
	BASE_CLASS(o2::Component);
}
END_META;
CLASS_FIELDS_META(o2::EditorTestComponent)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mVecs).PUBLIC();
	FIELD().NAME(spritePropPtr).PUBLIC();
	FIELD().NAME(spriteProp).PUBLIC();
	FIELD().NAME(arr).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mInteger).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mFloat).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mString).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mWString).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mBool).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mImageAsset).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mActorAsset).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mDataAsset).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mAnimationAsset).PUBLIC();
	FIELD().DONT_DELETE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(mnew Sprite()).NAME(mSprite).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mActor).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mTags).PUBLIC();
	FIELD().NAME(mLayer).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mComponent).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mRigidBody).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mImageComponent).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mParticlesComponent).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mColor).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mVec2F).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mVec2I).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mVertex).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mRectF).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mRectI).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mBorderF).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mBorderI).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Curve::EaseInOut()).NAME(mCurve).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mTestInside).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mTestEnum).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mTestInsidePtr).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mIntVector).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mTestInsideVector).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mTestInsideptrsVector).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mActorVector).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mAssetsVector).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mVectorOfVector).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mDictionary).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mFloat2).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mFloat3).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mFloat4).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mFloat5).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mFloat6).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mFloat7).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mFloat8).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mFloat9).PUBLIC();
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
	PUBLIC_STATIC_FUNCTION(String, GetCategory);
	PUBLIC_FUNCTION(void, Test);
}
END_META;

CLASS_BASES_META(o2::EditorTestComponent::TestInside)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::EditorTestComponent::TestInside)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mFloat).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mString).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mWString).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mBool).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mComponent).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mRigidBody).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::EditorTestComponent::TestInside)
{
}
END_META;
