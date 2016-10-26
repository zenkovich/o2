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
		// Default constructor
		PropertiesWindow();

		// Destructor
		~PropertiesWindow();

		// Sets target object
		void SetTarget(IObject* target);

		// Sets target objects
		void SetTargets(const Vector<IObject*> targets);

		// Returns target object
		Vector<IObject*> GetTargets() const;

		// Updates window: check next viewer and targets and updates current viewer
		void Update(float dt);

		// Draws current viewer
		void Draw();

		// Builds layout viewer by type for objects
		void BuildTypeViewer(UIVerticalLayout* layout, const Type* type, FieldPropertiesInfo& propertiesInfo);

		// Makes smarter field name
		String MakeSmartFieldName(const String& fieldName);

		IOBJECT(PropertiesWindow);

	protected:
		typedef Vector<IObjectPropertiesViewer*> PropViewersVec;
		typedef Vector<IPropertyField*> PropertiesFieldsVec;
		typedef Dictionary<const Type*, PropertiesFieldsVec> TypePropFieldsDict;
		typedef Vector<UILabel*> LabelsVec;
		typedef Vector<UIHorizontalLayout*> HorLayoutsVec;
		typedef Vector<UIVerticalLayout*> VerLayoutsVec;

		int                      mPropertyFieldsPoolStep = 5; // Field properties pools resize step

		Vector<IObject*>         mTargets;                    // Target objects
		IObjectPropertiesViewer* mCurrentViewer;              // Current properties viewer
		PropViewersVec           mViewers;                    // All available object types viewers

		PropertiesFieldsVec      mAvailablePropertiesFields;  // Available properties fields

	protected:
		// Initializes window
		void InitializeWindow();

		// Initializes viewers
		void InitializeViewers();

		// Initializes available properties fields and pools
		void InitializePropertiesFields();

		// Creates property field by type
		IPropertyField* CreatePropertyField(const Type* type);

		// Returns available field by type
		IPropertyField* GetAvailableField(const Type* type);

		// Creates regular primitive property field
		void CreateRegularField(IPropertyField* fieldSample, FieldInfo* fieldInfo, FieldPropertiesInfo& propertiesInfo,
								UIVerticalLayout* layout);

		// Creates field for IObject based
		void CreateObjectField(FieldInfo* fieldInfo, FieldPropertiesInfo& propertiesInfo, UIVerticalLayout* layout);
	};
}