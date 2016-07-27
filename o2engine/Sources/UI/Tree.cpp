#include "Tree.h"

#include "Animation/Animate.h"
#include "Animation/AnimatedFloat.h"
#include "Application/Input.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "Utils/TaskManager.h"
#include "Scene/Actor.h"

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

		mExpandBtn = (UIButton*)GetChild("expandBtn");
		if (mExpandBtn)
			mExpandBtn->onClick = [&]() { if (IsExpanded()) Collapse(); else Expand(); o2UI.FocusWidget(mOwnerTree); };

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
		mNodeWidgetSample->layout.minHeight = 20;
		mNodeWidgetSample->AddLayer("caption", nullptr);

		mFakeDragNode     = mnew UITreeNode();
		mHoverDrawable    = mnew Sprite();
		mHightlightSprite = mnew Sprite();

		mHightlightAnim.SetTarget(mHightlightSprite);
	}

	UITree::UITree(const UITree& other):
		UIScrollArea(other)
	{
		mRearrangeType        = other.mRearrangeType;
		mMultiSelectAvailable = other.mMultiSelectAvailable;
		mNodeWidgetSample     = other.mNodeWidgetSample->Clone();
		mFakeDragNode         = other.mNodeWidgetSample->Clone();
		mHoverDrawable        = other.mHoverDrawable->Clone();
		mHightlightSprite     = other.mHightlightSprite->Clone();

		mHightlightAnim.SetTarget(mHightlightSprite);

		mHoverLayout          = other.mHoverLayout;
		mHightlightLayout     = other.mHightlightLayout;
		mHightlightAnim       = other.mHightlightAnim;

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
		delete mHightlightSprite;

		for (auto node : mAllNodes)
			delete node;
	}

	UITree& UITree::operator=(const UITree& other)
	{
		UIScrollArea::operator=(other);

		delete mNodeWidgetSample;
		delete mHoverDrawable;
		delete mFakeDragNode;
		delete mHightlightSprite;

		mRearrangeType        = other.mRearrangeType;
		mMultiSelectAvailable = other.mMultiSelectAvailable;
		mNodeWidgetSample     = other.mNodeWidgetSample->Clone();
		mFakeDragNode         = other.mNodeWidgetSample->Clone();
		mHoverDrawable        = other.mHoverDrawable->Clone();
		mHightlightSprite     = other.mHightlightSprite->Clone();

		mHightlightAnim.SetTarget(mHightlightSprite);

		mHoverLayout          = other.mHoverLayout;
		mHightlightLayout     = other.mHightlightLayout;
		mHightlightAnim       = other.mHightlightAnim;

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

		if (mExpandingNodeState == ExpandState::None)
		{
			for (auto child : mChilds)
			{
				child->Draw();

// 				if (child->GetType().IsBasedOn(TypeOf(UITreeNode)))
// 				{
// 					String dbg = getDbgString(((UITreeNode*)child)->mNodeDef->object);
// 					o2Debug.DrawText(child->layout.GetAbsoluteRect().Center(), dbg);
// 				}
			}
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
		mHightlightSprite->Draw();
		//o2Render.DrawCross(mHightlightSprite->position);

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

		if (mHightlightAnim.IsPlaying())
		{
			if (mHightlighNode && mHightlighNode->widget)
			{
				mHightlightSprite->SetScale(Vec2F(1.0f, 1.0f));
				mHightlightSprite->SetRect(mHightlightLayout.Calculate(mHightlighNode->widget->layout.absRect));
			}

			mHightlightAnim.Update(dt);

			if (!mHightlightAnim.IsPlaying())
				mHightlighNode = nullptr;
		}

		UpdateNodeExpanding(dt);

		if (mDragEnded)
			EndDragging();

		if (mIsDraggingNodes)
			UpdateDraggingInsertionAnim(dt);

		if (mIsNeedUpdateView)
			UpdateNodesStructure();

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

		if (someSelected)
			OnSelectionChanged();
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

		Vec2F currClickPos = mAbsoluteViewArea.LeftTop() - point + mScrollPos;

		float selectionUp   = Math::Max<float>(currClickPos.y, mLastClickPos.y);
		float selectionDown = Math::Min<float>(currClickPos.y, mLastClickPos.y);

		bool someSelected = false;

		int idx = 0;
		float nodeHeight = mNodeWidgetSample->layout.GetMinimalHeight();
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

	void UITree::UpdateView(bool immediately /*= true*/)
	{
		if (immediately)
			UpdateNodesStructure();
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
		return mSelectedObjects;
	}

	void UITree::SetSelectedObjects(const Vector<UnknownPtr>& objects)
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
		UpdateLayout();
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
		UpdateLayout();
	}

	void UITree::SelectAndHightlightObject(UnknownPtr object)
	{
		ScrollToAndHightlight(object);
		SelectObject(object);
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
		UpdateLayout();
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
			ResetSroll();
			return;
		}

		ExpandParentObjects(object);

		int idx = mAllNodes.FindIdx([=](Node* x) { return x->object == object; });

		if (idx >= 0)
			SetScroll(Vec2F(mScrollPos.x, (float)idx*mNodeWidgetSample->layout.minHeight - layout.height*0.5f));
	}

	void UITree::ScrollToAndHightlight(UnknownPtr object)
	{
		if (!object)
		{
			ResetSroll();
			return;
		}

		ExpandParentObjects(object);

		int idx = mAllNodes.FindIdx([=](Node* x) { return x->object == object; });

		if (idx >= 0)
		{
			float position = (float)idx*mNodeWidgetSample->layout.minHeight;
			float scroll = position - layout.height*0.5f;
			SetScroll(Vec2F(mScrollPos.x, scroll));

			mHightlighNode = mAllNodes[idx];
			mHightlightAnim.RewindAndPlay();
		}
	}

	void UITree::ExpandParentObjects(UnknownPtr object)
	{
		Vector<UnknownPtr> parentsStack;
		UnknownPtr current = getParentFunc(object);
		while (current)
		{
			parentsStack.Add(current);
			current = getParentFunc(current);
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
		UpdateLayout();
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
			auto node = mAllNodes.FindMatch([=](Node* x) { return x->object == object; });

			if (!node || !node->widget)
				continue;

			setupNodeFunc(node->widget, object);
		}
	}

	void UITree::UpdateNodesStructure()
	{
		mIsNeedUpdateView = false;

		if (mExpandingNodeState != ExpandState::None)
		{
			UpdateNodeExpanding(mExpandNodeTime);

			for (auto child : mChilds)
				child->UpdateLayout();
		}

		Vector<UnknownPtr> rootObjects = getChildsFunc(UnknownPtr());

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
		mChilds.Clear();
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

		UpdateLayout();

		o2Debug.Log("Nodes structure updated");

		if (o2Input.IsKeyDown('A'))
			o2Debug.Log("dd");
	}

	int UITree::InsertNodes(Node* parentNode, int position, NodesVec* newNodes /*= nullptr*/)
	{
		int initialPosition = position;

		if (mExpandedObjects.Contains(parentNode->object))
		{
			auto childObjects = getChildsFunc(parentNode->object);
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

		node->parent     = parent;
		node->object     = object;
		node->widget     = nullptr;
		node->isSelected = mSelectedObjects.Contains(object);
		node->isExpanded = mExpandedObjects.Contains(object);
		node->level      = parent ? parent->level + 1 : 0;

		node->id = getDbgString(object);

		if (parent)
			parent->childs.Add(node);

		if (node->isSelected)
			mSelectedNodes.Add(node);

		return node;
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

	void UITree::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		UIScrollArea::UpdateLayout(forcible, false);

		if ((!layout.mDrivenByParent || forcible) && withChildren)
			UpdateVisibleNodes();

		mIsNeedUdateLayout = false;
	}

	void UITree::UpdateVisibleNodes()
	{
		mIsNeedUpdateVisibleNodes = false;

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
					mChilds.Remove(node->widget);
					node->widget->mParent = nullptr;
					node->widget->mNodeDef = nullptr;
					node->widget = nullptr;
				}
			}
		}

		float nodeHeight = mNodeWidgetSample->layout.minHeight;
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
				node->widget->UpdateLayout(true);
		}

		mNodeWidgetsBuf.Add(mVisibleWidgetsCache.FindAll([](const VisibleWidgetDef& x) { return x.widget != nullptr; })
							.Select<UITreeNode*>([](const VisibleWidgetDef& x) { return x.widget; }));
		mVisibleWidgetsCache.Clear();

		if (mIsDraggingNodes)
			OnDraggedAbove(this);
	}

	void UITree::CreateVisibleNodeWidget(Node* node, int i)
	{
		int cacheIdx = mVisibleWidgetsCache.FindIdx([=](const VisibleWidgetDef& x) {
			return x.object == node->object && x.position == i; });

		UITreeNode* widget;

		if (cacheIdx < 0)
		{
			widget = CreateTreeNode();
			UpdateNode(node, widget, i);
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
		mChilds.Add(widget);
	}

	void UITree::UpdateNode(Node* node, UITreeNode* widget, int idx)
	{
		float nodeHeight = mNodeWidgetSample->layout.GetMinimalHeight();
		node->widget = widget;

		setupNodeFunc(widget, node->object);

		if (getChildsFunc(node->object).IsEmpty())
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

		widget->SetStateForcible("selected", node->isSelected);
		widget->SetStateForcible("focused", mIsFocused);

		UpdateNodeWidgetLayout(node, idx);
	}

	void UITree::UpdateNodeWidgetLayout(Node* node, int idx)
	{
		float nodeHeight = mNodeWidgetSample->layout.GetMinimalHeight();
		float dragModeOffset = 0.0f;

		if (mIsDraggingNodes)
			dragModeOffset = mExpandingNodeFunc.Evaluate(node->insertCoef)*nodeHeight;

		node->widget->layout.CopyFrom(UIWidgetLayout::HorStretch(VerAlign::Top, mChildsOffset*(float)node->level, 0,
									                             nodeHeight, GetNodePosition(idx) + dragModeOffset));
	}

	int UITree::GetNodeIndex(float position) const
	{
		int res = 0;
		float nodeHeight = mNodeWidgetSample->layout.GetMinimalHeight();

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
		float nodeHeight = mNodeWidgetSample->layout.GetMinimalHeight();

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

			float nodeHeight = mNodeWidgetSample->layout.GetMinimalHeight();
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

					UITreeNode* nodeWidget = mNodesBuf.IsEmpty() ? CreateTreeNode() : mNodeWidgetsBuf.PopBack();

					node->widget = nodeWidget;
					nodeWidget->mNodeDef = node;
					nodeWidget->mParent = nullptr;

					UpdateNode(node, nodeWidget, idx);

					nodeWidget->mParent = this;
					mVisibleNodes.Add(node);
					mChilds.Add(nodeWidget);

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

		float nodeHeight = mNodeWidgetSample->layout.GetMinimalHeight();

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
						mChilds.Remove(node->widget);
						node->widget->mParent = nullptr;
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

		float nodeHeight = mNodeWidgetSample->layout.GetMinimalHeight();
		float offs = mExpandingNodeBottomPosition - lastExpandBottom;
		for (int i = Math::Max(mExpandingNodeIdx + mExpandingNodeChildsCount + 1, mMinVisibleNodeIdx);
			 i <= mMaxVisibleNodeIdx && i < mAllNodes.Count(); i++)
		{
			Node* node = mAllNodes[i];

			if (node->widget)
			{
				node->widget->layout.mOffsetMin.y -= offs;
				node->widget->layout.mOffsetMax.y -= offs;
			}
		}

		mIsNeedUdateLayout = true;
	}

	void UITree::CalculateScrollArea()
	{
		mScrollArea = RectF(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());
		mScrollArea.bottom = Math::Min(mScrollArea.bottom,
									   mScrollArea.top - (float)mAllNodes.Count()*mNodeWidgetSample->layout.minHeight - 100.0f);
	}

	void UITree::MoveScrollPosition(const Vec2F& delta)
	{
		mScrollPos += delta;

		UpdateLayout(true, true);
	}

	void UITree::OnCursorPressed(const Input::Cursor& cursor)
	{}

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
	{
		if (!mIsDraggingNodes)
			UpdateHover(GetTreeNodeUnderPoint(cursor.position));
	}

	void UITree::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (!mIsDraggingNodes)
		{
			float a = (float)mAllNodes.Count()*mNodeWidgetSample->layout.GetMinimalHeight();
			float b = mAbsoluteViewArea.top - cursor.position.y + mScrollPos.y;
			bool isCursorUnderNode = a > b;

			if (o2Input.IsKeyDown(VK_SHIFT))
			{
				if (CheckMultipleSelection(cursor.position))
				{
					OnSelectionChanged();
					UpdateLayout();
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
			if (node->widget && node->widget->layout.mAbsoluteRect.IsInside(point))
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
		res->mIsSelected = false;

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
		mFakeDragNode->SetStateForcible("selected", true);
		mFakeDragNode->SetStateForcible("focused", true);

		setupNodeFunc(mFakeDragNode, mSelectedNodes.Last()->object);

		if (mSelectedNodes.Count() > 1)
		{
			if (auto nameLayer = mFakeDragNode->FindLayer<Text>())
				nameLayer->text = String::Format("%i items", mSelectedNodes.Count());
		}

		for (auto node : mVisibleNodes)
			node->widget->SetInteractable(false);

		mIsDraggingNodes = true;
		UpdateNodesStructure();
	}

	void UITree::EndDragging(bool updateNodes /*= true*/)
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
				node->widget->UpdateLayout();
			}

			idx++;
		}

		if (updateNodes)
			UpdateNodesStructure();
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
			if (!node->widget)
				return;

			float y = node->widget->layout.mAbsoluteRect.Center().y;
			float h = node->widget->layout.mAbsoluteRect.Height();
			float cy = first ? y : (y > ycursor ? y + h*mNodeDragIntoZone : y - h*mNodeDragIntoZone);
			first = false;

			node->inserting = cy < ycursor;
		}
	}

	void UITree::UpdateDraggingInsertionAnim(float dt)
	{
		float nodeHeight = mNodeWidgetSample->layout.GetMinimalHeight();
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
				node->widget->UpdateLayout(true, true);
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
			o2Debug.Log("Insert candidate %s", mInsertNodeCandidate ? getDbgString(mInsertNodeCandidate->mNodeDef->object):String("null"));

			if (mRearrangeType == RearrangeType::Enabled)
				UpdateDraggingInsertion();
		}

		UpdateHover(mInsertNodeCandidate);

		if (mInsertNodeCandidate && !mInsertNodeCandidate->IsUnderPoint(cursor.position))
			mTargetHoverRect += Vec2F::Up()*mInsertNodeCandidate->layout.height;
	}

	void UITree::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		for (auto node : mVisibleNodes)
			node->inserting = false;
	}

	void UITree::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		auto underCursorItem = GetTreeNodeUnderPoint(o2Input.GetCursorPos());

		EndDragging(false);
		//OnSelectionChanged();

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
				auto parentChilds = getChildsFunc(targetParent);

				int idx = parentChilds.Find(insertNodeCandidate->object);
				if (idx > 0)
					targetPrevObject = parentChilds[idx - 1];
				else
					targetPrevObject = UnknownPtr();
			}
			else
			{
				auto rootObjects = getChildsFunc(UnknownPtr());
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

			UnknownPtr parent = getParentFunc(sel);
			while (parent)
			{
				if (mSelectedObjects.Contains(parent))
				{
					processing = false;
					break;
				}

				parent = getParentFunc(parent);
			}

			if (processing)
				objects.Add(sel);
		}

		o2Debug.Log("Drop parent:%s, prev:%s", getDbgString(targetParent), getDbgString(targetPrevObject));
		onDraggedObjects(objects, targetParent, targetPrevObject);

		o2Scene.CheckChangedActors();
		UpdateNodesStructure();
	}

	void UITree::OnDeserialized(const DataNode& node)
	{
		UIScrollArea::OnDeserialized(node);
		mHightlightAnim.SetTarget(mHightlightSprite);
	}

	void UITree::OnSelectionChanged()
	{
		onItemsSelectionChanged(mSelectedObjects);
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
		return mHightlightSprite;
	}

	void UITree::SetHightlightAnimation(const Animation& animation)
	{
		mHightlightAnim.SetTarget(mHightlightSprite);
		mHightlightAnim = animation;
	}

	void UITree::SetHightlightLayout(const Layout& layout)
	{
		mHightlightLayout = layout;
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
		mChildsOffset = offset;
		UpdateLayout();
	}

	float UITree::GetChildsNodesOffset() const
	{
		return mChildsOffset;
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