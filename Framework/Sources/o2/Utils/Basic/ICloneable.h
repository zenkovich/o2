#pragma once

#include "o2/Utils/Types/Ref.h"

namespace o2
{
    // -------------------
    // Cloneable interface
    // -------------------
    class ICloneable
    {
    public:
        virtual ~ICloneable() {}

        // Cloning interface
        virtual ICloneable* Clone() const { return nullptr; };

        // Cloning as type
        template<typename _cast_type>
        _cast_type* CloneAs() const { return dynamic_cast<_cast_type*>(Clone()); }
    };

    // ---------------------------
    // Cloneable reference counter
    // ---------------------------
    class ICloneableRef: virtual public ICloneable
    {
    public:
        // Cloning reference interface
        virtual Ref<RefCounterable> CloneRef() const { return nullptr; }

        // Cloning as casted reference
        template<typename _cast_type>
        Ref<_cast_type> CloneAsRef() const
        {
            return DynamicCast<_cast_type>(CloneRef());
        }
    };

    template<typename _type>
    struct SafeClone
    {
        static _type* Clone(const _type& origin)
        {
            if constexpr (std::is_copy_constructible<_type>::value)
                return mnew _type(origin);
            else
                return nullptr;
        }
    };
}

// Defines clone method for class
#define CLONEABLE(CLASS) \
    virtual ICloneable* Clone() const override { return o2::SafeClone<CLASS>::Clone(*this); }

// Defines clone method for class with reference counting
#define CLONEABLE_REF(CLASS) \
    virtual Ref<RefCounterable> CloneRef() const override { return mmake<CLASS>(*this); }
