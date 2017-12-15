#include "VectorFont.h"

#include "Render/Windows/OpenGL.h"

#include "Application/Application.h"
#include "Render/Render.h"
#include "Utils/Bitmap.h"
#include "Utils/Debug.h"
#include "Utils/Log/LogStream.h"
#include "Utils/Timer.h"

namespace o2
{
	VectorFont::VectorFont():
		Font(), mFreeTypeFace(nullptr)
	{
		mTexture = TextureRef(Vec2I(256, 256));
		mTextureSrcRect.Set(0, 0, 256, 256);
	}

	VectorFont::VectorFont(const String& fileName):
		Font(), mFreeTypeFace(nullptr)
	{
		mTexture = TextureRef(Vec2I(256, 256));
		mTextureSrcRect.Set(0, 0, 256, 256);

		Load(fileName);
	}

	VectorFont::VectorFont(const VectorFont& other):
		Font(), mFreeTypeFace(other.mFreeTypeFace)
	{
		mTexture = TextureRef(Vec2I(256, 256));
		mTextureSrcRect.Set(0, 0, 256, 256);
	}

	VectorFont::~VectorFont()
	{
		if (mFreeTypeFace)
			FT_Done_Face(mFreeTypeFace);

		for (auto effect : mEffects)
			delete effect;
	}

	bool VectorFont::Load(const String& fileName)
	{
		FT_Error error = FT_New_Face(o2Render.mFreeTypeLib, fileName.Data(), 0, &mFreeTypeFace);
		if (error)
		{
			o2Render.mLog->Error("Failed to load vector font: %s, error: %i", fileName, error);
			return false;
		}

		mFileName = fileName;

		return true;
	}

// 	void VectorFont::SetSize(UInt size)
// 	{
// 		if (size == mSize)
// 			return;
// 
// 		mSize = size;
// 		Vec2I dpi = o2Render.GetDPI();
// 		FT_Error error = FT_Set_Char_Size(mFreeTypeFace, 0, mSize*64, dpi.x, dpi.y);
// 
// 		FT_Load_Char(mFreeTypeFace, 'A', FT_LOAD_RENDER);
// 		mBaseHeight = mFreeTypeFace->glyph->metrics.horiBearingY/64.0f;
// 		mLineHeight = mBaseHeight*2.0f;
// 
// // 		mBaseHeight = mFreeTypeFace->ascender/64.0f;
// // 		mLineHeight = (mFreeTypeFace->ascender - mFreeTypeFace->descender)/64.0f + 5.0f;
// 
// 		Reset();
// 	}

	String VectorFont::GetFileName() const
	{
		return mFileName;
	}

	float VectorFont::GetHeightPx(int height) const
	{
		Vec2I dpi = o2Render.GetDPI();
		FT_Error error = FT_Set_Char_Size(mFreeTypeFace, 0, height*64, dpi.x, dpi.y);
		
		FT_Load_Char(mFreeTypeFace, 'A', FT_LOAD_RENDER);
		return mFreeTypeFace->glyph->metrics.horiBearingY/64.0f;
	}

	float VectorFont::GetLineHeightPx(int height) const
	{
		Vec2I dpi = o2Render.GetDPI();
		FT_Error error = FT_Set_Char_Size(mFreeTypeFace, 0, height * 64, dpi.x, dpi.y);

		FT_Load_Char(mFreeTypeFace, 'A', FT_LOAD_RENDER);
		return (mFreeTypeFace->glyph->metrics.horiBearingY / 64.0f)*2.0f;
	}

	void VectorFont::CheckCharacters(const WString& needChararacters, int height)
	{
		int len = needChararacters.Length();
		Vector<wchar_t> needToRenderChars(len);
		for (int i = 0; i < len; i++)
		{
			bool isNew = true;
			wchar_t c = needChararacters[i];
			for (auto ch : mCharacters)
			{
				if (ch.mId == c && ch.mHeight == height)
				{
					isNew = false;
					break;
				}
			}

			if (isNew)
				isNew = !needToRenderChars.Contains(c);

			if (!isNew)
				continue;

			needToRenderChars.Add(c);
		}

		if (needToRenderChars.Count() > 0)
			UpdateCharacters(needToRenderChars, height);
	}

	VectorFont::Effect* VectorFont::AddEffect(Effect* effect)
	{
		mEffects.Add(effect);
		return effect;
	}

	void VectorFont::RemoveEffect(Effect* effect)
	{
		mEffects.Remove(effect);
		delete effect;
	}

	void VectorFont::RemoveAllEffects()
	{
		for (auto effect : mEffects)
			delete effect;

		mEffects.Clear();
	}

	void VectorFont::Reset()
	{
		mCharacters.Clear();
	}

	void VectorFont::UpdateCharacters(Vector<wchar_t>& newCharacters, int height)
	{
		CharDefsVec charactersDefs;
		mRectsPacker.Clear();
		
		ExtractCharacterDefsFromTexture(charactersDefs);
		RenderNewCharacters(charactersDefs, newCharacters, height);
		PackCharactersDefs(charactersDefs);

		onCharactersRebuild();
	}

	void VectorFont::ExtractCharacterDefsFromTexture(CharDefsVec& charDefs)
	{
		if (mCharacters.Count() == 0)
			return;

		Bitmap texBitmap(Bitmap::Format::R8G8B8A8, mTexture->GetSize());

		glBindTexture(GL_TEXTURE_2D, mTexture->mHandle);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBitmap.GetData());

		//glBindTexture(GL_TEXTURE_2D, o2Render.mLastDrawTexture->mHandle);

		for (auto& ch : mCharacters)
		{
			CharDef charDef;

			Bitmap* charBitmap = mnew Bitmap(Bitmap::Format::R8G8B8A8, ch.mSize);

			RectI srcRect;
			Vec2F texSize = texBitmap.GetSize();
			srcRect.left = (int)(ch.mTexSrc.left*texSize.x);
			srcRect.right = (int)(ch.mTexSrc.right*texSize.x);
			srcRect.bottom = (int)(ch.mTexSrc.top*texSize.y);
			srcRect.top = (int)(ch.mTexSrc.bottom*texSize.y);

			charBitmap->CopyImage(&texBitmap, Vec2I(), srcRect);

			charDef.mCharacter = ch;
			charDef.mBitmap = charBitmap;
			charDef.mPackRect = mRectsPacker.AddRect(ch.mSize);

			charDefs.Add(charDef);
		}
	}

	void VectorFont::RenderNewCharacters(CharDefsVec& charDefs, Vector<wchar_t>& newCharacters, int height)
	{
		Vec2I dpi = o2Render.GetDPI();
		FT_Set_Char_Size(mFreeTypeFace, 0, height * 64, dpi.x, dpi.y);

		Vec2I border;
		for (auto effect : mEffects)
		{
			Vec2I effectExt = effect->GetSizeExtend();
			border.x = Math::Max(border.x, effectExt.x);
			border.y = Math::Max(border.y, effectExt.y);
		}

		border += Vec2I(2, 2);
		//border = Vec2F();

		for (auto ch : newCharacters)
		{
			CharDef newCharDef;

			FT_Load_Char(mFreeTypeFace, ch, FT_LOAD_RENDER);
			auto glyph = mFreeTypeFace->glyph;

			Vec2I glyphSize(glyph->bitmap.width, glyph->bitmap.rows);

			Bitmap* newBitmap = mnew Bitmap(Bitmap::Format::R8G8B8A8, glyphSize + border*2);
			newBitmap->Fill(Color4(255, 255, 255, 0));
			UInt8* newBitmapData = newBitmap->GetData();
			Vec2I newBitmapSize = newBitmap->GetSize();

			for (int x = 0; x < (int)glyph->bitmap.width; x++)
			{
				for (int y = 0; y < (int)glyph->bitmap.rows; y++)
				{
					Color4 c(255, 255, 255, glyph->bitmap.buffer[y*glyph->bitmap.width + x]);
					ULong cl = c.ABGR();
					memcpy(&newBitmapData[((newBitmapSize.y - y - 1 - border.y)*newBitmapSize.x + x + border.x)*4], &cl, 4);
				}
			}

			for (auto effect : mEffects)
				effect->Process(newBitmap);

			newCharDef.mPackRect = mRectsPacker.AddRect(glyphSize + border*2);
			newCharDef.mBitmap = newBitmap;
			newCharDef.mCharacter.mId = ch;
			newCharDef.mCharacter.mHeight = height;
			newCharDef.mCharacter.mSize = newBitmapSize;
			newCharDef.mCharacter.mAdvance = glyph->advance.x/64.0f;
			newCharDef.mCharacter.mOrigin.x = -glyph->metrics.horiBearingX/64.0f + border.x;
			newCharDef.mCharacter.mOrigin.y = (glyph->metrics.height - glyph->metrics.horiBearingY)/64.0f + border.y;

			charDefs.Add(newCharDef);
		}
	}

	void VectorFont::PackCharactersDefs(CharDefsVec& charDefs)
	{
		mCharacters.Clear();

		int texSizes[] ={64, 128, 256, 512, 1024};
		Vec2I newTexSize;

		for (int i = 0; i < 5; i++)
		{
			newTexSize = Vec2I(texSizes[i], texSizes[i]);
			mRectsPacker.SetMaxSize(newTexSize);

			if (!mRectsPacker.Pack())
				continue;

			if (mRectsPacker.GetPagesCount() > 1)
				continue;

			break;
		}

		if (newTexSize != mTexture->GetSize())
		{
			mTexture = TextureRef(newTexSize, Texture::Format::R8G8B8A8, Texture::Usage::Default);
			mTextureSrcRect.Set(Vec2I(), newTexSize);
		}

		Bitmap texBitmap(Bitmap::Format::R8G8B8A8, mTexture->GetSize());
		texBitmap.Fill(Color4(255, 255, 255, 0));
		Vec2F invTexSize(1.0f/mTexture->GetSize().x, 1.0f/mTexture->GetSize().y);
		for (auto& def : charDefs)
		{
			texBitmap.CopyImage(def.mBitmap, def.mPackRect->mRect.LeftBottom());

			def.mCharacter.mTexSrc.left = def.mPackRect->mRect.left*invTexSize.x;
			def.mCharacter.mTexSrc.right = def.mPackRect->mRect.right*invTexSize.x;
			def.mCharacter.mTexSrc.top = def.mPackRect->mRect.bottom*invTexSize.y;
			def.mCharacter.mTexSrc.bottom = def.mPackRect->mRect.top*invTexSize.y;

			mCharacters.Add(def.mCharacter);

			delete def.mBitmap;
		}

		glBindTexture(GL_TEXTURE_2D, mTexture->mHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texBitmap.GetSize().x, texBitmap.GetSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
					 texBitmap.GetData());

		GL_CHECK_ERROR(o2Render.mLog);
		//glBindTexture(GL_TEXTURE_2D, o2Render.mLastDrawTexture->mHandle);
	}
}

CLASS_META(o2::VectorFont::Effect)
{
	BASE_CLASS(o2::ISerializable);


	PUBLIC_FUNCTION(void, Process, Bitmap*);
	PUBLIC_FUNCTION(Vec2I, GetSizeExtend);
	PUBLIC_FUNCTION(bool, IsEqual, Effect*);
}
END_META;
