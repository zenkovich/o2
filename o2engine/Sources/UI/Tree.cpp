#include "Tree.h"

#include "Animation/AnimatedFloat.h"
#include "Application/Input.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "UI/Button.h"
#include "UI/UIManager.h"

namespace o2
{
	UITreeNode::UITreeNode()
	{
		onDraw = [&]() { CursorAreaEventsListener::OnDrawn(); };
	}

	UITreeNode::UITreeNode(const UITreeNode& other):
		UIWidget(other)
	{
		onDraw = [&]() { CursorAreaEventsListener::OnDrawn(); };

		UIButton* expandBtn = (UIButton*)GetChild("expandBtn");
		if (expandBtn)
			expandBtn->onClick = [&]() { if (IsExpanded()) Collapse(); else Expand(); o2UI.FocusWidget(mOwnerTree); };

		SetSelectionGroup(other.GetSelectionGroup());

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UITreeNode::~UITreeNode()
	{}

	UITreeNode& UITreeNode::operator=(const UITreeNode& other)
	{
		UIWidget::operator=(other);

		SetSelectionGroup(other.GetSelectionGroup());

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UITreeNode::SetExpanded(bool expanded, bool forcible /*= false*/)
	{
		if (expanded == IsExpanded())
			return;

		if (expanded) mOwnerTree->ExpandNode(this);
		else          mOwnerTree->CollapseNode(this);

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

	void UITreeNode::UpdateTreeLayout(float dt)
	{
		mOwnerTree->mIsNeedUdateLayout = true;
	}

	void UITreeNode::OnCursorDblClicked(const Input::Cursor& cursor)
	{
		if (mOwnerTree)
			mOwnerTree->onItemDblClick(this);
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
		mOwnerTree->EndDragging();
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
		mNodeWidgetSample->layout.minHeight = 20;
		mNodeWidgetSample->AddLayer("caption", nullptr);

		mFakeDragNode  = mnew UITreeNode();
		mHoverDrawable = mnew Sprite();
	}

	UITree::UITree(const UITree& other):
		UIScrollArea(other)
	{
		mRearrangeType        = other.mRearrangeType;
		mMultiSelectAvailable = other.mMultiSelectAvailable;
		mNodeWidgetSample     = other.mNodeWidgetSample->Clone();
		mFakeDragNode         = other.mNodeWidgetSample->Clone();
		mHoverDrawable        = other.mHoverDrawable->Clone();
		mHoverLayout          = other.mHoverLayout;

		RetargetStatesAnimations();
		UpdateLayout();

		const int widgetsBufferInitialSize = 20;
		TreeNodesVec widgets;
		for (int i = 0; i < widgetsBufferInitialSize; i++)
			widgets.Add(CreateTreeNode());

		mNodeWidgetsBuf.Add(widgets);
	}

	UITree::~UITree()
	{
		delete mNodeWidgetSample;
		delete mFakeDragNode;
		delete mHoverDrawable;

		for (auto node : mAllNodes)
			delete node;
	}

	UITree& UITree::operator=(const UITree& other)
	{
		UIScrollArea::operator=(other);

		delete mNodeWidgetSample;
		delete mHoverDrawable;
		delete mFakeDragNode;

		mNodeWidgetSample = other.mNodeWidgetSample->Clone();
		mFakeDragNode     = other.mNodeWidgetSample->Clone();
		mHoverDrawable    = other.mHoverDrawable->Clone();
		mHoverLayout      = other.mHoverLayout;

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UITree::Draw()
	{
		if (mFullyDisabled || mIsClipped)
			return;

		if (mIsDraggingNodes)
			o2UI.DrawWidgetAtTop(mFakeDragNode);

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

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

		DrawDebugFrame();
	}

	void UITree::Update(float dt)
	{
		if (mFullyDisabled || mIsClipped)
			return;

		if (mIsNeedUpdateView)
			UpdateNodes();

		if (mIsNeedUdateLayout)
			UpdateLayout();

		if (mIsNeedUpdateVisibleNodes)
			UpdateVisibleNodes();

		UIScrollArea::Update(dt);

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

	ISelectableDragableObjectsGroup::SelectDragObjectsVec UITree::GetSelectedDragObjects() const
	{
		return SelectDragObjectsVec();
	}

	ISelectableDragableObjectsGroup::SelectDragObjectsVec UITree::GetAllObjects() const
	{
		return SelectDragObjectsVec();
	}

	void UITree::Select(SelectableDragableObject* object)
	{
		bool someSelected = false;
		UITreeNode* uiNode = (UITreeNode*)object;

		if (o2Input.IsKeyDown(VK_SHIFT) && mSelectedNodes.Count() > 0)
		{
			someSelected = CheckMultipleSelection(o2Input.GetCursorPos());
		}
		else if (!o2Input.IsKeyDown(VK_CONTROL))
		{
			DeselectAllObjects();
			someSelected = true;
		}

		if (!mSelectedNodes.ContainsPred([=](Node* x) { return x->widget == uiNode; }))
		{
			someSelected = true;

			Node* node = mAllNodes.FindMatch([=](Node* x) { return x->widget == uiNode; });
			node->SetSelected(true);
			mSelectedNodes.Add(node);
		}

		if (someSelected)
		{
			OnSelectionChanged();
			UpdateLayout();
		}
	}

	void UITree::Deselect(SelectableDragableObject* object)
	{
		UITreeNode* uiNode = (UITreeNode*)object;
		int idx = mSelectedNodes.FindIdx([&](auto x) { return x->widget == uiNode; });

		if (idx < 0)
			return;

		mSelectedNodes[idx]->SetSelected(false);
		mSelectedNodes.RemoveAt(idx);
	}

	void UITree::AddSelectableObject(SelectableDragableObject* object)
	{}

	void UITree::RemoveSelectableObject(SelectableDragableObject* object)
	{}

	void UITree::OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor)
	{
		if (!o2Input.IsKeyDown(VK_SHIFT) && !o2Input.IsKeyDown(VK_CONTROL))
			DeselectAllObjects();

		object->SetSelected(!object->IsSelected());
	}

	void UITree::OnSelectableObjectBeganDragging(SelectableDragableObject* object)
	{
		if (!object->IsSelected())
		{
			DeselectAllObjects();
			Select(object);
		}
	}

	bool UITree::CheckMultipleSelection(const Vec2F& point)
	{
		if (mSelectedNodes.IsEmpty())
			return false;

		float selectionUp   = Math::Max<float>(point.y, mLastClickPos.y - mScrollPos.y);
		float selectionDown = Math::Min<float>(point.y, mLastClickPos.y - mScrollPos.y);

		bool someSelected = false;

		for (auto node : mAllNodes)
		{
			if (!node->widget || mSelectedNodes.Contains(node))
				continue;

			float top = node->widget->layout.absTop;
			float bottom = node->widget->layout.absBottom;

			if ((top > selectionDown && top < selectionUp) || (bottom > selectionDown && bottom < selectionUp))
			{
				node->SetSelected(true);
				mSelectedNodes.Add(node);

				someSelected = true;
			}
		}

		return someSelected;
	}

	void UITree::UpdatePressedNodeExpand(float dt)
	{
		if (o2Input.IsCursorDown())
		{
			if (mRearrangeType != RearrangeType::Disabled && mIsDraggingNodes)
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
					mExpandNodeCandidate->SetState("inserting", false);

				mExpandNodeCandidate = nullptr;
			}
		}

		mExpandInsertTime -= dt;
	}

	void UITree::UpdateView(bool immediately /*= true*/)
	{
		if (immediately)
			UpdateNodes();
		else
			mIsNeedUpdateView = true;
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
		return mSelectedNodes.Select<UnknownPtr>([&](auto x) { return x->object; });
	}

	void UITree::SetSelectedObjects(const Vector<UnknownPtr>& objects)
	{
		for (auto sel : mSelectedNodes)
			sel->SetSelected(false);

		mSelectedNodes.Clear();

		for (auto obj : objects)
		{
			auto node = mAllNodes.FindMatch([=](Node* x) { return x->object == obj; });

			if (!node)
				continue;

			node->SetSelected(true);
			mSelectedNodes.Add(node);
		}

		OnSelectionChanged();
		UpdateLayout();
	}

	void UITree::SelectObject(UnknownPtr object)
	{
		if (!mMultiSelectAvailable)
			DeselectAllObjects();

		if (mSelectedNodes.ContainsPred([=](Node* x) { return x->object == object; }))
			return;

		auto node = mAllNodes.FindMatch([=](Node* x) { return x->object == object; });

		if (!node)
			return;

		node->SetSelected(true);
		mSelectedNodes.Add(node);

		OnSelectionChanged();
		UpdateLayout();
	}

	void UITree::SelectAndExpandObject(UnknownPtr object)
	{
		Vector<UnknownPtr> parentsStack;
		UnknownPtr treeVisibleNodeObject = object;
		while (!GetNode(treeVisibleNodeObject))
		{
			treeVisibleNodeObject = getParentFunc(treeVisibleNodeObject);

			if (!treeVisibleNodeObject)
			{
				SelectObject(object);
				return;
			}

			parentsStack.Add(treeVisibleNodeObject);
		}

		for (int i = parentsStack.Count() - 1; i >= 0; i--)
		{
			auto node = GetNode(parentsStack[i]);
			node->Expand();
		}

		SelectObject(object);
		ScrollTo(object);
	}

	void UITree::DeselectObject(UnknownPtr object)
	{
		int idx = mSelectedNodes.FindIdx([&](auto x) { return x->object == object; });

		if (idx < 0)
			return;

		mSelectedNodes[idx]->SetSelected(false);
		mSelectedNodes.RemoveAt(idx);

		OnSelectionChanged();
		UpdateLayout();
	}

	void UITree::DeselectAllObjects()
	{
		for (auto sel : mSelectedNodes)
			sel->SetSelected(false);

		mSelectedNodes.Clear();
		OnSelectionChanged();
	}

	void UITree::ScrollTo(UnknownPtr object)
	{
		if (!object)
		{
			ResetSroll();
			return;
		}

		int idx = mAllNodes.FindIdx([=](Node* x) { return x->object == object; });

		if (idx >= 0)
			SetScroll(Vec2F(mScrollPos.x, (float)idx*mNodeWidgetSample->layout.minHeight - layout.height*0.5f));
	}

	void UITree::OnObjectCreated(UnknownPtr object, UnknownPtr parent)
	{
		auto parentNodeIdx = mAllNodes.FindIdx([=](Node* x) { return x->object == object; });
		Node* parentNode = nullptr;

		if (parentNodeIdx < 0)
			parentNodeIdx = 0;
		else
			parentNode = mAllNodes[parentNodeIdx];

		auto allParentChilds = getChildsFunc(parent);
		int objectIdx = allParentChilds.Find(object);

		Node* newNode = mnew Node;
		newNode->object = object;
		newNode->parent = parentNode;

		if (parentNode)
			parentNode->childs.Insert(newNode, objectIdx);

		mAllNodes.Insert(newNode, parentNodeIdx + objectIdx);

		mIsNeedUdateLayout = true;
	}

	void UITree::OnObjectRemoved(UnknownPtr object)
	{
		auto nodeIdx = mAllNodes.FindIdx([=](Node* x) { return x->object == object; });

		if (nodeIdx < 0)
			return;

		delete mAllNodes[nodeIdx];
		mAllNodes.RemoveAt(nodeIdx);

		mIsNeedUdateLayout = true;
	}

	void UITree::OnObjectsChanged(const UnknownPtrsVec& objects)
	{
		for (auto object : objects)
		{
			auto node = mAllNodes.FindMatch([=](Node* x) { return x->object == object; });

			if (!node || !node->widget)
				continue;

			setupNodeFunc(node->widget, object);
		}
	}

	void UITree::UpdateNodes()
	{
		mIsNeedUpdateView = false;

		Vector<UnknownPtr> rootObjects = getChildsFunc(UnknownPtr());

		mNodesBuf.Add(mAllNodes);
		mNodeWidgetsBuf.Add(mVisibleNodes.Select<UITreeNode*>([](Node* x) { return x->widget; }));

		mAllNodes.Clear();
		mVisibleNodes.Clear();

		int position = 0;
		for (auto object : rootObjects)
		{
			Node* node = mNodesBuf.IsEmpty() ? mnew Node() : mNodesBuf.PopBack();
			node->object = object;
			node->widget = nullptr;
			node->selected = false;
			node->level = 0;

			mAllNodes.Insert(node, position);
			position++;

			position += InsertNodes(node, position);
		}

		mInvalidVisibleNodeIdx = 0;

		UpdateVisibleNodes();
		UpdateLayout();
	}

	int UITree::InsertNodes(Node* parentNode, int position, NodesVec* newNodes /*= nullptr*/)
	{
		int initialPosition = position;
		mInvalidVisibleNodeIdx = mInvalidVisibleNodeIdx < 0 ? position - 1 : Math::Min(mInvalidVisibleNodeIdx, position - 1);

		if (mExpandedObjects.Contains(parentNode->object))
		{
			auto childObjects = getChildsFunc(parentNode->object);
			for (auto child : childObjects)
			{
				Node* node = mNodesBuf.IsEmpty() ? mnew Node() : mNodesBuf.PopBack();

				node->parent   = parentNode;
				node->object   = child;
				node->widget   = nullptr;
				node->selected = false;
				node->level    = parentNode->level + 1;
				parentNode->childs.Add(node);

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

	void UITree::OnFocused()
	{
		for (auto node : mVisibleNodes)
			node->widget->SetState("focused", true);

		onFocused();
	}

	void UITree::OnUnfocused()
	{
		for (auto node : mVisibleNodes)
			node->widget->SetState("focused", false);

		onUnfocused();
	}

	void UITree::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		UIScrollArea::UpdateLayout(forcible, withChildren);

		if (!layout.mDrivenByParent || forcible)
			UpdateVisibleNodes();

		mIsNeedUdateLayout = false;
	}

	void UITree::UpdateVisibleNodes()
	{
		mIsNeedUpdateVisibleNodes = false;

		int lastMinItemIdx = mMinVisibleNodeIdx;
		int lastMaxItemIdx = mMaxVisibleNodeIdx;

		float itemHeight = mNodeWidgetSample->layout.minHeight;

		if (itemHeight < FLT_EPSILON)
			return;

		mMinVisibleNodeIdx = Math::FloorToInt(mScrollPos.y / itemHeight);
		mMaxVisibleNodeIdx = Math::FloorToInt((mScrollPos.y + mAbsoluteViewArea.Height()) / itemHeight);
		mMaxVisibleNodeIdx = Math::Min(mMaxVisibleNodeIdx, mAllNodes.Count() - 1);

		auto itemsInRange = mAllNodes.Take(mMinVisibleNodeIdx, mMaxVisibleNodeIdx + 1);
		Vector<UITreeNode*> itemsWidgets;
		itemsWidgets.Resize(mMaxVisibleNodeIdx - mMinVisibleNodeIdx + 1);

		TreeNodesVec removingItems;
		for (int i = lastMinItemIdx; i <= lastMaxItemIdx; i++)
		{
			if (i < 0)
				continue;

			int childIdx = i - lastMinItemIdx;
			bool validChild = childIdx >= 0 && childIdx < mChilds.Count();

			if (i < mMinVisibleNodeIdx || i > mMaxVisibleNodeIdx)
			{
				if (validChild)
					removingItems.Add((UITreeNode*)mChilds[childIdx]);
			}
			else
			{
				itemsWidgets[i - mMinVisibleNodeIdx] = validChild ? (UITreeNode*)mChilds[childIdx] : nullptr;
			}
		}

		for (auto item : removingItems)
		{
			item->mNodeDef->widget = nullptr;
			mNodeWidgetsBuf.Add(item);
		}

		mChilds.Clear();

		for (int i = mMinVisibleNodeIdx; i <= mMaxVisibleNodeIdx; i++)
		{
			if (i < 0)
				continue;

			if (i >= itemsInRange.Count())
				break;

			if (i >= lastMinItemIdx && i <= lastMaxItemIdx)
			{
				if (i <= mInvalidVisibleNodeIdx || mInvalidVisibleNodeIdx < 0)
					continue;

				auto itemWidget = itemsWidgets[i - mMinVisibleNodeIdx];
				auto itemNode = itemsInRange[i - mMinVisibleNodeIdx];
				setupNodeFunc(itemWidget, itemNode->object);

				itemWidget->mNodeDef = itemNode;
				itemWidget->layout = UIWidgetLayout::HorStretch(VerAlign::Top, mChildsOffset*(float)itemNode->level, 0,
																itemHeight, itemHeight*(float)i);
			}
			else
			{
				UITreeNode* newItem = CreateTreeNode();
				Node* itemNode = itemsInRange[i - mMinVisibleNodeIdx];

				setupNodeFunc(newItem, itemNode->object);

				itemsWidgets[i - mMinVisibleNodeIdx] = newItem;
				itemNode->widget = newItem;

				newItem->layout = UIWidgetLayout::HorStretch(VerAlign::Top, mChildsOffset*(float)itemNode->level, 0,
															 itemHeight, itemHeight*(float)i);
				newItem->mParent = this;
				newItem->mNodeDef = itemNode;
			}
		}

		mChilds.Add(itemsWidgets.Cast<UIWidget*>().FindAll([](UIWidget* x) { return x != nullptr; }));

		mInvalidVisibleNodeIdx = -1;
	}

	int UITree::GetNodeIndex(float position) const
	{
		return 0;
	}

	float UITree::GetNodePosition(int idx) const
	{
		return 0.0f;
	}

	void UITree::ExpandNode(UITreeNode* node)
	{
		mIsNeedUpdateVisibleNodes = true;

		Node* nodeDef = node->mNodeDef;
		mExpandedObjects.Add(nodeDef->object);

		int position = mAllNodes.Find(nodeDef) + 1;

		NodesVec newNodes;
		InsertNodes(nodeDef, position, &newNodes);

		float nodeHeight = mNodeWidgetSample->layout.GetMinimalHeight();
		float nodeViewPosition = (float)position*nodeHeight;
		float bottomViewBorder = mAbsoluteViewArea.Height() + mScrollPos.y;
		for (auto node : newNodes)
		{
			if (nodeViewPosition > bottomViewBorder)
				break;

			UITreeNode* nodeWidget = mNodesBuf.IsEmpty() ? CreateTreeNode() : mNodeWidgetsBuf.PopBack();

			node->widget = nodeWidget;
			nodeWidget->mNodeDef = node;
			nodeWidget->mParent = nullptr;

			setupNodeFunc(nodeWidget, node->object);
			nodeWidget->layout = UIWidgetLayout::HorStretch(VerAlign::Top, (float)node->level*mChildsOffset, 0,
															nodeHeight, nodeViewPosition);

			nodeWidget->mParent = this;
			mVisibleNodes.Add(node);
			mChilds.Add(nodeWidget);

			nodeViewPosition += nodeHeight;
		}

		mIsNeedUdateLayout = true;
	}

	void UITree::CollapseNode(UITreeNode* node)
	{

	}

	void UITree::CalculateScrollArea()
	{
		mScrollArea = RectF(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());
		mScrollArea.top = Math::Max(mScrollArea.top, (float)mAllNodes.Count()*mNodeWidgetSample->layout.minHeight);
	}

	void UITree::MoveScrollPosition(const Vec2F& delta)
	{
		mScrollPos += delta;

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildsAbsRect = mAbsoluteViewArea + roundedScrollPos;

		UpdateVisibleNodes();

		Vec2F widgetsMove(-delta.x, delta.y);
		for (auto child : mChilds)
			MoveWidgetAndCheckClipping(child, widgetsMove);

		UpdateScrollParams();
		UpdateScrollBarsLayout();
	}

	void UITree::OnCursorPressed(const Input::Cursor& cursor)
	{
		mLastClickPos = cursor.position - mAbsoluteViewArea.LeftTop() + mScrollPos;
	}

	void UITree::UpdateHover(UITreeNode* itemUnderCursor)
	{
		bool hoverVisible = false;
		if (itemUnderCursor)
		{
			mTargetHoverRect = mHoverLayout.Calculate(itemUnderCursor->layout.mAbsoluteRect);
			hoverVisible = true;
		}

		if (auto hoverState = state["hover"])
			hoverState->SetState(hoverVisible);
		else
			mHoverDrawable->SetEnabled(hoverVisible);
	}

	void UITree::OnCursorMoved(const Input::Cursor& cursor)
	{}

	void UITree::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (!mIsDraggingNodes)
		{
			if (o2Input.IsKeyDown(VK_SHIFT))
			{
				if (CheckMultipleSelection(cursor.position))
				{
					OnSelectionChanged();
					UpdateLayout();
				}
			}
			else if (!o2Input.IsKeyDown(VK_CONTROL))
				DeselectAllObjects();
		}
	}

	void UITree::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		auto node = GetTreeNodeUnderPoint(cursor.position);
		if (mSelectedNodes.Count() < 2 && node)
		{
			DeselectAllObjects();
			SelectObject(node->GetObject());
		}

		Focus();

		onItemRBClick(node);
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
			if (node->widget->layout.mAbsoluteRect.IsInside(point))
				return node->widget;
		}

		return nullptr;
	}

	UITreeNode* UITree::CreateTreeNode()
	{
		UITreeNode* res;

		if (mNodeWidgetsBuf.IsEmpty())
		{
			res = mNodeWidgetSample->Clone();
			res->Show(true);
			res->SetSelectionGroup(this);
			res->messageFallDownListener = this;
			res->mOwnerTree = this;
		}
		else res = mNodeWidgetsBuf.PopBack();

		res->SetInteractable(!mIsDraggingNodes);

		return res;
	}

	void UITree::BeginDragging(UITreeNode* node)
	{
		if (mRearrangeType == RearrangeType::Disabled)
			return;

		mBeforeDragSelectedItems = mSelectedNodes;

		mDragOffset = node->layout.mAbsoluteRect.Center() - node->GetCursorPressedPoint();
		mFakeDragNode->Show(true);
		mFakeDragNode->SetInteractable(false);

		setupNodeFunc(mFakeDragNode, mSelectedNodes.Last()->object);

		if (mSelectedNodes.Count() > 1)
		{
			if (auto nameLayer = mFakeDragNode->FindLayer<Text>())
				nameLayer->text = String::Format("%i items", mSelectedNodes.Count());
		}

		for (auto sel : mSelectedNodes)
		{
			if (sel->widget)
				sel->widget->SetState("dragging", true);
		}

		for (auto node : mVisibleNodes)
			node->widget->SetInteractable(false);

		mIsDraggingNodes = true;
	}

	void UITree::EndDragging()
	{
		mIsDraggingNodes = false;
		mFakeDragNode->Hide(true);

		for (auto node : mVisibleNodes)
		{
			node->widget->SetState("inserting", false);
			node->widget->SetState("dragging", false);
			node->widget->SetInteractable(true);
		}
	}

	void UITree::UpdateDraggingGraphics()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		float width = layout.mAbsoluteRect.Width();
		float height = mNodeWidgetSample->layout.minHeight;
		RectF dragNodeRect = RectF(cursor.position.x - width*0.5f, cursor.position.y + height*0.5f,
								   cursor.position.x + width*0.5f, cursor.position.y - height*0.5f);

		dragNodeRect += mDragOffset;
		mFakeDragNode->layout.absRect = dragNodeRect;
		mFakeDragNode->UIWidget::UpdateLayout();
	}

	void UITree::UpdateDraggingInsertion()
	{
		bool first = true;
		float ycursor = o2Input.GetCursor(0)->position.y;
		for (auto node : mVisibleNodes)
		{
			float y = node->widget->layout.mAbsoluteRect.Center().y;
			float h = node->widget->layout.mAbsoluteRect.Height();
			float cy = first ? y : (y > ycursor ? y + h*mNodeDragIntoZone : y - h*mNodeDragIntoZone);
			first = false;

			node->widget->SetState("inserting", cy < ycursor);
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

			float dst = cursor.position.y - node->widget->layout.mAbsoluteRect.Center().y;
			if (!first)
				dst += node->widget->layout.mAbsoluteRect.Height()*mNodeDragIntoZone;

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
			{
				UpdateDraggingInsertion();
			}
		}

		UpdateHover(mInsertNodeCandidate);
		if (mInsertNodeCandidate && !mInsertNodeCandidate->IsUnderPoint(cursor.position))
			mTargetHoverRect += Vec2F::Up()*mInsertNodeCandidate->layout.height;
	}

	void UITree::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		for (auto node : mVisibleNodes)
			node->widget->SetState("inserting", false);
	}

	void UITree::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		OnSelectionChanged();

		mIsDraggingNodes = false;
		mFakeDragNode->Hide(true);

		for (auto node : mVisibleNodes)
		{
			node->widget->SetState("inserting", false);
			node->widget->SetState("dragging", false);
			node->widget->SetInteractable(true);
		}

		UnknownPtrsVec objects;
		UnknownPtr targetParent = UnknownPtr();
		UnknownPtr targetPrevObject = UnknownPtr();
		auto underCursorItem = GetTreeNodeUnderPoint(o2Input.GetCursorPos());

		if (underCursorItem)
		{
			targetParent = underCursorItem->mNodeDef->object;
		}
		else
		{
			if (mInsertNodeCandidate)
			{
				if (mInsertNodeCandidate->mParent && mInsertNodeCandidate->mParent->GetType() == UITreeNode::type)
					targetParent = ((UITreeNode*)mInsertNodeCandidate->mParent)->mNodeDef->object;

				int idx = mInsertNodeCandidate->mParent->mChilds.Find(mInsertNodeCandidate);
				if (idx > 0)
				{
					auto child = mInsertNodeCandidate->mParent->mChilds[idx - 1];
					if (child->GetType() == TypeOf(UITreeNode))
						targetPrevObject = ((UITreeNode*)(child))->mNodeDef->object;
					else
						targetPrevObject = UnknownPtr();
				}
				else targetPrevObject = UnknownPtr();
			}
			else
			{
				if (mChilds.Count() > 0)
				{
					targetPrevObject = ((UITreeNode*)(mChilds.Last()))->mNodeDef->object;

					if (mSelectedNodes.ContainsPred([=](auto x) { return x->object == targetPrevObject; }))
					{
						if (mChilds.Count() > 1)
							targetPrevObject = ((UITreeNode*)(mChilds[mChilds.Count() - 2]))->mNodeDef->object;
						else
							targetPrevObject = UnknownPtr();
					}
				}
			}
		}

		for (auto sel : mSelectedNodes)
		{
			bool processing = true;

			UnknownPtr parent = getParentFunc(sel->object);
			while (parent)
			{
				if (mSelectedNodes.ContainsPred([&](auto x) { return parent == x->object; }))
				{
					processing = false;
					break;
				}

				parent = getParentFunc(parent);
			}

			if (processing)
				objects.Add(sel->object);
		}

		onDraggedObjects(objects, targetParent, targetPrevObject);

		UpdateView();
	}

	void UITree::OnSelectionChanged()
	{
		onItemsSelectionChanged(mSelectedNodes.Select<UnknownPtr>([](Node* x) { return x->object; }));
	}

	UITreeNode* UITree::GetNodeSample() const
	{
		return mNodeWidgetSample;
	}

	Sprite* UITree::GetHoverDrawable() const
	{
		return mHoverDrawable;
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
			if (mSelectedNodes.Count() > 0)
			{
				UnknownPtr lastSelected = mSelectedNodes.Last()->object;
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
		mChildsOffset = offset;
		UpdateLayout();
	}

	float UITree::GetChildsNodesOffset() const
	{
		return mChildsOffset;
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
		this->selected = selected;
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
}