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
}
END_META;
CLASS_METHODS_META(Editor::AnimationTree)
{
}
END_META;
