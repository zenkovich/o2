#pragma once

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Application/Application.h"
#include "Assets/Asset.h"
#include "Assets/Assets.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/AssetsWindow.h"
#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "Utils/DragAndDrop.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Property.h"

using namespace o2;

namespace Editor
{
	// ------------------------------
	// Asset property field interface
	// ------------------------------
	class IAssetProperty: public IPropertyField, public DragDropArea, public KeyboardEventsListener
	{
	public:
		// Sets value asset id
		virtual void SetAssetId(UID id) {}

		IOBJECT(IAssetProperty);
	};

	// -------------------------
	// Editor asset property box
	// -------------------------
	template<typename _type>
	class AssetProperty: public IAssetProperty
	{
	public:
		// Default constructor
		AssetProperty(UIWidget* widget = nullptr);

		// Destructor
		~AssetProperty();

		// Sets fields
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns value
		const _type& GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		// Sets value asset id
		void SetAssetId(UID id);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(AssetProperty);

	protected:
		Function<void(void*, const _type&)> mAssignFunc; // Value assign function
		Function<_type(void*)>              mGetFunc;    // Get value function

		TargetsVec mValuesPointers;           // Fields' pointers
		_type      mCommonValue;              // Common field value (if not different)
		bool       mValuesDifferent = true;   // Are values different

		UIWidget*  mPropertyWidget = nullptr; // Property widget
		UIButton*  mRevertBtn = nullptr;      // Revert to source prototype button
		UIWidget*  mBox = nullptr;            // Property edit box
		Text*      mNameText = nullptr;       // Asset name text

	protected:
		// Sets common value asset id
		void SetCommonAssetId(UID id);

		// Checks value for reverting to prototype
		void CheckRevertableState();

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

		// Sets asset id, checks value changed, calls onChangeCompleted
		void SetAssetIdByUser(UID id);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};

	template<typename _type>
	AssetProperty<_type>::AssetProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("asset property");

		mBox = mPropertyWidget->GetChildWidget("box");
		if (!mBox)
			mBox = mPropertyWidget;

		mBox->onDraw += [&]() { OnDrawn(); };
		mBox->SetFocusable(true);

		mNameText = mBox->GetLayerDrawable<Text>("caption");
		mNameText->text = "--";

		mRevertBtn = mPropertyWidget->GetChildByType<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);
	}

	template<typename _type>
	AssetProperty<_type>::~AssetProperty()
	{
		delete mPropertyWidget;
	}

	template<typename _type>
	void AssetProperty<_type>::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const _type& value) { *((Property<_type>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<_type>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const _type& value) { *((_type*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((_type*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	template<typename _type>
	void AssetProperty<_type>::Refresh()
	{
		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (newCommonValue != mGetFunc(mValuesPointers[i].first))
			{
				newDifferent = true;
				break;
			}
		}

		if (newDifferent)
		{
			if (!lastDifferent)
				SetUnknownValue();
		}
		else if (lastCommonValue != newCommonValue || lastDifferent)
			SetCommonAssetId(newCommonValue ? newCommonValue->GetAssetId() : 0);

		CheckRevertableState();
	}

	template<typename _type>
	void AssetProperty<_type>::Revert()
	{
		for (auto ptr : mValuesPointers)
		{
			if (ptr.second)
			{
				mAssignFunc(ptr.first, mGetFunc(ptr.second));
			}
		}

		Refresh();
	}

	template<typename _type>
	UIWidget* AssetProperty<_type>::GetWidget() const
	{
		return mPropertyWidget;
	}

	template<typename _type>
	const _type& AssetProperty<_type>::GetCommonValue() const
	{
		return mCommonValue;
	}

	template<typename _type>
	bool AssetProperty<_type>::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	template<typename _type>
	const Type* AssetProperty<_type>::GetFieldType() const
	{
		return &TypeOf(_type);
	}

	template<typename _type>
	void AssetProperty<_type>::SetAssetId(UID id)
	{
		mCommonValue = id == 0 ? _type() : _type(id);

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, mCommonValue);

		SetCommonAssetId(id);
	}

	template<typename _type>
	void AssetProperty<_type>::SetCommonAssetId(UID id)
	{
		mCommonValue = id == 0 ? _type() : _type(id);
		mValuesDifferent = false;

		if (!mCommonValue || !o2Assets.IsAssetExist(mCommonValue->GetAssetId()))
		{
			mNameText->text = "Null:" + TypeOf(_type).GetName().SubStr(4);
			mBox->layer["caption"]->transparency = 0.5f;
		}
		else
		{
			auto name = o2FileSystem.GetFileNameWithoutExtension(
				o2FileSystem.GetPathWithoutDirectories(mCommonValue->GetPath()));

			mNameText->text = name;
			mBox->layer["caption"]->transparency = 1.0f;
		}

		CheckRevertableState();

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	template<typename _type>
	void AssetProperty<_type>::SetUnknownValue()
	{
		mValuesDifferent = true;
		mCommonValue = _type();
		mNameText->text = "--";
		mBox->layer["caption"]->transparency = 1.0f;

		CheckRevertableState();

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	template<typename _type>
	void AssetProperty<_type>::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesPointers)
		{
			if (ptr.second && !Math::Equals(mGetFunc(ptr.first), mGetFunc(ptr.second)))
			{
				revertable = true;
				break;
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
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
		if (mBox->IsFocused() && (key == VK_DELETE || key == VK_BACK))
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

	template<typename _type>
	void AssetProperty<_type>::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	template<typename _type>
	void AssetProperty<_type>::SetAssetIdByUser(UID id)
	{
		StoreValues(mBeforeChangeValues);
		SetAssetId(id);
		CheckValueChangeCompleted();
	}

	template<typename _type>
	void AssetProperty<_type>::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}
}

META_TEMPLATES(typename _type)
CLASS_TEMPLATE_META(Editor::AssetProperty<typename _type>)
{
	BASE_CLASS(Editor::IAssetProperty);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mBox);
	PROTECTED_FIELD(mNameText);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(const _type&, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SetAssetId, UID);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, SetCommonAssetId, UID);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, SetAssetIdByUser, UID);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
