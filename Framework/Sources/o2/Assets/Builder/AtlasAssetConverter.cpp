#include "o2/stdafx.h"
#include "AtlasAssetConverter.h"

#include "o2/Assets/Assets.h"
#include "o2/Assets/AtlasAsset.h"
#include "o2/Assets/ImageAsset.h"
#include "o2/Utils/Bitmap/Bitmap.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	Vector<const Type*> AtlasAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<const Type*> res;
		res.Add(&TypeOf(AtlasAsset));
		return res;
	}

	void AtlasAssetConverter::ConvertAsset(const AssetsTree::AssetInfo& node)
	{
		String sourceAssetPath = o2Assets.GetAssetsPath() + node.mPath;
		String buildedAssetPath = mAssetsBuilder->GetBuiltAssetsPath() + node.mPath;

		if (!o2FileSystem.IsFileExist(buildedAssetPath))
			o2FileSystem.WriteFile(buildedAssetPath, "");
	}

	void AtlasAssetConverter::RemoveAsset(const AssetsTree::AssetInfo& node)
	{
		String buildedAssetPath = mAssetsBuilder->GetBuiltAssetsPath() + node.mPath;

		DataNode atlasData;
		atlasData.LoadFromFile(buildedAssetPath);
		int pagesCount = atlasData["mPages"].GetChildNodes().Count();

		for (int i = 0; i < pagesCount; i++)
			o2FileSystem.FileDelete(buildedAssetPath + (String)i + ".png");

		o2FileSystem.FileDelete(buildedAssetPath);
	}

	void AtlasAssetConverter::MoveAsset(const AssetsTree::AssetInfo& nodeFrom, const AssetsTree::AssetInfo& nodeTo)
	{
		String fullPathFrom = mAssetsBuilder->GetBuiltAssetsPath() + nodeFrom.mPath;
		String fullPathTo = mAssetsBuilder->GetBuiltAssetsPath() + nodeTo.mPath;

		DataNode atlasData;
		atlasData.LoadFromFile(fullPathFrom);
		int pagesCount = atlasData["mPages"].GetChildNodes().Count();

		for (int i = 0; i < pagesCount; i++)
			o2FileSystem.FileMove(fullPathFrom + (String)i + ".png", fullPathTo + (String)i + ".png");

		o2FileSystem.FileMove(fullPathFrom, fullPathTo);
	}

	Vector<UID> AtlasAssetConverter::AssetsPostProcess()
	{
		CheckBasicAtlas();
		return CheckRebuildingAtlases();
	}

	void AtlasAssetConverter::Reset()
	{}

	void AtlasAssetConverter::CheckBasicAtlas()
	{
		const Type* imageType = &TypeOf(ImageAsset);
		const Type* atlasType = &TypeOf(AtlasAsset);

		AssetsTree::AssetInfo* basicAtlasInfo = nullptr;

		Vector<UID> availableAtlasesIds;
		for (auto assetInfo : mAssetsBuilder->mBuiltAssetsTree.allAssets)
		{
			if (assetInfo->mAssetType == atlasType)
			{
				availableAtlasesIds.Add(assetInfo->mMeta->ID());
			}

			if (assetInfo->mPath == GetBasicAtlasPath())
				basicAtlasInfo = assetInfo;
		}

		for (auto assetInfo : mAssetsBuilder->mBuiltAssetsTree.allAssets)
		{
			if (assetInfo->mAssetType == imageType)
			{
				ImageAsset::MetaInfo* imageMeta = (ImageAsset::MetaInfo*)assetInfo->mMeta;

				if (!availableAtlasesIds.Contains(imageMeta->mAtlasId))
					imageMeta->mAtlasId = basicAtlasInfo->mMeta->ID();
			}
		}
	}

	Vector<UID> AtlasAssetConverter::CheckRebuildingAtlases()
	{
		Vector<UID> res;
		const Type* atlasAssetType = &TypeOf(AtlasAsset);

		for (auto info : mAssetsBuilder->mBuiltAssetsTree.allAssets)
		{
			if (info->mAssetType == atlasAssetType)
				if (CheckAtlasRebuilding(info))
					res.Add(info->id);
		}

		return res;
	}

	bool AtlasAssetConverter::CheckAtlasRebuilding(AssetsTree::AssetInfo* atlasInfo)
	{
		DataNode atlasData;
		atlasData.LoadFromFile(mAssetsBuilder->mBuiltAssetsPath + atlasInfo->mPath);

		Vector<Image> lastImages;
		lastImages = atlasData["AllImages"];

		Vector<Image> currentImages;
		const Type* imageType = &TypeOf(ImageAsset);
		const UID& atlasId = atlasInfo->mMeta->ID();
		for (auto assetInfo : mAssetsBuilder->mBuiltAssetsTree.allAssets)
		{
			if (assetInfo->mAssetType == imageType)
			{
				ImageAsset::MetaInfo* imageMeta = (ImageAsset::MetaInfo*)assetInfo->mMeta;
				if (imageMeta->mAtlasId == atlasId)
					currentImages.Add(Image(imageMeta->ID(), assetInfo->mEditTime));
			}
		}

		if (IsAtlasNeedRebuild(currentImages, lastImages))
		{
			RebuildAtlas(atlasInfo, currentImages);
			return true;
		}

		return false;
	}

	bool AtlasAssetConverter::IsAtlasNeedRebuild(Vector<Image>& currentImages, Vector<Image>& lastImages)
	{
		if (currentImages.Count() != lastImages.Count())
			return true;

		for (auto lastImg : lastImages)
		{
			bool found = false;
			for (auto curImg : currentImages)
			{
				if (lastImg.id == curImg.id)
				{
					if (lastImg.time != curImg.time)
					{
						return true;
					}
					else
					{
						found = true;
						break;
					}
				}
			}

			if (!found)
				return true;
		}

		for (auto curImg : currentImages)
		{
			if (mAssetsBuilder->mModifiedAssets.ContainsPred([&](auto x) { return x->id == curImg.id; }))
				return true;
		}

		return false;
	}

	void AtlasAssetConverter::RebuildAtlas(AssetsTree::AssetInfo* atlasInfo, Vector<Image>& images)
	{
		auto meta = (AtlasAsset::MetaInfo*)atlasInfo->mMeta;

		RectsPacker packer(meta->mWindows.mMaxSize);
		float imagesBorder = (float)meta->mBorder;

		// Initialize pack images
		Vector<ImagePackDef> packImages;
		for (auto img : images)
		{
			// Find image info
			AssetsTree::AssetInfo* imgInfo =
				mAssetsBuilder->mBuiltAssetsTree.allAssets.FindMatch([&](auto info) { return info->meta->ID() == img.id; });

			if (!imgInfo)
			{
				mAssetsBuilder->mLog->Error("Can't find asset info by id: " + (String)img.id);
				continue;
			}

			String assetFullPath = mAssetsBuilder->GetSourceAssetsPath() + imgInfo->mPath;

			// Load bitmap
			Bitmap* bitmap = mnew Bitmap();
			if (!bitmap->Load(assetFullPath))
			{
				mAssetsBuilder->mLog->Error("Can't load bitmap for image asset: " + imgInfo->mPath);
				delete bitmap;
				continue;
			}

			// Create packing rect
			RectsPacker::Rect* packRect = packer.AddRect(bitmap->GetSize() +
														 Vec2F(imagesBorder*2.0f, imagesBorder*2.0f));

			ImagePackDef imagePackDef;
			imagePackDef.assetInfo = imgInfo;
			imagePackDef.bitmap = bitmap;
			imagePackDef.packRect = packRect;

			packImages.Add(imagePackDef);
		}

		// Try to pack
		if (!packer.Pack())
		{
			mAssetsBuilder->mLog->Error("Atlas " + atlasInfo->mPath + " packing failed");
			return;
		}
		else
			mAssetsBuilder->mLog->Out("Atlas " + atlasInfo->mPath + " successfully packed");

		// Initialize bitmaps and pages
		int pagesCount = packer.GetPagesCount();
		Vector<Bitmap*> resAtlasBitmaps;
		Vector<AtlasAsset::Page> resAtlasPages;
		for (int i = 0; i < pagesCount; i++)
		{
			AtlasAsset::Page atlasPage;
			atlasPage.mId = 0;
			atlasPage.mSize = packer.GetMaxSize();
			resAtlasPages.Add(atlasPage);

			Bitmap* newBitmap = mnew Bitmap(PixelFormat::R8G8B8A8, packer.GetMaxSize());
			newBitmap->Fill(Color4(255, 255, 255, 0));

			resAtlasBitmaps.Add(newBitmap);
		}

		// Save image assets data and fill pages
		Vector<AssetInfo> atlasImagesInfos;
		for (auto imgDef : packImages)
		{
			imgDef.packRect->rect.left += imagesBorder;
			imgDef.packRect->rect.right -= imagesBorder;
			imgDef.packRect->rect.top -= imagesBorder;
			imgDef.packRect->rect.bottom += imagesBorder;

			atlasImagesInfos.Add(AssetInfo(imgDef.assetInfo->mPath, imgDef.assetInfo->mMeta->ID(), &TypeOf(ImageAsset)));

			resAtlasBitmaps[imgDef.packRect->page]->CopyImage(imgDef.bitmap,
																imgDef.packRect->rect.LeftBottom());

			resAtlasPages[imgDef.packRect->page].mImagesRects.Add(imgDef.assetInfo->mMeta->ID(),
																	imgDef.packRect->rect);

			SaveImageAsset(imgDef);
		}

		// Save pages bitmaps
		for (int i = 0; i < pagesCount; i++)
		{
			resAtlasBitmaps[i]->Save(mAssetsBuilder->GetBuiltAssetsPath() + atlasInfo->mPath + (String)i + ".png",
									 Bitmap::ImageType::Png);

			delete resAtlasBitmaps[i];
		}

		// Save atlas data
		DataNode atlasData;
		atlasData["mPages"] = resAtlasPages;
		atlasData["AllImages"] = images;
		atlasData["mImagesAssetsInfos"] = atlasImagesInfos;

		String atlasFullPath = mAssetsBuilder->GetBuiltAssetsPath() + atlasInfo->mPath;
		atlasData.SaveToFile(atlasFullPath);
		o2FileSystem.SetFileEditDate(atlasFullPath, atlasInfo->mEditTime);
	}

	void AtlasAssetConverter::SaveImageAsset(ImagePackDef& imgDef)
	{
		DataNode imgData;
		imgData["mAtlasPage"] = imgDef.packRect->page;
		imgData["mAtlasRect"] = (RectI)(imgDef.packRect->rect);
		String imageFullPath = mAssetsBuilder->GetBuiltAssetsPath() + imgDef.assetInfo->mPath;
		imgData.SaveToFile(imageFullPath);
		o2FileSystem.SetFileEditDate(imageFullPath, imgDef.assetInfo->mEditTime);

		DataNode metaData;
		metaData = imgDef.assetInfo->mMeta;
		metaData.SaveToFile(mAssetsBuilder->GetSourceAssetsPath() + imgDef.assetInfo->mPath + ".meta");
	}

	AtlasAssetConverter::Image::Image(const UID& id, const TimeStamp& time):
		id(id), time(time)
	{}

	bool AtlasAssetConverter::Image::operator==(const Image& other) const
	{
		return id == other.id;
	}

	bool AtlasAssetConverter::ImagePackDef::operator==(const ImagePackDef& other) const
	{
		return assetInfo == other.assetInfo && bitmap == other.bitmap && packRect == other.packRect;
	}
}

DECLARE_CLASS(o2::AtlasAssetConverter);

DECLARE_CLASS(o2::AtlasAssetConverter::Image);
