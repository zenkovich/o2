#pragma once
#include "o2/Scene/UI/Widgets/VerticalLayout.h"

using namespace o2;

namespace Editor
{
	class IObjectPropertiesViewer;
	class PropertiesContext;

	// -----------------------------------------------------------------
	// Basic automatic object viewer. Selects suitable properties viewer
	// -----------------------------------------------------------------
	class ObjectViewer: public VerticalLayout
	{
	public:
		// Default constructor, creates view widget as vertical layout
		ObjectViewer();

		// Refreshing controls and properties by target objects with prototypes
		void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets);

		// Refreshing controls and properties by target objects
		void Refresh(const Vector<IObject*>& targetObjets);

		// Sets parent context
		void SetParentContext(PropertiesContext* context);

		// Enable viewer event function
		void OnEnabled() override;

		// Disable viewer event function
		void OnDisabled() override;

		// Returns viewing objects type
		const Type* GetViewingObjectType() const;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(ObjectViewer);

	protected:
		PropertiesContext* mParentContext = nullptr; // Parent properties context

		IObjectPropertiesViewer* mPropertiesViewer = nullptr; // Object properties viewer
	};
}

CLASS_BASES_META(Editor::ObjectViewer)
{
	BASE_CLASS(o2::VerticalLayout);
}
END_META;
CLASS_FIELDS_META(Editor::ObjectViewer)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mParentContext);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPropertiesViewer);
}
END_META;
CLASS_METHODS_META(Editor::ObjectViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh, _tmp1);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh, const Vector<IObject*>&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetParentContext, PropertiesContext*);
	FUNCTION().PUBLIC().SIGNATURE(void, OnEnabled);
	FUNCTION().PUBLIC().SIGNATURE(void, OnDisabled);
	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
}
END_META;
