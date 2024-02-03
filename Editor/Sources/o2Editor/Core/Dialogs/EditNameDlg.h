#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Function/Function.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/SmartPointers/Ref.h"

using namespace o2;

namespace o2
{
	class EditBox;
	class Label;
	class VerticalProgress;
	class Widget;
	class WidgetLayer;
	class Window;
}

namespace Editor
{
	// -------------------
	// Name editing dialog
	// -------------------
	class NameEditDlg : public Singleton<NameEditDlg>, public CursorEventsListener
	{
	public:
		// Default constructor
		NameEditDlg();

		// Destructor
		~NameEditDlg();

		// Shows name editing window. Delegate onCompleted is called when ok pressed and onCancelled when cancel
		static void Show(const String& name,
			const Function<void(const String&)>& onCompleted,
			const Function<void()>& onCancelled = Function<void()>());

	protected:
		Function<void(const String&)> mOnCompletedCallback;
		Function<void()> mOnCancelledCallback;


		Ref<Window> mWindow;
		Ref<EditBox> mNameEditBox;

	protected:
		// Calls when hiding dialog
		void OnHide();

		// Initializes edit box and buttons
		void InitializeControls();

		// Called when Ok button pressed, calls mOnCompletedCallback and closes window
		void OnOkPressed();

		// Called when Cancel button pressed, calls mOnCancelledCallback and closes window
		void OnCancelPressed();

		// Called when cursor pressed outside from window
		void OnCursorPressedOutside();
	};
}