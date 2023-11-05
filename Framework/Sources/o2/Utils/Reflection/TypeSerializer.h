#pragma once

#include "o2/Utils/Memory/MemoryManager.h"
#include "o2/Utils/Reflection/TypeTraits.h"

namespace o2
{
    class DataValue;

    struct ITypeSerializer
    {
        virtual ~ITypeSerializer() { }

        virtual bool IsDefault(void* object) const { return false; }

        virtual void Serialize(void* object, DataValue& data) const { }
        virtual void Deserialize(void* object, const DataValue& data) const { }

        virtual bool Equals(void* objectA, void* objectB) const { return false; }
        virtual void Copy(void* objectA, void* objectB) const { }

        virtual ITypeSerializer* Clone() const { return mnew ITypeSerializer(); }
    };
    template<typename _type>
    struct TypeSerializer : public ITypeSerializer
    {
        static constexpr bool isSerializable = DataValue::IsSupports<_type>::value;
        static constexpr bool isEqualsSupport = SupportsEqualOperator<_type>::value;
        static constexpr bool isCopyable = std::is_assignable<_type&, _type>::value;

    public:
        bool IsDefault(void* object) const override;

        void Serialize(void* object, DataValue& data) const override;
        void Deserialize(void* object, const DataValue& data) const override;

        bool Equals(void* objectA, void* objectB) const override;
        void Copy(void* objectA, void* objectB) const override;

        ITypeSerializer* Clone() const override;
    };

    template<typename _type>
    bool TypeSerializer<_type>::IsDefault(void* object) const
    {
        if constexpr (std::is_default_constructible<_type>::value && SupportsEqualOperator<_type>::value)
            return Math::Equals(*(_type*)object, _type());

        return false;
    }

    template<typename _type>
    void TypeSerializer<_type>::Serialize(void* object, DataValue& data) const
    {
        if constexpr (isSerializable)
            data.Set(*(_type*)object);
    }

    template<typename _type>
    void TypeSerializer<_type>::Deserialize(void* object, const DataValue& data) const
    {
        if constexpr (isSerializable)
            data.Get(*(_type*)object);
    }

    template<typename _type>
    bool TypeSerializer<_type>::Equals(void* objectA, void* objectB) const
    {
        if constexpr (isEqualsSupport)
            return Math::Equals(*(_type*)objectA, *(_type*)objectB);

        return false;
    }

    template<typename _type>
    void TypeSerializer<_type>::Copy(void* objectA, void* objectB) const
    {
        if constexpr (isCopyable)
            *(_type*)objectA = *(_type*)objectB;
    }

    template<typename _type>
    ITypeSerializer* TypeSerializer<_type>::Clone() const
    {
        return mnew TypeSerializer();
    }
}
