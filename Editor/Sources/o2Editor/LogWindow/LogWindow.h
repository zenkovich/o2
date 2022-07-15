#pragma once

#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

using namespace o2;

namespace o2
{
	class Label;
	class LongList;
	class Text;
}

namespace Editor
{
	class LogWindow: public IEditorWindow, public LogStream
	{
		IOBJECT(LogWindow);

	public:
		class LogMessage
		{
		public:
			enum class Type { Regular, Warning, Error };

			Type   type;
			String message;
			int    idx;

			bool operator==(const LogMessage& other) const;
		};

		// Updates window logic
		void Update(float dt) override;

	protected:
		LongList* mList = nullptr;
		Widget*   mLastMessageView = nullptr;
		Text*     mMessagesCountLabel = nullptr;
		Text*     mWarningsCountLabel = nullptr;
		Text*     mErrorsCountLabel = nullptr;

		Vector<LogMessage> mAllMessages;
		Vector<LogMessage> mVisibleMessages;

		bool mRegularMessagesEnabled;
		bool mWarningMessagesEnabled;
		bool mErrorMessagesEnabled;

		int mRegularMessagesCount;
		int mWarningMessagesCount;
		int mErrorMessagesCount;

	public:
		// Default constructor
		LogWindow();

		// Destructor
		~LogWindow();

	protected:
		// Initializes window
		void InitializeWindow();

		// It is called when clear button pressed
		void OnClearPressed();

		// It is called when regular messages toggled
		void OnRegularMessagesToggled(bool value);

		// It is called when warning messages toggled
		void OnWarningMessagesToggled(bool value);

		// It is called when error messages toggled
		void OnErrorMessagesToggled(bool value);

		// Updates visible messages
		void UpdateVisibleMessages();

		// Returns visible items count
		int GetVisibleMessagesCount();

		// Returns array of visible messages in range from min to max
		Vector<void*> GetVisibleMessagesRange(int min, int max);

		// Sets list item by message
		void SetupListMessage(Widget* item, void* object);

		// Outs string to stream
		void OutStrEx(const WString& str) override;

		// Outs error to stream
		void OutErrorEx(const WString& str) override;

		// Outs warning to stream
		void OutWarningEx(const WString& str) override;

		// Updates last message view
		void UpdateLastMessageView();
	};
}

PRE_ENUM_META(Editor::LogWindow::LogMessage::Type);

CLASS_BASES_META(Editor::LogWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
	BASE_CLASS(o2::LogStream);
}
END_META;
CLASS_FIELDS_META(Editor::LogWindow)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mList);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLastMessageView);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mMessagesCountLabel);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mWarningsCountLabel);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mErrorsCountLabel);
	FIELD().PROTECTED().NAME(mAllMessages);
	FIELD().PROTECTED().NAME(mVisibleMessages);
	FIELD().PROTECTED().NAME(mRegularMessagesEnabled);
	FIELD().PROTECTED().NAME(mWarningMessagesEnabled);
	FIELD().PROTECTED().NAME(mErrorMessagesEnabled);
	FIELD().PROTECTED().NAME(mRegularMessagesCount);
	FIELD().PROTECTED().NAME(mWarningMessagesCount);
	FIELD().PROTECTED().NAME(mErrorMessagesCount);
}
END_META;
CLASS_METHODS_META(Editor::LogWindow)
{

	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindow);
	FUNCTION().PROTECTED().SIGNATURE(void, OnClearPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRegularMessagesToggled, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnWarningMessagesToggled, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnErrorMessagesToggled, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateVisibleMessages);
	FUNCTION().PROTECTED().SIGNATURE(int, GetVisibleMessagesCount);
	FUNCTION().PROTECTED().SIGNATURE(Vector<void*>, GetVisibleMessagesRange, int, int);
	FUNCTION().PROTECTED().SIGNATURE(void, SetupListMessage, Widget*, void*);
	FUNCTION().PROTECTED().SIGNATURE(void, OutStrEx, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, OutErrorEx, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, OutWarningEx, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateLastMessageView);
}
END_META;
