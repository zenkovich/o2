#include "o2Editor/stdafx.h"
#include "IPropertyField.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Label.h"
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

	IPropertyField& IPropertyField::operator=(const IPropertyField& other)
	{
		HorizontalLayout::operator=(other);
		return *this;
	}

	void IPropertyField::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;
		Refresh();
	}

	void IPropertyField::SetValueProxy(const Vector<IAbstractValueProxy*>& targets)
	{
		auto protoTargets = targets.Select<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
			[](IAbstractValueProxy* x) { return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(x, nullptr); });

		SetValueAndPrototypeProxy(protoTargets);
	}

	void IPropertyField::SetCaption(const WString& text)
	{
		if (mCaption)
			mCaption->text = text;

		auto captionWidget = dynamic_cast<Label*>(FindChild("propertyName"));
		if (captionWidget)
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

		auto captionWidget = dynamic_cast<Label*>(FindChild("propertyName"));
		if (captionWidget)
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

	void IPropertyField::OnFreeProperty()
	{}

	void IPropertyField::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
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
				AddChild(mRevertBtn);

				Animation revertStateAnim = Animation::EaseInOut(this, "child/revert/layout/maxWidth", 0.0f, 20.0f, 0.15f);
				*revertStateAnim.AddAnimationValue<bool>("child/revert/enabled") = AnimatedValue<bool>::EaseInOut(false, true, 0.15f);
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

DECLARE_CLASS(Editor::IPropertyField);
