#include "o2/stdafx.h"
#include "VectorFontAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Render/BitmapFont.h"
#include "o2/Render/Font.h"
#include "o2/Render/Render.h"

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

	bool VectorFontAsset::MetaInfo::IsEqual(AssetMeta* other) const
	{
		if (!AssetMeta::IsEqual(other))
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
		FontAsset()
	{
		mMeta = mnew MetaInfo();
	}

	VectorFontAsset::VectorFontAsset(const String& path):
		FontAsset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		ID() = o2Assets.GetAssetId(path);

		Load();
	}

	VectorFontAsset::VectorFontAsset(const UID& id)
	{
		mMeta = mnew MetaInfo();
		ID() = id;
		mPath = o2Assets.GetAssetPath(id);

		Load();
	}

	VectorFontAsset::VectorFontAsset(const VectorFontAsset& asset):
		FontAsset(asset), meta(this)
	{
		mMeta = mnew MetaInfo();
		mPath = asset.mPath;
		ID() = asset.GetUID();
	}

	VectorFontAsset::~VectorFontAsset()
	{}

	VectorFontAsset& VectorFontAsset::operator=(const VectorFontAsset& asset)
	{
		FontAsset::operator=(asset);
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

	const Vector<VectorFont::Effect*>& VectorFontAsset::GetEffects() const
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

	VectorFontAssetRef VectorFontAssetRef::CreateAsset()
	{
		return o2Assets.CreateAsset<VectorFontAsset>();
	}

}

DECLARE_CLASS(o2::VectorFontAsset);

DECLARE_CLASS(o2::VectorFontAssetRef);

DECLARE_CLASS(o2::VectorFontAsset::MetaInfo);
