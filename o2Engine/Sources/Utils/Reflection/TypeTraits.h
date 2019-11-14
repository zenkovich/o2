#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Math/Basis.h"
#include "Utils/Math/Color.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Types/Containers/Map.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Types/UID.h"

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
	struct SupportsMultiply : std::false_type {};

	template<class T>
	struct SupportsMultiply<T, void_t<decltype(std::declval<T>() * std::declval<T>())>> : std::true_type {};

	namespace EqualsOperator
	{
		struct No {};
		template<typename T, typename Arg> No operator== (const T&, const Arg&);

		template<typename T, typename Arg = T>
		struct IsExists
		{
			enum x { value = !std::is_same<decltype(*(T*)(0) == *(Arg*)(0)), No>::value };
		};
	}

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
}

#include "Utils/Reflection/Type.h"

namespace o2
{
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
		std::is_same<T, DataNode>::value, std::true_type, std::false_type>::type {};

	// type trait
	template<typename T, typename X =
	/* if */   typename std::conditional<std::is_base_of<IObject, T>::value,
	/* then */ T,
	/* else */ typename std::conditional<IsFundamental<T>::value && !std::is_const<T>::value,
		       /* then */ FundamentalTypeContainer<T>,
		       /* else */ typename std::conditional<
		                  /* if */   std::is_enum<T>::value,
		                  /* then */ EnumTypeContainer<T>,
		                  /* else */ Type::Dummy
			              >::type
	           >::type
	>::type>
	struct GetTypeHelper
	{
		typedef X type;
	};

	template<typename _type, typename _getter>
	const Type& GetTypeOf();

	template<typename T>
	struct RegularTypeGetter
	{
		static const Type& GetType() { return *GetTypeHelper<T>::type::type; }
	};

	template<typename T>
	struct PointerTypeGetter
	{
		static const Type& GetType();
	};

	template<typename T>
	struct PropertyTypeGetter
	{
		static const Type& GetType() { return *Reflection::InitializePropertyType<typename T::valueType, T>(); }
	};

	template<typename T>
	struct VectorTypeGetter
	{
		static const Type& GetType() { return *Reflection::InitializeVectorType<typename ExtractVectorElementType<T>::type>(); }
	};

	template<typename T>
	struct MapTypeGetter
	{
		static const Type& GetType() 
		{
			return *Reflection::InitializeMapType<typename ExtractMapKeyType<T>::type, typename ExtractMapValueType<T>::type>();
		}
	};

	template<typename T>
	struct AccessorTypeGetter
	{
		static const Type& GetType() { return *Reflection::InitializeAccessorType<typename T::valueType, T>(); }
	};

	// Returns type of template parameter
	template<typename _type, typename _getter = 
		typename std::conditional<
		/* if */   std::is_pointer<_type>::value,
		/* then */ PointerTypeGetter<_type>,
		/* else */ typename std::conditional<
		           /* if */   IsVector<_type>::value,
		           /* then */ VectorTypeGetter<_type>,
		           /* else */ typename std::conditional<
		                      /* if */   IsStringAccessor<_type>::value,
		                      /* then */ AccessorTypeGetter<_type>,
		                      /* else */ typename std::conditional<
		                                 /* if */   IsMap<_type>::value,
		                                 /* then */ MapTypeGetter<_type>,
		                                            typename std::conditional<
		                                            /* if */   IsProperty<_type>::value,
		                                            /* then */ PropertyTypeGetter<_type>,
		                                            /* else */ RegularTypeGetter<_type>
													>::type
							             >::type
				              >::type
		           >::type
		>::type
	>
	const Type& GetTypeOf()
	{
		return _getter::GetType();
	}

	template<typename T>
	const Type& PointerTypeGetter<T>::GetType()
	{
		return *GetTypeOf<typename std::remove_pointer<T>::type>().GetPointerType();
	}
}
