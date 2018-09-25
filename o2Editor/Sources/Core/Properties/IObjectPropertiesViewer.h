#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Types/Containers/Pair.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/ValueProxy.h"

using namespace o2;

namespace o2
{
	class UIVerticalLayout;
}

namespace Editor
{
	class IPropertyField;

	// ----------------------------------------------------------------------------------
	// Object properties viewer interface. Used in IObjectProperty and IObjectPtrProperty
	// Override this class to create new object properties viewer
	// ----------------------------------------------------------------------------------
	class IObjectPropertiesViewer: public IObject
	{
	public:
		typedef Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>> TargetsVec;

		typedef Function<void(IPropertyField*)> OnChangedFunc;
		typedef Function<void(const String&, const Vector<DataNode>&, const Vector<DataNode>&)> OnChangeCompletedFunc;

	public:
		// Initializes controls and properties inside layout, with specialized type and change delegates
		virtual void InitializeControls(UIVerticalLayout* layout, const Type& objectType, 
										const OnChangeCompletedFunc& onChangeCompleted, 
										const OnChangedFunc& onChanged);

		// Refreshing controls and properties by target objects
		virtual void Refresh(const TargetsVec& targetObjets);

		// Returns specialized viewing object type
		virtual const Type& GetSpecializedObjectType() const;

		// Returns viewing objects base type
		virtual const Type* GetViewingObjectType() const;

		IOBJECT(IObjectPropertiesViewer);

	protected:
		const Type* mSpecializedType = nullptr; // Specialized viewing object type

		// Sets value via proxy
		template<typename T>
		void SetProxy(IAbstractValueProxy* proxy, const T& value) const;

		// Returns value from proxy
		template<typename T>
		T GetProxy(IAbstractValueProxy* proxy) const;
	};

	template<typename T>
	T IObjectPropertiesViewer::GetProxy(IAbstractValueProxy* proxy) const
	{
		T res;
		proxy->GetValuePtr(&res);
		return res;
	}

	template<typename T>
	void IObjectPropertiesViewer::SetProxy(IAbstractValueProxy* proxy, const T& value) const
	{
		proxy->SetValuePtr(&const_cast<T&>(value));
	}

}

CLASS_BASES_META(Editor::IObjectPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IObjectPropertiesViewer)
{
	PROTECTED_FIELD(mSpecializedType);
}
END_META;
CLASS_METHODS_META(Editor::IObjectPropertiesViewer)
{

	PUBLIC_FUNCTION(void, InitializeControls, UIVerticalLayout*, const Type&, const OnChangeCompletedFunc&, const OnChangedFunc&);
	PUBLIC_FUNCTION(void, Refresh, const TargetsVec&);
	PUBLIC_FUNCTION(const Type&, GetSpecializedObjectType);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
}
END_META;
