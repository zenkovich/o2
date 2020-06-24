#pragma once

#include "o2/Utils/Delegates.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/String.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"

using namespace o2;

namespace o2
{
	class VerticalLayout;
	class Widget;
}

// Editor properties window accessor macros
#define o2EditorProperties Editor::Properties::Instance()

namespace Editor
{
	class IObjectPropertiesViewer;

	// ------------------------------------------------------------------------------
	// Editor properties building utility. Can cache, build and reuse property fields
	// ------------------------------------------------------------------------------
	class Properties: public Singleton<Properties>
	{
	public:
		Function<void(IPropertyField*)> onFieldChanged; // Some field changed event

	public:
		// Default constructor. Initializes properties samples and fills pools
		Properties();

		// Destructor
		~Properties();

		// Sets private field viewing state
		void SetPrivateFieldsVisible(bool visible);

		// Returns is private fields visible
		bool IsPrivateFieldsVisible() const;

		// Free properties and put in cache
		void FreeProperties(PropertiesContext& context);

		// Free property field and put in cache
		void FreeProperty(IPropertyField* field);

		// Builds layout viewer by type for objects
		void BuildObjectProperties(VerticalLayout* layout, const Type* type, PropertiesContext& context, const String& path,
								   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
								   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Builds layout viewer by fields
		void BuildObjectProperties(VerticalLayout* layout, Vector<const FieldInfo*> fields, PropertiesContext& context, const String& path,
								   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
								   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Build layout viewer for field
		IPropertyField* BuildField(VerticalLayout* layout, const FieldInfo* fieldInfo, PropertiesContext& context, const String& path,
								   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
								   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Build layout viewer for field
		IPropertyField* BuildField(VerticalLayout* layout, const Type& objectType, const String& fieldName, const String& path,
								   PropertiesContext& context,
								   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
								   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Build layout viewer for fields
		void BuildFields(VerticalLayout* layout, const Type& objectType, const Vector<String>& fieldsNames, const String& path,
										  PropertiesContext& context,
										  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
										  const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Build layout viewer for field
		template<typename PropertyFieldType>
		PropertyFieldType* BuildFieldType(VerticalLayout* layout, const FieldInfo* fieldInfo, PropertiesContext& context, const String& path,
										  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
										  const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Build layout viewer for field
		template<typename PropertyFieldType>
		PropertyFieldType* BuildFieldType(VerticalLayout* layout, const Type& objectType, const String& fieldName, const String& path,
										  PropertiesContext& context,
										  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
										  const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Checks that property with type can be created
		bool IsFieldTypeSupported(const Type* type) const;

		// Checks is property visible in properties window: 
		// checking protection section, ignore and editor attributes
		bool IsPropertyVisible(const FieldInfo* info, bool allowPrivate) const;

		// Checks is property visible in properties window using current private visibility state: 
		// checking protection section, ignore and editor attributes
		bool IsPropertyVisible(const FieldInfo* info) const;

		// Returns available field by type
		const Type* GetFieldPropertyType(const Type* valueType) const;

		// Creates field property by type 
		IPropertyField* CreateFieldProperty(const Type* type, const String& name,
											const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
											const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Creates regular primitive property field
		IPropertyField* CreateRegularField(const Type* type, const String& name,
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
										   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Creates regular enumerate primitive property field
		IPropertyField* CreateEnumField(const Type* type, const String& name,
										const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
										const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Creates object field
		IPropertyField* CreateObjectField(const String& name,
										  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
										  const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Creates object pointer field
		IPropertyField* CreateObjectPtrField(const ObjectType* basicType, const String& name,
											 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
											 const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Creates vector field
		IPropertyField* CreateVectorField(const Type* type, const String& name,
										  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
										  const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Returns object properties viewer
		IObjectPropertiesViewer* CreateObjectViewer(const Type* type, const String& path,
													const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
													const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Free object viewer, store in pool for reuse
		void FreeObjectViewer(IObjectPropertiesViewer* viewer);

		// Makes smarter field name
		static String MakeSmartFieldName(const String& fieldName);

	protected:
		typedef Map<const Type*, const Type*> PropertiesFieldsMap;
		typedef Map<const Type*, Vector<IPropertyField*>> TypePropertyMap;

		typedef Map<const Type*, const Type*> IObjectPropertiesViewersMap;
		typedef Map<const Type*, Vector<IObjectPropertiesViewer*>> TypeObjectPropertiesViewerMap;

		int  mPropertyFieldsPoolStep = 5; // Field properties pools resize step						    
		bool mPrivateVisible = false;     // Is private fields visible

		PropertiesFieldsMap         mAvailablePropertiesFields;        // Available properties fields samples
		IObjectPropertiesViewersMap mAvailableObjectPropertiesViewers; // Available object properties viewers samples

		TypePropertyMap               mPropertiesPool;              // Pool of properties, grouped by property type
		TypeObjectPropertiesViewerMap mObjectPropertiesViewersPool; // Pool of object properties viewers, grouped by object type

		static IPropertyField::OnChangeCompletedFunc mOnPropertyCompletedChangingUndoCreateDelegate;

	protected:
		// Initializes available properties fields samples
		void InitializeAvailablePropertiesFields();

		// Initializes available object properties viewers samples
		void InitializeAvailableObjectPropertiesViewers();

		// Returns type of object viewer, which is closest ty type by based types hierarchy depth
		const Type* GetClosesBasedTypeObjectViewer(const Type* type) const;

		// Builds layout viewer by fields without filtering
		void BuildFields(VerticalLayout* layout, Vector<const FieldInfo*> fields, PropertiesContext& context, const String& path,
						 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
						 const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);
	};

	template<typename PropertyFieldType>
	PropertyFieldType* Properties::BuildFieldType(VerticalLayout* layout, const FieldInfo* fieldInfo, 
												  PropertiesContext& context, const String& path, 
												  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/, 
												  const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		return dynamic_cast<PropertyFieldType*>(BuildField(layout, fieldInfo, context, path, onChangeCompleted, onChanged));
	}

	template<typename PropertyFieldType>
	PropertyFieldType* Properties::BuildFieldType(VerticalLayout* layout, const Type& objectType, const String& fieldName, 
												  const String& path, PropertiesContext& context, 
												  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted /*= mOnPropertyCompletedChangingUndoCreateDelegate*/, 
												  const IPropertyField::OnChangedFunc& onChanged /*= IPropertyField::OnChangedFunc::empty*/)
	{
		return dynamic_cast<PropertyFieldType*>(BuildField(layout, objectType, fieldName, path, context, onChangeCompleted, onChanged));
	}

}
