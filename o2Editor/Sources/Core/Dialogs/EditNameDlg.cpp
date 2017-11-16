#include "EditNameDlg.h"

#include "Core/UIRoot.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/Window.h"

DECLARE_SINGLETON(Editor::NameEditDlg);

namespace Editor
{
	NameEditDlg::NameEditDlg()
	{
		mWindow = dynamic_cast<UIWindow*>(EditorUIRoot.AddWidget(o2UI.CreateWindow("Enter name")));

		InitializeControls();

		mWindow->Hide(true);
		mWindow->layout->size = Vec2F(300, 100);

		mWindow->GetBackCursorListener().onCursorReleased = [&](const Input::Cursor& c) { OnCursorPressedOutside(); };
		mWindow->onHide = Func(this, &NameEditDlg::OnHide);
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
		
		auto okButton = o2UI.CreateButton("Ok", Func(this, &NameEditDlg::OnOkPressed));
		horLayout->AddChild(okButton);

		auto cancelButton = o2UI.CreateButton("Cancel", Func(this, &NameEditDlg::OnCancelPressed));
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