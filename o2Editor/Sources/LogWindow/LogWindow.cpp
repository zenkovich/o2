#include "stdafx.h"
#include "LogWindow.h"

#include "Render/Sprite.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/List.h"
#include "Scene/UI/Widgets/LongList.h"
#include "Scene/UI/Widgets/Toggle.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Utils/System/Time/Time.h"

namespace Editor
{
	void LogWindow::Update(float dt)
	{
		if (o2Input.IsKeyDown('H'))
			o2Debug.Log("Regular message " + (String)o2Time.GetLocalTime());

		if (o2Input.IsKeyDown('J'))
			o2Debug.LogWarning("Warning message " + (String)o2Time.GetLocalTime());

		if (o2Input.IsKeyDown('K'))
			o2Debug.LogError("Error message " + (String)o2Time.GetLocalTime());
	}

	LogWindow::LogWindow():
		mRegularMessagesEnabled(true), mWarningMessagesEnabled(true), mErrorMessagesEnabled(true), mRegularMessagesCount(0),
		mWarningMessagesCount(0), mErrorMessagesCount(0)
	{
		InitializeWindow();
		BindStream(o2Debug.GetLog());
	}

	LogWindow::~LogWindow()
	{}

	void LogWindow::InitializeWindow()
	{
		mWindow->caption = "Log";
		mWindow->name = "log window";
		mWindow->SetIcon(mnew Sprite("ui/UI_list_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 1)));
		mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 18));

		mList = o2UI.CreateWidget<LongList>();
		*mList->layout = WidgetLayout::BothStretch(0, 18, 0, -1);
		mList->SetViewLayout(Layout::BothStretch());
		mList->getItemsCountFunc = THIS_FUNC(GetVisibleMessagesCount);
		mList->getItemsRangeFunc = THIS_FUNC(GetVisibleMessagesRange);
		mList->setupItemFunc = THIS_FUNC(SetupListMessage);

		Widget* listItemSample = mnew Widget();
		listItemSample->layout->minHeight = 25;
		listItemSample->AddLayer("back", mnew Sprite(Color4(212, 216, 224, 255)));
		listItemSample->AddLayer("warning", mnew Sprite(Color4(239, 196, 25, 255)),
								 Layout::VerStretch(HorAlign::Left, 0, 0, 10, 0));
		listItemSample->AddLayer("error", mnew Sprite(Color4(211, 87, 40, 255)),
								 Layout::VerStretch(HorAlign::Left, 0, 0, 10, 0));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "text";
		captionText->horAlign = HorAlign::Left;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		listItemSample->AddLayer("caption", captionText, Layout::BothStretch(15, 0, 0, 0));

		mList->SetItemSample(listItemSample);

		mWindow->AddChild(mList);

		Widget* downPanel = mnew Widget();
		downPanel->AddLayer("back", mnew Sprite("ui/UI2_small_panel_down_back.png"),
							Layout::BothStretch(-4, -5, -4, -5));
		*downPanel->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 20, 0);
		mWindow->AddChild(downPanel);

		auto clearBtn = o2UI.CreateWidget<Button>("down panel trash");
		*clearBtn->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(0, 0));
		clearBtn->onClick = [&]() { OnClearPressed(); };
		downPanel->AddChild(clearBtn);

		auto messagesToggle = o2UI.CreateWidget<UIToggle>("log messages");
		*messagesToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(60, 20), Vec2F(20, 0));
		mMessagesCountLabel = messagesToggle->GetLayerDrawable<Text>("caption");
		mMessagesCountLabel->text = "0";
		messagesToggle->value = true;
		messagesToggle->onToggle = [&](bool value) { OnRegularMessagesToggled(value); };
		downPanel->AddChild(messagesToggle);

		auto warningsToggle = o2UI.CreateWidget<UIToggle>("log warnings");
		*warningsToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(60, 20), Vec2F(80, 0));
		mWarningsCountLabel = warningsToggle->GetLayerDrawable<Text>("caption");
		mWarningsCountLabel->text = "0";
		warningsToggle->value = true;
		warningsToggle->onToggle = [&](bool value) { OnWarningMessagesToggled(value); };
		downPanel->AddChild(warningsToggle);

		auto errorsToggle = o2UI.CreateWidget<UIToggle>("log errors");
		*errorsToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(60, 20), Vec2F(140, 0));
		mErrorsCountLabel = errorsToggle->GetLayerDrawable<Text>("caption");
		mErrorsCountLabel->text = "0";
		errorsToggle->value = true;
		errorsToggle->onToggle = [&](bool value) { OnErrorMessagesToggled(value); };
		downPanel->AddChild(errorsToggle);

		mLastMessageView = listItemSample->CloneAs<Widget>();
		*mLastMessageView->layout = WidgetLayout::BothStretch(200, 1, 0, 1);
		downPanel->AddChild(mLastMessageView);
		mLastMessageView->Hide(true);
	}

	void LogWindow::OnClearPressed()
	{
		mAllMessages.Clear();
		mVisibleMessages.Clear();
		mList->OnItemsUpdated();

		mRegularMessagesCount = 0;
		mWarningMessagesCount = 0;
		mErrorMessagesCount = 0;

		mMessagesCountLabel->text = "0";
		mWarningsCountLabel->text = "0";
		mErrorsCountLabel->text = "0";

		mLastMessageView->Hide(true);
	}

	void LogWindow::OnRegularMessagesToggled(bool value)
	{
		mRegularMessagesEnabled = value;
		UpdateVisibleMessages();
	}

	void LogWindow::OnWarningMessagesToggled(bool value)
	{
		mWarningMessagesEnabled = value;
		UpdateVisibleMessages();
	}

	void LogWindow::OnErrorMessagesToggled(bool value)
	{
		mErrorMessagesEnabled = value;
		UpdateVisibleMessages();
	}

	void LogWindow::UpdateVisibleMessages()
	{
		mVisibleMessages.Clear();

		for (auto& msg : mAllMessages)
		{
			msg.idx = mVisibleMessages.Count();

			if (msg.type == LogMessage::Type::Regular && mRegularMessagesEnabled)
				mVisibleMessages.Add(msg);

			if (msg.type == LogMessage::Type::Warning && mWarningMessagesEnabled)
				mVisibleMessages.Add(msg);

			if (msg.type == LogMessage::Type::Error && mErrorMessagesEnabled)
				mVisibleMessages.Add(msg);
		}

		mList->OnItemsUpdated(true);
	}

	int LogWindow::GetVisibleMessagesCount()
	{
		return mVisibleMessages.Count();
	}

	Vector<UnknownType*> LogWindow::GetVisibleMessagesRange(int min, int max)
	{
		Vector<UnknownType*> res;
		for (int i = min; i < max; i++)
		{
			if (i == mVisibleMessages.Count())
				break;

			res.Add((UnknownType*)(void*)&mVisibleMessages[i]);
		}

		return res;
	}

	void LogWindow::SetupListMessage(Widget* item, UnknownType* object)
	{
		LogMessage* message = (LogMessage*)(void*)object;

		item->layer["warning"]->GetDrawable()->enabled = message->type == LogMessage::Type::Warning;
		item->layer["error"]->GetDrawable()->enabled = message->type == LogMessage::Type::Error;
		item->layer["back"]->GetDrawable()->enabled = message->idx % 2 == 1;

		Text* text = item->GetLayerDrawable<Text>("caption");
		text->text = message->message.SubStr(0, message->message.Find("\n"));
	}

	void LogWindow::OutStrEx(const WString& str)
	{
		bool isScrollDown = Math::Equals(mList->GetScroll().y, mList->GetScrollRange().bottom, 5.0f);

		LogMessage msg;
		msg.message = str;
		msg.type = LogMessage::Type::Regular;
		msg.idx = mAllMessages.Count();

		mAllMessages.Add(msg);
		if (mRegularMessagesEnabled)
		{
			msg.idx = mVisibleMessages.Count();
			mVisibleMessages.Add(msg);
		}

		mList->OnItemsUpdated();

		if (isScrollDown)
			mList->SetScrollForcible(Vec2F(0, mList->GetScrollRange().top));

		mRegularMessagesCount++;
		mMessagesCountLabel->text = (String)mRegularMessagesCount;

		UpdateLastMessageView();
	}

	void LogWindow::OutErrorEx(const WString& str)
	{
		bool isScrollDown = Math::Equals(mList->GetScroll().y, mList->GetScrollRange().bottom, 5.0f);

		LogMessage msg;
		msg.message = str;
		msg.type = LogMessage::Type::Error;
		msg.idx = mAllMessages.Count();

		mAllMessages.Add(msg);
		if (mErrorMessagesEnabled)
		{
			msg.idx = mVisibleMessages.Count();
			mVisibleMessages.Add(msg);
		}

		mList->OnItemsUpdated();

		if (isScrollDown)
			mList->SetScrollForcible(Vec2F(0, mList->GetScrollRange().top));

		mErrorMessagesCount++;
		mErrorsCountLabel->text = (String)mErrorMessagesCount;

		UpdateLastMessageView();
	}

	void LogWindow::OutWarningEx(const WString& str)
	{
		bool isScrollDown = Math::Equals(mList->GetScroll().y, mList->GetScrollRange().bottom, 5.0f);

		LogMessage msg;
		msg.message = str;
		msg.type = LogMessage::Type::Warning;
		msg.idx = mAllMessages.Count();

		mAllMessages.Add(msg);
		if (mWarningMessagesEnabled)
		{
			msg.idx = mVisibleMessages.Count();
			mVisibleMessages.Add(msg);
		}

		mList->OnItemsUpdated();

		if (isScrollDown)
			mList->SetScrollForcible(Vec2F(0, mList->GetScrollRange().top));

		mWarningMessagesCount++;
		mWarningsCountLabel->text = (String)mWarningMessagesCount;

		UpdateLastMessageView();
	}

	void LogWindow::UpdateLastMessageView()
	{
		if (mAllMessages.Count() > 0)
		{
			mLastMessageView->Show(true);
			SetupListMessage(mLastMessageView, (UnknownType*)(void*)&mAllMessages.Last());
		}
		else mLastMessageView->Hide(true);
	}

	bool LogWindow::LogMessage::operator==(const LogMessage& other) const
	{
		return type == other.type && message == other.message;
	}

}

DECLARE_CLASS(Editor::LogWindow);

ENUM_META_(Editor::LogWindow::LogMessage::Type, Type)
{
	ENUM_ENTRY(Error);
	ENUM_ENTRY(Regular);
	ENUM_ENTRY(Warning);
}
END_ENUM_META;
