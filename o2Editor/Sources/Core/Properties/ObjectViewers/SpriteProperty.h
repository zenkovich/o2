#pragma once

#include "Core/Properties/FieldPropertiesInfo.h"
#include "Core/Properties/IPropertyField.h"
#include "Assets/ImageAsset.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class UISpoiler;
}

namespace Editor
{
	template<typename _type>
	class AssetProperty;

	class FloatProperty;
	class ColorProperty;
	class EnumProperty;
	class BorderIProperty;

	// ----------------------
	// Editor sprite property
	// ----------------------
	class SpriteProperty : public IPropertyField
	{
	public:
		// Default constructor
		SpriteProperty();

		// Copy constructor
		SpriteProperty(const SpriteProperty& other);

		// Copy operator
		SpriteProperty& operator=(const SpriteProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(SpriteProperty);

	protected:
		typedef AssetProperty<ImageAssetRef> ImageAssetProperty;

		bool                mPropertiesInitialized = false; // True when properties were built and initialized
		TargetsVec          mTargetObjects;                 // Target objects
		FieldPropertiesInfo mFieldProperties;               // Field properties information

		UISpoiler*          mSpoiler = nullptr;             // Properties spoiler
		UIVerticalLayout*   mHiddenProperties = nullptr;    // Hidden properties

		ImageAssetProperty* mImageProperty = nullptr;
		ColorProperty*      mColorProperty = nullptr;
		FloatProperty*      mAlphaProperty = nullptr;
		EnumProperty*       mModeProperty = nullptr;

		UISpoiler*          mFillPropertiesSpoiler = nullptr;
		FloatProperty*      mFillProperty = nullptr;

		UISpoiler*          mSlicedPropertiesSpoiler = nullptr;
		BorderIProperty*    mSliceBorderProperty = nullptr;

		UISpoiler*          mTiledPropertiesSpoiler = nullptr;
		FloatProperty*      mTileScaleProperty = nullptr;

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// It is called when expanding spoiler, initializes properties
		void OnExpand();

		// It is called when type enum selected, shows required property spoiler
		void OnModeSelected();
	};
}

CLASS_BASES_META(Editor::SpriteProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::SpriteProperty)
{
	PROTECTED_FIELD(mPropertiesInitialized);
	PROTECTED_FIELD(mTargetObjects);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mSpoiler);
	PROTECTED_FIELD(mHiddenProperties);
	PROTECTED_FIELD(mImageProperty);
	PROTECTED_FIELD(mColorProperty);
	PROTECTED_FIELD(mAlphaProperty);
	PROTECTED_FIELD(mModeProperty);
	PROTECTED_FIELD(mFillPropertiesSpoiler);
	PROTECTED_FIELD(mFillProperty);
	PROTECTED_FIELD(mSlicedPropertiesSpoiler);
	PROTECTED_FIELD(mSliceBorderProperty);
	PROTECTED_FIELD(mTiledPropertiesSpoiler);
	PROTECTED_FIELD(mTileScaleProperty);
}
END_META;
CLASS_METHODS_META(Editor::SpriteProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnExpand);
	PROTECTED_FUNCTION(void, OnModeSelected);
}
END_META;
