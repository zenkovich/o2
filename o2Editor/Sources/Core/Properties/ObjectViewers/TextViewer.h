#pragma once

#include "Assets/ImageAsset.h"
#include "Core/Properties/IObjectPropertiesViewer.h"
#include "Render/Text.h"

using namespace o2;

namespace o2
{
	class Text;
	class Spoiler;
}

namespace Editor
{
	template<typename _type>
	class AssetProperty;

	class FloatProperty;
	class ColorProperty;
	class EnumProperty;
	class WStringProperty;
	class BooleanProperty;

	// --------------------
	// Editor text property
	// --------------------
	class TextViewer : public TObjectPropertiesViewer<Text>
	{
	public:
		// Default constructor. Initializes fields controls
		TextViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const TargetsVec& targetObjets) override;

		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		IOBJECT(TextViewer);

	protected:
		typedef AssetProperty<FontAssetRef> FontAssetProperty;

		ColorProperty*     mColorProperty = nullptr;
		FloatProperty*     mAlphaProperty = nullptr;
		FontAssetProperty* mFontProperty = nullptr;
		WStringProperty*   mTextProperty = nullptr;
		FloatProperty*     mHeightProperty = nullptr;
		EnumProperty*      mHorAlignProperty = nullptr;
		EnumProperty*      mVerAlignProperty = nullptr;
		BooleanProperty*   mWordWrapProperty = nullptr;
		BooleanProperty*   mDotsEndingsProperty = nullptr;
		FloatProperty*     mSymbolsDistCoefProperty = nullptr;
		FloatProperty*     mLinesDistCoefProperty = nullptr;
	};
}

CLASS_BASES_META(Editor::TextViewer)
{
	BASE_CLASS(Editor::TObjectPropertiesViewer<Text>);
}
END_META;
CLASS_FIELDS_META(Editor::TextViewer)
{
	PROTECTED_FIELD(mColorProperty);
	PROTECTED_FIELD(mAlphaProperty);
	PROTECTED_FIELD(mFontProperty);
	PROTECTED_FIELD(mTextProperty);
	PROTECTED_FIELD(mHeightProperty);
	PROTECTED_FIELD(mHorAlignProperty);
	PROTECTED_FIELD(mVerAlignProperty);
	PROTECTED_FIELD(mWordWrapProperty);
	PROTECTED_FIELD(mDotsEndingsProperty);
	PROTECTED_FIELD(mSymbolsDistCoefProperty);
	PROTECTED_FIELD(mLinesDistCoefProperty);
}
END_META;
CLASS_METHODS_META(Editor::TextViewer)
{

	PUBLIC_FUNCTION(void, Refresh, const TargetsVec&);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
}
END_META;
