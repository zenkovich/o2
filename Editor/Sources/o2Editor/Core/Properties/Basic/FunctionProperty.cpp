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

using namespace o2;

namespace Editor
{
	FunctionProperty::FunctionProperty()
	{
		InitializeControls();
	}

	FunctionProperty::FunctionProperty(const FunctionProperty& other) :
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

		auto sampleRefProperty = dynamic_cast<ActorProperty*>(o2UI.CreateWidget<ActorProperty>());
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
		for (auto valueProxy : mValuesProxies)
		{
			auto funcProxy = dynamic_cast<PointerValueProxy<AbstractFunction>*>(valueProxy.first);
			auto funcProxyProto = dynamic_cast<PointerValueProxy<AbstractFunction>*>(valueProxy.first);

			if (!funcProxy)
				continue;

			int idx = 0;
			funcProxy->GetValuePointer()->ForEachAbstract(
				Function<void(const IAbstractFunction*)>(
					[&](const IAbstractFunction* f)
					{
						if (auto actorSubscription = dynamic_cast<const IActorSubscription*>(f))
						{
							if (idx > mInstances.Count() - 1)
								mInstances.Add(mnew FunctionInstance());

							mInstances[idx++]->values.Add({ const_cast<IActorSubscription*>(actorSubscription), nullptr });
						}
					}));

			if (funcProxyProto)
			{
				idx = 0;
				funcProxyProto->GetValuePointer()->ForEachAbstract(
					Function<void(const IAbstractFunction*)>(
						[&](const IAbstractFunction* f)
						{
							if (auto actorSubscription = dynamic_cast<const IActorSubscription*>(f))
							{
								if (idx < mInstances.Count() && proxyIdx < mInstances[idx]->values.Count())
									mInstances[idx]->values[proxyIdx].second = const_cast<IActorSubscription*>(actorSubscription);
							}
						}));
			}

			proxyIdx++;
		}
	}

	void FunctionProperty::Refresh()
	{
		PushEditorScopeOnStack scope;

		auto prevInstances = mInstances;
		RefreshInstances();

		mSpoiler->RemoveAllChildren(false);

		int idx = 0;
		for (auto& instance : mInstances)
		{
			auto prevInstanceIdx = prevInstances.IndexOf([&](FunctionInstance* x) { return x->values == instance->values; });
			if (prevInstanceIdx >= 0)
			{
				instance = prevInstances[prevInstanceIdx];
				prevInstances.RemoveAt(prevInstanceIdx);
			}
			else
			{
				auto widget = CreateWidget();

				instance->layout = widget->layout;
				instance->refProperty = widget->refProperty;
				instance->removeBtn = widget->removeBtn;
				instance->caption = widget->caption;
				instance->funcDropDown = widget->funcDropDown;
			}

			instance->refProperty->SelectValueAndPrototypePointers<ActorRef, IActorSubscription>(
				instance->values.Convert<IActorSubscription*>([](auto p) { return p.first; }),
				instance->values.Convert<IActorSubscription*>([](auto p) { return p.second; }),
				[](IActorSubscription* s)
				{
					if (!s->actorRef && s->componentRef)
						s->actorRef = s->componentRef->GetOwnerActor();

					return &s->actorRef;
				}
			);

			instance->refProperty->onChangeCompleted =
				[=](const String&, const Vector<DataDocument>&, const Vector<DataDocument>&)
			{
				instance->UpdateFunctionsList(instance->refProperty->GetCommonValue(),
											  !instance->values.IsEmpty() ? instance->values[0].first->componentRef : ComponentRef(),
											  !instance->values.IsEmpty() ? instance->values[0].first->method : String());
			};

			instance->funcDropDown->onSelectedPos = [=](int idx) { instance->OnFunctionSelected(idx); };

			instance->removeBtn->onClick = [=]() { OnRemovePressed(instance); };

			instance->UpdateFunctionsList(instance->refProperty->GetCommonValue(),
										  !instance->values.IsEmpty() ? instance->values[0].first->componentRef : ComponentRef(),
										  !instance->values.IsEmpty() ? instance->values[0].first->method : String());

			instance->caption->text = "#" + (String)idx;
			mSpoiler->AddChild(instance->layout);
			idx++;
		}

		for (auto instance : prevInstances)
			delete instance;
	}

	void FunctionProperty::OnAddPressed()
	{
		for (auto valueProxy : mValuesProxies)
		{
			if (auto funcProxy = dynamic_cast<PointerValueProxy<AbstractFunction>*>(valueProxy.first))
				funcProxy->GetValuePointer()->AddActorSubscription();
		}

		Expand();
		Refresh();
	}

	void FunctionProperty::OnRemovePressed(FunctionInstance* instance)
	{
		for (auto valueProxy : mValuesProxies)
		{
			if (auto funcProxy = dynamic_cast<PointerValueProxy<AbstractFunction>*>(valueProxy.first))
			{
				for (auto value : instance->values)
					funcProxy->GetValuePointer()->RemoveFunction(dynamic_cast<IAbstractFunction*>(value.first));
			}
		}

		Refresh();
	}

	FunctionProperty::FunctionInstance* FunctionProperty::CreateWidget()
	{
		FunctionInstance* res = mnew FunctionInstance();

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

	void FunctionProperty::FunctionInstance::UpdateFunctionsList(const ActorRef& actor, const ComponentRef& selectedComponent,
																 const String& selectedMethod)
	{
		funcDropDown->RemoveAllItems();
		functionsDropdownMap.Clear();

		if (!actor)
			return;

		int selectedIdx = -1;

		auto addItems = [&](const Type& type, Component* comp)
		{
			String typeName = type.InvokeStatic<String>("GetName");
			if (typeName.IsEmpty())
				typeName = GetSmartName(type.GetName());

			String iconName = type.InvokeStatic<String>("GetIcon");
			if (iconName.IsEmpty())
				iconName = "ui/UI4_component_icon.png";

			auto typeItem = funcDropDown->AddItem();

			typeItem->AddLayer("back", mnew Sprite(Color4(0, 0, 0, 0)))->transparency = 0.05f;

			auto icon = o2UI.CreateImage(iconName);
			*icon->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20));
			icon->GetImage()->color = Color4(0, 156, 141, 255);
			typeItem->AddChild(icon);

			auto text = o2UI.CreateLabel(typeName);
			*text->layout = WidgetLayout::BothStretch(20, 0, 0, 0);
			text->horAlign = HorAlign::Left;
			typeItem->AddChild(text);

			for (auto funcInfo : type.GetFunctions())
			{
				if (funcInfo->GetProtectionSection() == ProtectSection::Public && funcInfo->GetParameters().IsEmpty() &&
					funcInfo->GetReturnType() == &TypeOf(void))
				{
					auto funcItem = funcDropDown->AddItem();

					auto text = o2UI.CreateLabel(funcInfo->GetName());
					*text->layout = WidgetLayout::BothStretch();
					text->horAlign = HorAlign::Left;
					text->transparency = 0.8f;
					funcItem->AddChild(text);

					functionsDropdownMap.Add({ comp, funcInfo->GetName() });

					if (comp == selectedComponent && selectedMethod == funcInfo->GetName())
						selectedIdx = funcDropDown->GetItemsCount() - 2;
				}
			}
		};

		addItems(actor->GetType(), nullptr);

		for (auto component : actor->GetComponents())
			addItems(component->GetType(), component);

		funcDropDown->SelectItemAt(selectedIdx);
	}

	void FunctionProperty::FunctionInstance::OnFunctionSelected(int idx)
	{
		for (auto& value : values)
		{
			value.first->componentRef = functionsDropdownMap[idx].first;
			value.first->method = functionsDropdownMap[idx].second;
		}
	}

	const Type* FunctionProperty::GetValueType() const
	{
		return GetValueTypeStatic();
	}

	const Type* FunctionProperty::GetValueTypeStatic()
	{
		return FunctionType::commonType;
	}

	void FunctionProperty::SetFieldInfo(const FieldInfo* fieldInfo)
	{
		IPropertyField::SetFieldInfo(fieldInfo);

		if (fieldInfo)
		{
			if (fieldInfo->HasAttribute<ExpandedByDefaultAttribute>())
				mSpoiler->Expand();
		}
	}

	void FunctionProperty::SetCaption(const WString& text)
	{
		mSpoiler->SetCaption(text);

		Text* spoilerCaptionLayer = mSpoiler->GetLayerDrawable<Text>("caption");
		if (spoilerCaptionLayer)
		{
			Vec2F captionSize = Text::GetTextSize(text, spoilerCaptionLayer->GetFont().Get(), spoilerCaptionLayer->GetFontHeight());
			*mHeaderContainer->layout = WidgetLayout::HorStretch(VerAlign::Top, captionSize.x + 20.0f, 0, 17, 0);
		}
	}

	WString FunctionProperty::GetCaption() const
	{
		return mSpoiler->GetCaption();
	}

	Button* FunctionProperty::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			mRemoveBtn = o2UI.CreateWidget<Button>("remove small");
			*mRemoveBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, 0));
			AddInternalWidget(mRemoveBtn);
		}

		return mRemoveBtn;
	}

	void FunctionProperty::Expand()
	{
		SetExpanded(true);
	}

	void FunctionProperty::Collapse()
	{
		SetExpanded(false);
	}

	void FunctionProperty::SetExpanded(bool expanded)
	{
		mSpoiler->SetExpanded(expanded);
	}

	bool FunctionProperty::IsExpanded() const
	{
		return mSpoiler->IsExpanded();
	}

	void FunctionProperty::OnPropertyChanged(const String& path, const Vector<DataDocument>& before,
											 const Vector<DataDocument>& after)
	{
		// 		for (auto& pair : mTargetObjects)
		// 			pair.first.SetValue();

		onChangeCompleted(path, before, after);
	}

	bool FunctionProperty::FunctionInstance::operator==(const FunctionInstance& other) const
	{
		return values == other.values;
	}
}

DECLARE_CLASS(Editor::FunctionProperty);
