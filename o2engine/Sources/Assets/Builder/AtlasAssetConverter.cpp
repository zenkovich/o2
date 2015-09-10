#include "AtlasAssetConverter.h"

#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Assets/ImageAsset.h"
#include "Utils/Bitmap.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	IOBJECT_CPP(AtlasAssetConverter);
	IOBJECT_CPP(AtlasAssetConverter::Image);

	Vector<Type::Id> AtlasAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<Type::Id> res;
		res.Add(AtlasAsset::type.ID());
		return res;
	}

	void AtlasAssetConverter::ConvertAsset(const String& path)
	{
		String sourceAssetPath = o2Assets.GetAssetsPath() + path;
		String buildedAssetPath = o2Assets.GetDataPath() + path;
		String sourceAssetMetaPath = sourceAssetPath + ".meta";
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		if (!o2FileSystem.IsFileExist(buildedAssetPath))
			o2FileSystem.WriteFile(buildedAssetPath, "");

		o2FileSystem.FileCopy(sourceAssetMetaPath, buildedAssetMetaPath);
	}

	void AtlasAssetConverter::RemoveAsset(const String& path)
	{
		String buildedAssetPath = o2Assets.GetDataPath() + path;
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		DataNode atlasData;
		atlasData.LoadFromFile(buildedAssetPath);
		int pagesCount = atlasData["mPages"]->GetChildNodes().Count();

		for (int i = 0; i < pagesCount; i++)
			o2FileSystem.FileDelete(buildedAssetPath + (String)i + ".png");

		o2FileSystem.FileDelete(buildedAssetPath);
		o2FileSystem.FileDelete(buildedAssetMetaPath);
	}

	void AtlasAssetConverter::MoveAsset(const String& pathFrom, const String& pathTo)
	{
		String fullPathFrom = o2Assets.GetDataPath() + pathFrom;
		String fullPathTo = o2Assets.GetDataPath() + pathTo;
		String fullMetaPathFrom = fullPathFrom + ".meta";
		String fullMetaPathTo = fullPathTo + ".meta";

		DataNode atlasData;
		atlasData.LoadFromFile(fullPathFrom);
		int pagesCount = atlasData["mPages"]->GetChildNodes().Count();

		for (int i = 0; i < pagesCount; i++)
			o2FileSystem.FileMove(fullPathFrom + (String)i + ".png",
								  fullPathTo + (String)i + ".png");

		o2FileSystem.FileMove(fullPathFrom, fullPathTo);
		o2FileSystem.FileMove(fullMetaPathFrom, fullMetaPathTo);
	}

	void AtlasAssetConverter::AssetsPostProcess()
	{
		CheckBasicAtlas();
		CheckRebuildingAtlases();
	}

	void AtlasAssetConverter::Reset()
	{
	}

	void AtlasAssetConverter::CheckBasicAtlas()
	{
		Type::Id imageTypeId = ImageAsset::type.ID();
		Type::Id atlasTypeId = AtlasAsset::type.ID();

		Ptr<AssetTree::AssetNode> basicAtlasInfo;

		Vector<AssetId> availableAtlasesIds;
		for (auto assetInfo : mAssetsBuilder->mBuildedAssetsTree.mAllAssets)
		{
			if (assetInfo->mType == atlasTypeId)
			{
				availableAtlasesIds.Add(assetInfo->mMeta->ID());
			}

			if (assetInfo->mPath == BASIC_ATLAS_PATH)
				basicAtlasInfo = assetInfo;
		}

		for (auto assetInfo : mAssetsBuilder->mBuildedAssetsTree.mAllAssets)
		{
			if (assetInfo->mType == imageTypeId)
			{
				Ptr<ImageAsset::MetaInfo> imageMeta = assetInfo->mMeta.Cast<ImageAsset::MetaInfo>();

				if (!availableAtlasesIds.Contains(imageMeta->mAtlasId))
					imageMeta->mAtlasId = basicAtlasInfo->mMeta->ID();
			}
		}
	}

	void AtlasAssetConverter::CheckRebuildingAtlases()
	{
		Type::Id atlasAssetTypeId = AtlasAsset::type.ID();

		for (auto info : mAssetsBuilder->mBuildedAssetsTree.mAllAssets)
		{
			if (info->mType == atlasAssetTypeId)
				CheckAtlasRebuilding(info);
		}
	}

	void AtlasAssetConverter::CheckAtlasRebuilding(Ptr<AssetTree::AssetNode> atlasInfo)
	{
		DataNode atlasData;
		atlasData.LoadFromFile(mAssetsBuilder->mBuildedAssetsPath + atlasInfo->mPath);

		ImagesVec lastImages;
		lastImages = *atlasData["AllImages"];

		ImagesVec currentImages;
		Type::Id imageTypeId = ImageAsset::type.ID();
		AssetId atlasId = atlasInfo->mMeta->ID();
		for (auto assetInfo : mAssetsBuilder->mBuildedAssetsTree.mAllAssets)
		{
			if (assetInfo->mType == imageTypeId)
			{
				Ptr<ImageAsset::MetaInfo> imageMeta = assetInfo->mMeta.Cast<ImageAsset::MetaInfo>();
				if (imageMeta->mAtlasId == atlasId)
				{
					currentImages.Add(Image(imageMeta->ID(), assetInfo->mTime));
				}
			}
		}

		if (IsAtlasNeedRebuild(currentImages, lastImages))
			RebuildAtlas(atlasInfo, currentImages);
	}

	bool AtlasAssetConverter::IsAtlasNeedRebuild(ImagesVec& currentImages, ImagesVec& lastImages)
	{
		bool needRebuild = currentImages.Count() != lastImages.Count();
		if (!needRebuild)
		{
			for (auto lastImg : lastImages)
			{
				bool found = false;
				for (auto curImg : currentImages)
				{
					if (lastImg.mId == curImg.mId)
					{
						if (lastImg.mTime != curImg.mTime)
						{
							needRebuild = true;
							break;
						}
						else
						{
							found = true;
							break;
						}
					}
				}

				if (needRebuild)
					break;
			}
		}
		return needRebuild;
	}

	void AtlasAssetConverter::RebuildAtlas(Ptr<AssetTree::AssetNode> atlasInfo, ImagesVec& images)
	{
		RectsPacker packer(atlasInfo->mMeta.Cast<AtlasAsset::MetaInfo>()->mWindows.mMaxSize);

		// Initialize pack images
		ImagePackDefsVec packImages;
		for (auto img : images)
		{
			// Find image info
			Ptr<AssetTree::AssetNode> imgInfo =
				mAssetsBuilder->mBuildedAssetsTree.mAllAssets.FindMatch([&](auto info) { return info->mMeta->ID() == img.mId; });

			if (!imgInfo)
			{
				mAssetsBuilder->mLog->Error("Can't find asset info by id: %i", img.mId);
				continue;
			}

			String assetFullPath = mAssetsBuilder->mSourceAssetsPath + imgInfo->mPath;

			// Load bitmap
			Ptr<Bitmap> bitmap = mnew Bitmap();
			if (!bitmap->Load(assetFullPath))
			{
				mAssetsBuilder->mLog->Error("Can't load bitmap for image asset: %s", imgInfo->mPath);
				bitmap.Release();
				continue;
			}

			// Create packing rect
			Ptr<RectsPacker::Rect> packRect = packer.AddRect(bitmap->GetSize());

			ImagePackDef imagePackDef;
			imagePackDef.mAssetInfo = imgInfo;
			imagePackDef.mBitmap = bitmap;
			imagePackDef.mPackRect = packRect;

			packImages.Add(imagePackDef);
		}

		// Try to pack
		if (!packer.Pack())
		{
			mAssetsBuilder->mLog->Error("Atlas %s packing failed", atlasInfo->mPath);
			return;
		}
		else
			mAssetsBuilder->mLog->Out("Atlas %s successfully packed", atlasInfo->mPath);

		// Initialize bitmaps and pages
		int pagesCount = packer.GetPagesCount();
		Vector<Ptr<Bitmap>> resAtlasBitmaps;
		Vector<AtlasAsset::Page> resAtlasPages;
		for (int i = 0; i < pagesCount; i++)
		{
			AtlasAsset::Page atlasPage;
			atlasPage.mId = 0;
			atlasPage.mSize = packer.GetMaxSize();
			resAtlasPages.Add(atlasPage);

			resAtlasBitmaps.Add(mnew Bitmap(Bitmap::Format::R8G8B8A8, packer.GetMaxSize()));
		}

		// Save image assets data and fill pages
		AssetInfosVec atlasImagesInfos;
		for (auto imgDef : packImages)
		{
			atlasImagesInfos.Add(AssetInfo(imgDef.mAssetInfo->mPath, imgDef.mAssetInfo->mMeta->ID(), ImageAsset::type.ID()));

			resAtlasBitmaps[imgDef.mPackRect->mPage]->CopyImage(imgDef.mBitmap, imgDef.mPackRect->mRect.LeftBottom());

			resAtlasPages[imgDef.mPackRect->mPage].mImagesRects.Add(imgDef.mAssetInfo->mMeta->ID(), imgDef.mPackRect->mRect);

			SaveImageAsset(imgDef);
		}

		// Save pages bitmaps
		for (int i = 0; i < pagesCount; i++)
		{
			resAtlasBitmaps[i]->Save(mAssetsBuilder->mBuildedAssetsPath + atlasInfo->mPath + (String)i + ".png",
									 Bitmap::ImageType::Png);

			resAtlasBitmaps[i].Release();
		}

		// Save atlas data
		DataNode atlasData;
		*atlasData["mPages"] = resAtlasPages;
		*atlasData["AllImages"] = images;
		*atlasData["mImagesAssetsInfos"] = atlasImagesInfos;

		String atlasFullPath = mAssetsBuilder->mBuildedAssetsPath + atlasInfo->mPath;
		atlasData.SaveToFile(atlasFullPath);
		o2FileSystem.SetFileEditDate(atlasFullPath, atlasInfo->mTime);
	}

	void AtlasAssetConverter::SaveImageAsset(ImagePackDef& imgDef)
	{
		DataNode imgData;
		*imgData["mAtlasPage"] = 0;
		*imgData["mAtlasRect"] = (RectI)(imgDef.mPackRect->mRect);
		String imageFullPath = mAssetsBuilder->mBuildedAssetsPath + imgDef.mAssetInfo->mPath;
		imgData.SaveToFile(imageFullPath);
		o2FileSystem.SetFileEditDate(imageFullPath, imgDef.mAssetInfo->mTime);
	}

	AtlasAssetConverter::Image::Image(AssetId id, const TimeStamp& time):
		mId(id), mTime(time)
	{}

	bool AtlasAssetConverter::Image::operator==(const Image& other) const
	{
		return mId == other.mId;
	}

	bool AtlasAssetConverter::ImagePackDef::operator==(const ImagePackDef& other) const
	{
		return mAssetInfo == other.mAssetInfo && mBitmap == other.mBitmap && mPackRect == other.mPackRect;
	}
}