#include "o2Editor/stdafx.h"
#include "AssetProperty.h"

namespace Editor
{
	AssetProperty::AssetProperty()
	{}

	AssetProperty::AssetProperty(const AssetProperty& other) :
		TPropertyField(other)
	{
		InitializeControls();
	}

	AssetProperty& AssetProperty::operator=(const AssetProperty& other)
	{
		TPropertyField::operator =(other);
		return *this;
	}

	void AssetProperty::CopyData(const Actor& otherActor)
	{
		TPropertyField::CopyData(otherActor);
		InitializeControls();
	}

	void AssetProperty::InitializeControls()
	{
		mBox = GetChildWidget("container/layout/box");
		if (mBox)
		{
			mBox->SetFocusable(true);
			mBox->onDraw += MakeFunction<DragDropArea, void>(this, &DragDropArea::OnDrawn);

			mNameText = mBox->GetLayerDrawable<Text>("caption");
			if (mNameText)
				mNameText->text = "--";
		}
	}

	void AssetProperty::UpdateValueView()
	{
		if (!mValuesDifferent)
		{
			if (!mCommonValue || !o2Assets.IsAssetExist(mCommonValue->GetUID()))
			{
				mNameText->text = "Null";
				mBox->layer["caption"]->transparency = 0.5f;
			}
			else
			{
				auto name = o2FileSystem.GetFileNameWithoutExtension(
					o2FileSystem.GetPathWithoutDirectories(mCommonValue->GetPath()));

				mNameText->text = name;
				mBox->layer["caption"]->transparency = 1.0f;
			}
		}
		else
		{
			mNameText->text = "--";
			mBox->layer["caption"]->transparency = 1.0f;
		}
	}

	void AssetProperty::SetAssetId(const UID& id)
	{
		mCommonValue = id == 0 ? AssetRef() : AssetRef(id);

		for (auto ptr : mValuesProxies)
			SetProxy(ptr.first, mCommonValue);

		SetCommonAssetId(id);
	}

	void AssetProperty::SetCommonAssetId(const UID& id)
	{
		mCommonValue = id == 0 ? AssetRef() : AssetRef(id);
		mValuesDifferent = false;

		UpdateValueView();
		CheckRevertableState();
		OnValueChanged();
	}

	void AssetProperty::SetAssetIdByUser(const UID& id)
	{
		StoreValues(mBeforeChangeValues);
		SetAssetId(id);
		CheckValueChangeCompleted();
	}

	void AssetProperty::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(mBox);
		if (mCommonValue)
			o2EditorAssets.ShowAssetIcon(mCommonValue->GetPath());
	}

	void AssetProperty::OnCursorExit(const Input::Cursor& cursor)
	{
		mBox->SetState("select", false);
	}

	void AssetProperty::OnCursorEnter(const Input::Cursor& cursor)
	{
		mBox->SetState("select", true);
	}

	void AssetProperty::OnKeyPressed(const Input::Key& key)
	{
		if (mBox && mBox->IsFocused() && (key == VK_DELETE || key == VK_BACK))
			SetAssetIdByUser(0);
	}

	bool AssetProperty::IsUnderPoint(const Vec2F& point)
	{
		return mBox->IsUnderPoint(point);
	}

	void AssetProperty::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}

	void AssetProperty::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		auto assetIconsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (!assetIconsScroll)
			return;

		auto lastSelectedAsset = assetIconsScroll->GetSelectedAssets().Last();
		if (!lastSelectedAsset->meta->GetAssetType()->IsBasedOn(mCommonValue.GetAssetType()))
			return;

		o2Application.SetCursor(CursorType::Hand);
		mBox->SetState("focused", true);
	}

	void AssetProperty::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		auto assetIconsScroll = dynamic_cast<AssetsIconsScrollArea*>(group);
		if (!assetIconsScroll)
			return;

		auto lastSelectedAsset = assetIconsScroll->GetSelectedAssets().Last();
		if (!lastSelectedAsset->meta->GetAssetType()->IsBasedOn(mCommonValue.GetAssetType()))
			return;

		SetAssetIdByUser(lastSelectedAsset->meta->ID());

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}
}

DECLARE_CLASS(Editor::AssetProperty);
