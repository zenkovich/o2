#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/KeyboardEventsListener.h"
#include <Ref.h>

using namespace o2;

namespace Editor
{
    class IntegerProperty;

    // ----------------------------
    // Editor int vector property
    // ----------------------------
    class Vec2IProperty : public IPropertyField
    {
    public:
        // Default constructor
        Vec2IProperty();

        // Copy constructor
        Vec2IProperty(const Vec2IProperty& other);

        // Copy operator
        Vec2IProperty& operator=(const Vec2IProperty& other);

        // Sets fields
        void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

        // Updates and checks value
        void Refresh() override;

        // Reverts value to prototype value
        void Revert() override;

        // Sets value
        void SetValue(const Ref<Vec2I>& value);

        // Sets value X
        void SetValueX(int value);

        // Sets value Y
        void SetValueY(int value);

        // Sets value as unknown
        void SetUnknownValue(const Ref<Vec2I>& defaultValue = Ref<Vec2I>());

        // Sets value X as unknown
        void SetXUnknownValue(int defaultValue = 0);

        // Sets value Y as unknown
        void SetYUnknownValue(int defaultValue = 0);

        // Returns value
        Ref<Vec2I> GetCommonValue() const;

        // Returns is values different
        bool IsValuesDifferent() const;

        // Returns editing by this field type
        const Type* GetValueType() const override;

        // Returns editing by this field type by static function, can't be changed during runtime
        static const Type* GetValueTypeStatic();

        IOBJECT(Vec2IProperty);

    protected:
        Ref<IntegerProperty> mXProperty;
        Ref<IntegerProperty> mYProperty;

    protected:
        // Searches controls widgets and layers and initializes them
        void InitializeControls();

    protected:
        class XValueProxy : public IValueProxy<int>
        {
            IAbstractValueProxy* mProxy = nullptr;

        public:
            XValueProxy();
            XValueProxy(IAbstractValueProxy* proxy);

            void SetValue(const int& value) override;
            int GetValue() const override;
        };

        class YValueProxy : public IValueProxy<int>
        {
            IAbstractValueProxy* mProxy = nullptr;

        public:
            YValueProxy();
            YValueProxy(IAbstractValueProxy* proxy);

            void SetValue(const int& value) override;
            int GetValue() const override;
        };
    };
}

REFLEX_CLASS_BEGIN(Editor::Vec2IProperty)
REFLEX_FIELD(mXProperty)
REFLEX_FIELD(mYProperty)
REFLEX_METHOD(SetValueAndPrototypeProxy)
REFLEX_METHOD(Refresh)
REFLEX_METHOD(Revert)
REFLEX_METHOD(SetValue)
REFLEX_METHOD(SetValueX)
REFLEX_METHOD(SetValueY)
REFLEX_METHOD(SetUnknownValue)
REFLEX_METHOD(SetXUnknownValue)
REFLEX_METHOD(SetYUnknownValue)
REFLEX_METHOD(GetCommonValue)
REFLEX_METHOD(IsValuesDifferent)
REFLEX_METHOD(GetValueType)
REFLEX_METHOD(GetValueTypeStatic)
REFLEX_CONSTRUCTOR()
REFLEX_CONSTRUCTOR(const Editor::Vec2IProperty&)
REFLEX_CLASS_END(Vec2IProperty)