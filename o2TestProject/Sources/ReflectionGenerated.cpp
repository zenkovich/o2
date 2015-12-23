#include "C:\work\o2\o2TestProject\Platforms\Windows\..\..\Sources\ReflectionGenerated.h"

#include "Utils/Reflection/Reflection.h"

// Includes
#include "C:\work\o2\o2TestProject\Platforms\Windows\..\..\Sources\ITestScreen.h"
#include "C:\work\o2\o2Engine\Sources\Animation\Animatable.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimatedFloat.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimatedValue.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimatedVector.h"
#include "C:\work\o2\o2Engine\Sources\Animation\Animation.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimationMask.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimationState.h"
#include "C:\work\o2\o2Engine\Sources\Animation\IAnimation.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Asset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\AssetInfo.h"
#include "C:\work\o2\o2Engine\Sources\Assets\AtlasAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\BinaryAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\BitmapFontAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\FolderAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\ImageAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\VectorFontAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Builder\AtlasAssetConverter.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Builder\FolderAssetConverter.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Builder\IAssetConverter.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Builder\ImageAssetConverter.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Builder\StdAssetConverter.h"
#include "C:\work\o2\o2Engine\Sources\Config\ProjectConfig.h"
#include "C:\work\o2\o2Engine\Sources\Render\Camera.h"
#include "C:\work\o2\o2Engine\Sources\Render\RectDrawable.h"
#include "C:\work\o2\o2Engine\Sources\Render\Sprite.h"
#include "C:\work\o2\o2Engine\Sources\Render\Text.h"
#include "C:\work\o2\o2Engine\Sources\Render\VectorFontEffects.h"
#include "C:\work\o2\o2Engine\Sources\Scene\Actor.h"
#include "C:\work\o2\o2Engine\Sources\Scene\ActorTransform.h"
#include "C:\work\o2\o2Engine\Sources\Scene\Component.h"
#include "C:\work\o2\o2Engine\Sources\Scene\DrawableComponent.h"
#include "C:\work\o2\o2Engine\Sources\Scene\Components\ImageComponent.h"
#include "C:\work\o2\o2Engine\Sources\UI\Button.h"
#include "C:\work\o2\o2Engine\Sources\UI\ContextMenu.h"
#include "C:\work\o2\o2Engine\Sources\UI\CustomDropDown.h"
#include "C:\work\o2\o2Engine\Sources\UI\CustomList.h"
#include "C:\work\o2\o2Engine\Sources\UI\DropDown.h"
#include "C:\work\o2\o2Engine\Sources\UI\EditBox.h"
#include "C:\work\o2\o2Engine\Sources\UI\HorizontalLayout.h"
#include "C:\work\o2\o2Engine\Sources\UI\HorizontalProgress.h"
#include "C:\work\o2\o2Engine\Sources\UI\HorizontalScrollBar.h"
#include "C:\work\o2\o2Engine\Sources\UI\Label.h"
#include "C:\work\o2\o2Engine\Sources\UI\List.h"
#include "C:\work\o2\o2Engine\Sources\UI\ScrollArea.h"
#include "C:\work\o2\o2Engine\Sources\UI\Toggle.h"
#include "C:\work\o2\o2Engine\Sources\UI\VerticalLayout.h"
#include "C:\work\o2\o2Engine\Sources\UI\VerticalProgress.h"
#include "C:\work\o2\o2Engine\Sources\UI\VerticalScrollBar.h"
#include "C:\work\o2\o2Engine\Sources\UI\Widget.h"
#include "C:\work\o2\o2Engine\Sources\UI\WidgetLayer.h"
#include "C:\work\o2\o2Engine\Sources\UI\WidgetLayout.h"
#include "C:\work\o2\o2Engine\Sources\UI\WidgetState.h"
#include "C:\work\o2\o2Engine\Sources\UI\Window.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Serialization.h"
#include "C:\work\o2\o2Engine\Sources\Utils\TimeStamp.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Math\Curve.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Math\Layout.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Math\Transform.h"
#include "C:\work\o2\o2Engine\Sources\Assets\AssetsTree.h"
#include "C:\work\o2\o2Engine\Sources\Render\VectorFont.h"

// Types declarations
o2::Type* ::Test::type;
o2::Type* o2::Animatable::type;
o2::Type* o2::AnimatedValue<float>::type;
o2::Type* o2::IAnimatedValue::type;
o2::Type* o2::AnimatedValue<Vec2F>::type;
o2::Type* o2::Animation::type;
o2::Type* o2::AnimationMask::type;
o2::Type* o2::AnimationState::type;
o2::Type* o2::IAnimation::type;
o2::Type* o2::Asset::type;
o2::Type* o2::AssetInfo::type;
o2::Type* o2::AtlasAsset::type;
o2::Type* o2::BinaryAsset::type;
o2::Type* o2::BitmapFontAsset::type;
o2::Type* o2::FolderAsset::type;
o2::Type* o2::ImageAsset::type;
o2::Type* o2::VectorFontAsset::type;
o2::Type* o2::AtlasAssetConverter::type;
o2::Type* o2::FolderAssetConverter::type;
o2::Type* o2::IAssetConverter::type;
o2::Type* o2::ImageAssetConverter::type;
o2::Type* o2::StdAssetConverter::type;
o2::Type* o2::ProjectConfig::type;
o2::Type* o2::Camera::type;
o2::Type* o2::IRectDrawable::type;
o2::Type* o2::Sprite::type;
o2::Type* o2::Text::type;
o2::Type* o2::FontStrokeEffect::type;
o2::Type* o2::FontGradientEffect::type;
o2::Type* o2::FontColorEffect::type;
o2::Type* o2::FontShadowEffect::type;
o2::Type* o2::Actor::type;
o2::Type* o2::ActorTransform::type;
o2::Type* o2::Component::type;
o2::Type* o2::DrawableComponent::type;
o2::Type* o2::ImageComponent::type;
o2::Type* o2::UIButton::type;
o2::Type* o2::UIContextMenu::type;
o2::Type* o2::UICustomDropDown::type;
o2::Type* o2::UICustomList::type;
o2::Type* o2::UIDropDown::type;
o2::Type* o2::UIEditBox::type;
o2::Type* o2::UIHorizontalLayout::type;
o2::Type* o2::UIHorizontalProgress::type;
o2::Type* o2::UIHorizontalScrollBar::type;
o2::Type* o2::UILabel::type;
o2::Type* o2::UIList::type;
o2::Type* o2::UIScrollArea::type;
o2::Type* o2::UIToggle::type;
o2::Type* o2::UIVerticalLayout::type;
o2::Type* o2::UIVerticalProgress::type;
o2::Type* o2::UIVerticalScrollBar::type;
o2::Type* o2::UIWidget::type;
o2::Type* o2::UIWidgetLayer::type;
o2::Type* o2::UIWidgetLayout::type;
o2::Type* o2::UIWidgetState::type;
o2::Type* o2::UIWindow::type;
o2::Type* o2::ISerializable::type;
o2::Type* o2::TimeStamp::type;
o2::Type* o2::Curve::type;
o2::Type* o2::Layout::type;
o2::Type* o2::Transform::type;
o2::Type* o2::AnimatedValue<Vec2F>::Key::type;
o2::Type* o2::Animation::AnimatedValueDef::type;
o2::Type* o2::Asset::IMetaInfo::type;
o2::Type* o2::AssetTree::AssetNode::type;
o2::Type* o2::AtlasAsset::PlatformMeta::type;
o2::Type* o2::AtlasAsset::MetaInfo::type;
o2::Type* o2::AtlasAsset::Page::type;
o2::Type* o2::BinaryAsset::MetaInfo::type;
o2::Type* o2::BitmapFontAsset::MetaInfo::type;
o2::Type* o2::FolderAsset::MetaInfo::type;
o2::Type* o2::ImageAsset::PlatformMeta::type;
o2::Type* o2::ImageAsset::MetaInfo::type;
o2::Type* o2::VectorFontAsset::MetaInfo::type;
o2::Type* o2::AtlasAssetConverter::Image::type;
o2::Type* o2::VectorFont::Effect::type;
o2::Type* o2::UIContextMenu::Item::type;
o2::Type* o2::Curve::Key::type;
o2::Type* o2::AnimatedValue<RectF>::type;
o2::Type* o2::AnimatedValue<RectF>::Key::type;

// Types initializations
void ::Test::InitializeType(::Test* sample)
{
}

void o2::Animatable::InitializeType(o2::Animatable* sample)
{
	FIELD(mStates).AddAttribute<SerializableAttribute<decltype(mStates)>>();
	FIELD(mValues);
	FIELD(mBlend);
}

void o2::AnimatedValue<float>::InitializeType(o2::AnimatedValue<float>* sample)
{
	FIELD(curve).AddAttribute<SerializableAttribute<decltype(curve)>>();
	FIELD(value);
	FIELD(target);
	FIELD(targetDelegate);
	FIELD(targetProperty);
	FIELD(key);
	FIELD(keys);
	FIELD(mValue);
	FIELD(mTarget);
	FIELD(mTargetDelegate);
	FIELD(mTargetProperty);
}

void o2::IAnimatedValue::InitializeType(o2::IAnimatedValue* sample)
{
	FIELD(onKeysChanged);
}

void o2::AnimatedValue<Vec2F>::InitializeType(o2::AnimatedValue<Vec2F>* sample)
{
	FIELD(value);
	FIELD(target);
	FIELD(targetDelegate);
	FIELD(targetProperty);
	FIELD(key);
	FIELD(keys);
	FIELD(mKeys).AddAttribute<SerializableAttribute<decltype(mKeys)>>();
	FIELD(mValue);
	FIELD(mTarget);
	FIELD(mTargetDelegate);
	FIELD(mTargetProperty);
}

void o2::Animation::InitializeType(o2::Animation* sample)
{
	FIELD(mAnimatedValues).AddAttribute<SerializableAttribute<decltype(mAnimatedValues)>>();
	FIELD(mTarget);
	FIELD(mAnimationState);
}

void o2::AnimationMask::InitializeType(o2::AnimationMask* sample)
{
	FIELD(weights).AddAttribute<SerializableAttribute<decltype(weights)>>();
}

void o2::AnimationState::InitializeType(o2::AnimationState* sample)
{
	FIELD(name).AddAttribute<SerializableAttribute<decltype(name)>>();
	FIELD(animation).AddAttribute<SerializableAttribute<decltype(animation)>>();
	FIELD(mask).AddAttribute<SerializableAttribute<decltype(mask)>>();
	FIELD(weight).AddAttribute<SerializableAttribute<decltype(weight)>>();
	FIELD(workWeight);
	FIELD(mOwner);
}

void o2::IAnimation::InitializeType(o2::IAnimation* sample)
{
	FIELD(playing);
	FIELD(reversed);
	FIELD(speed);
	FIELD(time);
	FIELD(relTime);
	FIELD(beginBound);
	FIELD(endBound);
	FIELD(loop);
	FIELD(duration);
	FIELD(onPlayEvent);
	FIELD(onStopEvent);
	FIELD(onPlayedEvent);
	FIELD(onUpdate);
	FIELD(mTime);
	FIELD(mInDurationTime);
	FIELD(mDuration).AddAttribute<SerializableAttribute<decltype(mDuration)>>();
	FIELD(mBeginTime);
	FIELD(mEndTime);
	FIELD(mDirection);
	FIELD(mSpeed);
	FIELD(mLoop).AddAttribute<SerializableAttribute<decltype(mLoop)>>();
	FIELD(mPlaying);
	FIELD(mTimeEvents);
}

void o2::Asset::InitializeType(o2::Asset* sample)
{
	FIELD(path);
	FIELD(fullPath);
	FIELD(id);
	FIELD(meta);
	FIELD(mPath);
	FIELD(mMeta);
}

void o2::AssetInfo::InitializeType(o2::AssetInfo* sample)
{
	FIELD(mType).AddAttribute<SerializableAttribute<decltype(mType)>>();
	FIELD(mPath).AddAttribute<SerializableAttribute<decltype(mPath)>>();
	FIELD(mId).AddAttribute<SerializableAttribute<decltype(mId)>>();
}

void o2::AtlasAsset::InitializeType(o2::AtlasAsset* sample)
{
	FIELD(meta);
	FIELD(imagesInfos);
	FIELD(images);
	FIELD(pages);
	FIELD(mImagesAssetsInfos).AddAttribute<SerializableAttribute<decltype(mImagesAssetsInfos)>>();
	FIELD(mPages).AddAttribute<SerializableAttribute<decltype(mPages)>>();
}

void o2::BinaryAsset::InitializeType(o2::BinaryAsset* sample)
{
	FIELD(data);
	FIELD(dataSize);
	FIELD(meta);
	FIELD(mData);
	FIELD(mDataSize);
}

void o2::BitmapFontAsset::InitializeType(o2::BitmapFontAsset* sample)
{
	FIELD(meta);
	FIELD(font);
	FIELD(mFont);
}

void o2::FolderAsset::InitializeType(o2::FolderAsset* sample)
{
	FIELD(meta);
	FIELD(insideAssets);
	FIELD(mContainingAssetsInfos);
}

void o2::ImageAsset::InitializeType(o2::ImageAsset* sample)
{
	FIELD(bitmap);
	FIELD(atlasId);
	FIELD(atlas);
	FIELD(atlasPage);
	FIELD(atlasRect);
	FIELD(meta);
	FIELD(mBitmap);
	FIELD(mAtlasPage).AddAttribute<SerializableAttribute<decltype(mAtlasPage)>>();
	FIELD(mAtlasRect).AddAttribute<SerializableAttribute<decltype(mAtlasRect)>>();
}

void o2::VectorFontAsset::InitializeType(o2::VectorFontAsset* sample)
{
	FIELD(meta);
	FIELD(font);
	FIELD(mFont);
}

void o2::AtlasAssetConverter::InitializeType(o2::AtlasAssetConverter* sample)
{
}

void o2::FolderAssetConverter::InitializeType(o2::FolderAssetConverter* sample)
{
	FIELD(mRemovedFolders);
}

void o2::IAssetConverter::InitializeType(o2::IAssetConverter* sample)
{
	FIELD(mAssetsBuilder);
}

void o2::ImageAssetConverter::InitializeType(o2::ImageAssetConverter* sample)
{
}

void o2::StdAssetConverter::InitializeType(o2::StdAssetConverter* sample)
{
}

void o2::ProjectConfig::InitializeType(o2::ProjectConfig* sample)
{
	FIELD(projectName);
	FIELD(currentPlatform);
	FIELD(mProjectName).AddAttribute<SerializableAttribute<decltype(mProjectName)>>();
	FIELD(mPlatform);
}

void o2::Camera::InitializeType(o2::Camera* sample)
{
}

void o2::IRectDrawable::InitializeType(o2::IRectDrawable* sample)
{
	FIELD(color);
	FIELD(transparency);
	FIELD(enabled);
	FIELD(mColor).AddAttribute<SerializableAttribute<decltype(mColor)>>();
	FIELD(mEnabled).AddAttribute<SerializableAttribute<decltype(mEnabled)>>();
}

void o2::Sprite::InitializeType(o2::Sprite* sample)
{
	FIELD(texture);
	FIELD(textureSrcRect);
	FIELD(imageAssetId);
	FIELD(imageAssetPath);
	FIELD(imageAsset);
	FIELD(bitmap);
	FIELD(leftTopColor);
	FIELD(rightTopColor);
	FIELD(leftBottomColor);
	FIELD(rightBottomColor);
	FIELD(mode);
	FIELD(fill);
	FIELD(sliceBorder);
	FIELD(mTextureSrcRect);
	FIELD(mCornersColors);
	FIELD(mImageAssetId);
	FIELD(mMode).AddAttribute<SerializableAttribute<decltype(mMode)>>();
	FIELD(mSlices).AddAttribute<SerializableAttribute<decltype(mSlices)>>();
	FIELD(mFill).AddAttribute<SerializableAttribute<decltype(mFill)>>();
	FIELD(mMesh);
	FIELD(mMeshBuildFunc);
}

void o2::Text::InitializeType(o2::Text* sample)
{
	FIELD(font);
	FIELD(text);
	FIELD(ctext);
	FIELD(verAlign);
	FIELD(horAlign);
	FIELD(wordWrap);
	FIELD(dotsEngings);
	FIELD(symbolsDistanceCoef);
	FIELD(linesDistanceCoef);
	FIELD(mMeshMaxPolyCount);
	FIELD(mText).AddAttribute<SerializableAttribute<decltype(mText)>>();
	FIELD(mFontAssetId).AddAttribute<SerializableAttribute<decltype(mFontAssetId)>>();
	FIELD(mFont);
	FIELD(mSymbolsDistCoef).AddAttribute<SerializableAttribute<decltype(mSymbolsDistCoef)>>();
	FIELD(mLinesDistanceCoef).AddAttribute<SerializableAttribute<decltype(mLinesDistanceCoef)>>();
	FIELD(mVerAlign).AddAttribute<SerializableAttribute<decltype(mVerAlign)>>();
	FIELD(mHorAlign).AddAttribute<SerializableAttribute<decltype(mHorAlign)>>();
	FIELD(mWordWrap).AddAttribute<SerializableAttribute<decltype(mWordWrap)>>();
	FIELD(mDotsEndings).AddAttribute<SerializableAttribute<decltype(mDotsEndings)>>();
	FIELD(mMeshes);
	FIELD(mLastTransform);
	FIELD(mSymbolsSet);
	FIELD(mUpdatingMesh);
}

void o2::FontStrokeEffect::InitializeType(o2::FontStrokeEffect* sample)
{
	FIELD(radius).AddAttribute<SerializableAttribute<decltype(radius)>>();
	FIELD(alphaThreshold).AddAttribute<SerializableAttribute<decltype(alphaThreshold)>>();
	FIELD(color).AddAttribute<SerializableAttribute<decltype(color)>>();
}

void o2::FontGradientEffect::InitializeType(o2::FontGradientEffect* sample)
{
	FIELD(color1).AddAttribute<SerializableAttribute<decltype(color1)>>();
	FIELD(color2).AddAttribute<SerializableAttribute<decltype(color2)>>();
	FIELD(angle).AddAttribute<SerializableAttribute<decltype(angle)>>();
	FIELD(length).AddAttribute<SerializableAttribute<decltype(length)>>();
	FIELD(origin).AddAttribute<SerializableAttribute<decltype(origin)>>();
}

void o2::FontColorEffect::InitializeType(o2::FontColorEffect* sample)
{
	FIELD(color).AddAttribute<SerializableAttribute<decltype(color)>>();
}

void o2::FontShadowEffect::InitializeType(o2::FontShadowEffect* sample)
{
	FIELD(blurRadius).AddAttribute<SerializableAttribute<decltype(blurRadius)>>();
	FIELD(offset).AddAttribute<SerializableAttribute<decltype(offset)>>();
	FIELD(color).AddAttribute<SerializableAttribute<decltype(color)>>();
}

void o2::Actor::InitializeType(o2::Actor* sample)
{
	FIELD(name);
	FIELD(enabled);
	FIELD(enabledInHierarchy);
	FIELD(parent);
	FIELD(childs);
	FIELD(child);
	FIELD(components);
	FIELD(component);
	FIELD(transform).AddAttribute<SerializableAttribute<decltype(transform)>>();
	FIELD(mName).AddAttribute<SerializableAttribute<decltype(mName)>>();
	FIELD(mParent);
	FIELD(mChilds).AddAttribute<SerializableAttribute<decltype(mChilds)>>();
	FIELD(mCompontents).AddAttribute<SerializableAttribute<decltype(mCompontents)>>();
	FIELD(mEnabled).AddAttribute<SerializableAttribute<decltype(mEnabled)>>();
	FIELD(mResEnabled);
}

void o2::ActorTransform::InitializeType(o2::ActorTransform* sample)
{
	FIELD(actor);
	FIELD(worldPosition);
	FIELD(worldRect);
	FIELD(worldAngle);
	FIELD(worldBasis);
	FIELD(worldAABB);
	FIELD(mWorldTransform);
	FIELD(mParentInvertedTransform);
	FIELD(mParentTransform);
	FIELD(mIsParentInvTransformActual);
	FIELD(mOwner);
}

void o2::Component::InitializeType(o2::Component* sample)
{
	FIELD(actor);
	FIELD(enabled);
	FIELD(enabledInHierarchy);
	FIELD(mOwner);
	FIELD(mEnabled).AddAttribute<SerializableAttribute<decltype(mEnabled)>>();
	FIELD(mResEnabled);
}

void o2::DrawableComponent::InitializeType(o2::DrawableComponent* sample)
{
	FIELD(drawDepth);
	FIELD(mDrawingDepth);
}

void o2::ImageComponent::InitializeType(o2::ImageComponent* sample)
{
	FIELD(texture);
	FIELD(textureSrcRect);
	FIELD(imageAssetId);
	FIELD(imageAssetPath);
	FIELD(imageAsset);
	FIELD(bitmap);
	FIELD(leftTopColor);
	FIELD(rightTopColor);
	FIELD(leftBottomColor);
	FIELD(rightBottomColor);
	FIELD(mode);
	FIELD(fill);
	FIELD(sliceBorder);
	FIELD(mSprite).AddAttribute<SerializableAttribute<decltype(mSprite)>>();
}

void o2::UIButton::InitializeType(o2::UIButton* sample)
{
	FIELD(caption);
	FIELD(icon);
	FIELD(onClick);
	FIELD(mCaptionText);
	FIELD(mIconSprite);
}

void o2::UIContextMenu::InitializeType(o2::UIContextMenu* sample)
{
	FIELD(mOpenSubMenuDelay);
	FIELD(mParentContextMenu);
	FIELD(mChildContextMenu);
	FIELD(mLayout);
	FIELD(mClickFunctions);
	FIELD(mItemSample);
	FIELD(mSelectionDrawable).AddAttribute<SerializableAttribute<decltype(mSelectionDrawable)>>();
	FIELD(mSelectionLayout).AddAttribute<SerializableAttribute<decltype(mSelectionLayout)>>();
	FIELD(mCurrentSelectionRect);
	FIELD(mTargetSelectionRect);
	FIELD(mLastSelectCheckCursor);
	FIELD(mSelectedItem);
	FIELD(mPressedItem);
	FIELD(mSelectSubContextTime);
}

void o2::UICustomDropDown::InitializeType(o2::UICustomDropDown* sample)
{
	FIELD(selectedItem);
	FIELD(selectedItemPos);
	FIELD(item);
	FIELD(itemsCount);
	FIELD(onSelectedPos);
	FIELD(onSelectedItem);
	FIELD(mList).AddAttribute<SerializableAttribute<decltype(mList)>>();
	FIELD(mClipLayout);
	FIELD(mAbsoluteClip);
	FIELD(mMaxListItems);
}

void o2::UICustomList::InitializeType(o2::UICustomList* sample)
{
	FIELD(selectedItem);
	FIELD(selectedItemPos);
	FIELD(item);
	FIELD(itemsCount);
	FIELD(onSelectedPos);
	FIELD(onSelectedItem);
	FIELD(mVerLayout);
	FIELD(mItemSample).AddAttribute<SerializableAttribute<decltype(mItemSample)>>();
	FIELD(mSelectionDrawable).AddAttribute<SerializableAttribute<decltype(mSelectionDrawable)>>();
	FIELD(mHoverDrawable).AddAttribute<SerializableAttribute<decltype(mHoverDrawable)>>();
	FIELD(mSelectionLayout).AddAttribute<SerializableAttribute<decltype(mSelectionLayout)>>();
	FIELD(mHoverLayout).AddAttribute<SerializableAttribute<decltype(mHoverLayout)>>();
	FIELD(mSelectedItem);
	FIELD(mCurrentSelectionRect);
	FIELD(mTargetSelectionRect);
	FIELD(mCurrentHoverRect);
	FIELD(mTargetHoverRect);
	FIELD(mLastHoverCheckCursor);
	FIELD(mLastSelectCheckCursor);
}

void o2::UIDropDown::InitializeType(o2::UIDropDown* sample)
{
	FIELD(value);
	FIELD(textItem);
	FIELD(onSelectedText);
}

void o2::UIEditBox::InitializeType(o2::UIEditBox* sample)
{
	FIELD(text);
	FIELD(caret);
	FIELD(selectionBegin);
	FIELD(selectionEnd);
	FIELD(onChanged);
	FIELD(mText).AddAttribute<SerializableAttribute<decltype(mText)>>();
	FIELD(mAvailableSymbols).AddAttribute<SerializableAttribute<decltype(mAvailableSymbols)>>();
	FIELD(mTextDrawable).AddAttribute<SerializableAttribute<decltype(mTextDrawable)>>();
	FIELD(mSelectionMesh);
	FIELD(mCaretDrawable).AddAttribute<SerializableAttribute<decltype(mCaretDrawable)>>();
	FIELD(mCaretBlinkDelay).AddAttribute<SerializableAttribute<decltype(mCaretBlinkDelay)>>();
	FIELD(mCaretBlinkTime);
	FIELD(mSelectionBegin);
	FIELD(mSelectionEnd);
	FIELD(mSelectionColor).AddAttribute<SerializableAttribute<decltype(mSelectionColor)>>();
	FIELD(mSelectingByWords);
	FIELD(mSelWordBegin);
	FIELD(mSelWordEnd);
	FIELD(mMultiLine).AddAttribute<SerializableAttribute<decltype(mMultiLine)>>();
	FIELD(mWordWrap).AddAttribute<SerializableAttribute<decltype(mWordWrap)>>();
	FIELD(mMaxLineChars).AddAttribute<SerializableAttribute<decltype(mMaxLineChars)>>();
	FIELD(mMaxLinesCount).AddAttribute<SerializableAttribute<decltype(mMaxLinesCount)>>();
	FIELD(mDrawDepth);
	FIELD(mLastClickTime);
	FIELD(mLastCursorPos);
}

void o2::UIHorizontalLayout::InitializeType(o2::UIHorizontalLayout* sample)
{
	FIELD(baseCorner);
	FIELD(spacing);
	FIELD(border);
	FIELD(borderLeft);
	FIELD(borderRight);
	FIELD(borderTop);
	FIELD(borderBottom);
	FIELD(expandWidth);
	FIELD(expandHeight);
	FIELD(fitByChildren);
	FIELD(mBaseCorner);
	FIELD(mSpacing);
	FIELD(mBorder);
	FIELD(mExpandWidth);
	FIELD(mExpandHeight);
	FIELD(mFitByChildren);
}

void o2::UIHorizontalProgress::InitializeType(o2::UIHorizontalProgress* sample)
{
	FIELD(value);
	FIELD(minValue);
	FIELD(maxValue);
	FIELD(scrollSense);
	FIELD(onChange);
	FIELD(mValue).AddAttribute<SerializableAttribute<decltype(mValue)>>();
	FIELD(mSmoothValue);
	FIELD(mMinValue).AddAttribute<SerializableAttribute<decltype(mMinValue)>>();
	FIELD(mMaxValue).AddAttribute<SerializableAttribute<decltype(mMaxValue)>>();
	FIELD(mScrollSense).AddAttribute<SerializableAttribute<decltype(mScrollSense)>>();
	FIELD(mOrientation).AddAttribute<SerializableAttribute<decltype(mOrientation)>>();
	FIELD(mBarLayer);
	FIELD(mBackLayer);
}

void o2::UIHorizontalScrollBar::InitializeType(o2::UIHorizontalScrollBar* sample)
{
	FIELD(value);
	FIELD(minValue);
	FIELD(maxValue);
	FIELD(scrollSense);
	FIELD(scrollSize);
	FIELD(onChange);
	FIELD(onSmoothChange);
	FIELD(mValue).AddAttribute<SerializableAttribute<decltype(mValue)>>();
	FIELD(mSmoothValue);
	FIELD(mMinValue).AddAttribute<SerializableAttribute<decltype(mMinValue)>>();
	FIELD(mMaxValue).AddAttribute<SerializableAttribute<decltype(mMaxValue)>>();
	FIELD(mScrollSense).AddAttribute<SerializableAttribute<decltype(mScrollSense)>>();
	FIELD(mScrollHandleSize).AddAttribute<SerializableAttribute<decltype(mScrollHandleSize)>>();
	FIELD(mScrollhandleMinPxSize);
	FIELD(mPressHandleOffset);
	FIELD(mHandlePressed);
	FIELD(mHandleLayer);
	FIELD(mBackLayer);
}

void o2::UILabel::InitializeType(o2::UILabel* sample)
{
	FIELD(font);
	FIELD(text);
	FIELD(verAlign);
	FIELD(horAlign);
	FIELD(horOverflow);
	FIELD(verOverflow);
	FIELD(symbolsDistanceCoef);
	FIELD(linesDistanceCoef);
	FIELD(mTextLayer);
	FIELD(mHorOverflow);
	FIELD(mVerOverflow);
}

void o2::UIList::InitializeType(o2::UIList* sample)
{
	FIELD(value);
	FIELD(textItem);
	FIELD(onSelectedText);
}

void o2::UIScrollArea::InitializeType(o2::UIScrollArea* sample)
{
	FIELD(scroll);
	FIELD(horScroll);
	FIELD(verScroll);
	FIELD(onScrolled);
	FIELD(mHorScrollBar);
	FIELD(mVerScrollBar);
	FIELD(mOwnHorScrollBar);
	FIELD(mOwnVerScrollBar);
	FIELD(mViewAreaLayout).AddAttribute<SerializableAttribute<decltype(mViewAreaLayout)>>();
	FIELD(mAbsoluteViewArea);
	FIELD(mClipAreaLayout).AddAttribute<SerializableAttribute<decltype(mClipAreaLayout)>>();
	FIELD(mAbsoluteClipArea);
	FIELD(mScrollPos).AddAttribute<SerializableAttribute<decltype(mScrollPos)>>();
	FIELD(mScrollSpeed);
	FIELD(mScrollSpeedDamp);
	FIELD(mScrollArea);
	FIELD(mScrollRange);
	FIELD(mEnableHorScroll);
}

void o2::UIToggle::InitializeType(o2::UIToggle* sample)
{
	FIELD(caption);
	FIELD(value);
	FIELD(onClick);
	FIELD(onToggle);
	FIELD(mValue);
	FIELD(mCaptionText);
	FIELD(mBackLayer);
}

void o2::UIVerticalLayout::InitializeType(o2::UIVerticalLayout* sample)
{
	FIELD(baseCorner);
	FIELD(spacing);
	FIELD(border);
	FIELD(borderLeft);
	FIELD(borderRight);
	FIELD(borderTop);
	FIELD(borderBottom);
	FIELD(expandWidth);
	FIELD(expandHeight);
	FIELD(fitByChildren);
	FIELD(mBaseCorner);
	FIELD(mSpacing);
	FIELD(mBorder);
	FIELD(mExpandWidth);
	FIELD(mExpandHeight);
	FIELD(mFitByChildren);
}

void o2::UIVerticalProgress::InitializeType(o2::UIVerticalProgress* sample)
{
	FIELD(value);
	FIELD(minValue);
	FIELD(maxValue);
	FIELD(scrollSense);
	FIELD(onChange);
	FIELD(mValue).AddAttribute<SerializableAttribute<decltype(mValue)>>();
	FIELD(mSmoothValue);
	FIELD(mMinValue).AddAttribute<SerializableAttribute<decltype(mMinValue)>>();
	FIELD(mMaxValue).AddAttribute<SerializableAttribute<decltype(mMaxValue)>>();
	FIELD(mScrollSense).AddAttribute<SerializableAttribute<decltype(mScrollSense)>>();
	FIELD(mOrientation).AddAttribute<SerializableAttribute<decltype(mOrientation)>>();
	FIELD(mBarLayer);
	FIELD(mBackLayer);
}

void o2::UIVerticalScrollBar::InitializeType(o2::UIVerticalScrollBar* sample)
{
	FIELD(value);
	FIELD(minValue);
	FIELD(maxValue);
	FIELD(scrollSense);
	FIELD(scrollSize);
	FIELD(onChange);
	FIELD(onSmoothChange);
	FIELD(mValue).AddAttribute<SerializableAttribute<decltype(mValue)>>();
	FIELD(mSmoothValue);
	FIELD(mMinValue).AddAttribute<SerializableAttribute<decltype(mMinValue)>>();
	FIELD(mMaxValue).AddAttribute<SerializableAttribute<decltype(mMaxValue)>>();
	FIELD(mScrollSense).AddAttribute<SerializableAttribute<decltype(mScrollSense)>>();
	FIELD(mScrollHandleSize).AddAttribute<SerializableAttribute<decltype(mScrollHandleSize)>>();
	FIELD(mScrollhandleMinPxSize);
	FIELD(mPressHandleOffset);
	FIELD(mHandlePressed);
	FIELD(mHandleLayer);
	FIELD(mBackLayer);
}

void o2::UIWidget::InitializeType(o2::UIWidget* sample)
{
	FIELD(name);
	FIELD(parent);
	FIELD(childs);
	FIELD(layers);
	FIELD(states);
	FIELD(transparency);
	FIELD(resTransparency);
	FIELD(visible);
	FIELD(child);
	FIELD(layer);
	FIELD(state);
	FIELD(layout).AddAttribute<SerializableAttribute<decltype(layout)>>();
	FIELD(mName).AddAttribute<SerializableAttribute<decltype(mName)>>();
	FIELD(mLayers).AddAttribute<SerializableAttribute<decltype(mLayers)>>();
	FIELD(mStates).AddAttribute<SerializableAttribute<decltype(mStates)>>();
	FIELD(mParent);
	FIELD(mChilds).AddAttribute<SerializableAttribute<decltype(mChilds)>>();
	FIELD(mChildsAbsRect);
	FIELD(mTransparency).AddAttribute<SerializableAttribute<decltype(mTransparency)>>();
	FIELD(mResTransparency);
	FIELD(mDrawingLayers);
	FIELD(mSelectedState);
	FIELD(mIsSelected);
	FIELD(mVisibleState);
	FIELD(mVisible);
	FIELD(mResVisible);
	FIELD(mFullyDisabled);
}

void o2::UIWidgetLayer::InitializeType(o2::UIWidgetLayer* sample)
{
	FIELD(layout).AddAttribute<SerializableAttribute<decltype(layout)>>();
	FIELD(interactableLayout);
	FIELD(name).AddAttribute<SerializableAttribute<decltype(name)>>();
	FIELD(depth);
	FIELD(transparency);
	FIELD(drawable).AddAttribute<SerializableAttribute<decltype(drawable)>>();
	FIELD(child);
	FIELD(mTransparency).AddAttribute<SerializableAttribute<decltype(mTransparency)>>();
	FIELD(mResTransparency);
	FIELD(mDepth).AddAttribute<SerializableAttribute<decltype(mDepth)>>();
	FIELD(mAbsolutePosition);
	FIELD(mInteractableArea);
	FIELD(mOwnerWidget);
	FIELD(mParent);
	FIELD(mChilds).AddAttribute<SerializableAttribute<decltype(mChilds)>>();
}

void o2::UIWidgetLayout::InitializeType(o2::UIWidgetLayout* sample)
{
	FIELD(pivot);
	FIELD(position);
	FIELD(size);
	FIELD(width);
	FIELD(height);
	FIELD(absPosition);
	FIELD(absLeftTop);
	FIELD(absLeftBottom);
	FIELD(absRightTop);
	FIELD(absRightBottom);
	FIELD(absRect);
	FIELD(absLeft);
	FIELD(absRight);
	FIELD(absBottom);
	FIELD(absTop);
	FIELD(pivotX);
	FIELD(pivotY);
	FIELD(anchorMin);
	FIELD(anchorMax);
	FIELD(offsetMin);
	FIELD(offsetMax);
	FIELD(anchorLeft);
	FIELD(anchorRight);
	FIELD(anchorBottom);
	FIELD(anchorTop);
	FIELD(offsetLeft);
	FIELD(offsetRight);
	FIELD(offsetBottom);
	FIELD(offsetTop);
	FIELD(minSize);
	FIELD(minWidth);
	FIELD(minHeight);
	FIELD(maxSize);
	FIELD(maxWidth);
	FIELD(maxHeight);
	FIELD(weight);
	FIELD(widthWeight);
	FIELD(heigthWeight);
	FIELD(mOwner);
	FIELD(mPivot).AddAttribute<SerializableAttribute<decltype(mPivot)>>();
	FIELD(mAnchorMin).AddAttribute<SerializableAttribute<decltype(mAnchorMin)>>();
	FIELD(mAnchorMax).AddAttribute<SerializableAttribute<decltype(mAnchorMax)>>();
	FIELD(mOffsetMin).AddAttribute<SerializableAttribute<decltype(mOffsetMin)>>();
	FIELD(mOffsetMax).AddAttribute<SerializableAttribute<decltype(mOffsetMax)>>();
	FIELD(mMinSize).AddAttribute<SerializableAttribute<decltype(mMinSize)>>();
	FIELD(mMaxSize).AddAttribute<SerializableAttribute<decltype(mMaxSize)>>();
	FIELD(mAbsoluteRect).AddAttribute<SerializableAttribute<decltype(mAbsoluteRect)>>();
	FIELD(mLocalRect);
	FIELD(mWeight).AddAttribute<SerializableAttribute<decltype(mWeight)>>();
	FIELD(mDrivenByParent).AddAttribute<SerializableAttribute<decltype(mDrivenByParent)>>();
}

void o2::UIWidgetState::InitializeType(o2::UIWidgetState* sample)
{
	FIELD(name).AddAttribute<SerializableAttribute<decltype(name)>>();
	FIELD(animation).AddAttribute<SerializableAttribute<decltype(animation)>>();
	FIELD(offStateAnimationSpeed).AddAttribute<SerializableAttribute<decltype(offStateAnimationSpeed)>>();
	FIELD(onStateFullyTrue);
	FIELD(onStateFullyFalse);
	FIELD(onStateBecomesTrue);
	FIELD(onStateBecomesFalse);
	FIELD(mState).AddAttribute<SerializableAttribute<decltype(mState)>>();
	FIELD(mOwner);
}

void o2::UIWindow::InitializeType(o2::UIWindow* sample)
{
	FIELD(caption);
	FIELD(icon);
	FIELD(mIconDrawable);
	FIELD(mCaptionDrawable);
	FIELD(mWindowElements).AddAttribute<SerializableAttribute<decltype(mWindowElements)>>();
	FIELD(mDrawingDepth);
	FIELD(mHeadDragHandle);
	FIELD(mHeadDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mHeadDragAreaLayout)>>();
	FIELD(mHeadDragAreaRect);
	FIELD(mTopDragHandle);
	FIELD(mTopDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mTopDragAreaLayout)>>();
	FIELD(mTopDragAreaRect);
	FIELD(mBottomDragHandle);
	FIELD(mBottomDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mBottomDragAreaLayout)>>();
	FIELD(mBottomDragAreaRect);
	FIELD(mLeftDragHandle);
	FIELD(mLeftDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mLeftDragAreaLayout)>>();
	FIELD(mLeftDragAreaRect);
	FIELD(mRightDragHandle);
	FIELD(mRightDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mRightDragAreaLayout)>>();
	FIELD(mRightDragAreaRect);
	FIELD(mLeftTopDragHandle);
	FIELD(mLeftTopDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mLeftTopDragAreaLayout)>>();
	FIELD(mLeftTopDragAreaRect);
	FIELD(mRightTopDragHandle);
	FIELD(mRightTopDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mRightTopDragAreaLayout)>>();
	FIELD(mRightTopDragAreaRect);
	FIELD(mLeftBottomDragHandle);
	FIELD(mLeftBottomDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mLeftBottomDragAreaLayout)>>();
	FIELD(mLeftBottomDragAreaRect);
	FIELD(mRightBottomDragHandle);
	FIELD(mRightBottomDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mRightBottomDragAreaLayout)>>();
	FIELD(mRightBottomDragAreaRect);
}

void o2::ISerializable::InitializeType(o2::ISerializable* sample)
{
}

void o2::TimeStamp::InitializeType(o2::TimeStamp* sample)
{
	FIELD(mYear).AddAttribute<SerializableAttribute<decltype(mYear)>>();
	FIELD(mMonth).AddAttribute<SerializableAttribute<decltype(mMonth)>>();
	FIELD(mDay).AddAttribute<SerializableAttribute<decltype(mDay)>>();
	FIELD(mHour).AddAttribute<SerializableAttribute<decltype(mHour)>>();
	FIELD(mMinute).AddAttribute<SerializableAttribute<decltype(mMinute)>>();
	FIELD(mSecond).AddAttribute<SerializableAttribute<decltype(mSecond)>>();
}

void o2::Curve::InitializeType(o2::Curve* sample)
{
	FIELD(value);
	FIELD(key);
	FIELD(keys);
	FIELD(length);
	FIELD(onKeysChanged);
	FIELD(mKeys).AddAttribute<SerializableAttribute<decltype(mKeys)>>();
}

void o2::Layout::InitializeType(o2::Layout* sample)
{
	FIELD(anchorMin).AddAttribute<SerializableAttribute<decltype(anchorMin)>>();
	FIELD(anchorMax).AddAttribute<SerializableAttribute<decltype(anchorMax)>>();
	FIELD(offsetMin).AddAttribute<SerializableAttribute<decltype(offsetMin)>>();
	FIELD(offsetMax).AddAttribute<SerializableAttribute<decltype(offsetMax)>>();
}

void o2::Transform::InitializeType(o2::Transform* sample)
{
	FIELD(position);
	FIELD(size);
	FIELD(scale);
	FIELD(pivot);
	FIELD(worldPivot);
	FIELD(szPivot);
	FIELD(rect);
	FIELD(angle);
	FIELD(shear);
	FIELD(basis);
	FIELD(AABB);
	FIELD(leftTop);
	FIELD(leftBottom);
	FIELD(rightTop);
	FIELD(rightBottom);
	FIELD(right);
	FIELD(left);
	FIELD(up);
	FIELD(down);
	FIELD(lookAtPoint);
	FIELD(mPosition).AddAttribute<SerializableAttribute<decltype(mPosition)>>();
	FIELD(mSize).AddAttribute<SerializableAttribute<decltype(mSize)>>();
	FIELD(mScale).AddAttribute<SerializableAttribute<decltype(mScale)>>();
	FIELD(mPivot).AddAttribute<SerializableAttribute<decltype(mPivot)>>();
	FIELD(mAngle).AddAttribute<SerializableAttribute<decltype(mAngle)>>();
	FIELD(mShear).AddAttribute<SerializableAttribute<decltype(mShear)>>();
	FIELD(mTransform);
}

void o2::AnimatedValue<Vec2F>::Key::InitializeType(o2::AnimatedValue<Vec2F>::Key* sample)
{
	FIELD(position).AddAttribute<SerializableAttribute<decltype(position)>>();
	FIELD(value).AddAttribute<SerializableAttribute<decltype(value)>>();
	FIELD(prevSupportValue).AddAttribute<SerializableAttribute<decltype(prevSupportValue)>>();
	FIELD(nextSupportValue).AddAttribute<SerializableAttribute<decltype(nextSupportValue)>>();
	FIELD(curvePrevCoef).AddAttribute<SerializableAttribute<decltype(curvePrevCoef)>>();
	FIELD(curvePrevCoefPos).AddAttribute<SerializableAttribute<decltype(curvePrevCoefPos)>>();
	FIELD(curveNextCoef).AddAttribute<SerializableAttribute<decltype(curveNextCoef)>>();
	FIELD(curveNextCoefPos).AddAttribute<SerializableAttribute<decltype(curveNextCoefPos)>>();
	FIELD(mApproxValues);
	FIELD(mCurveApproxValues);
	FIELD(mApproxLengths);
	FIELD(mApproxTotalLength);
}

void o2::Animation::AnimatedValueDef::InitializeType(o2::Animation::AnimatedValueDef* sample)
{
	FIELD(mTargetPath).AddAttribute<SerializableAttribute<decltype(mTargetPath)>>();
	FIELD(mTargetPtr);
	FIELD(mAnimatedValue).AddAttribute<SerializableAttribute<decltype(mAnimatedValue)>>();
}

void o2::Asset::IMetaInfo::InitializeType(o2::Asset::IMetaInfo* sample)
{
	FIELD(mId).AddAttribute<SerializableAttribute<decltype(mId)>>();
}

void o2::AssetTree::AssetNode::InitializeType(o2::AssetTree::AssetNode* sample)
{
	FIELD(mMeta);
	FIELD(mTime);
}

void o2::AtlasAsset::PlatformMeta::InitializeType(o2::AtlasAsset::PlatformMeta* sample)
{
	FIELD(mMaxSize).AddAttribute<SerializableAttribute<decltype(mMaxSize)>>();
	FIELD(mFormat).AddAttribute<SerializableAttribute<decltype(mFormat)>>();
}

void o2::AtlasAsset::MetaInfo::InitializeType(o2::AtlasAsset::MetaInfo* sample)
{
	FIELD(mIOS).AddAttribute<SerializableAttribute<decltype(mIOS)>>();
	FIELD(mAndroid).AddAttribute<SerializableAttribute<decltype(mAndroid)>>();
	FIELD(mMacOS).AddAttribute<SerializableAttribute<decltype(mMacOS)>>();
	FIELD(mWindows).AddAttribute<SerializableAttribute<decltype(mWindows)>>();
	FIELD(mBorder).AddAttribute<SerializableAttribute<decltype(mBorder)>>();
}

void o2::AtlasAsset::Page::InitializeType(o2::AtlasAsset::Page* sample)
{
	FIELD(mId).AddAttribute<SerializableAttribute<decltype(mId)>>();
	FIELD(mSize).AddAttribute<SerializableAttribute<decltype(mSize)>>();
	FIELD(mImagesRects).AddAttribute<SerializableAttribute<decltype(mImagesRects)>>();
}

void o2::BinaryAsset::MetaInfo::InitializeType(o2::BinaryAsset::MetaInfo* sample)
{
}

void o2::BitmapFontAsset::MetaInfo::InitializeType(o2::BitmapFontAsset::MetaInfo* sample)
{
}

void o2::FolderAsset::MetaInfo::InitializeType(o2::FolderAsset::MetaInfo* sample)
{
}

void o2::ImageAsset::PlatformMeta::InitializeType(o2::ImageAsset::PlatformMeta* sample)
{
	FIELD(mMaxSize).AddAttribute<SerializableAttribute<decltype(mMaxSize)>>();
	FIELD(mScale).AddAttribute<SerializableAttribute<decltype(mScale)>>();
	FIELD(mFormat).AddAttribute<SerializableAttribute<decltype(mFormat)>>();
}

void o2::ImageAsset::MetaInfo::InitializeType(o2::ImageAsset::MetaInfo* sample)
{
	FIELD(mAtlasId).AddAttribute<SerializableAttribute<decltype(mAtlasId)>>();
	FIELD(mIOS).AddAttribute<SerializableAttribute<decltype(mIOS)>>();
	FIELD(mAndroid).AddAttribute<SerializableAttribute<decltype(mAndroid)>>();
	FIELD(mMacOS).AddAttribute<SerializableAttribute<decltype(mMacOS)>>();
	FIELD(mWindows).AddAttribute<SerializableAttribute<decltype(mWindows)>>();
	FIELD(mSliceBorder).AddAttribute<SerializableAttribute<decltype(mSliceBorder)>>();
	FIELD(mDefaultMode).AddAttribute<SerializableAttribute<decltype(mDefaultMode)>>();
}

void o2::VectorFontAsset::MetaInfo::InitializeType(o2::VectorFontAsset::MetaInfo* sample)
{
	FIELD(mEffects).AddAttribute<SerializableAttribute<decltype(mEffects)>>();
}

void o2::AtlasAssetConverter::Image::InitializeType(o2::AtlasAssetConverter::Image* sample)
{
	FIELD(mId).AddAttribute<SerializableAttribute<decltype(mId)>>();
	FIELD(mTime).AddAttribute<SerializableAttribute<decltype(mTime)>>();
}

void o2::VectorFont::Effect::InitializeType(o2::VectorFont::Effect* sample)
{
}

void o2::UIContextMenu::Item::InitializeType(o2::UIContextMenu::Item* sample)
{
	FIELD(text).AddAttribute<SerializableAttribute<decltype(text)>>();
	FIELD(shortcut).AddAttribute<SerializableAttribute<decltype(shortcut)>>();
	FIELD(icon).AddAttribute<SerializableAttribute<decltype(icon)>>();
	FIELD(subItems).AddAttribute<SerializableAttribute<decltype(subItems)>>();
	FIELD(onClick);
}

void o2::Curve::Key::InitializeType(o2::Curve::Key* sample)
{
	FIELD(value).AddAttribute<SerializableAttribute<decltype(value)>>();
	FIELD(position).AddAttribute<SerializableAttribute<decltype(position)>>();
	FIELD(leftCoef).AddAttribute<SerializableAttribute<decltype(leftCoef)>>();
	FIELD(leftCoefPosition).AddAttribute<SerializableAttribute<decltype(leftCoefPosition)>>();
	FIELD(rightCoef).AddAttribute<SerializableAttribute<decltype(rightCoef)>>();
	FIELD(rightCoefPosition).AddAttribute<SerializableAttribute<decltype(rightCoefPosition)>>();
	FIELD(mApproxValues);
}

void o2::AnimatedValue<RectF>::InitializeType(o2::AnimatedValue<RectF>* sample)
{
	FIELD(value);
	FIELD(target);
	FIELD(targetDelegate);
	FIELD(targetProperty);
	FIELD(key);
	FIELD(keys);
	FIELD(mKeys).AddAttribute<SerializableAttribute<decltype(mKeys)>>();
	FIELD(mValue);
	FIELD(mTarget);
	FIELD(mTargetDelegate);
	FIELD(mTargetProperty);
}

void o2::AnimatedValue<RectF>::Key::InitializeType(o2::AnimatedValue<RectF>::Key* sample)
{
	FIELD(position).AddAttribute<SerializableAttribute<decltype(position)>>();
	FIELD(value).AddAttribute<SerializableAttribute<decltype(value)>>();
	FIELD(curvePrevCoef).AddAttribute<SerializableAttribute<decltype(curvePrevCoef)>>();
	FIELD(curvePrevCoefPos).AddAttribute<SerializableAttribute<decltype(curvePrevCoefPos)>>();
	FIELD(curveNextCoef).AddAttribute<SerializableAttribute<decltype(curveNextCoef)>>();
	FIELD(curveNextCoefPos).AddAttribute<SerializableAttribute<decltype(curveNextCoefPos)>>();
	FIELD(mCurveApproxValues);
}


// Registering all types
void RegReflectionTypes()
{
	// Create types
	::Test::type = mnew Type();
	o2::Animatable::type = mnew Type();
	o2::AnimatedValue<float>::type = mnew Type();
	o2::IAnimatedValue::type = mnew Type();
	o2::AnimatedValue<Vec2F>::type = mnew Type();
	o2::Animation::type = mnew Type();
	o2::AnimationMask::type = mnew Type();
	o2::AnimationState::type = mnew Type();
	o2::IAnimation::type = mnew Type();
	o2::Asset::type = mnew Type();
	o2::AssetInfo::type = mnew Type();
	o2::AtlasAsset::type = mnew Type();
	o2::BinaryAsset::type = mnew Type();
	o2::BitmapFontAsset::type = mnew Type();
	o2::FolderAsset::type = mnew Type();
	o2::ImageAsset::type = mnew Type();
	o2::VectorFontAsset::type = mnew Type();
	o2::AtlasAssetConverter::type = mnew Type();
	o2::FolderAssetConverter::type = mnew Type();
	o2::IAssetConverter::type = mnew Type();
	o2::ImageAssetConverter::type = mnew Type();
	o2::StdAssetConverter::type = mnew Type();
	o2::ProjectConfig::type = mnew Type();
	o2::Camera::type = mnew Type();
	o2::IRectDrawable::type = mnew Type();
	o2::Sprite::type = mnew Type();
	o2::Text::type = mnew Type();
	o2::FontStrokeEffect::type = mnew Type();
	o2::FontGradientEffect::type = mnew Type();
	o2::FontColorEffect::type = mnew Type();
	o2::FontShadowEffect::type = mnew Type();
	o2::Actor::type = mnew Type();
	o2::ActorTransform::type = mnew Type();
	o2::Component::type = mnew Type();
	o2::DrawableComponent::type = mnew Type();
	o2::ImageComponent::type = mnew Type();
	o2::UIButton::type = mnew Type();
	o2::UIContextMenu::type = mnew Type();
	o2::UICustomDropDown::type = mnew Type();
	o2::UICustomList::type = mnew Type();
	o2::UIDropDown::type = mnew Type();
	o2::UIEditBox::type = mnew Type();
	o2::UIHorizontalLayout::type = mnew Type();
	o2::UIHorizontalProgress::type = mnew Type();
	o2::UIHorizontalScrollBar::type = mnew Type();
	o2::UILabel::type = mnew Type();
	o2::UIList::type = mnew Type();
	o2::UIScrollArea::type = mnew Type();
	o2::UIToggle::type = mnew Type();
	o2::UIVerticalLayout::type = mnew Type();
	o2::UIVerticalProgress::type = mnew Type();
	o2::UIVerticalScrollBar::type = mnew Type();
	o2::UIWidget::type = mnew Type();
	o2::UIWidgetLayer::type = mnew Type();
	o2::UIWidgetLayout::type = mnew Type();
	o2::UIWidgetState::type = mnew Type();
	o2::UIWindow::type = mnew Type();
	o2::ISerializable::type = mnew Type();
	o2::TimeStamp::type = mnew Type();
	o2::Curve::type = mnew Type();
	o2::Layout::type = mnew Type();
	o2::Transform::type = mnew Type();
	o2::AnimatedValue<Vec2F>::Key::type = mnew Type();
	o2::Animation::AnimatedValueDef::type = mnew Type();
	o2::Asset::IMetaInfo::type = mnew Type();
	o2::AssetTree::AssetNode::type = mnew Type();
	o2::AtlasAsset::PlatformMeta::type = mnew Type();
	o2::AtlasAsset::MetaInfo::type = mnew Type();
	o2::AtlasAsset::Page::type = mnew Type();
	o2::BinaryAsset::MetaInfo::type = mnew Type();
	o2::BitmapFontAsset::MetaInfo::type = mnew Type();
	o2::FolderAsset::MetaInfo::type = mnew Type();
	o2::ImageAsset::PlatformMeta::type = mnew Type();
	o2::ImageAsset::MetaInfo::type = mnew Type();
	o2::VectorFontAsset::MetaInfo::type = mnew Type();
	o2::AtlasAssetConverter::Image::type = mnew Type();
	o2::VectorFont::Effect::type = mnew Type();
	o2::UIContextMenu::Item::type = mnew Type();
	o2::Curve::Key::type = mnew Type();
	o2::AnimatedValue<RectF>::type = mnew Type();
	o2::AnimatedValue<RectF>::Key::type = mnew Type();

	// Initialize types
	o2::Reflection::InitializeType<::Test>("::Test");
	o2::Reflection::InitializeType<o2::Animatable>("o2::Animatable");
	o2::Reflection::InitializeType<o2::AnimatedValue<float>>("o2::AnimatedValue<float>");
	o2::Reflection::InitializeType<o2::IAnimatedValue>("o2::IAnimatedValue");
	o2::Reflection::InitializeType<o2::AnimatedValue<Vec2F>>("o2::AnimatedValue<Vec2F>");
	o2::Reflection::InitializeType<o2::Animation>("o2::Animation");
	o2::Reflection::InitializeType<o2::AnimationMask>("o2::AnimationMask");
	o2::Reflection::InitializeType<o2::AnimationState>("o2::AnimationState");
	o2::Reflection::InitializeType<o2::IAnimation>("o2::IAnimation");
	o2::Reflection::InitializeType<o2::Asset>("o2::Asset");
	o2::Reflection::InitializeType<o2::AssetInfo>("o2::AssetInfo");
	o2::Reflection::InitializeType<o2::AtlasAsset>("o2::AtlasAsset");
	o2::Reflection::InitializeType<o2::BinaryAsset>("o2::BinaryAsset");
	o2::Reflection::InitializeType<o2::BitmapFontAsset>("o2::BitmapFontAsset");
	o2::Reflection::InitializeType<o2::FolderAsset>("o2::FolderAsset");
	o2::Reflection::InitializeType<o2::ImageAsset>("o2::ImageAsset");
	o2::Reflection::InitializeType<o2::VectorFontAsset>("o2::VectorFontAsset");
	o2::Reflection::InitializeType<o2::AtlasAssetConverter>("o2::AtlasAssetConverter");
	o2::Reflection::InitializeType<o2::FolderAssetConverter>("o2::FolderAssetConverter");
	o2::Reflection::InitializeType<o2::IAssetConverter>("o2::IAssetConverter");
	o2::Reflection::InitializeType<o2::ImageAssetConverter>("o2::ImageAssetConverter");
	o2::Reflection::InitializeType<o2::StdAssetConverter>("o2::StdAssetConverter");
	o2::Reflection::InitializeType<o2::ProjectConfig>("o2::ProjectConfig");
	o2::Reflection::InitializeType<o2::Camera>("o2::Camera");
	o2::Reflection::InitializeType<o2::IRectDrawable>("o2::IRectDrawable");
	o2::Reflection::InitializeType<o2::Sprite>("o2::Sprite");
	o2::Reflection::InitializeType<o2::Text>("o2::Text");
	o2::Reflection::InitializeType<o2::FontStrokeEffect>("o2::FontStrokeEffect");
	o2::Reflection::InitializeType<o2::FontGradientEffect>("o2::FontGradientEffect");
	o2::Reflection::InitializeType<o2::FontColorEffect>("o2::FontColorEffect");
	o2::Reflection::InitializeType<o2::FontShadowEffect>("o2::FontShadowEffect");
	o2::Reflection::InitializeType<o2::Actor>("o2::Actor");
	o2::Reflection::InitializeType<o2::ActorTransform>("o2::ActorTransform");
	o2::Reflection::InitializeType<o2::Component>("o2::Component");
	o2::Reflection::InitializeType<o2::DrawableComponent>("o2::DrawableComponent");
	o2::Reflection::InitializeType<o2::ImageComponent>("o2::ImageComponent");
	o2::Reflection::InitializeType<o2::UIButton>("o2::UIButton");
	o2::Reflection::InitializeType<o2::UIContextMenu>("o2::UIContextMenu");
	o2::Reflection::InitializeType<o2::UICustomDropDown>("o2::UICustomDropDown");
	o2::Reflection::InitializeType<o2::UICustomList>("o2::UICustomList");
	o2::Reflection::InitializeType<o2::UIDropDown>("o2::UIDropDown");
	o2::Reflection::InitializeType<o2::UIEditBox>("o2::UIEditBox");
	o2::Reflection::InitializeType<o2::UIHorizontalLayout>("o2::UIHorizontalLayout");
	o2::Reflection::InitializeType<o2::UIHorizontalProgress>("o2::UIHorizontalProgress");
	o2::Reflection::InitializeType<o2::UIHorizontalScrollBar>("o2::UIHorizontalScrollBar");
	o2::Reflection::InitializeType<o2::UILabel>("o2::UILabel");
	o2::Reflection::InitializeType<o2::UIList>("o2::UIList");
	o2::Reflection::InitializeType<o2::UIScrollArea>("o2::UIScrollArea");
	o2::Reflection::InitializeType<o2::UIToggle>("o2::UIToggle");
	o2::Reflection::InitializeType<o2::UIVerticalLayout>("o2::UIVerticalLayout");
	o2::Reflection::InitializeType<o2::UIVerticalProgress>("o2::UIVerticalProgress");
	o2::Reflection::InitializeType<o2::UIVerticalScrollBar>("o2::UIVerticalScrollBar");
	o2::Reflection::InitializeType<o2::UIWidget>("o2::UIWidget");
	o2::Reflection::InitializeType<o2::UIWidgetLayer>("o2::UIWidgetLayer");
	o2::Reflection::InitializeType<o2::UIWidgetLayout>("o2::UIWidgetLayout");
	o2::Reflection::InitializeType<o2::UIWidgetState>("o2::UIWidgetState");
	o2::Reflection::InitializeType<o2::UIWindow>("o2::UIWindow");
	o2::Reflection::InitializeType<o2::ISerializable>("o2::ISerializable");
	o2::Reflection::InitializeType<o2::TimeStamp>("o2::TimeStamp");
	o2::Reflection::InitializeType<o2::Curve>("o2::Curve");
	o2::Reflection::InitializeType<o2::Layout>("o2::Layout");
	o2::Reflection::InitializeType<o2::Transform>("o2::Transform");
	o2::Reflection::InitializeType<o2::AnimatedValue<Vec2F>::Key>("o2::AnimatedValue<Vec2F>::Key");
	o2::Reflection::InitializeType<o2::Animation::AnimatedValueDef>("o2::Animation::AnimatedValueDef");
	o2::Reflection::InitializeType<o2::Asset::IMetaInfo>("o2::Asset::IMetaInfo");
	o2::Reflection::InitializeType<o2::AssetTree::AssetNode>("o2::AssetTree::AssetNode");
	o2::Reflection::InitializeType<o2::AtlasAsset::PlatformMeta>("o2::AtlasAsset::PlatformMeta");
	o2::Reflection::InitializeType<o2::AtlasAsset::MetaInfo>("o2::AtlasAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::AtlasAsset::Page>("o2::AtlasAsset::Page");
	o2::Reflection::InitializeType<o2::BinaryAsset::MetaInfo>("o2::BinaryAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::BitmapFontAsset::MetaInfo>("o2::BitmapFontAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::FolderAsset::MetaInfo>("o2::FolderAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::ImageAsset::PlatformMeta>("o2::ImageAsset::PlatformMeta");
	o2::Reflection::InitializeType<o2::ImageAsset::MetaInfo>("o2::ImageAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::VectorFontAsset::MetaInfo>("o2::VectorFontAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::AtlasAssetConverter::Image>("o2::AtlasAssetConverter::Image");
	o2::Reflection::InitializeType<o2::VectorFont::Effect>("o2::VectorFont::Effect");
	o2::Reflection::InitializeType<o2::UIContextMenu::Item>("o2::UIContextMenu::Item");
	o2::Reflection::InitializeType<o2::Curve::Key>("o2::Curve::Key");
	o2::Reflection::InitializeType<o2::AnimatedValue<RectF>>("o2::AnimatedValue<RectF>");
	o2::Reflection::InitializeType<o2::AnimatedValue<RectF>::Key>("o2::AnimatedValue<RectF>::Key");

	// Resolve inheritance
	o2::Animatable::type->AddBaseType(o2::ISerializable::type);
	o2::IAnimation::type->AddBaseType(o2::ISerializable::type);
	o2::IAnimatedValue::type->AddBaseType(o2::IAnimation::type);
	o2::AnimatedValue<float>::type->AddBaseType(o2::IAnimatedValue::type);
	o2::AnimatedValue<Vec2F>::type->AddBaseType(o2::IAnimatedValue::type);
	o2::Animation::type->AddBaseType(o2::IAnimation::type);
	o2::AnimationMask::type->AddBaseType(o2::ISerializable::type);
	o2::AnimationState::type->AddBaseType(o2::ISerializable::type);
	o2::Asset::type->AddBaseType(o2::ISerializable::type);
	o2::AssetInfo::type->AddBaseType(o2::ISerializable::type);
	o2::AtlasAsset::type->AddBaseType(o2::Asset::type);
	o2::BinaryAsset::type->AddBaseType(o2::Asset::type);
	o2::BitmapFontAsset::type->AddBaseType(o2::Asset::type);
	o2::FolderAsset::type->AddBaseType(o2::Asset::type);
	o2::ImageAsset::type->AddBaseType(o2::Asset::type);
	o2::VectorFontAsset::type->AddBaseType(o2::Asset::type);
	o2::AtlasAssetConverter::type->AddBaseType(o2::IAssetConverter::type);
	o2::FolderAssetConverter::type->AddBaseType(o2::IAssetConverter::type);
	o2::ImageAssetConverter::type->AddBaseType(o2::IAssetConverter::type);
	o2::StdAssetConverter::type->AddBaseType(o2::IAssetConverter::type);
	o2::ProjectConfig::type->AddBaseType(o2::ISerializable::type);
	o2::Transform::type->AddBaseType(o2::ISerializable::type);
	o2::Camera::type->AddBaseType(o2::Transform::type);
	o2::IRectDrawable::type->AddBaseType(o2::Transform::type);
	o2::Sprite::type->AddBaseType(o2::IRectDrawable::type);
	o2::Text::type->AddBaseType(o2::IRectDrawable::type);
	o2::VectorFont::Effect::type->AddBaseType(o2::ISerializable::type);
	o2::FontStrokeEffect::type->AddBaseType(o2::VectorFont::Effect::type);
	o2::FontGradientEffect::type->AddBaseType(o2::VectorFont::Effect::type);
	o2::FontColorEffect::type->AddBaseType(o2::VectorFont::Effect::type);
	o2::FontShadowEffect::type->AddBaseType(o2::VectorFont::Effect::type);
	o2::Actor::type->AddBaseType(o2::Animatable::type);
	o2::ActorTransform::type->AddBaseType(o2::Transform::type);
	o2::Component::type->AddBaseType(o2::ISerializable::type);
	o2::DrawableComponent::type->AddBaseType(o2::Component::type);
	o2::ImageComponent::type->AddBaseType(o2::DrawableComponent::type);
	o2::UIWidget::type->AddBaseType(o2::ISerializable::type);
	o2::UIButton::type->AddBaseType(o2::UIWidget::type);
	o2::UIScrollArea::type->AddBaseType(o2::UIWidget::type);
	o2::UIContextMenu::type->AddBaseType(o2::UIScrollArea::type);
	o2::UICustomDropDown::type->AddBaseType(o2::UIWidget::type);
	o2::UICustomList::type->AddBaseType(o2::UIScrollArea::type);
	o2::UIDropDown::type->AddBaseType(o2::UICustomDropDown::type);
	o2::UIEditBox::type->AddBaseType(o2::UIScrollArea::type);
	o2::UIHorizontalLayout::type->AddBaseType(o2::UIWidget::type);
	o2::UIHorizontalProgress::type->AddBaseType(o2::UIWidget::type);
	o2::UIHorizontalScrollBar::type->AddBaseType(o2::UIWidget::type);
	o2::UILabel::type->AddBaseType(o2::UIWidget::type);
	o2::UIList::type->AddBaseType(o2::UICustomList::type);
	o2::UIToggle::type->AddBaseType(o2::UIWidget::type);
	o2::UIVerticalLayout::type->AddBaseType(o2::UIWidget::type);
	o2::UIVerticalProgress::type->AddBaseType(o2::UIWidget::type);
	o2::UIVerticalScrollBar::type->AddBaseType(o2::UIWidget::type);
	o2::UIWidgetLayer::type->AddBaseType(o2::ISerializable::type);
	o2::UIWidgetLayout::type->AddBaseType(o2::ISerializable::type);
	o2::UIWidgetState::type->AddBaseType(o2::ISerializable::type);
	o2::UIWindow::type->AddBaseType(o2::UIScrollArea::type);
	o2::TimeStamp::type->AddBaseType(o2::ISerializable::type);
	o2::Curve::type->AddBaseType(o2::ISerializable::type);
	o2::Layout::type->AddBaseType(o2::ISerializable::type);
	o2::AnimatedValue<Vec2F>::Key::type->AddBaseType(o2::ISerializable::type);
	o2::Animation::AnimatedValueDef::type->AddBaseType(o2::ISerializable::type);
	o2::Asset::IMetaInfo::type->AddBaseType(o2::ISerializable::type);
	o2::AssetTree::AssetNode::type->AddBaseType(o2::AssetInfo::type);
	o2::AtlasAsset::PlatformMeta::type->AddBaseType(o2::ISerializable::type);
	o2::AtlasAsset::MetaInfo::type->AddBaseType(o2::Asset::IMetaInfo::type);
	o2::AtlasAsset::Page::type->AddBaseType(o2::ISerializable::type);
	o2::BinaryAsset::MetaInfo::type->AddBaseType(o2::Asset::IMetaInfo::type);
	o2::BitmapFontAsset::MetaInfo::type->AddBaseType(o2::Asset::IMetaInfo::type);
	o2::FolderAsset::MetaInfo::type->AddBaseType(o2::Asset::IMetaInfo::type);
	o2::ImageAsset::PlatformMeta::type->AddBaseType(o2::ISerializable::type);
	o2::ImageAsset::MetaInfo::type->AddBaseType(o2::Asset::IMetaInfo::type);
	o2::VectorFontAsset::MetaInfo::type->AddBaseType(o2::Asset::IMetaInfo::type);
	o2::AtlasAssetConverter::Image::type->AddBaseType(o2::ISerializable::type);
	o2::UIContextMenu::Item::type->AddBaseType(o2::ISerializable::type);
	o2::Curve::Key::type->AddBaseType(o2::ISerializable::type);
	o2::AnimatedValue<RectF>::type->AddBaseType(o2::IAnimatedValue::type);
	o2::AnimatedValue<RectF>::Key::type->AddBaseType(o2::ISerializable::type);

}