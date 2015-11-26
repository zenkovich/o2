#include "WindowsManager.h"

#include "Core/WindowsSystem/IEditorWindow.h"
#include "Core/WindowsSystem/UIDockWindowPlace.h"
#include "Core/WindowsSystem/UIDockableWindow.h"
#include "UI/UIManager.h"

WindowsManager::WindowsManager()
{
	if (mNeedRebuildWndStyle)
		InitializeDockableWndStyle();

	InitializeDock();
	InitializeWindows();
}

WindowsManager::~WindowsManager()
{
	for (auto wnd : mEditorWindows)
		wnd.Release();
}

void WindowsManager::InitializeWindows()
{
	auto windowTypes = IEditorWindow::type->DerivedTypes();

	for (auto type : windowTypes)
	{
		for (int i = 0; i < 5; i++)
		{
			auto newWindow = type->CreateSample();
			mEditorWindows.Add(newWindow);
		}
	}
}

void WindowsManager::InitializeDock()
{
	auto mainDockPlace = mnew UIDockWindowPlace();
	mainDockPlace->name = "main dock";
	mainDockPlace->layout = UIWidgetLayout::Both();
	mainDockPlace->SetResizibleSide(Side::None);
	o2UI.AddWidget(mainDockPlace);
}

void WindowsManager::Update(float dt)
{
	for (auto wnd : mEditorWindows)
		wnd->Update(dt);
}

void ProcHierarchy(String& hierarchy, Ptr<UIWidget> widget, int level)
{
	for (int i = 0; i < level; i++)
		hierarchy += ' ';

	hierarchy += widget->GetName();

	if (widget->GetTypeId() == UIDockWindowPlace::type->ID())
	{
		hierarchy += ": ";
		hierarchy += (String)(bool)widget.Cast<UIDockWindowPlace>()->interactable;
		RectF rt = widget->layout.GetAbsoluteRect();
		hierarchy += (String)rt.left + " " + (String)rt.bottom + " " + (String)rt.right + " " + (String)rt.top;
	}

	hierarchy += '\n';

	for (auto child : widget->GetChilds())
		ProcHierarchy(hierarchy, child, level + 1);
}

#undef DrawText

void WindowsManager::Draw()
{
	for (auto wnd : mEditorWindows)
		wnd->Draw();

	String hierarchy;
	ProcHierarchy(hierarchy, o2UI.GetScreenWidget(), 0);
	o2Debug.DrawText((Vec2F)(o2Render.GetResolution().InvertedX())*0.5f, hierarchy);
}

void WindowsManager::AddWindow(Ptr<IEditorWindow> window)
{
	mEditorWindows.Add(window);
}

void WindowsManager::InitializeDockableWndStyle()
{
	Ptr<UIDockableWindow> sample = mnew UIDockableWindow();

	sample->layout.minSize = Vec2F(100, 50);

	*sample->GetDockingFrameSample() = Sprite("ui/UI_Window_place.png");

	auto regularBackLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_window_frame_regular.png"),
											 Layout::Both(-12, -12, -12, -12));

	auto dockedBackLayer = sample->AddLayer("dockedBack", mnew Sprite("ui/UI_window_frame_docked.png"),
											 Layout::Both(-12, -13, -14, -13));

	auto iconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI_o2_sign.png"),
									  Layout(Vec2F(0.0f, 1.0f), Vec2F(0.0f, 1.0f), Vec2F(3, -16), Vec2F(17, -1)));

	Ptr<Text> captionText = mnew Text("arial.ttf");
	captionText->text = "Window";
	captionText->horAlign = Text::HorAlign::Left;
	captionText->verAlign = Text::VerAlign::Middle;
	auto textLayer = sample->AddLayer("caption", captionText,
									  Layout(Vec2F(0.0f, 1.0f), Vec2F(1.0f, 1.0f), Vec2F(19, -15), Vec2F(-28, 1)));

	sample->SetClippingLayout(Layout::Both(0, 1, -1, 15));
	sample->SetViewLayout(Layout::Both(5, 5, 5, 20));
	sample->SetEnableScrollsHiding(true);

	Ptr<UIHorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
	horScrollBar->layout.anchorMin = Vec2F(0, 0);
	horScrollBar->layout.anchorMax = Vec2F(1, 0);
	horScrollBar->layout.offsetMin = Vec2F(5, 0);
	horScrollBar->layout.offsetMax = Vec2F(-15, 15);
	sample->SetHorizontalScrollBar(horScrollBar);

	Ptr<UIVerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
	verScrollBar->layout.anchorMin = Vec2F(1, 0);
	verScrollBar->layout.anchorMax = Vec2F(1, 1);
	verScrollBar->layout.offsetMin = Vec2F(-15, 15);
	verScrollBar->layout.offsetMax = Vec2F(0, -20);
	sample->SetVerticalScrollBar(verScrollBar);

	Animation dockedStateAnim;
	dockedStateAnim.SetTarget(sample);
	*dockedStateAnim.AddAnimationValue<float>(&dockedBackLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);
	*dockedStateAnim.AddAnimationValue<float>(&regularBackLayer->transparency) = AnimatedValue<float>::EaseInOut(1, 0, 0.2f);

	auto dockedState = sample->AddState("docked", dockedStateAnim);

	Animation enableHorScrollAnim;
	enableHorScrollAnim.SetTarget(sample);
	*enableHorScrollAnim.AddAnimationValue<float>(&sample->GetVerticalScrollbar()->layout.offsetBottom) =
		AnimatedValue<float>::EaseInOut(5, 15, 0.2f);

	auto enableHorScrollState = sample->AddState("enableHorBar", enableHorScrollAnim);

	Animation enableVerScrollAnim;
	enableVerScrollAnim.SetTarget(sample);
	*enableVerScrollAnim.AddAnimationValue<float>(&sample->GetHorizontalScrollbar()->layout.offsetRight) =
		AnimatedValue<float>::EaseInOut(-5, -15, 0.2f);

	auto enableVerScrollState = sample->AddState("enableVerBar", enableVerScrollAnim);

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);

	Ptr<UIButton> closeBtn = o2UI.CreateWidget<UIButton>("close");
	closeBtn->name = "closeButton";
	closeBtn->layout.anchorMin = Vec2F(1, 1);
	closeBtn->layout.anchorMax = Vec2F(1, 1);
	closeBtn->layout.offsetMin = Vec2F(-21, -17);
	closeBtn->layout.offsetMax = Vec2F(-1, 1);
	sample->AddWindowElement(closeBtn);

	Ptr<UIButton> optionsBtn = o2UI.CreateWidget<UIButton>("arrow");
	optionsBtn->name = "optionsButton";
	optionsBtn->layout.anchorMin = Vec2F(1, 1);
	optionsBtn->layout.anchorMax = Vec2F(1, 1);
	optionsBtn->layout.offsetMin = Vec2F(-36, -17);
	optionsBtn->layout.offsetMax = Vec2F(-18, 1);
	sample->AddWindowElement(optionsBtn);

	sample->SetDragAreaLayouts(Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(5, -15), Vec2F(-5, -2)),    // head
							   Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(5, -2), Vec2F(-5, 5)),      // top
							   Layout(Vec2F(0, 0), Vec2F(1, 0), Vec2F(5, -5), Vec2F(-5, 5)),      // bottom
							   Layout(Vec2F(0, 0), Vec2F(0, 1), Vec2F(-5, 5), Vec2F(5, -5)),      // left
							   Layout(Vec2F(1, 0), Vec2F(1, 1), Vec2F(-5, 5), Vec2F(5, -5)),      // right
							   Layout(Vec2F(0, 1), Vec2F(0, 1), Vec2F(-5, -5), Vec2F(5, 5)),      // left top
							   Layout(Vec2F(1, 1), Vec2F(1, 1), Vec2F(-5, -5), Vec2F(5, 5)),      // right top
							   Layout(Vec2F(0, 0), Vec2F(0, 0), Vec2F(-5, -5), Vec2F(5, 5)),      // left bottom 
							   Layout(Vec2F(1, 0), Vec2F(1, 0), Vec2F(-5, -5), Vec2F(5, 5)));     // right bottom

	o2UI.RemoveWidgetStyle<UIDockableWindow>("standard");
	o2UI.AddWidgetStyle(sample, "standard");
	o2UI.SaveStyle("ui_style.xml");
}

