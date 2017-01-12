	#include "Vector2FloatProperty.h"

	#include "Application/Application.h"
	#include "SceneWindow/SceneEditScreen.h"
	#include "UI/EditBox.h"
	#include "UI/HorizontalLayout.h"
	#include "UI/UIManager.h"

	namespace Editor
	{
		Vec2FProperty::Vec2FProperty(UIWidget* widget /*= nullptr*/)
		{
			if (widget)
				mWidget = widget;
			else
				mWidget = o2UI.CreateWidget<UIWidget>("vector2 property");

			mXEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("x edit"));
			mYEditBox = dynamic_cast<UIEditBox*>(mWidget->GetChild("y edit"));

			mXEditBox->onChangeCompleted = Function<void(const WString&)>(this, &Vec2FProperty::OnXEdited);
			mXEditBox->text = "--";
			mXEditBox->SetFilterFloat();

			if (auto xHandleLayer = mXEditBox->GetLayer("arrows"))
			{
				mXEditBox->onDraw += [&]() { mXDragHangle.OnDrawn(); };

				mXDragHangle.cursorType = CursorType::SizeNS;
				mXDragHangle.isUnderPoint = [=](const Vec2F& point) { return xHandleLayer->IsUnderPoint(point); };
				mXDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &Vec2FProperty::OnXDragHandleMoved);
				mXDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
				mXDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
			}

			mYEditBox->onChangeCompleted = Function<void(const WString&)>(this, &Vec2FProperty::OnYEdited);
			mYEditBox->text = "--";
			mYEditBox->SetFilterFloat();

			if (auto yHandleLayer = mYEditBox->GetLayer("arrows"))
			{
				mYEditBox->onDraw += [&]() { mYDragHangle.OnDrawn(); };

				mYDragHangle.cursorType = CursorType::SizeNS;
				mYDragHangle.isUnderPoint = [=](const Vec2F& point) { return yHandleLayer->IsUnderPoint(point); };
				mYDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &Vec2FProperty::OnYDragHandleMoved);
				mYDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
				mYDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
			}
		}

		Vec2FProperty::~Vec2FProperty()
		{
			delete mWidget;
		}

		void Vec2FProperty::SetValue(const Vec2F& value)
		{
			for (auto ptr : mValuesPointers)
				mAssignFunc(ptr, value);

			SetCommonValue(value);
		}

		void Vec2FProperty::SetValueX(float value)
		{
			for (auto ptr : mValuesPointers)
				mXAssignFunc(ptr, value);

			SetCommonValueX(value);
		}

		void Vec2FProperty::SetValueY(float value)
		{
			for (auto ptr : mValuesPointers)
				mYAssignFunc(ptr, value);

			SetCommonValueY(value);
		}

		void Vec2FProperty::SetUnknownValue(const Vec2F& defaultValue /*= Vec2F()*/)
		{
			mXValuesDifferent = true;
			mYValuesDifferent = true;

			mCommonValue = defaultValue;

			mXEditBox->text = "--";
			mYEditBox->text = "--";

			onChanged();
			o2EditorSceneScreen.OnSceneChanged();
		}

		void Vec2FProperty::SetXUnknownValue(float defaultValue /*= 0.0f*/)
		{
			mXValuesDifferent = true;
			mCommonValue.x = defaultValue;
			mXEditBox->text = "--";

			onChanged();
			o2EditorSceneScreen.OnSceneChanged();
		}

		void Vec2FProperty::SetYUnknownValue(float defaultValue /*= 0.0f*/)
		{
			mYValuesDifferent = true;
			mCommonValue.y = defaultValue;
			mYEditBox->text = "--";

			onChanged();
			o2EditorSceneScreen.OnSceneChanged();
		}

		void Vec2FProperty::Setup(const Vector<void*>& targets, bool isProperty)
		{
			if (isProperty)
			{
				mAssignFunc = [](void* ptr, const Vec2F& value) { *((Property<Vec2F>*)(ptr)) = value; };
				mGetFunc = [](void* ptr) { return ((Property<Vec2F>*)(ptr))->Get(); };

				mXAssignFunc = [](void* ptr, float value) { auto p = ((Property<Vec2F>*)(ptr)); Vec2F v = p->Get(); v.x = value; p->Set(v); };
				mXGetFunc = [](void* ptr) { return ((Property<Vec2F>*)(ptr))->Get().x; };

				mYAssignFunc = [](void* ptr, float value) { auto p = ((Property<Vec2F>*)(ptr)); Vec2F v = p->Get(); v.y = value; p->Set(v); };
				mYGetFunc = [](void* ptr) { return ((Property<Vec2F>*)(ptr))->Get().y; };
			}
			else
			{
				mAssignFunc = [](void* ptr, const Vec2F& value) { *((Vec2F*)(ptr)) = value; };
				mGetFunc = [](void* ptr) { return *((Vec2F*)(ptr)); };

				mXAssignFunc = [](void* ptr, float value) { ((Vec2F*)(ptr))->x = value; };
				mXGetFunc = [](void* ptr) { return ((Vec2F*)(ptr))->x; };

				mYAssignFunc = [](void* ptr, float value) { ((Vec2F*)(ptr))->y = value; };
				mYGetFunc = [](void* ptr) { return ((Vec2F*)(ptr))->y; };
			}

			mValuesPointers = targets;

			Refresh();
		}

		void Vec2FProperty::Refresh()
		{
			if (mValuesPointers.IsEmpty())
				return;

			auto lastCommonValue = mCommonValue;
			auto lastXDifferent = mXValuesDifferent;
			auto lastYDifferent = mYValuesDifferent;

			auto newCommonValue = mGetFunc(mValuesPointers[0]);
			auto newXValuesDifferent = false;
			auto newYValuesDifferent = false;

			for (int i = 1; i < mValuesPointers.Count(); i++)
			{
				auto value = mGetFunc(mValuesPointers[i]);
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
		}

		UIWidget* Vec2FProperty::GetWidget() const
		{
			return mWidget;
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

			onChanged();
			o2EditorSceneScreen.OnSceneChanged();
		}

		void Vec2FProperty::SetCommonValueX(float value)
		{
			mXValuesDifferent = false;
			mCommonValue.x = value;
			mXEditBox->text = (WString)value;

			onChanged();
			o2EditorSceneScreen.OnSceneChanged();
		}

		void Vec2FProperty::SetCommonValueY(float value)
		{
			mYValuesDifferent = false;
			mCommonValue.y = value;
			mYEditBox->text = (WString)value;

			onChanged();
			o2EditorSceneScreen.OnSceneChanged();
		}

		void Vec2FProperty::OnXEdited(const WString& data)
		{
			if (mXValuesDifferent && data == "--")
				return;

			SetValueX((const float)data);
		}

		void Vec2FProperty::OnYEdited(const WString& data)
		{
			if (mYValuesDifferent && data == "--")
				return;

			SetValueY((const float)data);
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
					SetValueX(Math::Ceil(mCommonValue.x + 0.01f));
					mXEditBox->SelectAll();
				}

				if (key == VK_DOWN)
				{
					SetValueX(Math::Floor(mCommonValue.x - 0.01f));
					mXEditBox->SelectAll();
				}
			}

			if (mYEditBox->IsFocused())
			{
				if (key == VK_UP)
				{
					SetValueY(Math::Ceil(mCommonValue.y + 0.01f));
					mYEditBox->SelectAll();
				}

				if (key == VK_DOWN)
				{
					SetValueY(Math::Floor(mCommonValue.y - 0.01f));
					mYEditBox->SelectAll();
				}
			}
		}

	}
 
CLASS_META(Editor::Vec2FProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::KeyboardEventsListener);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mXAssignFunc);
	PROTECTED_FIELD(mXGetFunc);
	PROTECTED_FIELD(mYAssignFunc);
	PROTECTED_FIELD(mYGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mXValuesDifferent);
	PROTECTED_FIELD(mYValuesDifferent);
	PROTECTED_FIELD(mWidget);
	PROTECTED_FIELD(mXEditBox);
	PROTECTED_FIELD(mYEditBox);
	PROTECTED_FIELD(mXDragHangle);
	PROTECTED_FIELD(mYDragHangle);

	PUBLIC_FUNCTION(void, Setup, const Vector<void*>&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SetValue, const Vec2F&);
	PUBLIC_FUNCTION(void, SetValueX, float);
	PUBLIC_FUNCTION(void, SetValueY, float);
	PUBLIC_FUNCTION(void, SetUnknownValue, const Vec2F&);
	PUBLIC_FUNCTION(void, SetXUnknownValue, float);
	PUBLIC_FUNCTION(void, SetYUnknownValue, float);
	PUBLIC_FUNCTION(Vec2F, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const Vec2F&);
	PROTECTED_FUNCTION(void, SetCommonValueX, float);
	PROTECTED_FUNCTION(void, SetCommonValueY, float);
	PROTECTED_FUNCTION(void, OnXEdited, const WString&);
	PROTECTED_FUNCTION(void, OnYEdited, const WString&);
	PROTECTED_FUNCTION(float, GetValueMultiplier, float);
	PROTECTED_FUNCTION(void, OnXDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnYDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
}
END_META;
 