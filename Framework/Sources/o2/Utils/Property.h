#pragma once

#include "o2/Utils/Reflection/TypeTraits.h"

namespace o2
{
#define PROPERTIES(CLASSNAME) \
    typedef CLASSNAME _propertiesClassType

#define PROPERTY(TYPE, NAME, SETTER, GETTER)                                                                                                                   \
    class NAME##_PROPERTY                                                                                                                                      \
    {                                                                                                                                                          \
        _propertiesClassType* _this;                                                                                                                           \
                                                                                                                                                               \
    public:                                                                                                                                                    \
        typedef TYPE valueType;                                                                                                                                \
        NAME##_PROPERTY(_propertiesClassType* _this):_this(_this) {}                                                                                           \
                                                                                                                                                               \
        operator valueType() const { return _this->GETTER(); }                                                                                                 \
        NAME##_PROPERTY& operator=(const valueType& value) { _this->SETTER(const_cast<valueType&>(value)); return *this; }                                     \
                                                                                                                                                               \
        NAME##_PROPERTY& operator=(const NAME##_PROPERTY& value) { _this->SETTER(value.Get()); return *this; }                                                 \
                                                                                                                                                               \
        template<typename vt, typename X = typename std::enable_if<std::is_same<vt, valueType>::value && SupportsEqualOperator<valueType>::value>::type>       \
        bool operator==(const vt& value) const { return Math::Equals(_this->GETTER(), value); }                                                                \
                                                                                                                                                               \
        template<typename vt, typename X = typename std::enable_if<std::is_same<vt, valueType>::value && SupportsEqualOperator<valueType>::value>::type>       \
        bool operator!=(const vt& value) const { return !Math::Equals(_this->GETTER(), value); }                                                               \
                                                                                                                                                               \
        template<typename T, typename X = typename std::enable_if<o2::SupportsPlus<valueType>::value && std::is_same<T, valueType>::value>::type>              \
        valueType operator+(const T& value) { return _this->GETTER() + value; }                                                                                \
                                                                                                                                                               \
        template<typename T, typename X = typename std::enable_if<o2::SupportsMinus<valueType>::value && std::is_same<T, valueType>::value>::type>             \
        valueType operator-(const T& value) { return _this->GETTER() - value; }                                                                                \
                                                                                                                                                               \
        template<typename T, typename X = typename std::enable_if<o2::SupportsDivide<valueType>::value && std::is_same<T, valueType>::value>::type>            \
        valueType operator/(const T& value) { return _this->GETTER() / value; }                                                                                \
                                                                                                                                                               \
        template<typename T, typename X = typename std::enable_if<o2::SupportsMultiply<valueType>::value && std::is_same<T, valueType>::value>::type>          \
        valueType operator*(const T& value) { return _this->GETTER() * value; }                                                                                \
                                                                                                                                                               \
        template<typename T, typename X = typename std::enable_if<o2::SupportsPlus<valueType>::value && std::is_same<T, valueType>::value>::type>              \
        NAME##_PROPERTY& operator+=(const T& value) { _this->SETTER(_this->GETTER() + value); return *this; }                                                  \
                                                                                                                                                               \
        template<typename T, typename X = typename std::enable_if<o2::SupportsMinus<valueType>::value && std::is_same<T, valueType>::value>::type>             \
        NAME##_PROPERTY& operator-=(const T& value) { _this->SETTER(_this->GETTER() - value); return *this; }                                                  \
                                                                                                                                                               \
        template<typename T, typename X = typename std::enable_if<o2::SupportsDivide<valueType>::value && std::is_same<T, valueType>::value>::type>            \
        NAME##_PROPERTY& operator/=(const T& value) { _this->SETTER(_this->GETTER() / value); return *this; }                                                  \
                                                                                                                                                               \
        template<typename T, typename X = typename std::enable_if<o2::SupportsMultiply<valueType>::value && std::is_same<T, valueType>::value>::type>          \
        NAME##_PROPERTY& operator*=(const T& value) { _this->SETTER(_this->GETTER() * value); return *this; }                                                  \
                                                                                                                                                               \
        valueType Get() const { return _this->GETTER(); }                                                                                                      \
        void Set(const valueType& value) { _this->SETTER(const_cast<valueType&>(value)); }                                                                     \
                                                                                                                                                               \
        PropertyValueProxy<valueType, NAME##_PROPERTY> GetValueProxy() { return PropertyValueProxy<valueType, NAME##_PROPERTY>(this); }                        \
                                                                                                                                                               \
        bool IsProperty() const { return true; }                                                                                                               \
    };                                                                                                                                                         \
                                                                                                                                                               \
    NAME##_PROPERTY NAME = NAME##_PROPERTY(this);

#define GETTER(TYPE, NAME, GETTER)                                                                       \
    class NAME##_GET_PROPERTY                                                                            \
    {                                                                                                    \
        _propertiesClassType* _this;                                                                     \
                                                                                                         \
    public:                                                                                              \
        typedef TYPE valueType;                                                                          \
        NAME##_GET_PROPERTY(_propertiesClassType* _this):_this(_this) {}                                 \
        operator valueType() { return _this->GETTER(); }                                                 \
        bool operator==(const valueType& value) const { return _this->GETTER() == value; }               \
        bool operator!=(const valueType& value) const { return _this->GETTER() != value; }               \
        TYPE Get() const { return _this->GETTER(); }                                                     \
    };                                                                                                   \
                                                                                                         \
    NAME##_GET_PROPERTY NAME = NAME##_GET_PROPERTY(this);                                                

#define SETTER(TYPE, NAME, SETTER)                                                                       \
    class NAME##_SET_PROPERTY                                                                            \
    {                                                                                                    \
        _propertiesClassType* _this;                                                                     \
                                                                                                         \
    public:                                                                                              \
        typedef TYPE valueType;                                                                          \
        NAME##_SET_PROPERTY(_propertiesClassType* _this):_this(_this) {}                                 \
        NAME##_SET_PROPERTY& operator=(const valueType& value) { _this->SETTER(value); return *this; }   \
        void Set(const valueType& value) { _this->SETTER(const_cast<valueType&>(value)); }               \
    };                                                                                                   \
                                                                                                         \
    NAME##_SET_PROPERTY NAME = NAME##_SET_PROPERTY(this);                                                \

#define ACCESSOR(TYPE, NAME, KEY_TYPE, GETTER, GET_ALL)                                                  \
    class NAME##_ACCESSOR                                                                                \
    {                                                                                                    \
        _propertiesClassType* _this;                                                                     \
                                                                                                         \
    public:                                                                                              \
        typedef TYPE valueType;                                                                          \
        typedef KEY_TYPE keyType;                                                                        \
        NAME##_ACCESSOR(_propertiesClassType* _this):_this(_this) {}                                     \
        valueType Get(const keyType& key) const { return _this->GETTER(key); }                           \
        Map<keyType, TYPE> GetAll() const { return _this->GET_ALL(); }                                   \
        valueType operator[](const keyType& key) const { return _this->GETTER(key); }                    \
                                                                                                         \
        bool IsAccessor() const { return true; }                                                         \
    };                                                                                                   \
                                                                                                         \
    NAME##_ACCESSOR NAME = NAME##_ACCESSOR(this);    

}
