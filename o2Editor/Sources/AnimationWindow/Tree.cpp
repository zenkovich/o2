#include "stdafx.h"
#include "Tree.h"

namespace Editor
{

	AnimationTree::AnimationTree()
	{

	}

	AnimationTree::AnimationTree(const AnimationTree& other)
	{

	}

	AnimationTree::~AnimationTree()
	{

	}

	AnimationTree& AnimationTree::operator=(const AnimationTree& other)
	{

	}

	void AnimationTree::SetAnimation(Animation* animation)
	{
		mAnimation = animation;
	}

	UnknownPtr AnimationTree::GetObjectParent(UnknownPtr object)
	{

	}

	Vector<UnknownPtr> AnimationTree::GetObjectChilds(UnknownPtr object)
	{

	}

	String AnimationTree::GetObjectDebug(UnknownPtr object)
	{

	}

	void AnimationTree::FillNodeDataByObject(UITreeNode* nodeWidget, UnknownPtr object)
	{

	}

}

DECLARE_CLASS(Editor::AnimationTree);
