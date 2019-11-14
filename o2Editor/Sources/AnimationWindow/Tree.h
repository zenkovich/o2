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
	class AnimationWindow;
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

		// Draws widget
		void Draw() override;

		// Sets animation and updates tree structure
		void SetAnimation(Animation* animation);

		// It is called when animation changed, checks count of animation values, updates tree structure if needed
		void OnAnimationChanged();

		// Sets width of tree part
		void SetTreeWidth(float width);

		// Sets color of track by path
		void SetAnimationValueColor(String path, const Color4& color);

		// Returns animated value line number by world position, dependent on scroll
		float GetLineNumber(float worldPosition) const;

		// Returns world positionof animated value line
		float GetLineWorldPosition(float lineNumber) const;

		SERIALIZABLE(AnimationTree);

	public:
		struct AnimationValueNode
		{
			String name;
			String path;

			Color4 color;

			IAnimatedValue* animatedValue = nullptr;

			ITrackControl* trackControl = nullptr; 

			AnimationValueNode* parent = nullptr;
			Vector<AnimationValueNode*> children;
		};

	private:
		AnimationWindow* mAnimationWindow = nullptr; // Animation window

		int mAnimationValuesCount = 0; // Last stored animation values count. Used in checking of animation changes for tracking new values

		AnimationValueNode* mRootValue = nullptr;       // Root animation properties tree node
		ContextMenu*        mContextMenu;               // Context menu

		float mTreeWidth = 100.0f; // Tree - part width

	private:
		// Initializes context menu
		void InitializeContext();

		// Rebuilds animation values tree - mRootValues
		void RebuildAnimationTree();

		// Adds animated value to tree. Creates intermediate nodes when required
		void AddAnimatedValue(Animation::AnimatedValueDef& value);

		//Updates tree node width
		void UpdateTreeWidth();

		// Sets curve view mode
		void SetCurveViewMode(bool enable);

		// Returns object's parent
		UnknownPtr GetObjectParent(UnknownPtr object) override;

		// Returns object's children
		Vector<UnknownPtr> GetObjectChilds(UnknownPtr object) override;

		// Returns debugging string for object
		String GetObjectDebug(UnknownPtr object) override;

		// Sets nodeWidget data by object
		void FillNodeDataByObject(TreeNode* nodeWidget, UnknownPtr object) override;

		// Free node data
		void FreeNodeData(TreeNode* nodeWidget, UnknownPtr object) override;

		// Updates visible nodes (calculates range and initializes nodes), updates tree width on visible nodes
		void UpdateVisibleNodes() override;

		// It is called when tree node clicked by right button, opening context menu
		void OnNodeRBClick(TreeNode* node) override;

		// It is called when list of selected objects was changed
		void OnNodesSelectionChanged(UnknownPtrsVec objects) override;

		// Gets tree node from pool or creates new, in editor scope
		TreeNode* CreateTreeNodeWidget();

		// Removes selected property
		void OnDeletePropertyPressed();

		friend class AnimationTreeNode;
		friend class AnimationWindow;
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

		// Sets object and updates track control
		void Setup(AnimationTree::AnimationValueNode* node, AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet);

		// Free node, unregister track control
		void Free();

		// Sets width of tree part and control part
		void SetTreeWidth(float width);

		// it is called from handles sheet, when user double clicked, creates new key under cursor
		void OnDoubleClicked(const Input::Cursor& cursor);

		SERIALIZABLE(AnimationTreeNode);

	protected:
		float mPropertyBorder = 2.0f;
		float mAddKeyButtonSize = 25.0f;
		float mPropertySize = 130.0f;

		AnimationTree::AnimationValueNode* mData = nullptr; // Editing animated value data

		AnimationTimeline* mTimeline = nullptr;     // Animation timeline pointer, passes into track controller
		KeyHandlesSheet*   mHandlesSheet = nullptr; // Handles sheet group, passes into track controller

		Text* mNameDrawable = nullptr; // Object name drawable

		ITrackControl* mTrackControl = nullptr;     // Animated value editor

		static Map<const Type*, Vector<ITrackControl*>> mTrackControlsCache; // Shared track controls cache

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called on deserialization, initializes controls
		void OnDeserialized(const DataNode& node) override;

		// initializes controls and widgets
		void InitializeControls();

		// Initializes suitable track control for animated value by type. Caching track controls
		void InitilizeTrackControl();

		// Free track control and put it unto buffer
		void FreeTrackControl();

		// Updates drag handles positions on timeline
		void UpdateTrackControlView();

		friend class AnimationTree;
	};
}

CLASS_BASES_META(Editor::AnimationTree)
{
	BASE_CLASS(o2::Tree);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationTree)
{
	PRIVATE_FIELD(mAnimationWindow);
	PRIVATE_FIELD(mAnimationValuesCount);
	PRIVATE_FIELD(mRootValue);
	PRIVATE_FIELD(mContextMenu);
	PRIVATE_FIELD(mTreeWidth);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTree)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetAnimation, Animation*);
	PUBLIC_FUNCTION(void, OnAnimationChanged);
	PUBLIC_FUNCTION(void, SetTreeWidth, float);
	PUBLIC_FUNCTION(void, SetAnimationValueColor, String, const Color4&);
	PUBLIC_FUNCTION(float, GetLineNumber, float);
	PUBLIC_FUNCTION(float, GetLineWorldPosition, float);
	PRIVATE_FUNCTION(void, InitializeContext);
	PRIVATE_FUNCTION(void, RebuildAnimationTree);
	PRIVATE_FUNCTION(void, AddAnimatedValue, Animation::AnimatedValueDef&);
	PRIVATE_FUNCTION(void, UpdateTreeWidth);
	PRIVATE_FUNCTION(void, SetCurveViewMode, bool);
	PRIVATE_FUNCTION(UnknownPtr, GetObjectParent, UnknownPtr);
	PRIVATE_FUNCTION(Vector<UnknownPtr>, GetObjectChilds, UnknownPtr);
	PRIVATE_FUNCTION(String, GetObjectDebug, UnknownPtr);
	PRIVATE_FUNCTION(void, FillNodeDataByObject, TreeNode*, UnknownPtr);
	PRIVATE_FUNCTION(void, FreeNodeData, TreeNode*, UnknownPtr);
	PRIVATE_FUNCTION(void, UpdateVisibleNodes);
	PRIVATE_FUNCTION(void, OnNodeRBClick, TreeNode*);
	PRIVATE_FUNCTION(void, OnNodesSelectionChanged, UnknownPtrsVec);
	PRIVATE_FUNCTION(TreeNode*, CreateTreeNodeWidget);
	PRIVATE_FUNCTION(void, OnDeletePropertyPressed);
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
	PROTECTED_FIELD(mTrackControl);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTreeNode)
{

	PUBLIC_FUNCTION(void, Setup, AnimationTree::AnimationValueNode*, AnimationTimeline*, KeyHandlesSheet*);
	PUBLIC_FUNCTION(void, Free);
	PUBLIC_FUNCTION(void, SetTreeWidth, float);
	PUBLIC_FUNCTION(void, OnDoubleClicked, const Input::Cursor&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, InitilizeTrackControl);
	PROTECTED_FUNCTION(void, FreeTrackControl);
	PROTECTED_FUNCTION(void, UpdateTrackControlView);
}
END_META;
