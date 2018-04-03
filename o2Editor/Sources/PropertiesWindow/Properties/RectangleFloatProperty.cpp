#include "stdafx.h"
#include "RectangleFloatProperty.h"

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
	RectFProperty::RectFProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("rectangle property");

		mLeftEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget->GetChild("layout/left edit"));
		mBottomEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget->GetChild("layout/bottom edit"));
		mRightEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget->GetChild("layout/right edit"));
		mTopEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget->GetChild("layout/top edit"));

		mRevertBtn = mPropertyWidget->GetChildByType<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);

		mLeftEditBox->onChangeCompleted = THIS_FUNC(OnLeftEdited);
		mLeftEditBox->text = "--";
		mLeftEditBox->SetFilterFloat();

		if (auto leftHandleLayer = mLeftEditBox->FindLayer("arrows"))
		{
			mLeftEditBox->onDraw += [&]() { mLeftDragHangle.OnDrawn(); };

			mLeftDragHangle.cursorType = CursorType::SizeNS;
			mLeftDragHangle.isUnderPoint = [=](const Vec2F& point) { return leftHandleLayer->IsUnderPoint(point); };
			mLeftDragHangle.onMoved = THIS_FUNC(OnLeftDragHandleMoved);
			mLeftDragHangle.onCursorPressed = THIS_FUNC(OnLeftMoveHandlePressed);
			mLeftDragHangle.onCursorReleased = THIS_FUNC(OnLeftMoveHandleReleased);
		}

		mBottomEditBox->onChangeCompleted = THIS_FUNC(OnBottomEdited);
		mBottomEditBox->text = "--";
		mBottomEditBox->SetFilterFloat();

		if (auto bottomHandleLayer = mBottomEditBox->FindLayer("arrows"))
		{
			mBottomEditBox->onDraw += [&]() { mBottomDragHangle.OnDrawn(); };

			mBottomDragHangle.cursorType = CursorType::SizeNS;
			mBottomDragHangle.isUnderPoint = [=](const Vec2F& point) { return bottomHandleLayer->IsUnderPoint(point); };
			mBottomDragHangle.onMoved = THIS_FUNC(OnBottomDragHandleMoved);
			mBottomDragHangle.onCursorPressed = THIS_FUNC(OnBottomMoveHandlePressed);
			mBottomDragHangle.onCursorReleased = THIS_FUNC(OnBottomMoveHandleReleased);
		}

		mRightEditBox->onChangeCompleted = THIS_FUNC(OnRightEdited);
		mRightEditBox->text = "--";
		mRightEditBox->SetFilterFloat();

		if (auto rightHandleLayer = mRightEditBox->FindLayer("arrows"))
		{
			mRightEditBox->onDraw += [&]() { mRightDragHangle.OnDrawn(); };

			mRightDragHangle.cursorType = CursorType::SizeNS;
			mRightDragHangle.isUnderPoint = [=](const Vec2F& point) { return rightHandleLayer->IsUnderPoint(point); };
			mRightDragHangle.onMoved = THIS_FUNC(OnRightDragHandleMoved);
			mRightDragHangle.onCursorPressed = THIS_FUNC(OnRightMoveHandlePressed);
			mRightDragHangle.onCursorReleased = THIS_FUNC(OnRightMoveHandleReleased);
		}

		mTopEditBox->onChangeCompleted = THIS_FUNC(OnTopEdited);
		mTopEditBox->text = "--";
		mTopEditBox->SetFilterFloat();

		if (auto topHandleLayer = mTopEditBox->FindLayer("arrows"))
		{
			mTopEditBox->onDraw += [&]() { mTopDragHangle.OnDrawn(); };

			mTopDragHangle.cursorType = CursorType::SizeNS;
			mTopDragHangle.isUnderPoint = [=](const Vec2F& point) { return topHandleLayer->IsUnderPoint(point); };
			mTopDragHangle.onMoved = THIS_FUNC(OnTopDragHandleMoved);
			mTopDragHangle.onCursorPressed = THIS_FUNC(OnTopMoveHandlePressed);
			mTopDragHangle.onCursorReleased = THIS_FUNC(OnTopMoveHandleReleased);
		}
	}

	RectFProperty::~RectFProperty()
	{
		delete mPropertyWidget;
	}

	void RectFProperty::SetValue(const RectF& value)
	{
		for (auto ptr : mValuesProxies)
			SetProxy<RectF>(ptr.first, value);

		SetCommonValue(value);
	}

	void RectFProperty::SetValueLeft(float value)
	{
		for (auto ptr : mValuesProxies)
		{
			RectF newValue = GetProxy<RectF>(ptr.first);
			newValue.left = value;
			SetProxy<RectF>(ptr.first, newValue);
		}

		SetCommonValueLeft(value);
	}

	void RectFProperty::SetValueRight(float value)
	{
		for (auto ptr : mValuesProxies)
		{
			RectF newValue = GetProxy<RectF>(ptr.first);
			newValue.right = value;
			SetProxy<RectF>(ptr.first, newValue);
		}

		SetCommonValueRight(value);
	}

	void RectFProperty::SetValueTop(float value)
	{
		for (auto ptr : mValuesProxies)
		{
			RectF newValue = GetProxy<RectF>(ptr.first);
			newValue.top = value;
			SetProxy<RectF>(ptr.first, newValue);
		}

		SetCommonValueTop(value);
	}

	void RectFProperty::SetValueBottom(float value)
	{
		for (auto ptr : mValuesProxies)
		{
			RectF newValue = GetProxy<RectF>(ptr.first);
			newValue.bottom = value;
			SetProxy<RectF>(ptr.first, newValue);
		}

		SetCommonValueBottom(value);
	}

	void RectFProperty::SetUnknownValue(const RectF& defaultValue /*= RectF()*/)
	{
		mLeftValuesDifferent = true;
		mBottomValuesDifferent = true;
		mRightValuesDifferent = true;
		mTopValuesDifferent = true;

		mCommonValue = defaultValue;

		mLeftEditBox->text = "--";
		mBottomEditBox->text = "--";
		mRightEditBox->text = "--";
		mTopEditBox->text = "--";

		OnChanged();
	}

	void RectFProperty::SetLeftUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mLeftValuesDifferent = true;
		mCommonValue.left = defaultValue;
		mLeftEditBox->text = "--";

		OnChanged();
	}

	void RectFProperty::SetRightUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mRightValuesDifferent = true;
		mCommonValue.right = defaultValue;
		mRightEditBox->text = "--";

		OnChanged();
	}

	void RectFProperty::SetTopUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mTopValuesDifferent = true;
		mCommonValue.top = defaultValue;
		mTopEditBox->text = "--";

		OnChanged();
	}

	void RectFProperty::SetBottomUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mBottomValuesDifferent = true;
		mCommonValue.bottom = defaultValue;
		mBottomEditBox->text = "--";

		OnChanged();
	}

	void RectFProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;

		Refresh();
	}

	void RectFProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastLeftDifferent = mLeftValuesDifferent;
		auto lastRightDifferent = mRightValuesDifferent;
		auto lastTopDifferent = mTopValuesDifferent;
		auto lastBottomDifferent = mBottomValuesDifferent;

		auto newCommonValue = GetProxy<RectF>(mValuesProxies[0].first);
		auto newLeftValuesDifferent = false;
		auto newBottomValuesDifferent = false;
		auto newRightValuesDifferent = false;
		auto newTopValuesDifferent = false;

		for (int i = 1; i < mValuesProxies.Count(); i++)
		{
			auto value = GetProxy<RectF>(mValuesProxies[i].first);
			if (!Math::Equals(newCommonValue.left, value.left))
				newLeftValuesDifferent = true;

			if (!Math::Equals(newCommonValue.bottom, value.bottom))
				newBottomValuesDifferent = true;

			if (!Math::Equals(newCommonValue.right, value.right))
				newRightValuesDifferent = true;

			if (!Math::Equals(newCommonValue.top, value.top))
				newTopValuesDifferent = true;
		}

		if (newLeftValuesDifferent)
		{
			if (!lastLeftDifferent)
				SetLeftUnknownValue(newCommonValue.left);
		}
		else if (!Math::Equals(lastCommonValue.left, newCommonValue.left) || lastLeftDifferent)
			SetCommonValueLeft(newCommonValue.left);

		if (newRightValuesDifferent)
		{
			if (!lastRightDifferent)
				SetRightUnknownValue(newCommonValue.right);
		}
		else if (!Math::Equals(lastCommonValue.right, newCommonValue.right) || lastRightDifferent)
			SetCommonValueRight(newCommonValue.right);

		if (newTopValuesDifferent)
		{
			if (!lastTopDifferent)
				SetTopUnknownValue(newCommonValue.top);
		}
		else if (!Math::Equals(lastCommonValue.top, newCommonValue.top) || lastTopDifferent)
			SetCommonValueTop(newCommonValue.top);

		if (newBottomValuesDifferent)
		{
			if (!lastBottomDifferent)
				SetBottomUnknownValue(newCommonValue.bottom);
		}
		else if (!Math::Equals(lastCommonValue.bottom, newCommonValue.bottom) || lastBottomDifferent)
			SetCommonValueBottom(newCommonValue.bottom);

		CheckRevertableState();
	}

	void RectFProperty::Revert()
	{
		for (auto ptr : mValuesProxies)
		{
			if (ptr.second)
			{
				SetProxy<RectF>(ptr.first, GetProxy<RectF>(ptr.second));
			}
		}

		Refresh();
	}

	UIWidget* RectFProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	RectF RectFProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool RectFProperty::IsValuesDifferent() const
	{
		return mLeftValuesDifferent || mBottomValuesDifferent || mRightValuesDifferent || mTopValuesDifferent;
	}

	const Type* RectFProperty::GetFieldType() const
	{
		return &TypeOf(RectF);
	}

	void RectFProperty::SetCommonValue(const RectF& value)
	{
		mLeftValuesDifferent = false;
		mBottomValuesDifferent = false;
		mRightValuesDifferent = false;
		mTopValuesDifferent = false;

		mCommonValue = value;

		mLeftEditBox->text = (WString)mCommonValue.left;
		mBottomEditBox->text = (WString)mCommonValue.bottom;
		mRightEditBox->text = (WString)mCommonValue.right;
		mTopEditBox->text = (WString)mCommonValue.top;

		OnChanged();
	}

	void RectFProperty::SetCommonValueLeft(float value)
	{
		mLeftValuesDifferent = false;
		mCommonValue.left = value;
		mLeftEditBox->text = (WString)value;

		OnChanged();
	}

	void RectFProperty::SetCommonValueRight(float value)
	{
		mRightValuesDifferent = false;
		mCommonValue.right = value;
		mRightEditBox->text = (WString)value;

		OnChanged();
	}

	void RectFProperty::SetCommonValueTop(float value)
	{
		mTopValuesDifferent = false;
		mCommonValue.top = value;
		mTopEditBox->text = (WString)value;

		OnChanged();
	}

	void RectFProperty::SetCommonValueBottom(float value)
	{
		mBottomValuesDifferent = false;
		mCommonValue.bottom = value;
		mBottomEditBox->text = (WString)value;

		OnChanged();
	}

	void RectFProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesProxies)
		{
			if (ptr.second && !Math::Equals(GetProxy<RectF>(ptr.first), GetProxy<RectF>(ptr.second)))
			{
				revertable = true;
				break;
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
	}

	void RectFProperty::OnLeftEdited(const WString& data)
	{
		if (mLeftValuesDifferent && data == "--")
			return;

		SetLeftValueByUser((const float)data);
	}

	void RectFProperty::OnBottomEdited(const WString& data)
	{
		if (mBottomValuesDifferent && data == "--")
			return;

		SetBottomValueByUser((const float)data);
	}

	void RectFProperty::OnRightEdited(const WString& data)
	{
		if (mRightValuesDifferent && data == "--")
			return;

		SetRightValueByUser((const float)data);
	}

	void RectFProperty::OnTopEdited(const WString& data)
	{
		if (mTopValuesDifferent && data == "--")
			return;

		SetTopValueByUser((const float)data);
	}

	float RectFProperty::GetValueMultiplier(float delta) const
	{
		return Math::Abs(delta) < 20 ? 0.01f : 0.05f;
	}

	void RectFProperty::OnLeftDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueLeft(mCommonValue.left + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void RectFProperty::OnRightDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueRight(mCommonValue.right + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void RectFProperty::OnTopDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueTop(mCommonValue.top + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void RectFProperty::OnBottomDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueBottom(mCommonValue.bottom + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void RectFProperty::OnKeyReleased(const Input::Key& key)
	{
		auto func =[&](UIEditBox* editbox, float value, void(RectFProperty::*setter)(float))
		{
			if (editbox->IsFocused())
			{
				if (key == VK_UP)
				{
					(this->*setter)(Math::Ceil(value + 0.01f));
					editbox->SelectAll();
				}

				if (key == VK_DOWN)
				{
					(this->*setter)(Math::Floor(value - 0.01f));
					editbox->SelectAll();
				}
			}
		};

		func(mLeftEditBox, mCommonValue.left, &RectFProperty::SetLeftValueByUser);
		func(mRightEditBox, mCommonValue.right, &RectFProperty::SetRightValueByUser);
		func(mTopEditBox, mCommonValue.top, &RectFProperty::SetTopValueByUser);
		func(mBottomEditBox, mCommonValue.bottom, &RectFProperty::SetBottomValueByUser);
	}

	void RectFProperty::OnLeftMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void RectFProperty::OnLeftMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void RectFProperty::OnRightMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void RectFProperty::OnRightMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void RectFProperty::OnTopMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void RectFProperty::OnTopMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void RectFProperty::OnBottomMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void RectFProperty::OnBottomMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void RectFProperty::SetLeftValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueLeft(value);
		CheckValueChangeCompleted();
	}

	void RectFProperty::SetRightValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueRight(value);
		CheckValueChangeCompleted();
	}

	void RectFProperty::SetBottomValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueBottom(value);
		CheckValueChangeCompleted();
	}

	void RectFProperty::SetTopValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueTop(value);
		CheckValueChangeCompleted();
	}

	void RectFProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void RectFProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesProxies)
		{
			data.Add(DataNode());
			data.Last() = GetProxy<RectF>(ptr.first);
		}
	}

}

DECLARE_CLASS(Editor::RectFProperty);
