#include "UIAssetIcon.h"

#include "Render/Text.h"
#include "Utils/FileSystem/FileSystem.h"

UIAssetIcon::UIAssetIcon():
	UIWidget(), mNameText(nullptr)
{
	RetargetStatesAnimations();
}

UIAssetIcon::UIAssetIcon(const UIAssetIcon& other):
	UIWidget(other)
{
	mNameText = GetLayerDrawable<Text>("assetName");

	RetargetStatesAnimations();
	UpdateLayout();
}

UIAssetIcon::~UIAssetIcon()
{}

UIAssetIcon& UIAssetIcon::operator=(const UIAssetIcon& other)
{
	UIWidget::operator=(other);
	mNameText = GetLayerDrawable<Text>("assetName");

	return *this;
}

void UIAssetIcon::SetAssetInfo(const AssetInfo& info)
{
	mAssetInfo = info;

	if (mNameText)
		mNameText->text = o2FileSystem.GetPathWithoutDirectories(info.mPath);
}

const AssetInfo& UIAssetIcon::GetAssetInfo() const
{
	return mAssetInfo;
}
