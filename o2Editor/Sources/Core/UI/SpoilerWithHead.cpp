#include "stdafx.h"
#include "SpoilerWithHead.h"

#include "Scene/UI/Button.h"

namespace Editor
{

	UISpoilerWithHead::UISpoilerWithHead() :
		UISpoiler()
	{}

	UISpoilerWithHead::UISpoilerWithHead(const UISpoilerWithHead& other) :
		UISpoiler(other)
	{
		InitializeControls();
		RetargetStatesAnimations();
	}

	UISpoilerWithHead& UISpoilerWithHead::operator=(const UISpoilerWithHead& other)
	{
		UISpoiler::operator=(other);
		return *this;
	}

	UIImage* UISpoilerWithHead::GetIcon() const
	{
		return mIcon;
	}

	void UISpoilerWithHead::InitializeControls()
	{
		mOptionsBtn = FindInternalWidgetByType<UIButton>("options");
		if (mOptionsBtn)
			mOptionsBtn->onClick = [&]() { onOptionsPressed(); };

		mSaveBtn = FindInternalWidgetByType<UIButton>("save");
		if (mSaveBtn)
			mSaveBtn->onClick = [&]() { onSavePressed(); };

		mIcon = FindInternalWidgetByType<UIImage>("icon");
	}

	void UISpoilerWithHead::CopyData(const Actor& otherActor)
	{
		UISpoiler::CopyData(otherActor);
		InitializeControls();

	}
}

DECLARE_CLASS(Editor::UISpoilerWithHead);
