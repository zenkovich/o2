#pragma once

#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/ValueProxy.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"

using namespace o2;

namespace o2
{
	class VerticalLayout;
}

namespace Editor
{
	class IPropertyField;

	// ----------------------------------------------------------------------------------
	// Object properties viewer interface. Used in IObjectProperty and IObjectPtrProperty
	// Override this class to create new object properties viewer
	// ----------------------------------------------------------------------------------
	class IObjectPropertiesViewer : public IObject
	{
	public:
		typedef Function<void(IPropertyField*)> OnChangedFunc;
		typedef Function<void(const String&, const Vector<DataDocument>&, const Vector<DataDocument>&)> OnChangeCompletedFunc;

	public:
		OnChangedFunc onChanged;                 // Immediate change value by user event
		OnChangeCompletedFunc onChangeCompleted; // Change completed by user event

		String path; // Path to viewing object fields

	public:
		// Default constructor
		IObjectPropertiesViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets);

		// Returns viewing objects base type
		virtual const Type* GetViewingObjectType() const;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		// Sets parent context
		void SetParentContext(PropertiesContext* context);

		// Returns view widget
		Spoiler* GetSpoiler();

		// Sets is header enabled and properties can be collapsed in spoiler
		virtual void SetHeaderEnabled(bool enabled);

		// Returns is header enabled
		bool IsHeaderEnabled() const;

		// Expands or collapses spoiler
		virtual void SetExpanded(bool expanded);

		// Returns is spoiler expanded
		bool IsExpanded() const;

		// Sets spoiler caption
		virtual void SetCaption(const WString& caption);

		// Returns caption
		const WString& GetCaption() const;

		// Returns is viewer empty
		bool IsEmpty() const;

		// This is called when object viewer enabled
		virtual void OnEnabled() {}

		// This is called when object viewer disabled
		virtual void OnDisabled() {}

		IOBJECT(IObjectPropertiesViewer);

	protected:
		Spoiler* mSpoiler = nullptr;    // Properties spoiler. Expands forcible when viewer hasn't header
		bool     mHeaderEnabled = true; // Is header enabled and properties hiding in spoiler

		bool mPropertiesBuilt = false; // True when properties built at first refreshing

		Vector<Pair<IObject*, IObject*>> mTargetObjets; // Target objects

		PropertiesContext mPropertiesContext; // Field properties information

		OnChangeCompletedFunc mOnChildFieldChangeCompleted; // Default field change completed callback, calls
		                                                    // inChangeCompleted from this with full combined path

	protected:
		// Creates spoiler for properties
		virtual Spoiler* CreateSpoiler();

		// It is called when header enable changed
		virtual void OnHeaderEnableChanged(bool enabled) {}

		// Checks if properties need to be rebuilt, rebuilds if necessary; returns true when properties was rebuilt
		virtual bool CheckBuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets);

		// It is called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		virtual void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) {}

		// It is called when viewer is refreshed
		virtual void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) {}

		// This is called when the viewer is freed
		virtual void OnFree() {}

		// It is called when some child field were changed
		void OnFieldChangeCompleted(const String& path, const Vector<DataDocument>& before, 
									const Vector<DataDocument>& after);

		friend class Properties;
	};

	// --------------------------------------
	// Specialize object properties interface
	// --------------------------------------
	template<typename _object_type>
	class TObjectPropertiesViewer : public IObjectPropertiesViewer
	{
	public:
		// Returns viewing objects base type
		const Type* GetViewingObjectType() const override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(TObjectPropertiesViewer<_object_type>);
	};

	template<typename _object_type>
	const Type* TObjectPropertiesViewer<_object_type>::GetViewingObjectType() const
	{
		return GetViewingObjectTypeStatic();
	}

	template<typename _object_type>
	const Type* TObjectPropertiesViewer<_object_type>::GetViewingObjectTypeStatic()
	{
		return &TypeOf(_object_type);
	}

}

CLASS_BASES_META(Editor::IObjectPropertiesViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IObjectPropertiesViewer)
{
	FIELD().NAME(onChanged).PUBLIC();
	FIELD().NAME(onChangeCompleted).PUBLIC();
	FIELD().NAME(path).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSpoiler).PROTECTED();
	FIELD().DEFAULT_VALUE(true).NAME(mHeaderEnabled).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mPropertiesBuilt).PROTECTED();
	FIELD().NAME(mTargetObjets).PROTECTED();
	FIELD().NAME(mPropertiesContext).PROTECTED();
	FIELD().NAME(mOnChildFieldChangeCompleted).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::IObjectPropertiesViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;
	typedef const Vector<Pair<IObject*, IObject*>>& _tmp2;
	typedef const Vector<Pair<IObject*, IObject*>>& _tmp3;
	typedef const Vector<Pair<IObject*, IObject*>>& _tmp4;

	PUBLIC_FUNCTION(void, Refresh, _tmp1);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
	PUBLIC_FUNCTION(void, SetParentContext, PropertiesContext*);
	PUBLIC_FUNCTION(Spoiler*, GetSpoiler);
	PUBLIC_FUNCTION(void, SetHeaderEnabled, bool);
	PUBLIC_FUNCTION(bool, IsHeaderEnabled);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(const WString&, GetCaption);
	PUBLIC_FUNCTION(bool, IsEmpty);
	PUBLIC_FUNCTION(void, OnEnabled);
	PUBLIC_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(Spoiler*, CreateSpoiler);
	PROTECTED_FUNCTION(void, OnHeaderEnableChanged, bool);
	PROTECTED_FUNCTION(bool, CheckBuildProperties, _tmp2);
	PROTECTED_FUNCTION(void, RebuildProperties, _tmp3);
	PROTECTED_FUNCTION(void, OnRefreshed, _tmp4);
	PROTECTED_FUNCTION(void, OnFree);
	PROTECTED_FUNCTION(void, OnFieldChangeCompleted, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
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
}
END_META;
META_TEMPLATES(typename _object_type)
CLASS_METHODS_META(Editor::TObjectPropertiesViewer<_object_type>)
{

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
}
END_META;
