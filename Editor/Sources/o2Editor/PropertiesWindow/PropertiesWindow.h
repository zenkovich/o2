#include "o2/Utils/Singleton.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"
#include "o2/Utils/Ref.h"
#include <functional>

using namespace o2;

namespace o2
{
	class HorizontalLayout;
	class Label;
	class VerticalLayout;
}

// Editor properties window accessor macros
#define o2EditorPropertiesWindow PropertiesWindow::Instance()

namespace Editor
{
	class DefaultPropertiesViewer;
	class IPropertiesViewer;
	class PropertiesContext;

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
		void SetTarget(const Ref<IObject>& target);

		// Sets target objects
		void SetTargets(const Vector<Ref<IObject>>& targets, const Function<void()>& targetsChangedDelegate = Function<void()>());

		// Returns target object
		Vector<Ref<IObject>> GetTargets() const;

		// Updates window: check next viewer and targets and updates current viewer
		void Update(float dt) override;

		// Draws current viewer
		void Draw() override;

		// Returns is targets changed
		bool IsTargetsChanged() const;

		IOBJECT(PropertiesWindow);

	protected:
		Vector<Ref<IObject>> mTargets; // Target objects

		Ref<IPropertiesViewer>         mCurrentViewer; // Current properties viewer
		Vector<Ref<IPropertiesViewer>> mViewers;                 // All available object types viewers
		Ref<DefaultPropertiesViewer>   mDefaultViewer; // Default properties viewer

		Function<void()> mOnTargetsChangedDelegate; // Called when targets array changing
		bool             mTargetsChanged = false;   // True when targets was changed    

		float mRefreshDelay = 0.5f;         // Values refreshing delay
		float mRefreshRemainingTime = 0.5f; // Time to next values refreshing

	protected:
		// Initializes window
		void InitializeWindow();

		// Initializes window context menu
		void InitializeWindowContext();

		// Initializes viewers
		void InitializeViewers();

		// Called when private fields visibility changed
		void OnPrivateFieldsVisibleChanged(bool visible);

		// Called when some property field was changed
		void OnPropertyChanged(const Ref<IPropertyField>& field);
	};
}
// --- META ---

CLASS_BASES_META(Editor::PropertiesWindow)
{
    BASE_CLASS(Editor::IEditorWindow);
    BASE_CLASS(o2::Singleton<PropertiesWindow>);
}
END_META;
CLASS_FIELDS_META(Editor::PropertiesWindow)
{
    FIELD().PROTECTED().NAME(mTargets);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mCurrentViewer);
    FIELD().PROTECTED().NAME(mViewers);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDefaultViewer);
    FIELD().PROTECTED().NAME(mOnTargetsChangedDelegate);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mTargetsChanged);
    FIELD().PROTECTED().DEFAULT_VALUE(0.5f).NAME(mRefreshDelay);
    FIELD().PROTECTED().DEFAULT_VALUE(0.5f).NAME(mRefreshRemainingTime);
}
END_META;
CLASS_METHODS_META(Editor::PropertiesWindow)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, ResetTargets);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTarget, const Ref<IObject>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargets, const Vector<Ref<IObject>>&, const Function<void()>&);
    FUNCTION().PUBLIC().SIGNATURE(Vector<Ref<IObject>>, GetTargets);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsTargetsChanged);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindow);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindowContext);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeViewers);
    FUNCTION().PROTECTED().SIGNATURE(void, OnPrivateFieldsVisibleChanged, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChanged, const Ref<IPropertyField>&);
}
END_META;
// --- END META ---