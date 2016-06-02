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

		SetSelectionGroup(other.GetSelectionGroup());

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

		//if (o2Input.IsKeyDown(VK_F1))
		//	o2Debug.DrawText(layout.absRect->Center(), (String)mIsSelected);

		IDrawable::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

		bool visibleChilds = mExpandCoef > FLT_EPSILON;
		bool clipping = mExpandCoef > FLT_EPSILON && mExpandCoef < 1.0f - FLT_EPSILON;

		if (clipping)
		{
			o2Render.EnableScissorTest(RectF(-o2Render.GetResolution().x*0.5f, layout.mAbsoluteRect.top - GetCurrentHeight(),
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

		if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
			DrawDebugFrame();
	}

	void UITreeNode::Update(float dt)
	{
		UIWidget::Update(dt);

		if (mNeedRebuild)
		{
			UpdateView();
			mNeedRebuild = false;
		}
	}

	void UITreeNode::SetExpanded(bool expanded, bool forcible /*= false*/)
	{
		if (expanded == IsExpanded())
			return;

		if (expanded)
		{
			if (!mOwnerTree->mExpandedObjects.Contains(mObject))
				mOwnerTree->mExpandedObjects.Add(mObject);
		}
		else mOwnerTree->mExpandedObjects.Remove(mObject);

		if (expanded)
			RebuildChildrenNodes(mOwnerTree->getChildsFunc(mObject), true);

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

	void UITreeNode::UpdateView(bool withChilds /*= true*/, bool deepRebuild /*= true*/, bool immediately /*= true*/)
	{
		if (!mOwnerTree)
			return;

		if (!immediately)
		{
			mNeedRebuild = true;
			return;
		}

		Vector<UnknownType*> objects = mOwnerTree->getChildsFunc(mObject);

		mOwnerTree->setupNodeFunc(this, mObject);

		UIButton* expandBtn = (UIButton*)GetChild("expandBtn");
		if (expandBtn)
			expandBtn->SetVisible(objects.Count() > 0);

		if (!withChilds || !IsExpanded())
			return;

		RebuildChildrenNodes(objects, deepRebuild);
	}

	void UITreeNode::RebuildChildrenNodes(Vector<UnknownType*> objects, bool deepRebuild)
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
			mOwnerTree->mAllNodes.Remove(node);
			RemoveChild(node, false);
			mOwnerTree->FreeTreeNode(node);
		}

		//check new objects and rebuild old
		for (auto obj : objects)
		{
			auto node = thisNodes.FindMatch([&](auto x) { return x->mObject == obj; });
			if (node)
			{
				node->UpdateView();
				continue;
			}

			UITreeNode* newNode = mOwnerTree->CreateTreeNode();
			newNode->mObject    = obj;
			newNode->mOwnerTree = mOwnerTree;
			newNode->mParent    = this;
			newNode->SetExpanded(mOwnerTree->mExpandedObjects.Contains(obj), true);
			mChilds.Add(newNode);

			mOwnerTree->mAllNodes.Add(newNode);

			if (deepRebuild)
				newNode->UpdateView();
		}

		if (!mOwnerTree->mWaitSelectionsUpdate)
			mOwnerTree->CheckSelectedNodes();

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

		mOwnerTree->mNeedUpdateLayout = true;
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

			mOwnerTree->mAllNodes.Remove(childNode);
			mChilds.Remove(child);
			mOwnerTree->FreeTreeNode(childNode);
		}

		if (!mOwnerTree->mWaitSelectionsUpdate)
			mOwnerTree->CheckSelectedNodes();
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

	bool UITreeNode::IsUnderPoint(const Vec2F& point)
	{
		return UIWidget::IsUnderPoint(point);
	}

	void UITreeNode::UpdateTreeLayout(float dt)
	{
		mOwnerTree->mNeedUpdateLayout = true;
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

		Vec2F parentSize, parentPos;
		if (mParent)
		{
			parentSize = mParent->mChildsAbsRect.Size();
			parentPos = mParent->mChildsAbsRect.LeftBottom();
		}

		float insertOffs = layout.mMinSize.y*mInsertSizeCoef;

		layout.mLocalRect.left   = layout.mOffsetMin.x;
		layout.mLocalRect.right  = parentSize.x;
		layout.mLocalRect.bottom = parentSize.y + layout.mOffsetMin.y;
		layout.mLocalRect.top    = parentSize.y + layout.mOffsetMax.y;
		layout.mAbsoluteRect     = layout.mLocalRect + parentPos - Vec2F(0, insertOffs);

		UpdateLayersLayouts();

		mChildsAbsRect = layout.mAbsoluteRect;

		for (auto child : mChilds)
		{
			bool isTreeNode = child->GetType() == UITreeNode::type;
			if (isTreeNode)
			{
				mChildsAbsRect.top    += insertOffs;
				mChildsAbsRect.bottom += insertOffs;
			}

			child->UpdateLayout(true);

			if (isTreeNode)
			{
				mChildsAbsRect.top    -= insertOffs;
				mChildsAbsRect.bottom -= insertOffs;
			}
		}
	}

	float UITreeNode::GetCurrentHeight(bool straight /*= false*/) const
	{
		float res = 0;
		float expandCoef = straight ? 1.0f : mExpandCoef;

		for (auto child : mChilds)
		{
			if (child->GetType() != UITreeNode::type)
				continue;

			res += ((UITreeNode*)child)->GetCurrentHeight(straight);
		}

		res = (res*expandCoef + layout.mMinSize.y)*mDragSizeCoef;
		return res;
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

		mFakeDragNode = mNodeSample->Clone();

		mHoverDrawable = mnew Sprite();
		mSelectedDrawable = mnew Sprite();
		mFakeDragNodeBack = mnew Sprite();
	}

	UITree::UITree(const UITree& other):
		UIScrollArea(other)
	{
		mRearrangeType        = other.mRearrangeType;
		mSelectedColor        = other.mSelectedColor;
		mUnselectedColor      = other.mUnselectedColor;
		mHoverColor           = other.mHoverColor;
		mMultiSelectAvailable = other.mMultiSelectAvailable;

		mNodeSample           = other.mNodeSample->Clone();
		mHoverDrawable        = other.mHoverDrawable->Clone();
		mSelectedDrawable     = other.mSelectedDrawable->Clone();
		mFakeDragNode         = other.mNodeSample->Clone();
		mFakeDragNodeBack     = other.mSelectedDrawable->Clone();
		mHoverLayout          = other.mHoverLayout;

		mFakeDragNode->AddLayer("selectionBack", mSelectedDrawable->Clone(), mHoverLayout, -1.0f)
			->drawable->SetColor(mSelectedColor);

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UITree::~UITree()
	{
		delete mNodeSample;
		delete mHoverDrawable;
		delete mSelectedDrawable;
		delete mFakeDragNode;
		delete mFakeDragNodeBack;

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
		delete mFakeDragNode;
		delete mFakeDragNodeBack;

		mNodeSample       = other.mNodeSample->Clone();
		mHoverDrawable    = other.mHoverDrawable->Clone();
		mSelectedDrawable = other.mSelectedDrawable->Clone();
		mFakeDragNode     = other.mNodeSample->Clone();
		mFakeDragNodeBack = other.mSelectedDrawable->Clone();
		mHoverLayout      = other.mHoverLayout;
		mSelectedColor    = other.mSelectedColor;
		mUnselectedColor  = other.mUnselectedColor;
		mHoverColor       = other.mHoverColor;

		mFakeDragNode->AddLayer("selectionBack", mSelectedDrawable->Clone(), mHoverLayout, -1.0f);

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UITree::Draw()
	{
		if (mFullyDisabled)
			return;

		if (mIsDraggingNodes)
			o2UI.DrawWidgetAtTop(mFakeDragNode);

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

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

		if (mNeedUpdateView)
		{
			UpdateRootNodes(true);
			mNeedUpdateView = false;
		}

		if (mNeedUpdateLayout)
		{
			UpdateLayout();
			mNeedUpdateLayout = false;
		}

		UIScrollArea::Update(dt);

		const float rectLerpCoef = 10.0f;

		if (mCurrentHoverRect != mTargetHoverRect)
		{
			mCurrentHoverRect = Math::Lerp(mCurrentHoverRect, mTargetHoverRect, dt*rectLerpCoef);
			mHoverDrawable->SetRect(mCurrentHoverRect);

			float alpha = mHoverDrawable->GetTransparency();
			mHoverDrawable->SetColor(mHoverColor);
			mHoverDrawable->SetTransparency(alpha);
		}

		UpdatePressedNodeExpand(dt);

		mPressedTime += dt;
	}

	ISelectableDragableObjectsGroup::SelectDragObjectsVec UITree::GetSelectedDragObjects() const
	{
		return mSelectedItems.FindAll([](const SelectedNode& x) { return x.node != nullptr; }).
			Select<SelectableDragableObject*>([](const SelectedNode& x) { return x.node; });
	}

	ISelectableDragableObjectsGroup::SelectDragObjectsVec UITree::GetAllObjects() const
	{
		return mAllNodes.Select<SelectableDragableObject*>([](const UITreeNode* x) { return (SelectableDragableObject*)x; });
	}

	void UITree::Select(SelectableDragableObject* object)
	{
		bool someSelected = false;
		UITreeNode* node = (UITreeNode*)object;

		if (o2Input.IsKeyDown(VK_SHIFT) && mSelectedItems.Count() > 0)
		{
			someSelected = CheckMultipleSelection(o2Input.GetCursorPos());
		}
		else if (!o2Input.IsKeyDown(VK_CONTROL))
		{
			DeselectAllObjects();
			someSelected = true;
		}

		if (!mSelectedItems.ContainsPred([=](const SelectedNode& x) { return x.node == node; }))
		{
			someSelected = true;

			SelectedNode selectionNode;
			selectionNode.object          = node->GetObject();
			selectionNode.selectionSprite = CreateSelectionSprite();
			selectionNode.node            = node;
			mSelectedItems.Add(selectionNode);

			node->mIsSelected = true;
			node->OnSelected();
		}

		if (someSelected)
		{
			OnSelectionChanged();
			UpdateLayout();
		}
	}

	void UITree::Deselect(SelectableDragableObject* object)
	{
		UITreeNode* node = (UITreeNode*)object;
		int idx = mSelectedItems.FindIdx([&](auto x) { return x.node == node; });

		if (idx < 0)
			return;

		if (mSelectedItems[idx].node)
		{
			mSelectedItems[idx].node->mIsSelected = false;
			mSelectedItems[idx].node->OnDeselected();
		}

		FreeSelectionSprite(mSelectedItems[idx].selectionSprite);
		mSelectedItems.RemoveAt(idx);
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
		if (mSelectedItems.IsEmpty())
			return false;

		float selectionUp   = Math::Max<float>(point.y, mSelectedItems.Last().node->layout.absTop);
		float selectionDown = Math::Min<float>(point.y, mSelectedItems.Last().node->layout.absBottom);

		bool someSelected = false;

		for (auto node : mAllNodes)
		{
			float top = node->layout.absTop;
			float bottom = node->layout.absBottom;

			if ((top > selectionDown && top < selectionUp) || (bottom > selectionDown && bottom < selectionUp))
			{
				if (mSelectedItems.FindIdx([&](auto x) { return x.node == node; }) < 0)
				{
					SelectedNode selectionNode;
					selectionNode.object          = node->GetObject();
					selectionNode.selectionSprite = CreateSelectionSprite();
					selectionNode.node            = node;
					mSelectedItems.Add(selectionNode);

					node->mIsSelected = true;
					node->OnSelected();

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
			UpdateRootNodes(true);
		else
			mNeedUpdateView = true;
	}

	void UITree::UpdateNodeView(UnknownType* object)
	{
		if (!object)
		{
			UpdateView(false);
			return;
		}

		auto node = GetNode(object);
		if (!node)
			return;

		node->UpdateView(true, true, false);
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
		for (auto sel : mSelectedItems)
		{
			if (sel.node)
			{
				sel.node->mIsSelected = false;
				sel.node->OnDeselected();
			}

			FreeSelectionSprite(sel.selectionSprite);
		}

		mSelectedItems.Clear();

		for (auto obj : objects)
		{
			auto node = GetNode(obj);

			SelectedNode selectionNode;
			selectionNode.object          = obj;
			selectionNode.selectionSprite = CreateSelectionSprite();
			selectionNode.node            = node;

			if (node)
			{
				node->mIsSelected = true;
				node->OnSelected();
			}

			mSelectedItems.Add(selectionNode);
		}

		OnSelectionChanged();
		UpdateLayout();
	}

	void UITree::SelectObject(UnknownType* object)
	{
		if (!mMultiSelectAvailable)
			DeselectAllObjects();

		if (mSelectedItems.ContainsPred([=](const SelectedNode& x) { return x.object == object; }))
			return;

		auto node = GetNode(object);

		SelectedNode selectionNode;
		selectionNode.object          = object;
		selectionNode.selectionSprite = CreateSelectionSprite();
		selectionNode.node            = node;
		mSelectedItems.Add(selectionNode);

		if (node)
		{
			node->mIsSelected = true;
			node->OnSelected();
		}

		OnSelectionChanged();
		UpdateLayout();
	}

	void UITree::SelectAndExpandObject(UnknownType* object)
	{
		Vector<UnknownType*> parentsStack;
		UnknownType* treeVisibleNodeObject = object;
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

	void UITree::DeselectObject(UnknownType* object)
	{
		int idx = mSelectedItems.FindIdx([&](auto x) { return x.object == object; });

		if (idx < 0)
			return;

		if (mSelectedItems[idx].node)
		{
			mSelectedItems[idx].node->mIsSelected = false;
			mSelectedItems[idx].node->OnDeselected();
		}

		FreeSelectionSprite(mSelectedItems[idx].selectionSprite);
		mSelectedItems.RemoveAt(idx);
		OnSelectionChanged();

		UpdateLayout();
	}

	void UITree::DeselectAllObjects()
	{
		for (auto sel : mSelectedItems)
		{
			FreeSelectionSprite(sel.selectionSprite);

			if (sel.node)
			{
				sel.node->mIsSelected = false;
				sel.node->OnDeselected();
			}
		}

		mSelectedItems.Clear();
		OnSelectionChanged();
	}

	void UITree::ScrollTo(UnknownType* object)
	{
		if (!object)
		{
			ResetSroll();
			return;
		}

		auto node = GetNode(object);

		if (node)
			SetScroll(Vec2F(mScrollPos.x, GetTreeNodeOffset(node) - layout.height*0.5f));
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
		objTreeNode->mOwnerTree = this;
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

	void UITree::OnObjectsChanged(const UnknownObjectsVec& objects)
	{
		UpdateRootNodes(false);

		for (auto obj : objects)
		{
			UITreeNode* node = GetNode(obj);

			if (node)
				node->UpdateView(true, false);
		}
	}

	void UITree::UpdateRootNodes(bool updateChilds)
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

	void UITree::OnFocused()
	{
		for (auto& sel : mSelectedItems)
			sel.selectionSprite->SetColor(mSelectedColor);

		onFocused();
	}

	void UITree::OnUnfocused()
	{
		for (auto& sel : mSelectedItems)
			sel.selectionSprite->SetColor(mUnselectedColor);

		onUnfocused();
	}

	void UITree::UpdateLayout(bool forcible /*= false*/)
	{
		if (layout.mDrivenByParent && !forcible)
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
		if (mSelectedItems.Count() < 2 && node)
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
		for (auto& sel : mSelectedItems)
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

		mBeforeDragSelectedItems = mSelectedItems;

		mDragOffset = node->layout.mAbsoluteRect.Center() - node->GetCursorPressedPoint();
		mFakeDragNode->Show(true);
		mFakeDragNode->SetInteractable(false);

		setupNodeFunc(mFakeDragNode, mSelectedItems.Last().object);

		if (mSelectedItems.Count() > 1)
		{
			if (auto nameLayer = mFakeDragNode->FindLayer<Text>())
				nameLayer->text = String::Format("%i items", mSelectedItems.Count());
		}

		for (auto sel : mSelectedItems)
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

		UnknownObjectsVec objects;
		UnknownType* targetParent = nullptr;
		UnknownType* targetPrevObject = nullptr;
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

					if (mSelectedItems.ContainsPred([=](auto x) { return x.object == targetPrevObject; }))
					{
						if (mChilds.Count() > 1)
							targetPrevObject = ((UITreeNode*)(mChilds[mChilds.Count() - 2]))->mObject;
						else
							targetPrevObject = nullptr;
					}
				}
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
		onItemsSelectionChanged(mSelectedItems.Select<UnknownType*>([](auto x) { return x.object; }));
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
			if (mSelectedItems.Count() > 0)
			{
				UnknownType* lastSelected = mSelectedItems.Last().object;
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

	bool UITree::IsFocusable() const
	{
		return true;
	}

	bool UITree::IsUnderPoint(const Vec2F& point)
	{
		return UIWidget::IsUnderPoint(point);
	}
}