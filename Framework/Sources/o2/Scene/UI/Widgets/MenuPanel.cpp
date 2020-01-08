#include "o2/stdafx.h"
#include "MenuPanel.h"

#include "o2/Render/Sprite.h"
#include "o2/Render/Text.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"

namespace o2
{

	MenuPanel::MenuPanel():
		Widget(), DrawableCursorEventsListener(this)
	{
		mItemSample = mnew Widget();
		mItemSample->AddLayer("text", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));

		mSelectionDrawable = mnew Sprite();

		mLayout = mnew HorizontalLayout();
		AddChild(mLayout);

		mLayout->expandHeight  = true;
		mLayout->expandWidth   = false;
		mLayout->baseCorner    = BaseCorner::LeftTop;
		mLayout->fitByChildren = true;
		*mLayout->layout       = WidgetLayout::BothStretch();
	}

	MenuPanel::MenuPanel(const MenuPanel& other):
		Widget(other), DrawableCursorEventsListener(this)
	{
		mItemSample        = other.mItemSample->CloneAs<Widget>();
		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mSelectionLayout   = other.mSelectionLayout;
		mLayout            = FindChildByType<HorizontalLayout>();

		RetargetStatesAnimations();
		UpdateSelfTransform();
		UpdateChildrenTransforms();
	}

	MenuPanel::~MenuPanel()
	{
		delete mItemSample;
		delete mSelectionDrawable;
	}

	MenuPanel& MenuPanel::operator=(const MenuPanel& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void MenuPanel::Update(float dt)
	{
		Widget::Update(dt);

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
						if (auto contextMenu = mLayout->mChildWidgets[mSelectedItem]->FindChildByType<ContextMenu>())
						{
							contextMenu->Show(mLayout->mChildWidgets[mSelectedItem]->layout->worldLeftBottom);
							mOpenedContext = contextMenu;
						}
					}
				}
			}
		}
	}

	void MenuPanel::Draw()
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

	Widget* MenuPanel::AddItem(const Item& item)
	{
		Widget* newItem = CreateItem(item);
		mLayout->AddChild(newItem);
		mClickFunctions.Add(item.onClick);

		return newItem;
	}

	ContextMenu* MenuPanel::CreateSubContext(WString& path)
	{
		int slashPos = path.Find("/");
		if (slashPos < 0)
			return nullptr;

		WString subMenu = path.SubStr(0, slashPos);

		Widget* subChild = mLayout->mChildWidgets.FindMatch([&](auto x) {
			if (auto text = x->template GetLayerDrawable<Text>("text"))
				return text->text == subMenu;

			return false;
		});

		if (!subChild)
			subChild = AddItem(subMenu);

		ContextMenu* subContext = subChild->FindChildByType<ContextMenu>();
		if (!subContext)
		{
			subContext = o2UI.CreateWidget<ContextMenu>();
			subChild->AddChild(subContext);
		}

		path = path.SubStr(slashPos + 1);

		return subContext;
	}

	Widget* MenuPanel::AddItem(const WString& path, 
								   const Function<void()>& clickFunc /*= Function<void()>()*/,
								   const ImageAssetRef& icon /*= ImageAssetRef()*/,
								   const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		WString itemPath = path;
		ContextMenu* subContext = CreateSubContext(itemPath);
		if (!subContext)
			return AddItem(Item(path, clickFunc));

		return subContext->AddItem(itemPath, clickFunc, icon, shortcut);
	}

	Widget* MenuPanel::AddToggleItem(const WString& path, bool value,
										 const Function<void(bool)>& clickFunc /*= Function<void(bool)>()*/, 
										 const ImageAssetRef& icon /*= ImageAssetRef()*/, 
										 const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		WString itemPath = path;
		ContextMenu* subContext = CreateSubContext(itemPath);
		if (!subContext)
			return nullptr;

		return subContext->AddToggleItem(itemPath, value, clickFunc, icon, shortcut);
	}

	Widget* MenuPanel::InsertItem(const Item& item, int position)
	{
		Widget* newItem = CreateItem(item);
		mLayout->AddChild(newItem, position);

		if (item.subItems.Count() > 0)
		{
			mClickFunctions.Insert([=]()
			{
				newItem->FindChildByType<ContextMenu>()->Show(newItem->layout->worldRightBottom);
			}, position);
		}
		else mClickFunctions.Insert(item.onClick, position);

		return newItem;
	}

	void MenuPanel::AddItems(Vector<Item> items)
	{
		for (auto item : items)
			AddItem(item);
	}

	void MenuPanel::InsertItems(Vector<Item> items, int position)
	{
		int i = 0;
		for (auto item : items)
		{
			InsertItem(item, position + i);
			i++;
		}
	}

	MenuPanel::Item MenuPanel::GetItem(int position)
	{
		if (position > 0 && position < mLayout->GetChildren().Count())
			return GetItemDef(position);

		return Item();
	}

	Vector<MenuPanel::Item> MenuPanel::GetItems() const
	{
		Vector<Item> res;
		for (int i = 0; i < mLayout->GetChildren().Count(); i++)
			res.Add(GetItemDef(i));

		return res;
	}

	void MenuPanel::RemoveItem(int position)
	{
		if (position > 0 && position < mLayout->GetChildren().Count())
			mLayout->RemoveChild(mLayout->GetChildren()[position]);
	}

	void MenuPanel::RemoveItem(const WString& path)
	{
		int slashPos = path.Find("/");
		if (slashPos < 0)
		{
			Widget* removingItem = mLayout->mChildWidgets.FindMatch([&](auto x) {
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

		Widget* subChild = mLayout->mChildWidgets.FindMatch([&](auto x) {
			if (auto text = x->template GetLayerDrawable<Text>("text"))
				return text->text == subMenu;

			return false;
		});

		if (!subChild)
		{
			o2Debug.LogError("Failed to remove menu item " + path);
			return;
		}

		ContextMenu* subContext = subChild->FindChildByType<ContextMenu>();
		if (!subContext)
		{
			o2Debug.LogError("Failed to remove menu item " + path);
			return;
		}

		subContext->RemoveItem(path.SubStr(slashPos + 1));
	}

	void MenuPanel::RemoveAllItems()
	{
		mLayout->RemoveAllChildren();
	}

	HorizontalLayout* MenuPanel::GetItemsLayout() const
	{
		return mLayout;
	}

	Widget* MenuPanel::GetItemSample() const
	{
		return mItemSample;
	}

	void MenuPanel::SetItemSample(Widget* sample)
	{
		delete mItemSample;
		mItemSample = sample;
	}

	Sprite* MenuPanel::GetSelectionDrawable() const
	{
		return mSelectionDrawable;
	}

	void MenuPanel::SetSelectionDrawableLayout(const Layout& layout)
	{
		mSelectionLayout = layout;
	}

	Layout MenuPanel::GetSelectionDrawableLayout() const
	{
		return mSelectionLayout;
	}

	Widget* MenuPanel::CreateItem(const Item& item)
	{
		Widget* newItem = mItemSample->CloneAs<Widget>();
		newItem->name = (WString)"Menu Item:" + item.text;

		if (auto textLayer = newItem->GetLayerDrawable<Text>("text"))
			textLayer->text = item.text;

		if (item.subItems.Count() > 0)
		{
			ContextMenu* subMenu = o2UI.CreateWidget<ContextMenu>();
			subMenu->AddItems(item.subItems);
			newItem->AddChild(subMenu);
		}

		return newItem;
	}

	MenuPanel::Item MenuPanel::GetItemDef(int idx) const
	{
		Item res;
		auto item = mLayout->mChildWidgets[idx];

		if (auto textLayer = item->GetLayerDrawable<Text>("text"))
			res.text = textLayer->text;

		if (auto subMenu = item->FindChildByType<ContextMenu>())
			res.subItems = subMenu->GetItems();

		res.onClick = mClickFunctions[idx];

		return res;
	}

	void MenuPanel::CopyData(const Actor& otherActor)
	{
		const MenuPanel& other = dynamic_cast<const MenuPanel&>(otherActor);

		Widget::CopyData(other);

		delete mItemSample;
		delete mSelectionDrawable;

		mItemSample        = other.mItemSample->CloneAs<Widget>();
		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mSelectionLayout   = other.mSelectionLayout;
		mLayout            = FindChildByType<HorizontalLayout>();

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void MenuPanel::OnResEnableInHierarchyChanged()
	{
		interactable = mResEnabled;
	}

	Widget* MenuPanel::GetItemUnderPoint(const Vec2F& point, int* idxPtr)
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

	void MenuPanel::UpdateHover(const Vec2F& point)
	{
		int itemIdx = -1;
		Widget* itemUnderCursor = GetItemUnderPoint(point, &itemIdx);

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

	void MenuPanel::OnCursorPressed(const Input::Cursor& cursor)
	{}

	void MenuPanel::OnCursorStillDown(const Input::Cursor& cursor)
	{}

	void MenuPanel::OnCursorReleased(const Input::Cursor& cursor)
	{
		int itemIdx = -1;
		Widget* itemUnderCursor = GetItemUnderPoint(cursor.position, &itemIdx);

		if (itemIdx >= 0)
		{
			mClickFunctions[itemIdx]();

			if (mOpenedContext)
				mOpenedContext->HideWithChild();

			if (auto context = itemUnderCursor->FindChildByType<ContextMenu>())
			{
				context->Show(itemUnderCursor->layout->worldLeftBottom);
				mOpenedContext = context;
			}
		}
	}

	void MenuPanel::OnCursorPressBreak(const Input::Cursor& cursor)
	{}

	void MenuPanel::OnCursorMoved(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastSelectCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastSelectCheckCursor = cursor.position;

		UpdateHover(cursor.position);
	}

	void MenuPanel::OnCursorExit(const Input::Cursor& cursor)
	{
		UpdateHover(cursor.position);
	}

	MenuPanel::Item::Item()
	{}

	MenuPanel::Item::Item(const WString& text, Vector<ContextMenu::Item> subItems):
		text(text), subItems(subItems)
	{}

	MenuPanel::Item::Item(const WString& text, const Function<void()> onClick) :
		text(text), onClick(onClick)
	{}

	bool MenuPanel::Item::operator==(const Item& other) const
	{
		return text == other.text;
	}
}

DECLARE_CLASS(o2::MenuPanel);

DECLARE_CLASS(o2::MenuPanel::Item);
