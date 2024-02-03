#include "o2Editor/stdafx.h"
#include "WStringProperty.h"
#include "o2/Scene/UI/Widgets/EditBox.h"

namespace Editor
{
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T>
    using WeakRef = std::weak_ptr<T>;

    template <typename T>
    using DynamicCast = std::dynamic_pointer_cast<T>;

    template <typename T, typename... Args>
    Ref<T> mmake(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    WStringProperty::WStringProperty()
    {}

    WStringProperty::WStringProperty(const WStringProperty& other):
        TPropertyField<WString>(other)
    {
        InitializeControls();
    }

    WStringProperty& WStringProperty::operator=(const WStringProperty& other)
    {
        TPropertyField<WString>::operator=(other);
        InitializeControls();
        return *this;
    }

    void WStringProperty::InitializeControls()
    {
        mEditBox = FindChildByType<EditBox>();
        if (mEditBox)
        {
            mEditBox->onChangeCompleted = THIS_FUNC(OnEdited);
            mEditBox->text = "--";
        }
    }

    void WStringProperty::UpdateValueView()
    {
        if (mValuesDifferent)
            mEditBox->text = "--";
        else
            mEditBox->text = mCommonValue;
    }

    void WStringProperty::OnEdited(const WString& data)
    {
        if (mValuesDifferent && data == "--")
            return;

        SetValueByUser(data);
    }
}
DECLARE_TEMPLATE_CLASS(Editor::TPropertyField<o2::WString>);
// --- META ---

DECLARE_CLASS(Editor::WStringProperty, Editor__WStringProperty);
// --- END META ---