#include "o2Editor/stdafx.h"
#include "AssetProperty.h"

#include "o2Editor/Core/Properties/Properties.h"

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
		mSpoiler = GetChildByType<Spoiler>("spoiler");
		mHeaderContainer = mSpoiler->GetInternalWidgetByType<HorizontalLayout>("mainLayout/container/layout");
		mCaption = mSpoiler->GetInternalWidgetByType<Label>("mainLayout/propertyName");

		mBox = mSpoiler->GetInternalWidget("mainLayout/container/layout/box");
		if (mBox)
		{
			mBox->SetFocusable(true);
			mBox->onDraw += MakeFunction<DragDropArea, void>(this, &DragDropArea::OnDrawn);

			mNameText = mBox->GetLayerDrawable<Text>("caption");
			if (mNameText)
				mNameText->text = "--";
		}

		auto createInstanceBtn = mSpoiler->GetInternalWidgetByType<Button>("mainLayout/container/layout/create");
		createInstanceBtn->onClick = THIS_FUNC(OnCreateInstancePressed);

		auto saveInstanceBtn = mSpoiler->GetInternalWidgetByType<Button>("mainLayout/container/layout/save");
		saveInstanceBtn->onClick = THIS_FUNC(OnSaveInstancePressed);

		auto removeInstanceBtn = mSpoiler->GetInternalWidgetByType<Button>("mainLayout/container/layout/remove");
		removeInstanceBtn->onClick = THIS_FUNC(OnRemoveInstancePressed);
	}

	void AssetProperty::UpdateValueView()
	{
		if (!mValuesDifferent)
		{
			if (!mCommonValue || !o2Assets.IsAssetExist(mCommonValue->GetUID()))
			{
				mNameText->text = "Null";
				if (mAssetType)
				{
					mNameText->text += WString(": " + o2EditorProperties.MakeSmartFieldName(mAssetType->GetName()));
				}
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

	void AssetProperty::SetAssetType(const Type* assetType)
	{
		mAssetType = assetType;
	}

	void AssetProperty::SetFieldInfo(const FieldInfo* fieldInfo)
	{
		SetAssetType(fieldInfo->GetType()->InvokeStatic<const Type*>("GetAssetTypeStatic"));
		IPropertyField::SetFieldInfo(fieldInfo);
	}

	void AssetProperty::SetCaption(const WString& text)
	{
		if (mCaption)
			mCaption->SetText(text);
	}

	WString AssetProperty::GetCaption() const
	{
		if (mCaption)
			return mCaption->GetText();

		return WString();
	}

	Button* AssetProperty::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			mRemoveBtn = o2UI.CreateWidget<Button>("remove small");
			mRemoveBtn->layout->maxWidth = 20;
			mRemoveBtn->layout->minHeight = 20;
			mHeaderContainer->AddChild(mRemoveBtn, 0);
		}

		return mRemoveBtn;
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

	void AssetProperty::OnCreateInstancePressed()
	{
		SetState("instance", true);
	}

	void AssetProperty::OnRemoveInstancePressed()
	{
		SetState("instance", false);
	}

	void AssetProperty::OnSaveInstancePressed()
	{

	}

	AssetRef AssetProperty::GetProxy(IAbstractValueProxy* proxy) const
	{
		auto proxyType = dynamic_cast<const ObjectType*>(&proxy->GetType());
		auto proxySample = proxyType->CreateSample();
		proxy->GetValuePtr(proxySample);
		auto objectSample = proxyType->DynamicCastToIObject(proxySample);
		AssetRef* assetSample = dynamic_cast<AssetRef*>(objectSample);
		AssetRef res = *assetSample;
		delete assetSample;
		return res;
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

		if (mAssetType)
		{
			auto lastSelectedAsset = assetIconsScroll->GetSelectedAssets().Last();
			if (!lastSelectedAsset->meta->GetAssetType()->IsBasedOn(*mAssetType))
				return;
		}

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

DECLARE_CLASS_MANUAL(Editor::TPropertyField<AssetRef>);

DECLARE_CLASS(Editor::AssetProperty);
