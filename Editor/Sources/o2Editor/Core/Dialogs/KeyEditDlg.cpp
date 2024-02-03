#include "o2Editor/stdafx.h"
#include "KeyEditDlg.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Scene/UI/Widgets/Window.h"
#include "o2Editor/Core/UIRoot.h"

DECLARE_SINGLETON(Editor::KeyEditDlg);

namespace Editor
{

	KeyEditDlg::KeyEditDlg()
	{
		mWindow = DynamicCast<o2::Window>(EditorUIRoot.AddWidget(mmake<o2::Window>("Edit key")));

		auto verLayout = mmake<o2::VerticalLayout>();
		verLayout->spacing = 10;
		verLayout->expandHeight = false;
		verLayout->fitByChildren = false;
		*verLayout->layout = WidgetLayout::BothStretch(0, 30, 0, 0);
		mWindow->AddChild(verLayout);

		// Position and value
		{
			auto horLayout = mmake<o2::HorizontalLayout>();
			horLayout->spacing = 10;
			horLayout->expandHeight = false;
			horLayout->fitByChildren = true;

			{
				auto positionVerLayout = mmake<o2::VerticalLayout>();
				positionVerLayout->expandHeight = false;
				positionVerLayout->fitByChildren = true;
				positionVerLayout->AddChild(mmake<o2::Label>("Position"));

				mPosition = mmake<o2::EditBox>("singleline");
				mPosition->name = "position";
				mPosition->SetFilterFloat();
				positionVerLayout->AddChild(mPosition);
				horLayout->AddChild(positionVerLayout);
			}

			{
				auto valueVerLayout = mmake<o2::VerticalLayout>();
				valueVerLayout->expandHeight = false;
				valueVerLayout->fitByChildren = true;
				valueVerLayout->AddChild(mmake<o2::Label>("Value"));

				mValue = mmake<o2::EditBox>("singleline");
				mValue->SetFilterFloat();
				mValue->name = "value";
				valueVerLayout->AddChild(mValue);
				horLayout->AddChild(valueVerLayout);
			}

			verLayout->AddChild(horLayout);
		}

		// left support
		{
			auto label = mmake<o2::Label>("Left support:");
			auto horLayout = mmake<o2::HorizontalLayout>();
			horLayout->spacing = 10;
			horLayout->expandHeight = false;
			horLayout->fitByChildren = true;

			{
				auto positionVerLayout = mmake<o2::VerticalLayout>();
				positionVerLayout->expandHeight = false;
				positionVerLayout->fitByChildren = true;
				positionVerLayout->AddChild(mmake<o2::Label>("Position"));

				mLeftSupportPosition = mmake<o2::EditBox>("singleline");
				mLeftSupportPosition->name = "leftSupportPosition";
				mLeftSupportPosition->SetFilterFloat();
				positionVerLayout->AddChild(mLeftSupportPosition);
				horLayout->AddChild(positionVerLayout);
			}

			{
				auto valueVerLayout = mmake<o2::VerticalLayout>();
				valueVerLayout->expandHeight = false;
				valueVerLayout->fitByChildren = true;
				valueVerLayout->AddChild(mmake<o2::Label>("Value"));

				mLeftSupportValue = mmake<o2::EditBox>("singleline");
				mLeftSupportValue->name = "leftSupportValue";
				mLeftSupportValue->SetFilterFloat();
				valueVerLayout->AddChild(mLeftSupportValue);
				horLayout->AddChild(valueVerLayout);
			}

			verLayout->AddChild(label);
			verLayout->AddChild(horLayout);
		}

		// right support
		{
			auto label = mmake<o2::Label>("Right support:");
			auto horLayout = mmake<o2::HorizontalLayout>();
			horLayout->spacing = 10;
			horLayout->expandHeight = false;
			horLayout->fitByChildren = true;

			{
				auto positionVerLayout = mmake<o2::VerticalLayout>();
				positionVerLayout->expandHeight = false;
				positionVerLayout->fitByChildren = true;
				positionVerLayout->AddChild(mmake<o2::Label>("Position"));

				mRightSupportPosition = mmake<o2::EditBox>("singleline");
				mRightSupportPosition->name = "rightSupportPosition";
				mRightSupportPosition->SetFilterFloat();
				positionVerLayout->AddChild(mRightSupportPosition);
				horLayout->AddChild(positionVerLayout);
			}

			{
				auto valueVerLayout = mmake<o2::VerticalLayout>();
				valueVerLayout->expandHeight = false;
				valueVerLayout->fitByChildren = true;
				valueVerLayout->AddChild(mmake<o2::Label>("Value"));

				mRightSupportValue = mmake<o2::EditBox>("singleline");
				mRightSupportValue->name = "rightSupportValue";
				mRightSupportValue->SetFilterFloat();
				valueVerLayout->AddChild(mRightSupportValue);
				horLayout->AddChild(valueVerLayout);
			}

			verLayout->AddChild(label);
			verLayout->AddChild(horLayout);
		}

		{
			auto horLayout = mmake<o2::HorizontalLayout>();
			horLayout->spacing = 10;
			horLayout->expandHeight = true;
			*horLayout->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 25);

			auto okButton = mmake<o2::Button>("Ok", MakeFunction(this, &KeyEditDlg::OnOkPressed));
			horLayout->AddChild(okButton);

			auto cancelButton = mmake<o2::Button>("Cancel", [&]() { mWindow->Hide(); });
			horLayout->AddChild(cancelButton);

			mWindow->AddChild(horLayout);
		}

		mWindow->Hide(true);
		mWindow->layout->size = Vec2F(200, 300);

		mWindow->GetBackCursorListener().onCursorReleased = [&](const Input::Cursor& c) { mWindow->Hide(); };
	}

	KeyEditDlg::~KeyEditDlg()
	{
		// Smart pointers will handle the deletion
	}

	void KeyEditDlg::OnOkPressed()
	{
		// Do something
	}
}Replace raw pointers with Ref<> and const Ref<>& for function arguments. Remove the default nullptr value for class members with type Ref<>. Use WeakRef<> for "parent" class variables. Use mmake<>() to create Ref<> instead of make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

```
#include <Ref>

neyEditDlg::Show(const Curve::Key& key, const Function<void(const Curve::Key& key)>& onClosed)
{
    Instance().mPosition->SetText(String(key.position));
    Instance().mValue->SetText(String(key.value));
    Instance().mLeftSupportPosition->SetText(String(key.leftSupportPosition));
    Instance().mLeftSupportValue->SetText(String(key.leftSupportValue));
    Instance().mRightSupportPosition->SetText(String(key.rightSupportPosition));
    Instance().mRightSupportValue->SetText(String(key.rightSupportValue));

    Instance().mOnClosed = onClosed;

    Instance().mWindow->ShowModal();
}

void KeyEditDlg::OnOkPressed()
{
    Curve::Key key;
    key.position = (float)mPosition->GetText();
    key.value = (float)mValue->GetText();
    key.leftSupportPosition = (float)mLeftSupportPosition->GetText();
    key.leftSupportValue = (float)mLeftSupportValue->GetText();
    key.rightSupportPosition = (float)mRightSupportPosition->GetText();
    key.rightSupportValue = (float)mRightSupportValue->GetText();

    mOnClosed(key);
    mWindow->Hide();
}
```