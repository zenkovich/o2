#pragma once

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Assets/Asset.h"
#include "Assets/Assets.h"
#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Property.h"

using namespace o2;

namespace Editor
{
	// ------------------------------
	// Asset property field interface
	// ------------------------------
	class IAssetProperty: public IPropertyField, public DrawableCursorEventsListener, public KeyboardEventsListener
	{
	public:
		// Sets value asset id
		virtual void SetAssetId(AssetId id) {}

		IOBJECT(IAssetProperty);
	};

	// -------------------------
	// Editor asset property box
	// -------------------------
	// Specialization: AssetProperty<ActorAsset> AssetProperty<AnimationAsset> AssetProperty<AtlasAsset> 
	// AssetProperty<BinaryAsset> AssetProperty<BitmapFontAsset> AssetProperty<DataAsset> AssetProperty<FolderAsset>
	// AssetProperty<ImageAsset> AssetProperty<VectorFontAsset>
	template<typename _type>
	class AssetProperty: public IAssetProperty
	{
	public:
		// Default constructor
		AssetProperty();

		// Constructor
		AssetProperty(const Vector<void*>& targets, bool isProperty);

		// Destructor
		~AssetProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Update();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		const _type& GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		// Sets value asset id
		void SetAssetId(AssetId id);

		IOBJECT(AssetProperty);

	protected:
		Function<void(void*, const _type&)> mAssignFunc; // Value assign function
		Function<_type(void*)>              mGetFunc;    // Get value function

		Vector<void*> mValuesPointers;  // Fields' pointers
		_type         mCommonValue;     // Common field value (if not different)
		bool          mValuesDifferent; // Are values different

		UIWidget*     mBox;             // Edit box 
		Text*         mNameText;        // Asset name text

	protected:
		// Initializes widget
		void InitializeWidget();

		// Calls when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when key was pressed
		void OnKeyPressed(const Input::Key& key);
	};

	template<typename _type>
	AssetProperty<_type>::AssetProperty()
	{
		InitializeWidget();
	}

	template<typename _type>
	AssetProperty<_type>::AssetProperty(const Vector<void*>& targets, bool isProperty)
	{
		InitializeWidget();
		Setup(targets, isProperty);
	}

	template<typename _type>
	AssetProperty<_type>::~AssetProperty()
	{
		SetEventHandleDrawable(nullptr);
		delete mBox;
	}

	template<typename _type>
	void AssetProperty<_type>::InitializeWidget()
	{
		mBox = mnew UIWidget();

		mBox->SetFocusable(true);

		auto backLayer = mBox->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto selectLayer = mBox->AddLayer("select", mnew Sprite("ui/UI_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto focusLayer = mBox->AddLayer("focus", mnew Sprite("ui/UI_Editbox_focus.png"), Layout::BothStretch(-9, -9, -9, -9));

		mBox->AddState("selected", Animation::EaseInOut(mBox, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		mBox->AddState("select", Animation::EaseInOut(mBox, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		mNameText = mnew Text("stdFont.ttf");
		mNameText->text = "--";
		mNameText->horAlign = HorAlign::Left;
		mNameText->verAlign = VerAlign::Middle;
		mNameText->dotsEngings = true;
		mBox->AddLayer("caption", mNameText, Layout::BothStretch(2, 2, 2, 2));

		auto linkBtn = o2UI.CreateWidget<UIButton>("asset link");
		linkBtn->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(15, 15), Vec2F());
		mBox->AddChild(linkBtn);

		SetEventHandleDrawable(mBox);
	}

	template<typename _type>
	void AssetProperty<_type>::Setup(const Vector<void*>& targets, bool isProperty)
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

		Update();
	}

	template<typename _type>
	void AssetProperty<_type>::Update()
	{
		if (mValuesPointers.IsEmpty())
			return;

		mCommonValue = mGetFunc(mValuesPointers[0]);
		mValuesDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (mCommonValue != mGetFunc(mValuesPointers[i]))
			{
				mValuesDifferent = true;
				break;
			}
		}

		if (mValuesDifferent)
		{
			mNameText->text = "--";
			mBox->layer["caption"]->transparency = 1.0f;
		}
		else
		{
			if (!o2Assets.IsAssetExist(mCommonValue.GetAssetId()))
			{
				mNameText->text = "Null:" + TypeOf(_type).Name().SubStr(4);
				mBox->layer["caption"]->transparency = 0.5f;
			}
			else
			{
				mNameText->text = o2FileSystem.GetFileNameWithoutExtension(
					o2FileSystem.GetPathWithoutDirectories(mCommonValue.GetPath()));
				mBox->layer["caption"]->transparency = 1.0f;
			}
		}
	}

	template<typename _type>
	UIWidget* AssetProperty<_type>::GetWidget() const
	{
		return mBox;
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
	void AssetProperty<_type>::SetAssetId(AssetId id)
	{
		mCommonValue = id == 0 ? _type() : _type(id);
		mValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);

		Update();
	}

	template<typename _type>
	void AssetProperty<_type>::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(mBox);
		o2EditorAssets.ShowAssetIcon(mCommonValue.GetPath());
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
		if (mBox->IsFocused() && key == VK_DELETE || key == VK_BACK)
			SetAssetId(0);
	}
}