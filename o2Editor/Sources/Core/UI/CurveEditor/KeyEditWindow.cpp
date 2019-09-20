#include "stdafx.h"
#include "KeyEditWindow.h"

#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/Widgets/HorizontalLayout.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "Scene/UI/Widgets/Button.h"

namespace Editor
{

	KeyEditWindow::KeyEditWindow()
	{
		InitializeControls();
	}

	KeyEditWindow::KeyEditWindow(const KeyEditWindow& other):
		Window(other)
	{
		InitializeControls();
	}

	KeyEditWindow& KeyEditWindow::operator=(const KeyEditWindow& other)
	{
		Window::operator=(other);
		InitializeControls();
		return *this;
	}

	void KeyEditWindow::Show(const Curve::Key& key, const Function<void(const Curve::Key& key)>& onClosed)
	{
		mPosition->SetText(String(key.position));
		mValue->SetText(String(key.value));
		mLeftSupportPosition->SetText(String(key.leftSupportPosition));
		mLeftSupportValue->SetText(String(key.leftSupportValue));
		mRightSupportPosition->SetText(String(key.rightSupportPosition));
		mRightSupportValue->SetText(String(key.rightSupportValue));

		mOnClosed = onClosed;
	}

	void KeyEditWindow::InitializeControls()
	{
		auto verLayout = o2UI.CreateVerLayout();
		verLayout->spacing = 10;
		verLayout->expandHeight = false;
		verLayout->fitByChildren = true;

		// Position and value
		{
			auto horLayout = o2UI.CreateHorLayout();
			horLayout->spacing = 10;
			horLayout->expandHeight = false;
			horLayout->fitByChildren = true;

			{
				auto positionVerLayout = o2UI.CreateVerLayout();
				positionVerLayout->expandHeight = false;
				positionVerLayout->fitByChildren = true;
				positionVerLayout->AddChild(o2UI.CreateLabel("Position"));

				mPosition = o2UI.CreateEditBox("singleline");
				mPosition->name = "position";
				mPosition->SetFilterFloat();
				positionVerLayout->AddChild(mPosition);
				horLayout->AddChild(positionVerLayout);
			}

			{
				auto valueVerLayout = o2UI.CreateVerLayout();
				valueVerLayout->expandHeight = false;
				valueVerLayout->fitByChildren = true;
				valueVerLayout->AddChild(o2UI.CreateLabel("Value"));

				mValue = o2UI.CreateEditBox("singleline");
				mValue->SetFilterFloat();
				mValue->name = "value";
				valueVerLayout->AddChild(mValue);
				horLayout->AddChild(valueVerLayout);
			}

			verLayout->AddChild(horLayout);
		}

		// left support
		{
			auto label = o2UI.CreateLabel("Left support:");
			auto horLayout = o2UI.CreateHorLayout();
			horLayout->spacing = 10;
			horLayout->expandHeight = false;
			horLayout->fitByChildren = true;

			{
				auto positionVerLayout = o2UI.CreateVerLayout();
				positionVerLayout->expandHeight = false;
				positionVerLayout->fitByChildren = true;
				positionVerLayout->AddChild(o2UI.CreateLabel("Position"));

				mLeftSupportPosition = o2UI.CreateEditBox("singleline");
				mLeftSupportPosition->name = "leftSupportPosition";
				mLeftSupportPosition->SetFilterFloat();
				positionVerLayout->AddChild(mLeftSupportPosition);
				horLayout->AddChild(positionVerLayout);
			}

			{
				auto valueVerLayout = o2UI.CreateVerLayout();
				valueVerLayout->expandHeight = false;
				valueVerLayout->fitByChildren = true;
				valueVerLayout->AddChild(o2UI.CreateLabel("Value"));

				mLeftSupportValue = o2UI.CreateEditBox("singleline");
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
			auto label = o2UI.CreateLabel("Right support:");
			auto horLayout = o2UI.CreateHorLayout();
			horLayout->spacing = 10;
			horLayout->expandHeight = false;
			horLayout->fitByChildren = true;

			{
				auto positionVerLayout = o2UI.CreateVerLayout();
				positionVerLayout->expandHeight = false;
				positionVerLayout->fitByChildren = true;
				positionVerLayout->AddChild(o2UI.CreateLabel("Position"));

				mRightSupportPosition = o2UI.CreateEditBox("singleline");
				mRightSupportPosition->name = "rightSupportPosition";
				mRightSupportPosition->SetFilterFloat();
				positionVerLayout->AddChild(mRightSupportPosition);
				horLayout->AddChild(positionVerLayout);
			}

			{
				auto valueVerLayout = o2UI.CreateVerLayout();
				valueVerLayout->expandHeight = false;
				valueVerLayout->fitByChildren = true;
				valueVerLayout->AddChild(o2UI.CreateLabel("Value"));

				mRightSupportValue = o2UI.CreateEditBox("singleline");
				mRightSupportValue->name = "rightSupportValue";
				mRightSupportValue->SetFilterFloat();
				valueVerLayout->AddChild(mRightSupportValue);
				horLayout->AddChild(valueVerLayout);
			}

			verLayout->AddChild(label);
			verLayout->AddChild(horLayout);
		}

		{
			auto horLayout = o2UI.CreateHorLayout();
			horLayout->spacing = 10;
			horLayout->expandHeight = false;
			horLayout->fitByChildren = true;

			auto okButton = o2UI.CreateButton("Ok", THIS_FUNC(OnOkPressed));
			horLayout->AddChild(okButton);

			auto cancelButton = o2UI.CreateButton("Cancel", [&]() { Hide(); });
			horLayout->AddChild(cancelButton);

			verLayout->AddChild(horLayout);
		}

		AddChild(verLayout);
		layout->size = Vec2F(200, 50);
	}

	void KeyEditWindow::FindControls()
	{
		mPosition = FindChildByTypeAndName<EditBox>("position");
		mValue = FindChildByTypeAndName<EditBox>("value");
		mLeftSupportPosition = FindChildByTypeAndName<EditBox>("leftSupportPosition");
		mLeftSupportValue = FindChildByTypeAndName<EditBox>("leftSupportValue");
		mRightSupportPosition = FindChildByTypeAndName<EditBox>("rightSupportPosition");
		mRightSupportValue = FindChildByTypeAndName<EditBox>("rightSupportValue");
	}

	void KeyEditWindow::OnOkPressed()
	{
		Curve::Key key;
		key.position = (float)mPosition->GetText();
		key.value = (float)mValue->GetText();
		key.leftSupportPosition = (float)mLeftSupportPosition->GetText();
		key.leftSupportValue = (float)mLeftSupportValue->GetText();
		key.rightSupportPosition = (float)mRightSupportPosition->GetText();
		key.rightSupportValue = (float)mRightSupportValue->GetText();

		mOnClosed(key);
		Hide();
	}

}

DECLARE_CLASS(Editor::KeyEditWindow);
