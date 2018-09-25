#pragma once

#include "Core/EditorApplication.h"
#include "Core/Properties/FieldPropertiesInfo.h"
#include "Core/Properties/IPropertyField.h"
#include "Utils/Delegates.h"
#include "Utils/Singleton.h"
#include "Utils/Types/String.h"

using namespace o2;

namespace o2
{
	class UIVerticalLayout;
	class UIWidget;
}

// Editor properties window accessor macros
#define o2EditorProperties Editor::Properties::Instance()

namespace Editor
{
	// ------------------------------------------------------------------------------
	// Editor properties building utility. Can cache, build and reuse property fields
	// ------------------------------------------------------------------------------
	class Properties : public Singleton<Properties>
	{
	public:
		Function<void(IPropertyField*)> onFieldChanged;   // Some field changed event

		// Default constructor. Initializes properties samples and fills pools
		Properties();

		// Destructor
		~Properties();

		// Sets private field viewing state
		void SetPrivateFieldsVisible(bool visible);

		// Returns is private fields visible
		bool IsPrivateFieldsVisible() const;

		// Free properties and put in cache
		void FreeProperties(FieldPropertiesInfo& propertiesInfo);

		// Free property field and put in cache
		void FreeProperty(IPropertyField* field);

		// Builds layout viewer by type for objects
		void BuildObjectProperties(UIVerticalLayout* layout, const Type* type, FieldPropertiesInfo& propertiesInfo, const String& path,
								   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
								   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Builds layout viewer by fields
		void BuildObjectProperties(UIVerticalLayout* layout, Vector<FieldInfo*> fields, FieldPropertiesInfo& propertiesInfo, const String& path,
								   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
								   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Build layout viewer for field
		IPropertyField* BuildField(UIVerticalLayout* layout, FieldInfo* fieldInfo, FieldPropertiesInfo& propertiesInfo, const String& path,
								   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
								   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Build layout viewer for field
		IPropertyField* BuildField(UIVerticalLayout* layout, const Type& objectType, const String& fieldName, const String& path, 
								   FieldPropertiesInfo& propertiesInfo,
								   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
								   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Checks that property with type can be created
		bool IsFieldTypeSupported(const Type* type) const;

		// Checks is property visible in properties window: 
		// checking protection section, ignore and editor attributes
		bool IsPropertyVisible(FieldInfo* info, bool allowPrivate) const;

		// Checks is property visible in properties window using current private visibility state: 
		// checking protection section, ignore and editor attributes
		bool IsPropertyVisible(FieldInfo* info) const;

		// Returns available field by type
		IPropertyField* GetFieldPropertyPrototype(const Type* type) const;

		// Creates field property by type field info
		IPropertyField* CreateFieldProperty(const Type* type, const String& name,
											const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
											const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Creates regular primitive property field
		IPropertyField* CreateRegularField(const Type* fieldPropertyType, const String& name,
										   const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
										   const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Creates object field
		IPropertyField* CreateObjectField(const Type* type, const String& name,
										  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
										  const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Creates object pointer field
		IPropertyField* CreateObjectPtrField(const Type* type, const String& name,
											 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
											 const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Creates vector field
		IPropertyField* CreateVectorField(const Type* type, const String& name,
										  const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
										  const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);

		// Makes smarter field name
		static String MakeSmartFieldName(const String& fieldName);

	protected:
		typedef Vector<IPropertyField*> PropertiesFieldsVec;
		typedef Dictionary<const Type*, PropertiesFieldsVec> TypePropertyDict;

		int  mPropertyFieldsPoolStep = 5; // Field properties pools resize step						    
		bool mPrivateVisible = false;     // Is private fields visible

		PropertiesFieldsVec mAvailablePropertiesFields;  // Available properties fields

		TypePropertyDict mPropertiesPool; // Pool of properties, grouped by property type

		static IPropertyField::OnChangeCompletedFunc mOnPropertyCompletedChangingUndoCreateDelegate;

	protected:
		// Initializes available properties fields and pools
		void InitializePropertiesFields();

		// Builds layout viewer by fields without filtering
		void BuildFields(UIVerticalLayout* layout, Vector<FieldInfo*> fields, FieldPropertiesInfo& propertiesInfo, const String& path,
						 const IPropertyField::OnChangeCompletedFunc& onChangeCompleted = mOnPropertyCompletedChangingUndoCreateDelegate,
						 const IPropertyField::OnChangedFunc& onChanged = IPropertyField::OnChangedFunc::empty);
	};
}
