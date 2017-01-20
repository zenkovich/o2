#pragma once

namespace o2
{
	template<class T> struct IsVectorHelper: std::false_type {};
	template<class T> struct IsVectorHelper<Vector<T>>: std::true_type {};
	template<class T> struct IsVector: IsVectorHelper<typename std::remove_cv<T>::type> {};
	template<class T> struct ExtractVectorElementType { typedef T type; };
	template<class T> struct ExtractVectorElementType<Vector<T>> { typedef T type; };

	template<class T> struct IsPropertyHelper: std::false_type {};
	template<class T> struct IsPropertyHelper<Property<T>>: std::true_type {};
	template<class T> struct IsProperty: IsPropertyHelper<typename std::remove_cv<T>::type> {};
	template<class T> struct ExtractPropertyValueType { typedef T type; };
	template<class T> struct ExtractPropertyValueType<Property<T>> { typedef T type; };

	template<class T, class T2> struct IsDictionaryHelper: std::false_type {};
	template<class T, class T2> struct IsDictionaryHelper<Dictionary<T, T2>, void>: std::true_type {};
	template<class T> struct IsDictionary: IsDictionaryHelper<typename std::remove_cv<T>::type, void> {};

	template<class T, class T2> struct DictionaryKeyTypeGetterHelper { typedef T type; };
	template<class T, class T2> struct DictionaryKeyTypeGetterHelper<Dictionary<T, T2>, void> { typedef T type; };
	template<class T> struct ExtractDictionaryKeyType: DictionaryKeyTypeGetterHelper<typename std::remove_cv<T>::type, void> {};

	template<class T, class T2> struct DictionaryValueTypeGetterHelper { typedef T2 type; };
	template<class T, class T2> struct DictionaryValueTypeGetterHelper<Dictionary<T, T2>, void> { typedef T2 type; };
	template<class T> struct ExtractDictionaryValueType: DictionaryValueTypeGetterHelper<typename std::remove_cv<T>::type, void> {};

	template<class T> struct IsStringAccessorHelper: std::false_type {};
	template<class T> struct IsStringAccessorHelper<Accessor<T*, const String&>>: std::true_type {};
	template<class T> struct IsStringAccessor: IsStringAccessorHelper<typename std::remove_cv<T>::type> {};
	template<class T> struct ExtractStringAccessorType { typedef T type; };
	template<class T> struct ExtractStringAccessorType<Accessor<T*, const String&>> { typedef T type; };

	namespace EqualsTrait
	{
		struct No {};
		template<typename T, typename Arg> No operator== (const T&, const Arg&);

		template<typename T, typename Arg = T>
		struct IsExists
		{
			enum x { value = !std::is_same<decltype(*(T*)(0) == *(Arg*)(0)), No>::value };
		};
	}

	// type trait
	template<typename T, typename X =
	/* if */   std::conditional<std::is_base_of<IObject, T>::value,
	/* then */ T,
	/* else */ std::conditional<(
	           /* if */   std::is_fundamental<T>::value ||
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
		                  std::is_same<T, DataNode>::value) && !std::is_const<T>::value,
		       /* then */ FundamentalTypeContainer<T>,
		       /* else */ std::conditional<
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
		static const Type& GetType() { return *GetTypeOf<std::remove_pointer<T>::type>().GetPointerType(); }
	};

	template<typename T>
	struct PropertyTypeGetter
	{
		static const Type& GetType() { return *Reflection::InitializePropertyType<ExtractPropertyValueType<T>::type>(); }
	};

	template<typename T>
	struct VectorTypeGetter
	{
		static const Type& GetType() { return *Reflection::InitializeVectorType<ExtractVectorElementType<T>::type>(); }
	};

	template<typename T>
	struct DictionaryTypeGetter
	{
		static const Type& GetType() {
			return *Reflection::InitializeDictionaryType<ExtractDictionaryKeyType<T>::type, ExtractDictionaryValueType<T>::type>();
		}
	};

	template<typename T>
	struct AccessorTypeGetter
	{
		static const Type& GetType() { return *Reflection::InitializeAccessorType<ExtractStringAccessorType<T>::type>(); }
	};

	// Returns type of template parameter
	template<typename _type, typename _getter = 
		std::conditional<
		/* if */   std::is_pointer<_type>::value,
		/* then */ PointerTypeGetter<_type>,
		/* else */ std::conditional<
		           /* if */   IsVector<_type>::value,
		           /* then */ VectorTypeGetter<_type>,
		           /* else */ std::conditional<
		                      /* if */   IsStringAccessor<_type>::value,
		                      /* then */ AccessorTypeGetter<_type>,
		                      /* else */ std::conditional<
		                                 /* if */   IsDictionary<_type>::value,
		                                 /* then */ DictionaryTypeGetter<_type>,
		                                            std::conditional<
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
}