#pragma once

#include "o2/Scene/UI/Widget.h"
#include "o2/Render/TextureRef.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class Label;
}

namespace Editor
{
	// ----------------------
	// Texture preview widget
	// ----------------------
	class TexturePreview: public Widget
	{
	public:
		// Default constructor. Initializes image
		TexturePreview();

		// Default copy-constructor
		TexturePreview(const TexturePreview& other);

		// Copy operator
		TexturePreview& operator=(const TexturePreview& other);

		// Sets viewing curve
		void SetTexture(const TextureRef& texture, const RectI& srcRect = RectI());

		SERIALIZABLE(TexturePreview);

	protected:
		float mLabelHeight = 20.0f;

		Sprite* mBackgroundSprite = nullptr;
		Sprite* mTextureSprite = nullptr;
		Label*  mTextureInfoLabel = nullptr;

	protected:
		// Updates layers layouts, calls after updating widget layout; calculates sprite size
		void UpdateLayersLayouts() override;
	};
}
// --- META ---

CLASS_BASES_META(Editor::TexturePreview)
{
    BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::TexturePreview)
{
    FIELD().PROTECTED().DEFAULT_VALUE(20.0f).NAME(mLabelHeight);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mBackgroundSprite);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTextureSprite);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTextureInfoLabel);
}
END_META;
CLASS_METHODS_META(Editor::TexturePreview)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const TexturePreview&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTexture, const TextureRef&, const RectI&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateLayersLayouts);
}
END_META;
// --- END META ---
