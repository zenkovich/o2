#pragma once

#include "o2/Scripts/ScriptValueDef.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Math/Basis.h"
#include "o2/Utils/Math/Color.h"
#include "o2/Utils/Math/Rect.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Math/Vertex2.h"
#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/UID.h"

namespace o2
{
	template<class T> struct IsVectorHelper : std::false_type {};
	template<class T> struct IsVectorHelper<Vector<T>> : std::true_type {};
	template<class T> struct IsVector : IsVectorHelper<typename std::remove_cv<T>::type> {};
	template<class T> struct ExtractVectorElementType { typedef T type; };
	template<class T> struct ExtractVectorElementType<Vector<T>> { typedef T type; };

	template<class T, class T2> struct IsMapHelper : std::false_type {};
	template<class T, class T2> struct IsMapHelper<Map<T, T2>, void> : std::true_type {};
	template<class T> struct IsMap : IsMapHelper<typename std::remove_cv<T>::type, void> {};

	template<class T, class T2> struct MapKeyTypeGetterHelper { typedef T type; };
	template<class T, class T2> struct MapKeyTypeGetterHelper<Map<T, T2>, void> { typedef T type; };
	template<class T> struct ExtractMapKeyType : MapKeyTypeGetterHelper<typename std::remove_cv<T>::type, void> {};

	template<class T, class T2> struct MapValueTypeGetterHelper { typedef T2 type; };
	template<class T, class T2> struct MapValueTypeGetterHelper<Map<T, T2>, void> { typedef T2 type; };
	template<class T> struct ExtractMapValueType : MapValueTypeGetterHelper<typename std::remove_cv<T>::type, void> {};
	
	template<typename... Ts> struct make_void { typedef void type; };
	template<typename... Ts> using void_t = typename make_void<Ts...>::type;

	template<class T, class = void_t<>>
	struct IsCloneable: std::false_type { };

	template<class T>
	struct IsCloneable<T, void_t<decltype(&T::Clone)>>: std::true_type { };

	template<class T, class = void_t<>>
	struct IsAccessor : std::false_type { };

	template<class T>
	struct IsAccessor<T, void_t<decltype(&T::IsAccessor)>> : std::true_type { };

	template<class T, class = void_t<>>
	struct IsStringAccessorHelper : std::false_type { };

	template<class T>
	struct IsStringAccessorHelper<T, typename std::enable_if<IsAccessor<T>::value && std::is_same<typename T::keyType, String>::value>::type> : std::true_type { };

	template<class T> struct IsStringAccessor : IsStringAccessorHelper<typename std::remove_cv<T>::type> { };

	template<class T, class = void_t<>>
	struct IsProperty : std::false_type { };

	template<class T>
	struct IsProperty<T, void_t<decltype(&T::IsProperty)>> : std::true_type { };

	template<class T, class = void_t<>>
	struct SupportsPlus : std::false_type {};

	template<class T>
	struct SupportsPlus<T, void_t<decltype(std::declval<T>() + std::declval<T>())>> : std::true_type {};

	template<class T, class = void_t<>>
	struct SupportsMinus : std::false_type {};

	template<class T>
	struct SupportsMinus<T, void_t<decltype(std::declval<T>() - std::declval<T>())>> : std::true_type {};

	template<class T, class = void_t<>>
	struct SupportsDivide : std::false_type {};

	template<class T>
	struct SupportsDivide<T, void_t<decltype(std::declval<T>() / std::declval<T>())>> : std::true_type {};

	template<class T, class = void_t<>>
	struct SupportsMultiply: std::false_type {};

	template<class T>
	struct SupportsMultiply<T, void_t<decltype(std::declval<T>() * std::declval<T>())>>: std::true_type {};

	template<class T, class = void_t<>>
	struct SupportsEqualOperator: std::false_type {};

	template<class T>
	struct SupportsEqualOperator<T, void_t<decltype(std::declval<T>() == std::declval<T>())>>: std::true_type {};

	template<class T, class = void>
	struct ExtractPropertyValueType
	{
		typedef T type;
	};

	template<class T>
	struct ExtractPropertyValueType<T, void_t<decltype(std::declval<T>().IsProperty())>>
	{
		typedef typename T::valueType type;
	};

	template<typename T>
	struct IsFundamental: public std::conditional<
		std::is_fundamental<T>::value ||
		std::is_same<T, Basis>::value ||
		std::is_same<T, Color4>::value ||
		std::is_same<T, RectI>::value ||
		std::is_same<T, RectF>::value ||
		std::is_same<T, BorderI>::value ||
		std::is_same<T, BorderF>::value ||
		std::is_same<T, Vec2I>::value ||
		std::is_same<T, Vec2F>::value ||
		std::is_same<T, Vertex2>::value ||
		std::is_same<T, String>::value ||
		std::is_same<T, WString>::value ||
		std::is_same<T, UID>::value ||
		std::is_same<T, ScriptValue>::value ||
		std::is_same<T, DataValue>::value, std::true_type, std::false_type>::type {};
}

#include "o2/Utils/Reflection/Type.h"
#include "o2/Utils/Reflection/Reflection.h"

namespace o2
{
#if defined PLATFORM_IOS || defined PLATFORM_MAC || defined PLATFORM_ANDROID
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-var-template"
#endif

	// Returns type of template parameter
	template<typename _type>
	const Type& GetTypeOf()
	{
		if constexpr (std::is_pointer<_type>::value)
		{
			return *GetTypeOf<typename std::remove_pointer<_type>::type>().GetPointerType();
		}
		else if constexpr (IsVector<_type>::value)
		{
			return *Reflection::InitializeVectorType<typename ExtractVectorElementType<_type>::type>();
		}
		else if constexpr (IsStringAccessor<_type>::value)
		{
			return *Reflection::InitializeAccessorType<typename _type::valueType, _type>();
		}
		else if constexpr (IsMap<_type>::value)
		{
			return *Reflection::InitializeMapType<typename ExtractMapKeyType<_type>::type, typename ExtractMapValueType<_type>::type>();
		}
		else if constexpr (IsProperty<_type>::value)
		{
			return *Reflection::InitializePropertyType<typename _type::valueType, _type>();
		}
		else if constexpr (std::is_base_of<IObject, _type>::value)
		{
			return *_type::type;
		}
		else if constexpr (IsFundamental<_type>::value && !std::is_const<_type>::value)
		{
			return *FundamentalTypeContainer<_type>::type;
		}
		else if constexpr (std::is_enum<_type>::value && IsEnumReflectable<_type>::value)
		{
			return *EnumTypeContainer<_type>::type;
		}
		else
		{
			return *Type::Dummy::type;
		}
	}

#if defined PLATFORM_IOS || defined PLATFORM_MAC || defined PLATFORM_ANDROID
#pragma GCC diagnostic pop
#endif

}
