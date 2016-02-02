#include "Tree.h"

#include "Animation/AnimatedFloat.h"
#include "Application/Input.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "UI/Button.h"
#include "UI/UIManager.h"

namespace o2
{
	UITreeNode::UITreeNode():
		UIWidget(), mExpandCoef(0), mDragSizeCoef(1.0f), mInsertSizeCoef(0.0f), mExpandedState(nullptr), mTree(nullptr), 
		mObject(nullptr)
	{
		AddState("visible", Animation::EaseInOut<float>(this, &transparency, 0.0f, 1.0f, 0.1f));
		AddState("inserting", Animation::EaseInOut<float>(this, &mInsertSizeCoef, 0.0f, 1.0f, 0.1f));

		Animation dragStateAnim = Animation::EaseInOut<float>(this, &mDragSizeCoef, 1.0f, 0.0f, 0.1f);
		*dragStateAnim.AddAnimationValue(&transparency) = AnimatedValue<float>::EaseInOut(1.0f, 0.0f, 0.01f);
		AddState("dragging", dragStateAnim);
	}

	UITreeNode::UITreeNode(const UITreeNode& other):
		UIWidget(other), mExpandCoef(0), mChildsOffset(other.mChildsOffset), mDragSizeCoef(1.0f), mInsertSizeCoef(0.0f)
	{
		mExpandedState = GetStateObject("expanded");
		if (mExpandedState)
		{
			mExpandedState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);
			mExpandedState->onStateFullyFalse += Function<void()>(this, &UITreeNode::RemoveChildrenNodes);
		}

		if (auto draggingState = GetStateObject("dragging"))
			draggingState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);

		if (auto insertingState = GetStateObject("inserting"))
			insertingState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);

		UIButton* expandBtn = (UIButton*)GetChild("expandBtn");
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
		{
			mExpandedState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);
			mExpandedState->onStateFullyFalse += Function<void()>(this, &UITreeNode::RemoveChildrenNodes);
		}

		if (auto draggingState = GetStateObject("dragging"))
			draggingState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);

		if (auto insertingState = GetStateObject("inserting"))
			insertingState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UITreeNode::Draw()
	{
		if (!o2Render.IsScissorTestEnabled() || layout.mAbsoluteRect.IsIntersects((RectF)o2Render.GetScissorRect()))
		{
			for (auto layer : mDrawingLayers)
				layer->Draw();
		}

		bool visibleChilds = mExpandCoef > FLT_EPSILON;
		bool clipping = mExpandCoef > FLT_EPSILON && mExpandCoef < 1.0f - FLT_EPSILON;

		if (clipping)
			o2Render.EnableScissorTest(RectF(-o2Render.GetResolution().x*0.5f, layout.mAbsoluteRect.top - GetCurrentHeight(),
											 o2Render.GetResolution().x*0.5f, layout.mAbsoluteRect.top));

		for (auto child : mChilds)
		{
			if (child->GetType() == UITreeNode::type)
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

	void UITreeNode::SetExpanded(bool expanded, bool forcible /*= false*/)
	{
		if (expanded == IsExpanded())
			return;

		if (expanded)
		{
			if (!mTree->mExpandedObjects.Contains(mObject))
				mTree->mExpandedObjects.Add(mObject);
		}
		else mTree->mExpandedObjects.Remove(mObject);

		if (expanded)
			RebuildChildrenNodes(mTree->getChildsFunc(mObject));

		if (mExpandedState)
		{
			if (forcible)
				mExpandedState->SetStateForcible(expanded);
			else
				mExpandedState->SetState(expanded);
		}
	}

	bool UITreeNode::IsExpanded() const
	{
		if (mExpandedState)
			return mExpandedState->GetState();

		return false;
	}

	void UITreeNode::Expand(bool forcible /*= false*/)
	{
		SetExpanded(true, forcible);
	}

	void UITreeNode::Collapse(bool forcible /*= false*/)
	{
		SetExpanded(false, forcible);
	}

	void UITreeNode::ExpandAll()
	{
		Expand();

		for (auto child : mChilds)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			UITreeNode* childNode = (UITreeNode*)child;
			childNode->ExpandAll();
		}
	}

	void UITreeNode::CollapseAll()
	{
		Collapse();

		for (auto child : mChilds)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			UITreeNode* childNode = (UITreeNode*)child;
			childNode->CollapseAll();
		}
	}

	UITreeNode* UITreeNode::GetNode(UnknownType* object)
	{
		for (auto child : mChilds)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			UITreeNode* childNode = (UITreeNode*)child;
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

		Vector<UnknownType*> objects = mTree->getChildsFunc(mObject);

		mTree->setupNodeFunc(this, mObject);

		UIButton* expandBtn = (UIButton*)GetChild("expandBtn");
		if (expandBtn)
			expandBtn->SetVisible(objects.Count() > 0);

		if (!withChilds || !IsExpanded())
			return;

		RebuildChildrenNodes(objects);
	}

	void UITreeNode::RebuildChildrenNodes(Vector<UnknownType*> objects)
	{
		//check removed objects
		Vector<UITreeNode*> removedNodes;
		Vector<UITreeNode*> thisNodes;
		for (auto child : mChilds)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			UITreeNode* childNode = (UITreeNode*)child;

			thisNodes.Add(childNode);

			if (!objects.Contains(childNode->mObject))
				removedNodes.Add(childNode);
		}

		for (auto node : removedNodes)
		{
			node->RemoveChildrenNodes();
			mTree->mAllNodes.Remove(node);
			RemoveChild(node, false);
			mTree->FreeTreeNode(node);
		}

		//check new objects and rebuild old
		for (auto obj : objects)
		{
			auto node = thisNodes.FindMatch([&](auto x) { return x->mObject == obj; });
			if (node)
			{
				node->Rebuild();
				continue;
			}

			UITreeNode* newNode = mTree->CreateTreeNode();
			newNode->mObject = obj;
			newNode->mTree = mTree;
			newNode->mParent = this;
			newNode->SetExpanded(mTree->mExpandedObjects.Contains(obj), true);
			mChilds.Add(newNode);

			mTree->mAllNodes.Add(newNode);

			newNode->Rebuild();
		}

		if (!mTree->mWaitSelectionsUpdate)
			mTree->CheckSelectedNodes();

		// rearrange nodes by objects arranging
		auto childs = mChilds;
		mChilds.RemoveAll([](auto x) { return x->GetType() == UITreeNode::type; });

		for (auto obj : objects)
		{
			auto node = childs.FindMatch([&](auto x) {
				return x->GetType() == UITreeNode::type && ((UITreeNode*)x)->mObject == obj;
			});

			mChilds.Add(node);
		}

		mTree->mNeedUpdateLayout = true;
	}

	void UITreeNode::RemoveChildrenNodes()
	{
		auto childs = mChilds;
		for (auto child : childs)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			UITreeNode* childNode = (UITreeNode*)child;
			childNode->RemoveChildrenNodes();

			mTree->mAllNodes.Remove(childNode);
			mChilds.Remove(child);
			mTree->FreeTreeNode(childNode);
		}

		if (!mTree->mWaitSelectionsUpdate)
			mTree->CheckSelectedNodes();
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

	UnknownType* UITreeNode::GetObject() const
	{
		return mObject;
	}

	void UITreeNode::UpdateTreeLayout(float dt)
	{
		mTree->mNeedUpdateLayout = true;
	}

	void UITreeNode::UpdateLayout(bool forcible /*= false*/)
	{
		if (!forcible)
		{
			if (mParent)
				mParent->UpdateLayout();

			return;
		}

		float height = layout.mMinSize.y*mDragSizeCoef;
		layout.mOffsetMin.y = layout.mOffsetMax.y - height;

		float childPos = height;
		for (auto child : mChilds)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			auto node = (UITreeNode*)child;

			child->layout.mOffsetMin.x = mChildsOffset;
			child->layout.mOffsetMax.y = -childPos;

			childPos += node->GetCurrentHeight();
			child->layout.mOffsetMin.y = -childPos;
		}

		Vec2F parentSize = mParent->mChildsAbsRect.Size(), parentPos = mParent->mChildsAbsRect.LeftBottom();
		float insertOffs = layout.mMinSize.y*mInsertSizeCoef;

		layout.mLocalRect.left = layout.mOffsetMin.x;
		layout.mLocalRect.right = parentSize.x;
		layout.mLocalRect.bottom = parentSize.y + layout.mOffsetMin.y;
		layout.mLocalRect.top = parentSize.y + layout.mOffsetMax.y;
		layout.mAbsoluteRect = layout.mLocalRect + parentPos - Vec2F(0, insertOffs);

		UpdateLayersLayouts();

		mChildsAbsRect = layout.mAbsoluteRect;

		for (auto child : mChilds)
		{
			bool isTreeNode = child->GetType() == UITreeNode::type;
			if (isTreeNode)
			{
				mChildsAbsRect.top += insertOffs;
				mChildsAbsRect.bottom += insertOffs;
			}

			child->UpdateLayout(true);

			if (isTreeNode)
			{
				mChildsAbsRect.top -= insertOffs;
				mChildsAbsRect.bottom -= insertOffs;
			}
		}
	}

	float UITreeNode::GetCurrentHeight() const
	{
		float res = 0;

		for (auto child : mChilds)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			res += ((UITreeNode*)child)->GetCurrentHeight();
		}

		res = (res*mExpandCoef + layout.mMinSize.y)*mDragSizeCoef;
		return res;
	}

	UITree::UITree():
		UIScrollArea(), mWaitSelectionsUpdate(false), mDraggingNodes(false), mNeedUpdateLayout(false),
		mExpandInsertTime(-1), mPressedTime(10.0f), mHoveredItem(nullptr), mPressedNode(nullptr), mInsertNodeCandidate(nullptr),
		mUnderCursorItem(nullptr), mExpandNodeCandidate(nullptr)
	{
		mNodeSample = mnew UITreeNode();
		mNodeSample->layout.minHeight = 20;
		mNodeSample->AddLayer("caption", nullptr);

		mDragNode = mNodeSample->Clone();

		mHoverDrawable = mnew Sprite();
		mSelectedDrawable = mnew Sprite();
		mDragNodeBack = mnew Sprite();
	}

	UITree::UITree(const UITree& other):
		UIScrollArea(other), mWaitSelectionsUpdate(false), mDraggingNodes(false), mNeedUpdateLayout(false),
		mExpandInsertTime(-1), mPressedTime(10.0f), mHoveredItem(nullptr), mPressedNode(nullptr), mInsertNodeCandidate(nullptr),
		mUnderCursorItem(nullptr), mExpandNodeCandidate(nullptr)
	{
		mNodeSample = other.mNodeSample->Clone();
		mHoverDrawable = other.mHoverDrawable->Clone();
		mSelectedDrawable = other.mSelectedDrawable->Clone();
		mDragNode = other.mNodeSample->Clone();
		mDragNodeBack = other.mSelectedDrawable->Clone();
		mHoverLayout = other.mHoverLayout;

		mDragNode->AddLayer("selectionBack", mSelectedDrawable->Clone(), mHoverLayout, -1.0f);

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UITree::~UITree()
	{
		delete mNodeSample;
		delete mHoverDrawable;
		delete mSelectedDrawable;
		delete mDragNode;
		delete mDragNodeBack;

		for (auto node : mNodesPool)
			delete node;

		for (auto sel : mSelectedItems)
			delete sel.selectionSprite;

		for (auto spr : mSelectionSpritesPool)
			delete spr;
	}

	UITree& UITree::operator=(const UITree& other)
	{
		UIScrollArea::operator=(other);

		delete mNodeSample;
		delete mHoverDrawable;
		delete mSelectedDrawable;
		delete mDragNode;
		delete mDragNodeBack;

		mNodeSample = other.mNodeSample->Clone();
		mHoverDrawable = other.mHoverDrawable->Clone();
		mSelectedDrawable = other.mSelectedDrawable->Clone();
		mDragNode = other.mNodeSample->Clone();
		mDragNodeBack = other.mSelectedDrawable->Clone();
		mHoverLayout = other.mHoverLayout;

		mDragNode->AddLayer("selectionBack", mSelectedDrawable->Clone(), mHoverLayout, -1.0f);

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UITree::Draw()
	{
		if (mFullyDisabled)
			return;

		if (mDraggingNodes)
			o2UI.RegTopWidget(mDragNode);

		for (auto layer : mDrawingLayers)
			layer->Draw();

		mDrawDepth = o2Render.GetDrawingDepth() + 1;

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		for (auto sel : mSelectedItems)
		{
			if (sel.node)
				sel.selectionSprite->Draw();
		}

		for (auto child : mChilds)
			child->Draw();

		mHoverDrawable->Draw();

		// 		if (mInsertNodeCandidate)
		// 			o2Render.DrawRectFrame(mInsertNodeCandidate->layout.absRect);

		o2Render.DisableScissorTest();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (mOwnHorScrollBar)
			mHorScrollBar->Draw();

		if (mOwnVerScrollBar)
			mVerScrollBar->Draw();

		if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
			DrawDebugFrame();
	}

	void UITree::Update(float dt)
	{
		if (mFullyDisabled)
			return;

		if (mNeedUpdateLayout)
		{
			UpdateLayout();
			mNeedUpdateLayout = false;
		}

		UIScrollArea::Update(dt);

		const float rectLerpCoef = 20.0f;

		if (mCurrentHoverRect != mTargetHoverRect)
		{
			mCurrentHoverRect = Math::Lerp(mCurrentHoverRect, mTargetHoverRect, dt*rectLerpCoef);
			mHoverDrawable->SetRect(mCurrentHoverRect);
		}

		if (mExpandNodeCandidate)
		{
			mExpandInsertTime -= dt;

			if (mExpandInsertTime <= 0.0f)
			{
				mExpandNodeCandidate->Expand();
				mExpandNodeCandidate = nullptr;
			}
		}

		mPressedTime += dt;
	}

	void UITree::RebuildTree()
	{
		mWaitSelectionsUpdate = true;
		Vector<UnknownType*> objects = getChildsFunc(nullptr);

		//check removed objects
		Vector<UITreeNode*> removedNodes;
		Vector<UITreeNode*> thisNodes;
		for (auto child : mChilds)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			UITreeNode* childNode = (UITreeNode*)child;

			thisNodes.Add(childNode);

			if (!objects.Contains(childNode->mObject))
				removedNodes.Add(childNode);
		}

		for (auto node : removedNodes)
		{
			node->RemoveChildrenNodes();
			mAllNodes.Remove(node);
			RemoveChild(node, false);
			FreeTreeNode(node);
		}

		//check new objects and rebuild old
		for (auto obj : objects)
		{
			auto node = thisNodes.FindMatch([&](auto x) { return x->mObject == obj; });

			if (node)
			{
				node->Rebuild();
				continue;
			}

			UITreeNode* newNode = CreateTreeNode();
			newNode->mObject = obj;
			newNode->mTree = this;
			newNode->mParent = this;
			newNode->SetExpanded(mExpandedObjects.Contains(obj), true);
			mChilds.Add(newNode);

			mAllNodes.Add(newNode);

			newNode->Rebuild();
		}

		// rearrange nodes by objects arranging
		auto childs = mChilds;
		mChilds.Clear();

		for (auto obj : objects)
		{
			auto node = childs.FindMatch([&](auto x) {
				return x->GetType() == UITreeNode::type && ((UITreeNode*)x)->mObject == obj;
			});
			mChilds.Add(node);
		}

		CheckSelectedNodes();
		UpdateLayout();
	}

	void UITree::UpdateTreeNode(UnknownType* object)
	{
		if (!object)
		{
			RebuildTree();
			return;
		}

		auto node = GetNode(object);
		if (!node)
			return;

		node->Rebuild(true);
	}

	UITreeNode* UITree::GetNode(UnknownType* object)
	{
		for (auto child : mAllNodes)
		{
			UITreeNode* childNode = (UITreeNode*)child;

			if (childNode->mObject == object)
				return childNode;
		}

		return nullptr;
	}

	void UITree::ExpandAll()
	{
		for (auto child : mAllNodes)
		{
			UITreeNode* childNode = (UITreeNode*)child;
			childNode->Expand();
		}
	}

	void UITree::CollapseAll()
	{
		for (auto child : mAllNodes)
		{
			UITreeNode* childNode = (UITreeNode*)child;
			childNode->Collapse();
		}
	}

	Vector<UnknownType*> UITree::GetSelectedObjects() const
	{
		return mSelectedItems.Select<UnknownType*>([&](auto x) { return x.object; });
	}

	void UITree::SetSelectedObjects(const Vector<UnknownType*>& objects)
	{
		DeselectAllObjects();

		for (auto obj : objects)
		{
			SelectedNode selectionNode;
			selectionNode.object = obj;
			selectionNode.selectionSprite = CreateSelectionSprite();
			selectionNode.node = GetNode(obj);

			mSelectedItems.Add(selectionNode);
		}

		OnItemsSelected();
		UpdateLayout();
	}

	void UITree::SelectObject(UnknownType* object)
	{
		SelectedNode selectionNode;
		selectionNode.object = object;
		selectionNode.selectionSprite = CreateSelectionSprite();
		selectionNode.node = GetNode(object);

		mSelectedItems.Add(selectionNode);
		OnItemsSelected();

		UpdateLayout();
	}

	void UITree::DeselectObject(UnknownType* object)
	{
		int idx = mSelectedItems.FindIdx([&](auto x) { return x.object == object; });

		if (idx < 0)
			return;

		FreeSelectionSprite(mSelectedItems[idx].selectionSprite);
		mSelectedItems.RemoveAt(idx);
		OnItemsSelected();

		UpdateLayout();
	}

	void UITree::DeselectAllObjects()
	{
		for (auto sel : mSelectedItems)
			FreeSelectionSprite(sel.selectionSprite);

		mSelectedItems.Clear();
		OnItemsSelected();
	}

	UITreeNode* UITree::GetNodeSample() const
	{
		return mNodeSample;
	}

	Sprite* UITree::GetHoverDrawable() const
	{
		return mHoverDrawable;
	}

	Sprite* UITree::GetSelectionDrawable() const
	{
		return mSelectedDrawable;
	}

	void UITree::SetSelectionDrawableLayout(const Layout& layout)
	{
		mHoverLayout = layout;
	}

	Layout UITree::GetSelectionDrawableLayout() const
	{
		return mHoverLayout;
	}

	bool UITree::IsUnderPoint(const Vec2F& point)
	{
		return layout.GetAbsoluteRect().IsInside(point);
	}

	float UITree::Depth()
	{
		return mDrawDepth;
	}

	bool UITree::IsScrollable() const
	{
		return true;
	}

	void UITree::SetNodesPoolResizeCount(int count)
	{
		mNodesPoolResizeCount = count;
	}

	void UITree::SetSelectionSpritesPoolResizeCount(int count)
	{
		mSelectionSpritesPoolResizeCount = count;
	}

	void UITree::OnObjectCreated(UnknownType* object, UnknownType* parent)
	{
		auto parentNode = GetNode(parent);

		if (!parentNode)
			return;

		auto allParentChilds = getChildsFunc(parent);
		int objectIdx = allParentChilds.Find(object);

		auto objTreeNode = CreateTreeNode();
		objTreeNode->mObject = object;
		objTreeNode->mTree = this;
		mAllNodes.Add(objTreeNode);
		AddChild(objTreeNode, objectIdx);
	}

	void UITree::OnObjectRemoved(UnknownType* object)
	{
		auto node = GetNode(object);

		if (node)
		{
			mAllNodes.Remove(node);
			node->GetParent()->RemoveChild(node, false);
			FreeTreeNode(node);
		}

		CheckSelectedNodes();
	}

	void UITree::UpdateLayout(bool forcible /*= false*/)
	{
		if (!forcible)
		{
			if (mParent)
				mParent->UpdateLayout();

			return;
		}

		float currentHeight = GetCurrentHeight();

		float childPos = 0;
		for (auto child : mChilds)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			auto node = (UITreeNode*)child;

			child->layout.mOffsetMin.x = 0.0f;
			child->layout.mOffsetMax.y = -childPos;

			childPos += node->GetCurrentHeight();
			child->layout.mOffsetMin.y = -childPos;
		}

		RecalculateAbsRect();
		UpdateLayersLayouts();

		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout.mAbsoluteRect);
		mAbsoluteClipArea = mClipAreaLayout.Calculate(layout.mAbsoluteRect);
		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));

		mChildsAbsRect = mAbsoluteViewArea + roundedScrollPos;

		for (auto child : mChilds)
			child->UpdateLayout(true);

		UpdateScrollParams();

		RectF _mChildsAbsRect = mChildsAbsRect;
		mChildsAbsRect = layout.mAbsoluteRect;

		if (mOwnHorScrollBar)
			mHorScrollBar->UpdateLayout(true);

		if (mOwnVerScrollBar)
			mVerScrollBar->UpdateLayout(true);

		mChildsAbsRect = _mChildsAbsRect;

		for (auto sel : mSelectedItems)
		{
			if (sel.node)
			{
				sel.selectionSprite->SetRect(mHoverLayout.Calculate(sel.node->layout.GetAbsoluteRect()));
				sel.selectionSprite->SetEnabled(true);
				sel.selectionSprite->transparency = sel.node->resTransparency;
			}
			else sel.selectionSprite->SetEnabled(false);
		}

		if (mDraggingNodes)
			UpdateDragging(*o2Input.GetCursor(0));
	}

	void UITree::UpdateScrollParams()
	{
		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout.mAbsoluteRect);
		RectF localViewArea(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());

		mScrollArea = RectF(0.0f, 0.0f, localViewArea.Width(), localViewArea.Height());

		Vec2F thisWorldPos = mChildsAbsRect.LeftBottom();
		float downHeight = mNodeSample->layout.mMinSize.y*3.0f;
		for (auto child : mAllNodes)
		{
			RectF rt = child->layout.mAbsoluteRect - thisWorldPos;

			mScrollArea.left = Math::Min(mScrollArea.left, rt.left);
			mScrollArea.bottom = Math::Min(mScrollArea.bottom, rt.bottom - downHeight);
			mScrollArea.right = Math::Max(mScrollArea.right, rt.right);
			mScrollArea.top = Math::Max(mScrollArea.top, rt.top);
		}

		mScrollRange = RectF(mScrollArea.left - localViewArea.left,
							 localViewArea.Height() - mScrollArea.top + localViewArea.bottom,
							 -(localViewArea.Width() - mScrollArea.right + localViewArea.left),
							 -mScrollArea.bottom + localViewArea.bottom);

		if (mHorScrollBar)
		{
			if (Math::Equals(mScrollRange.left, mScrollRange.right, 1.0f))
			{
				if (mEnableHorScroll)
				{
					auto enableHorBarState = state["enableHorBar"];
					if (enableHorBarState)
						*enableHorBarState = false;

					mHorScrollBar->Hide();
				}

				mEnableHorScroll = false;
			}
			else
			{
				if (!mEnableHorScroll)
				{
					auto enableHorBarState = state["enableHorBar"];
					if (enableHorBarState)
						*enableHorBarState = true;

					mHorScrollBar->Show();
				}

				mEnableHorScroll = true;

				mHorScrollBar->SetValueRange(mScrollRange.left, mScrollRange.right);
				mHorScrollBar->SetScrollHandleSize(localViewArea.Width());
			}
		}

		if (mVerScrollBar)
		{
			if (Math::Equals(mScrollRange.bottom, mScrollRange.top, 1.0f))
			{
				if (mEnableVerScroll)
				{
					auto enableVerBarState = state["enableVerBar"];
					if (enableVerBarState)
						*enableVerBarState = false;

					mVerScrollBar->Hide();
				}

				mEnableVerScroll = false;
			}
			else
			{
				if (!mEnableVerScroll)
				{
					auto enableVerBarState = state["enableVerBar"];
					if (enableVerBarState)
						*enableVerBarState = true;

					mVerScrollBar->Show();
				}

				mEnableVerScroll = true;

				mVerScrollBar->SetValueRange(mScrollRange.bottom, mScrollRange.top);
				mVerScrollBar->SetScrollHandleSize(localViewArea.Height());
			}
		}
	}

	float UITree::GetCurrentHeight() const
	{
		float res = 0;

		for (auto child : mChilds)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			UITreeNode* childNode = (UITreeNode*)child;
			res += childNode->GetCurrentHeight();
		}

		return res;
	}

	void UITree::OnCursorPressed(const Input::Cursor& cursor)
	{
		mPressedPoint = cursor.mPosition;
		mPressedNode = GetItemUnderPoint(cursor.mPosition);

		if (mPressedNode && mPressedTime < 0.3f)
		{
			onItemDblClick(mPressedNode);
		}
		else
		{
			if (o2Input.IsKeyDown(VK_SHIFT)) 
				SelectMultipleNodes(cursor);
		}

		mPressedTime = 0.0f;
	}

	void UITree::OnCursorStillDown(const Input::Cursor& cursor)
	{
		const float dragThreshold = 5.0f;

		if (!mDraggingNodes && (cursor.mPosition - mPressedPoint).Length() > dragThreshold)
			BeginDragging();

		if (mDraggingNodes && cursor.mDelta.Length() > 0.5f)
			UpdateDragging(cursor);
	}

	void UITree::BeginDragging()
	{
		if (!mPressedNode)
			return;

		if (!mSelectedItems.Any([&](auto x) { return x.node == mPressedNode; }))
		{
			DeselectAllObjects();

			SelectedNode selectionNode;
			selectionNode.object = mPressedNode->mObject;
			selectionNode.selectionSprite = CreateSelectionSprite();
			selectionNode.node = GetNode(mPressedNode->mObject);

			mSelectedItems.Add(selectionNode);
			OnItemsSelected();

			UpdateLayout();
		}

		mDraggingNodes = true;

		mDragOffset = mPressedNode->layout.mAbsoluteRect.Center() - mPressedPoint;
		mDragNode->Show(true);

		setupNodeFunc(mDragNode, mSelectedItems.Last().object);

		if (mSelectedItems.Count() > 1)
		{
			if (auto nameLayer = mDragNode->FindLayer<Text>())
				nameLayer->text = String::Format("%i items", mSelectedItems.Count());
		}

		for (auto sel : mSelectedItems)
		{
			if (sel.node)
				sel.node->SetState("dragging", true);
		}
	}

	void UITree::UpdateDragging(const Input::Cursor &cursor)
	{
		float width = layout.mAbsoluteRect.Width();
		float height = mNodeSample->layout.minHeight;
		RectF dragNodeRect = RectF(cursor.mPosition.x - width*0.5f, cursor.mPosition.y + height*0.5f,
								   cursor.mPosition.x + width*0.5f, cursor.mPosition.y - height*0.5f);

		dragNodeRect += mDragOffset;
		mDragNode->layout.absRect = dragNodeRect;
		mDragNode->UIWidget::UpdateLayout(true);

		mDragNodeBack->SetRect(mHoverLayout.Calculate(dragNodeRect));

		UITreeNode* currentInsertCandidate = nullptr;
		float nodeDst = FLT_MAX;
		for (auto node : mAllNodes)
		{
			if (node->mDragSizeCoef < 0.95f)
				continue;

			float dst = cursor.mPosition.y - node->layout.mAbsoluteRect.Center().y;
			if (dst > 0 && dst < nodeDst)
			{
				nodeDst = dst;
				currentInsertCandidate = node;
			}
		}

		if (currentInsertCandidate != mInsertNodeCandidate)
		{
			mInsertNodeCandidate = currentInsertCandidate;

			for (auto node : mAllNodes)
				node->SetState("inserting", node->layout.mAbsoluteRect.Center().y < cursor.mPosition.y);
		}

		mUnderCursorItem = GetItemUnderPoint(cursor.mPosition);
		if (mUnderCursorItem != mExpandNodeCandidate)
		{
			mExpandNodeCandidate = mUnderCursorItem;
			mExpandInsertTime = 0.6f;
		}

		UpdateHover(mUnderCursorItem);
	}

	void UITree::EndDragging()
	{
		mDraggingNodes = false;
		mDragNode->Hide(true);

		for (auto node : mAllNodes)
			node->SetState("inserting", false);

		for (auto sel : mSelectedItems)
		{
			if (sel.node)
				sel.node->SetState("dragging", false);
		}

		UnknownObjectsVec objects;
		UnknownType* targetParent = nullptr;
		UnknownType* targetPrevObject = nullptr;

		if (mUnderCursorItem)
		{
			targetParent = mUnderCursorItem->mObject;
		}
		else
		{
			if (mInsertNodeCandidate)
			{
				if (mInsertNodeCandidate->mParent && mInsertNodeCandidate->mParent->GetType() == UITreeNode::type)
					targetParent = ((UITreeNode*)mInsertNodeCandidate->mParent)->mObject;

				targetPrevObject = mInsertNodeCandidate->mObject;
			}
		}

		for (auto sel : mSelectedItems)
		{
			bool processing = true;

			UnknownType* parent = getParentFunc(sel.object);
			while (parent)
			{
				if (mSelectedItems.ContainsPred([&](auto x) { return parent == x.object; }))
				{
					processing = false;
					break;
				}

				parent = getParentFunc(parent);
			}

			if (processing)
				objects.Add(sel.object);
		}

		onDraggedObjects(objects, targetParent, targetPrevObject);

		RebuildTree();
	}

	void UITree::UpdateHover(UITreeNode* itemUnderCursor)
	{
		bool hoverVisible = false;
		if (itemUnderCursor)
		{
			mTargetHoverRect = mHoverLayout.Calculate(itemUnderCursor->layout.mAbsoluteRect);
			hoverVisible = true;
		}
		else if (mInsertNodeCandidate)
		{
			hoverVisible = true;
			mTargetHoverRect = mHoverLayout.Calculate(
				mInsertNodeCandidate->layout.mAbsoluteRect +
				Vec2F(0, mInsertNodeCandidate->layout.mMinSize.y*mInsertNodeCandidate->mInsertSizeCoef));
		}
		else if (mChilds.Count() > 0)
		{
			hoverVisible = true;
			auto lastNode = mChilds.Last();
			mTargetHoverRect = mHoverLayout.Calculate(lastNode->layout.mAbsoluteRect - Vec2F(0, lastNode->layout.mMinSize.y));
		}

		if (auto hoverState = state["hover"])
			hoverState->SetState(hoverVisible);
		else
			mHoverDrawable->SetEnabled(hoverVisible);
	}

	void UITree::OnCursorMoved(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.mPosition - mLastHoverCheckCursor).Length() < checkDeltaThreshold || mDraggingNodes)
			return;

		auto nodeUnderCursor = GetItemUnderPoint(cursor.mPosition);

		if (auto hoverState = state["hover"])
			hoverState->SetState(nodeUnderCursor != nullptr);
		else
			mHoverDrawable->SetEnabled(nodeUnderCursor != nullptr);

		if (nodeUnderCursor)
			mTargetHoverRect = mHoverLayout.Calculate(nodeUnderCursor->layout.mAbsoluteRect);
	}

	void UITree::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mDraggingNodes)
			EndDragging();
		else
			if (!o2Input.IsKeyDown(VK_SHIFT)) SelectSingleNode(cursor);
	}

	void UITree::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		if (mSelectedItems.Count() < 2) 
			SelectSingleNode(cursor);

		auto node = GetItemUnderPoint(cursor.mPosition);
		if (node)
			onItemRBClick(node);
	}

	void UITree::SelectSingleNode(const Input::Cursor &cursor)
	{
		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAllObjects();

		auto nodeUnderCursor = GetItemUnderPoint(cursor.mPosition);

		if (nodeUnderCursor)
		{
			int selIdx = mSelectedItems.FindIdx([&](auto x) { return x.node == nodeUnderCursor; });
			if (selIdx < 0)
			{
				SelectedNode selectionNode;
				selectionNode.object = nodeUnderCursor->mObject;
				selectionNode.selectionSprite = CreateSelectionSprite();
				selectionNode.node = GetNode(nodeUnderCursor->mObject);

				mSelectedItems.Add(selectionNode);
				OnItemsSelected();

				UpdateLayout();
			}
			else
			{
				FreeSelectionSprite(mSelectedItems[selIdx].selectionSprite);
				mSelectedItems.RemoveAt(selIdx);
				OnItemsSelected();
			}
		}
	}

	void UITree::SelectMultipleNodes(const Input::Cursor &cursor)
	{
		auto nodeUnderCursor = GetItemUnderPoint(cursor.mPosition);

		if (nodeUnderCursor)
		{
			if (mSelectedItems.Count() > 0)
			{
				float selectionUp = Math::Max<float>(cursor.mPosition.y, mSelectedItems.Last().node->layout.absTop);
				float selectionDown = Math::Min<float>(cursor.mPosition.y, mSelectedItems.Last().node->layout.absBottom);

				for (auto node : mAllNodes)
				{
					float top = node->layout.absTop;
					float bottom = node->layout.absBottom;
					if ((top > selectionDown && top < selectionUp) || (bottom > selectionDown && bottom < selectionUp))
					{
						if (mSelectedItems.FindIdx([&](auto x) { return x.node == node; }) < 0)
						{
							SelectedNode selectionNode;
							selectionNode.object = node->mObject;
							selectionNode.selectionSprite = CreateSelectionSprite();
							selectionNode.node = GetNode(node->mObject);

							mSelectedItems.Add(selectionNode);
							OnItemsSelected();
						}
					}
				}

				UpdateLayout();
			}
			else SelectSingleNode(cursor);
		}
		else DeselectAllObjects();
	}

	void UITree::OnCursorPressBreak(const Input::Cursor& cursor)
	{

	}

	void UITree::OnCursorExit(const Input::Cursor& cursor)
	{
		if (auto hoverState = state["hover"])
			hoverState->SetState(false);
		else
			mHoverDrawable->SetEnabled(false);
	}

	UITreeNode* UITree::GetItemUnderPoint(const Vec2F& point)
	{
		for (auto node : mAllNodes)
		{
			if (node->layout.mAbsoluteRect.IsInside(point))
				return node;
		}

		return nullptr;
	}

	UITreeNode* UITree::CreateTreeNode()
	{
		if (mNodesPool.Count() == 0)
		{
			for (int i = 0; i < mNodesPoolResizeCount; i++)
			{
				auto newNode = mNodeSample->Clone();
				newNode->Hide(true);
				mNodesPool.Add(newNode);
			}
		}

		auto res = mNodesPool.PopBack();
		res->Show(true);
		return res;
	}

	void UITree::FreeTreeNode(UITreeNode* node)
	{
		node->Hide(true);
		mNodesPool.Add(node);
	}

	Sprite* UITree::CreateSelectionSprite()
	{
		if (mSelectionSpritesPool.Count() == 0)
		{
			for (int i = 0; i < mSelectionSpritesPoolResizeCount; i++)
				mSelectionSpritesPool.Add(mSelectedDrawable->Clone());
		}

		return mSelectionSpritesPool.PopBack();
	}

	void UITree::FreeSelectionSprite(Sprite* sprite)
	{
		mSelectionSpritesPool.Add(sprite);
	}

	void UITree::CheckSelectedNodes()
	{
		for (auto& sel : mSelectedItems)
		{
			if (sel.node)
			{
				if (mAllNodes.Find(sel.node) < 0)
					sel.node = nullptr;
			}
			else
			{
				sel.node = GetNode(sel.object);
			}
		}

		mWaitSelectionsUpdate = false;
	}

	void UITree::OnItemsSelected()
	{
		onItemsSelectionChanged(mSelectedItems.Select<UnknownType*>([](auto x) { return x.object; }));
	}

	bool UITree::SelectedNode::operator==(const SelectedNode& other) const
	{
		return object == other.object;
	}
}