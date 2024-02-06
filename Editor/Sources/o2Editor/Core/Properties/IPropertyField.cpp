#include "o2Editor/stdafx.h"
#include "IPropertyField.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	IPropertyField::IPropertyField()
	{
		mCaption = FindChildByType<Label>("caption");
	}

	IPropertyField::IPropertyField(const IPropertyField& other) :
		HorizontalLayout(other)
	{}

	IPropertyField::~IPropertyField()
	{
		FreeValuesProxies();
	}

	IPropertyField& IPropertyField::operator=(const IPropertyField& other)
	{
		HorizontalLayout::operator=(other);
		return *this;
	}

	void IPropertyField::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		FreeValuesProxies();

		mValuesProxies = targets;

		if (!mValuesProxies.IsEmpty())
			OnTypeSpecialized(mValuesProxies[0].first->GetType());

		Refresh();
	}

	void IPropertyField::FreeValuesProxies()
	{
		for (auto& pair : mValuesProxies)
		{
			delete pair.first;

			if (pair.second)
				delete pair.second;
		}

		mValuesProxies.Clear();
	}

	const IPropertyField::TargetsVec& IPropertyField::GetValueAndPrototypeProxy() const
	{
		return mValuesProxies;
	}

	void IPropertyField::SetValueProxy(const Vector<IAbstractValueProxy*>& targets)
	{
		auto protoTargets = targets.Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
			[](IAbstractValueProxy* x) { return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(x, nullptr); });

		SetValueAndPrototypeProxy(protoTargets);
	}

	void IPropertyField::SetParentContext(PropertiesContext* context)
	{
		mParentContext = context;
	}

	void IPropertyField::SetCaption(const WString& text)
	{
		if (mCaption)
			mCaption->text = text;

		if (auto captionWidget = dynamic_cast<Label*>(FindChild("propertyName")))
			captionWidget->text = text;
		else if (auto captionWidget = dynamic_cast<Label*>(FindInternalWidget("propertyName")))
			captionWidget->text = text;
		else
		{
			auto captionLayer = GetLayerDrawable<Text>("caption");
			if (captionLayer)
				captionLayer->text = text;
		}
	}

	WString IPropertyField::GetCaption() const
	{
		if (mCaption)
			return mCaption->text;

		if (auto captionWidget = dynamic_cast<Label*>(FindChild("propertyName")))
			return captionWidget->text;
		else if (auto captionWidget = dynamic_cast<Label*>(FindInternalWidget("propertyName")))
			return captionWidget->text;
		else
		{
			auto captionLayer = GetLayerDrawable<Text>("caption");
			if (captionLayer)
				return captionLayer->text;
		}

		return "";
	}

	Button* IPropertyField::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			auto buttonContainer = mnew Widget();
			buttonContainer->name = "remove container";
			buttonContainer->layout->maxWidth = 20;

			if (auto layout = FindChild("layout"))
				layout->AddChild(buttonContainer);
			else
				AddChild(buttonContainer);

			mRemoveBtn = o2UI.CreateWidget<Button>("remove small");
			mRemoveBtn->name = "remove";
			*mRemoveBtn->layout = WidgetLayout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(2, 0));
			buttonContainer->AddChild(mRemoveBtn);
		}

		return mRemoveBtn;
	}

	const Type* IPropertyField::GetValueType() const
	{
		return GetValueTypeStatic();
	}

	const Type* IPropertyField::GetValueTypeStatic()
	{
		return nullptr;
	}

	bool IPropertyField::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void IPropertyField::SetValuePath(const String& path)
	{
		mValuesPath = path;
	}

	const String& IPropertyField::GetValuePath() const
	{
		return mValuesPath;
	}

	void IPropertyField::SetCaptionLabel(Label* label)
	{
		mCaption = label;
	}

	Label* IPropertyField::GetCaptionLabel() const
	{
		return mCaption;
	}

	void IPropertyField::SetRevertable(bool revertable)
	{
		mRevertable = revertable;
		CheckRevertableState();
	}

	bool IPropertyField::IsRevertable() const
	{
		return mRevertable;
	}

	void IPropertyField::SetFieldInfo(const FieldInfo* fieldInfo)
	{
		mFieldInfo = fieldInfo;
	}

	String IPropertyField::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void IPropertyField::OnFreeProperty()
	{}

	void IPropertyField::CheckValueChangeCompleted()
	{
		Vector<DataDocument> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData || valuesData.IsEmpty())
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	bool IPropertyField::IsValueRevertable() const
	{
		return false;
	}

	void IPropertyField::CheckRevertableState()
	{
		bool isRevertable = IsValueRevertable();

		if (isRevertable)
		{
			mRevertBtn = FindChildByType<Button>("revert");

			if (!mRevertBtn)
			{
				mRevertBtn = o2UI.CreateWidget<Button>("revert");
				*mRevertBtn->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F());
				mRevertBtn->layout->maxWidth = 0;

				if (auto layout = FindChild("layout"))
					layout->AddChild(mRevertBtn);
				else
					AddChild(mRevertBtn);

				String path;
				Actor* itActor = mRevertBtn;
				while (itActor != this)
				{
					path = "child/" + itActor->GetName() + "/" + path;
					itActor = itActor->GetParent();
				}

				auto revertStateAnim = AnimationClip::EaseInOut(path + "layout/maxWidth", 0.0f, 20.0f, 0.15f);
				*revertStateAnim->AddTrack<bool>(path + "enabled") = AnimationTrack<bool>::EaseInOut(false, true, 0.15f);
				AddState("revert", revertStateAnim);
			}

			if (mRevertBtn)
				mRevertBtn->onClick = THIS_FUNC(Revert);
		}

		if (auto revertState = state["revert"])
			*revertState = isRevertable;
	}

	void IPropertyField::OnValueChanged()
	{
		CheckRevertableState();
		onChanged(this);

		if (SceneEditScreen::IsSingletonInitialzed())
			o2EditorSceneScreen.OnSceneChanged();
	}

	void IPropertyField::BeginUserChanging()
	{
		StoreValues(mBeforeChangeValues);
	}

	void IPropertyField::EndUserChanging()
	{
		CheckValueChangeCompleted();
	}
}
// --- META ---

DECLARE_CLASS(Editor::IPropertyField, Editor__IPropertyField);
// --- END META ---
