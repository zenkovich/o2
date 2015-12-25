#include "Tree.h"

#include "Animation/AnimatedFloat.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "UI/Button.h"

namespace o2
{
	UITreeNode::UITreeNode():
		UIWidget(), mExpandCoef(0)
	{}

	UITreeNode::UITreeNode(const UITreeNode& other) :
		UIWidget(other), mExpandCoef(0), mChildsOffset(other.mChildsOffset)
	{
		mExpandedState = GetStateObject("expanded");
		if (mExpandedState)
			mExpandedState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateExpanding);

		Ptr<UIButton> expandBtn = GetChild("expandBtn").Cast<UIButton>();
		if (expandBtn)
			expandBtn->onClick = [&]() { if (IsExpanded()) Collapse(); else Expand(); };

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UITreeNode::~UITreeNode()
	{}

	UITreeNode& UITreeNode::operator=(const UITreeNode& other)
	{
		UIWidget::operator=(other);

		mChildsOffset = other.mChildsOffset;

		mExpandedState = GetStateObject("expanded");
		if (mExpandedState)
			mExpandedState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateExpanding);

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UITreeNode::Draw()
	{
		if (mFullyDisabled)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		bool visibleChilds = mExpandCoef > FLT_EPSILON;
		bool clipping = mExpandCoef > FLT_EPSILON && mExpandCoef < 1.0f - FLT_EPSILON;

		if (clipping)
			o2Render.EnableScissorTest(layout.mAbsoluteRect);

		for (auto child : mChilds)
		{
			if (child->GetType() == *UITreeNode::type)
			{
				if (visibleChilds)
					child->Draw();
			}
			else child->Draw();
		}

		if (clipping)
			o2Render.DisableScissorTest();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
			DrawDebugFrame();
	}

	void UITreeNode::SetExpanded(bool expanded)
	{
		if (mExpandedState)
			mExpandedState->SetState(expanded);
	}

	bool UITreeNode::IsExpanded() const
	{
		if (mExpandedState)
			return mExpandedState->GetState();

		return false;
	}

	void UITreeNode::Expand()
	{
		if (mExpandedState)
			mExpandedState->SetState(true);
	}

	void UITreeNode::Collapse()
	{
		if (mExpandedState)
			mExpandedState->SetState(false);
	}

	void UITreeNode::ExpandAll()
	{
		Expand();

		for (auto child : mChilds)
		{
			if (child->GetType() != *UITreeNode::type)
				continue;

			Ptr<UITreeNode> childNode = child.Cast<UITreeNode>();
			childNode->ExpandAll();
		}
	}

	void UITreeNode::CollapseAll()
	{
		Collapse();

		for (auto child : mChilds)
		{
			if (child->GetType() != *UITreeNode::type)
				continue;

			Ptr<UITreeNode> childNode = child.Cast<UITreeNode>();
			childNode->CollapseAll();
		}
	}

	Ptr<UITreeNode> UITreeNode::GetNode(Ptr<char> object)
	{
		for (auto child : mChilds)
		{
			if (child->GetType() != *UITreeNode::type)
				continue;

			Ptr<UITreeNode> childNode = child.Cast<UITreeNode>();
			if (childNode->mObject == object)
				return childNode;

			auto res = childNode->GetNode(object);
			if (res)
				return res;
		}

		return nullptr;
	}

	void UITreeNode::Rebuild(bool withChilds /*= true*/)
	{
		if (!mTree)
			return;

		mTree->mSetupNodeFunc(this, mObject);

		if (!withChilds)
			return;

		Vector<Ptr<UnknownType>> objects = mTree->mGetChildsFunc(mObject);

		//check removed objects
		Vector<Ptr<UITreeNode>> removedNodes;
		Vector<Ptr<UITreeNode>> thisNodes;
		for (auto child : mChilds)
		{
			if (child->GetType() != *UITreeNode::type)
				continue;

			Ptr<UITreeNode> childNode = child.Cast<UITreeNode>();

			thisNodes.Add(childNode);

			if (!objects.Contains(childNode->mObject))
				removedNodes.Add(childNode);
		}

		for (auto node : removedNodes)
			RemoveChild(node);

		//check new objects and rebuild old
		for (auto obj : objects)
		{
			auto node = thisNodes.FindMatch([&](auto x) { return x->mObject == obj; });
			if (node)
			{
				node->Rebuild();
				continue;
			}

			Ptr<UITreeNode> newNode = mTree->mNodeSample->Clone();
			newNode->mObject = obj;
			newNode->mTree = mTree;
			AddChild(newNode);

			newNode->Rebuild();
		}

		Ptr<UIButton> expandBtn = GetChild("expandBtn").Cast<UIButton>();
		if (expandBtn)
			expandBtn->SetVisible(mChilds.ContainsPred([](auto x) { return x->GetType() == *UITreeNode::type; }));
	}

	void UITreeNode::SetChildrenOffset(float offset)
	{
		mChildsOffset = offset;
		UpdateLayout();
	}

	float UITreeNode::GetChildrenOffset() const
	{
		return mChildsOffset;
	}

	void UITreeNode::UpdateExpanding(float dt)
	{
		UpdateLayout();
	}

	void UITreeNode::UpdateLayout(bool forcible /*= false*/)
	{
		if (!forcible)
		{
			if (mParent)
				mParent->UpdateLayout();

			return;
		}

		float currentHeight = GetCurrentHeight();
		layout.mOffsetMin.y = layout.mOffsetMax.y - currentHeight;

		float childPos = layout.mMinSize.y;
		for (auto child : mChilds)
		{
			if (child->GetType() != *UITreeNode::type)
				continue;

			float height = child->layout.height;
			child->layout.mAnchorMin = Vec2F(0, 1);
			child->layout.mAnchorMax = Vec2F(1, 1);
			child->layout.mOffsetMin.x = mChildsOffset;
			child->layout.mOffsetMax.x = 0.0f;
			child->layout.mOffsetMax.y = -childPos;
			childPos += height;
			child->layout.mOffsetMin.y = -childPos;
		}

		RecalculateAbsRect();
		UpdateLayersLayouts();

		mChildsAbsRect = layout.mAbsoluteRect;

		for (auto child : mChilds)
			child->UpdateLayout(true);
	}

	float UITreeNode::GetCurrentHeight() const
	{
		float res = 0;

		for (auto child : mChilds)
		{
			if (child->GetType() != *UITreeNode::type)
				continue;

			res += child.Cast<UITreeNode>()->GetCurrentHeight();
		}

		res = res*mExpandCoef + layout.mMinSize.y;
		return res;
	}

	UITree::UITree():
		UIScrollArea(), mSelectedItem(-1)
	{
		mNodeSample = mnew UITreeNode();
		mNodeSample->layout.minHeight = 20;
		mNodeSample->AddLayer("caption", nullptr);

		mSelectionDrawable = mnew Sprite();

		mLayout = mnew UIVerticalLayout();
		AddChild(mLayout);

		mLayout->expandHeight = false;
		mLayout->expandWidth = true;
		mLayout->baseCorner = BaseCorner::LeftTop;
		mLayout->fitByChildren = true;
		mLayout->layout = UIWidgetLayout::Both();
	}

	UITree::UITree(const UITree& other):
		UIScrollArea(other), mSelectedItem(-1)
	{
		mNodeSample = other.mNodeSample->Clone();
		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mSelectionLayout = other.mSelectionLayout;
		mLayout = FindChild<UIVerticalLayout>();

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UITree::~UITree()
	{
		mNodeSample.Release();
		mSelectionDrawable.Release();
	}

	UITree& UITree::operator=(const UITree& other)
	{
		UIScrollArea::operator=(other);

		mNodeSample.Release();
		mSelectionDrawable.Release();

		mNodeSample = other.mNodeSample->Clone();
		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mSelectionLayout = other.mSelectionLayout;

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UITree::SetSourceFunctions(const Function<Vector<Ptr<UnknownType>>(Ptr<UnknownType>)> getChildsFunc,
									const Function<void(Ptr<UITreeNode>, Ptr<UnknownType>)> setupNodeFunc)
	{
		mGetChildsFunc = getChildsFunc;
		mSetupNodeFunc = setupNodeFunc;
	}

	void UITree::RebuildTree()
	{
		Vector<Ptr<UnknownType>> objects = mGetChildsFunc(nullptr);

		//check removed objects
		Vector<Ptr<UITreeNode>> removedNodes;
		Vector<Ptr<UITreeNode>> thisNodes;
		for (auto child : mLayout->mChilds)
		{
			if (child->GetType() != *UITreeNode::type)
				continue;

			Ptr<UITreeNode> childNode = child.Cast<UITreeNode>();

			thisNodes.Add(childNode);

			if (!objects.Contains(childNode->mObject))
				removedNodes.Add(childNode);
		}

		for (auto node : removedNodes)
			mLayout->RemoveChild(node);

		//check new objects and rebuild old
		for (auto obj : objects)
		{
			auto node = thisNodes.FindMatch([&](auto x) { return x->mObject == obj; });
			if (node)
			{
				node->Rebuild();
				continue;
			}

			Ptr<UITreeNode> newNode = mNodeSample->Clone();
			newNode->mObject = obj;
			newNode->mTree = this;
			mLayout->AddChild(newNode);

			newNode->Rebuild();
		}
	}

	void UITree::UpdateTreeNode(Ptr<UnknownType> object)
	{
		if (!object)
		{
			RebuildTree();
			return;
		}

		auto node = GetNode(object);
		if (!node)
			return;

		mSetupNodeFunc(node, object);
	}

	Ptr<UITreeNode> UITree::GetNode(Ptr<UnknownType> object)
	{
		for (auto child : mLayout->mChilds)
		{
			if (child->GetType() != *UITreeNode::type)
				continue;

			Ptr<UITreeNode> childNode = child.Cast<UITreeNode>();
			if (childNode->mObject == object)
				return childNode;

			auto res = childNode->GetNode(object);
			if (res)
				return res;
		}

		return nullptr;
	}

	void UITree::ExpandAll()
	{
		for (auto child : mLayout->mChilds)
		{
			if (child->GetType() != *UITreeNode::type)
				continue;

			Ptr<UITreeNode> childNode = child.Cast<UITreeNode>();
			childNode->ExpandAll();
		}
	}

	void UITree::CollapseAll()
	{
		for (auto child : mLayout->mChilds)
		{
			if (child->GetType() != *UITreeNode::type)
				continue;

			Ptr<UITreeNode> childNode = child.Cast<UITreeNode>();
			childNode->CollapseAll();
		}
	}

	Ptr<UIVerticalLayout> UITree::GetItemsLayout() const
	{
		return mLayout;
	}

	Ptr<UITreeNode> UITree::GetNodeSample() const
	{
		return mNodeSample;
	}

	Ptr<Sprite> UITree::GetSelectionDrawable() const
	{
		return mSelectionDrawable;
	}

	void UITree::SetSelectionDrawableLayout(const Layout& layout)
	{
		mSelectionLayout = layout;
	}

	Layout UITree::GetSelectionDrawableLayout() const
	{
		return mSelectionLayout;
	}

	bool UITree::IsUnderPoint(const Vec2F& point)
	{
		return layout.GetAbsoluteRect().IsInside(point);
	}

	float UITree::Depth()
	{
		return GetMaxDrawingDepth();
	}

	bool UITree::IsScrollable() const
	{
		return true;
	}
}