#include "stdafx.h"
#include "Vector2FloatProperty.h"

#include "Application/Application.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace Editor
{
	Vec2FProperty::Vec2FProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("vector2 property");

		mRevertBtn = mPropertyWidget->GetChildByType<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);

		mXEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget->GetChild("layout/x edit"));
		mYEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget->GetChild("layout/y edit"));

		mXEditBox->onChangeCompleted = THIS_FUNC(OnXEdited);
		mXEditBox->text = "--";
		mXEditBox->SetFilterFloat();

		if (auto xHandleLayer = mXEditBox->FindLayer("arrows"))
		{
			mXEditBox->onDraw += [&]() { mXDragHangle.OnDrawn(); };

			mXDragHangle.cursorType = CursorType::SizeNS;
			mXDragHangle.isUnderPoint = [=](const Vec2F& point) { return xHandleLayer->IsUnderPoint(point); };
			mXDragHangle.onMoved = THIS_FUNC(OnXDragHandleMoved);
			mXDragHangle.onCursorPressed =  THIS_FUNC(OnXMoveHandlePressed);
			mXDragHangle.onCursorReleased =  THIS_FUNC(OnXMoveHandleReleased);
		}

		mYEditBox->onChangeCompleted = THIS_FUNC(OnYEdited);
		mYEditBox->text = "--";
		mYEditBox->SetFilterFloat();

		if (auto yHandleLayer = mYEditBox->FindLayer("arrows"))
		{
			mYEditBox->onDraw += [&]() { mYDragHangle.OnDrawn(); };

			mYDragHangle.cursorType = CursorType::SizeNS;
			mYDragHangle.isUnderPoint = [=](const Vec2F& point) { return yHandleLayer->IsUnderPoint(point); };
			mYDragHangle.onMoved = THIS_FUNC(OnYDragHandleMoved);
			mYDragHangle.onCursorPressed =  THIS_FUNC(OnYMoveHandlePressed);
			mYDragHangle.onCursorReleased =  THIS_FUNC(OnYMoveHandleReleased);
		}
	}

	Vec2FProperty::~Vec2FProperty()
	{
		delete mPropertyWidget;
	}

	void Vec2FProperty::SetValue(const Vec2F& value)
	{
		for (auto ptr : mValuesProxies)
			SetProxy<Vec2F>(ptr.first, value);

		SetCommonValue(value);
	}

	void Vec2FProperty::SetValueX(float value)
	{
		for (auto ptr : mValuesProxies)
		{
			Vec2F newValue = GetProxy<Vec2F>(ptr.first);
			newValue.x = value;
			SetProxy<Vec2F>(ptr.first, newValue);
		}

		SetCommonValueX(value);
	}

	void Vec2FProperty::SetValueY(float value)
	{
		for (auto ptr : mValuesProxies)
		{
			Vec2F newValue = GetProxy<Vec2F>(ptr.first);
			newValue.y = value;
			SetProxy<Vec2F>(ptr.first, newValue);
		}

		SetCommonValueY(value);
	}

	void Vec2FProperty::SetUnknownValue(const Vec2F& defaultValue /*= Vec2F()*/)
	{
		mXValuesDifferent = true;
		mYValuesDifferent = true;

		mCommonValue = defaultValue;

		mXEditBox->text = "--";
		mYEditBox->text = "--";

		OnChanged();
	}

	void Vec2FProperty::SetXUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mXValuesDifferent = true;
		mCommonValue.x = defaultValue;
		mXEditBox->text = "--";

		OnChanged();
	}

	void Vec2FProperty::SetYUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mYValuesDifferent = true;
		mCommonValue.y = defaultValue;
		mYEditBox->text = "--";

		OnChanged();
	}

	void Vec2FProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;

		Refresh();
	}

	void Vec2FProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastXDifferent = mXValuesDifferent;
		auto lastYDifferent = mYValuesDifferent;

		auto newCommonValue = GetProxy<Vec2F>(mValuesProxies[0].first);
		auto newXValuesDifferent = false;
		auto newYValuesDifferent = false;

		for (int i = 1; i < mValuesProxies.Count(); i++)
		{
			auto value = GetProxy<Vec2F>(mValuesProxies[i].first);
			if (!Math::Equals(newCommonValue.x, value.x))
				newXValuesDifferent = true;

			if (!Math::Equals(newCommonValue.y, value.y))
				newYValuesDifferent = true;
		}

		if (newXValuesDifferent)
		{
			if (!lastXDifferent)
				SetXUnknownValue(newCommonValue.x);
		}
		else if (!Math::Equals(lastCommonValue.x, newCommonValue.x) || lastXDifferent)
			SetCommonValueX(newCommonValue.x);

		if (newYValuesDifferent)
		{
			if (!lastYDifferent)
				SetYUnknownValue(newCommonValue.y);
		}
		else if (!Math::Equals(lastCommonValue.y, newCommonValue.y) || lastYDifferent)
			SetCommonValueY(newCommonValue.y);

		CheckRevertableState();
	}

	void Vec2FProperty::Revert()
	{
		for (auto ptr : mValuesProxies)
		{
			if (ptr.second)
			{
				SetProxy<Vec2F>(ptr.first, GetProxy<Vec2F>(ptr.second));
			}
		}

		Refresh();
	}

	UIWidget* Vec2FProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	Vec2F Vec2FProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool Vec2FProperty::IsValuesDifferent() const
	{
		return mXValuesDifferent || mYValuesDifferent;
	}

	const Type* Vec2FProperty::GetFieldType() const
	{
		return &TypeOf(Vec2F);
	}

	void Vec2FProperty::SetCommonValue(const Vec2F& value)
	{
		mXValuesDifferent = false;
		mYValuesDifferent = false;

		mCommonValue = value;

		mXEditBox->text = (WString)mCommonValue.x;
		mYEditBox->text = (WString)mCommonValue.y;

		OnChanged();
	}

	void Vec2FProperty::SetCommonValueX(float value)
	{
		mXValuesDifferent = false;
		mCommonValue.x = value;
		mXEditBox->text = (WString)value;

		OnChanged();
	}

	void Vec2FProperty::SetCommonValueY(float value)
	{
		mYValuesDifferent = false;
		mCommonValue.y = value;
		mYEditBox->text = (WString)value;

		OnChanged();
	}

	void Vec2FProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesProxies)
		{
			if (ptr.second && !Math::Equals(GetProxy<Vec2F>(ptr.first), GetProxy<Vec2F>(ptr.second)))
			{
				revertable = true;
				break;
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
	}

	void Vec2FProperty::OnXEdited(const WString& data)
	{
		if (mXValuesDifferent && data == "--")
			return;

		SetXValueByUser((const float)data);
	}

	void Vec2FProperty::OnYEdited(const WString& data)
	{
		if (mYValuesDifferent && data == "--")
			return;

		SetYValueByUser((const float)data);
	}

	float Vec2FProperty::GetValueMultiplier(float delta) const
	{
		return Math::Abs(delta) < 20 ? 0.01f : 0.05f;
	}

	void Vec2FProperty::OnXDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueX(mCommonValue.x + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void Vec2FProperty::OnYDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueY(mCommonValue.y + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void Vec2FProperty::OnKeyReleased(const Input::Key& key)
	{
		if (mXEditBox->IsFocused())
		{
			if (key == VK_UP)
			{
				SetXValueByUser(Math::Ceil(mCommonValue.x + 0.01f));
				mXEditBox->SelectAll();
			}

			if (key == VK_DOWN)
			{
				SetXValueByUser(Math::Floor(mCommonValue.x - 0.01f));
				mXEditBox->SelectAll();
			}
		}

		if (mYEditBox->IsFocused())
		{
			if (key == VK_UP)
			{
				SetYValueByUser(Math::Ceil(mCommonValue.y + 0.01f));
				mYEditBox->SelectAll();
			}

			if (key == VK_DOWN)
			{
				SetYValueByUser(Math::Floor(mCommonValue.y - 0.01f));
				mYEditBox->SelectAll();
			}
		}
	}

	void Vec2FProperty::OnXMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void Vec2FProperty::OnYMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void Vec2FProperty::OnXMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void Vec2FProperty::OnYMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void Vec2FProperty::SetXValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueX(value);
		CheckValueChangeCompleted();
	}

	void Vec2FProperty::SetYValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueY(value);
		CheckValueChangeCompleted();
	}

	void Vec2FProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesProxies)
		{
			data.Add(DataNode());
			data.Last() = GetProxy<Vec2F>(ptr.first);
		}
	}

	void Vec2FProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}
}

DECLARE_CLASS(Editor::Vec2FProperty);
