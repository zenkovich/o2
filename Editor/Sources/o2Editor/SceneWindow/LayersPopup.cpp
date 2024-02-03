#include "o2Editor/stdafx.h"
#include "LayersPopup.h"

#include "o2/Scene/Scene.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/Widgets/Toggle.h"
#include "o2/Utils/Editor/EditorScope.h"

namespace Editor
{
	LayersPopup::LayersPopup()
	{
		InitializeControls();
	}

	LayersPopup::~LayersPopup()
	{
		delete mItemSample;
		mItemsCache.ForEach([](auto x) { delete x; });
	}

	void LayersPopup::Show(const Vec2F& position)
	{
		UpdateLayersList();
		PopupWidget::Show(position);
	}

	void LayersPopup::Update(float dt)
	{
		PopupWidget::Update(dt);

		if (mDraggingItem)
			UpdateDragAnimation(dt);
	}

	String LayersPopup::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void LayersPopup::SpecialDraw()
	{
		PopupWidget::SpecialDraw();
	}

	Vec2F LayersPopup::GetContentSize() const
	{
		return Vec2F(200, mChildWidgets.Count() * mItemSample->layout->GetMinHeight());
	}

	void LayersPopup::InitializeControls()
	{
		AddLayer("back", mmake<Ref<Sprite>>("ui/UI4_Context_menu.png"), mmake<Ref<Layout::BothStretch>>(-20, -19, -20, -19));

		mItemSample = mmake<Ref<LayerPopupItem>>();

		mAddButtonLayout = mmake<Ref<HorizontalLayout>>();
		*mAddButtonLayout->layout = mmake<Ref<WidgetLayout::HorStretch>>(VerAlign::Bottom, 0, 0, 20);
		mAddButtonLayout->layout->minHeight = 20;
		mAddButtonLayout->AddLayer("line", mmake<Ref<Sprite>>("ui/UI4_Separator.png"),
			mmake<Ref<Layout::HorStretch>>(VerAlign::Top, 0, 0, 5, -2));
		mAddButtonLayout->AddChild(mmake<Ref<Widget>>());

		mAddButton = o2UI.CreateWidget<Button>("add small");
		mAddButton->layout->maxWidth = 20;

		mAddButton->isPointInside = [=](const Vec2F& p) {
			return mAddButton->layout->IsPointInside(p) || mAddButtonLayout->layout->IsPointInside(p);
		};

		mAddButton->onClick = [=]() {
			String newLayerName = "New layer";

			int idx = 1;
			while (o2Scene.HasLayer(newLayerName))
				newLayerName = "New layer " + (String)idx++;

			auto newLayer = o2Scene.AddLayer(newLayerName);
			UpdateLayersListAndFit();

			auto item = mChildWidgets.FindOrDefault([=](Ref<Widget> x) {
				if (auto item = DynamicCast<LayerPopupItem>(x))
					return item->mLayer == newLayer;

				return false;
			});

			DynamicCast<LayerPopupItem>(item)->BeginEditName();
		};

		mAddButtonLayout->AddChild(mAddButton);
	}

	void LayersPopup::UpdateItemLayout(Ref<LayerPopupItem> item, int idx)
	{
		float itemHeight = mItemSample->layout->GetMinHeight();
		float dragModeOffset = 0.0f;

		if (mDraggingItem)
			dragModeOffset = mDragAnimFunc.Evaluate(item->mDragInsertCoef) * itemHeight;

		item->layout->CopyFrom(mmake<Ref<WidgetLayout::HorStretch>>(VerAlign::Top, 0, 0, itemHeight, itemHeight * (float)idx + dragModeOffset));
		item->layout->minSize = Vec2F(200, 20);
	}

	void LayersPopup::UpdateLayersList()
	{
		PushEditorScopeOnStack scope;

		RemoveChild(mAddButtonLayout, false);

		mItemsCache.Add(mChildWidgets.DynamicCast<LayerPopupItem>());
		RemoveAllChildren(false);

		int idx = 0;
		for (auto layer : o2Scene.GetLayers())
		{
			Ref<LayerPopupItem> item;
			if (!mItemsCache.IsEmpty())
			{
				auto sameLayerItem = mItemsCache.FindOrDefault([=](Ref<LayerPopupItem> x) { return x->mLayer == layer; });
				if (sameLayerItem)
				{
					item = sameLayerItem;
					mItemsCache.Remove(sameLayerItem);
				}
				else
					item = mItemsCache.PopBack();
			}
			else
				item = mItemSample->CloneAs<LayerPopupItem>();

			item->SetLayer(layer);
			item->mPopup = this;
			AddChild(item);
			UpdateItemLayout(item, idx);

			idx++;
		}

		AddChild(mAddButtonLayout);
	}

	void LayersPopup::UpdateLayersListAndFit()
	{
		UpdateLayersList();
		UpdateTransform();
		FitSizeAndPosition(layout->GetWorldLeftTop());
	}

	void LayersPopup::BeginDragging(Ref<LayerPopupItem> item)
	{
		mDraggingItem = item;
		mDragOffset = o2Input.GetCursorPos() - mDraggingItem->layout->GetWorldLeftBottom();

		float itemPos = GetChildrenWorldRect().top - item->layout->worldTop;
		float threshold = itemPos - 10.0f;
		float itemHeight = mItemSample->layout->minHeight;
		int idx = 0;
		for (auto child : mChildWidgets)
		{
			if (auto item = DynamicCast<LayerPopupItem>(child))
			{
				if (item != mDraggingItem)
				{
					float itemOriginPos = itemHeight * (float)idx;
					item->mDragInsertCoef = itemOriginPos < threshold ? 0.0f : 1.0f;
					item->mDragTargetInsertCoef = item->mDragInsertCoef;
					item->BreakEditName();
					UpdateItemLayout(item, idx);
					idx++;
				}
			}
		}
	}

	void LayersPopup::UpdateDragging()
	{
		float itemPos = layout->worldTop - o2Input.GetCursorPos().y - mDragOffset.y;
		*mDraggingItem->layout = mmake<Ref<WidgetLayout::HorStretch>>(VerAlign::Top, 0, 0, mDraggingItem->layout->GetMinHeight(),
			itemPos);
		mDraggingItem->layout->minHeight = 20;

		float threshold = itemPos - 10.0f;
		float itemHeight = mItemSample->layout->minHeight;
		int idx = 0;
		for (auto child : mChildWidgets)
		{
			if (auto item = DynamicCast<LayerPopupItem>(child))
			{
				if (item != mDraggingItem)
				{
					float itemOriginPos = itemHeight * (float)idx;
					item->mDragInsertCoef = itemOriginPos < threshold ? 0.0f : 1.0f;
					item->mDragTargetInsertCoef = item->mDragInsertCoef;
					item->BreakEditName();
					UpdateItemLayout(item, idx);
					idx++;
				}
			}
		}
	}
}#include <memory> // for shared_ptr, make_shared
#include <string>
#include <vector>
#include "Widget.h" // I assumed this header defines the classes used in the code snippet

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
auto mmake(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using DynamicCast = std::shared_ptr<T>;

namespace Editor {

    class SceneLayer {
    public:
        bool visible;
        std::string GetName() const;
        void SetName(const std::string& name);
    };

    class Label : public Widget {
    public:
        std::string text;
    };

    class Toggle : public Widget {
    public:
        bool value;
        std::function<void(bool)> onToggleByUser;
    };

    class Button : public Widget {
    public:
        std::function<void()> onClick;
    };

    class EditBox : public Widget {
    public:
        std::string text;
        std::function<void(const std::wstring&)> onChangeCompleted;
        void Focus();
        void Unfocus();
        void SelectAll();
    };

    class LayersPopup : public Widget {
    private:
        Ref<LayerPopupItem> mDraggingItem;
        std::vector<Ref<Widget>> mChildWidgets;
        float threshold;

    public:
        void BeginDragging(const Ref<LayerPopupItem>& item);
        void UpdateDragging();
        void UpdateLayersList();
        void UpdateLayersListAndFit();
        void UpdateDragAnimation(float dt);
        void EndDragging();
        void OnObjectChanged(const Ref<SceneLayer>& layer);

        // ... rest of the class definition ...
    };

    class LayerPopupItem : public Widget {
    private:
        LayersPopup* mPopup; // Assuming this is the correct type
        Ref<Toggle> mVisibleToggle;
        Ref<Label> mNameCaption;
        Ref<Button> mRemoveBtn;
        Ref<EditBox> mEditBox;
        Ref<SceneLayer> mLayer; // Assuming this is the correct type
        
    public:
        LayerPopupItem();
        LayerPopupItem(const LayerPopupItem& other);
        LayerPopupItem& operator=(const LayerPopupItem& other);

        void SetLayer(const Ref<SceneLayer>& layer);

        void BeginEditName();
        void BreakEditName();
        void Draw() override;
        bool IsUnderPoint(const Vec2F& point);
        std::string GetCreateMenuCategory();
        void OnCursorDblClicked(const Input::Cursor& cursor);
        void OnDragStart(const Input::Cursor& cursor);
        void OnDragged(const Input::Cursor& cursor, const Ref<DragDropArea>& area);
        void OnDragEnd(const Input::Cursor& cursor);
        void OnNameEditChanged(const std::wstring& str);
        void OnVisibleChanged(bool visible);
        void OnRemovePressed();
    };

} // namespace Editor

// --- META ---

CLASS_META(Editor::LayersPopup) {
    // ... class meta data ...
};

CLASS_META(Editor::LayerPopupItem) {
    // ... class meta data ...
};

// --- END META ---