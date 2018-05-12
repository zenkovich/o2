#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"
#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class UIHorizontalLayout;
	class UILabel;
	class UIVerticalLayout;
}

// Editor properties window accessor macros
#define o2EditorProperties PropertiesWindow::Instance()

namespace Editor
{
	class IObjectPropertiesViewer;
	struct FieldPropertiesInfo;

	// ------------------------
	// Editor properties window
	// ------------------------
	class PropertiesWindow: public IEditorWindow, public Singleton<PropertiesWindow>
	{
	public:
		Function<void(IPropertyField*)> onFieldChanged;   // Some field changed event
		Function<void()>                onTargetsChanged; // Targets changing event

		// Default constructor
		PropertiesWindow();

		// Destructor
		~PropertiesWindow();

		// Resets targets objects
		void ResetTargets();

		// Sets target object
		void SetTarget(IObject* target);

		// Sets target objects
		void SetTargets(const Vector<IObject*> targets, const Function<void()>& targetsChangedDelegate = Function<void()>());

		// Returns target object
		Vector<IObject*> GetTargets() const;

		// Updates window: check next viewer and targets and updates current viewer
		void Update(float dt) override;

		// Draws current viewer
		void Draw() override;

		// Returns is targets changed
		bool IsTargetsChanged() const;

		// Sets private field viewing state
		void SetPrivateFieldsVisible(bool visible);

		// Returns is private fields visible
		bool IsPrivateFieldsVisible() const;

		// Checks that property with type can be created
		bool IsFieldTypeSupported(const Type* type) const;

		// Free properties and putin cache
		void FreeProperties(FieldPropertiesInfo& propertiesInfo);

		// Builds layout viewer by type for objects
		void BuildObjectProperties(UIVerticalLayout* layout, const Type* type, FieldPropertiesInfo& propertiesInfo,
								   const String& path);

		// Builds layout viewer by fields
		void BuildObjectProperties(UIVerticalLayout* layout, Vector<FieldInfo*> fields, FieldPropertiesInfo& propertiesInfo,
								   const String& path);

		// Build layout viewer for field
		void BuildField(UIVerticalLayout* layout, FieldInfo* fieldInfo, FieldPropertiesInfo& propertiesInfo,
						const String& path);

	    // Creates field property by type field info
		Pair<IPropertyField*, UIWidget*> CreateFieldProperty(const Type* type, const String& name);

		// Checks is property visible in properties window: 
		// checking protection section, ignore and editor attributes
		bool IsPropertyVisible(FieldInfo* info, bool allowPrivate) const;

		// Checks is property visible in properties window using current private visibility state: 
		// checking protection section, ignore and editor attributes
		bool IsPropertyVisible(FieldInfo* info) const;

		// Returns available field by type
		IPropertyField* GetFieldPropertyPrototype(const Type* type) const;

		// Creates regular primitive property field
		Pair<IPropertyField*, UIWidget*> CreateRegularField(const Type* fieldPropertyType, const String& name);

		// Creates object field
		Pair<IPropertyField*, UIWidget*> CreateObjectField(const Type* type, const String& name);

		// Creates object pointer field
		Pair<IPropertyField*, UIWidget*> CreateObjectPtrField(const Type* type, const String& name);

		// Creates vector field
		Pair<IPropertyField*, UIWidget*> CreateVectorField(const Type* type, const String& name);

		// Makes smarter field name
		static String MakeSmartFieldName(const String& fieldName);

		IOBJECT(PropertiesWindow);

	protected:
		typedef Vector<IObjectPropertiesViewer*> PropViewersVec;
		typedef Vector<IPropertyField*> PropertiesFieldsVec;

		int                      mPropertyFieldsPoolStep = 5; // Field properties pools resize step

		bool                     mPrivateVisible = false;     // Is private fields visible

		Vector<IObject*>         mTargets;                    // Target objects
		IObjectPropertiesViewer* mCurrentViewer = nullptr;    // Current properties viewer
		PropViewersVec           mViewers;                    // All available object types viewers

		PropertiesFieldsVec      mAvailablePropertiesFields;  // Available properties fields

		Function<void()>         mOnTargetsChangedDelegate;   // It is called when targets array changing
		bool                     mTargetsChanged = false;     // True when targets was changed    

	protected:
		// Initializes window
		void InitializeWindow();

		// Initializes window context menu
		void InitializeWindowContext();

		// Initializes viewers
		void InitializeViewers();

		// Initializes available properties fields and pools
		void InitializePropertiesFields();

		// Removes all cached viewers
		void ClearViewers();

		// Builds layout viewer by fields without filtering
		void BuildFields(UIVerticalLayout* layout, Vector<FieldInfo*> fields, FieldPropertiesInfo& propertiesInfo,
						 const String& path);

		// It is called when some property field was changed
		void OnPropertyChanged(IPropertyField* field);
	};
}

CLASS_BASES_META(Editor::PropertiesWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
	BASE_CLASS(o2::Singleton<PropertiesWindow>);
}
END_META;
CLASS_FIELDS_META(Editor::PropertiesWindow)
{
	PUBLIC_FIELD(onFieldChanged);
	PUBLIC_FIELD(onTargetsChanged);
	PROTECTED_FIELD(mPropertyFieldsPoolStep);
	PROTECTED_FIELD(mPrivateVisible);
	PROTECTED_FIELD(mTargets);
	PROTECTED_FIELD(mCurrentViewer);
	PROTECTED_FIELD(mViewers);
	PROTECTED_FIELD(mAvailablePropertiesFields);
	PROTECTED_FIELD(mOnTargetsChangedDelegate);
	PROTECTED_FIELD(mTargetsChanged);
}
END_META;
CLASS_METHODS_META(Editor::PropertiesWindow)
{

	typedef Pair<IPropertyField*, UIWidget*> _tmp1;
	typedef Pair<IPropertyField*, UIWidget*> _tmp2;
	typedef Pair<IPropertyField*, UIWidget*> _tmp3;
	typedef Pair<IPropertyField*, UIWidget*> _tmp4;
	typedef Pair<IPropertyField*, UIWidget*> _tmp5;

	PUBLIC_FUNCTION(void, ResetTargets);
	PUBLIC_FUNCTION(void, SetTarget, IObject*);
	PUBLIC_FUNCTION(void, SetTargets, const Vector<IObject*>, const Function<void()>&);
	PUBLIC_FUNCTION(Vector<IObject*>, GetTargets);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsTargetsChanged);
	PUBLIC_FUNCTION(void, SetPrivateFieldsVisible, bool);
	PUBLIC_FUNCTION(bool, IsPrivateFieldsVisible);
	PUBLIC_FUNCTION(bool, IsFieldTypeSupported, const Type*);
	PUBLIC_FUNCTION(void, FreeProperties, FieldPropertiesInfo&);
	PUBLIC_FUNCTION(void, BuildObjectProperties, UIVerticalLayout*, const Type*, FieldPropertiesInfo&, const String&);
	PUBLIC_FUNCTION(void, BuildObjectProperties, UIVerticalLayout*, Vector<FieldInfo*>, FieldPropertiesInfo&, const String&);
	PUBLIC_FUNCTION(void, BuildField, UIVerticalLayout*, FieldInfo*, FieldPropertiesInfo&, const String&);
	PUBLIC_FUNCTION(_tmp1, CreateFieldProperty, const Type*, const String&);
	PUBLIC_FUNCTION(bool, IsPropertyVisible, FieldInfo*, bool);
	PUBLIC_FUNCTION(bool, IsPropertyVisible, FieldInfo*);
	PUBLIC_FUNCTION(IPropertyField*, GetFieldPropertyPrototype, const Type*);
	PUBLIC_FUNCTION(_tmp2, CreateRegularField, const Type*, const String&);
	PUBLIC_FUNCTION(_tmp3, CreateObjectField, const Type*, const String&);
	PUBLIC_FUNCTION(_tmp4, CreateObjectPtrField, const Type*, const String&);
	PUBLIC_FUNCTION(_tmp5, CreateVectorField, const Type*, const String&);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeWindowContext);
	PROTECTED_FUNCTION(void, InitializeViewers);
	PROTECTED_FUNCTION(void, InitializePropertiesFields);
	PROTECTED_FUNCTION(void, ClearViewers);
	PROTECTED_FUNCTION(void, BuildFields, UIVerticalLayout*, Vector<FieldInfo*>, FieldPropertiesInfo&, const String&);
	PROTECTED_FUNCTION(void, OnPropertyChanged, IPropertyField*);
}
END_META;
