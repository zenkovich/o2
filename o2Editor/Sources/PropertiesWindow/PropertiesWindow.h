#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"
#include "Core/Properties/IPropertyField.h"
#include "Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class UIHorizontalLayout;
	class UILabel;
	class UIVerticalLayout;
}

// Editor properties window accessor macros
#define o2EditorPropertiesWindow PropertiesWindow::Instance()

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

		IOBJECT(PropertiesWindow);

	protected:
		typedef Vector<IObjectPropertiesViewer*> PropViewersVec;

		Vector<IObject*>         mTargets;                    // Target objects
		IObjectPropertiesViewer* mCurrentViewer = nullptr;    // Current properties viewer
		PropViewersVec           mViewers;                    // All available object types viewers

		Function<void()>         mOnTargetsChangedDelegate;   // It is called when targets array changing
		bool                     mTargetsChanged = false;     // True when targets was changed    

	protected:
		// Initializes window
		void InitializeWindow();

		// Initializes window context menu
		void InitializeWindowContext();

		// Initializes viewers
		void InitializeViewers();

		// Removes all cached viewers
		void ClearViewers();

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
	PROTECTED_FIELD(mTargets);
	PROTECTED_FIELD(mCurrentViewer);
	PROTECTED_FIELD(mViewers);
	PROTECTED_FIELD(mOnTargetsChangedDelegate);
	PROTECTED_FIELD(mTargetsChanged);
}
END_META;
CLASS_METHODS_META(Editor::PropertiesWindow)
{

	PUBLIC_FUNCTION(void, ResetTargets);
	PUBLIC_FUNCTION(void, SetTarget, IObject*);
	PUBLIC_FUNCTION(void, SetTargets, const Vector<IObject*>, const Function<void()>&);
	PUBLIC_FUNCTION(Vector<IObject*>, GetTargets);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsTargetsChanged);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeWindowContext);
	PROTECTED_FUNCTION(void, InitializeViewers);
	PROTECTED_FUNCTION(void, ClearViewers);
	PROTECTED_FUNCTION(void, OnPropertyChanged, IPropertyField*);
}
END_META;
