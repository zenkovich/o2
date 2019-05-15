#pragma once

#include "AnimationWindow/TrackControls/ITrackControl.h"
#include "Scene/UI/Widgets/Tree.h"

using namespace o2;

namespace o2
{
	class Animation;
}

namespace Editor
{
	class KeyHandlesSheet;

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

		// Initializes tree with timeline and handles sheet
		void Initialize(AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet);

		// Draws widget
		void Draw() override;

		// Sets animation and updates tree structure
		void SetAnimation(Animation* animation);

		// Sets width of tree part
		void SetTreeWidth(float width);

		// Return zebra back line sprite
		Sprite* GetZebraBackLine() const;

		// Returns animated value line number by world position, dependent on scroll
		float GetLineNumber(float worldPosition) const;

		// Returns world positionof animated value line
		float GetLineWorldPosition(float lineNumber) const;

		SERIALIZABLE(AnimationTree);

	public:
		struct AnimationValueNode
		{
			String name;
			IAnimatedValue* animatedValue = nullptr;

			ITrackControl* trackControl = nullptr; 

			AnimationValueNode* parent = nullptr;
			Vector<AnimationValueNode*> children;
		};

	private:
		Animation*          mAnimation = nullptr;
		AnimationValueNode* mRootValue = nullptr;
		AnimationTimeline*  mTimeline = nullptr;
		KeyHandlesSheet*    mHandlesSheet = nullptr;

		Sprite* mZebraBackLine = nullptr; // Dark zebra line sprite @SERIALIZABLE

		float mTreeWidth = 100.0f; // Tree - part width

	private:
		// Rebuilds animation values tree - mRootValues
		void RebuildAnimationTree();

		// Adds animated value to tree. Creates intermediate nodes when required
		void AddAnimatedValue(Animation::AnimatedValueDef& value);

		// Draws zebra back with scroll offset
		void DrawZebraBack();

		//Updates tree node width
		void UpdateTreeWidth();

		// Returns object's parent
		UnknownPtr GetObjectParent(UnknownPtr object) override;

		// Returns object's children
		Vector<UnknownPtr> GetObjectChilds(UnknownPtr object) override;

		// Returns debugging string for object
		String GetObjectDebug(UnknownPtr object) override;

		// Sets nodeWidget data by object
		void FillNodeDataByObject(TreeNode* nodeWidget, UnknownPtr object) override;

		// Updates visible nodes (calculates range and initializes nodes), updates tree width on visible nodes
		void UpdateVisibleNodes() override;

		// Gets tree node from pool or creates new, in editor scope
		TreeNode* CreateTreeNodeWidget();

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
		void Setup(AnimationTree::AnimationValueNode* node, AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet);


		// Sets width of tree part and control part
		void SetTreeWidth(float width);

		SERIALIZABLE(AnimationTreeNode);

	protected:
		float mPropertyBorder = 2.0f;
		float mAddKeyButtonSize = 20.0f;
		float mPropertySize = 60.0f;

		AnimationTree::AnimationValueNode* mData = nullptr; // Editing animated value data

		AnimationTimeline* mTimeline = nullptr;     // Animation timeline pointer, passes into track controller
		KeyHandlesSheet*   mHandlesSheet = nullptr; // Handles sheet group, passes into track controller

		Text*   mNameDrawable = nullptr; // Object name drawable
		Button* mAddKeyButton = nullptr; // Add key button, enables when property field from track controller is available

		ITrackControl* mTrackControl = nullptr; // Animated value editor

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called on deserialization, initializes controls
		void OnDeserialized(const DataNode& node) override;

		// initializes controls and widgets
		void InitializeControls();

		// Initializes suitable track control for animated value by type. Caching track controls
		void InitilizeTrackControl();

		// Updates drag handles positions on timeline
		void UpdateTrackControlView();

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
	PRIVATE_FIELD(mRootValue);
	PRIVATE_FIELD(mTimeline);
	PRIVATE_FIELD(mHandlesSheet);
	PRIVATE_FIELD(mZebraBackLine).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mTreeWidth);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTree)
{

	PUBLIC_FUNCTION(void, Initialize, AnimationTimeline*, KeyHandlesSheet*);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetAnimation, Animation*);
	PUBLIC_FUNCTION(void, SetTreeWidth, float);
	PUBLIC_FUNCTION(Sprite*, GetZebraBackLine);
	PUBLIC_FUNCTION(float, GetLineNumber, float);
	PUBLIC_FUNCTION(float, GetLineWorldPosition, float);
	PRIVATE_FUNCTION(void, RebuildAnimationTree);
	PRIVATE_FUNCTION(void, AddAnimatedValue, Animation::AnimatedValueDef&);
	PRIVATE_FUNCTION(void, DrawZebraBack);
	PRIVATE_FUNCTION(void, UpdateTreeWidth);
	PRIVATE_FUNCTION(UnknownPtr, GetObjectParent, UnknownPtr);
	PRIVATE_FUNCTION(Vector<UnknownPtr>, GetObjectChilds, UnknownPtr);
	PRIVATE_FUNCTION(String, GetObjectDebug, UnknownPtr);
	PRIVATE_FUNCTION(void, FillNodeDataByObject, TreeNode*, UnknownPtr);
	PRIVATE_FUNCTION(void, UpdateVisibleNodes);
	PRIVATE_FUNCTION(TreeNode*, CreateTreeNodeWidget);
}
END_META;

CLASS_BASES_META(Editor::AnimationTreeNode)
{
	BASE_CLASS(o2::TreeNode);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationTreeNode)
{
	PROTECTED_FIELD(mPropertyBorder);
	PROTECTED_FIELD(mAddKeyButtonSize);
	PROTECTED_FIELD(mPropertySize);
	PROTECTED_FIELD(mData);
	PROTECTED_FIELD(mTimeline);
	PROTECTED_FIELD(mHandlesSheet);
	PROTECTED_FIELD(mNameDrawable);
	PROTECTED_FIELD(mAddKeyButton);
	PROTECTED_FIELD(mTrackControl);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTreeNode)
{

	PUBLIC_FUNCTION(void, Setup, AnimationTree::AnimationValueNode*, AnimationTimeline*, KeyHandlesSheet*);
	PUBLIC_FUNCTION(void, SetTreeWidth, float);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, InitilizeTrackControl);
	PROTECTED_FUNCTION(void, UpdateTrackControlView);
}
END_META;
