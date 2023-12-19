#include "o2/stdafx.h"
#include "TextureRef.h"

#include "o2/Assets/Assets.h"
#include "o2/Render/Render.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	Ref<Texture>::Ref(const Vec2I& size,
					  TextureFormat format /*= TextureFormat::R8G8B8A8*/,
					  Texture::Usage usage /*= Texture::Usage::Default*/)
	{
		mPtr = mnew Texture(size, format, usage);
		IncrementRef();
	}

	Ref<Texture>::Ref(const String& fileName)
	{
		mPtr = o2Render.mTextures.FindOrDefault([&](const Ref<Texture>& tex) { return tex->GetFileName() == fileName; }).Get();

		if (!mPtr)
			mPtr = mnew Texture(fileName);

		IncrementRef();
	}

	Ref<Texture>::Ref(const Bitmap& bitmap)
	{
		mPtr = mnew Texture(bitmap);
		IncrementRef();
	}

	Ref<Texture>::Ref(UID atlasAssetId, int page)
	{
		mPtr = (o2Render.mTextures.FindOrDefault(
			[&](const Ref<Texture>& tex)
			{
				return tex->GetAtlasAssetId() == atlasAssetId && tex->GetAtlasPage() == page;
			}).Get());

		if (!mPtr)
			mPtr = mnew Texture(atlasAssetId, page);

		IncrementRef();
	}

	Ref<Texture>::Ref(const String& atlasAssetName, int page)
	{
		UID atlasAssetId = o2Assets.GetAssetId(atlasAssetName);
		if (atlasAssetId == 0) {
			o2Render.mLog->Error("Can't load texture for atlas " + atlasAssetName + " and page " + (String)page + ": atlas isn't exist");
			mPtr = nullptr;
			return;
		}

		mPtr = (o2Render.mTextures.FindOrDefault(
			[&](const Ref<Texture>& tex)
			{
				return tex->GetAtlasAssetId() == atlasAssetId && tex->GetAtlasPage() == page;
			}).Get());

		if (!mPtr)
			mPtr = mnew Texture(atlasAssetId, page);

		IncrementRef();
	}

	Ref<Texture> Ref<Texture>::Null()
	{
		return Ref<Texture>();
	}
}
// --- META ---

DECLARE_CLASS(o2::Ref<o2::Texture>, o2__Ref_o2__Texture_);
// --- END META ---
