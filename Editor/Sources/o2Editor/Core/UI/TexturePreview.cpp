#include "o2Editor/stdafx.h"
#include "TexturePreview.h"

#include "o2/Render/Sprite.h"
#include "o2Editor/Utils/CommonTextures.h"
#include "o2/Scene/ActorTransform.h"

namespace Editor
{
	TexturePreview::TexturePreview()
	{
		mTextureSprite = mmake<Ref<Sprite>>();
		mBackgroundSprite = mmake<Ref<Sprite>>(CommonTextures::checkedBackground);
		mBackgroundSprite->SetMode(SpriteMode::Tiled);

		AddLayer("background", mBackgroundSprite, Layout::BothStretch(0, mLabelHeight, 0, 0));
		AddLayer("texture", mTextureSprite, Layout::BothStretch(0, mLabelHeight, 0, 0));

		mTextureInfoLabel = o2UI.CreateLabel("");
		*mTextureInfoLabel->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, mLabelHeight);
		AddChild(mTextureInfoLabel);
	}

	TexturePreview::TexturePreview(const TexturePreview& other):
		Widget(other)
	{
		mBackgroundSprite = GetLayerDrawable<Ref<Sprite>>("background");
		mTextureSprite = GetLayerDrawable<Ref<Sprite>>("texture");

		mTextureInfoLabel = FindChildByType<Label>();
	}

	void TexturePreview::SetTexture(const Ref<Texture>& texture, const RectI& srcRect /*= RectI()*/)
	{
		mTextureSprite->SetTexture(texture);

		RectI rect = srcRect == RectI() ? RectI(Vec2I(), texture->GetSize()) : srcRect;
		mTextureSprite->SetTextureSrcRect(rect);

		if (texture->GetFileName().IsEmpty())
			mTextureInfoLabel->SetText("(Runtime): " + (String)rect.Width() + "x" + (String)rect.Height());
		else
			mTextureInfoLabel->SetText(texture->GetFileName() + ": " + (String)rect.Width() + "x" + (String)rect.Height());
	}

	void TexturePreview::UpdateLayersLayouts()
	{
		Widget::UpdateLayersLayouts();

		Vec2F spriteSize = layout->GetWorldRect().Size();
		spriteSize.y -= mLabelHeight;

		Vec2F textureSize = mTextureSprite->GetTextureSrcRect().Size();
		float xSize = textureSize.x * spriteSize.y / textureSize.y;

		if (xSize > spriteSize.x)
		{
			float ySize = textureSize.y * spriteSize.x / textureSize.x;
			mTextureSprite->SetSize(Vec2F(spriteSize.x, ySize));
			mBackgroundSprite->SetSize(Vec2F(spriteSize.x, ySize));
		}
		else
		{
			mTextureSprite->SetSize(Vec2F(xSize, spriteSize.y));
			mBackgroundSprite->SetSize(Vec2F(xSize, spriteSize.y));
		}
	}

	TexturePreview& TexturePreview::operator=(const TexturePreview& other)
	{
		Widget::operator=(other);

		mBackgroundSprite = GetLayerDrawable<Ref<Sprite>>("background");
		mTextureSprite = GetLayerDrawable<Ref<Sprite>>("texture");

		mTextureInfoLabel = FindChildByType<Label>();

		return *this;
	}
}
// --- META ---

DECLARE_CLASS(Editor::TexturePreview, Editor__TexturePreview);
// --- END META ---