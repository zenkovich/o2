#pragma once

#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2Editor/AnimationWindow/TrackControls/ITrackControl.h"

using namespace o2;

namespace o2
{
	class AnimationClip;
}

namespace Editor
{
	class AnimationWindow;
	class KeyHandlesSheet;

	// ---------------------
	// animation tracks tree
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
		void SetAnimation(AnimationClip* animation);

		// It is called when animation changed, checks count of animation tracks, updates tree structure if needed
		void OnAnimationChanged();

		// Sets width of tree part
		void SetTreeWidth(float width);

		// Sets color of track by path
		void SetAnimationValueColor(String path, const Color4& color);

		// Returns Animation track line number by world position, dependent on scroll
		float GetLineNumber(float worldPosition) const;

		// Returns world position of Animation track line
		float GetLineWorldPosition(float lineNumber) const;

		SERIALIZABLE(AnimationTree);

	public:
		struct TrackNode
		{
			String name;
			String path;

			Color4 color;

			IAnimationTrack*          track = nullptr;
			IAnimationTrack::IPlayer* player = nullptr;

			ITrackControl* trackControl = nullptr; 

			TrackNode*         parent = nullptr;
			Vector<TrackNode*> children;
		};

	private:
		AnimationWindow* mAnimationWindow = nullptr; // Animation window

		int mAnimationValuesCount = 0; // Last stored animation tracks count. Used in checking of animation changes for tracking new values

		TrackNode*   mRootValue = nullptr; // Root animation properties tree node
		ContextMenu* mContextMenu;         // Context menu

		float mTreeWidth = 100.0f; // Tree - part width

	private:
		// Initializes context menu
		void InitializeContext();

		// Rebuilds animation tracks tree - mRootValues
		void RebuildAnimationTree();

		// Adds Animation track to tree. Creates intermediate nodes when required
		void AddAnimationTrack(IAnimationTrack* track, IAnimationTrack::IPlayer* player = nullptr);

		//Updates tree node width
		void UpdateTreeWidth();

		// Sets curve view mode
		void SetCurveViewMode(bool enable);

		// Returns object's parent
		void* GetObjectParent(void* object) override;

		// Returns object's children
		Vector<void*> GetObjectChilds(void* object) override;

		// Returns debugging string for object
		String GetObjectDebug(void* object) override;

		// Sets nodeWidget data by object
		void FillNodeDataByObject(TreeNode* nodeWidget, void* object) override;

		// Free node data
		void FreeNodeData(TreeNode* nodeWidget, void* object) override;

		// Updates visible nodes (calculates range and initializes nodes), updates tree width on visible nodes
		void UpdateVisibleNodes() override;

		// It is called when tree node clicked by right button, opening context menu
		void OnNodeRBClick(TreeNode* node) override;

		// It is called when list of selected objects was changed
		void OnNodesSelectionChanged(Vector<void*> objects) override;

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
		void Setup(AnimationTree::TrackNode* node, AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet);

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

		AnimationTree::TrackNode* mData = nullptr; // Editing Animation track data

		AnimationTimeline* mTimeline = nullptr;     // Animation timeline pointer, passes into track controller
		KeyHandlesSheet*   mHandlesSheet = nullptr; // Handles sheet group, passes into track controller

		Text* mNameDrawable = nullptr; // Object name drawable

		ITrackControl* mTrackControl = nullptr;     // Animation track editor

		static Map<const Type*, Vector<ITrackControl*>> mTrackControlsCache; // Shared track controls cache

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called on deserialization, initializes controls
		void OnDeserialized(const DataValue& node) override;

		// initializes controls and widgets
		void InitializeControls();

		// Initializes suitable track control for Animation track by type. Caching track controls
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
	PRIVATE_FIELD(mAnimationWindow).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mAnimationValuesCount).DEFAULT_VALUE(0);
	PRIVATE_FIELD(mRootValue).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mContextMenu);
	PRIVATE_FIELD(mTreeWidth).DEFAULT_VALUE(100.0f);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTree)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetAnimation, AnimationClip*);
	PUBLIC_FUNCTION(void, OnAnimationChanged);
	PUBLIC_FUNCTION(void, SetTreeWidth, float);
	PUBLIC_FUNCTION(void, SetAnimationValueColor, String, const Color4&);
	PUBLIC_FUNCTION(float, GetLineNumber, float);
	PUBLIC_FUNCTION(float, GetLineWorldPosition, float);
	PRIVATE_FUNCTION(void, InitializeContext);
	PRIVATE_FUNCTION(void, RebuildAnimationTree);
	PRIVATE_FUNCTION(void, AddAnimationTrack, IAnimationTrack*, IAnimationTrack::IPlayer*);
	PRIVATE_FUNCTION(void, UpdateTreeWidth);
	PRIVATE_FUNCTION(void, SetCurveViewMode, bool);
	PRIVATE_FUNCTION(void*, GetObjectParent, void*);
	PRIVATE_FUNCTION(Vector<void*>, GetObjectChilds, void*);
	PRIVATE_FUNCTION(String, GetObjectDebug, void*);
	PRIVATE_FUNCTION(void, FillNodeDataByObject, TreeNode*, void*);
	PRIVATE_FUNCTION(void, FreeNodeData, TreeNode*, void*);
	PRIVATE_FUNCTION(void, UpdateVisibleNodes);
	PRIVATE_FUNCTION(void, OnNodeRBClick, TreeNode*);
	PRIVATE_FUNCTION(void, OnNodesSelectionChanged, Vector<void*>);
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
	PROTECTED_FIELD(mPropertyBorder).DEFAULT_VALUE(2.0f);
	PROTECTED_FIELD(mAddKeyButtonSize).DEFAULT_VALUE(25.0f);
	PROTECTED_FIELD(mPropertySize).DEFAULT_VALUE(130.0f);
	PROTECTED_FIELD(mData).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mTimeline).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mHandlesSheet).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mNameDrawable).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mTrackControl).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(Editor::AnimationTreeNode)
{

	PUBLIC_FUNCTION(void, Setup, AnimationTree::TrackNode*, AnimationTimeline*, KeyHandlesSheet*);
	PUBLIC_FUNCTION(void, Free);
	PUBLIC_FUNCTION(void, SetTreeWidth, float);
	PUBLIC_FUNCTION(void, OnDoubleClicked, const Input::Cursor&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, InitilizeTrackControl);
	PROTECTED_FUNCTION(void, FreeTrackControl);
	PROTECTED_FUNCTION(void, UpdateTrackControlView);
}
END_META;
