#include "o2Editor/stdafx.h"
#include "LogWindow.h"

#include "o2/Render/Sprite.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/List.h"
#include "o2/Scene/UI/Widgets/LongList.h"
#include "o2/Scene/UI/Widgets/Toggle.h"
#include "o2/Utils/System/Time/Time.h"

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
		//BindStream(o2Debug.GetLog());
	}

	LogWindow::~LogWindow()
	{}

	void LogWindow::InitializeWindow()
	{
		mWindow->caption = "Log";
		mWindow->name = "log window";
		mWindow->SetIcon(mmake<o2::Render::Sprite>("ui/UI4_log_wnd_icon.png"));
		mWindow->SetIconLayout(o2::Scene::UI::Layout::Based(o2::Scene::UI::BaseCorner::LeftTop, o2::Vec2F(20, 20), o2::Vec2F(-1, 1)));
		mWindow->SetViewLayout(o2::Scene::UI::Layout::BothStretch(-2, 0, 0, 18));
		mWindow->SetClippingLayout(o2::Scene::UI::Layout::BothStretch(-1, 0, 0, 18));

		mList = o2UI.CreateWidget<LongList>();
		*mList->layout = o2::Scene::UI::WidgetLayout::BothStretch(0, 18, 0, -1);
		mList->SetViewLayout(o2::Scene::UI::Layout::BothStretch());
		mList->getItemsCountFunc = THIS_FUNC(GetVisibleMessagesCount);
		mList->getItemsRangeFunc = THIS_FUNC(GetVisibleMessagesRange);
		mList->setupItemFunc = THIS_FUNC(SetupListMessage);

		o2::Scene::UI::Widget* listItemSample = mmake<o2::Scene::UI::Widget>();
		listItemSample->layout->minHeight = 25;
		listItemSample->AddLayer("back", mmake<o2::Render::Sprite>(o2::Color4(0, 0, 0, 255)));
		listItemSample->AddLayer("warning", mmake<o2::Render::Sprite>(o2::Color4(226, 198, 83, 255)),
								 o2::Scene::UI::Layout::VerStretch(o2::Scene::UI::HorAlign::Left, 0, 0, 10, 0));
		listItemSample->AddLayer("error", mmake<o2::Render::Sprite>(o2::Color4(248, 94, 72, 255)),
								 o2::Scene::UI::Layout::VerStretch(o2::Scene::UI::HorAlign::Left, 0, 0, 10, 0));

		o2::Scene::UI::Text* captionText = mmake<o2::Scene::UI::Text>("stdFont.ttf");
		captionText->text = "text";
		captionText->horAlign = o2::Scene::UI::HorAlign::Left;
		captionText->verAlign = o2::Scene::UI::VerAlign::Middle;
		captionText->dotsEngings = true;
		captionText->color = o2::Color4(96, 125, 139);
		listItemSample->AddLayer("caption", captionText, o2::Scene::UI::Layout::BothStretch(15, 0, 0, 0));

		mList->SetItemSample(listItemSample);

		mWindow->AddChild(mList);

		o2::Scene::UI::Widget* downPanel = mmake<o2::Scene::UI::Widget>();
		downPanel->AddLayer("back", mmake<o2::Render::Sprite>("ui/UI4_small_panel_down_back.png"),
							o2::Scene::UI::Layout::BothStretch(-4, -5, -4, -5));
		*downPanel->layout = o2::Scene::UI::WidgetLayout::HorStretch(o2::Scene::UI::VerAlign::Bottom, 0, 0, 20, 0);
		mWindow->AddChild(downPanel);

		auto clearBtn = o2UI.CreateWidget<Button>("down panel trash");
		*clearBtn->layout = o2::Scene::UI::WidgetLayout::Based(o2::Scene::UI::BaseCorner::Left, o2::Vec2F(20, 20), o2::Vec2F(0, 0));
		clearBtn->onClick = [&]() { OnClearPressed(); };
		downPanel->AddChild(clearBtn);

		auto messagesToggle = o2UI.CreateWidget<Toggle>("log messages");
		*messagesToggle->layout = o2::Scene::UI::WidgetLayout::Based(o2::Scene::UI::BaseCorner::Left, o2::Vec2F(60, 20), o2::Vec2F(20, 0));
		mMessagesCountLabel = messagesToggle->GetLayerDrawable<o2::Scene::UI::Text>("caption");
		mMessagesCountLabel->text = "0";
		messagesToggle->value = true;
		messagesToggle->onToggle = [&](bool value) { OnRegularMessagesToggled(value); };
		downPanel->AddChild(messagesToggle);

		auto warningsToggle = o2UI.CreateWidget<Toggle>("log warnings");
		*warningsToggle->layout = o2::Scene::UI::WidgetLayout::Based(o2::Scene::UI::BaseCorner::Left, o2::Vec2F(60, 20), o2::Vec2F(80, 0));
		mWarningsCountLabel = warningsToggle->GetLayerDrawable<o2::Scene::UI::Text>("caption");
		mWarningsCountLabel->text = "0";
		warningsToggle->value = true;
		warningsToggle->onToggle = [&](bool value) { OnWarningMessagesToggled(value); };
		downPanel->AddChild(warningsToggle);

		auto errorsToggle = o2UI.CreateWidget<Toggle>("log errors");
		*errorsToggle->layout = o2::Scene::UI::WidgetLayout::Based(o2::Scene::UI::BaseCorner::Left, o2::Vec2F(60, 20), o2::Vec2F(140, 0));
		mErrorsCountLabel = errorsToggle->GetLayerDrawable<o2::Scene::UI::Text>("caption");
		mErrorsCountLabel->text = "0";
		errorsToggle->value = true;
		errorsToggle->onToggle = [&](bool value) { OnErrorMessagesToggled(value); };
		downPanel->AddChild(errorsToggle);

		mLastMessageView = listItemSample->CloneAs<o2::Scene::UI::Widget>();
		*mLastMessageView->layout = o2::Scene::UI::WidgetLayout::BothStretch(200, 1, 0, 1);
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

	int LogWindow::GetVisibleMessagesCount()
	{
		UpdateVisibleMessages();
		return mVisibleMessages.Size();
	}

	void LogWindow::GetVisibleMessagesRange(int& start, int& end)
	{
		UpdateVisibleMessages();
		start = 0;
		end = mVisibleMessages.Size();
	}

	void LogWindow::SetupListMessage(o2::Scene::UI::Widget* item, int index)
	{
		const LogMessage& message = mVisibleMessages[index];

		o2::Scene::UI::Widget* backLayer = item->GetLayer("back");
		o2::Scene::UI::Widget* warningLayer = item->GetLayer("warning");
		o2::Scene::UI::Widget* errorLayer = item->GetLayer("error");
		o2::Scene::UI::Text* captionText = item->GetLayerDrawable<o2::Scene::UI::Text>("caption");

		if (message.type == LogMessageType::Regular)
		{
			backLayer->Hide(false);
			warningLayer->Hide(true);
			errorLayer->Hide(true);
		}
		else if (message.type == LogMessageType::Warning)
		{
			backLayer->Hide(false);
			warningLayer->Hide(false);
			errorLayer->Hide(true);
		}
		else if (message.type == LogMessageType::Error)
		{
			backLayer->Hide(false);
			warningLayer->Hide(true);
			errorLayer->Hide(false);
		}

		captionText->text = message.text;
		captionText->color = message.type == LogMessageType::Regular ? o2::Color4(96, 125, 139) : o2::Color4(255, 255, 255);
	}

	void LogWindow::UpdateVisibleMessages()
	{
		mVisibleMessages.Clear();

		for (const LogMessage& message : mAllMessages)
		{
			if ((message.type == LogMessageType::Regular && mRegularMessagesEnabled) ||
				(message.type == LogMessageType::Warning && mWarningMessagesEnabled) ||
				(message.type == LogMessageType::Error && mErrorMessagesEnabled))
			{
				mVisibleMessages.PushBack(message);
			}
		}
	}
}#include <Ref.h>

namespace Editor
{
    class LogWindow
    {
    public:
        void SetRegularMessagesToggled(bool value)
        {
            mRegularMessagesEnabled = value;
            UpdateVisibleMessages();
        }

        void SetWarningMessagesToggled(bool value)
        {
            mWarningMessagesEnabled = value;
            UpdateVisibleMessages();
        }

        void SetErrorMessagesToggled(bool value)
        {
            mErrorMessagesEnabled = value;
            UpdateVisibleMessages();
        }

        void UpdateVisibleMessages()
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

        int GetVisibleMessagesCount()
        {
            return mVisibleMessages.Count();
        }

        Vector<void*> GetVisibleMessagesRange(int min, int max)
        {
            Vector<void*> res;
            for (int i = min; i < max; i++)
            {
                if (i == mVisibleMessages.Count())
                    break;

                res.Add((void*)(size_t)mVisibleMessages[i]);
            }

            return res;
        }

        void SetupListMessage(Widget* item, void* object)
        {
            LogMessage* message = (LogMessage*)(size_t)object;

            item->layer["warning"]->GetDrawable()->enabled = message->type == LogMessage::Type::Warning;
            item->layer["error"]->GetDrawable()->enabled = message->type == LogMessage::Type::Error;
            item->layer["back"]->GetDrawable()->transparency = message->idx % 2 == 1 ? 0.05f : 0.0f;

            Text* text = item->GetLayerDrawable<Text>("caption");
            text->text = message->message.SubStr(0, message->message.Find("\n"));
        }

        void OutStrEx(const WString& str)
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

        void OutErrorEx(const WString& str)
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

        void OutWarningEx(const WString& str)
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

        void UpdateLastMessageView()
        {
            if (mAllMessages.Count() > 0)
            {
                mLastMessageView->Show(true);
                SetupListMessage(mLastMessageView, (void*)(size_t)&mAllMessages.Last());
            }
            else mLastMessageView->Hide(true);
        }

        bool LogMessage::operator==(const LogMessage& other) const
        {
            return type == other.type && message == other.message;
        }

    private:
        Ref<Widget> mList;
        bool mRegularMessagesEnabled;
        bool mWarningMessagesEnabled;
        bool mErrorMessagesEnabled;
        Vector<Ref<LogMessage>> mVisibleMessages;
        int mRegularMessagesCount;
        int mWarningMessagesCount;
        int mErrorMessagesCount;
        Ref<Label> mMessagesCountLabel;
        Ref<Label> mErrorsCountLabel;
        Ref<Label> mWarningsCountLabel;
        Ref<Widget> mLastMessageView;
        Vector<Ref<LogMessage>> mAllMessages;
    };

    // --- META ---

    ENUM_META(Editor::LogWindow::LogMessage::Type)
    {
        ENUM_ENTRY(Error);
        ENUM_ENTRY(Regular);
        ENUM_ENTRY(Warning);
    }
    END_ENUM_META

    DECLARE_CLASS(Editor::LogWindow, Editor__LogWindow);
    // --- END META ---
}