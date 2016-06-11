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
		AddState("visible", Animation::EaseInOut<float>(this, &transparency, 0.0f, 1.0f, 0.1f));
		AddState("inserting", Animation::EaseInOut<float>(this, &mInsertSizeCoef, 0.0f, 1.0f, 0.1f));

		Animation dragStateAnim = Animation::EaseInOut<float>(this, &mDragSizeCoef, 1.0f, 0.0f, 0.1f);
		*dragStateAnim.AddAnimationValue(&transparency) = AnimatedValue<float>::EaseInOut(1.0f, 0.0f, 0.01f);
		AddState("dragging", dragStateAnim);
	}

	UITreeNode::UITreeNode(const UITreeNode& other):
		UIWidget(other)
	{

		mExpandedState = GetStateObject("expanded");
		if (mExpandedState)
			mExpandedState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);

		if (auto draggingState = GetStateObject("dragging"))
			draggingState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);

		if (auto insertingState = GetStateObject("inserting"))
			insertingState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);

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

		mExpandedState = GetStateObject("expanded");
		if (mExpandedState)
			mExpandedState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);

		if (auto draggingState = GetStateObject("dragging"))
			draggingState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);

		if (auto insertingState = GetStateObject("inserting"))
			insertingState->animation.onUpdate += Function<void(float)>(this, &UITreeNode::UpdateTreeLayout);

		SetSelectionGroup(other.GetSelectionGroup());

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UITreeNode::Draw()
	{
		if (mFullyDisabled || mIsClipped)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		//if (o2Input.IsKeyDown(VK_F1))
		//	o2Debug.DrawText(layout.absRect->Center(), (String)mIsSelected);

		IDrawable::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

		bool visibleChilds = mExpandCoef > FLT_EPSILON;
		bool clipping = mExpandCoef > FLT_EPSILON && mExpandCoef < 1.0f - FLT_EPSILON;

		if (clipping)
		{
			o2Render.EnableScissorTest(RectF(-o2Render.GetResolution().x*0.5f, layout.mAbsoluteRect.top - mNodeDef->GetHeight(),
									         o2Render.GetResolution().x*0.5f, layout.mAbsoluteRect.top));
		}

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

		DrawDebugFrame();
	}

	void UITreeNode::SetExpanded(bool expanded, bool forcible /*= false*/)
	{
		if (expanded == IsExpanded())
			return;

		if (expanded)
			mOwnerTree->UpdateNodes(mNodeDef);

		if (mExpandedState)
		{
			if (forcible)
				mExpandedState->SetStateForcible(expanded);
			else
				mExpandedState->SetState(expanded);
		}
		else mExpandCoef = expanded ? 1.0f : 0.0f;
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
		mNodeSample = mnew UITreeNode();
		mNodeSample->layout.minHeight = 20;
		mNodeSample->AddLayer("caption", nullptr);

		mFakeDragNode  = mnew UITreeNode();
		mHoverDrawable = mnew Sprite();
	}

	UITree::UITree(const UITree& other):
		UIScrollArea(other)
	{
		mRearrangeType        = other.mRearrangeType;
		mMultiSelectAvailable = other.mMultiSelectAvailable;
		mNodeSample           = other.mNodeSample->Clone();
		mFakeDragNode         = other.mNodeSample->Clone();
		mHoverDrawable        = other.mHoverDrawable->Clone();
		mHoverLayout          = other.mHoverLayout;

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UITree::~UITree()
	{
		delete mNodeSample;
		delete mFakeDragNode;
		delete mHoverDrawable;

		for (auto node : mAllNodes)
			delete node;
	}

	UITree& UITree::operator=(const UITree& other)
	{
		UIScrollArea::operator=(other);

		delete mNodeSample;
		delete mHoverDrawable;
		delete mFakeDragNode;

		mNodeSample    = other.mNodeSample->Clone();
		mFakeDragNode  = other.mNodeSample->Clone();
		mHoverDrawable = other.mHoverDrawable->Clone();
		mHoverLayout   = other.mHoverLayout;

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

		if (mNeedUpdateView)
			UpdateRootNodes();

		if (mIsNeedUdateLayout)
			UpdateLayout();

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
			if (!node->widget)
				continue;

			float top = node->widget->layout.absTop;
			float bottom = node->widget->layout.absBottom;

			if ((top > selectionDown && top < selectionUp) || (bottom > selectionDown && bottom < selectionUp))
			{
				if (mSelectedNodes.FindIdx([&](auto x) { return x.node == node; }) < 0)
				{
					node->SetSelected(true);
					mSelectedNodes.Add(node);

					someSelected = true;
				}
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
			UpdateRootNodes();
		else
			mNeedUpdateView = true;
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
		return mSelectedNodes.Select<UnknownPtr>([&](auto x) { return x.object; });
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
		int idx = mSelectedNodes.FindIdx([&](auto x) { return x.object == object; });

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
			SetScroll(Vec2F(mScrollPos.x, (float)idx*mNodeSample->layout.minHeight - layout.height*0.5f));
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

			if (!node->widget)
				continue;

			setupNodeFunc(node->widget, object);
		}
	}

	void UITree::UpdateRootNodes()
	{
		mNeedUpdateView = false;

		Vector<UnknownPtr> rootObjects = getChildsFunc(UnknownPtr());

		for (auto node : mAllNodes)
		{
			if (node->parent)
				continue;

			int rootObjectIdx = rootObjects.Find(node->object);

			if (rootObjectIdx < 0)

		}

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
				if (updateChilds)
					node->UpdateView();

				continue;
			}

			UITreeNode* newNode = CreateTreeNode();
			newNode->mObject = obj;
			newNode->mOwnerTree = this;
			newNode->mParent = this;
			newNode->SetExpanded(mExpandedObjects.Contains(obj), true);
			mChilds.Add(newNode);

			mAllNodes.Add(newNode);

			if (updateChilds)
				newNode->UpdateView();
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

	void UITree::UpdateNodes(Node* node)
	{

	}

	void UITree::OnFocused()
	{
		for (auto& sel : mSelectedNodes)
			sel.selectionSprite->SetColor(mSelectedColor);

		onFocused();
	}

	void UITree::OnUnfocused()
	{
		for (auto& sel : mSelectedNodes)
			sel.selectionSprite->SetColor(mUnselectedColor);

		onUnfocused();
	}

	void UITree::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (CheckIsLayoutDrivenByParent(forcible))
			return;

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

		if (withChildren)
			UpdateChildrenLayouts(true);

		UpdateScrollParams();

		RectF _mChildsAbsRect = mChildsAbsRect;
		mChildsAbsRect = layout.mAbsoluteRect;

		if (mOwnHorScrollBar)
			mHorScrollBar->UpdateLayout(true);

		if (mOwnVerScrollBar)
			mVerScrollBar->UpdateLayout(true);

		mChildsAbsRect = _mChildsAbsRect;

		for (auto sel : mSelectedNodes)
		{
			if (sel.node)
			{
				sel.selectionSprite->SetRect(mHoverLayout.Calculate(sel.node->layout.GetAbsoluteRect()));
				sel.selectionSprite->SetEnabled(true);
				sel.selectionSprite->transparency = sel.node->resTransparency;
			}
			else sel.selectionSprite->SetEnabled(false);
		}

		mIsNeedUdateLayout = false;
	}

	void UITree::UpdateVisibleNodes()
	{

	}

	void UITree::CalculateScrollArea()
	{
		mScrollArea = RectF(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());

		Vec2F thisWorldPos = mChildsAbsRect.LeftBottom();
		float downHeight = mNodeSample->layout.mMinSize.y*3.0f;
		for (auto child : mAllNodes)
		{
			RectF rt = child->layout.mAbsoluteRect - thisWorldPos;

			mScrollArea.left   = Math::Min(mScrollArea.left, rt.left);
			mScrollArea.bottom = Math::Min(mScrollArea.bottom, rt.bottom - downHeight);
			mScrollArea.right  = Math::Max(mScrollArea.right, rt.right);
			mScrollArea.top    = Math::Max(mScrollArea.top, rt.top);
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
		res->SetSelectionGroup(this);
		res->messageFallDownListener = this;
		res->SetInteractable(!mIsDraggingNodes);

		return res;
	}

	void UITree::FreeTreeNode(UITreeNode* node)
	{
		node->Hide(true);
		mNodesPool.Add(node);
	}

	Sprite* UITree::CreateSelectionSprite()
	{
		Sprite* res = nullptr;

		if (mSelectionSpritesPool.Count() == 0)
		{
			for (int i = 0; i < mSelectionSpritesPoolResizeCount; i++)
				mSelectionSpritesPool.Add(mSelectedDrawable->Clone());
		}

		res = mSelectionSpritesPool.PopBack();
		res->SetColor(mIsFocused ? mSelectedColor : mUnselectedColor);

		return res;
	}

	void UITree::FreeSelectionSprite(Sprite* sprite)
	{
		mSelectionSpritesPool.Add(sprite);
	}

	void UITree::CheckSelectedNodes()
	{
		for (auto& sel : mSelectedNodes)
		{
			if (sel.node)
			{
				if (mAllNodes.Find(sel.node) < 0)
				{
					sel.node->mIsSelected = false;
					sel.node->OnDeselected();
					sel.node = nullptr;
				}
			}
			else
			{
				sel.node = GetNode(sel.object);

				if (sel.node)
				{
					sel.node->mIsSelected = true;
					sel.node->OnSelected();
				}
			}
		}

		mWaitSelectionsUpdate = false;
	}

	void UITree::BeginDragging(UITreeNode* node)
	{
		if (mRearrangeType == RearrangeType::Disabled)
			return;

		mBeforeDragSelectedItems = mSelectedNodes;

		mDragOffset = node->layout.mAbsoluteRect.Center() - node->GetCursorPressedPoint();
		mFakeDragNode->Show(true);
		mFakeDragNode->SetInteractable(false);

		setupNodeFunc(mFakeDragNode, mSelectedNodes.Last().object);

		if (mSelectedNodes.Count() > 1)
		{
			if (auto nameLayer = mFakeDragNode->FindLayer<Text>())
				nameLayer->text = String::Format("%i items", mSelectedNodes.Count());
		}

		for (auto sel : mSelectedNodes)
		{
			if (sel.node)
				sel.node->SetState("dragging", true);
		}

		for (auto node : mAllNodes)
			node->SetInteractable(false);

		mIsDraggingNodes = true;
	}

	void UITree::EndDragging()
	{
		mIsDraggingNodes = false;
		mFakeDragNode->Hide(true);

		for (auto node : mAllNodes)
		{
			node->SetState("inserting", false);
			node->SetState("dragging", false);
			node->SetInteractable(true);
		}
	}

	void UITree::UpdateDraggingGraphics()
	{
		const Input::Cursor& cursor = *o2Input.GetCursor(0);

		float width = layout.mAbsoluteRect.Width();
		float height = mNodeSample->layout.minHeight;
		RectF dragNodeRect = RectF(cursor.position.x - width*0.5f, cursor.position.y + height*0.5f,
								   cursor.position.x + width*0.5f, cursor.position.y - height*0.5f);

		dragNodeRect += mDragOffset;
		mFakeDragNode->layout.absRect = dragNodeRect;
		mFakeDragNode->UIWidget::UpdateLayout();

		mFakeDragNodeBack->SetRect(mHoverLayout.Calculate(dragNodeRect));
	}

	void UITree::UpdateDraggingInsertion()
	{
		bool first = true;
		float ycursor = o2Input.GetCursor(0)->position.y;
		for (auto node : mAllNodes)
		{
			float y = node->layout.mAbsoluteRect.Center().y;
			float h = node->layout.mAbsoluteRect.Height();
			float cy = first ? y : (y > ycursor ? y + h*mNodeDragIntoZone : y - h*mNodeDragIntoZone);
			first = false;

			node->SetState("inserting", cy < ycursor);
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
		for (auto node : mAllNodes)
		{
			if (node->mDragSizeCoef < 0.95f)
				continue;

			float dst = cursor.position.y - node->layout.mAbsoluteRect.Center().y;
			if (!first)
				dst += node->layout.mAbsoluteRect.Height()*mNodeDragIntoZone;

			if (dst > 0 && dst < nodeDst)
			{
				nodeDst = dst;
				currentInsertCandidate = node;
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
		for (auto node : mAllNodes)
			node->SetState("inserting", false);
	}

	void UITree::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		OnSelectionChanged();

		mIsDraggingNodes = false;
		mFakeDragNode->Hide(true);

		for (auto node : mAllNodes)
		{
			node->SetState("inserting", false);
			node->SetState("dragging", false);
			node->SetInteractable(true);
		}

		UnknownPtrsVec objects;
		UnknownPtr targetParent = nullptr;
		UnknownPtr targetPrevObject = nullptr;
		auto underCursorItem = GetTreeNodeUnderPoint(o2Input.GetCursorPos());

		if (underCursorItem)
		{
			targetParent = underCursorItem->mObject;
		}
		else
		{
			if (mInsertNodeCandidate)
			{
				if (mInsertNodeCandidate->mParent && mInsertNodeCandidate->mParent->GetType() == UITreeNode::type)
					targetParent = ((UITreeNode*)mInsertNodeCandidate->mParent)->mObject;

				int idx = mInsertNodeCandidate->mParent->mChilds.Find(mInsertNodeCandidate);
				if (idx > 0)
				{
					auto child = mInsertNodeCandidate->mParent->mChilds[idx - 1];
					if (child->GetType() == TypeOf(UITreeNode))
						targetPrevObject = ((UITreeNode*)(child))->mObject;
					else
						targetPrevObject = nullptr;
				}
				else targetPrevObject = nullptr;
			}
			else
			{
				if (mChilds.Count() > 0)
				{
					targetPrevObject = ((UITreeNode*)(mChilds.Last()))->mObject;

					if (mSelectedNodes.ContainsPred([=](auto x) { return x.object == targetPrevObject; }))
					{
						if (mChilds.Count() > 1)
							targetPrevObject = ((UITreeNode*)(mChilds[mChilds.Count() - 2]))->mObject;
						else
							targetPrevObject = nullptr;
					}
				}
			}
		}

		for (auto sel : mSelectedNodes)
		{
			bool processing = true;

			UnknownPtr parent = getParentFunc(sel.object);
			while (parent)
			{
				if (mSelectedNodes.ContainsPred([&](auto x) { return parent == x.object; }))
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

		UpdateView();
	}

	float UITree::GetTreeNodeOffset(UITreeNode* node)
	{
		UIWidget* current = node;

		float res = 0;
		while (current->mParent)
		{
			for (auto child : current->mParent->mChilds)
			{
				if (child == current)
					break;

				if (child->GetType() != TypeOf(UITreeNode))
					continue;

				UITreeNode* treeNodeChild = (UITreeNode*)child;
				res += treeNodeChild->GetCurrentHeight(true);
			}

			current = current->mParent;
			if (current == this)
				break;

			res += current->layout.minHeight;
		}

		return res;
	}

	void UITree::OnSelectionChanged()
	{
		onItemsSelectionChanged(mSelectedNodes.Select<UnknownPtr>([](auto x) { return x.object; }));
	}

	bool UITree::SelectedNode::operator==(const SelectedNode& other) const
	{
		return object == other.object;
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
				UnknownPtr lastSelected = mSelectedNodes.Last().object;
				DeselectAllObjects();
				SelectObject(lastSelected);
			}
		}
	}

	bool UITree::IsMultiSelectionAvailable() const
	{
		return mMultiSelectAvailable;
	}

	void UITree::SetSelectedColor(const Color4& color)
	{
		mSelectedColor = color;
	}

	Color4 UITree::GetSelectedColor() const
	{
		return mSelectedColor;
	}

	void UITree::SetUnselectedColor(const Color4& color)
	{
		mUnselectedColor = color;
	}

	Color4 UITree::GetUnselectedColor() const
	{
		return mUnselectedColor;
	}

	void UITree::SetHoverColor(const Color4& color)
	{
		mHoverColor = color;
	}

	Color4 UITree::GetHoverColor() const
	{
		return mHoverColor;
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

}