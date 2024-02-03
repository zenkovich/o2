#include "o2Editor/stdafx.h"
#include "EditNameDlg.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Scene/UI/Widgets/Window.h"
#include "o2Editor/Core/UIRoot.h"

DECLARE_SINGLETON(Editor::NameEditDlg);

namespace Editor
{
	NameEditDlg::NameEditDlg()
	{
		mWindow = DynamicCast<o2::Window>(EditorUIRoot.AddWidget(o2UI.CreateWindow("Enter name")));

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

	void NameEditDlg::Show(const String& name, const Function<void(const String&)>& onCompleted,
		const Function<void()>& onCancelled /*= Function<void()>()*/)
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
		auto verLayout = mmake<o2::VerticalLayout>();
		verLayout->spacing = 10;

		mNameEditBox = mmake<o2::EditBox>("singleline");
		verLayout->AddChild(mNameEditBox);

		auto horLayout = mmake<o2::HorizontalLayout>();
		horLayout->spacing = 10;

		auto okButton = mmake<o2::Button>("Ok", MakeFunction(this, &NameEditDlg::OnOkPressed));
		horLayout->AddChild(okButton);

		auto cancelButton = mmake<o2::Button>("Cancel", MakeFunction(this, &NameEditDlg::OnCancelPressed));
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