#include "stdafx.h"
#include "IPropertyField.h"

#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/Label.h"
#include "UI/Widget.h"
#include "UI/WidgetState.h"

namespace Editor
{

	IPropertyField::IPropertyField()
	{
		mRevertBtn = FindChildByType<UIButton>("revert");
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);

		mCaption = FindChildByType<UILabel>("caption")
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

		auto captionWidget = dynamic_cast<UILabel*>(FindChild("propertyName"));
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

		auto captionWidget = dynamic_cast<UILabel*>(FindChild("propertyName"));
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

	void IPropertyField::SetCaptionLabel(UILabel* label)
	{
		mCaption = label;
	}

	UILabel* IPropertyField::GetCaptionLabel() const
	{
		return mCaption;
	}

	bool IPropertyField::IsRevertable() const
	{
		return false;
	}

	void IPropertyField::CheckRevertableState()
	{
		if (auto revertState = state["revert"])
			*revertState = IsRevertable();
	}

	void IPropertyField::OnChanged()
	{
		CheckRevertableState();
		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}
}

DECLARE_CLASS(Editor::IPropertyField);
