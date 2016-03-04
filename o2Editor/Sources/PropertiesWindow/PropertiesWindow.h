#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"
#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class UILabel;
	class UIHorizontalLayout;
}

class IObjectPropertiesViewer;

// Editor properties window accessor macros
#define o2EditorProperties PropertiesWindow::Instance()

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
	void BuildTypeViewer(UIVerticalLayout* layout, const Type* type, const Vector<IObject*>& objects, 
						 Vector<IEditorPropertyField*>& usedPropertyFields);

	// Makes smarter field name
	String MakeSmartFieldName(const String& fieldName);

	IOBJECT(PropertiesWindow);

protected:
	typedef Vector<IObjectPropertiesViewer*> PropViewersVec;
	typedef Dictionary<const Type*, Vector<IEditorPropertyField*>> TypePropFieldsDict;
	typedef Vector<UILabel*> LabelsVec;
	typedef Vector<UIHorizontalLayout*> HorLayoutsVec;

	int                      mPropertyFieldsPoolStep = 5; // Field properties pools resize step

	Vector<IObject*>         mTargets;                    // Target objects
	IObjectPropertiesViewer* mCurrentViewer;              // Current properties viewer
	PropViewersVec           mViewers;                    // All available object types viewers

	TypePropFieldsDict       mFieldPropertiesPool;        // Field properties pools
	LabelsVec                mLabelsPool;                 // Labels pool
	HorLayoutsVec            mHorLayoutsPool;             // Horizontal layouts pool

protected:
	// Initializes window
	void InitializeWindow();

	// Initializes viewers
	void InitializeViewers();

	// Creates property field by type
	IEditorPropertyField* CreatePropertyField(const Type* type);
};
