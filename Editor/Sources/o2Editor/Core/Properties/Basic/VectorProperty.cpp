```
#include "o2Editor/stdafx.h"
#include "VectorProperty.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/IntegerProperty.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
    VectorProperty::VectorProperty()
    {
        InitializeControls();
    }

    VectorProperty::VectorProperty(const VectorProperty& other):
        IPropertyField(other)
    {
        InitializeControls();
    }

    VectorProperty& VectorProperty::operator=(const VectorProperty& other)
    {
        IPropertyField::operator=(other);
        InitializeControls();
        return *this;
    }

    void VectorProperty::InitializeControls()
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

        mHeaderContainer = Ref<HorizontalLayout>::make();
        *mHeaderContainer.layout = WidgetLayout::HorStretch(VerAlign::Top, 100, 0, 20, 0);
        mHeaderContainer.baseCorner = BaseCorner::Right;
        mHeaderContainer.expandHeight = false;
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

        // Count property
        mCountProperty = mSpoiler->FindChildByType<IntegerProperty>(false);
        if (!mCountProperty)
            mCountProperty = o2UI.CreateWidget<IntegerProperty>();

        if (mCountProperty)
        {
            mHeaderContainer->AddChild(mCountProperty);

            mCountProperty->layout->maxWidth = 40;
            mCountProperty->SetValue(0);
            mCountProperty->onChanged = THIS_FUNC(OnCountChanged);
        }

        // Other
        expandHeight = true;
        expandWidth = true;
        fitByChildren = true;
    }

    VectorProperty::~VectorProperty()
    {
        for (auto x : mValuePropertiesPool)
            delete x;
    }

    void VectorProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
    {
        for (auto& pair : mTargetObjects) {
            if (pair.first.isCreated)
                delete pair.first.data;

            if (pair.second.isCreated)
                delete pair.second.data;
        }

        mTargetObjects.Clear();

        for (auto& pair : targets)
            mTargetObjects.Add({ GetObjectFromProxy(pair.first), GetObjectFromProxy(pair.second) });

        Refresh();
    }

    void VectorProperty::Refresh()
    {
        PushEditorScopeOnStack scope;

        if (mTargetObjects.IsEmpty())
            return;
        
        mIsRefreshing = true;

        if (IsExpanded())
        {
            for (auto& pair : mTargetObjects)
            {
                pair.first.Refresh();
                pair.second.Refresh();
            }
        }

        auto lastDifferent = mCountDifferents;

        mCountOfElements = mVectorType->GetObjectVectorSize(mTargetObjects[0].first.data);
        mCountDifferents = false;

        for (auto target : mTargetObjects)
        {
            int targetCount = mVectorType->GetObjectVectorSize(target.first.data);
            if (targetCount != mCountOfElements)
            {
                mCountDifferents = true;
                break;
            }
        }

        if (mCountDifferents)
        {
            if (!lastDifferent)
            {
                mCountProperty->SetUnknownValue();

                if (IsExpanded())
                {
                    for (auto prop : mValueProperties)
                    {
                        mSpoiler->RemoveChild(prop, false);
                        FreeValueProperty(prop);
                    }

                    mValueProperties.Clear();

                    mAddButton->Hide(true);

                    onChanged(this);
                    o2EditorSceneScreen.OnSceneChanged();
                }
            }
        }
        else if (lastDifferent || mValueProperties.Count() != mCountOfElements)
        {
            mCountProperty->SetValue(mCountOfElements);

            if (IsExpanded())
            {
                int i = 0;
                for (; i < mCountOfElements; i++)
                {
                    auto itemTargetValues = mTargetObjects.Convert<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
                        [&](const Pair<TargetObjectData, TargetObjectData>& x)
                    {
                        return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
                            mVectorType->GetObjectVectorElementProxy(x.first.data, i),
                            x.second.data ? mVectorType->GetObjectVectorElementProxy(x.second.data, i) : nullptr);
                    });

                    IPropertyField* propertyDef;
                    if (i < mValueProperties.Count())
                        propertyDef = mValueProperties[i];
                    else
                    {
                        propertyDef = GetFreeValueProperty();
                        mValueProperties.Add(propertyDef);
                    }

                    mSpoiler->AddChild(propertyDef);
                    propertyDef->SetCaption((String)"# " + (String)i);
                    propertyDef->SetValueAndPrototypeProxy(itemTargetValues);
                    propertyDef->SetValuePath((String)i);
                    propertyDef->GetRemoveButton()->onClick = [=]() { Remove(i); };
                    UpdateElementCaption(propertyDef);

                    propertyDef->onChangeCompleted =
                        [&](const String& path, const Vector<DataDocument#include <memory>
#include <vector>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

class VectorProperty
{
    struct TargetObjectData
    {
        Ref<IAbstractValueProxy> proxy;
        void* data;
        bool isCreated;
    };

    struct IPropertyField
    {
        virtual void SetValueAndPrototypeProxy(const Vector<TargetObjectData>& values) = 0;
        virtual void SetParentContext(void* context) = 0;
    };

private:
    size_t mCountOfElements;
    Vector<Ref<IPropertyField>> mValueProperties;
    Vector<Ref<IPropertyField>> mValuePropertiesPool;
    Ref<IPropertyField> mCountProperty;
    Ref<Spoiler> mSpoiler;
    Ref<Button> mAddButton;
    Ref<Widget> mHeaderContainer;
    Ref<Button> mRemoveBtn;
    const Type* mVectorType;
    bool mIsRefreshing;
    Vector<Pair<TargetObjectData, TargetObjectData>> mTargetObjects;
    const FieldInfo* mFieldInfo;
    void* mParentContext;

public:
    void Resize(size_t count)
    {
        mCountOfElements = count;

        if (mCountOfElements > mValueProperties.Count())
        {
            size_t i = mValueProperties.Count();

            for (; i < mCountOfElements; i++)
            {
                auto propertyDef = GetFreeValueProperty();
                mValueProperties.PushBack(propertyDef);

                propertyDef->SetParentContext(mParentContext);
                propertyDef->SetFieldInfo(mFieldInfo);

                propertyDef->OnCountChanged += [&](IPropertyField* def) {
                    OnCountChanged(def);
                };

                mSpoiler->AddChild(propertyDef);
            }

            mValueProperties.Resize(mCountOfElements);
        }
        else if (mCountOfElements < mValueProperties.Count())
        {
            for (size_t i = mCountOfElements; i < mValueProperties.Count(); i++)
            {
                mSpoiler->RemoveChild(mValueProperties[i], false);
                FreeValueProperty(mValueProperties[i]);
            }

            mValueProperties.Resize(mCountOfElements);
        }

        mAddButton->Show(true);
        mSpoiler->SetLayoutDirty();
        onChanged(this);
        o2EditorSceneScreen.OnSceneChanged();
    }

    const Type* GetValueType() const
    {
        return mVectorType;
    }

    void SpecializeType(const Type* type)
    {
        mVectorType = nullptr;

        if (type->GetUsage() == Type::Usage::Vector)
            mVectorType = dynamic_cast<const VectorType*>(type);
        else if (type->GetUsage() == Type::Usage::Property)
        {
            auto propertyType = dynamic_cast<const PropertyType*>(type);

            if (propertyType->GetValueType()->GetUsage() == Type::Usage::Vector)
                mVectorType = dynamic_cast<const VectorType*>(propertyType->GetValueType());
        }
    }

    void SetFieldInfo(const FieldInfo* fieldInfo)
    {
        IPropertyField::SetFieldInfo(fieldInfo);

        if (fieldInfo)
        {
            if (fieldInfo->HasAttribute<ExpandedByDefaultAttribute>())
                mSpoiler->Expand();
        }
    }

    const Type* GetSpecializedType() const
    {
        return mVectorType;
    }

    void SetCaption(const WString& text)
    {
        mSpoiler->SetCaption(text);

        Text* spoilerCaptionLayer = mSpoiler->GetLayerDrawable<Text>("caption");
        if (spoilerCaptionLayer)
        {
            Vec2F captionSize = Text::GetTextSize(text, spoilerCaptionLayer->GetFont(), spoilerCaptionLayer->GetFontHeight());
            *mHeaderContainer->layout = WidgetLayout::HorStretch(VerAlign::Top, captionSize.x + 20.0f, 0, 17, 0);
        }
    }

    WString GetCaption() const
    {
        return mSpoiler->GetCaption();
    }

    Ref<Button> GetRemoveButton()
    {
        if (!mRemoveBtn)
        {
            mRemoveBtn = mmake<Button>("remove small");
            mRemoveBtn->layout->maxWidth = 20;
            mRemoveBtn->layout->minHeight = 20;
            mHeaderContainer->AddChild(mRemoveBtn, 0);
        }

        return mRemoveBtn;
    }

    void Expand()
    {
        SetExpanded(true);
    }

    void Collapse()
    {
        SetExpanded(false);
    }

    void SetExpanded(bool expanded)
    {
        mSpoiler->SetExpanded(expanded);
    }

    bool IsExpanded() const
    {
        return mSpoiler->IsExpanded();
    }

    void* GetProxyValuePointer(Ref<IAbstractValueProxy> proxy) const
    {
        Ref<IPointerValueProxy> variableProxy = DynamicCast<IPointerValueProxy>(proxy);
        if (variableProxy)
            return variableProxy->GetValueVoidPointer();

        return nullptr;
    }

    Ref<IPropertyField> GetFreeValueProperty()
    {
        if (!mValuePropertiesPool.IsEmpty())
            return mValuePropertiesPool.PopBack();

        Ref<IPropertyField> res = mmake<IPropertyField>();
        res->SetParentContext(mParentContext);

        res->AddLayer("drag", mmake<Sprite>("ui/UI4_drag_handle.png"), Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-18, 0)));

        if (res)
            res->SetFieldInfo(mFieldInfo);

        return res;
    }

    void FreeValueProperty(Ref<IPropertyField> def)
    {
        mValuePropertiesPool.Add(def);
    }

    void OnPropertyChanged(const String& path, const Vector<DataDocument>& before, const Vector<DataDocument>& after)
    {
        for (auto& pair : mTargetObjects)
            pair.first.SetValue#include <memory>

template<typename T>
class Ref
{
public:
	Ref() : mPtr(nullptr) {}
	explicit Ref(T* ptr) : mPtr(ptr) {}
	Ref(const Ref<T>& ref) : mPtr(ref.mPtr) {}
	Ref<T>& operator=(const Ref<T>& ref) { mPtr = ref.mPtr; return *this; }
	void reset() { mPtr = nullptr; }
	T* get() const { return mPtr; }
	bool operator==(const Ref<T>& ref) const { return mPtr == ref.mPtr; }
	bool operator!=(const Ref<T>& ref) const { return mPtr != ref.mPtr; }

	T* operator->() const { return mPtr; }
	T& operator*() const { return *mPtr; }
	explicit operator bool() const { return mPtr != nullptr; }

private:
	T* mPtr;
};

template<typename T>
Ref<T> mmake()
{
	return Ref<T>(new T());
}

template<typename T>
class WeakRef
{
public:
	WeakRef() : mPtr(nullptr) {}
	explicit WeakRef(Ref<T> ref) : mPtr(ref.get()) {}
	WeakRef(const WeakRef<T>& weakRef) : mPtr(weakRef.mPtr) {}
	WeakRef<T>& operator=(const WeakRef<T>& weakRef) { mPtr = weakRef.mPtr; return *this; }
	void reset() { mPtr = nullptr; }
	T* get() const { return mPtr; }
	bool operator==(const WeakRef<T>& weakRef) const { return mPtr == weakRef.mPtr; }
	bool operator!=(const WeakRef<T>& weakRef) const { return mPtr != weakRef.mPtr; }

	explicit operator bool() const { return mPtr != nullptr; }

private:
	T* mPtr;
};

template<typename T>
Ref<T> DynamicCast(Ref<T> ref)
{
	return std::dynamic_pointer_cast<typename T::element_type>(ref);
}

void onChangeCompleted(const Ref<DataType>& path, const Ref<DataType>& before, const Ref<DataType>& after)
{
	// Code implementation
}

void onChanged(const Ref<DataType>& data)
{
	// Code implementation
}

namespace Editor
{
	class VectorProperty
	{
	public:
		void Resize(int newCount)
		{
			newCount = Math::Max(0, newCount);

			Vector<DataDocument> prevValues, newValues;
			auto elementFieldInfo = mVectorType->GetElementFieldInfo();

			for (auto& obj : mTargetObjects)
			{
				prevValues.Add(DataDocument());
				prevValues.Last()["Size"].Set(mVectorType->GetObjectVectorSize(obj.first.data));
				DataValue& elementsData = prevValues.Last()["Elements"];

				int lastCount = mVectorType->GetObjectVectorSize(obj.first.data);
				for (int i = newCount; i < lastCount; i++)
				{
					elementFieldInfo->Serialize(mVectorType->GetObjectVectorElementPtr(obj.first.data, i),
						elementsData.AddMember("Element" + (String)i));
				}

				newValues.Add(DataDocument());
				newValues.Last()["Size"].Set(newCount);

				mVectorType->SetObjectVectorSize(obj.first.data, newCount);
			}

			Refresh();

			if (prevValues != newValues)
				onChangeCompleted(mValuesPath + "/count", prevValues, newValues);

			onChanged(this);
			o2EditorSceneScreen.OnSceneChanged();
		}

		void Remove(int idx)
		{
			Vector<DataDocument> prevValues, newValues;

			for (auto& obj : mTargetObjects)
			{
				prevValues.Add(DataDocument());
				mVectorType->Serialize(obj.first.data, prevValues.Last());

				mVectorType->RemoveObjectVectorElement(obj.first.data, idx);

				newValues.Add(DataDocument());
				mVectorType->Serialize(obj.first.data, newValues.Last());
			}

			Refresh();

			if (prevValues != newValues)
				onChangeCompleted(mValuesPath, prevValues, newValues);

			onChanged(this);
			o2EditorSceneScreen.OnSceneChanged();
		}

		void UpdateElementCaption(const Ref<IPropertyField>& propertyDef) const
		{
			auto& proxies = propertyDef->GetValueAndPrototypeProxy();
			if (!proxies.IsEmpty())
			{
				const Type* itemType = &proxies[0].first->GetType();
				if (auto ptrType = DynamicCast<const PointerType>(itemType))
					itemType = ptrType->GetBaseType();

				if (auto objectType = DynamicCast<const ObjectType>(itemType))
				{
					static String nameVariants[] = { "name", "mName", "_name", "id", "mId" };
					for (auto& name : nameVariants)
					{
						if (auto nameField = objectType->GetField(name))
						{
							if (nameField->GetType() == &TypeOf(String))
							{
								void* objectPtr;
								proxies[0].first->GetValuePtr(&objectPtr);
								if (objectPtr)
								{
									String name = nameField->GetValue<String>(objectPtr);
									if (!name.IsEmpty())
										propertyDef->SetCaption("#" + propertyDef->GetValuePath() + " - " + name);
								}

								return;
							}
						}
					}
				}
			}

			propertyDef->SetCaption("#" + propertyDef->GetValuePath());
		}

	private:
		struct TargetObjectData
		{
			Ref<IPropertyOwnerProxy> proxy;
			Ref<void> data;
			bool isCreated;

			void Refresh()
			{
				if (!isCreated)
					return;

				proxy->GetValuePtr(data.get());
			}

			void SetValue()
			{
				if (!isCreated)
					return;

				proxy->SetValuePtr(data.get());
			}
		};

		Vector<Ref<TargetObjectData>> mTargetObjects;
		Ref<VectorType> mVectorType;
		Ref<DataType> mValuesPath;

		void Refresh()
		{
			for (const auto& obj : mTargetObjects)
			{
				obj->Refresh();
			}
		}
	};

}

// --- META ---
DECLARE_CLASS(Editor::VectorProperty, Editor__VectorProperty);
// --- END META ---