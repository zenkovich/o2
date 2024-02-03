#include "o2Editor/stdafx.h"
#include "FunctionProperty.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Utils/Editor/Attributes/NoHeaderAttribute.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2/Utils/StringUtils.h"
#include "o2Editor/Core/Properties/Basic/ActorProperty.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Properties.h"
#include <o2/Scene/Components/ScriptableComponent.h>

using namespace o2;

namespace Editor
{
	FunctionProperty::FunctionProperty()
	{
		InitializeControls();
	}

	FunctionProperty::FunctionProperty(const FunctionProperty& other):
		IPropertyField(other)
	{
		InitializeControls();
	}

	FunctionProperty& FunctionProperty::operator=(const FunctionProperty& other)
	{
		IPropertyField::operator=(other);
		InitializeControls();
		return *this;
	}

	void FunctionProperty::OnFreeProperty()
	{
	}

	void FunctionProperty::InitializeControls()
	{
		// Spoiler
		mSpoiler = FindChildByType<Spoiler>(false);

		if (!mSpoiler)
		{
			mSpoiler = o2UI.CreateWidget<Spoiler>("expand with caption");
			AddChild(mSpoiler);
		}

		if (mSpoiler)
			mSpoiler->onExpand = THIS_FUNC(OnExpand);

		mSpoiler->borderTop = 5;
		mSpoiler->AddLayer("func icon", mnew Sprite("ui/function_icon.png"), Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(6, 0)));
		mSpoiler->GetLayer("caption")->layout.offsetLeft = 24;

		mHeaderContainer = mnew HorizontalLayout();
		*mHeaderContainer->layout = WidgetLayout::HorStretch(VerAlign::Top, 100, 0, 20, 0);
		mHeaderContainer->baseCorner = BaseCorner::Right;
		mHeaderContainer->expandHeight = false;
		mHeaderContainer->SetInternalParent(mSpoiler, false);

		// Add button
		mAddButton = mSpoiler->FindChildByTypeAndName<Button>("add button");
		if (!mAddButton)
		{
			mAddButton = o2UI.CreateWidget<Button>("add small");
			mAddButton->name = "add button";
			mAddButton->layout->maxWidth = 20;
			mAddButton->layout->minHeight = 20;
			mAddButton->onClick = THIS_FUNC(OnAddPressed);

			mHeaderContainer->AddChild(mAddButton);
		}

		// Widget sample
		mWidgetSample = o2UI.CreateHorLayout();
		mWidgetSample->layout->minHeight = 20;
		mWidgetSample->expandHeight = true;
		mWidgetSample->expandWidth = true;
		mWidgetSample->fitByChildren = false;

		auto sampleCaption = o2UI.CreateLabel("");
		sampleCaption->name = "caption";
		sampleCaption->layout->maxWidth = 50.0f;
		sampleCaption->horAlign = HorAlign::Left;
		mWidgetSample->AddChild(sampleCaption);

		auto controllersContainerWrapper = mnew Widget();
		*controllersContainerWrapper->layout = WidgetLayout::BothStretch();
		controllersContainerWrapper->name = "controls";
		mWidgetSample->AddChild(controllersContainerWrapper);

		auto controlsContainer = o2UI.CreateHorLayout();
		*controlsContainer->layout = WidgetLayout::BothStretch();
		controlsContainer->expandHeight = true;
		controlsContainer->expandWidth = true;
		controlsContainer->spacing = 5.0f;
		controlsContainer->name = "layout";
		controllersContainerWrapper->AddChild(controlsContainer);

		auto sampleRefProperty = DynamicCast<ActorProperty>(o2UI.CreateWidget<ActorProperty>());
		sampleRefProperty->name = "actor ref";
		controlsContainer->AddChild(sampleRefProperty);

		auto sampleFuncDropdown = o2UI.CreateCustomDropdown();
		sampleFuncDropdown->name = "drop down";
		sampleFuncDropdown->SetMaxListSizeInItems(15);
		sampleFuncDropdown->GetItemSample()->layout->minHeight = 20;
		controlsContainer->AddChild(sampleFuncDropdown);

		auto sampleRemoveBtn = o2UI.CreateWidget<Button>("remove small");
		sampleRemoveBtn->name = "remove btn";
		sampleRemoveBtn->layout->maxWidth = 20;
		controlsContainer->AddChild(sampleRemoveBtn);

		// Other
		expandHeight = true;
		expandWidth = true;
		fitByChildren = true;
	}

	void FunctionProperty::OnExpand()
	{
		Refresh();
	}

	FunctionProperty::~FunctionProperty()
	{
		for (auto instance : mInstances)
			delete instance;
	}

	void FunctionProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;
		Refresh();
	}

	void FunctionProperty::RefreshInstances()
	{
		mInstances.Clear();

		int proxyIdx = 0;
		for (const auto& valueProxy : mValuesProxies)
		{
			const auto& funcProxy = DynamicCast<PointerValueProxy<AbstractFunction>>(valueProxy.first);
			const auto& funcProxyProto = DynamicCast<PointerValueProxy<AbstractFunction>>(valueProxy.first);

			if (!funcProxy)
				continue;

			int idx = 0;
			funcProxy->GetValuePointer()->ForEachAbstract(
				Function<void(const IAbstractFunction*)>(
					[&](const IAbstractFunction* f)
			{
				if (const auto actorSubscription = DynamicCast<const IActorSubscription>(f))
				{
					if (idx > mInstances.Count() - 1)
						mInstances.Add(mnew FunctionInstance());

					mInstances[idx++]->values.Add({ const_cast<IActorSubscription*>(actorSubscription), nullptr });
				}
			}));

			if (funcProxyProto)
			{
				idx = 0;
				funcProxyProto->GetValures->refProperty instead of this->refProperty. 
Text->SetText, instead of (*text).SetText.
.cpp should include only .h files. 
.newLine(), is replaced with \n in strings. 
In AddLayer function, instead of mnew, new is used.
Text->staticText "caption" from CreateWidget function should be used like widget->staticText.caption.

******************************************************************************/

refPointer()->ForEachAbstract(Function<const IAbstractFunction*>([&](const IAbstractFunction* f)
	{
		if (auto actorSubscription = DynamicCast<const IActorSubscription>(f))
		{
			if (idx < mInstances.Count() && proxyIdx < mInstances[idx]->values.Count())
				mInstances[idx]->values[proxyIdx].second = const_cast<IActorSubscription*>(actorSubscription);
		}
	}));

	proxyIdx++;
}

void FunctionProperty::Refresh()
{
	PushEditorScopeOnStack scope;

	auto prevInstances = mInstances;
	RefreshInstances();

	mSpoiler->RemoveAllChildren(false);

	int idx = 0;
	for (Ref<FunctionInstance>& instance : mInstances)
	{
		int prevInstanceIdx = prevInstances.IndexOf([&](FunctionInstance* x) { return x->values == instance->values; });
		if (prevInstanceIdx >= 0)
		{
			instance = prevInstances[prevInstanceIdx];
			prevInstances.RemoveAt(prevInstanceIdx);
		}
		else
		{
			Ref<FunctionInstance> widget = CreateWidget();

			instance->layout = widget->layout;
			instance->refProperty = widget->refProperty;
			instance->removeBtn = widget->removeBtn;
			instance->caption = widget->caption;
			instance->funcDropDown = widget->funcDropDown;
		}

		instance->refProperty->SelectValueAndPrototypePointers<const Ref<Actor>&, IActorSubscription>(
			instance->values.Convert<IActorSubscription*>([](auto p) { return p.first; }),
			instance->values.Convert<IActorSubscription*>([](auto p) { return p.second; }),
			[](IActorSubscription* s)
		{
			if (!s->actorRef && s->componentRef)
				s->actorRef = s->componentRef->GetOwnerActor();

			return &s->actorRef;
		});

		instance->refProperty->onChangeCompleted =
			[=](const String& a, const Vector<DataDocument>& b, const Vector<DataDocument>& c)
		{
			instance->UpdateFunctionsList(instance->refProperty->GetCommonValue(),
				!instance->values.IsEmpty() ? instance->values[0].first->componentRef : nullptr,
				!instance->values.IsEmpty() ? instance->values[0].first->method : "");
		};

		instance->funcDropDown->onSelectedPos = [=](int idx) { instance->OnFunctionSelected(idx); };

		instance->removeBtn->onClick = [=]() { OnRemovePressed(instance); };

		instance->UpdateFunctionsList(instance->refProperty->GetCommonValue(),
			!instance->values.IsEmpty() ? instance->values[0].first->componentRef : nullptr,
			!instance->values.IsEmpty() ? instance->values[0].first->method : "");

		instance->caption->SetText("#" + std::to_string(idx));
		mSpoiler->AddChild(instance->layout);
		idx++;
	}

	for (FunctionInstance* instance : prevInstances)
		delete instance;
}

void FunctionProperty::OnAddPressed()
{
	for (Ref<IProxiedValue>& valueProxy : mValuesProxies)
	{
		if (auto funcProxy = DynamicCast<PointerValueProxy<AbstractFunction>>(valueProxy))
			funcProxy->GetValuePtr()->AddActorSubscription();
	}

	Expand();
	Refresh();
}

void FunctionProperty::OnRemovePressed(Ref<FunctionInstance>& instance)
{
	for (Ref<IProxiedValue>& valueProxy : mValuesProxies)
	{
		if (auto funcProxy = DynamicCast<PointerValueProxy<AbstractFunction>>(valueProxy))
		{
			for (auto value : instance->values)
				funcProxy->GetValuePtr()->RemoveFunction(DynamicCast<IAbstractFunction>(value.first));
		}
	}

	Refresh();
}

FunctionProperty::Ref<FunctionProperty::FunctionInstance> FunctionProperty::CreateWidget()
{
	Ref<FunctionInstance> res = mnew FunctionInstance();

	if (mWidgetsBuffer.IsEmpty())
		res->layout = mWidgetSample->CloneAs<HorizontalLayout>();
	else
		res->layout = mWidgetsBuffer.PopBack();

	res->caption = res->layout->GetChildByType<Label>("caption");
	res->funcDropDown = res->layout->GetChildByType<CustomDropDown>("controls/layout/drop down");
	res->refProperty = res->layout->GetChildByType<ActorProperty>("controls/layout/actor ref");
	res->removeBtn = res->layout->GetChildByType<Button>("controls/layout/remove btn");

	return res;
}

void FunctionProperty::FunctionInstance::UpdateFunctionsList(const Ref<Actor>& actor, const Ref<Component>& selectedComponent,
	const String& selectedMethod)
{
	funcDropDown->RemoveAllItems();
	functionsDropdownMap.Clear();

	if (!actor)
		return;

	int selectedIdx = -1;

	auto collectFunctions = [&](const String& typeName, const String& iconName, const Vector<String>& functionsList, Component* comp)
	{
		refPointer<ItemContainer> typeItem = funcDropDown->AddItem();
		functionsDropdownMap.Add({ nullptr, "" });

		Ref<Sprite> icon = new Sprite(Color4(0, 0, 0, 0));
		icon->transparency = 0.05f;
		typeItem->AddLayer("back", icon);

		Ref<ImageWidget> iconWidget = o2UI.CreateImage(iconName);
		iconWidget->GetImage()->SetColor(Color4(0, 156, 141, 255));
		iconWidget->SetLayout(WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20)));
		typeItem->AddChild(iconWidget);

		Ref<TextLabel> text = o2UI.CreateLabel(typeName);
		text->SetLayout(WidgetLayout::BothStretch(20, 0, 0, 0));
		text->SetHorAlign(TextLabel::HorAlign::Left);
		typeItem->AddChild(text);

		for (const auto& funcName : functionsList)
		{
			refPointer<ItemContainer> funcItem = funcDropDown->AddItem();
			functionsDropdownMap.Add({ DynamicCast<IAbstractFunction>(comp), ReflectCodeBase::FunctionToCode(funcName) });

			Ref<TextLabel> text = o2UI.CreateLabel(reflectCode.FunctionToDisplay(funcName));
			text->SetText(funcName);
			funcItem->AddChild(text);

			const auto thisMethod = funcName.LeftOf('(');

			if (thisMethod.Compare(selectedMethod) == 0)
				selectedIdx = funcDropDown->GetItemsCount() - 1;
		}
	};

	MethodInfo info;
	actor->foreachComponent([&](Component* component)
	{
		const ComponentTypeInfo* compInfo = ComponentsDictionary::Instance().GetTypeInfo(component->GetType());
		if (!compInfo)
			return;

		auto m = ActorTypeInfo::GetTargetsFunctions(component->GetType());
		if (!m)
			return;

		const auto& em = (*m).GetFunctions();

		for (const auto& methodFunction : em)
		{
			if (!methodFunction.getAttribute())
				continue;

			Vector<String> attrToFunctionList;

			bool accepted = (*methodFunction.getAttribute())->isTypeOf<Function>();
			if (!accepted)
				continue;

			Function::Parser(parser);
			if (!parser.success)
				continue;

			const_cast<ReflectCodeBase&>(reflectCode).SetMapName(parser.currentMapName);
			const auto name = reflectCode.FunctionToMapName(typeid(*component).name(), (String)estd::to_string(component).c_str(), methodFunction.getType()->name, methodFunction.getName());

			collectFunctions(compInfo->GetType().name, iconPath, funInfo->Names, component);
		}
	});

	for (auto& funInfo: info.Types)
	{
		if (funInfo->GetType() == typeid(void))
		{
			auto stringizedPackage = (String)ESTD_STRINGIZE_PACKAGE_WITH_SEPARATOR(funInfo->GetPackage());
			collectFunctions(funInfo->GetName(), "Editor2/Icons/Type/" + stringizedPackage + funInfo->GetName(), funInfo->Names, nullptr);
		}
	}

	funcDropDown->SetSelectedPos(selectedIdx);
}

FunctionProperty::FunctionInstance::~FunctionInstance()
{
	functionsDropdownMap.Clear();
}

FunctionProperty::FunctionProperty(
	const Ref<SerializableProperty>& serializedObject, const Ref<Object>& object,
	const Ref<IObjectEditor>& editor, const std::type_info& valueType, const Func<Ref<IValueProxy>(IProxiedValue&)>& createValueProxy)
	: ComplexProperty(serializedObject, object, editor, valueType, createValueProxy),
	mRefObject(nullptr),
	mMetadata(MetaInfo::GetMetadata(valueType))
{
	if (!mSerializedObject->Has("Functions") || mSerializedObject->Get("Functions").IsNone())
		mSerializedObject->Set("Functions", Vector<DataDocument>());

	mValuesProxies << createValueProxy(mRefProperty);

	{
		auto editorInterface = editor->GetEditorInterface()->AsEditorInterface();
		o2UI.Create(editorInterface);
	}

	mWidgetSample = o2UI.LoadLayoutFromFile("/Game/Widgets/FunctionProperty/Function");
	mSpoiler = mWidgetSample->GetChildByType<Ref<Spoiler>>("spoiler");

	mSpoiler->SetCaption("Functions");
	mSpoiler->SetState(Spoiler::STATE::Expanded);
	mSpoiler->SetScrollToVisibleForNewChildItems(true);

	const auto props = mWidgetSample->GetChildrenByType<Ref<SerializableProperty>>();
	mPropertySample = props[0];

	Ref<IconSample> iconSample = mnew IconSample();

	const Vector<Ref<AbstractFunction>>& functionsList = dynamic_cast<Factory*>(serializedObject.Get())->GetFunctions();

	mRefProperty = nullptr;

	if (mRefObject)
		mRefProperty = mnew ActorProperty();
	else
		mRefProperty = mnew ObjectProperty();

	mRefProperty->SetInstance(editor->GetEditorObject()->AsObject());

	foreachXmlElement(mSerializedObject->Root()["Functions"], [&](const XmlElement& xfunc)
	{
		Ref<FunctionInstance> inst = mnew FunctionInstance();

		ObjectInstanceMetadata fakeInstance;
		fakeInstance.SetValue(nullptr);

		auto propClone = mPropertySample->CloneAs<SerializableProperty>();
		propClone->Setup(fakeInstance, xfunc);

		inst->refProperty = mRefProperty;
		inst->refProperty->Serialize(propClone->NestedObject(), false, false, false);

		auto layout = mWidgetSample->CloneAs<HorizontalLayout>();

		BuildEditorForObjectProperty(layout, mRefProperty, inst->refProperty);

		inst->layout = layout;
		inst->caption = layout->GetChildByType<TextLabel>("caption");
		inst->funcDropDown = layout->GetChildByType<Ref<CustomDropDown>>("controls/layout/drop down");
		inst->removeBtn = layout->GetChildByType<Ref<Button>>("controls/layout/remove btn");

		int refIndex = 0;

		Vector<AbstractFunction*> linkedFunctionsList;

		for (const auto& f : functionsList)
		{
			const auto& fa = f->abstractFunctions;

			for (const auto& l : fa)
				if (l.second->GetDisplayName().StartsWith(xfunc.GetName()))
					linkedFunctionsList.PushBack(l.second);
		}

		// fill value with actual values if available
		if (xfunc.HasChild("RegisteredFunctions"))
		{
			const auto regFuncsNode = xfunc.GetChild("RegisteredFunctions");

			foreachXmlElement(regFuncsNode, [&](const XmlElement& regFunc)
			{
				const auto& funcName = regFunc.GetName();

				auto func = mRefProperty->FindFunction(funcName).get();
				AbstractFunction* abstractFunc = nullptr;

				if (func)
					abstractFunc = dynamic_cast<AbstractFunction*>(func);

				if (!abstractFunc)
				{
					auto knownFunc = RegistryDatabase::Instance().FindFunction(funcName);
					if (knownFunc)
						linkedFunctionsList << knownFunc;
					else
					{
						Debug::Log(Debug::LEVEL::Error, "Unable to find function '%s'", (String)funcName);
					}
				}
			});
		}

		Vector<AbstractFunction*> availableFunctions;

		for (const auto& f : linkedFunctionsList)
			availableFunctions.push_back(f);

		inst->values << Make```
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template< typename T, typename... Args >
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
struct DynamicCast
{
    template<typename U>
    static Ref<T> Cast(Ref<U> ptr)
    {
        return std::dynamic_pointer_cast<T>(ptr);
    }
};

namespace Editor
{
    using namespace std;

    class FunctionProperty : public IPropertyField
    {
    public:
        struct FunctionInstance
        {
            Ref<Component> componentRef;
            String method;
            Vector< KeyValuePair<RefPtr<IPropertyField>, Ref<Component>>> values;

            bool operator==(const FunctionInstance& other) const;
        };

        using FunctionsMap = UnorderedMap<size_t, Pair<RefPtr<Component>, String>>;

        FunctionProperty(Widget* parent = nullptr);

        void UpdateFunctions(const Ref<Actor>& actor, const Ref<Actor>& oldActor);

        void SetSelectedFunction(const String& componentType, const String& methodName, const Ref<Component>& component);

        itkGetterSetter(const WString&, Caption);
        itkGetterSetter(bool, Expanded);

        void AppendFunctionInstance(const String &caption, Func<void(EditValue<FunctionInstance>&)> instanceBuilder,
                                    bool allowMulti = true);
    protected:
        struct FunctionEntry
        {
            Ref<Button> expandBtn;
            Ref<Panel> expandGroup;
            Ref<Text> label;
        };

        Ref<Panel>             mContent;
        Ref<ScrollPanel>       mScrollPanel;
        Ref<Button>            mNewBtn;
        Ref<SingleInvokable>   mSizeHandler;
        FunctionsMap           mComponentFunctionMap;
        Ref<Widget>            mPlaceholder;

        inline Ref<Widget> CreateHeader(const WString& caption);

        void OnPropertyExpandedChanged();

        inline FunctionEntry CreateEntry();

        void FindFunctionEntries(const Ref<Panel>& panel,
                                Vector<FunctionEntry>& result);

        size_t CalculateExpandedHeight();

        void CalculatePanelSize();

        static void InitializeComponent(Vector <RefPtr<Component>>& components, const Ref <Widget>& funcDropDown,
                                       int& selectedIdx, const Ref <Component>& selectedComponent,
                                       const String& selectedMethod);

        static void FunctionClicked(const Ref <Component>& component, const String& method, const Ref <Component>&
        selectedComponent,
                                  String& selectedMethod, const Ref <Component>& comp, const String&
                                  funcName,
                                  const Ref <Widget>& funcDropDown, int& selectedIdx);

        void OnFunctionSelected(int idx);

        const Type* GetValueType() const override;

        static const Type* GetValueTypeStatic();

        void SetFieldInfo(const FieldInfo* fieldInfo) override;

        void SetCaption(const WString& text) override;

        WString GetCaption() const override;

        Button* GetRemoveButton();

        void Expand();

        void Collapse();

        void SetExpanded(bool expanded);

        bool IsExpanded() const;

        void OnPropertyChanged(const String& path, const Vector<DataDocument>& before,
        const Vector<DataDocument>& after);

    private:
        struct FunctionData
        {
            FunctionInstance instance;
            RefPtr<IPropertyField> field;
        };

        using MultiFunctionData = RefPtr<Ref<Vector<FunctionData>>>;

        MultiFunctionData mFunctionData;

        Ref<Button> mRemoveBtn;
        Ref<Panel> mHeaderContainer;
        Ref<Spoiler> mSpoiler;
        Vector<Ref<FunctionEntry>> mFunctionEntries;
    };

    BEGIN_META(Editor::FunctionProperty);
    END_META();
}
```