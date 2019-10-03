#include "stdafx.h"

#include "AnimationWindow/AnimationWindow.h"
#include "Core/EditorScope.h"
#include "PropertiesListDlg.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/Button.h"
#include "TrackControls/KeyFramesTrackControl.h"
#include "TrackControls/MapKeyFramesTrackControl.h"
#include "Tree.h"

namespace Editor
{
	AnimationTree::AnimationTree() :
		Tree()
	{
		SetRearrangeType(Tree::RearrangeType::Disabled);
		InitializeContext();
	}

	AnimationTree::AnimationTree(const AnimationTree& other) :
		Tree(other)
	{
		SetRearrangeType(Tree::RearrangeType::Disabled);
		InitializeContext();
	}

	AnimationTree::~AnimationTree()
	{
		if (mRootValue)
			delete mRootValue;
	}

	AnimationTree& AnimationTree::operator=(const AnimationTree& other)
	{
		delete mZebraBackLine;

		Tree::operator=(other);

		mZebraBackLine = other.mZebraBackLine->CloneAs<Sprite>();
		InitializeContext();

		return *this;
	}

	void AnimationTree::Draw()
	{
		o2Render.EnableScissorTest(mAbsoluteClipArea);
		DrawZebraBack();
		o2Render.DisableScissorTest();

		mAnimationWindow->mHandlesSheet->Draw();

		// Disable zebra
		auto buf = mZebraBackLine;
		mZebraBackLine = nullptr;

		Tree::Draw();

		// Enable
		mZebraBackLine = buf;

		mAnimationWindow->mHandlesSheet->UpdateInputDrawOrder();
	}

	void AnimationTree::SetAnimation(Animation* animation)
	{
		mAnimationWindow->mHandlesSheet->UnregAllTrackControls();

		RebuildAnimationTree();
		ExpandAll();
		OnObjectsChanged({ (UnknownPtr)mRootValue });
	}

	void AnimationTree::SetTreeWidth(float width)
	{
		mTreeWidth = width;
		UpdateTreeWidth();
	}

	float AnimationTree::GetLineNumber(float worldPosition) const
	{
		return (layout->GetWorldTop() - worldPosition + mScrollPos.y) / mNodeWidgetSample->layout->GetMinimalHeight();
	}

	float AnimationTree::GetLineWorldPosition(float lineNumber) const
	{
		return -(lineNumber * mNodeWidgetSample->layout->GetMinimalHeight() - mScrollPos.y - layout->GetWorldTop());
	}

	void AnimationTree::InitializeContext()
	{
		mContextMenu = o2UI.CreateWidget<ContextMenu>();

		mContextMenu->AddItem("New", [&]() { });
		mContextMenu->AddItem("Save", [&]() { }, ImageAssetRef(), ShortcutKeys('S', true));
		mContextMenu->AddItem("Save as...", [&]() { }, ImageAssetRef(), ShortcutKeys('S', true, true));
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Edit properties", [&]() { PropertiesListDlg::Show(mAnimationWindow->mAnimation, mAnimationWindow->mTargetActor); });

		onFocused = [&]() { mContextMenu->SetItemsMaxPriority(); };
		onUnfocused = [&]() { mContextMenu->SetItemsMinPriority(); };

		AddInternalWidget(mContextMenu);
	}

	void AnimationTree::RebuildAnimationTree()
	{
		if (mRootValue)
			delete mRootValue;

		mRootValue = nullptr;

		if (!mAnimationWindow->mAnimation)
			return;

		mRootValue = mnew AnimationValueNode();
		mRootValue->name = "Track name";

		for (auto& value : mAnimationWindow->mAnimation->GetAnimationsValues())
			AddAnimatedValue(value);
	}

	void AnimationTree::AddAnimatedValue(Animation::AnimatedValueDef& value)
	{
		AnimationValueNode* current = nullptr;

		int lastDel = 0;
		while (lastDel >= 0)
		{
			int del = value.targetPath.Find('/', lastDel);
			String subPath = value.targetPath.SubStr(lastDel, del);
			AnimationValueNode* next = (current ? current->children : mRootValue->children)
				.FindMatch([&](AnimationValueNode* x) { return x->name == subPath; });

			if (!next)
			{
				next = mnew AnimationValueNode();
				next->name = subPath;
				next->path = value.targetPath;

				if (current)
				{
					next->parent = current;
					current->children.Add(next);
				}
				else mRootValue->children.Add(next);
			}

			current = next;
			lastDel = del >= 0 ? del + 1 : -1;
		}

		current->animatedValue = value.animatedValue;
	}

#undef DrawText

	void AnimationTree::UpdateTreeWidth()
	{
		for (auto node : mVisibleNodes)
		{
			if (auto trackNode = dynamic_cast<AnimationTreeNode*>(node->widget))
				trackNode->SetTreeWidth(mTreeWidth);
		}
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
			return { (UnknownPtr)mRootValue };

		auto treeNode = (AnimationValueNode*)object;
		return treeNode->children.Cast<UnknownPtr>();
	}

	String AnimationTree::GetObjectDebug(UnknownPtr object)
	{
		auto treeNode = (AnimationValueNode*)object;
		return treeNode ? treeNode->name : "Empty";
	}

	void AnimationTree::FillNodeDataByObject(TreeNode* nodeWidget, UnknownPtr object)
	{
		AnimationTreeNode* node = dynamic_cast<AnimationTreeNode*>(nodeWidget);
		node->Setup((AnimationValueNode*)object, mAnimationWindow->mTimeline, mAnimationWindow->mHandlesSheet);
	}

	void AnimationTree::FreeNodeData(TreeNode* nodeWidget, UnknownPtr object)
	{
		AnimationTreeNode* node = dynamic_cast<AnimationTreeNode*>(nodeWidget);
		node->Free();
	}

	void AnimationTree::UpdateVisibleNodes()
	{
		Tree::UpdateVisibleNodes();
		UpdateTreeWidth();
	}

	void AnimationTree::OnNodeRBClick(TreeNode* node)
	{
		o2UI.FocusWidget(this);
		mContextMenu->Show();
	}

	TreeNode* AnimationTree::CreateTreeNodeWidget()
	{
		PushScopeEnterOnStack scope;
		return Tree::CreateTreeNodeWidget();
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

	void AnimationTreeNode::Setup(AnimationTree::AnimationValueNode* node, AnimationTimeline* timeline, KeyHandlesSheet* handlesSheet)
	{
		mTimeline = timeline;
		mHandlesSheet = handlesSheet;

		mTimeline->onViewChanged -= THIS_FUNC(UpdateTrackControlView);
		mTimeline->onViewChanged += THIS_FUNC(UpdateTrackControlView);

		mData = node;
		mNameDrawable->text = node->name;

		InitilizeTrackControl();

		mData->trackControl = mTrackControl;
	}

	void AnimationTreeNode::Free()
	{
		FreeTrackControl();
	}

	void AnimationTreeNode::SetTreeWidth(float width)
	{
		if (mTrackControl)
		{
			*mTrackControl->layout = WidgetLayout::BothStretch(width, 0, 0, 0);

			float right = width - layout->GetOffsetLeft();
			if (auto addKeyButton = mTrackControl->GetAddKeyButton()) 
			{
				*addKeyButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(mAddKeyButtonSize, mAddKeyButtonSize),
															Vec2F(right - mAddKeyButtonSize, 0.0f));
			}

			if (auto prop = mTrackControl->GetPropertyField())
			{
				*prop->layout = WidgetLayout::VerStretch(HorAlign::Left, mPropertyBorder, mPropertyBorder, mPropertySize,
														 right - mPropertySize - mPropertyBorder - mAddKeyButtonSize);
			}
		}
	}

	void AnimationTreeNode::OnDoubleClicked(const Input::Cursor& cursor)
	{
		if (mTrackControl)
			mTrackControl->InsertNewKey(mTimeline->WorldToLocal(cursor.position.x));
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

	Dictionary<const Type*, o2::Vector<ITrackControl*>> AnimationTreeNode::mTrackControlsCache;

	void AnimationTreeNode::InitilizeTrackControl()
	{
		PushScopeEnterOnStack scope;

		static Dictionary<const Type*, const Type*> animatedValueToControlTrackTypes =
		{
			{ &TypeOf(AnimatedValue<float>), &TypeOf(KeyFramesTrackControl<AnimatedValue<float>>) },
			{ &TypeOf(AnimatedValue<bool>), &TypeOf(KeyFramesTrackControl<AnimatedValue<bool>>) },
			{ &TypeOf(AnimatedValue<Vec2F>), &TypeOf(KeyFramesTrackControl<AnimatedValue<Vec2F>>) },
			{ &TypeOf(AnimatedValue<Color4>), &TypeOf(KeyFramesTrackControl<AnimatedValue<Color4>>) }
		};

		FreeTrackControl();

		if (!mData->animatedValue)
		{
			MapKeyFramesTrackControl* trackControl = nullptr;
			auto trackControlType = &TypeOf(MapKeyFramesTrackControl);
			if (mTrackControlsCache.ContainsKey(trackControlType) && !mTrackControlsCache[trackControlType].IsEmpty())
				trackControl = dynamic_cast<MapKeyFramesTrackControl*>(mTrackControlsCache[trackControlType].PopBack());
			else
				trackControl = mnew MapKeyFramesTrackControl();

			mTrackControl = trackControl;

			trackControl->Initialize(mTimeline, mHandlesSheet);
			trackControl->SetMappedTracks(*mData);

			AddChild(mTrackControl);
		}
		else
		{
			auto animatedValueType = &mData->animatedValue->GetType();
			if (!animatedValueToControlTrackTypes.ContainsKey(animatedValueType))
			{
				o2Debug.LogWarning("Can't create control track for type:" + animatedValueType->GetName());
				return;
			}

			auto trackControlType = dynamic_cast<const ObjectType*>(animatedValueToControlTrackTypes[animatedValueType]);
			if (mTrackControlsCache.ContainsKey(trackControlType) && !mTrackControlsCache[trackControlType].IsEmpty())
				mTrackControl = mTrackControlsCache[trackControlType].PopBack();
			else
				mTrackControl = dynamic_cast<ITrackControl*>(trackControlType->DynamicCastToIObject(trackControlType->CreateSample()));

			mTrackControl->Initialize(mTimeline, mHandlesSheet);
			mTrackControl->SetAnimatedValue(mData->animatedValue, mData->path);

			AddChild(mTrackControl);
		}

		if (auto prop = mTrackControl->GetPropertyField())
			AddChild(prop);

		if (auto addKey = mTrackControl->GetAddKeyButton())
			AddChild(addKey);

		mHandlesSheet->RegTrackControl(mTrackControl, mData->path);
	}

	void AnimationTreeNode::FreeTrackControl()
	{
		if (mTrackControl)
		{
			auto trackType = &mTrackControl->GetType();
			if (!mTrackControlsCache.ContainsKey(trackType))
				mTrackControlsCache.Add(trackType, Vector<ITrackControl*>());

			mTrackControlsCache[trackType].Add(mTrackControl);

			RemoveChild(mTrackControl, false);

			if (auto prop = mTrackControl->GetPropertyField())
				RemoveChild(prop, false);

			if (auto addKey = mTrackControl->GetAddKeyButton())
				RemoveChild(addKey, false);

			mHandlesSheet->UnregTrackControl(mTrackControl);
		}

		mTrackControl = nullptr;
	}

	void AnimationTreeNode::UpdateTrackControlView()
	{
		if (mTrackControl)
			mTrackControl->UpdateHandles();
	}

}

DECLARE_CLASS(Editor::AnimationTree);

DECLARE_CLASS(Editor::AnimationTreeNode);
