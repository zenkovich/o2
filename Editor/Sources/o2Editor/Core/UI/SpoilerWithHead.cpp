#include "o2Editor/stdafx.h"
#include "SpoilerWithHead.h"
#include "o2/Scene/UI/Widgets/Button.h"

namespace Editor
{

    SpoilerWithHead::SpoilerWithHead() :
        Spoiler()
    {}

    SpoilerWithHead::SpoilerWithHead(const SpoilerWithHead& other) :
        Spoiler(other)
    {
        InitializeControls();
        RetargetStatesAnimations();
    }

    SpoilerWithHead& SpoilerWithHead::operator=(const SpoilerWithHead& other)
    {
        Spoiler::operator=(other);
        InitializeControls();
        return *this;
    }

    Ref<Image> SpoilerWithHead::GetIcon() const
    {
        return mIcon;
    }

    String SpoilerWithHead::GetCreateMenuCategory()
    {
        return "UI/Editor";
    }

    void SpoilerWithHead::InitializeControls()
    {
        mOptionsBtn = FindInternalWidgetByType<Ref<Button>>("options");
        if (mOptionsBtn)
            mOptionsBtn->onClick = [&]() { onOptionsPressed(); };

        mSaveBtn = FindInternalWidgetByType<Ref<Button>>("save");
        if (mSaveBtn)
            mSaveBtn->onClick = [&]() { onSavePressed(); };

        mIcon = FindInternalWidgetByType<Ref<Image>>("icon");
    }
}

// --- META ---

DECLARE_CLASS(Editor::SpoilerWithHead, Editor__SpoilerWithHead);

// --- END META ---