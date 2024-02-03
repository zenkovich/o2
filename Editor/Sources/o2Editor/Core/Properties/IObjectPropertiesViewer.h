e>
	void TObjectPropertiesViewer<_object_type>::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		// Cast target objects to viewer type
		mTypeTargetObjects.clear();
		mTypeTargetObjects.reserve(targetObjets.size());
		for (const auto& target : targetObjets)
		{
			_object_type* from = dynamic_cast<_object_type*>(target.first);
			_object_type* to = dynamic_cast<_object_type*>(target.second);
			mTypeTargetObjects.emplace_back(from, to);
		}
		
		IObjectPropertiesViewer::OnRefreshed(targetObjets);
	}
} // namespace Editor#include <memory>

template<typename T>
class Ref
{
public:
    Ref() : m_ptr(nullptr) {}
    Ref(T* ptr) : m_ptr(ptr) {}
    template<typename U>
    Ref(const Ref<U>& other) : m_ptr(other.get()) {}

    T* get() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    operator bool() const { return m_ptr != nullptr; }

private:
    T* m_ptr;
};

template<typename T, typename U>
Ref<T> static_pointer_cast(const Ref<U>& ptr)
{
    return Ref<T>(static_cast<T*>(ptr.get()));
}

template<typename T, typename U>
Ref<T> dynamic_pointer_cast(const Ref<U>& ptr)
{
    return Ref<T>(dynamic_cast<T*>(ptr.get()));
}

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
    WeakRef(const Ref<T>& ptr) : m_ptr(ptr.get()) {}

    Ref<T> lock() const { return Ref<T>(m_ptr); }

private:
    T* m_ptr;
};

template<typename T>
Ref<T> const& as_const(const Ref<T>& ptr)
{
    return ptr;
}

template<typename T>
Ref<T>& as_mutable(const Ref<T>& ptr)
{
    return const_cast<Ref<T>&>(ptr);
}

template<typename T>
class DynamicCast
{
public:
    DynamicCast(const Ref<T>& ptr) : m_ptr(ptr.get()) {}

    template<typename U>
    operator Ref<U>() const
    {
        return dynamic_pointer_cast<U>(m_ptr);
    }

private:
    T* m_ptr;
};


template <typename _object_type>
class TObjectPropertiesViewer : public Editor::IObjectPropertiesViewer
{
public:
    void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
    {
        mTypeTargetObjects = targetObjets.Convert<Pair<_object_type*, _object_type*>>([](const auto& x) {
            return Pair<_object_type*, _object_type*>(DynamicCast<_object_type>(x.first), DynamicCast<_object_type>(x.second));
        });
    }
};

// --- META ---

CLASS_BASES_META(Editor::IObjectPropertiesViewer)
{
    BASE_CLASS(o2::IObject);
}
END_META;

CLASS_FIELDS_META(Editor::IObjectPropertiesViewer)
{
    FIELD().PUBLIC().NAME(onChanged);
    FIELD().PUBLIC().NAME(onChangeCompleted);
    FIELD().PUBLIC().NAME(path);
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<Spoiler>()).NAME(mSpoiler);
    FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mHeaderEnabled);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mPropertiesBuilt);
    FIELD().PROTECTED().NAME(mTargetObjects);
    FIELD().PROTECTED().NAME(mPropertiesContext);
    FIELD().PROTECTED().NAME(mOnChildFieldChangeCompleted);
}
END_META;

CLASS_METHODS_META(Editor::IObjectPropertiesViewer)
{
    typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh, const Ref<_tmp1>&);
    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetViewingObjectTypeStatic);
    FUNCTION().PUBLIC().SIGNATURE(void, SetParentContext, Ref<PropertiesContext>);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Spoiler>, GetSpoiler);
    FUNCTION().PUBLIC().SIGNATURE(void, SetHeaderEnabled, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsHeaderEnabled);
    FUNCTION().PUBLIC().SIGNATURE(void, SetExpanded, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsExpanded);
    FUNCTION().PUBLIC().SIGNATURE(void, SetCaption, const WString&);
    FUNCTION().PUBLIC().SIGNATURE(const WString&, GetCaption);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEmpty);
    FUNCTION().PUBLIC().SIGNATURE(void, OnEnabled);
    FUNCTION().PUBLIC().SIGNATURE(void, OnDisabled);
    FUNCTION().PROTECTED().SIGNATURE(Ref<Spoiler>, CreateSpoiler);
    FUNCTION().PROTECTED().SIGNATURE(void, OnHeaderEnableChanged, bool);
    FUNCTION().PROTECTED().SIGNATURE(bool, CheckBuildProperties, const Ref<_tmp1>&);
    FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, const Ref<_tmp1>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnRefreshed, const Ref<_tmp1>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnFree);
    FUNCTION().PROTECTED().SIGNATURE(void, OnFieldChangeCompleted, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;

META_TEMPLATES(typename _object_type)
CLASS_BASES_META(Editor::TObjectPropertiesViewer<_object_type>)
{
    BASE_CLASS(Editor::IObjectPropertiesViewer);
}
END_META;

META_TEMPLATES(typename _object_type)
CLASS_FIELDS_META(Editor::TObjectPropertiesViewer<_object_type>)
{
    FIELD().PROTECTED().NAME(mTypeTargetObjects);
}
END_META;

META_TEMPLATES(typename _object_type)
CLASS_METHODS_META(Editor::TObjectPropertiesViewer<_object_type>)
{
    typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetViewingObjectTypeStatic);
    FUNCTION().PROTECTED().SIGNATURE(void, OnRefreshed, const Ref<_tmp1>&);
}
END_META;
// --- END META ---