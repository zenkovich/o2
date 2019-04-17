#include "stdafx.h"
#include "Tree.h"
#include "Scene/UI/WidgetLayout.h"

namespace Editor
{

	AnimationTree::AnimationTree() :
		Tree()
	{
		mZebraBackLine = mnew Sprite();
	}

	AnimationTree::AnimationTree(const AnimationTree& other) :
		Tree(other), mZebraBackLine(other.mZebraBackLine->CloneAs<Sprite>())
	{}

	AnimationTree::~AnimationTree()
	{
		for (auto& val : mRootValues)
			delete val;

		delete mZebraBackLine;
	}

	AnimationTree& AnimationTree::operator=(const AnimationTree& other)
	{
		delete mZebraBackLine;

		Tree::operator=(other);

		mZebraBackLine = other.mZebraBackLine->CloneAs<Sprite>();

		return *this;
	}

	void AnimationTree::Draw()
	{
		DrawZebraBack();
		Tree::Draw();
	}

	void AnimationTree::SetAnimation(Animation* animation)
	{
		mAnimation = animation;

		RebuildAnimationTree();
		UpdateNodesView();
	}

	Sprite* AnimationTree::GetZebraBackLine() const
	{
		return mZebraBackLine;
	}

	void AnimationTree::RebuildAnimationTree()
	{
		for (auto& val : mRootValues)
			delete val;

		mRootValues.clear();

		if (!mAnimation)
			return;

		for (auto& value : mAnimation->GetAnimationsValues())
			AddAnimatedValue(value);
	}

	void AnimationTree::AddAnimatedValue(Animation::AnimatedValueDef& value)
	{
		AnimationValueNode* current = nullptr;

		int lastDel = 0;
		while (lastDel >= 0)
		{
			int del = value.mTargetPath.Find('/', lastDel);
			String subPath = value.mTargetPath.SubStr(lastDel, del);
			AnimationValueNode* next = (current ? current->children : mRootValues)
				.FindMatch([&](AnimationValueNode* x) { return x->name == subPath; });

			if (!next)
			{
				next = mnew AnimationValueNode();
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

#undef DrawText
	void AnimationTree::DrawZebraBack()
	{
		o2Render.EnableScissorTest(mAbsoluteClipArea);

		float pos = -mScrollPos.y;
		float lineSize = mNodeWidgetSample->layout->GetMinimalHeight();
		while (pos < -lineSize)
			pos += lineSize*2.0f;

		RectF layoutRect = layout->GetWorldRect();
		while (pos < layoutRect.Height())
		{
			mZebraBackLine->SetRect(RectF(layoutRect.left, layoutRect.top - pos, layoutRect.right, layoutRect.top - pos - lineSize));
			mZebraBackLine->Draw();
			pos += lineSize*2.0f;
		}

		o2Render.DisableScissorTest();
	}

	UnknownPtr AnimationTree::GetObjectParent(UnknownPtr object)
	{
		if (!object)
			return UnknownPtr((AnimationValueNode*)nullptr);

		auto treeNode = (AnimationValueNode*)object;
		return treeNode->parent;
	}

	Vector<UnknownPtr> AnimationTree::GetObjectChilds(UnknownPtr object)
	{
		if (!object)
			return mRootValues.Cast<UnknownPtr>();

		auto treeNode = (AnimationValueNode*)object;
		return treeNode->children.Cast<UnknownPtr>();
	}

	String AnimationTree::GetObjectDebug(UnknownPtr object)
	{
		auto treeNode = (AnimationValueNode*)object;
		return treeNode->name;
	}

	void AnimationTree::FillNodeDataByObject(TreeNode* nodeWidget, UnknownPtr object)
	{
		AnimationTreeNode* node = dynamic_cast<AnimationTreeNode*>(nodeWidget);
		node->Setup((AnimationValueNode*)object);
	}

	AnimationTreeNode::AnimationTreeNode() :
		TreeNode()
	{}

	AnimationTreeNode::AnimationTreeNode(const AnimationTreeNode& other) :
		TreeNode(other)
	{
		InitializeControls();
	}

	AnimationTreeNode& AnimationTreeNode::operator=(const AnimationTreeNode& other)
	{
		TreeNode::operator=(other);
		InitializeControls();

		return *this;
	}

	void AnimationTreeNode::Setup(AnimationTree::AnimationValueNode* node)
	{
		mData = node;
		mNameDrawable->text = node->name;
	}

	void AnimationTreeNode::CopyData(const Actor& otherActor)
	{
		TreeNode::CopyData(otherActor);
		InitializeControls();
	}

	void AnimationTreeNode::OnDeserialized(const DataNode& node)
	{
		TreeNode::OnDeserialized(node);
		InitializeControls();
	}

	void AnimationTreeNode::InitializeControls()
	{
		mNameDrawable = GetLayerDrawable<Text>("name");
	}

}

DECLARE_CLASS(Editor::AnimationTree);

DECLARE_CLASS(Editor::AnimationTreeNode);
