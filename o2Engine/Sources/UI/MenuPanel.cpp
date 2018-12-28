#include "stdafx.h"
#include "MenuPanel.h"

#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/HorizontalLayout.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace o2
{

	UIMenuPanel::UIMenuPanel():
		UIWidget(), DrawableCursorEventsListener(this)
	{
		mItemSample = mnew UIWidget();
		mItemSample->AddLayer("text", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));

		mSelectionDrawable = mnew Sprite();

		mLayout = mnew UIHorizontalLayout();
		AddChild(mLayout);

		mLayout->expandHeight  = true;
		mLayout->expandWidth   = false;
		mLayout->baseCorner    = BaseCorner::LeftTop;
		mLayout->fitByChildren = true;
		*mLayout->layout       = UIWidgetLayout::BothStretch();
	}

	UIMenuPanel::UIMenuPanel(const UIMenuPanel& other):
		UIWidget(other), DrawableCursorEventsListener(this)
	{
		mItemSample        = other.mItemSample->CloneAs<UIWidget>();
		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mSelectionLayout   = other.mSelectionLayout;
		mLayout            = FindChildByType<UIHorizontalLayout>();

		RetargetStatesAnimations();
		UpdateSelfTransform();
	}

	UIMenuPanel::~UIMenuPanel()
	{
		delete mItemSample;
		delete mSelectionDrawable;
	}

	UIMenuPanel& UIMenuPanel::operator=(const UIMenuPanel& other)
	{
		UIWidget::operator=(other);
		return *this;
	}

	void UIMenuPanel::Update(float dt)
	{
		UIWidget::Update(dt);

		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		const float rectLerpCoef = 20.0f;
		if (mCurrentSelectionRect != mTargetSelectionRect)
		{
			mCurrentSelectionRect = Math::Lerp(mCurrentSelectionRect, mTargetSelectionRect, dt*rectLerpCoef);
			mSelectionDrawable->SetRect(mCurrentSelectionRect);
		}

		if (mSelectSubContextTime >= 0.0f)
		{
			mSelectSubContextTime -= dt;

			if (mSelectSubContextTime < 0.0f)
			{
				if (mOpenedContext)
				{
					mOpenedContext->HideWithChild();

					if (mSelectedItem >= 0)
					{
						if (auto contextMenu = mLayout->mChildWidgets[mSelectedItem]->FindChildByType<UIContextMenu>())
						{
							contextMenu->Show(mLayout->mChildWidgets[mSelectedItem]->layout->worldLeftBottom);
							mOpenedContext = contextMenu;
						}
					}
				}
			}
		}
	}

	void UIMenuPanel::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();

		for (auto child : mDrawingChildren)
			child->Draw();

		mSelectionDrawable->Draw();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		DrawDebugFrame();
	}

	UIWidget* UIMenuPanel::AddItem(const Item& item)
	{
		UIWidget* newItem = CreateItem(item);
		mLayout->AddChild(newItem);
		mClickFunctions.Add(item.onClick);

		return newItem;
	}

	UIContextMenu* UIMenuPanel::CreateSubContext(WString& path)
	{
		int slashPos = path.Find("/");
		if (slashPos < 0)
			return nullptr;

		WString subMenu = path.SubStr(0, slashPos);

		UIWidget* subChild = mLayout->mChildWidgets.FindMatch([&](auto x) {
			if (auto text = x->template GetLayerDrawable<Text>("text"))
				return text->text == subMenu;

			return false;
		});

		if (!subChild)
			subChild = AddItem(subMenu);

		UIContextMenu* subContext = subChild->FindChildByType<UIContextMenu>();
		if (!subContext)
		{
			subContext = o2UI.CreateWidget<UIContextMenu>();
			subChild->AddChild(subContext);
		}

		path = path.SubStr(slashPos + 1);

		return subContext;
	}

	UIWidget* UIMenuPanel::AddItem(const WString& path, 
								   const Function<void()>& clickFunc /*= Function<void()>()*/,
								   const ImageAssetRef& icon /*= ImageAssetRef()*/,
								   const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		WString itemPath = path;
		UIContextMenu* subContext = CreateSubContext(itemPath);
		if (!subContext)
			return AddItem(Item(path, clickFunc));

		return subContext->AddItem(itemPath, clickFunc, icon, shortcut);
	}

	UIWidget* UIMenuPanel::AddToggleItem(const WString& path, bool value,
										 const Function<void(bool)>& clickFunc /*= Function<void(bool)>()*/, 
										 const ImageAssetRef& icon /*= ImageAssetRef()*/, 
										 const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		WString itemPath = path;
		UIContextMenu* subContext = CreateSubContext(itemPath);
		if (!subContext)
			return nullptr;

		return subContext->AddToggleItem(itemPath, value, clickFunc, icon, shortcut);
	}

	UIWidget* UIMenuPanel::InsertItem(const Item& item, int position)
	{
		UIWidget* newItem = CreateItem(item);
		mLayout->AddChild(newItem, position);

		if (item.subItems.Count() > 0)
		{
			mClickFunctions.Insert([=]()
			{
				newItem->FindChildByType<UIContextMenu>()->Show(newItem->layout->worldRightBottom);
			}, position);
		}
		else mClickFunctions.Insert(item.onClick, position);

		return newItem;
	}

	void UIMenuPanel::AddItems(Vector<Item> items)
	{
		for (auto item : items)
			AddItem(item);
	}

	void UIMenuPanel::InsertItems(Vector<Item> items, int position)
	{
		int i = 0;
		for (auto item : items)
		{
			InsertItem(item, position + i);
			i++;
		}
	}

	UIMenuPanel::Item UIMenuPanel::GetItem(int position)
	{
		if (position > 0 && position < mLayout->GetChildren().Count())
			return GetItemDef(position);

		return Item();
	}

	Vector<UIMenuPanel::Item> UIMenuPanel::GetItems() const
	{
		Vector<Item> res;
		for (int i = 0; i < mLayout->GetChildren().Count(); i++)
			res.Add(GetItemDef(i));

		return res;
	}

	void UIMenuPanel::RemoveItem(int position)
	{
		if (position > 0 && position < mLayout->GetChildren().Count())
			mLayout->RemoveChild(mLayout->GetChildren()[position]);
	}

	void UIMenuPanel::RemoveItem(const WString& path)
	{
		int slashPos = path.Find("/");
		if (slashPos < 0)
		{
			UIWidget* removingItem = mLayout->mChildWidgets.FindMatch([&](auto x) {
				if (auto text = x->template GetLayerDrawable<Text>("text"))
					return text->text == path;

				return false;
			});

			if (!removingItem)
			{
				o2Debug.LogError("Failed to remove menu item " + path);
				return;
			}

			mLayout->RemoveChild(removingItem);

			return;
		}

		WString subMenu = path.SubStr(0, slashPos);

		UIWidget* subChild = mLayout->mChildWidgets.FindMatch([&](auto x) {
			if (auto text = x->template GetLayerDrawable<Text>("text"))
				return text->text == subMenu;

			return false;
		});

		if (!subChild)
		{
			o2Debug.LogError("Failed to remove menu item " + path);
			return;
		}

		UIContextMenu* subContext = subChild->FindChildByType<UIContextMenu>();
		if (!subContext)
		{
			o2Debug.LogError("Failed to remove menu item " + path);
			return;
		}

		subContext->RemoveItem(path.SubStr(slashPos + 1));
	}

	void UIMenuPanel::RemoveAllItems()
	{
		mLayout->RemoveAllChildren();
	}

	UIHorizontalLayout* UIMenuPanel::GetItemsLayout() const
	{
		return mLayout;
	}

	UIWidget* UIMenuPanel::GetItemSample() const
	{
		return mItemSample;
	}

	void UIMenuPanel::SetItemSample(UIWidget* sample)
	{
		delete mItemSample;
		mItemSample = sample;
	}

	Sprite* UIMenuPanel::GetSelectionDrawable() const
	{
		return mSelectionDrawable;
	}

	void UIMenuPanel::SetSelectionDrawableLayout(const Layout& layout)
	{
		mSelectionLayout = layout;
	}

	Layout UIMenuPanel::GetSelectionDrawableLayout() const
	{
		return mSelectionLayout;
	}

	UIWidget* UIMenuPanel::CreateItem(const Item& item)
	{
		UIWidget* newItem = mItemSample->CloneAs<UIWidget>();
		newItem->name = (WString)"Menu Item:" + item.text;

		if (auto textLayer = newItem->GetLayerDrawable<Text>("text"))
			textLayer->text = item.text;

		if (item.subItems.Count() > 0)
		{
			UIContextMenu* subMenu = o2UI.CreateWidget<UIContextMenu>();
			subMenu->AddItems(item.subItems);
			newItem->AddChild(subMenu);
		}

		return newItem;
	}

	UIMenuPanel::Item UIMenuPanel::GetItemDef(int idx) const
	{
		Item res;
		auto item = mLayout->mChildWidgets[idx];

		if (auto textLayer = item->GetLayerDrawable<Text>("text"))
			res.text = textLayer->text;

		if (auto subMenu = item->FindChildByType<UIContextMenu>())
			res.subItems = subMenu->GetItems();

		res.onClick = mClickFunctions[idx];

		return res;
	}

	void UIMenuPanel::CopyData(const Actor& otherActor)
	{
		const UIMenuPanel& other = dynamic_cast<const UIMenuPanel&>(otherActor);

		UIWidget::CopyData(other);

		delete mItemSample;
		delete mSelectionDrawable;

		mItemSample        = other.mItemSample->CloneAs<UIWidget>();
		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mSelectionLayout   = other.mSelectionLayout;
		mLayout            = FindChildByType<UIHorizontalLayout>();

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void UIMenuPanel::OnResEnableInHierarchyChanged()
	{
		interactable = mResEnabled;
	}

	UIWidget* UIMenuPanel::GetItemUnderPoint(const Vec2F& point, int* idxPtr)
	{
		if (!mLayout)
			return nullptr;

		int idx = 0;
		for (auto child : mLayout->mChildWidgets)
		{
			if (child->layout->IsPointInside(point))
			{
				if (idxPtr)
					*idxPtr = idx;

				return child;
			}

			idx++;
		}

		if (idxPtr)
			*idxPtr = -1;

		return nullptr;
	}

	void UIMenuPanel::UpdateHover(const Vec2F& point)
	{
		int itemIdx = -1;
		UIWidget* itemUnderCursor = GetItemUnderPoint(point, &itemIdx);

		if (itemIdx < 0)
		{
			auto hoverState = state["hover"];
			if (hoverState)
			{
				mSelectionDrawable->SetEnabled(true);
				*hoverState = mOpenedContext != nullptr;
			}
			else
				mSelectionDrawable->SetEnabled(false);

			mSelectedItem = itemIdx;
		}
		else
		{
			mTargetSelectionRect = mSelectionLayout.Calculate(itemUnderCursor->layout->worldRect);

			auto hoverState = state["hover"];
			if (hoverState)
			{
				mSelectionDrawable->SetEnabled(true);
				*hoverState = true;
			}
			else
				mSelectionDrawable->SetEnabled(true);

			if (itemIdx != mSelectedItem)
			{
				mSelectSubContextTime = 0.5f;
				mSelectedItem = itemIdx;
			}
		}
	}

	void UIMenuPanel::OnCursorPressed(const Input::Cursor& cursor)
	{}

	void UIMenuPanel::OnCursorStillDown(const Input::Cursor& cursor)
	{}

	void UIMenuPanel::OnCursorReleased(const Input::Cursor& cursor)
	{
		int itemIdx = -1;
		UIWidget* itemUnderCursor = GetItemUnderPoint(cursor.position, &itemIdx);

		if (itemIdx >= 0)
		{
			mClickFunctions[itemIdx]();

			if (mOpenedContext)
				mOpenedContext->HideWithChild();

			if (auto context = itemUnderCursor->FindChildByType<UIContextMenu>())
			{
				context->Show(itemUnderCursor->layout->worldLeftBottom);
				mOpenedContext = context;
			}
		}
	}

	void UIMenuPanel::OnCursorPressBreak(const Input::Cursor& cursor)
	{}

	void UIMenuPanel::OnCursorMoved(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastSelectCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastSelectCheckCursor = cursor.position;

		UpdateHover(cursor.position);
	}

	void UIMenuPanel::OnCursorExit(const Input::Cursor& cursor)
	{
		UpdateHover(cursor.position);
	}

	UIMenuPanel::Item::Item()
	{}

	UIMenuPanel::Item::Item(const WString& text, Vector<UIContextMenu::Item> subItems):
		text(text), subItems(subItems)
	{}

	UIMenuPanel::Item::Item(const WString& text, const Function<void()> onClick) :
		text(text), onClick(onClick)
	{}

	bool UIMenuPanel::Item::operator==(const Item& other) const
	{
		return text == other.text;
	}
}

DECLARE_CLASS(o2::UIMenuPanel);

DECLARE_CLASS(o2::UIMenuPanel::Item);
