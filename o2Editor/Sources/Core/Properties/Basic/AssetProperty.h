#pragma once

#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/AssetsWindow.h"
#include "Core/Properties/IPropertyField.h"
#include "Events/KeyboardEventsListener.h"
#include "Render/Text.h"
#include "UI/Widget.h"
#include "Utils/Editor/DragAndDrop.h"
#include "Utils/FileSystem/FileSystem.h"


using namespace o2;

namespace Editor
{
	// ------------------------------
	// Asset property field interface
	// ------------------------------
	class IAssetProperty
	{
	public:
		// Sets value asset id
		virtual void SetAssetId(UID id) {}
	};

	// -------------------------
	// Editor asset property box
	// -------------------------
	template<typename _type>
	class AssetProperty : public TPropertyField<_type>, public IAssetProperty, public DragDropArea, public KeyboardEventsListener
	{
	public:
		// Default constructor
		AssetProperty();

		// Copy constructor
		AssetProperty(const AssetProperty& other);

		// Copy operator
		AssetProperty& operator=(const AssetProperty& other);

		// Sets value asset id
		void SetAssetId(UID id);

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(AssetProperty);

	protected:
		UIWidget* mBox = nullptr;      // Property edit box
		Text*     mNameText = nullptr; // Asset name text

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Sets common value asset id
		void SetCommonAssetId(UID id);

		// Sets asset id, checks value changed, calls onChangeCompleted
		void SetAssetIdByUser(UID id);

		// Updates value view
		void UpdateValueView() override;

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key) override;

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;
	};

	template<typename _type>
	AssetProperty<_type>::AssetProperty()
	{}

	template<typename _type>
	AssetProperty<_type>::AssetProperty(const AssetProperty& other) :
		TPropertyField<_type>(other)
	{
		InitializeControls();
	}

	template<typename _type>
	AssetProperty<_type>& AssetProperty<_type>::operator=(const AssetProperty<_type>& other)
	{
		CopyData(other);
		return *this;
	}

	template<typename _type>
	void AssetProperty<_type>::CopyData(const Actor& otherActor)
	{
		TPropertyField<_type>::CopyData(otherActor);
		InitializeControls();
	}

	template<typename _type>
	void AssetProperty<_type>::InitializeControls()
	{
		mBox = GetChildWidget("layout/box");
		if (mBox)
		{
			mBox->SetFocusable(true);
			mBox->onDraw += Func<DragDropArea, void>(this, &DragDropArea::OnDrawn);

			mNameText = mBox->GetLayerDrawable<Text>("caption");
			if (mNameText)
				mNameText->text = "--";
		}
	}

	template<typename _type>
	void AssetProperty<_type>::UpdateValueView()
	{
		if (!mValuesDifferent)
		{
			if (!mCommonValue || !o2Assets.IsAssetExist(mCommonValue->GetAssetId()))
			{
				mNameText->text = "Null:" + TypeOf(_type).GetName();
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

	template<typename _type>
	void AssetProperty<_type>::SetAssetId(UID id)
	{
		mCommonValue = id == 0 ? _type() : _type(id);

		for (auto ptr : mValuesProxies)
			SetProxy<_type>(ptr.first, mCommonValue);

		SetCommonAssetId(id);
	}

	template<typename _type>
	void AssetProperty<_type>::SetCommonAssetId(UID id)
	{
		mCommonValue = id == 0 ? _type() : _type(id);
		mValuesDifferent = false;

		UpdateValueView();
		CheckRevertableState();
		OnValueChanged();
	}

	template<typename _type>
	void AssetProperty<_type>::SetAssetIdByUser(UID id)
	{
		StoreValues(mBeforeChangeValues);
		SetAssetId(id);
		CheckValueChangeCompleted();
	}

	template<typename _type>
	void AssetProperty<_type>::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(mBox);
		if (mCommonValue)
			o2EditorAssets.ShowAssetIcon(mCommonValue->GetPath());
	}

	template<typename _type>
	void AssetProperty<_type>::OnCursorExit(const Input::Cursor& cursor)
	{
		mBox->SetState("select", false);
	}

	template<typename _type>
	void AssetProperty<_type>::OnCursorEnter(const Input::Cursor& cursor)
	{
		mBox->SetState("select", true);
	}

	template<typename _type>
	void AssetProperty<_type>::OnKeyPressed(const Input::Key& key)
	{
		if (mBox && mBox->IsFocused() && (key == VK_DELETE || key == VK_BACK))
			SetAssetIdByUser(0);
	}

	template<typename _type>
	bool AssetProperty<_type>::IsUnderPoint(const Vec2F& point)
	{
		return mBox->IsUnderPoint(point);
	}

	template<typename _type>
	void AssetProperty<_type>::OnDragExit(ISelectableDragableObjectsGroup* group)
	{
		o2Application.SetCursor(CursorType::Arrow);
		mBox->SetState("focused", false);
	}

	template<typename _type>
	void AssetProperty<_type>::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{
		auto assetIconsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (!assetIconsScroll)
			return;

		auto lastSelectedAsset = assetIconsScroll->GetSelectedAssets().Last();
		if (*lastSelectedAsset.assetType != mCommonValue.GetAssetType())
			return;

		o2Application.SetCursor(CursorType::Hand);
		mBox->SetState("focused", true);
	}

	template<typename _type>
	void AssetProperty<_type>::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		auto assetIconsScroll = dynamic_cast<UIAssetsIconsScrollArea*>(group);
		if (!assetIconsScroll)
			return;

		auto lastSelectedAsset = assetIconsScroll->GetSelectedAssets().Last();
		if (*lastSelectedAsset.assetType != mCommonValue.GetAssetType())
			return;

		SetAssetIdByUser(lastSelectedAsset.id);

		o2Application.SetCursor(CursorType::Arrow);
		mBox->Focus();
	}
}

META_TEMPLATES(typename _type)
CLASS_BASES_META(Editor::AssetProperty<typename _type>)
{
	BASE_CLASS(Editor::TPropertyField<_type>);
	BASE_CLASS(Editor::IAssetProperty);
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_FIELDS_META(Editor::AssetProperty<typename _type>)
{
	PROTECTED_FIELD(mBox);
	PROTECTED_FIELD(mNameText);
}
END_META;
META_TEMPLATES(typename _type)
CLASS_METHODS_META(Editor::AssetProperty<typename _type>)
{

	PUBLIC_FUNCTION(void, SetAssetId, UID);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, SetCommonAssetId, UID);
	PROTECTED_FUNCTION(void, SetAssetIdByUser, UID);
	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
}
END_META;
