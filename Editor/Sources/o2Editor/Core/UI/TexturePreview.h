#pragma once

#include "o2/Scene/UI/Widget.h"
#include "o2/Render/TextureRef.h"
#include "o2/Core/Ref.h"
#include "o2/Core/WeakRef.h"

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
	class TexturePreview : public Widget
	{
	public:
		// Default constructor. Initializes image
		TexturePreview();

		// Default copy-constructor
		TexturePreview(const TexturePreview& other);

		// Copy operator
		TexturePreview& operator=(const TexturePreview& other);

		// Sets viewing curve
		void SetTexture(const Ref<Texture>& texture, const RectI& srcRect = RectI());

		SERIALIZABLE(TexturePreview);

	protected:
		float mLabelHeight = 20.0f;

		Ref<Sprite> mBackgroundSprite = mmake<Sprite>();
		Ref<Sprite> mTextureSprite = mmake<Sprite>();
		Ref<Label> mTextureInfoLabel = mmake<Label>();

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
	FIELD().PROTECTED().DEFAULT_VALUE(Ref<Sprite>()).WEAK().NAME(mBackgroundSprite);
	FIELD().PROTECTED().DEFAULT_VALUE(Ref<Sprite>()).WEAK().NAME(mTextureSprite);
	FIELD().PROTECTED().DEFAULT_VALUE(Ref<Label>()).WEAK().NAME(mTextureInfoLabel);
}
END_META;

CLASS_METHODS_META(Editor::TexturePreview)
{
	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const TexturePreview&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTexture, const Ref<Texture>&, const RectI&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateLayersLayouts);
}
END_META;

// --- END META ---