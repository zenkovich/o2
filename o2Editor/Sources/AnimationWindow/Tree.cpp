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
			int lastDel = 0;
			int del = value.mTargetPath.Find('/');
			while (del >= 0) 
			{
				String subPath = value.mTargetPath.SubStr(lastDel, del);
			}
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
	}

}

DECLARE_CLASS(Editor::AnimationTree);
