#pragma once

#include "Scene/UI/Widgets/Tree.h"

using namespace o2;

namespace o2
{
	class Animation;
}

namespace Editor
{
	// ---------------------
	// Animation values tree
	// ---------------------
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


		// Draws widget
		void Draw() override;


		// Sets animation and updates tree structure
		void SetAnimation(Animation* animation);


		// Return zebra back line sprite
		Sprite* GetZebraBackLine() const;

		SERIALIZABLE(AnimationTree);

	private:
		struct AnimationValueNode
		{
			String name;
			IAnimatedValue* animatedValue;

			AnimationValueNode* parent = nullptr;
			Vector<AnimationValueNode*> children;
		};

	private:
		Animation* mAnimation = nullptr;
		Vector<AnimationValueNode*> mRootValues;

		Sprite* mZebraBackLine = nullptr; // Dark zebra line sprite @SERIALIZABLE

	private:
		// Rebuilds animation values tree - mRootValues
		void RebuildAnimationTree();

		// Adds animated value to tree. Creates intermediate nodes when required
		void AddAnimatedValue(Animation::AnimatedValueDef& value);

		// Draws zebra back with scroll offset
		void DrawZebraBack();

		// Returns object's parent
		UnknownPtr GetObjectParent(UnknownPtr object) override;

		// Returns object's children
		Vector<UnknownPtr> GetObjectChilds(UnknownPtr object) override;

		// Returns debugging string for object
		String GetObjectDebug(UnknownPtr object) override;

		// Sets nodeWidget data by object
		void FillNodeDataByObject(TreeNode* nodeWidget, UnknownPtr object) override;

		friend class AnimationTreeNode;
	};

	// -------------------
	// Animation tree node
	// -------------------
	class AnimationTreeNode : public TreeNode
	{
	public:
		// Default constructor
		AnimationTreeNode();

		// Copy-constructor
		AnimationTreeNode(const AnimationTreeNode& other);

		// Copy operator
		AnimationTreeNode& operator=(const AnimationTreeNode& other);


		// Sets object and updates content
		void Setup(AnimationTree::AnimationValueNode* node);

		SERIALIZABLE(AnimationTreeNode);

	protected:
		AnimationTree::AnimationValueNode* mData = nullptr;

		Text* mNameDrawable = nullptr; // Object name drawable

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called on deserialization, initializes controls
		void OnDeserialized(const DataNode& node) override;

		// initializes controls and widgets
		void InitializeControls();

		friend class SceneTree;
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
	PRIVATE_FIELD(mZebraBackLine).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::AnimationTree)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetAnimation, Animation*);
	PUBLIC_FUNCTION(Sprite*, GetZebraBackLine);
	PRIVATE_FUNCTION(void, RebuildAnimationTree);
	PRIVATE_FUNCTION(void, AddAnimatedValue, Animation::AnimatedValueDef&);
	PRIVATE_FUNCTION(void, DrawZebraBack);
	PRIVATE_FUNCTION(UnknownPtr, GetObjectParent, UnknownPtr);
	PRIVATE_FUNCTION(Vector<UnknownPtr>, GetObjectChilds, UnknownPtr);
	PRIVATE_FUNCTION(String, GetObjectDebug, UnknownPtr);
	PRIVATE_FUNCTION(void, FillNodeDataByObject, TreeNode*, UnknownPtr);
}
END_META;

CLASS_BASES_META(Editor::AnimationTreeNode)
{
	BASE_CLASS(o2::TreeNode);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationTreeNode)
{
	PROTECTED_FIELD(mData);
	PROTECTED_FIELD(mNameDrawable);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTreeNode)
{

	PUBLIC_FUNCTION(void, Setup, AnimationTree::AnimationValueNode*);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, InitializeControls);
}
END_META;
