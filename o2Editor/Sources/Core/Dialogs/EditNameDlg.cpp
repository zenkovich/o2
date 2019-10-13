#include "stdafx.h"
#include "EditNameDlg.h"

#include "Core/UIRoot.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/Widgets/HorizontalLayout.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/Window.h"

DECLARE_SINGLETON(Editor::NameEditDlg);

namespace Editor
{
	NameEditDlg::NameEditDlg()
	{
		mWindow = dynamic_cast<Window*>(EditorUIRoot.AddWidget(o2UI.CreateWindow("Enter name")));

		InitializeControls();

		mWindow->Hide(true);
		mWindow->layout->size = Vec2F(300, 100);

		mWindow->GetBackCursorListener().onCursorReleased = [&](const Input::Cursor& c) { OnCursorPressedOutside(); };
		mWindow->onHide = MakeFunction(this, &NameEditDlg::OnHide);
	}

	NameEditDlg::~NameEditDlg()
	{
		if (mWindow)
			delete mWindow;
	}

	void NameEditDlg::Show(const String& name, Function<void(const String&)> onCompleted, 
						   Function<void()> onCancelled /*= Function<void()>()*/)
	{
		mInstance->mNameEditBox->SetText(name);
		mInstance->mWindow->ShowModal();
		mInstance->mOnCompletedCallback = onCompleted;
		mInstance->mOnCancelledCallback = onCancelled;
	}

	void NameEditDlg::OnHide()
	{
		mOnCancelledCallback();
	}

	void NameEditDlg::InitializeControls()
	{
		auto verLayout = o2UI.CreateVerLayout();
		verLayout->spacing = 10;

		mNameEditBox = o2UI.CreateEditBox("singleline");
		verLayout->AddChild(mNameEditBox);

		auto horLayout =  o2UI.CreateHorLayout();
		horLayout->spacing = 10;
		
		auto okButton = o2UI.CreateButton("Ok", MakeFunction(this, &NameEditDlg::OnOkPressed));
		horLayout->AddChild(okButton);

		auto cancelButton = o2UI.CreateButton("Cancel", MakeFunction(this, &NameEditDlg::OnCancelPressed));
		horLayout->AddChild(cancelButton);

		verLayout->AddChild(horLayout);

		mWindow->AddChild(verLayout);
	}

	void NameEditDlg::OnOkPressed()
	{
		mOnCompletedCallback(mNameEditBox->GetText());
		mWindow->Hide();
	}

	void NameEditDlg::OnCancelPressed()
	{
		mOnCancelledCallback();
		mWindow->Hide();
	}

	void NameEditDlg::OnCursorPressedOutside()
	{
		mOnCancelledCallback();
		mWindow->Hide();
	}

}
