#include "VectorFontAsset.h"

#include "Assets/Assets.h"
#include "Render/BitmapFont.h"
#include "Render/Font.h"
#include "Render/Render.h"

namespace o2
{
	VectorFontAsset::MetaInfo::~MetaInfo()
	{
		for (auto eff : mEffects)
			delete eff;
	}

	const Type* VectorFontAsset::MetaInfo::GetAssetType() const
	{
		return &TypeOf(VectorFontAsset);
	}

	bool VectorFontAsset::MetaInfo::IsEqual(IMetaInfo* other) const
	{
		if (!IMetaInfo::IsEqual(other))
			return false;

		MetaInfo* otherMeta = (MetaInfo*)other;
		for (auto eff : mEffects)
		{
			bool found = false;
			for (auto otherEff : otherMeta->mEffects)
			{
				if (eff && eff->IsEqual(otherEff))
				{
					found = true;
					break;
				}
			}

			if (!found)
				return false;
		}

		return true;
	}

	VectorFontAsset::VectorFontAsset():
		Asset()
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	VectorFontAsset::VectorFontAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);

		InitializeProperties();

		Load();
	}

	VectorFontAsset::VectorFontAsset(UID id)
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		Load();
	}

	VectorFontAsset::VectorFontAsset(const VectorFontAsset& asset):
		Asset(asset), mFont(asset.mFont)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		IdRef() = asset.GetAssetId();
		InitializeProperties();
	}

	VectorFontAsset::~VectorFontAsset()
	{}

	VectorFontAsset& VectorFontAsset::operator=(const VectorFontAsset& asset)
	{
		Asset::operator=(asset);
		mFont = asset.mFont;
		*mMeta = *(MetaInfo*)(asset.mMeta);
		return *this;
	}

	bool VectorFontAsset::operator==(const VectorFontAsset& other) const
	{
		return mMeta->IsEqual(other.mMeta);
	}

	bool VectorFontAsset::operator!=(const VectorFontAsset& other) const
	{
		return !mMeta->IsEqual(other.mMeta);
	}

	FontRef VectorFontAsset::GetFont() const
	{
		return mFont;
	}

	const VectorFontAsset::EffectsVec& VectorFontAsset::GetEffects() const
	{
		return GetMeta()->mEffects;
	}

	void VectorFontAsset::AddEffect(VectorFont::Effect* effect)
	{
		GetMeta()->mEffects.Add(effect);
		((VectorFont*)mFont.mFont)->AddEffect(effect);
	}

	void VectorFontAsset::RemoveEffect(VectorFont::Effect* effect)
	{
		GetMeta()->mEffects.Remove(effect);
		((VectorFont*)mFont.mFont)->RemoveEffect(effect);
	}

	void VectorFontAsset::RemoveAllEffects()
	{
		for (auto eff : GetMeta()->mEffects)
			delete eff;

		GetMeta()->mEffects.Clear();

		((VectorFont*)mFont.mFont)->RemoveAllEffects();
	}

	const char* VectorFontAsset::GetFileExtensions() const
	{
		return "ttf";
	}

	VectorFontAsset::MetaInfo* VectorFontAsset::GetMeta() const
	{
		return (MetaInfo*)mMeta;
	}

	void VectorFontAsset::LoadData(const String& path)
	{
		mFont = o2Render.mFonts.FindMatch([&](auto fnt) { return fnt->GetFileName() == path; });

		if (!mFont)
		{
			mFont = mnew VectorFont(path);

			VectorFont* vectorFont = (VectorFont*)mFont.mFont;
			for (auto eff : GetMeta()->mEffects)
			{
				if (eff)
					vectorFont->AddEffect(eff->CloneAs<VectorFont::Effect>());
			}
		}
	}

	void VectorFontAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(VectorFontAsset, meta, GetMeta);
		INITIALIZE_GETTER(VectorFontAsset, font, GetFont);
	}

	VectorFontAssetRef VectorFontAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<VectorFontAsset>();
	}

}

CLASS_META(o2::VectorFontAsset)
{
	BASE_CLASS(o2::Asset);

	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(font);
	PROTECTED_FIELD(mFont);

	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(FontRef, GetFont);
	PUBLIC_FUNCTION(const EffectsVec&, GetEffects);
	PUBLIC_FUNCTION(void, AddEffect, VectorFont::Effect*);
	PUBLIC_FUNCTION(void, RemoveEffect, VectorFont::Effect*);
	PUBLIC_FUNCTION(void, RemoveAllEffects);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(o2::VectorFontAssetRef)
{
	BASE_CLASS(o2::AssetRef);


	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_META(o2::VectorFontAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);

	PROTECTED_FIELD(mEffects).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(bool, IsEqual, IMetaInfo*);
}
END_META;
