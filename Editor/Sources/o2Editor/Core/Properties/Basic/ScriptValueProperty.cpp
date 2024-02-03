#include "o2Editor/stdafx.h"
#include "ScriptValueProperty.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Utils/Editor/Attributes/NoHeaderAttribute.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/IntegerProperty.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Properties.h"

#include "o2/Utils/SmartPointers/Ref.h"
#include "o2/Utils/SmartPointers/WeakRef.h"

using namespace o2;

namespace Editor
{
    ScriptValueProperty::ScriptValueProperty()
    {
        InitializeControls();
    }

    ScriptValueProperty::ScriptValueProperty(const ScriptValueProperty& other) :
        IPropertyField(other)
    {
        InitializeControls();
    }

    ScriptValueProperty& ScriptValueProperty::operator=(const ScriptValueProperty& other)
    {
        IPropertyField::operator=(other);
        InitializeControls();
        return *this;
    }

    void ScriptValueProperty::OnFreeProperty()
    {
    }

    void ScriptValueProperty::InitializeControls()
    {
        expandHeight = true;
        expandWidth = true;
        fitByChildren = true;

        mSpoiler = o2UI.CreateWidget<Ref<Spoiler>>("expand with caption");
        AddChild(mSpoiler.get());

        if (mSpoiler)
            mSpoiler->onExpand = THIS_FUNC(OnExpand);

        mSpoiler->borderTop = 5;

        mHeaderContainer = mnew HorizontalLayout();
        mHeaderContainer->layout = Ref<WidgetLayout>::make<WidgetLayout::HorStretch>(VerAlign::Top, 100, 0, 20, 0);
        mHeaderContainer->baseCorner = BaseCorner::Right;
        mHeaderContainer->expandHeight = false;
        mHeaderContainer->SetInternalParent(mSpoiler, false);

        // Add button
        mAddButton = mSpoiler->FindChildByTypeAndName<Ref<Button>>("add button");
        if (!mAddButton)
        {
            mAddButton = o2UI.CreateWidget<Ref<Button>>("add small");
            mAddButton->name = "add button";
            mAddButton->layout->maxWidth = 20;
            mAddButton->layout->minHeight = 20;
            mAddButton->onClick = THIS_FUNC(OnAddPressed);

            mHeaderContainer->AddChild(mAddButton);
        }

        // Count property
        mCountProperty = mSpoiler->FindChildByType<Ref<IntegerProperty>>(false);
        if (!mCountProperty)
            mCountProperty = o2UI.CreateWidget<Ref<IntegerProperty>>();

        if (mCountProperty)
        {
            mHeaderContainer->AddChild(mCountProperty);

            mCountProperty->layout->maxWidth = 40;
            mCountProperty->SetValue(0);
            mCountProperty->onChanged = THIS_FUNC(OnCountChanged);
        }
    }

    ScriptValueProperty::PropertiesList ScriptValueProperty::GetCommonProperties(
        const Vector<Pair<ScriptValue, ScriptValue>>& values, bool& isArray) const
    {
        PropertiesList res;

        for (auto kv : values)
        {
            if (kv.first.IsObject())
            {
                isArray = false;

                kv.first.ForEachProperties(
                    [&](const ScriptValue& name, const ScriptValue& value)
                    {
                        auto nameStr = name.ToString();
                        if (nameStr[0] != '_')
                        {
                            Pair<IScriptValueProperty*, IScriptValueProperty*> elem;
                            elem.first = mnew ScriptValueProperty{ kv.first, name };
                            if (kv.second.IsObject())
                                elem.second = mnew ScriptValueProperty{ kv.second, name };

                            auto fnd = res.Find([&](auto& x) { return x.first == nameStr; });
                            if (fnd)
                                fnd->second.Add(elem);
                            else
                                res.Add({ nameStr, { elem } });
                        }

                        return true;
                    });
            }
            else if (kv.first.IsArray())
            {
                isArray = true;

                for (int i = 0; i < kv.first.GetLength(); i++)
                {
                    Pair<IScriptValueProperty*, IScriptValueProperty*> elem;
                    elem.first = mnew ScriptValueArrayElement{ kv.first, i };
                    if (kv.second.IsObject())
                        elem.second = mnew ScriptValueArrayElement{ kv.second, i };

                    auto nameStr = (String)i;
                    auto fnd = res.Find([&](auto& x) { return x.first == nameStr; });
                    if (fnd)
                        fnd->second.Add(elem);
                    else
                        res.Add({ nameStr, { elem } });
                }
            }
        }

        // Leave only common properties
        res.RemoveAll([&](const auto& item) { return item.second.Count() != values.Count(); });

        return res;
    }

    void ScriptValueProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
    {
        mNeedUpdateProxies = targets != mValuesProxies;
        IPropertyField::SetValueAndPrototypeProxy(targets);
    }

    void ScriptValueProperty::Refresh()
    {
        mIsRefreshing = true;

        PushEditorScopeOnStack scope;

        // Get values from proxies
        Vector<Pair<ScriptValue, ScriptValue>> values =
            mValuesProxies.Convert<Pair<ScriptValue, ScriptValue>>(
                [](const Pair<IAbstractValueProxy*, IAbstractValueProxy*>& x)
                {
                    Pair<ScriptValue, ScriptValue> res;
                    x.first->GetValuePtr(&res.first);

                    if (x.second)
                        x.second->GetValuePtr(&res.second);

                    return res;
                }
            );

        // Get common properties
        auto commonProperties = GetCommonProperties(values, mIsArray);

        if (mIsArray && !values.IsEmpty())
        {
            int commonCount = values[0].first.GetLength();
            bool coundDifferents = false;

            for (int i = 1; i < values.Count(); i++)
            {
                int count = values[i].first.GetLength();
                if (count != commonCount)
                    coundDifferents = true;
            }

            if (mCountDifferents != coundDifferents || mCountOfElements != commonCount)
            {
                if (mCountDifferentconst Ref<Sprite> &mnew ScriptValueProxy(x.first->Clone());#include <memory>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using WeakRef = std::weak_ptr<T>;

template <typename T, typename... Args>
auto mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
class DynamicCast
{
public:
    static Ref<T>& Cast(const Ref<Base>& ref)
    {
        return std::dynamic_pointer_cast<T>(ref);
    }
};

class ScriptValueProperty : public IPropertyField
{
public:
    ScriptValueProperty()
    {
        mSpoiler = mmake<Spoiler>();
    }

    virtual ~ScriptValueProperty() {}

    void Initialize()
    {
        mRemoveBtn = GetRemoveButton();

        mSpoiler->SetParent(this);
    }

    Ref<Spoiler> mSpoiler;

    void AddToBuiltProperties(const String& name, const Ref<IPropertyField>& prop)
    {
        mBuiltProperties.Add(name, prop);
    }

    void ScriptValueProperty::OnCountChanged(const Ref<IPropertyField>& def)
    {
        if (mIsRefreshing)
            return;

        Resize(def->GetCommonValue());
    }

    void ScriptValueProperty::OnExpand()
    {
        Refresh();
    }

    void ScriptValueProperty::Resize(int newCount)
    {
        if (mIsRefreshing)
            return;

        for (const auto& p : mValuesProxies)
        {
            Ref<ScriptValue> first;
            p.first->GetValuePtr(&first);

            int length = first->GetLength();
            if (length > newCount)
            {
                for (int i = 0; i < length - newCount; i++)
                    first->RemoveElement(first->GetLength() - 1);
            }
            else
            {
                for (int i = 0; i < newCount - length; i++)
                    OnAddPressed();
            }
        }

        Refresh();
    }

    void ScriptValueProperty::Remove(int idx)
    {
        for (const auto& p : mValuesProxies)
        {
            Ref<ScriptValue> first;
            p.first->GetValuePtr(&first);
            first->RemoveElement(idx);
        }

        Refresh();
    }

    void ScriptValueProperty::OnAddPressed()
    {
        for (const auto& p : mValuesProxies)
        {
            Ref<ScriptValue> first;
            p.first->GetValuePtr(&first);
            if (first->GetLength() > 0)
                first->AddElement(first->GetElement(first->GetLength() - 1).Copy());
            else
                first->AddElement(ScriptValue(0));
        }

        Refresh();
    }

    const Type* ScriptValueProperty::GetValueType() const
    {
        return GetValueTypeStatic();
    }

    static const Type* ScriptValueProperty::GetValueTypeStatic()
    {
        return &TypeOf(ScriptValue);
    }

    void ScriptValueProperty::SetFieldInfo(const FieldInfo* fieldInfo)
    {
        IPropertyField::SetFieldInfo(fieldInfo);

        if (fieldInfo)
        {
            mExpanded = fieldInfo->HasAttribute<ExpandedByDefaultAttribute>();
            SetHeaderEnabled(!fieldInfo->HasAttribute<NoHeaderAttribute>());
        }
    }

    void ScriptValueProperty::SetCaption(const WString& text)
    {
        mSpoiler->SetCaption(text);
    }

    WString ScriptValueProperty::GetCaption() const
    {
        return mSpoiler->GetCaption();
    }

    Ref<Button> GetRemoveButton()
    {
        if (!mRemoveBtn)
        {
            mRemoveBtn = mmake<Button>("remove small");
            *mRemoveBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, 0));
            AddInternalWidget(mRemoveBtn);
        }

        return mRemoveBtn;
    }

    void ScriptValueProperty::Expand()
    {
        SetExpanded(true);
    }

    void ScriptValueProperty::Collapse()
    {
        SetExpanded(false);
    }

    void ScriptValueProperty::SetExpanded(bool expanded)
    {
        mExpanded = expanded;
        mSpoiler->SetExpanded(expanded);
    }

    bool ScriptValueProperty::IsExpanded() const
    {
        return mSpoiler->IsExpanded();
    }

    void ScriptValueProperty::SetHeaderEnabled(bool enabled)
    {
        mHeaderEnabled = enabled;

        if (mHeaderEnabled)
        {
            mSpoiler->SetHeadHeight(20);
            mSpoiler->GetLayerDrawable<Text>("caption")->enabled = true;
            mSpoiler->GetInternalWidget("expand")->enabledForcibly = true;
            mSpoiler->borderLeft = 10;
            mSpoiler->borderTop = 2;
        }
        else
        {
            mSpoiler->SetHeadHeight(0);
            mSpoiler->GetLayerDrawable<Text>("caption")->enabled = false;
            mSpoiler->GetInternalWidget("expand")->enabledForcibly = false;
            mSpoiler->borderLeft = 0;
            mSpoiler->borderTop = 0;
            mSpoiler->Expand();
        }
    }

    bool ScriptValueProperty::IsHeaderEnabled() const
    {
        return mHeaderEnabled;
    }

    void ScriptValueProperty::OnPropertyChanged(const String& path, const Vector<DataDocument>& before,
                                                const Vector<DataDocument>& after)
    {
        onChangeCompleted(path, before, after);
    }
};
// --- META ---

DECLARE_CLASS(Editor::ScriptValueProperty, Editor__ScriptValueProperty);
// --- END META ---