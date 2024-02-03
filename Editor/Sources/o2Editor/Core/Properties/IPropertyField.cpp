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
		for (auto pair : mValuesProxies)
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
			[](IAbstractValueProxy* x)
			{
				return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(x, nullptr);
			}
		);

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

		if (auto captionWidget = DynamicCast<Label>(FindChild("propertyName")))
			captionWidget->text = text;
		else if (auto captionWidget = DynamicCast<Label>(FindInternalWidget("propertyName")))
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

		if (auto captionWidget = DynamicCast<Label>(FindChild("propertyName")))
			return captionWidget->text;
		else if (auto captionWidget = DynamicCast<Label>(FindInternalWidget("propertyName")))
			return captionWidget->text;
		else
		{
			auto captionLayer = GetLayerDrawable<Text>("caption");
			if (captionLayer)
				return captionLayer->text;
		}

		return WString();
	}

	Button* IPropertyField::GetRemoveButton()
	{
		if (!mRemoveBtn)
		{
			auto buttonContainer = MakeRef<Widget>();
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
			}
		}
		else
		{
			if (mRevertBtn)
				RemoveChild(mRevertBtn);
		}
	}
}#include <memory>

template<typename T>
class Ref
{
public:
	Ref() : m_ptr(nullptr) {}
	Ref(T* ptr) : m_ptr(ptr) {}
	Ref(const Ref<T>& other) : m_ptr(other.m_ptr) {}
	Ref<T>& operator=(const Ref<T>& other) { m_ptr = other.m_ptr; return *this; }
	
	bool operator==(const Ref<T>& other) const { return m_ptr == other.m_ptr; }
	bool operator!=(const Ref<T>& other) const { return !(*this == other); }

	T* operator->() const { return m_ptr; }
	T& operator*() const { return *m_ptr; }

private:
	T* m_ptr;
};

template<typename T>
bool operator==(const Ref<T>& ptr, std::nullptr_t) { return ptr.operator==(Ref<T>(nullptr)); }
template<typename T>
bool operator!=(const Ref<T>& ptr, std::nullptr_t) { return ptr.operator!=(Ref<T>(nullptr)); }
	
template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
	return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
class WeakRef
{
public:
	WeakRef() : m_ptr(nullptr) {}
	WeakRef(T* ptr) : m_ptr(ptr) {}
	WeakRef(const WeakRef<T>& other) : m_ptr(other.m_ptr) {}
	WeakRef<T>& operator=(const WeakRef<T>& other) { m_ptr = other.m_ptr; return *this; }
	
	bool operator==(const WeakRef<T>& other) const { return m_ptr == other.m_ptr; }
	bool operator!=(const WeakRef<T>& other) const { return !(*this == other); }

	T* operator->() const { return m_ptr; }
	T& operator*() const { return *m_ptr; }

private:
	T* m_ptr;
};

template<typename T>
bool operator==(const WeakRef<T>& ptr, std::nullptr_t) { return ptr.operator==(WeakRef<T>(nullptr)); }
template<typename T>
bool operator!=(const WeakRef<T>& ptr, std::nullptr_t) { return ptr.operator!=(WeakRef<T>(nullptr)); }

template<typename Derived, typename Base>
Ref<Derived> DynamicCast(const Ref<Base>& ptr)
{
	return std::dynamic_pointer_cast<Derived>(ptr);
}

class String
{
	// implementation
};

template<typename T>
class AnimationClip
{
	// implementation
};

template<typename T>
class AnimationTrack
{
	// implementation
};

using Actor = Ref<Actor>;

class IPropertyField
{
public:
	IPropertyField() : onChanged(nullptr) {}
	virtual ~IPropertyField() {}

	virtual void OnValueChanged();
	virtual void BeginUserChanging();
	virtual void EndUserChanging();

	void CheckRevertableState();
	void StoreValues();
	void CheckValueChangeCompleted();
	void AddChild(const Ref<Actor>& actor);
	Ref<Actor> GetParent() const;
};

class Button : public Actor
{
public:
	Button() : onClick(nullptr) {}

	void Click()
	{
		if (onClick)
			onClick();
	}

	std::function<void()> onClick;
};

class AnimationPropertyField : public IPropertyField
{
public:
	AnimationPropertyField() : mRevertBtn(nullptr) {}

	void SetRevertableState(bool isRevertable)
	{
		if (isRevertable)
		{
			if (!mRevertBtn)
			{
				mRevertBtn = mmake<Button>();
				AddChild(mRevertBtn);
			}

			String path;
			Ref<Actor> itActor = mRevertBtn;
			while (itActor != this)
			{
				path = "child/" + itActor->GetName() + "/" + path;
				itActor = itActor->GetParent();
			}

			auto revertStateAnim = mmake<AnimationClip<float>>();
			revertStateAnim->EaseInOut(path + "layout/maxWidth", 0.0f, 20.0f, 0.15f);
			*revertStateAnim->AddTrack<bool>(path + "enabled") = AnimationTrack<bool>::EaseInOut(false, true, 0.15f);
			AddState("revert", revertStateAnim);
		}

		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);
	}

	void OnValueChanged() override
	{
		CheckRevertableState();
		onChanged(this);

		if (SceneEditScreen::IsSingletonInitialzed())
			o2EditorSceneScreen.OnSceneChanged();
	}

	void BeginUserChanging() override
	{
		StoreValues(mBeforeChangeValues);
	}

	void EndUserChanging() override
	{
		CheckValueChangeCompleted();
	}

private:
	Ref<Button> mRevertBtn;
};

DECLARTE_CLASS(ref<IPropertyField>, Editor::IPropertyField);
