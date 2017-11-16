#include "Vector2IntProperty.h"

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
	Vec2IProperty::Vec2IProperty(UIWidget* widget /*= nullptr*/)
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

		if (auto xHandleLayer = mXEditBox->GetLayer("arrows"))
		{
			mXEditBox->onDraw += [&]() { mXDragHangle.OnDrawn(); };

			mXDragHangle.cursorType = CursorType::SizeNS;
			mXDragHangle.isUnderPoint = [=](const Vec2F& point) { return xHandleLayer->IsUnderPoint(point); };
			mXDragHangle.onMoved = THIS_FUNC(OnXDragHandleMoved);
			mXDragHangle.onCursorPressed = THIS_FUNC(OnXMoveHandlePressed);
			mXDragHangle.onCursorReleased = THIS_FUNC(OnXMoveHandleReleased);
		}

		mYEditBox->onChangeCompleted = THIS_FUNC(OnYEdited);
		mYEditBox->text = "--";
		mYEditBox->SetFilterFloat();

		if (auto yHandleLayer = mYEditBox->GetLayer("arrows"))
		{
			mYEditBox->onDraw += [&]() { mYDragHangle.OnDrawn(); };

			mYDragHangle.cursorType = CursorType::SizeNS;
			mYDragHangle.isUnderPoint = [=](const Vec2F& point) { return yHandleLayer->IsUnderPoint(point); };
			mYDragHangle.onMoved = THIS_FUNC(OnYDragHandleMoved);
			mYDragHangle.onCursorPressed = THIS_FUNC(OnYMoveHandlePressed);
			mYDragHangle.onCursorReleased = THIS_FUNC(OnYMoveHandleReleased);
		}
	}

	Vec2IProperty::~Vec2IProperty()
	{
		delete mPropertyWidget;
	}

	void Vec2IProperty::SetValue(const Vec2I& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void Vec2IProperty::SetValueX(int value)
	{
		for (auto ptr : mValuesPointers)
			mXAssignFunc(ptr.first, value);

		SetCommonValueX(value);
	}

	void Vec2IProperty::SetValueY(int value)
	{
		for (auto ptr : mValuesPointers)
			mYAssignFunc(ptr.first, value);

		SetCommonValueY(value);
	}

	void Vec2IProperty::SetUnknownValue(const Vec2I& defaultValue /*= Vec2I()*/)
	{
		mXValuesDifferent = true;
		mYValuesDifferent = true;

		mCommonValue = defaultValue;

		mXEditBox->text = "--";
		mYEditBox->text = "--";

		OnChanged();
	}

	void Vec2IProperty::SetXUnknownValue(int defaultValue /*= 0*/)
	{
		mXValuesDifferent = true;
		mCommonValue.x = defaultValue;
		mXEditBox->text = "--";

		OnChanged();
	}

	void Vec2IProperty::SetYUnknownValue(int defaultValue /*= 0*/)
	{
		mYValuesDifferent = true;
		mCommonValue.y = defaultValue;
		mYEditBox->text = "--";

		OnChanged();
	}

	void Vec2IProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const Vec2I& value) { *((Property<Vec2I>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<Vec2I>*)(ptr))->Get(); };

			mXAssignFunc = [](void* ptr, int value) { auto p = ((Property<Vec2I>*)(ptr)); Vec2I v = p->Get(); v.x = value; p->Set(v); };
			mXGetFunc = [](void* ptr) { return ((Property<Vec2I>*)(ptr))->Get().x; };

			mYAssignFunc = [](void* ptr, int value) { auto p = ((Property<Vec2I>*)(ptr)); Vec2I v = p->Get(); v.y = value; p->Set(v); };
			mYGetFunc = [](void* ptr) { return ((Property<Vec2I>*)(ptr))->Get().y; };
		}
		else
		{
			mAssignFunc = [](void* ptr, const Vec2I& value) { *((Vec2I*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Vec2I*)(ptr)); };

			mXAssignFunc = [](void* ptr, int value) { ((Vec2I*)(ptr))->x = value; };
			mXGetFunc = [](void* ptr) { return ((Vec2I*)(ptr))->x; };

			mYAssignFunc = [](void* ptr, int value) { ((Vec2I*)(ptr))->y = value; };
			mYGetFunc = [](void* ptr) { return ((Vec2I*)(ptr))->y; };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void Vec2IProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastXDifferent = mXValuesDifferent;
		auto lastYDifferent = mYValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0].first);
		auto newXValuesDifferent = false;
		auto newYValuesDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			auto value = mGetFunc(mValuesPointers[i].first);
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

	void Vec2IProperty::Revert()
	{
		for (auto ptr : mValuesPointers)
		{
			if (ptr.second)
			{
				mAssignFunc(ptr.first, mGetFunc(ptr.second));
			}
		}

		Refresh();
	}

	UIWidget* Vec2IProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	Vec2I Vec2IProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool Vec2IProperty::IsValuesDifferent() const
	{
		return mXValuesDifferent || mYValuesDifferent;
	}

	const Type* Vec2IProperty::GetFieldType() const
	{
		return &TypeOf(Vec2I);
	}

	void Vec2IProperty::SetCommonValue(const Vec2I& value)
	{
		mXValuesDifferent = false;
		mYValuesDifferent = false;

		mCommonValue = value;

		mXEditBox->text = (WString)mCommonValue.x;
		mYEditBox->text = (WString)mCommonValue.y;

		OnChanged();
	}

	void Vec2IProperty::SetCommonValueX(int value)
	{
		mXValuesDifferent = false;
		mCommonValue.x = value;
		mXEditBox->text = (WString)value;

		OnChanged();
	}

	void Vec2IProperty::SetCommonValueY(int value)
	{
		mYValuesDifferent = false;
		mCommonValue.y = value;
		mYEditBox->text = (WString)value;

		OnChanged();
	}

	void Vec2IProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesPointers)
		{
			if (ptr.second && !Math::Equals(mGetFunc(ptr.first), mGetFunc(ptr.second)))
			{
				revertable = true;
				break;
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
	}

	void Vec2IProperty::OnXEdited(const WString& data)
	{
		if (mXValuesDifferent && data == "--")
			return;

		SetXValueByUser((const int)data);
	}

	void Vec2IProperty::OnYEdited(const WString& data)
	{
		if (mYValuesDifferent && data == "--")
			return;

		SetYValueByUser((const int)data);
	}

	void Vec2IProperty::OnKeyReleased(const Input::Key& key)
	{
		if (mXEditBox->IsFocused())
		{
			if (key == VK_UP)
			{
				SetXValueByUser(mCommonValue.x + 1);
				mXEditBox->SelectAll();
			}

			if (key == VK_DOWN)
			{
				SetXValueByUser(mCommonValue.x - 1);
				mXEditBox->SelectAll();
			}
		}

		if (mYEditBox->IsFocused())
		{
			if (key == VK_UP)
			{
				SetYValueByUser(mCommonValue.y + 1);
				mYEditBox->SelectAll();
			}

			if (key == VK_DOWN)
			{
				SetYValueByUser(mCommonValue.y - 1);
				mYEditBox->SelectAll();
			}
		}
	}

	void Vec2IProperty::OnXDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueX(mCommonValue.x + (int)cursor.delta.y);
	}

	void Vec2IProperty::OnYDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueY(mCommonValue.y + (int)cursor.delta.y);
	}

	void Vec2IProperty::OnXMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void Vec2IProperty::OnYMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void Vec2IProperty::OnXMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void Vec2IProperty::OnYMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void Vec2IProperty::SetXValueByUser(int value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueX(value);
		CheckValueChangeCompleted();
	}

	void Vec2IProperty::SetYValueByUser(int value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueY(value);
		CheckValueChangeCompleted();
	}

	void Vec2IProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

	void Vec2IProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

}

DECLARE_CLASS(Editor::Vec2IProperty);
