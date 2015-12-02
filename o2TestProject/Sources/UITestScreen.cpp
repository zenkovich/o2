#include "UITestScreen.h"

#include "Render/Render.h"
#include "TestApplication.h"
#include "UIStyleBuilding.h"

UITestScreen::UITestScreen(Ptr<TestApplication> application):
	ITestScreen(application)
{}

UITestScreen::~UITestScreen()
{}

void UITestScreen::Load()
{
	RebuildUIStyle();

	mBackground.LoadFromImage("ui/UI_Background.png");
	mBackground.size = (Vec2I)o2Render.resolution + Vec2I(30, 30);
	o2Application.onResizingEvent += [&]() { mBackground.size = (Vec2I)o2Render.resolution + Vec2I(30, 30); };

	auto window = o2UI.CreateWidget<UIWindow>();
	o2UI.AddWidget(window);
	window->layout.size = Vec2F(300, 300);

	auto verLayout = o2UI.CreateVerLayout();
	verLayout->baseCorner = BaseCorner::Top;
	verLayout->expandHeight = false;
	verLayout->fitByChildren = true;
	verLayout->border = RectF(5, 5, 5, 5);
	verLayout->spacing = 10;
	verLayout->layout = UIWidgetLayout::Both();
	window->AddChild(verLayout);

	//button
	auto buttonContainer = o2UI.CreateVerLayout();
	buttonContainer->layout.height = 50;
	buttonContainer->AddChild(o2UI.CreateLabel("Button"));
	buttonContainer->AddChild(o2UI.CreateButton("Click me!"));
	verLayout->AddChild(buttonContainer);

	//check box
	auto checkBoxContainer = o2UI.CreateVerLayout();
	checkBoxContainer->layout.height = 50;
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
	verProgress->layout.height = 100;
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
	verScroll->layout.height = 100;
	verScrollContainer->AddChild(verScroll);
	verLayout->AddChild(verScrollContainer);

	// list
	auto listContainer = o2UI.CreateVerLayout();
	listContainer->fitByChildren = true;
	listContainer->expandHeight = false;
	listContainer->AddChild(o2UI.CreateLabel("List"));

	auto list = o2UI.CreateList();
	list->layout.height = 100;
	for (int i = 0; i < 10; i++)
		list->AddItem(String::Format("List item #%i", i + 1));

	listContainer->AddChild(list);
	verLayout->AddChild(listContainer);

	// drop down
	auto dropdownContainer = o2UI.CreateVerLayout();
	dropdownContainer->fitByChildren = true;
	dropdownContainer->AddChild(o2UI.CreateLabel("Drop down"));

	auto dropdown = o2UI.CreateDropdown();
	for (int i = 0; i < 10; i++)
		dropdown->AddItem(String::Format("Drop down item #%i", i + 1));
	dropdown->SetMaxListSizeInItems(5);

	dropdownContainer->AddChild(dropdown);
	verLayout->AddChild(dropdownContainer);
}

void UITestScreen::Unload()
{
	o2UI.RemoveAllWidgets();
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
}

String UITestScreen::GetId() const
{
	return "UITestScreen";
}
