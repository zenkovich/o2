#pragma once

#include "Scene/UI/Widgets/Tree.h"

using namespace o2;

namespace o2
{
	class Animation;
}

namespace Editor
{
	class AnimationTree : public Tree
	{
	public:		
		// Default constructor
		AnimationTree();

		// Copy-constructor
		AnimationTree(const AnimationTree& other);

		// Destructor. Clearing tree data
		~AnimationTree();


		// Copy-operator
		AnimationTree& operator=(const AnimationTree& other);


		// Sets animation and updates tree structure
		void SetAnimation(Animation* animation);

		SERIALIZABLE(AnimationTree);

	private:
		struct TreeNode
		{
			String name;
			IAnimatedValue* animatedValue;

			TreeNode* parent = nullptr;
			Vector<TreeNode*> children;
		};

	private:
		Animation* mAnimation = nullptr;
		Vector<TreeNode*> mRootValues;

	private:
		// Rebuilds animation values tree - mRootValues
		void RebuildAnimationTree();

		// Returns object's parent
		UnknownPtr GetObjectParent(UnknownPtr object) override;

		// Returns object's children
		Vector<UnknownPtr> GetObjectChilds(UnknownPtr object) override;

		// Returns debugging string for object
		String GetObjectDebug(UnknownPtr object) override;

		// Sets nodeWidget data by object
		void FillNodeDataByObject(UITreeNode* nodeWidget, UnknownPtr object) override;
	};
}

CLASS_BASES_META(Editor::AnimationTree)
{
	BASE_CLASS(o2::Tree);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationTree)
{
	PRIVATE_FIELD(mAnimation);
	PRIVATE_FIELD(mRootValues);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTree)
{

	PUBLIC_FUNCTION(void, SetAnimation, Animation*);
	PRIVATE_FUNCTION(void, RebuildAnimationTree);
	PRIVATE_FUNCTION(UnknownPtr, GetObjectParent, UnknownPtr);
	PRIVATE_FUNCTION(Vector<UnknownPtr>, GetObjectChilds, UnknownPtr);
	PRIVATE_FUNCTION(String, GetObjectDebug, UnknownPtr);
	PRIVATE_FUNCTION(void, FillNodeDataByObject, UITreeNode*, UnknownPtr);
}
END_META;
