#include "stdafx.h"
#include "Tree.h"

namespace Editor
{

	AnimationTree::AnimationTree()
	{

	}

	AnimationTree::AnimationTree(const AnimationTree& other):
		Tree(other)
	{

	}

	AnimationTree::~AnimationTree()
	{
		for (auto& val : mRootValues)
			delete val;
	}

	AnimationTree& AnimationTree::operator=(const AnimationTree& other)
	{
		Tree::operator=(other);

		return *this;
	}

	void AnimationTree::SetAnimation(Animation* animation)
	{
		mAnimation = animation;

		RebuildAnimationTree();
		UpdateNodesView();
	}

	void AnimationTree::RebuildAnimationTree()
	{
		for (auto& val : mRootValues)
			delete val;

		mRootValues.clear();

		if (!mAnimation)
			return;

		for (auto& value : mAnimation->GetAnimationsValues()) 
		{
			TreeNode* current = nullptr;

			int lastDel = 0;
			while (lastDel >= 0) 
			{
				int del = value.mTargetPath.Find('/', lastDel);
				String subPath = value.mTargetPath.SubStr(lastDel, del);
				TreeNode* next = (current ? current->children : mRootValues).FindMatch([&](TreeNode* x) { return x->name == subPath; });

				if (!next)
				{
					next = mnew TreeNode();
					next->name = subPath;

					if (current)
					{
						next->parent = current;
						current->children.Add(next);
					}
					else mRootValues.Add(next);
				}

				current = next;
				lastDel = del >= 0 ? del + 1 : -1;
			}

			current->animatedValue = value.mAnimatedValue;
		}
	}

	UnknownPtr AnimationTree::GetObjectParent(UnknownPtr object)
	{
		if (!object)
			return UnknownPtr((TreeNode*)nullptr);

		auto treeNode = (TreeNode*)object;
		return treeNode->parent;
	}

	Vector<UnknownPtr> AnimationTree::GetObjectChilds(UnknownPtr object)
	{
		if (!object)
			return mRootValues.Cast<UnknownPtr>();

		auto treeNode = (TreeNode*)object;
		return treeNode->children.Cast<UnknownPtr>();
	}

	String AnimationTree::GetObjectDebug(UnknownPtr object)
	{
		auto treeNode = (TreeNode*)object;
		return treeNode->name;
	}

	void AnimationTree::FillNodeDataByObject(UITreeNode* nodeWidget, UnknownPtr object)
	{
		auto treeNode = (TreeNode*)object;
		nodeWidget->name = treeNode->name;

		auto nameLayer = nodeWidget->layer["name"];
		if (nameLayer)
			((Text*)nameLayer->GetDrawable())->text = treeNode->name;
	}

}

DECLARE_CLASS(Editor::AnimationTree);
