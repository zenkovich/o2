#include "UITestScreen.h"

#include "Assets/ImageAsset.h"
#include "BasicUIStyle.h"
#include "Render/Render.h"
#include "TestApplication.h"
#include "UI/Button.h"
#include "UI/ContextMenu.h"
#include "UI/DropDown.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/HorizontalProgress.h"
#include "UI/Label.h"
#include "UI/List.h"
#include "UI/Toggle.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/VerticalProgress.h"
#include "UI/WidgetLayout.h"
#include "UI/Window.h"

UITestScreen::UITestScreen(TestApplication* application):
	ITestScreen(application)
{}

UITestScreen::~UITestScreen()
{}

#undef CreateWindow

void UITestScreen::Load()
{
	Actor::SetDefaultCreationMode(ActorCreateMode::NotInScene);

	if (GetEnginePlatform() == Platform::Windows)
	{
		BasicUIStyleBuilder uiBuilder;
		uiBuilder.RebuildBasicUIManager();
	}
	else o2UI.LoadStyle("basic_ui_style.xml");

	Actor::SetDefaultCreationMode(ActorCreateMode::InScene);

	mBackground.LoadFromImage("ui/UI_Background.png");
	mBackground.size = (Vec2I)o2Render.resolution + Vec2I(30, 30);
	o2Application.onResizing += [&]() { mBackground.size = (Vec2I)o2Render.resolution + Vec2I(30, 30); };

	auto window = o2UI.CreateWindow("UI elements samples");
	*window->layout = UIWidgetLayout::Based(BaseCorner::Center, Vec2F(300, 300));
	window->name = "UI elements window";
	//window->SetEnableScrollsHiding(false);

	auto verLayout = o2UI.CreateVerLayout();
	*verLayout->layout = UIWidgetLayout::BothStretch();
	verLayout->baseCorner = BaseCorner::Top;
	verLayout->expandHeight = false;
	verLayout->expandWidth = true;
	verLayout->fitByChildren = true;
	verLayout->border = BorderF(5, 5, 5, 5);
	verLayout->spacing = 10;
	*verLayout->layout = UIWidgetLayout::BothStretch();
	window->AddChild(verLayout);

	//button
	auto buttonContainer = o2UI.CreateVerLayout();
	buttonContainer->layout->minHeight = 50;
	auto button = o2UI.CreateButton("Click me!");
	buttonContainer->AddChild(o2UI.CreateLabel("Button"));
	buttonContainer->AddChild(button);
	verLayout->AddChild(buttonContainer);

	auto buttonContext = o2UI.CreateWidget<UIContextMenu>();
	for (int i = 0; i < 15; i++)
	{
		WString itemName = WString::Format("Item #%i", i + 1);
		buttonContext->AddItem(UIContextMenu::Item(itemName, [=]() { o2Debug.Log("Pressed " + itemName); }));
	}

	buttonContext->AddItem(
		UIContextMenu::Item("Icon item", []() { o2Debug.Log("Pressed icon"); },
		ImageAssetRef("ui/UI_search_regular.png")));

	UIContextMenu::Item itm("Sub items", {
		UIContextMenu::Item("Sub 1", []() { o2Debug.Log("Pressed sub 1"); }, ImageAssetRef()),
		UIContextMenu::Item("Sub 2",{ UIContextMenu::Item("Sub 1", []() { o2Debug.Log("Pressed sub 1"); }, ImageAssetRef()),
							UIContextMenu::Item("Sub 2", []() { o2Debug.Log("Pressed sub 2"); }, ImageAssetRef()),
							UIContextMenu::Item("Sub 3", []() { o2Debug.Log("Pressed sub 3"); }, ImageAssetRef()) }),
							UIContextMenu::Item("Sub 3", []() { o2Debug.Log("Pressed sub 3"); }, ImageAssetRef())
	});

	buttonContext->AddItem(itm);
	buttonContext->AddItem(itm);

	button->AddChild(buttonContext);
	button->onClick = [=]() { buttonContext->Show(o2Input.GetCursorPos()); };

	//check box
	auto checkBoxContainer = o2UI.CreateVerLayout();
	checkBoxContainer->layout->minHeight = 50;
	checkBoxContainer->AddChild(o2UI.CreateLabel("Check box"));
	checkBoxContainer->AddChild(o2UI.CreateToggle("Toggle me!"));
	verLayout->AddChild(checkBoxContainer);

	//edit box
	auto editBoxContainer = o2UI.CreateVerLayout();
	editBoxContainer->fitByChildren = true;
	editBoxContainer->AddChild(o2UI.CreateLabel("Edit box"));
	editBoxContainer->AddChild(o2UI.CreateEditBox());
	verLayout->AddChild(editBoxContainer);

	// hor progress
	auto horProgressContainer = o2UI.CreateVerLayout();
	horProgressContainer->fitByChildren = true;
	horProgressContainer->AddChild(o2UI.CreateLabel("Horizontal progress"));
	horProgressContainer->AddChild(o2UI.CreateHorProgress());
	verLayout->AddChild(horProgressContainer);

	// ver progress
	auto verProgressContainer = o2UI.CreateVerLayout();
	verProgressContainer->fitByChildren = true;
	verProgressContainer->expandHeight = false;
	verProgressContainer->AddChild(o2UI.CreateLabel("Vertical progress"));
	auto verProgress = o2UI.CreateVerProgress();
	verProgress->layout->minHeight = 100;
	verProgressContainer->AddChild(verProgress);
	verLayout->AddChild(verProgressContainer);

	// hor scroll bar
	auto horScrollContainer = o2UI.CreateVerLayout();
	horScrollContainer->fitByChildren = true;
	horScrollContainer->AddChild(o2UI.CreateLabel("Horizontal scroll bar"));
	horScrollContainer->AddChild(o2UI.CreateHorScrollBar());
	verLayout->AddChild(horScrollContainer);

	// ver scroll bar
	auto verScrollContainer = o2UI.CreateVerLayout();
	verScrollContainer->fitByChildren = true;
	verScrollContainer->expandHeight = false;
	verScrollContainer->AddChild(o2UI.CreateLabel("Vertical scroll bar"));
	auto verScroll = o2UI.CreateVerScrollBar();
	verScroll->layout->minHeight = 100;
	verScrollContainer->AddChild(verScroll);
	verLayout->AddChild(verScrollContainer);

	// list
	auto listContainer = o2UI.CreateVerLayout();
	listContainer->fitByChildren = true;
	listContainer->expandHeight = false;
	listContainer->AddChild(o2UI.CreateLabel("List"));

	auto list = o2UI.CreateList();
	list->name = "test list";
	list->layout->minHeight = 100;
	for (int i = 0; i < 10; i++)
		list->AddItem(String::Format("List item #%i", i + 1));

	listContainer->AddChild(list);
	verLayout->AddChild(listContainer);

	// drop down
	auto dropdownContainer = o2UI.CreateVerLayout();
	dropdownContainer->fitByChildren = true;
	dropdownContainer->AddChild(o2UI.CreateLabel("Drop down"));

	auto dropdown = o2UI.CreateDropdown();
	dropdown->layout->pivot = Vec2F();
	for (int i = 0; i < 10; i++)
		dropdown->AddItem(String::Format("Drop down item #%i", i + 1));
	dropdown->SetMaxListSizeInItems(5);

	dropdownContainer->AddChild(dropdown);
	verLayout->AddChild(dropdownContainer);

	// label test window
	auto labelTestWindow = o2UI.CreateWindow("Label test");
	auto testLabel = o2UI.CreateLabel("Label text multi line Label text multi line\nLabel text multi line\nLabel text multi lineLabel text multi line\nLabel text multi line");
	labelTestWindow->AddChild(testLabel);
	*testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0);

	auto labelTestEdit = o2UI.CreateEditBox();
	*labelTestEdit->layout = UIWidgetLayout::BothStretch(0, 100, 0, 0);
	labelTestEdit->layout->anchorTop = 0;
	labelTestEdit->layout->offsetTop = 195;
	labelTestWindow->AddChild(labelTestEdit);
	labelTestEdit->onChanged += [=](const WString& text) { testLabel->text = text; };

	auto testButtonslayout = o2UI.CreateVerLayout();
	auto alignHorTestButtonsLayout = o2UI.CreateHorLayout();
	auto alignVerTestButtonsLayout = o2UI.CreateHorLayout();
	auto overflowHorTestButtonsLayout = o2UI.CreateHorLayout();
	auto overflowVerTestButtonsLayout = o2UI.CreateHorLayout();

	*testButtonslayout->layout = UIWidgetLayout::BothStretch();
	testButtonslayout->layout->anchorTop = 0;
	testButtonslayout->layout->offsetTop = 95;
	testButtonslayout->AddChild(alignHorTestButtonsLayout);
	testButtonslayout->AddChild(alignVerTestButtonsLayout);
	testButtonslayout->AddChild(overflowHorTestButtonsLayout);
	testButtonslayout->AddChild(overflowVerTestButtonsLayout);

	testButtonslayout->spacing = 5;

	alignHorTestButtonsLayout->AddChild(o2UI.CreateButton("|<-", [=]() { testLabel->horAlign = HorAlign::Left; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	alignHorTestButtonsLayout->AddChild(o2UI.CreateButton("->|<-", [=]() { testLabel->horAlign = HorAlign::Middle; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));

	alignHorTestButtonsLayout->AddChild(o2UI.CreateButton("->|", [=]() {
		testLabel->horAlign = HorAlign::Right;
		*testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0);
	}));

	alignHorTestButtonsLayout->AddChild(o2UI.CreateButton("---", [=]() { testLabel->horAlign = HorAlign::Both; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	alignHorTestButtonsLayout->spacing = 5;

	alignVerTestButtonsLayout->AddChild(o2UI.CreateButton("/\\", [=]() {
		testLabel->verAlign = VerAlign::Top;
		*testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0);
	}));

	alignVerTestButtonsLayout->AddChild(o2UI.CreateButton("---", [=]() { testLabel->verAlign = VerAlign::Middle; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	alignVerTestButtonsLayout->AddChild(o2UI.CreateButton("\\/", [=]() { testLabel->verAlign = VerAlign::Bottom; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	alignVerTestButtonsLayout->AddChild(o2UI.CreateButton("-\n-\n-", [=]() { testLabel->verAlign = VerAlign::Both; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	alignVerTestButtonsLayout->spacing = 5;

	overflowHorTestButtonsLayout->AddChild(o2UI.CreateButton("None", [=]() { testLabel->horOverflow = UILabel::HorOverflow::None; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	overflowHorTestButtonsLayout->AddChild(o2UI.CreateButton("Cut", [=]() { testLabel->horOverflow = UILabel::HorOverflow::Cut; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	overflowHorTestButtonsLayout->AddChild(o2UI.CreateButton("Dots", [=]() { testLabel->horOverflow = UILabel::HorOverflow::Dots; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	overflowHorTestButtonsLayout->AddChild(o2UI.CreateButton("Expand", [=]() { testLabel->horOverflow = UILabel::HorOverflow::Expand; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	overflowHorTestButtonsLayout->AddChild(o2UI.CreateButton("Wrap", [=]() { testLabel->horOverflow = UILabel::HorOverflow::Wrap; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	overflowHorTestButtonsLayout->spacing = 5;

	overflowVerTestButtonsLayout->AddChild(o2UI.CreateButton("None", [=]() { testLabel->verOverflow = UILabel::VerOverflow::None; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	overflowVerTestButtonsLayout->AddChild(o2UI.CreateButton("Cut", [=]() { testLabel->verOverflow = UILabel::VerOverflow::Cut; *testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0); }));
	overflowVerTestButtonsLayout->AddChild(o2UI.CreateButton("Expand", [=]() {
		testLabel->verOverflow = UILabel::VerOverflow::Expand;
		*testLabel->layout = UIWidgetLayout::BothStretch(0, 200, 0, 0);
	}));
	overflowVerTestButtonsLayout->spacing = 5;

	labelTestWindow->AddChild(testButtonslayout);
	labelTestWindow->layout->size = Vec2F(300, 300);

//	auto treeWnd = o2UI.CreateWindow("Tree");
//	treeWnd->layout->size = Vec2F(300, 300);
}

void UITestScreen::Unload()
{
	
}

void UITestScreen::Update(float dt)
{
	if (o2Input.IsKeyPressed('D'))
		o2Debug.Log("debug");

	if (o2Input.IsKeyPressed(VK_ESCAPE))
		mApplication->GoToScreen("MainTestScreen");
}

void UITestScreen::Draw()
{
	mBackground.Draw();

    o2Render.DrawCross(o2Input.GetCursorPos(), 50);
    o2Debug.DrawText(o2Application.GetContentSize().InvertedY()/-2, "FPS: " + String(o2Time.GetFPS()) +
            "\nCursor: " + (o2Input.IsCursorDown() ? (o2Input.IsCursorPressed() ? "pressed" : "down") : "up") +
            "\nCursors: " + String(o2Input.GetCursors().Count()));
}

String UITestScreen::GetId() const
{
	return "UITestScreen";
}
