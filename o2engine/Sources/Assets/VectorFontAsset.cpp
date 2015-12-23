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
			eff.Release();
	}

	Type::Id VectorFontAsset::MetaInfo::GetAssetType() const
	{
		return VectorFontAsset::type->ID();
	}

	bool VectorFontAsset::MetaInfo::IsEqual(Ptr<IMetaInfo> other) const
	{
		if (!IMetaInfo::IsEqual(other))
			return false;

		Ptr<MetaInfo> otherMeta = other.Cast<MetaInfo>();
		for (auto eff : mEffects)
		{
			bool found = false;
			for (auto otherEff : otherMeta->mEffects)
			{
				if (eff->IsEqual(otherEff))
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

	VectorFontAsset::VectorFontAsset(AssetId id)
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
		InitializeProperties();
	}

	VectorFontAsset::~VectorFontAsset()
	{}

	FontRef VectorFontAsset::GetFont() const
	{
		return mFont;
	}

	const VectorFontAsset::EffectsVec& VectorFontAsset::GetEffects() const
	{
		return GetMeta()->mEffects;
	}

	void VectorFontAsset::AddEffect(Ptr<VectorFont::Effect> effect)
	{
		GetMeta()->mEffects.Add(effect);
		mFont.mFont.Cast<VectorFont>()->AddEffect(effect);
	}

	void VectorFontAsset::RemoveEffect(Ptr<VectorFont::Effect> effect)
	{
		GetMeta()->mEffects.Remove(effect);
		mFont.mFont.Cast<VectorFont>()->RemoveEffect(effect);
	}

	void VectorFontAsset::RemoveAllEffects()
	{
		for (auto eff : GetMeta()->mEffects)
			eff.Release();

		GetMeta()->mEffects.Clear();

		mFont.mFont.Cast<VectorFont>()->RemoveAllEffects();
	}

	const char* VectorFontAsset::GetFileExtensions() const
	{
		return "ttf";
	}

	Ptr<VectorFontAsset::MetaInfo> VectorFontAsset::GetMeta() const
	{
		return mMeta.Cast<MetaInfo>();
	}

	void VectorFontAsset::LoadData(const String& path)
	{
		mFont = o2Render.mFonts.FindMatch([&](auto fnt) { return fnt->GetFileName() == path; });

		if (!mFont)
		{
			mFont = Ptr<Font>(mnew VectorFont(path));

			Ptr<VectorFont> vectorFont = mFont.mFont.Cast<VectorFont>();
			for (auto eff : GetMeta()->mEffects)
				vectorFont->AddEffect(eff->Clone());
		}
	}

	void VectorFontAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(VectorFontAsset, meta, GetMeta);
		INITIALIZE_GETTER(VectorFontAsset, font, GetFont);
	}

	VectorFontAsset& VectorFontAsset::operator=(const VectorFontAsset& asset)
	{
		mFont = asset.mFont;
		return *this;
	}
}