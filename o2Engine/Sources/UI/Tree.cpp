#include "stdafx.h"
#include "Tree.h"

#include "Animation/Animate.h"
#include "Animation/AnimatedFloat.h"
#include "Application/Input.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "Utils/Tasks/TaskManager.h"

namespace o2
{
	UITreeNode::UITreeNode()
	{
		onDraw = [&]() { CursorAreaEventsListener::OnDrawn(); };
	}

	UITreeNode::UITreeNode(const UITreeNode& other) :
		UIWidget(other)
	{
		onDraw = [&]() { CursorAreaEventsListener::OnDrawn(); };

		mExpandBtn = (UIButton*)GetChild("expandBtn");
		if (mExpandBtn)
			mExpandBtn->onClick = [&]() { if (IsExpanded()) Collapse(); else Expand(); o2UI.FocusWidget(mOwnerTree); };

		SetSelectionGroup(other.GetSelectionGroup());

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	UITreeNode::~UITreeNode()
	{}

	UITreeNode& UITreeNode::operator=(const UITreeNode& other)
	{
		UIWidget::operator=(other);
		return *this;
	}

	void UITreeNode::SetExpanded(bool expanded, bool forcible /*= false*/)
	{
		if (expanded == IsExpanded())
			return;

		if (expanded) mOwnerTree->ExpandNode(mNodeDef);
		else          mOwnerTree->CollapseNode(mNodeDef);

		SetState("expanded", expanded);
		mNodeDef->isExpanded = expanded;
	}

	bool UITreeNode::IsExpanded() const
	{
		return mNodeDef->isExpanded;
	}

	void UITreeNode::Expand(bool forcible /*= false*/)
	{
		SetExpanded(true, forcible);
	}

	void UITreeNode::Collapse(bool forcible /*= false*/)
	{
		SetExpanded(false, forcible);
	}

	UnknownPtr UITreeNode::GetObject() const
	{
		return mNodeDef->object;
	}

	bool UITreeNode::IsUnderPoint(const Vec2F& point)
	{
		return UIWidget::IsUnderPoint(point);
	}

	void UITreeNode::CopyData(const Actor& otherActor)
	{
		const UITreeNode& other = dynamic_cast<const UITreeNode&>(otherActor);

		UIWidget::CopyData(other);

		onDraw = [&]() { CursorAreaEventsListener::OnDrawn(); };

		mExpandBtn = (UIButton*)GetChild("expandBtn");
		if (mExpandBtn)
			mExpandBtn->onClick = [&]() { if (IsExpanded()) Collapse(); else Expand(); o2UI.FocusWidget(mOwnerTree); };

		SetSelectionGroup(other.GetSelectionGroup());

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void UITreeNode::UpdateTreeLayout(float dt)
	{
		mOwnerTree->mIsNeedUdateLayout = true;
	}

	void UITreeNode::OnCursorDblClicked(const Input::Cursor& cursor)
	{
		if (mOwnerTree)
			mOwnerTree->OnNodeDblClick(this);
	}

	void UITreeNode::OnCursorEnter(const Input::Cursor& cursor)
	{
		if (mOwnerTree)
			mOwnerTree->UpdateHover(this);
	}

	void UITreeNode::OnCursorExit(const Input::Cursor& cursor)
	{
		if (mOwnerTree)
			mOwnerTree->UpdateHover(nullptr);
	}

	void UITreeNode::OnDragStart(const Input::Cursor& cursor)
	{
		mOwnerTree->BeginDragging(this);
	}

	void UITreeNode::OnDragged(const Input::Cursor& cursor, DragDropArea* area)
	{
		mOwnerTree->UpdateDraggingGraphics();
	}

	void UITreeNode::OnDragEnd(const Input::Cursor& cursor)
	{
		mOwnerTree->mDragEnded = true;
	}

	void UITreeNode::OnSelected()
	{
		mOwnerTree->Focus();
	}

	void UITreeNode::OnDeselected()
	{}

	UITree::UITree()
	{
		mNodeWidgetSample = mnew UITreeNode();
		mNodeWidgetSample->layout->minHeight = 20;
		mNodeWidgetSample->AddLayer("caption", nullptr);

		mFakeDragNode = mnew UITreeNode();
		mHoverDrawable = mnew Sprite();
		mHighlightSprite = mnew Sprite();

		mHighlightAnim.SetTarget(mHighlightSprite);
	}

	UITree::UITree(const UITree& other) :
		UIScrollArea(other)
	{
		mRearrangeType = other.mRearrangeType;
		mMultiSelectAvailable = other.mMultiSelectAvailable;
		mNodeWidgetSample = other.mNodeWidgetSample->CloneAs<UITreeNode>();
		mFakeDragNode = other.mNodeWidgetSample->CloneAs<UITreeNode>();
		mHoverDrawable = other.mHoverDrawable->CloneAs<Sprite>();
		mHighlightSprite = other.mHighlightSprite->CloneAs<Sprite>();

		mHighlightAnim.SetTarget(mHighlightSprite);

		mHoverLayout = other.mHoverLayout;
		mHighlightLayout = other.mHighlightLayout;
		mHighlightAnim = other.mHighlightAnim;

		RetargetStatesAnimations();
		SetLayoutDirty();

		const int widgetsBufferInitialSize = 20;
		TreeNodesVec widgets;
		for (int i = 0; i < widgetsBufferInitialSize; i++)
			widgets.Add(CreateTreeNodeWidget());

		mNodeWidgetsBuf.Add(widgets);
	}

	UITree::~UITree()
	{
		delete mNodeWidgetSample;
		delete mFakeDragNode;
		delete mHoverDrawable;
		delete mHighlightSprite;

		for (auto node : mAllNodes)
			delete node;
	}

	UITree& UITree::operator=(const UITree& other)
	{
		UIScrollArea::operator=(other);
		return *this;
	}

	void UITree::Draw()
	{
		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		if (mIsDraggingNodes)
			o2UI.DrawWidgetAtTop(mFakeDragNode);

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		if (mExpandingNodeState == ExpandState::None)
		{
			for (auto child : mDrawingChildren)
				child->Draw();
		}
		else
		{
			float clipTop = mAbsoluteViewArea.top - mExpandingNodePosition + mScrollPos.y;
			float clipBottom = mAbsoluteViewArea.top - (mExpandingNodePosition + mExpandingNodeCurrHeight) + mScrollPos.y;
			int clipTopIdx = Math::Max(mMinVisibleNodeIdx, mExpandingNodeIdx + 1);
			int clipBottomIdx = Math::Min(mMaxVisibleNodeIdx, mExpandingNodeIdx + mExpandingNodeChildsCount);

			o2Render.EnableScissorTest(RectF(mAbsoluteClipArea.left, clipTop, mAbsoluteClipArea.right, clipBottom));

			for (int i = clipTopIdx; i <= clipBottomIdx; i++)
			{
				Node* node = mAllNodes[i];
				if (node->widget)
					node->widget->Draw();
			}

			o2Render.DisableScissorTest();

			for (int i = mMinVisibleNodeIdx; i <= mMaxVisibleNodeIdx; i++)
			{
				Node* node = mAllNodes[i];
				if (node->widget && (i < clipTopIdx || i > clipBottomIdx))
					node->widget->Draw();
			}
		}

		mHoverDrawable->Draw();
		mHighlightSprite->Draw();

		o2Render.DisableScissorTest();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (mOwnHorScrollBar)
			mHorScrollBar->Draw();

		if (mOwnVerScrollBar)
			mVerScrollBar->Draw();

		DrawDebugFrame();
	}

	void UITree::Update(float dt)
	{
		UpdateHighlighting(dt);
		UpdateNodeExpanding(dt);

		if (mDragEnded)
			EndDragging();

		if (mIsDraggingNodes)
			UpdateDraggingInsertionAnim(dt);

		if (mIsNeedUpdateView || o2Input.IsKeyPressed('B'))
			UpdateNodesStructure();

		if (mIsNeedUdateLayout)
			SetLayoutDirty();

		if (mIsNeedUpdateVisibleNodes)
			UpdateVisibleNodes();

		UIScrollArea::Update(dt);

		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		const float rectLerpCoef = 10.0f;
		if (mCurrentHoverRect != mTargetHoverRect)
		{
			mCurrentHoverRect = Math::Lerp(mCurrentHoverRect, mTargetHoverRect, dt*rectLerpCoef);
			mHoverDrawable->SetRect(mCurrentHoverRect);

			float alpha = mHoverDrawable->GetTransparency();
			mHoverDrawable->SetTransparency(alpha);
		}

		UpdatePressedNodeExpand(dt);

		mPressedTime += dt;
	}

	void UITree::UpdateHighlighting(float dt)
	{
		if (mHighlightAnim.IsPlaying())
		{
			if (mHighlightObject && !mHighlighNode)
				mHighlighNode = mAllNodes.FindMatch([=](Node* x) { return x->object == mHighlightObject; });

			if (mHighlighNode && mHighlighNode->widget)
			{
				mHighlightSprite->SetScale(Vec2F(1.0f, 1.0f));
				mHighlightSprite->SetRect(mHighlightLayout.Calculate(mHighlighNode->widget->layout->worldRect));
			}

			mHighlightAnim.Update(dt);

			if (!mHighlightAnim.IsPlaying())
			{
				mHighlighNode = nullptr;
				mHighlightObject = UnknownPtr();
			}
		}
	}

	void UITree::UpdateChildren(float dt)
	{
		UIScrollArea::UpdateChildren(dt);

		if (mLayoutUpdated)
			UpdateVisibleNodes();
	}

	void UITree::UpdateChildrenTransforms()
	{
		UIScrollArea::UpdateChildrenTransforms();
		UpdateVisibleNodes();
	}

	UnknownPtr UITree::GetObjectParent(UnknownPtr object)
	{
		return getObjectParentDelegate(object);
	}

	Vector<UnknownPtr> UITree::GetObjectChilds(UnknownPtr object)
	{
		return getObjectChildrenDelegate(object);
	}

	String UITree::GetObjectDebug(UnknownPtr object)
	{
		return getDebugForObject(object);
	}

	void UITree::FillNodeDataByObject(UITreeNode* nodeWidget, UnknownPtr object)
	{
		fillNodeDataByObjectDelegate(nodeWidget, object);
	}

	void UITree::OnNodeDblClick(UITreeNode* nodeWidget)
	{
		onNodeDoubleClicked(nodeWidget);
	}

	void UITree::OnNodeRBClick(UITreeNode* nodeWidget)
	{
		onNodeRightButtonClicked(nodeWidget);
	}

	void UITree::OnNodesSelectionChanged(UnknownPtrsVec objects)
	{
		onObjectsSelectionChanged(objects);
	}

	void UITree::OnDraggedObjects(UnknownPtrsVec objects, UnknownPtr newParent, UnknownPtr prevObject)
	{
		onDraggedObjects(objects, newParent, prevObject);
	}

	ISelectableDragableObjectsGroup::SelectDragObjectsVec UITree::GetSelectedDragObjects() const
	{
		return SelectDragObjectsVec();
	}

	ISelectableDragableObjectsGroup::SelectDragObjectsVec UITree::GetAllObjects() const
	{
		return SelectDragObjectsVec();
	}

	void UITree::Select(SelectableDragableObject* object, bool sendOnSelectionChanged)
	{
		bool someSelected = false;
		UITreeNode* uiNode = (UITreeNode*)object;

		if (o2Input.IsKeyDown(VK_SHIFT) && mSelectedNodes.Count() > 0)
		{
			someSelected = CheckMultipleSelection(o2Input.GetCursorPos());
		}
		else if (!o2Input.IsKeyDown(VK_CONTROL))
		{
			for (auto sel : mSelectedNodes)
				sel->SetSelected(false);

			mSelectedNodes.Clear();
			mSelectedObjects.Clear();

			someSelected = true;
		}

		if (!mSelectedNodes.ContainsPred([=](Node* x) { return x->widget == uiNode; }))
		{
			someSelected = true;

			uiNode->mIsSelected = true;

			Node* node = mAllNodes.FindMatch([=](Node* x) { return x->widget == uiNode; });
			node->SetSelected(true);
			mSelectedNodes.Add(node);
			mSelectedObjects.Add(node->object);
		}

		if (someSelected && sendOnSelectionChanged)
			OnSelectionChanged();
	}

	void UITree::Select(SelectableDragableObject* object)
	{
		Select(object, true);
	}

	void UITree::Deselect(SelectableDragableObject* object)
	{
		UITreeNode* uiNode = (UITreeNode*)object;
		uiNode->mIsSelected = false;

		int idx = mSelectedNodes.FindIdx([&](auto x) { return x->widget == uiNode; });

		if (idx < 0)
			return;

		mSelectedNodes[idx]->SetSelected(false);
		mSelectedObjects.Remove(mSelectedNodes[idx]->object);
		mSelectedNodes.RemoveAt(idx);
	}

	void UITree::AddSelectableObject(SelectableDragableObject* object)
	{}

	void UITree::RemoveSelectableObject(SelectableDragableObject* object)
	{}

	void UITree::OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor)
	{
		if (!o2Input.IsKeyDown(VK_SHIFT) && !o2Input.IsKeyDown(VK_CONTROL))
		{
			for (auto sel : mSelectedNodes)
				sel->SetSelected(false);

			mSelectedNodes.Clear();
			mSelectedObjects.Clear();
		}

		object->SetSelected(!object->IsSelected());
	}

	void UITree::OnSelectableObjectBeganDragging(SelectableDragableObject* object)
	{
		mBeforeDragSelectedItems = mSelectedObjects;

		if (!object->IsSelected())
			Select(object, false);
	}

	bool UITree::CheckMultipleSelection(const Vec2F& point)
	{
		if (mSelectedNodes.IsEmpty())
			return false;

		Vec2F currClickPos = mAbsoluteViewArea.LeftTop() - point + mScrollPos;

		float selectionUp = Math::Max<float>(currClickPos.y, mLastClickPos.y);
		float selectionDown = Math::Min<float>(currClickPos.y, mLastClickPos.y);

		bool someSelected = false;

		int idx = 0;
		float nodeHeight = mNodeWidgetSample->layout->GetMinimalHeight();
		for (auto node : mAllNodes)
		{
			if (mSelectedNodes.Contains(node))
			{
				idx++;
				continue;
			}

			float top = (float)idx*nodeHeight;
			float bottom = top + nodeHeight;

			if ((top > selectionDown && top < selectionUp) || (bottom > selectionDown && bottom < selectionUp))
			{
				node->SetSelected(true);
				mSelectedNodes.Add(node);
				mSelectedObjects.Add(node->object);

				someSelected = true;
			}

			idx++;
		}

		return someSelected;
	}

	void UITree::UpdatePressedNodeExpand(float dt)
	{
		if (o2Input.IsCursorDown() && mIsDraggingNodes)
		{
			if (mRearrangeType != RearrangeType::Disabled)
			{
				auto underCursorItem = GetTreeNodeUnderPoint(o2Input.GetCursorPos());

				if (underCursorItem != mExpandNodeCandidate)
				{
					mExpandNodeCandidate = underCursorItem;
					mExpandInsertTime = mNodeExpandTime;
				}
			}

			if (mExpandInsertTime <= 0.0f && mExpandNodeCandidate)
			{
				mExpandNodeCandidate->Expand();

				if (mIsDraggingNodes)
					mExpandNodeCandidate->mNodeDef->inserting = false;

				mExpandNodeCandidate = nullptr;
			}
		}

		mExpandInsertTime -= dt;
	}

	void UITree::UpdateNodesView(bool immediately /*= true*/)
	{
		if (immediately)
		{
			UpdateNodesStructure();
			for (int i = 0; i < mAllNodes.Count(); i++)
			{
				if (mAllNodes[i]->widget)
					UpdateNodeView(mAllNodes[i], mAllNodes[i]->widget, i);
			}
		}
		else mIsNeedUpdateView = true;
	}

	UITreeNode* UITree::GetNode(UnknownPtr object)
	{
		Node* fnd = mAllNodes.FindMatch([=](Node* x) { return x->object == object; });
		if (fnd)
			return fnd->widget;

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

	Vector<UnknownPtr> UITree::GetSelectedObjects() const
	{
		return mSelectedObjects;
	}

	void UITree::SetSelectedObjects(const UnknownPtrsVec& objects)
	{
		for (auto sel : mSelectedNodes)
			sel->SetSelected(false);

		mSelectedNodes.Clear();
		mSelectedObjects.Clear();

		for (auto obj : objects)
		{
			auto node = mAllNodes.FindMatch([=](Node* x) { return x->object == obj; });

			if (!node)
				continue;

			node->SetSelected(true);
			mSelectedNodes.Add(node);
		}

		mSelectedObjects.Add(objects);

		OnSelectionChanged();
		SetLayoutDirty();
	}

	void UITree::SelectObject(UnknownPtr object)
	{
		if (!mMultiSelectAvailable)
			DeselectAllObjects();

		if (mSelectedObjects.Contains(object))
		{
			o2Debug.Log("Already selected");
			return;
		}

		auto node = mAllNodes.FindMatch([=](Node* x) { return x->object == object; });

		if (!node)
		{
			o2Debug.Log("Node not found");
			return;
		}

		node->SetSelected(true);
		mSelectedNodes.Add(node);
		mSelectedObjects.Add(node->object);

		OnSelectionChanged();
		SetLayoutDirty();
	}

	void UITree::SelectAndHightlightObject(UnknownPtr object)
	{
		SelectObject(object);
		ScrollToAndHightlight(object);
	}

	void UITree::DeselectObject(UnknownPtr object)
	{
		int idx = mSelectedNodes.FindIdx([&](auto x) { return x->object == object; });

		if (idx < 0)
			return;

		mSelectedNodes[idx]->SetSelected(false);
		mSelectedObjects.Remove(mSelectedNodes[idx]->object);
		mSelectedNodes.RemoveAt(idx);

		OnSelectionChanged();
		SetLayoutDirty();
	}

	void UITree::DeselectAllObjects()
	{
		for (auto sel : mSelectedNodes)
			sel->SetSelected(false);

		mSelectedNodes.Clear();
		mSelectedObjects.Clear();
		OnSelectionChanged();
	}

	void UITree::ScrollTo(UnknownPtr object)
	{
		if (!object)
		{
			ResetScroll();
			return;
		}

		ExpandParentObjects(object);

		int idx = mAllNodes.FindIdx([=](Node* x) { return x->object == object; });

		if (idx >= 0)
			SetScroll(Vec2F(mScrollPos.x, (float)idx*mNodeWidgetSample->layout->minHeight - layout->height*0.5f));
	}

	void UITree::ScrollToAndHightlight(UnknownPtr object)
	{
		if (!object)
		{
			ResetScroll();
			return;
		}

		ExpandParentObjects(object);

		int idx = mAllNodes.FindIdx([=](Node* x) { return x->object == object; });

		if (idx >= 0)
		{
			float position = (float)idx*mNodeWidgetSample->layout->minHeight;
			float scroll = position - layout->height*0.5f;
			SetScroll(Vec2F(mScrollPos.x, scroll));

			mHighlighNode = mAllNodes[idx];
			mHighlightObject = object;
			mHighlightAnim.RewindAndPlay();
		}
	}

	void UITree::ExpandParentObjects(UnknownPtr object)
	{
		Vector<UnknownPtr> parentsStack;
		UnknownPtr current = GetObjectParent(object);
		while (current)
		{
			parentsStack.Add(current);
			current = GetObjectParent(current);
		}

		for (int i = parentsStack.Count() - 1; i >= 0; i--)
		{
			auto node = mAllNodes.FindMatch([&](Node* x) { return x->object == parentsStack[i]; });

			if (!node)
			{
				o2Debug.LogWarning("UITree: failed to expand parents for node");
				return;
			}

			if (node->isExpanded)
				continue;

			if (node->widget)
				node->widget->SetExpanded(true);
			else
				ExpandNode(node);
		}

		UpdateNodeExpanding(mExpandNodeTime);
		SetLayoutDirty();
	}

	void UITree::OnObjectCreated(UnknownPtr object, UnknownPtr parent)
	{
		mIsNeedUpdateView = true;
	}

	void UITree::OnObjectRemoved(UnknownPtr object)
	{
		mIsNeedUpdateView = true;
	}

	void UITree::OnObjectsChanged(const UnknownPtrsVec& objects)
	{
		for (auto object : objects)
		{
			int idx = mAllNodes.FindIdx([=](Node* x) { return x->object == object; });

			if (idx < 0 || !mAllNodes[idx]->widget)
				continue;

			UpdateNodeView(mAllNodes[idx], mAllNodes[idx]->widget, idx);
		}
	}

	void UITree::UpdateNodesStructure()
	{
		mIsNeedUpdateView = false;

		mHighlighNode = nullptr;

		if (mExpandingNodeState != ExpandState::None)
		{
			UpdateNodeExpanding(mExpandNodeTime);

			for (auto child : mChildWidgets)
				child->SetLayoutDirty();
		}

		Vector<UnknownPtr> rootObjects = GetObjectChilds(UnknownPtr());

		mVisibleWidgetsCache.Clear();
		for (auto node : mVisibleNodes)
		{
			if (!node->widget)
				continue;

			VisibleWidgetDef cache;
			cache.object = node->object;
			cache.widget = node->widget;
			cache.position = mAllNodes.Find(node);

			mVisibleWidgetsCache.Add(cache);
		}

		mNodesBuf.Add(mAllNodes);

		mAllNodes.Clear();
		mVisibleNodes.Clear();
		mChildren.Clear();
		mChildWidgets.Clear();
		mDrawingChildren.Clear();
		mSelectedNodes.Clear();
		mMinVisibleNodeIdx = 0;
		mMaxVisibleNodeIdx = -1;

		int position = 0;
		for (auto object : rootObjects)
		{
			if (mIsDraggingNodes && mSelectedObjects.Contains(object))
				continue;

			Node* node = CreateNode(object, nullptr);
			mAllNodes.Insert(node, position++);
			position += InsertNodes(node, position);
		}

		SetLayoutDirty();
	}

	int UITree::InsertNodes(Node* parentNode, int position, NodesVec* newNodes /*= nullptr*/)
	{
		int initialPosition = position;

		if (mExpandedObjects.Contains(parentNode->object))
		{
			auto childObjects = GetObjectChilds(parentNode->object);
			for (auto child : childObjects)
			{
				if (mIsDraggingNodes && mSelectedObjects.Contains(child))
					continue;

				Node* node = CreateNode(child, parentNode);

				mAllNodes.Insert(node, position++);

				if (newNodes)
					newNodes->Add(node);

				position += InsertNodes(node, position, newNodes);
			}
		}

		return position - initialPosition;
	}

	void UITree::RemoveNodes(Node* parentNode)
	{
		int begin = mAllNodes.Find(parentNode) + 1;
		int end = begin - 1 + parentNode->GetChildCount();

		mAllNodes.RemoveRange(begin, end);
	}

	UITree::Node* UITree::CreateNode(UnknownPtr object, Node* parent)
	{
		Node* node = mNodesBuf.IsEmpty() ? mnew Node() : mNodesBuf.PopBack();
		node->childs.Clear();

		node->parent = parent;
		node->object = object;
		node->widget = nullptr;
		node->isSelected = mSelectedObjects.Contains(object);
		node->isExpanded = mExpandedObjects.Contains(object);
		node->level = parent ? parent->level + 1 : 0;

		node->id = GetObjectDebug(object);

		if (parent)
			parent->childs.Add(node);

		if (node->isSelected)
			mSelectedNodes.Add(node);

		return node;
	}

	void UITree::CopyData(const Actor& otherActor)
	{
		const UITree& other = dynamic_cast<const UITree&>(otherActor);

		UIScrollArea::CopyData(other);

		delete mNodeWidgetSample;
		delete mHoverDrawable;
		delete mFakeDragNode;
		delete mHighlightSprite;

		mRearrangeType = other.mRearrangeType;
		mMultiSelectAvailable = other.mMultiSelectAvailable;
		mNodeWidgetSample = other.mNodeWidgetSample->CloneAs<UITreeNode>();
		mFakeDragNode = other.mNodeWidgetSample->CloneAs<UITreeNode>();
		mHoverDrawable = other.mHoverDrawable->CloneAs<Sprite>();
		mHighlightSprite = other.mHighlightSprite->CloneAs<Sprite>();

		mHighlightAnim.SetTarget(mHighlightSprite);

		mHoverLayout = other.mHoverLayout;
		mHighlightLayout = other.mHighlightLayout;
		mHighlightAnim = other.mHighlightAnim;

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void UITree::OnFocused()
	{
		for (auto node : mVisibleNodes)
			if (node->widget)
				node->widget->SetState("focused", true);

		onFocused();
	}

	void UITree::OnUnfocused()
	{
		for (auto node : mVisibleNodes)
			if (node->widget)
				node->widget->SetState("focused", false);

		onUnfocused();
	}

	void UITree::UpdateTransform(bool withChildren /*= true*/)
	{
		UIScrollArea::UpdateTransform(false);

		if (withChildren)
			UpdateVisibleNodes();

		mIsNeedUdateLayout = false;
	}

	void UITree::UpdateVisibleNodes()
	{
		mIsNeedUpdateVisibleNodes = false;

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		int lastMinVisible = mMinVisibleNodeIdx;
		int lastMaxVisible = mMaxVisibleNodeIdx;

		float topVisiblePosition = mScrollPos.y;
		float bottomVisiblePosition = mScrollPos.y + mAbsoluteViewArea.Height();

		mMinVisibleNodeIdx = GetNodeIndex(topVisiblePosition);
		mMaxVisibleNodeIdx = GetNodeIndex(bottomVisiblePosition);

		if (mAllNodes.IsEmpty())
			mMaxVisibleNodeIdx = -1;

		for (int i = lastMinVisible; i <= lastMaxVisible; i++)
		{
			if (i < mMinVisibleNodeIdx || i > mMaxVisibleNodeIdx)
			{
				if (i >= mAllNodes.Count())
					break;

				Node* node = mAllNodes[i];

				if (node->widget)
				{
					mNodeWidgetsBuf.Add(node->widget);
					mChildren.Remove(node->widget);
					mChildWidgets.Remove(node->widget);
					mDrawingChildren.Remove(node->widget);

					node->widget->mParent = nullptr;
					node->widget->mParentWidget = nullptr;
					node->widget->mNodeDef = nullptr;
					node->widget = nullptr;
				}
			}
		}

		float nodeHeight = mNodeWidgetSample->layout->minHeight;
		for (int i = mMinVisibleNodeIdx; i <= mMaxVisibleNodeIdx && i < mAllNodes.Count(); i++)
		{
			Node* node = mAllNodes[i];

			if (i < lastMinVisible || i > lastMaxVisible)
				CreateVisibleNodeWidget(node, i);

			if (!node->widget)
			{
				float realPosition = (float)i*nodeHeight;
				if (realPosition > topVisiblePosition && realPosition < bottomVisiblePosition)
					CreateVisibleNodeWidget(node, i);
			}
		}

		mVisibleNodes = mAllNodes.Take(mMinVisibleNodeIdx, mMaxVisibleNodeIdx + 1);

		for (auto node : mVisibleNodes)
		{
			if (node->widget)
			{
				node->widget->UpdateTransform(true);
				node->widget->CheckClipping(mAbsoluteClipArea);
			}
		}

		mNodeWidgetsBuf.Add(mVisibleWidgetsCache.FindAll([](const VisibleWidgetDef& x) { return x.widget != nullptr; })
							.Select<UITreeNode*>([](const VisibleWidgetDef& x) { return x.widget; }));
		mVisibleWidgetsCache.Clear();

		if (mIsDraggingNodes)
			OnDraggedAbove(this);

		mChildrenWorldRect = mAbsoluteViewArea;
	}

	void UITree::CreateVisibleNodeWidget(Node* node, int i)
	{
		int cacheIdx = mVisibleWidgetsCache.FindIdx([=](const VisibleWidgetDef& x) {
			return x.object == node->object && x.position == i; });

		UITreeNode* widget;

		if (cacheIdx < 0)
		{
			widget = CreateTreeNodeWidget();
			UpdateNodeView(node, widget, i);
		}
		else
		{
			widget = mVisibleWidgetsCache[cacheIdx].widget;
			mVisibleWidgetsCache[cacheIdx].widget = nullptr;
		}

		widget->mIsSelected = node->isSelected;
		widget->mNodeDef = node;
		node->widget = widget;
		widget->mParent = this;
		widget->mParentWidget = this;

		mChildren.Add(widget);
		mChildWidgets.Add(widget);
		mDrawingChildren.Add(widget);
	}

	void UITree::UpdateNodeView(Node* node, UITreeNode* widget, int idx)
	{
		float nodeHeight = mNodeWidgetSample->layout->GetMinimalHeight();
		node->widget = widget;

		FillNodeDataByObject(widget, node->object);

		if (GetObjectChilds(node->object).IsEmpty())
		{
			if (widget->mExpandBtn)
				widget->mExpandBtn->Hide(true);
		}
		else
		{
			widget->SetStateForcible("expanded", node->isExpanded);
			if (widget->mExpandBtn)
				widget->mExpandBtn->Show(true);
		}

		widget->mIsSelected = node->isSelected;
		widget->SetStateForcible("selected", node->isSelected);
		widget->SetStateForcible("focused", mIsFocused);

		UpdateNodeWidgetLayout(node, idx);
	}

	void UITree::UpdateNodeWidgetLayout(Node* node, int idx)
	{
		float nodeHeight = mNodeWidgetSample->layout->GetMinimalHeight();
		float dragModeOffset = 0.0f;

		if (mIsDraggingNodes)
			dragModeOffset = mExpandingNodeFunc.Evaluate(node->insertCoef)*nodeHeight;

		node->widget->layout->CopyFrom(UIWidgetLayout::HorStretch(VerAlign::Top, mChildrenOffset*(float)node->level, 0,
																  nodeHeight, GetNodePosition(idx) + dragModeOffset));
	}

	int UITree::GetNodeIndex(float position) const
	{
		int res = 0;
		float nodeHeight = mNodeWidgetSample->layout->GetMinimalHeight();

		if (mExpandingNodeState != ExpandState::None && position > mExpandingNodeBottomPosition)
		{
			res = Math::Min(Math::FloorToInt((position + mExpandingNodeTargetHeight - mExpandingNodeCurrHeight)/nodeHeight),
							Math::Max(0, mAllNodes.Count() - 1));
		}
		else res = Math::Min(Math::FloorToInt(position/nodeHeight), Math::Max(0, mAllNodes.Count() - 1));

		return res;
	}

	float UITree::GetNodePosition(int idx) const
	{
		float nodeHeight = mNodeWidgetSample->layout->GetMinimalHeight();

		float res = (float)idx*nodeHeight;

		if (mExpandingNodeState != ExpandState::None && idx > mExpandingNodeIdx + mExpandingNodeChildsCount)
			res -= mExpandingNodeTargetHeight - mExpandingNodeCurrHeight;

		return res;
	}

	void UITree::ExpandNode(Node* node)
	{
		int position = mAllNodes.Find(node) + 1;

		if (mExpandingNodeState != ExpandState::None && mExpandingNodeIdx != position - 1)
			UpdateNodeExpanding(mExpandNodeTime);

		mExpandedObjects.Add(node->object);

		node->isExpanded = true;

		if (mExpandingNodeState != ExpandState::None && mExpandingNodeIdx == position - 1)
			mExpandingNodeState = ExpandState::Expanding;
		else
		{
			NodesVec newNodes;
			InsertNodes(node, position, &newNodes);

			float nodeHeight = mNodeWidgetSample->layout->GetMinimalHeight();
			float topViewBorder = mScrollPos.y;
			float bottomViewBorder = topViewBorder + mAbsoluteViewArea.Height();
			int idx = position;

			if (idx < mMinVisibleNodeIdx)
			{
				mMinVisibleNodeIdx += newNodes.Count();
				mMaxVisibleNodeIdx += newNodes.Count();
			}
			else if (idx <= mMaxVisibleNodeIdx)
			{
				for (auto node : newNodes)
				{
					float position = (float)idx*nodeHeight;
					if (position > bottomViewBorder)
						break;

					UITreeNode* nodeWidget = mNodesBuf.IsEmpty() ? CreateTreeNodeWidget() : mNodeWidgetsBuf.PopBack();

					node->widget = nodeWidget;
					nodeWidget->mNodeDef = node;
					nodeWidget->mParent = nullptr;

					UpdateNodeView(node, nodeWidget, idx);

					nodeWidget->mParent = this;
					nodeWidget->mParentWidget = this;

					mVisibleNodes.Add(node);
					mChildren.Add(nodeWidget);
					mChildWidgets.Add(nodeWidget);
					mDrawingChildren.Add(nodeWidget);

					idx++;
				}
				mMaxVisibleNodeIdx += newNodes.Count();
			}

			StartExpandingAnimation(ExpandState::Expanding, node, newNodes.Count());
		}

		mIsNeedUdateLayout = true;
	}

	void UITree::CollapseNode(Node* node)
	{
		int idx = mAllNodes.Find(node);

		if (mExpandingNodeState != ExpandState::None && mExpandingNodeIdx != idx)
			UpdateNodeExpanding(mExpandNodeTime);

		mExpandedObjects.Remove(node->object);

		node->isExpanded = false;

		int collapseNodesCount = node->GetChildCount();

		if (mExpandingNodeState != ExpandState::None && mExpandingNodeIdx == idx)
			mExpandingNodeState = ExpandState::Collaping;
		else
			StartExpandingAnimation(ExpandState::Collaping, node, collapseNodesCount);

		mIsNeedUdateLayout = true;
	}

	void UITree::StartExpandingAnimation(ExpandState direction, Node* node, int childrenCount)
	{
		int idx = mAllNodes.Find(node);

		float nodeHeight = mNodeWidgetSample->layout->GetMinimalHeight();

		mExpandingNodeIdx = idx;
		mExpandingNodePosition = mExpandingNodeIdx*nodeHeight + nodeHeight;
		mExpandingNodeTargetHeight = (float)childrenCount*nodeHeight;
		mExpandingNodeBottomPosition = direction == ExpandState::Expanding ? mExpandingNodePosition :
			mExpandingNodePosition + mExpandingNodeTargetHeight;
		mExpandingNodeChildsCount = childrenCount;
		mExpandingNodeCurrCoef = direction == ExpandState::Expanding ? 0.0f : 1.0f;
		mExpandingNodeCurrHeight = direction == ExpandState::Expanding ? 0.0f : mExpandingNodeTargetHeight;
		mExpandingNodeState = direction;
	}

	void UITree::UpdateNodeExpanding(float dt)
	{
		if (mExpandingNodeState == ExpandState::None)
			return;

		if (mExpandingNodeState == ExpandState::Expanding)
		{
			mExpandingNodeCurrCoef += dt/mExpandNodeTime;

			if (mExpandingNodeCurrCoef > 1.0f)
			{
				mExpandingNodeCurrCoef = 1.0f;
				mExpandingNodeState = ExpandState::None;
			}
		}
		else
		{
			mExpandingNodeCurrCoef -= dt/mExpandNodeTime;

			if (mExpandingNodeCurrCoef < 0.0f)
			{
				mExpandingNodeCurrCoef = 0.0f;
				mExpandingNodeState = ExpandState::None;

				mAllNodes[mExpandingNodeIdx]->childs.Clear();

				for (int i = mExpandingNodeIdx + 1; i <= mExpandingNodeIdx + mExpandingNodeChildsCount && i < mAllNodes.Count(); i++)
				{
					Node* node = mAllNodes[i];
					if (node->widget)
					{
						mNodeWidgetsBuf.Add(node->widget);
						mChildren.Remove(node->widget);
						mChildWidgets.Remove(node->widget);
						mDrawingChildren.Remove(node->widget);

						node->widget->mParent = nullptr;
						node->widget->mParentWidget = nullptr;
						node->widget->mNodeDef = nullptr;
						node->widget = nullptr;
					}

					mNodesBuf.Add(node);

					if (node->isSelected)
						mSelectedNodes.Remove(node);
				}

				mAllNodes.RemoveRange(mExpandingNodeIdx + 1, mExpandingNodeIdx + mExpandingNodeChildsCount + 1);
				mExpandingNodeChildsCount = 0;
			}
		}

		float lastExpandBottom = mExpandingNodeBottomPosition;
		mExpandingNodeCurrHeight = mExpandingNodeFunc.Evaluate(mExpandingNodeCurrCoef)*mExpandingNodeTargetHeight;
		mExpandingNodeBottomPosition = mExpandingNodePosition + mExpandingNodeCurrHeight;

		float nodeHeight = mNodeWidgetSample->layout->GetMinimalHeight();
		float offs = mExpandingNodeBottomPosition - lastExpandBottom;
		for (int i = Math::Max(mExpandingNodeIdx + mExpandingNodeChildsCount + 1, mMinVisibleNodeIdx);
			 i <= mMaxVisibleNodeIdx && i < mAllNodes.Count(); i++)
		{
			Node* node = mAllNodes[i];

			if (node->widget)
			{
				node->widget->layout->mData->offsetMin.y -= offs;
				node->widget->layout->mData->offsetMax.y -= offs;
			}
		}

		mIsNeedUdateLayout = true;
	}

	void UITree::CalculateScrollArea()
	{
		const float bottomAdditionalSpace = 100;

		Vec2F offset = mChildrenWorldRect.LeftBottom() - layout->mData->worldRectangle.LeftBottom() -
			mChildrenWorldRect.Size()*layout->pivot;

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		float itemsHeight = (float)mAllNodes.Count()*mNodeWidgetSample->layout->minHeight + bottomAdditionalSpace;

		mScrollArea = RectF(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());

		mScrollArea.top = mAbsoluteViewArea.Height() + roundedScrollPos.y - offset.y;
		mScrollArea.bottom = Math::Min(mScrollArea.bottom,
									   mAbsoluteViewArea.Height() + roundedScrollPos.y - itemsHeight - offset.y);
	}

	void UITree::MoveScrollPosition(const Vec2F& delta)
	{
		mScrollPos += delta;

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		UpdateVisibleNodes();

		mChildrenWorldRect = mAbsoluteViewArea;

		UpdateScrollParams();
	}

	void UITree::OnCursorPressed(const Input::Cursor& cursor)
	{}

	void UITree::UpdateHover(UITreeNode* itemUnderCursor)
	{
		bool hoverVisible = false;
		if (itemUnderCursor)
		{
			mTargetHoverRect = mHoverLayout.Calculate(itemUnderCursor->layout->worldRect);
			hoverVisible = true;
		}

		if (auto hoverState = state["hover"])
			hoverState->SetState(hoverVisible);
		else
			mHoverDrawable->SetEnabled(hoverVisible);
	}

	void UITree::OnCursorMoved(const Input::Cursor& cursor)
	{
		if (!mIsDraggingNodes)
			UpdateHover(GetTreeNodeUnderPoint(cursor.position));
	}

	void UITree::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (!mIsDraggingNodes)
		{
			float a = (float)mAllNodes.Count()*mNodeWidgetSample->layout->GetMinimalHeight();
			float b = mAbsoluteViewArea.top - cursor.position.y + mScrollPos.y;
			bool isCursorUnderNode = a > b;

			if (o2Input.IsKeyDown(VK_SHIFT))
			{
				if (CheckMultipleSelection(cursor.position))
				{
					OnSelectionChanged();
					SetLayoutDirty();
				}
			}
			else if (!o2Input.IsKeyDown(VK_CONTROL) && !isCursorUnderNode)
				DeselectAllObjects();

			mLastClickPos = mAbsoluteViewArea.LeftTop() - cursor.position + mScrollPos;
		}
	}

	void UITree::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		auto node = GetTreeNodeUnderPoint(cursor.position);
		if (mSelectedNodes.Count() < 2)
		{
			DeselectAllObjects();

			if (node)
				SelectObject(node->GetObject());
		}

		Focus();

		OnNodeRBClick(node);
	}

	void UITree::OnCursorPressBreak(const Input::Cursor& cursor)
	{}

	void UITree::OnCursorExit(const Input::Cursor& cursor)
	{
		if (auto hoverState = state["hover"])
			hoverState->SetState(false);
		else
			mHoverDrawable->SetEnabled(false);
	}

	UITreeNode* UITree::GetTreeNodeUnderPoint(const Vec2F& point)
	{
		for (auto node : mVisibleNodes)
		{
			if (node->widget && node->widget->layout->IsPointInside(point))
				return node->widget;
		}

		return nullptr;
	}

	UITreeNode* UITree::CreateTreeNodeWidget()
	{
		UITreeNode* res;

		if (mNodeWidgetsBuf.IsEmpty())
		{
			res = mNodeWidgetSample->CloneAs<UITreeNode>();
			res->ExcludeFromScene();
			res->Show(true);
			res->SetSelectionGroup(this);
			res->messageFallDownListener = this;
			res->mOwnerTree = this;
		}
		else res = mNodeWidgetsBuf.PopBack();

		res->SetInteractable(!mIsDraggingNodes);
		res->mIsSelected = false;
		res->mIsClipped = false;

		return res;
	}

	void UITree::BeginDragging(UITreeNode* node)
	{
		if (mRearrangeType == RearrangeType::Disabled)
			return;

		mDragOffset = node->layout->worldCenter - node->GetCursorPressedPoint();
		mFakeDragNode->Show(true);
		mFakeDragNode->SetInteractable(false);
		mFakeDragNode->SetStateForcible("selected", true);
		mFakeDragNode->SetStateForcible("focused", true);

		FillNodeDataByObject(mFakeDragNode, mSelectedNodes.Last()->object);

		if (mSelectedNodes.Count() > 1)
		{
			if (auto nameLayer = mFakeDragNode->GetLayerDrawableByType<Text>())
				nameLayer->text = String::Format("%i items", mSelectedNodes.Count());
		}

		for (auto node : mVisibleNodes)
			node->widget->SetInteractable(false);

		mIsDraggingNodes = true;
		UpdateNodesStructure();
	}

	void UITree::EndDragging(bool droppedToThis /*= false*/)
	{
		mIsDraggingNodes = false;
		mDragEnded = false;
		mFakeDragNode->Hide(true);

		int idx = 0;
		for (auto node : mAllNodes)
		{
			node->inserting = false;
			node->insertCoef = 0.0f;

			if (node->widget)
			{
				node->widget->SetInteractable(true);
				UpdateNodeWidgetLayout(node, idx);
				node->widget->SetLayoutDirty();
			}

			idx++;
		}

		if (!droppedToThis)
		{
			UpdateNodesStructure();
			SetSelectedObjects(mBeforeDragSelectedItems);
		}
	}

	void UITree::UpdateDraggingGraphics()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		float width = layout->width;
		float height = mNodeWidgetSample->layout->minHeight;
		RectF dragNodeRect = RectF(cursor.position.x - width*0.5f, cursor.position.y + height*0.5f,
								   cursor.position.x + width*0.5f, cursor.position.y - height*0.5f);

		dragNodeRect += mDragOffset;
		mFakeDragNode->layout->worldRect = dragNodeRect;
		mFakeDragNode->UIWidget::UpdateTransform();
	}

	void UITree::UpdateDraggingInsertion()
	{
		bool first = true;
		float ycursor = o2Input.GetCursor(0)->position.y;
		for (auto node : mVisibleNodes)
		{
			if (!node->widget)
				return;

			float y = node->widget->layout->GetWorldCenter().y;
			float h = node->widget->layout->height;
			float cy = first ? y : (y > ycursor ? y + h*mNodeDragIntoZone : y - h*mNodeDragIntoZone);
			first = false;

			node->inserting = cy < ycursor;
		}
	}

	void UITree::UpdateDraggingInsertionAnim(float dt)
	{
		float nodeHeight = mNodeWidgetSample->layout->GetMinimalHeight();
		for (auto node : mVisibleNodes)
		{
			bool changed = false;
			if (node->inserting && node->insertCoef < 1.0f)
			{
				node->insertCoef += dt/mExpandNodeTime*2.0f;

				if (node->insertCoef > 1.0f)
					node->insertCoef = 1.0f;

				changed = true;
			}

			if (!node->inserting && node->insertCoef > 0.0f)
			{
				node->insertCoef -= dt/mExpandNodeTime*2.0f;

				if (node->insertCoef < 0.0f)
					node->insertCoef = 0.0f;

				changed = true;
			}

			if (node->widget && changed)
			{
				UpdateNodeWidgetLayout(node, mAllNodes.Find(node));
				node->widget->SetLayoutDirty();
			}
		}
	}

	void UITree::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		mInsertNodeCandidate = nullptr;
		OnDraggedAbove(group);
	}

	void UITree::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		UITreeNode* currentInsertCandidate = nullptr;
		float nodeDst = FLT_MAX;
		bool first = true;
		for (auto node : mVisibleNodes)
		{
			// 			if (node->widget->mDragSizeCoef < 0.95f)
			// 				continue;
			// 				
			if (!node->widget)
				continue;

			float dst = cursor.position.y - node->widget->layout->GetWorldCenter().y;
			if (!first)
				dst += node->widget->layout->height*mNodeDragIntoZone;

			if (dst > 0 && dst < nodeDst)
			{
				nodeDst = dst;
				currentInsertCandidate = node->widget;
			}

			first = false;
		}

		if (currentInsertCandidate != mInsertNodeCandidate)
		{
			mInsertNodeCandidate = currentInsertCandidate;

			if (mRearrangeType == RearrangeType::Enabled)
				UpdateDraggingInsertion();
		}

		UpdateHover(mInsertNodeCandidate);

		if (mInsertNodeCandidate && !mInsertNodeCandidate->IsUnderPoint(cursor.position))
			mTargetHoverRect += Vec2F::Up()*mInsertNodeCandidate->layout->height;
	}

	void UITree::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		for (auto node : mVisibleNodes)
			node->inserting = false;
	}

	void UITree::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		if (dynamic_cast<UITree*>(group) == nullptr)
			return;

		auto underCursorItem = GetTreeNodeUnderPoint(o2Input.GetCursorPos());

		EndDragging(true);
		OnSelectionChanged();

		UnknownPtrsVec objects;
		UnknownPtr targetParent = UnknownPtr();
		UnknownPtr targetPrevObject = UnknownPtr();
		Node* insertNodeCandidate = mInsertNodeCandidate ? mInsertNodeCandidate->mNodeDef : nullptr;

		if (underCursorItem)
			targetParent = underCursorItem->mNodeDef->object;
		else
		{
			if (insertNodeCandidate)
			{
				targetParent = insertNodeCandidate->parent ? insertNodeCandidate->parent->object : UnknownPtr();
				auto parentChilds = GetObjectChilds(targetParent);

				int idx = parentChilds.Find(insertNodeCandidate->object);
				if (idx > 0)
				{
					targetPrevObject = parentChilds[idx - 1];

					if (mSelectedObjects.Contains(targetPrevObject) && idx > 1)
						targetPrevObject = parentChilds[idx - 2];
				}
				else
					targetPrevObject = UnknownPtr();
			}
			else
			{
				auto rootObjects = GetObjectChilds(UnknownPtr());
				if (rootObjects.Count() > 0)
				{
					targetPrevObject = rootObjects.Last();

					if (mSelectedObjects.ContainsPred([&](Node* x) { return x->object == targetPrevObject; }))
					{
						if (rootObjects.Count() > 1)
							targetPrevObject = rootObjects[rootObjects.Count() - 2];
						else
							targetPrevObject = UnknownPtr();
					}
				}
			}
		}

		for (auto sel : mSelectedObjects)
		{
			bool processing = true;

			UnknownPtr parent = GetObjectParent(sel);
			while (parent)
			{
				if (mSelectedObjects.Contains(parent))
				{
					processing = false;
					break;
				}

				parent = GetObjectParent(parent);
			}

			if (processing)
				objects.Add(sel);
		}

		o2Debug.Log("Drop parent:" + GetObjectDebug(targetParent) + ", prev:" + GetObjectDebug(targetPrevObject));
		OnDraggedObjects(objects, targetParent, targetPrevObject);

		UpdateNodesStructure();
	}

	void UITree::OnDeserialized(const DataNode& node)
	{
		UIScrollArea::OnDeserialized(node);
		mHighlightAnim.SetTarget(mHighlightSprite);
	}

	void UITree::OnSelectionChanged()
	{
		OnNodesSelectionChanged(mSelectedObjects);
	}

	UITreeNode* UITree::GetNodeSample() const
	{
		return mNodeWidgetSample;
	}

	Sprite* UITree::GetHoverDrawable() const
	{
		return mHoverDrawable;
	}

	Sprite* UITree::GetHightlightDrawable() const
	{
		return mHighlightSprite;
	}

	void UITree::SetHightlightAnimation(const Animation& animation)
	{
		mHighlightAnim.SetTarget(mHighlightSprite);
		mHighlightAnim = animation;
	}

	void UITree::SetHightlightLayout(const Layout& layout)
	{
		mHighlightLayout = layout;
	}

	bool UITree::IsScrollable() const
	{
		return true;
	}

	void UITree::SetRearrangeType(RearrangeType type)
	{
		mRearrangeType = type;
	}

	UITree::RearrangeType UITree::GetRearrangeType() const
	{
		return mRearrangeType;
	}

	void UITree::SetMultipleSelectionAvailable(bool available)
	{
		mMultiSelectAvailable = available;

		if (!mMultiSelectAvailable)
		{
			if (!mSelectedObjects.IsEmpty())
			{
				UnknownPtr lastSelected = mSelectedObjects.Last();
				DeselectAllObjects();
				SelectObject(lastSelected);
			}
		}
	}

	bool UITree::IsMultiSelectionAvailable() const
	{
		return mMultiSelectAvailable;
	}

	void UITree::SetNodeExpandTimer(float time)
	{
		mExpandInsertTime = time;
	}

	float UITree::GetNodeExpandTimer() const
	{
		return mExpandInsertTime;
	}

	void UITree::SetChildsNodesOffset(float offset)
	{
		mChildrenOffset = offset;
		SetLayoutDirty();
	}

	float UITree::GetChildsNodesOffset() const
	{
		return mChildrenOffset;
	}

	void UITree::SetHoverLayout(const Layout& layout)
	{
		mHoverLayout = layout;
	}

	bool UITree::IsFocusable() const
	{
		return true;
	}

	bool UITree::IsUnderPoint(const Vec2F& point)
	{
		return UIWidget::IsUnderPoint(point);
	}

	float UITree::Node::GetHeight() const
	{
		float res = 20;
		return res;
	}

	void UITree::Node::SetSelected(bool selected)
	{
		this->isSelected = selected;
		if (widget)
		{
			widget->SetState("selected", selected);
			widget->mIsSelected = selected;

			if (selected)
				widget->OnSelected();
			else
				widget->OnDeselected();
		}
	}

	int UITree::Node::GetChildCount() const
	{
		return childs.Count() + childs.Sum<int>([](Node* x) { return x->GetChildCount(); });
	}

	bool UITree::VisibleWidgetDef::operator==(const VisibleWidgetDef& other) const
	{
		return object == other.object;
	}

}

DECLARE_CLASS(o2::UITree);

DECLARE_CLASS(o2::UITreeNode);

ENUM_META_(o2::UITree::RearrangeType, RearrangeType)
{
	ENUM_ENTRY(Disabled);
	ENUM_ENTRY(Enabled);
	ENUM_ENTRY(OnlyReparent);
}
END_ENUM_META;

ENUM_META_(o2::UITree::ExpandState, ExpandState)
{
	ENUM_ENTRY(Collaping);
	ENUM_ENTRY(Expanding);
	ENUM_ENTRY(None);
}
END_ENUM_META;
