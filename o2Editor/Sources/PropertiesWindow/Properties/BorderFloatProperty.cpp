#include "BorderFloatProperty.h"

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
	BorderFProperty::BorderFProperty(UIWidget* widget /*= nullptr*/)
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

	BorderFProperty::~BorderFProperty()
	{
		delete mPropertyWidget;
	}

	void BorderFProperty::SetValue(const BorderF& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void BorderFProperty::SetValueLeft(float value)
	{
		for (auto ptr : mValuesPointers)
			mLeftAssignFunc(ptr.first, value);

		SetCommonValueLeft(value);
	}

	void BorderFProperty::SetValueRight(float value)
	{
		for (auto ptr : mValuesPointers)
			mRightAssignFunc(ptr.first, value);

		SetCommonValueRight(value);
	}

	void BorderFProperty::SetValueTop(float value)
	{
		for (auto ptr : mValuesPointers)
			mTopAssignFunc(ptr.first, value);

		SetCommonValueTop(value);
	}

	void BorderFProperty::SetValueBottom(float value)
	{
		for (auto ptr : mValuesPointers)
			mBottomAssignFunc(ptr.first, value);

		SetCommonValueBottom(value);
	}

	void BorderFProperty::SetUnknownValue(const BorderF& defaultValue /*= BorderF()*/)
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

	void BorderFProperty::SetLeftUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mLeftValuesDifferent = true;
		mCommonValue.left = defaultValue;
		mLeftEditBox->text = "--";

		OnChanged();
	}

	void BorderFProperty::SetRightUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mRightValuesDifferent = true;
		mCommonValue.right = defaultValue;
		mRightEditBox->text = "--";

		OnChanged();
	}

	void BorderFProperty::SetTopUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mTopValuesDifferent = true;
		mCommonValue.top = defaultValue;
		mTopEditBox->text = "--";

		OnChanged();
	}

	void BorderFProperty::SetBottomUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mBottomValuesDifferent = true;
		mCommonValue.bottom = defaultValue;
		mBottomEditBox->text = "--";

		OnChanged();
	}

	void BorderFProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const BorderF& value) { *((Property<BorderF>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<BorderF>*)(ptr))->Get(); };

			mLeftAssignFunc = [](void* ptr, float value) { auto p = ((Property<BorderF>*)(ptr)); BorderF v = p->Get(); v.left = value; p->Set(v); };
			mLeftGetFunc = [](void* ptr) { return ((Property<BorderF>*)(ptr))->Get().left; };

			mRightAssignFunc = [](void* ptr, float value) { auto p = ((Property<BorderF>*)(ptr)); BorderF v = p->Get(); v.right = value; p->Set(v); };
			mRightGetFunc = [](void* ptr) { return ((Property<BorderF>*)(ptr))->Get().right; };

			mTopAssignFunc = [](void* ptr, float value) { auto p = ((Property<BorderF>*)(ptr)); BorderF v = p->Get(); v.top = value; p->Set(v); };
			mTopGetFunc = [](void* ptr) { return ((Property<BorderF>*)(ptr))->Get().top; };

			mBottomAssignFunc = [](void* ptr, float value) { auto p = ((Property<BorderF>*)(ptr)); BorderF v = p->Get(); v.bottom = value; p->Set(v); };
			mBottomGetFunc = [](void* ptr) { return ((Property<BorderF>*)(ptr))->Get().bottom; };
		}
		else
		{
			mAssignFunc = [](void* ptr, const BorderF& value) { *((BorderF*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((BorderF*)(ptr)); };

			mLeftAssignFunc = [](void* ptr, float value) { ((BorderF*)(ptr))->left = value; };
			mLeftGetFunc = [](void* ptr) { return ((BorderF*)(ptr))->left; };

			mRightAssignFunc = [](void* ptr, float value) { ((BorderF*)(ptr))->right = value; };
			mRightGetFunc = [](void* ptr) { return ((BorderF*)(ptr))->right; };

			mTopAssignFunc = [](void* ptr, float value) { ((BorderF*)(ptr))->top = value; };
			mTopGetFunc = [](void* ptr) { return ((BorderF*)(ptr))->top; };

			mBottomAssignFunc = [](void* ptr, float value) { ((BorderF*)(ptr))->bottom = value; };
			mBottomGetFunc = [](void* ptr) { return ((BorderF*)(ptr))->bottom; };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void BorderFProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastLeftDifferent = mLeftValuesDifferent;
		auto lastRightDifferent = mRightValuesDifferent;
		auto lastTopDifferent = mTopValuesDifferent;
		auto lastBottomDifferent = mBottomValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0].first);
		auto newLeftValuesDifferent = false;
		auto newBottomValuesDifferent = false;
		auto newRightValuesDifferent = false;
		auto newTopValuesDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			auto value = mGetFunc(mValuesPointers[i].first);
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

	void BorderFProperty::Revert()
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

	UIWidget* BorderFProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	BorderF BorderFProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool BorderFProperty::IsValuesDifferent() const
	{
		return mLeftValuesDifferent || mBottomValuesDifferent || mRightValuesDifferent || mTopValuesDifferent;
	}

	const Type* BorderFProperty::GetFieldType() const
	{
		return &TypeOf(BorderF);
	}

	void BorderFProperty::SetCommonValue(const BorderF& value)
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

	void BorderFProperty::SetCommonValueLeft(float value)
	{
		mLeftValuesDifferent = false;
		mCommonValue.left = value;
		mLeftEditBox->text = (WString)value;

		OnChanged();
	}

	void BorderFProperty::SetCommonValueRight(float value)
	{
		mRightValuesDifferent = false;
		mCommonValue.right = value;
		mRightEditBox->text = (WString)value;

		OnChanged();
	}

	void BorderFProperty::SetCommonValueTop(float value)
	{
		mTopValuesDifferent = false;
		mCommonValue.top = value;
		mTopEditBox->text = (WString)value;

		OnChanged();
	}

	void BorderFProperty::SetCommonValueBottom(float value)
	{
		mBottomValuesDifferent = false;
		mCommonValue.bottom = value;
		mBottomEditBox->text = (WString)value;

		OnChanged();
	}

	void BorderFProperty::CheckRevertableState()
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

	void BorderFProperty::OnLeftEdited(const WString& data)
	{
		if (mLeftValuesDifferent && data == "--")
			return;

		SetLeftValueByUser((const float)data);
	}

	void BorderFProperty::OnBottomEdited(const WString& data)
	{
		if (mBottomValuesDifferent && data == "--")
			return;

		SetBottomValueByUser((const float)data);
	}

	void BorderFProperty::OnRightEdited(const WString& data)
	{
		if (mRightValuesDifferent && data == "--")
			return;

		SetRightValueByUser((const float)data);
	}

	void BorderFProperty::OnTopEdited(const WString& data)
	{
		if (mTopValuesDifferent && data == "--")
			return;

		SetTopValueByUser((const float)data);
	}

	float BorderFProperty::GetValueMultiplier(float delta) const
	{
		return Math::Abs(delta) < 20 ? 0.01f : 0.05f;
	}

	void BorderFProperty::OnLeftDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueLeft(mCommonValue.left + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void BorderFProperty::OnRightDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueRight(mCommonValue.right + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void BorderFProperty::OnTopDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueTop(mCommonValue.top + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void BorderFProperty::OnBottomDragHandleMoved(const Input::Cursor& cursor)
	{
		SetValueBottom(mCommonValue.bottom + cursor.delta.y*GetValueMultiplier(cursor.delta.y));
	}

	void BorderFProperty::OnKeyReleased(const Input::Key& key)
	{
		auto func =[&](UIEditBox* editbox, float value, void(BorderFProperty::*setter)(float))
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

		func(mLeftEditBox, mCommonValue.left, &BorderFProperty::SetLeftValueByUser);
		func(mRightEditBox, mCommonValue.right, &BorderFProperty::SetRightValueByUser);
		func(mTopEditBox, mCommonValue.top, &BorderFProperty::SetTopValueByUser);
		func(mBottomEditBox, mCommonValue.bottom, &BorderFProperty::SetBottomValueByUser);
	}

	void BorderFProperty::OnLeftMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void BorderFProperty::OnLeftMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void BorderFProperty::OnRightMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void BorderFProperty::OnRightMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void BorderFProperty::OnTopMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void BorderFProperty::OnTopMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void BorderFProperty::OnBottomMoveHandlePressed(const Input::Cursor& cursor)
	{
		StoreValues(mBeforeChangeValues);
		o2Application.SetCursorInfiniteMode(true);
	}

	void BorderFProperty::OnBottomMoveHandleReleased(const Input::Cursor& cursor)
	{
		o2Application.SetCursorInfiniteMode(false);
		CheckValueChangeCompleted();
	}

	void BorderFProperty::SetLeftValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueLeft(value);
		CheckValueChangeCompleted();
	}

	void BorderFProperty::SetRightValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueRight(value);
		CheckValueChangeCompleted();
	}

	void BorderFProperty::SetBottomValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueBottom(value);
		CheckValueChangeCompleted();
	}

	void BorderFProperty::SetTopValueByUser(float value)
	{
		StoreValues(mBeforeChangeValues);
		SetValueTop(value);
		CheckValueChangeCompleted();
	}

	void BorderFProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void BorderFProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

DECLARE_CLASS(Editor::BorderFProperty);
